cmake -B build -DGBHK_BUILD_EXAMPLE=OFF
make -C build -j
cmake --install build --prefix=install
