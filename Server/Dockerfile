# Use a base image with C development tools
FROM gcc:latest

# Set the working directory inside the container
WORKDIR /app

# Copy the server and client directories into the container
COPY . /app/server

# Compile the server and client code
WORKDIR /app/server
RUN make

# RUN gcc -o client/client_executable client/*.c

# Expose any necessary ports (if your server requires it)
EXPOSE 1110

# run the server
CMD ["/app/server/build/chat_server /app/server/src/server.properties"]
