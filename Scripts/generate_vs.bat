cd ../
mkdir Build
cd Build
cmake .. -G "Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=Debug
mkdir Sources\Resources
pause
cmake --build . --config Debug
pause
