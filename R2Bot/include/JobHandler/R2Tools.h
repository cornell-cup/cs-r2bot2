#ifndef _R2BOT_R2TOOLS
#define _R2BOT_R2TOOLS

#ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _SCL_SECURE_NO_WARNINGS
#	define _SCL_SECURE_NO_WARNINGS
#endif

#include "Global.h"

static std::vector<std::string> entries;

static int callbackTools(void *data, int argc, char **argv, char **azColName);

int maintainTools();

std::vector<std::string> getEntry();

#endif