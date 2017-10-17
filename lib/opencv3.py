# OpenCV 3.2
# http://opencv.org/

# 1. Download OpenCV source or prebuilt binary from http://opencv.org/releases.html and place in this folder
# 2. Add `opencv3\opencv\build\include` to the include path

import subprocess
import platform

if __name__=="__main__":
    if "MSYS" in platform.system():
        subprocess.call([ "pacman", "-S", "mingw-w64-x86_64-opencv" ])
    elif "Linux" in platform.system():
        subprocess.call([ "git", "clone", "https://github.com/cornell-cup/deps" ])
        subprocess.call([ "mv", "deps/opencv", "./"])
        subprocess.call([ "rm", "-rf", "deps" ])
