#!/bin/bash
g++ -c -Wall  -ggdb remote_fifo.cpp -o remote_fifo.o
rm ../bin/remote_fifo.a
ar -cvq ../bin/remote_fifo.a remote_fifo.o rpc/remote_fifo_xdr.o rpc/remote_fifo_clnt.o rpc/remote_fifo_client_instance.o rpc/remote_fifo_server.o data_queue.o
g++ rf_test.cpp -lpthread ../bin/remote_fifo.a -o ../bin/remote_fifo.bin
#g++ -Wall -ggdb rf_test.cpp remote_fifo.cpp rpc/remote_fifo_xdr.o rpc/remote_fifo_clnt.o rpc/remote_fifo_client_instance.o rpc/remote_fifo_server.o data_queue.o -lpthread -o ../bin/rf_test.bin
