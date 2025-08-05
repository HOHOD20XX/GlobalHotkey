@echo off

set "current_dir=%cd%"

cd ../../
cmake -B ./build/shared/debug -DCMAKE_BUILD_TYPE=Debug -DGBHK_BUILD_SHARED=ON -DGBHK_BUILD_EXAMPLE=OFF
make -C ./build/shared/debug -j
cmake -B ./build/shared/release -DCMAKE_BUILD_TYPE=Release -DGBHK_BUILD_SHARED=ON -DGBHK_BUILD_EXAMPLE=OFF
make -C ./build/shared/release -j

cd %current_dir%
