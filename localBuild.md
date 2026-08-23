Local terminal build
**************

---In directory EternalTerminal/build---
cd ..
rm -rf build
mkdir -p build
cd build
cmake ..
cmake --build . -j$(nproc)

./EternalTerminalApp
*****************
