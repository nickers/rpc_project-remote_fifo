#!/bin/bash
g++ -Wall -ggdb rf_test.cpp remote_fifo.cpp rpc/remote_fifo_xdr.o rpc/remote_fifo_clnt.o
