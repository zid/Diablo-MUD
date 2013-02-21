#ifndef _LOGIN_H
#define _LOGIN_H
#include "client.h"
void login_send_banner(struct client *);
void login_send_motd(struct client *);
#endif
