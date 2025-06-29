@echo off

set "current_dir=%cd%"

cd ../../
cmake -B ./build/shared -DCMAKE_BUILD_TYPE=Debug -DGBHK_BUILD_EXAMPLE=OFF -DGBHK_BUILD_SHARED=ON
make -C ./build/shared -j
cmake -B ./build/shared -DCMAKE_BUILD_TYPE=Release -DGBHK_BUILD_EXAMPLE=OFF -DGBHK_BUILD_SHARED=ON
make -C ./build/shared -j

cd %current_dir%
