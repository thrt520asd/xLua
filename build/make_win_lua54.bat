mkdir build64_54 & pushd build64_54
cmake -DLUA_VERSION=5.4.1 -G "Visual Studio 16 2019" -A x64  ..
popd
cmake --build build64_54 --config Debug
md plugin_lua54\Plugins\x86_64
copy /Y build64_54\Debug\xlua.dll plugin_lua54\Plugins\x86_64\xlua.dll

mkdir build32_54 & pushd build32_54
cmake -DLUA_VERSION=5.4.1 -G "Visual Studio 16 2019" -A Win32 ..
popd
cmake --build build32_54 --config Debug
md plugin_lua54\Plugins\x86
copy /Y build32_54\Debug\xlua.dll plugin_lua54\Plugins\x86\xlua.dll

xcopy /E /Y plugin_lua54\Plugins ..\Assets\Plugins
pause