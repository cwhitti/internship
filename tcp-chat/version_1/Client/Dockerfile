# Use an official Ubuntu as a parent image
FROM gcc:latest

# grab variables
ARG USERNAME
ARG PORT

# properties of file
ENV PROPERTY_FILE=$USERNAME.properties
ENV EXECUTABLE=./build/chat_client

# Create and set the working directory
WORKDIR /app

# Copy the source code and Makefile into the container
COPY src ./src
COPY Makefile .
COPY src/properties/properties/$PROPERTY_FILE .

# Build the application
RUN make clean
RUN make

# DONT FORGET TO SET UP AI STUFF

# Expose the port the server is listening on
RUN echo "Exposing port: $PORT"

EXPOSE $PORT

# Specify the command to run the application
CMD sh -c "$EXECUTABLE $PROPERTY_FILE"
