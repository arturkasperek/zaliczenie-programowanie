Project requirements
=============

Windows
=============
To run project You need Windows 7 or highter version and Chrome browser. You also need Visual Studio Communnity(free version). 

Boost install
=============
Download source of boost 1.6 from internet. Now search in system "Developer Command Prompt VS-<version>" and type:
```
 cd path/to/boost
 bootstrap
 .\b2
```
After that in folder stage/lib you should find .lib files. Now go to visual, open "Project --> currency-calculator --> VC++ Directories", add to Include Directoriec path to root folder of boost, for me that was "C:\boost_1_60_0", after that you should also add path for lib, for me "C:\boost_1_60_0\stage\lib"

Now You can compile and run project !