# sqlite
# https://sqlite.org

# SQLite is a light SQL database engine
# 1. Head to https://sqlite.org/download.html
# 2. Download the source amalgamation
# 3. Extract source amalgamation to `sqlite`
# 4. Download precompiled binaries
# 5. Extract to `sqlite`

import subprocess
import platform

if __name__=="__main__":
    subprocess.call([ "wget", "https://sqlite.org/2017/sqlite-amalgamation-3170000.zip" ])
    subprocess.call([ "unzip", "sqlite-amalgamation-3170000.zip", "-d", "." ])
    subprocess.call([ "mv", "sqlite-amalgamation-3170000/*", "sqlite" ])
    subprocess.call([ "rm", "sqlite-amalgamation-3170000.zip" ])
    subprocess.call([ "rm", "-r", "sqlite-amalgamation-3170000" ])

    if "MSYS" in platform.system():
        subprocess.call([ "wget", "https://sqlite.org/2017/sqlite-dll-win64-x64-3170000.zip" ])
        subprocess.call([ "zip", "sqlite-dll-win64-x64-3170000.zip", "-d", "sqlite" ])
        subprocess.call([ "C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\VC\\bin\\lib.exe", "/DEF:sqlite/sqlite3.def", "/MACHINE:X64", "/OUT:sqlite3.lib" ])
        subprocess.call([ "rm", "sqlite-dll-win64-x64-3170000.zip" ])
    elif "Windows" in platform.system():
        subprocess.call([ "wget", "https://sqlite.org/2017/sqlite-dll-win64-x64-3170000.zip" ])
        subprocess.call([ "zip", "sqlite-dll-win64-x64-3170000.zip", "-d", "sqlite" ])
        subprocess.call([ "/C/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/lib.exe", "/DEF:sqlite/sqlite3.def", "/MACHINE:X64", "/OUT:sqlite3.lib" ])
        subprocess.call([ "rm", "sqlite-dll-win64-x64-3170000.zip" ])