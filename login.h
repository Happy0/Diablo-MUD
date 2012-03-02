#ifndef LOGIN_H
#define LOGIN_H

#include "client.h"
#include "buffer.h"

typedef struct login login;

struct login *login_init(void);
int login_callback(struct client *c, struct buffer *);

#endif
