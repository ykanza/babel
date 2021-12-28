#!/bin/bash

cd build
conan install .. --build missing
cmake .. -G "Unix Makefiles"
cmake --build .
rm ../bin/babel_client
rm ../bin/babel_server
mv bin/babel_client ../bin
mv bin/babel_server ../bin