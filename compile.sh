#!/bin/bash

#gcc -O3 -std=c99 -fopenmp main.c -o main
g++ -O3 -std=c++11 main.cpp -lpthread -o main
