current_dir=$(pwd)

cd ../../
cmake --preset=disable-hook
cd ./build/disable-register
make
cd ./example
./example1

cd $current_dir
