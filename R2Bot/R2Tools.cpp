#include <stdio.h>
#include <stdlib.h>
#include <../sqlite-amalgamation-3170000/sqlite3.h> 

static int callbackTools(void *data, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
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
	//sql = "CREATE TABLE TOOLS("  \
		"RFID INT PRIMARY KEY     NOT NULL," \
		"NAME           TEXT    NOT NULL," \
		"TOOLNAME       TEXT    NOT NULL," \
		"DATE     TEXT     NOT NULL);";

	//sql = "INSERT INTO TOOLS (RFID, NAME, TOOLNAME, DATE)" \
		"VALUES(123, 'Emily', 'Laura', '4/11/17'); ";
	sql = "SELECT * from TOOLS";

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
