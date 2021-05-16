
cmake -H. -Bbuild -G "Visual Studio 14 2015 Win64"
cmake --build build

START Debug/WebCamMicrophone2Servers.exe 2 127.0.0.1 8554