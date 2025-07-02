current_dir=$(pwd)

cd ../../
cmake -B ./build/static -DCMAKE_BUILD_TYPE=Debug
make -C ./build/static -j
cmake -B ./build/static -DCMAKE_BUILD_TYPE=Release
make -C ./build/static -j

cd $current_dir
