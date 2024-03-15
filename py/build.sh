cd ..

mkdir build
cd build || exit
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target=digisim
