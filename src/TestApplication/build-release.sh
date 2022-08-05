#!/bin/bash
cd $HOME/Code/Cpp.Include.macOS/src/TestApplication
clang++ -target x86_64-apple-macos11.3 TestApplication.cpp -std=gnu++1z \
-framework CoreFoundation -framework CoreServices -o TestApplication