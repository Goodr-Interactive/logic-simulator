# This builds the Cython interface.
# You need to run this every time you modify digisim.pyx or the digisim C library.

mkdir build
cd build || exit
cmake ../.. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target=digisim

cd ..
python3 setup.py build_ext --inplace