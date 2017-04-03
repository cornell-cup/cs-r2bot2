# Boost C++ framework
# http://www.boost.org/

# 1. Download Boost source from http://www.boost.org/users/download/ and place in this folder
# OR
# 1. Download prebuilt Windows binaries from https://sourceforge.net/projects/boost/files/boost-binaries/
# 2. Add `boost_x_xx_x` to the include path

import subprocess

if __name__=="__main__":
    if "MSYS" in platform.system():
        subprocess.call([ "pacman", "-S", "mingw-w64-x86_64-boost" ])