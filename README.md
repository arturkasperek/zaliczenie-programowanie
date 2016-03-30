Project requirements
=============
To run project You need modern version of Windows, highter then Windows 7. You also need Visual Studio 2015 Communnity free verion. 

CURL install
=============
To start project you need curl. First donwload repo from https://github.com/curl/curl, run Visual Studio Commmand Promp and type: 

```
 cd path/to/libcurl
 nmake /f Makefile.vc mode=static VC=14 DEBUG=yes MACHINE=x86 
```
After that in root folder of curl you should see new folder, builds. There you can find 3 folders, in one of this folders you can find folder which contains /bin, /include and /lib folders. When you find this directory you must open Visual Studio project, open project property window and in VC++ Directories You add to Include Directories /include path that I told above and the same /lib path for Library Directories. Now in property page go to C/C++ and next to Preprocessor and add to Preprocessor Definitions CURL_STATICLIB. After that in Linker -> Input -> Additional Dependencies add name of lib that was compiled in curl/builds/lib, for me that was libcurl_a_debug.lib.

Boost install
=============
Download source of boost 1.6 from internet.

```
 cd path/to/boost
 bjam --toolset=msvc-14.0 variant=debug,release link=static runtime-link=static address-model=64
```
After that in folder stage/lib you should find .lib files. You must add the same that for CURL path to lib and path for include, path for include should be root folder of boost, for me that was C:\boost_1_60_0