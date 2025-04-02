#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "listener.h"

int main() {
  PolkitAgentListener *listener = dmenu_polkit_listener_new();
  PolkitSubject *session = polkit_unix_session_new_for_process_sync(getpid(), NULL, NULL);
  GError *err = NULL;

  if (!polkit_agent_listener_register(listener, POLKIT_AGENT_REGISTER_FLAGS_NONE, session, NULL, NULL, &err)) {
    fprintf(stderr, "%s\n", err->message);

    g_object_unref(session);
    g_object_unref(listener);

    return 1;
  }

  GMainLoop *loop = g_main_loop_new(NULL, TRUE);
  g_main_loop_run(loop);
  g_main_loop_unref(loop);
  g_object_unref(session);
  g_object_unref(listener);

  return 0;
}
