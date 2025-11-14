cmake -B build -DGBHK_BUILD_EXAMPLE=OFF -DGBHK_DISABLE_REGISTER=ON
make -C build -j
cmake --install build --prefix=install
