#!/bin/bash

g++ -Wall -o client -lsfml-system -lsfml-network client.cpp

g++ -Wall -o server -lsfml-system -lsfml-network server.cpp
