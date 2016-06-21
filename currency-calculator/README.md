Windows
=============
To run project You need Windows 7 or highter version and Chrome browser. You also need Visual Studio Communnity(free version). 

## Boost install

Download source of boost 1.6 from internet. Now search in system "Developer Command Prompt VS-<version>" and type:
```
 cd path/to/boost
 bootstrap
 .\b2
```
After that in folder stage/lib you should find .lib files. Now go to visual, open "Project --> currency-calculator --> VC++ Directories", add to Include Directoriec path to root folder of boost, for me that was "C:\boost_1_60_0", after that you should also add path for lib, for me "C:\boost_1_60_0\stage\lib"
##
Now You can compile and run project !

Linux
=============
g++ compilator is required capable of compiling using c++14 standard.

Needed libraries: boost, curl, pthread

g++-5 -I"/usr/include" main.cpp jsonxx.cpp -o ~/bin/cinkciarzApp -std=c++14 -L"/usr/lib/" -lboost_filesystem -lboost_regex -lboost_coroutine -lboost_context -lboost_system -lpthread -lcurl
