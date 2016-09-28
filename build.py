import glob, os, platform, subprocess
from os import path

CC = "g++"
CFLAGS = ["-std=c++11"]

SRC_FOLDER = "R2Bot/src"            # Source files
INCLUDE_FOLDER = "R2Bot/include"    # Header files
OBJ_FOLDER = "obj"            # Object code
BIN_FOLDER = "bin"            # Compiled executable libraries and binaries

BINARY_NAME = "R2Bot." + ("exe" if platform.system() is "Windows" else "x")

if __name__ == "__main__":
    # Folders
    if not path.isdir(OBJ_FOLDER):
        os.mkdir(OBJ_FOLDER)

    if not path.isdir(BIN_FOLDER):
        os.mkdir(BIN_FOLDER)

    # Find all source files
    sources = glob.glob("{src}/*.cpp".format(src=SRC_FOLDER))

    # Compile each to objects
    # TODO Parallelize with multiprocessing
    # TODO Don't recompile unchanged files
    objects = []
    for s in sources:
        o = path.join(OBJ_FOLDER, path.basename(s)[:-4] + ".o")
        objects.append(o)
        args = [CC] + CFLAGS + ["-c"] + ["-I", INCLUDE_FOLDER] + [s] + ["-o", o]
        print("Executing {}".format(args))
        subprocess.check_output(args)

    # Build the binary
    args = [CC] + CFLAGS + ["-I", INCLUDE_FOLDER] + objects \
        + ["-o", BIN_FOLDER + "/" + BINARY_NAME]
    print("Executing {}".format(args))
    subprocess.check_output(args)
