#!/bin/bash
g++ -Wall -c -ggdb ../data_queue.cpp -o ../data_queue.o
g++ -Wall -c -ggdb *.c
g++ -Wall -ggdb -o ../../bin/remote_fifo_server -lpthread ../data_queue.o remote_fifo_client_instance.o remote_fifo_clnt.o remote_fifo_server_instance.o remote_fifo_server.o remote_fifo_svc.o remote_fifo_xdr.o
