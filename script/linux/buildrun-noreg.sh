current_dir=$(pwd)

cd ../../
cmake --preset=disable-register
cd ./build/disable-register
make
cd ./example1
./example1.exe

cd $current_dir
