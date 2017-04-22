#include "JobHandler/R2Tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <vector>
#include <string>

#include <iostream>



static int callbackTools(void *data, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i<argc; i++) {
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

int maintainTools()
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char* data = "Callback function called";

	/* Open database */
	rc = sqlite3_open("tools.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}

	/* Create SQL statement */
	//sql = argv;
	//where sql will have to be a input from console or website command
	/*
	sql = "CREATE TABLE TOOLS("
		"RFID INT PRIMARY KEY     NOT NULL,"
		"NAME           TEXT    NOT NULL,"
		"TOOLNAME       TEXT    NOT NULL,"
		"DATE     TEXT     NOT NULL);";
	*/

	//sql = "INSERT INTO TOOLS (RFID, NAME, TOOLNAME, DATE)"
	//	"VALUES(123, 'Emily1', 'Laura1', '4/12/17'); ";
	sql = "SELECT * from TOOLS";
	//sql = "DROP TOOLS";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callbackTools, (void*)data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Operation done successfully\n");
	}
	sqlite3_close(db);
	return 0;
}

std::vector<std::string> getEntry() {
	return entries;
}