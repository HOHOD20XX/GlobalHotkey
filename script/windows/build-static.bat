@echo off

set "current_dir=%cd%"

cd ../../
cmake -B ./build/static/debug -DCMAKE_BUILD_TYPE=Debug -DGLOBAL_HOTKEY_BUILD_EXAMPLE=OFF
make -C ./build/static/debug -j
cmake -B ./build/static/release -DCMAKE_BUILD_TYPE=Release -DGLOBAL_HOTKEY_BUILD_EXAMPLE=OFF
make -C ./build/static/release -j

cd %current_dir%
