set "current_dir=%cd%"

cd ../../
cmake --preset=disable-hook
cd build/disable-hook
make
cd example
example1.exe

cd %current_dir%
