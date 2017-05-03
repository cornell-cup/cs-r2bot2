#include "JobHandler/R2Databases.h"

static int callback(void *data, int argc, char **argv, char **azColName) {
	entries.clear();
	for (int i = 0; i < argc; i++) {
		entries.push_back(azColName[i]);
		entries.push_back(",");
		entries.push_back(argv[i]);
		if (std::string(azColName[i]).compare("DATE") == 0) {
			entries.push_back("@");
		}
		else {
			entries.push_back("|");
		}
	}
}

string sqlCommand(string const& commandType, string database, string table) {
	
	string sql;

	if (commandType == "INSERT") {
		if (table == "TOOLS") {
			sql = "INSERT INTO " + table + " (RIFD, NAME, TOOLNAME, DATE)\" \ \"VALUES";
		}
		else if (table == "USERS") {
			sql = "INSERT INTO " + table + " (ID, NAME, TOOLCOUNT)\" \ \"VALUES";
		}
	}
	else if (commandType == "SELECT") {
		sql = "SELECT * from " + table;
		
	}
	else if (commandType == "CREATE") {
		if (table == "TOOLS") {
			sql = "CREATE TABLE TOOLS("
				 "RFID INT PRIMARY KEY     NOT NULL,"
				 "NAME           TEXT    NOT NULL,"
				 "TOOLNAME       TEXT    NOT NULL,"
				 "DATE     TEXT     NOT NULL);";
		}
		else if (table == "USERS") {
			sql = "CREATE TABLE USERS("  \
				 "ID INT PRIMARY KEY     NOT NULL," \
				 "NAME           TEXT    NOT NULL," \
				 "ToolCount      INT     NOT NULL);";
		}
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
	char * sql;

	auto inventoryD = data.find("DRAWER1");
	if (inventoryD != data.end()){
		sql = (char*)sqlCommand("INSERT", database, table).c_str(); //add on data
	}	
	char dbArray[] = ".db";
	sqlite3 *db;
	sqlite3_open(database.c_str() + *dbArray, &db);	
	char *zErrMsg = 0;
	const char* str = "Callback function called";

	sqlite3_exec(db, sql, callback, (void*)str, &zErrMsg);
	sql = (char*)sqlCommand("SELECT", database, table).c_str();
	sqlite3_exec(db, sql, callback, (void*)str, &zErrMsg);

	sqlite3_close(db);
	
}
	