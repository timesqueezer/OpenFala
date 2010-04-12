#!/bin/bash


if [ ! -d "./bin" ]; then
   mkdir -p "./bin" || exit 1
fi 


g++ -Wall -o bin/client -lsfml-system -lsfml-network client.cpp

g++ -Wall -o bin/server -lsfml-system -lsfml-network server.cpp
