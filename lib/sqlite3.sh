# sqlite
# https://sqlite.org

# SQLite is a light SQL database engine
# 1. Head to https://sqlite.org/download.html
# 2. Download the source amalgamation
# 3. Extract source amalgamation to `sqlite`
# 4. Download precompiled binaries
# 5. Extract to `sqlite`

wget https://sqlite.org/2017/sqlite-amalgamation-3170000.zip
unzip sqlite-amalgamation-3170000.zip -d .
mv sqlite-amalgamation-3170000/* sqlite
if [[ "$OSTYPE" == "msys" ]]; then
    wget https://sqlite.org/2017/sqlite-dll-win64-x64-3170000.zip
    unzip sqlite-dll-win64-x64-3170000.zip -d sqlite
else
    "/C/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/lib.exe" /DEF:sqlite/sqlite3.def /MACHINE:X64 /OUT:sqlite3.lib
fi

rm sqlite-amalgamation-3170000.zip
rm sqlite-dll-win64-x64-3170000.zip
rm -r sqlite-amalgamation-3170000
