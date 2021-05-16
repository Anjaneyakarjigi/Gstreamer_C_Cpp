

cmake -H. -Bbuild -G "Visual Studio 14 2015 Win64"
cmake --build build

COPY "test.mp4" "Debug/test.mp4"
START Debug/File2RTPStream.exe 
