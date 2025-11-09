current_dir=$(pwd)

cd ../../
cmake -B ./build/example_use_register -DCMAKE_BUILD_TYPE=Release -DGBHK_DISABLE_HOOK=ON -DGBHK_BUILD_EXAMPLE=ON
make -C ./build/example_use_register -j
cmake -B ./build/example_use_hook -DCMAKE_BUILD_TYPE=Release -DGBHK_DISABLE_REGISTER=ON -DGBHK_BUILD_EXAMPLE=ON -DGBHK_EXAMPLE_USE_HOOK=ON
make -C ./build/example_use_hook -j

cd $current_dir
