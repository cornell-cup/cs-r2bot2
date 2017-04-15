#ifndef _R2BOT_R2USERS
#define _R2BOT_R2USERS

#ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _SCL_SECURE_NO_WARNINGS
#	define _SCL_SECURE_NO_WARNINGS
#endif

#include "Global.h"

static int callbackUsers(void *data, int argc, char **argv, char **azColName);

int maintainUsers();

#endif