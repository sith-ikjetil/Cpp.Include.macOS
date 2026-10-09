#!/bin/bash
#: Title       : build-debug.sh
#: Date        : 2022-07-23
#: Author      : Kjetil Kristoffer Solberg <post@ikjetil.no>
#: Version     : 1.0
#: Description : Builds TestApplication/TestDaemon/TestClient and TestServer.
echo "Building TestApplication..."
echo "> using debug build <"
clang++ -g TestApplication.cpp -std=c++23 -pthread -framework CoreServices -o TestApplication
if [[ $? -eq 0 ]]
then
    echo "> TestApplication build ok <"
else
    echo "> TestApplication build error <"
fi
echo "> build process complete <"

echo ""

echo "Building TestDaemon ..."
echo "> using debug build <"
clang++ -g TestDaemon.cpp --std=c++23 -o TestDaemon
if [[ $? -eq 0 ]]
then
    echo "> TestDaemon build ok <"
else
    echo "> TestDaemon build error <"
fi
echo "> build process complete <"

echo ""

echo "Building TestClient..."
echo "> using debug build <"
clang++ -ggdb TestClient.cpp -o TestClient -std=c++23
if [[ $? -eq 0 ]]
then
    echo "> TestClient build ok <"
else
    echo "> TestClient build error <"
fi
echo "> build process complete <"

echo ""

echo "Building TestServer..."
echo "> using debug build <"
clang++ -ggdb TestServer.cpp -o TestServer -std=c++23
if [[ $? -eq 0 ]]
then
    echo "> TestServer build ok <"
else
    echo "> TestServer build error <"
fi
echo "> build process complete <"
