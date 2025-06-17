current_dir=$(pwd)

sudo apt install libevdev-dev
cd ../../
cmake --preset=disable-register
cd ./build/disable-register
make
cd ./example
sudo ./example1

cd $current_dir
