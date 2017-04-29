#include "JobHandler/R2Databases.h"


static int callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

string sqlCommand(string const& commandType, string database, string table) {
	string sql;

	if (commandType == "INSERT") {
		sql = "INSERT INTO " + table;
	}
	else if (commandType == "SELECT") {

	}
	else if (commandType == "CREATE") {

	}


	return sql;
}

R2Databases::R2Databases(string dB, string tab){
	database = dB;
	table = tab;
}

R2Databases::~R2Databases(){

}

string R2Databases::getName() {
	return "R2 Databases";
}

bool R2Databases::ping() {
	return true;
}

void R2Databases::fillData(smap<ptr<void>>& sensorData) {
	
}
void R2Databases::execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs) {
	
	auto inventory = data.find("DRAWER1");
	if (inventory != data.end()){
		sqlCommand("INSERT");
	}
	
	char dbArray[] = ".db";
	sqlite3 *db;
	sqlite3_open(database.c_str() + *dbArray, &db);
	char * sql;
	char *zErrMsg = 0;
	const char* str = "Callback function called";

	sqlite3_exec(db, sql, callback, (void*)str, &zErrMsg);

	sql = (char*)sqlCommand("SELECT").c_str();

	sqlite3_close(db);
}
	