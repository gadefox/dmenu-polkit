#include "listener.h"
#include <pthread.h>
#include <stdio.h>
#include "util.h"

G_DEFINE_TYPE(DmenuPolkitListener, dmenu_polkit_listener, POLKIT_AGENT_TYPE_LISTENER)

static void finalize(GObject *obj) {
  G_OBJECT_CLASS(dmenu_polkit_listener_parent_class)->finalize(obj);
}

typedef struct {
  GTask *task;
  GCancellable *cancellable;
  gchar *cookie;
  gchar *action_id;
  PolkitAgentSession *session;
  PolkitIdentity *identity;
} Auth;

static void on_session_completed(PolkitAgentSession *session, gboolean authorized, Auth *auth);
static void auth_free(Auth *auth);

static void on_cancelled(GCancellable *cancellable, Auth *auth) {
  (void)cancellable;

  if (auth->session) {
    polkit_agent_session_cancel(auth->session);
  } else {
    auth_free(auth);
  }
}

static void auth_free(Auth *auth) {
  g_signal_handlers_disconnect_by_func(auth->cancellable, (gpointer)on_cancelled, 0);
  g_object_unref(auth->task);
  g_object_unref(auth->session);
  g_free(auth->cookie);
  g_free(auth->action_id);
  free(auth);
}

static void *do_auth(Auth *auth) {
  char buf[128];

  const char *input = read_input(buf, 128);
  if (input == NULL || strlen(input) == 0) {
    g_cancellable_cancel(auth->cancellable);
  } else {
    polkit_agent_session_response(auth->session, input);
  }

  return NULL;
}

static void new_session(Auth *auth) {
  if (auth->session != NULL) {
    g_object_unref(auth->session);
  }

  auth->session = polkit_agent_session_new(auth->identity, auth->cookie);
  polkit_agent_session_initiate(auth->session);
  g_signal_connect(auth->session, "completed", G_CALLBACK(on_session_completed), auth);
}

static void on_session_completed(PolkitAgentSession *session,
                                 gboolean authorized, Auth *auth) {
  (void)session;

  if (authorized || g_cancellable_is_cancelled(auth->cancellable)) {
    g_task_return_pointer(auth->task, NULL, NULL);
    auth_free(auth);
  } else {
    new_session(auth);
    do_auth(auth);
  }
}

static void
initiate_authentication(PolkitAgentListener *listener, const gchar *action_id,
                        const gchar *message, const gchar *icon_name,
                        PolkitDetails *details, const gchar *cookie,
                        GList *identities, GCancellable *cancellable,
                        GAsyncReadyCallback callback, gpointer user_data) {
  (void)listener;
  (void)message;
  (void)icon_name;
  (void)details;
  (void)identities;

  Auth *auth = calloc(1, sizeof(Auth));
  auth->task = g_task_new(listener, cancellable, callback, user_data);
  auth->cancellable = cancellable;
  auth->cookie = g_strdup(cookie);
  auth->action_id = g_strdup(action_id);
  auth->identity = POLKIT_IDENTITY(identities->data);

  g_signal_connect(cancellable, "cancelled", G_CALLBACK(on_cancelled), auth);

  new_session(auth);
  do_auth(auth);
}

static gboolean initiate_authentication_finish(PolkitAgentListener *listener,
                                               GAsyncResult *res,
                                               GError **error) {
  (void)listener;
  g_debug("initiate_authentication_finish");
  return !g_task_propagate_boolean(G_TASK(res), error);
}

static void dmenu_polkit_listener_class_init(DmenuPolkitListenerClass *klass) {
  GObjectClass *g_object_class = G_OBJECT_CLASS(klass);
  g_object_class->finalize = finalize;

  PolkitAgentListenerClass *pkal_class = POLKIT_AGENT_LISTENER_CLASS(klass);
  pkal_class->initiate_authentication = initiate_authentication;
  pkal_class->initiate_authentication_finish = initiate_authentication_finish;
}

static void dmenu_polkit_listener_init(DmenuPolkitListener *self) {
  (void)self;
}

PolkitAgentListener *dmenu_polkit_listener_new(void) {
  return g_object_new(DMENU_TYPE_POLKIT_LISTENER, NULL);
}
