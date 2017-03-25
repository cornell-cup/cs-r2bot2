import glob, os, platform, subprocess, sys
from os import path

CC = "g++"
CFLAGS = ["-pthread", "-std=c++11", "-Wall"]

SRC_FOLDER = "{}/src"           # Source files
INC_FOLDER = "{}/include"       # Header files
OBJ_FOLDER = "{}/obj"           # Object code
BIN_FOLDER = "{}/bin"           # Compiled executable libraries and binaries
LIB_INC_FOLDER = [
    "-I", "lib/crow",
    "-I", "lib/cs-communication-utilities/Utilities",
    "-I", "lib/cs-r2-protocol/src"
]

BINARY_NAME = "{}." + ("exe" if platform.system() is "Windows" else "x")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python {} <project>".format(sys.argv[0]))
        sys.exit(0)

    # Set parameters
    PROJECT_NAME = sys.argv[1]
    SRC_FOLDER = SRC_FOLDER.format(PROJECT_NAME)
    INC_FOLDER = INC_FOLDER.format(PROJECT_NAME)
    OBJ_FOLDER = OBJ_FOLDER.format(PROJECT_NAME)
    BIN_FOLDER = BIN_FOLDER.format(PROJECT_NAME)
    BINARY_NAME = BINARY_NAME.format(PROJECT_NAME)

    # Make folders
    if not path.isdir(OBJ_FOLDER):
        os.mkdir(OBJ_FOLDER)

    if not path.isdir(BIN_FOLDER):
        os.mkdir(BIN_FOLDER)

    # Find all source files
    sources = glob.glob("{}/*.cpp".format(SRC_FOLDER))

    # Compile each to objects
    # TODO Parallelize with multiprocessing
    # TODO Don't recompile unchanged files
    objects = []
    for s in sources:
        o = path.join(OBJ_FOLDER, path.basename(s)[:-4] + ".o")
        objects.append(o)
        args = [CC] + CFLAGS + ["-c"] + ["-I", INC_FOLDER] + LIB_INC_FOLDER + [s] + ["-o", o]
        print("Executing {}".format(args))
        subprocess.check_output(args)

    # Build the binary
    args = [CC] + CFLAGS + ["-I", INC_FOLDER] + objects \
        + ["-o", BIN_FOLDER + "/" + BINARY_NAME]
    print("Executing {}".format(args))
    subprocess.check_output(args)
