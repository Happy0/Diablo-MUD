#ifndef CALLBACK_H
#define CALLBACK_H

#include "client.h"

typedef struct callback callback;

void callback_do(struct client *, struct callback *);
struct callback *callback_init(void);
#endif
