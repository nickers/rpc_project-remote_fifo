#!/bin/bash
mkdir bin
(cd ./src/rpc && ./make.sh)
(cd ./src && ./make.sh)
