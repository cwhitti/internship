#!/bin/bash

docker build -t server_image .

docker run -it --name server_container -p 1110:1110 server_image
