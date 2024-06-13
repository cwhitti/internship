# Use an official Ubuntu as a parent image
FROM gcc:latest

ARG PORT

ENV PROPERTY_FILE=server.properties
ENV EXECUTABLE=./build/chat_server

# Create and set the working directory
WORKDIR /app

# Copy the source code and Makefile into the container
COPY src ./src
COPY Makefile .
COPY src/properties/$PROPERTY_FILE .

# Build the application
RUN make clean
RUN make

# Expose the port
EXPOSE $PORT

# Specify the command to run the application
CMD sh -c "$EXECUTABLE $PROPERTY_FILE"
RUN echo "Booting up server on port $PORT..."