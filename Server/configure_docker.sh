#!/bin/bash


docker build -t chat_server_image .

docker run --name chat_server_container -p 1110:1110 chat_server_image
