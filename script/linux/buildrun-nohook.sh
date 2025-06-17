current_dir=$(pwd)

sudo apt install libevdev-dev
cd ../../
cmake --preset=disable-hook
cd ./build/disable-hook
make
cd ./example
./example1

cd $current_dir
