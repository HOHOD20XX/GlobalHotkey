current_dir=$(pwd)

cd ../../
cmake -B ./build/static -DCMAKE_BUILD_TYPE=Debug -DGBHK_BUILD_EXAMPLE=OFF
make -C ./build/static -j
cmake -B ./build/static -DCMAKE_BUILD_TYPE=Release -DGBHK_BUILD_EXAMPLE=OFF
make -C ./build/static -j

cd $current_dir
