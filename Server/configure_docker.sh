#!/bin/bash

docker build -t chat_server_image .

docker run --name chat_server_container -p 8080:8080 chat_server_image
