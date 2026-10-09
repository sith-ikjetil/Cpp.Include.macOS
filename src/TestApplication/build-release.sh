#!/bin/bash
#: Title       : build-release.sh
#: Date        : 2020-12-25
#: Author      : Kjetil Kristoffer Solberg <post@ikjetil.no>
#: Version     : 1.0
#: Description : Builds release TestApplication/TestDaemon/TestClient and TestServer.
echo "Building TestApplication..."
echo "> using release build <"
clang++ TestApplication.cpp -std=c++23 -pthread -framework CoreServices -o TestApplication
if [[ $? -eq 0 ]]
then
    echo "> TestApplication build ok <"
else
    echo "> TestApplication build error <"
fi
echo "> build process complete <"

echo ""

echo "Building TestDaemon ..."
echo "> using release build <"
clang++ TestDaemon.cpp --std=c++23 -o TestDaemon
if [[ $? -eq 0 ]]
then
    echo "> TestDaemon build ok <"
else
    echo "> TestDaemon build error <"
fi
echo "> build process complete <"

echo ""

echo "Building TestClient..."
echo "> using release build <"
clang++ TestClient.cpp -o TestClient -std=c++23
if [[ $? -eq 0 ]]
then
    echo "> TestClient build ok <"
else
    echo "> TestClient build error <"
fi
echo "> build process complete <"

echo ""

echo "Building TestServer..."
echo "> using release build <"
clang++ TestServer.cpp -o TestServer -std=c++23
if [[ $? -eq 0 ]]
then
    echo "> TestServer build ok <"
else
    echo "> TestServer build error <"
fi
echo "> build process complete <"
