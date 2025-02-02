#pragma once
#include <freeradius-devel/server/base.h>
#include <freeradius-devel/server/module.h>
#include <freeradius-devel/util/debug.h>

#include "acexport.h"

#define SAFE_STR(s) s==NULL?"EMPTY":s

typedef enum {
	INITIAL_STATE = 0,
	NEXT_CODE_REQUIRED_STATE = 100,
	NEW_PIN_REQUIRED_STATE = 200,
	NEW_PIN_USER_CONFIRM_STATE = 201,
	NEW_PIN_AUTH_VALIDATE_STATE = 202,
	NEW_PIN_SYSTEM_ACCEPT_STATE = 203,
	NEW_PIN_SYSTEM_CONFIRM_STATE = 204,
	NEW_PIN_USER_SELECT_STATE = 205,
} SECURID_SESSION_STATE;

/*
 * SECURID_SESSION is used to identify existing securID sessions
 * to continue Next-Token code and New-Pin conversations with a client
 *
 * next = pointer to next
 * state = state attribute from the reply we sent
 * state_len = length of data in the state attribute.
 * src_ipaddr = client which sent us the RADIUS request containing
 *	      this SecurID conversation.
 * timestamp  = timestamp when this handler was last used.
 * trips = number of trips
 * identity = Identity of the user
 * request = RADIUS request data structure
 */

#define SECURID_STATE_LEN 32
typedef struct {
	struct _securid_session_t	*prev, *next;
	fr_rb_node_t			node;
	SDI_HANDLE		 	 sdiHandle;
	SECURID_SESSION_STATE	  	securidSessionState;

	char			  	state[SECURID_STATE_LEN];

	fr_ipaddr_t			src_ipaddr;
	time_t				timestamp;
	unsigned int			session_id;
	uint32_t			trips;

	char				*pin;	     /* previous pin if user entered it during NEW-PIN mode process */
	char				 *identity; /* save user's identity name for future use */

} SECURID_SESSION;


/*
 *      Define a structure for our module configuration.
 *
 *      These variables do not need to be in a structure, but it's
 *      a lot cleaner to do so, and a pointer to the structure can
 *      be used as the instance handle.
 *      sessions = remembered sessions, in a tree for speed.
 *      mutex = ensure only one thread is updating the sessions list
 */
typedef struct {
	pthread_mutex_t	session_mutex;
	fr_rb_tree_t*	session_tree;
	SECURID_SESSION	*session_head, *session_tail;

	unsigned int	 last_session_id;

	/*
	 *	Configuration items.
	 */
	uint32_t	timer_limit;
	uint32_t	max_sessions;
	uint32_t	max_trips_per_session;
} rlm_securid_t;

extern HIDDEN fr_dict_attr_t const *attr_prompt;
extern HIDDEN fr_dict_attr_t const *attr_reply_message;
extern HIDDEN fr_dict_attr_t const *attr_state;
extern HIDDEN fr_dict_attr_t const *attr_user_password;

/* Memory Management */
SECURID_SESSION*     securid_session_alloc(void);
void		     securid_session_free(rlm_securid_t *inst, request_t *request,SECURID_SESSION *session)
		     CC_HINT(nonnull);

void		     securid_sessionlist_free(rlm_securid_t *inst,request_t *request) CC_HINT(nonnull);

int		     securid_sessionlist_add(rlm_securid_t *inst, request_t *request, SECURID_SESSION *session)
		     CC_HINT(nonnull);
SECURID_SESSION	     *securid_sessionlist_find(rlm_securid_t *inst, request_t *request) CC_HINT(nonnull);
