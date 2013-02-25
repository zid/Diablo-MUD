#ifndef _LOGIN_H
#define _LOGIN_H
#include "client.h"
void login_send_banner(client *);
void login_send_motd(client *);
void login_client(client *);
#endif
