#!/bin/bash


if [ ! -d "./bin" ]; then
   mkdir -p "./bin" || exit 1
fi 

clang++ -g -Wall -o client.bin -lsfml-system -lsfml-network client.cpp

clang++ -g -Wall -o server.bin -lsfml-system -lsfml-network server.cpp
