#include <stdlib.h>
#include "callback.h"
#include "login.h"

struct callback {
	int (*cb)(struct client *);
};

struct callback *callback_init(void)
{
	struct callback *callback;

	callback = malloc(sizeof(struct callback));

	callback->cb = login_callback;

	return callback;
}

void callback_do(struct client *c, struct callback *callback)
{
	int r;
	
	r = callback->cb(c);
	/*
	 * if r change state */
}

