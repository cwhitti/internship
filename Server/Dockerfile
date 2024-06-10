# Use an official Ubuntu as a parent image
FROM gcc:latest

# Set the maintainer label
LABEL maintainer="twiggleicious@gmail.com"

# Install dependencies


# RUN apt-get update && apt-get install -y \
#     build-essential \
#     cmake \
#     make \
#     g++ \
#     && rm -rf /var/lib/apt/lists/*

# Create and set the working directory
WORKDIR /app

# Copy the source code and Makefile into the container
COPY src ./src
COPY Makefile .
COPY src/server.properties .

# Build the application
RUN make

# Expose the port the server is listening on
EXPOSE 5000

# Specify the command to run the application
CMD ["./build/chat_server", "server.properties"]
