


##	Some of the help provided at http://google.github.io/VoltAir
##

Clone the repo 

git clone http://github.com/google/voltair.git

##Download qt5, with qtcreator



## Submodules
## Notes: for submodules , first they need to be initialized then clone.
##

cd voltair/third_party
git submodule init
git submodule update

## Build
For example:

cd liquidfun/Box2D
cmake -G'Unix Makefiles'
make
To perform a debug build:

cd liquidfun/Box2D
cmake -G'Unix Makefiles' -DCMAKE_BUILD_TYPE=Debug
##or -DCMAKE_BUILD_TYPE=Release if you want the release build.

make
