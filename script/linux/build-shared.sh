current_dir=$(pwd)

cd ../../
cmake -B ./build/shared/debug -DCMAKE_BUILD_TYPE=Debug -DGLOBAL_HOTKEY_BUILD_SHARED=ON -DGLOBAL_HOTKEY_BUILD_EXAMPLE=OFF
make -C ./build/shared/debug -j
cmake -B ./build/shared/release -DCMAKE_BUILD_TYPE=Release -DGLOBAL_HOTKEY_BUILD_SHARED=ON -DGLOBAL_HOTKEY_BUILD_EXAMPLE=OFF
make -C ./build/shared/release -j

cd $current_dir
