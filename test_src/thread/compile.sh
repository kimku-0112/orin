#!/usr/bin/env bash
echo "comm_thread compile"
g++ -c comm_thread.cpp -o comm_thread.o
g++ -c main.cpp -o main.o
g++ -o main main.o comm_thread.o
rm main.o
rm comm_thread.o
echo "comm_thread compile complete"
