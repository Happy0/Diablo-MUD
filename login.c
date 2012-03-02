#include <stdlib.h>
#include "client.h"
#include "login.h"
#include "buffer.h"
#include "send.h"

enum {
	SHOW_MENU,
	MENU_RESPONSE
};

struct login {
	int state;
	char username[32];
	char password[64];
};

struct login *login_init(void)
{
	struct login *l;

	l = malloc(sizeof(struct login));

	l->state = SHOW_MENU;
	l->username[0] = '\0';
	l->password[0] = '\0';

	return l;
}

int login_callback(struct client *c, struct buffer *b)
{
	unsigned char *buf;
	struct login *l;
	int s;


	buf = buffer_get(b);
	l   = client_get_login(c);
	s   = client_get_socket(c);

	switch(l->state)
	{
		case SHOW_MENU:
			login_display_menu(s);
			l->state = MENU_RESPONSE;
		break;
		case MENU_RESPONSE:

		break;
	}
	
	return 0;
}
