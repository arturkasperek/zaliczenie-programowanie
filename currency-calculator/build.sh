#!/bin/bash
g++-5 -I"/usr/include" main.cpp jsonxx.cpp -o ~/bin/cinkciarzApp -std=c++14 -L"/usr/lib/" -lboost_filesystem -lboost_regex -lboost_coroutine -lboost_context -lboost_system -lpthread -lcurl
exit 0
