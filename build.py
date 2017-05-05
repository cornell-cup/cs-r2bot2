import glob, os, platform, subprocess, sys
from functools import partial
from multiprocessing import Pool
import multiprocessing
from os import path

IS_WINDOWS = "Windows" in platform.system()
IS_MSYS = "MSYS" in platform.system()

CC = "g++"
CFLAGS = ["-pthread", "-std=c++11", "-Wall"]

SRC_FOLDER = "{}/src"           # Source files
INC_FOLDER = "{}/include"       # Header files
OBJ_FOLDER = "obj"              # Object code
BIN_FOLDER = "bin"              # Compiled executables
LIB_INC_FOLDER = [
    "-I", "lib/boost_1_63_0",
    "-I", "lib/crow/amalgamate",
    "-I", "lib/cs-communication-utilities/Utilities",
    "-I", "lib/cs-r2-protocol/src",
    "-I", "lib/opencv3/opencv/build/include",
    "-I", "lib/sqlite",
] + (
    ["-I", "/mingw64/include"] if IS_MSYS else []
)
LIBS = [
    "-lboost_system",
    "-lboost_date_time",
    "-lsqlite3",
]

BINARY_NAME = "{}." + ("exe" if IS_WINDOWS or IS_MSYS else "x")

def compile(s, includes, library_includes, objs):
    o = path.join(OBJ_FOLDER, path.basename(s)[:-4] + ".o")
    args = [CC] + CFLAGS + ["-c"] + includes + library_includes + [s] + ["-o", o]
    print("Executing {}".format(args))
    subprocess.check_output(args)
    return o


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python {} <project>".format(sys.argv[0]))
        sys.exit(0)

    # Set
    CORE_NAME = "R2Bot"
    PROJECT_NAME = sys.argv[1]
    BINARY_NAME = BINARY_NAME.format(PROJECT_NAME)

    # Make folders
    if not path.isdir(OBJ_FOLDER):
        os.mkdir(OBJ_FOLDER)

    if not path.isdir(BIN_FOLDER):
        os.mkdir(BIN_FOLDER)

    # Find all source files
    sources =   glob.glob("{}/*.cpp".format(SRC_FOLDER.format(CORE_NAME))) + \
                glob.glob("{}/*.cpp".format(SRC_FOLDER.format(PROJECT_NAME)))
    includes =  ["-I", INC_FOLDER.format(CORE_NAME)] + \
                ["-I", INC_FOLDER.format(PROJECT_NAME)]

    # Compile each to objects
    # TODO Don't recompile unchanged files
    #objects = list(map(lambda s: compile(s, includes, LIB_INC_FOLDER, OBJ_FOLDER), sources))
    if len(sys.argv) >= 3:
        threads = int(sys.argv[2])
    else:
        threads = multiprocessing.cpu_count()
    
    partial_compile = partial(compile, includes=includes, library_includes=LIB_INC_FOLDER, objs=OBJ_FOLDER)
    
    if threads == 1:
        objects = map(partial_compile, sources)
    else:
        p = Pool(threads)
        objects = p.map(partial_compile, sources)

    # Build the binary
    args = [CC] + CFLAGS + objects + LIBS + ["-o", path.join(BIN_FOLDER, BINARY_NAME)]
    print("Executing {}".format(args))
    subprocess.check_output(args)
