#!/bin/bash
echo "Building TestApplication"
echo "> using debug build <"
clang++ -g -target x86_64-apple-macos11.3 TestApplication.cpp -std=gnu++1z \
-framework CoreFoundation -framework CoreServices -o TestApplication
if [[ $? -eq 0 ]]
then
    echo "> TestApplication build ok <"
else
    echo "> Test application build error"
fi

echo "> build process complete <"