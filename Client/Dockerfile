# Use a base image with C development tools
FROM gcc:latest

# Set the working directory inside the container
WORKDIR /app

# Copy the server and client directories into the container
COPY . /app/client

# Compile the server and client code
WORKDIR /app/client
RUN make

# RUN gcc -o client/client_executable client/*.c

# Expose any necessary ports (if your server requires it)
EXPOSE 1110

#CMD ["./build/chat_client src/properties/server/claire.properties"]
