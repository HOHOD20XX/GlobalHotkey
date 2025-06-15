current_dir=$(pwd)

cd ../../
cmake -B ./build/lib -DCMAKE_BUILD_TYPE=Debug -DGBHK_BUILD_EXAMPLE=OFF
make -C ./build/lib
cmake -B ./build/lib -DCMAKE_BUILD_TYPE=Release -DGBHK_BUILD_EXAMPLE=OFF
make -C ./build/lib

cd $current_dir
