# crow C++ Web Framework
# https://github.com/ipkn/crow

# crow is a header only C++ framework
# 1. Clone crow from https://github.com/ipkn/crow
# 2. Add `crow/amalgamate` to the include path
# 3. Include `crow_all.h`

import subprocess

if __name__=="__main__":
    subprocess.call([ "git", "clone", "https://github.com/ipkn/crow" ])
    subprocess.call([ "python", "merge_all.py"], cwd="crow/amalgamate")
