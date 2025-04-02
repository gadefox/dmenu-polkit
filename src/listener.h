#pragma once

#define POLKIT_AGENT_I_KNOW_API_IS_SUBJECT_TO_CHANGE
#include <polkitagent/polkitagent.h>
#include <polkit/polkit.h>

G_BEGIN_DECLS

#define DMENU_TYPE_POLKIT_LISTENER             (dmenu_polkit_listener_get_type())
#define DMENU_POLKIT_LISTENER(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj, DMENU_TYPE_POLKIT_LISTENER, DmenuPolkitListener)))
#define DMENU_IS_POLKIT_LISTENER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), DMENU_TYPE_POLKIT_LISTENER))
#define DMENU_IS_POLKIT_LISTENER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), DMENU_TYPE_POLKIT_LISTENER))
#define DMENU_POLKIT_LISTENER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), DMENU_TYPE_POLKIT_LISTENER, DmenuPolkitListenerClass))


typedef struct _DmenuPolkitListener	      DmenuPolkitListener;
typedef struct _DmenuPolkitListenerClass  DmenuPolkitListenerClass;

struct _DmenuPolkitListener {
  PolkitAgentListener parent;
};

struct _DmenuPolkitListenerClass {
  PolkitAgentListenerClass parent_class;
};

GType dmenu_polkit_listener_get_type(void);
PolkitAgentListener* dmenu_polkit_listener_new(void);

G_END_DECLS
