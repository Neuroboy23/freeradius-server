/**
 * $Id$
 * @file rlm_eagent.c
 * @brief EAgent
 */
RCSID("$Id$")

#include <freeradius-devel/radiusd.h>
#include <freeradius-devel/modules.h>

#define EAGENT_LOG_PREFIX "rlm_eagent: "

static rlm_rcode_t CC_HINT(nonnull) mod_authenticate(void *instance, REQUEST *request) {
	INFO(EAGENT_LOG_PREFIX "mod_authenticate");
	return RLM_MODULE_NOOP;
}

static rlm_rcode_t CC_HINT(nonnull) mod_authorize(void *instance, REQUEST *request) {
	INFO(EAGENT_LOG_PREFIX "mod_authorize");
	return RLM_MODULE_NOOP;
}

/*
 *	The module name should be the only globally exported symbol.
 *	That is, everything else should be 'static'.
 *
 *	If the module needs to temporarily modify it's instantiation
 *	data, the type should be changed to RLM_TYPE_THREAD_UNSAFE.
 *	The server will then take care of ensuring that the module
 *	is single-threaded.
 */
extern module_t rlm_eagent;
module_t rlm_eagent = {
	.magic		= RLM_MODULE_INIT,
	.name		= "eagent",
	.type		= RLM_TYPE_THREAD_SAFE,
	.methods = {
		[MOD_AUTHENTICATE]	= mod_authenticate,
		[MOD_AUTHORIZE]		= mod_authorize,
	},
};
