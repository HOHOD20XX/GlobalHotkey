current_dir=$(pwd)

cd ../../
cmake --preset=disable-hook
cd ./build/disable-hook
make -j
cd ./example
./example1

cd $current_dir
