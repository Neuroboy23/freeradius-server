#ifndef _REQUEST_LIST_H
#define _REQUEST_LIST_H
/*
 * request_list.h	Hide the handling of the REQUEST list from
 *			the main server.
 *
 * Version:	$Id$
 *
 */

extern request_list_t *rl_init(void);
extern void rl_deinit(request_list_t *);
extern void rl_yank(request_list_t *, REQUEST *);
extern void rl_delete(request_list_t *, REQUEST *);
extern void rl_add(request_list_t *, REQUEST *);
extern REQUEST *rl_find(request_list_t *, RADIUS_PACKET *);
extern int rl_add_proxy(REQUEST *request);
extern REQUEST *rl_find_proxy(RADIUS_PACKET *packet);
extern REQUEST *rl_next(request_list_t *, REQUEST *);
extern int rl_num_requests(request_list_t *);

#define RL_WALK_CONTINUE (0)
#define RL_WALK_STOP     (-1)

typedef int (*RL_WALK_FUNC)(request_list_t *, REQUEST *, void *);

extern int rl_walk(request_list_t *, RL_WALK_FUNC walker, void *data);
extern int rl_clean_list(request_list_t *, time_t now);

#endif /* _REQUEST_LIST_H */
