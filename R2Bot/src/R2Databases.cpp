#include "JobHandler/R2Databases.h"
#include "Data/DrawerData.h"
#include "Data/RFIDData.h"


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
	return 0;
}

string sqlCommand(string const& commandType, string database, string table) {

	string sql;

	if (commandType == "INSERT") {
		if (table == "TOOLS") {
			sql = "INSERT INTO " + table + " (RIFD,TOOL0, TOOL1, TOOL2, TOOL3,TOOL4, TOOL5, T)" + "VALUES (";
		}
		else if (table == "USERS") {
			sql = "INSERT INTO " + table + " (ID, NAME)\" \ \"VALUES";
		}
	}
	else if (commandType == "SELECT") {
		sql = "SELECT * from " + table;

	}
	else if (commandType == "CREATE") {
		if (table == "TOOLS") {
			sql = "CREATE TABLE TOOLS("
				"RFID INT PRIMARY KEY     NOT NULL,"
				"TOOL0       TEXT    NOT NULL,"
				"TOOL1       TEXT    NOT NULL,"
				"TOOL2       TEXT    NOT NULL,"
				"TOOL3       TEXT    NOT NULL,"
				"TOOL4       TEXT    NOT NULL,"
				"TOOL5       TEXT    NOT NULL,"
				"T REAL DEFAULT (datetime('now', 'localtime'));";
		}
		else if (table == "USERS") {
			sql = "CREATE TABLE USERS("  \
				"ID INT PRIMARY KEY     NOT NULL," \
				"NAME           TEXT    NOT NULL;";
		}
	}
	else if (commandType == "DELETE") {
		if (table == "TOOLS") {
			sql = "DELETE FROM TOOLS WHERE T IN (SELECT T FROM TOOLS ORDER BY T DESC LIMIT 10";
		}
	}
	return sql;
}

R2Databases::R2Databases(string dB, string tab) {
	database = dB;
	table = tab;
}

R2Databases::~R2Databases() {

}

string R2Databases::getName() {
	return "R2 Databases";
}

bool R2Databases::ping() {
	return true;
}

void R2Databases::fillData(smap<ptr<void>>& sensorData) {}

void R2Databases::execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs) {
	char * sql;
	string sqlLite;
	entries.clear();
	auto inventory = data.find("DRAWER");
	auto rfid = data.find("RFID");

	bool authenticated = false;

	if (rfid != data.end()) {
		string command = "SELECT * from USERS WHERE RFID = " + std::static_pointer_cast<RFIDData>(rfid->second)->ID;

		char dbArray[] = ".db";
		sqlite3 *db;
		sqlite3_open(database.c_str() + *dbArray, &db);
		char *zErrMsg = 0;
		const char* str = "Callback function called";

		sqlite3_exec(db, sql, callback, (void*)str, &zErrMsg);
		sql = (char*)sqlCommand("SELECT", database, table).c_str();
		sqlite3_exec(db, sql, callback, (void*)str, &zErrMsg);

		sqlite3_close(db);

		if (entries.size() != 0) {
			authenticated = true;
			data["DATABASE"] = std::shared_ptr< int>(new int(1));
		}
		else {
			data["DATABASE"] = std::shared_ptr<int>(new int(0));
		}
	}

	if (inventory != data.end() && authenticated) {
		entries.clear();
		string inv = (std::static_pointer_cast<DrawerData>(inventory->second)->inventory);
		string invV = inv.substr(0, 1) + "," + inv.substr(1, 2) + "," + inv.substr(2, 3) + "," + inv.substr(3, 4) + "," + inv.substr(4, 5) + "," + inv.substr(5) + ",";
		string rfid = "1238";
		string total = rfid + "," + invV;
		sql = (char*)total.c_str(); //add on data

		char dbArray[] = ".db";
		sqlite3 *db;
		sqlite3_open(database.c_str() + *dbArray, &db);
		char *zErrMsg = 0;
		const char* str = "Callback function called";

		sqlite3_exec(db, sql, callback, (void*)str, &zErrMsg);
		sql = (char*)sqlCommand("SELECT", database, table).c_str();
		sqlite3_exec(db, sql, callback, (void*)str, &zErrMsg);

		sqlite3_close(db);

		data["TOOLS"] = std::make_shared<vector<string>>(entries);
	}

}
