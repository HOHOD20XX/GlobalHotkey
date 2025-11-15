current_dir=$(pwd)

cd ../../
cmake -B ./build/static -DCMAKE_BUILD_TYPE=Debug -DGLOBAL_HOTKEY_BUILD_EXAMPLE=OFF
make -C ./build/static -j
cmake -B ./build/static -DCMAKE_BUILD_TYPE=Release -DGLOBAL_HOTKEY_BUILD_EXAMPLE=OFF
make -C ./build/static -j

cd $current_dir
