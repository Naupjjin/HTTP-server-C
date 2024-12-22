#/bin/sh

gcc server.c -o server -lmicrohttpd
./server
