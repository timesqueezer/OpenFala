#!/bin/bash


if [ ! -d "./bin" ]; then
   mkdir -p "./bin" || exit 1
fi 


clang++ -Wall -o bin/client -lsfml-system -lsfml-network client.cpp

clang++ -Wall -o bin/server -lsfml-system -lsfml-network server.cpp
