#ifndef _R2BOT_R2DATABASES
#define _R2BOT_R2DATABASES

#ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _SCL_SECURE_NO_WARNINGS
#	define _SCL_SECURE_NO_WARNINGS
#endif
#include "JobHandler.h"
#include "Global.h"

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include <deque>
#include <iostream>
#include <string>
#include <vector>

static std::vector<std::string> entries;

class R2Databases : public JobHandler {
private:
	sqlite3 *db;
	const char* sql;
	const char* data = "Callback function called";
	std::string database;
	std::string table;
public:
	R2Databases(std::string database, std::string table);
	virtual ~R2Databases();
	std::string getName();
	bool ping();
	virtual void fillData(smap<ptr<void>>& sensorData);
	virtual void execute(std::deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};
#endif