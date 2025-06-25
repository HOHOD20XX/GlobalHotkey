current_dir=$(pwd)

cd ../../
cmake --preset=disable-register
cd ./build/disable-register
make -j
cd ./example
sudo ./example1

cd $current_dir
