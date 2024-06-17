# Use an official Ubuntu as a parent image
FROM gcc:latest

# grab variables
ARG USERNAME
ARG PORT

# Build the application
# properties of file
ENV PROPERTY_FILE=$USERNAME.properties

# Create and set the working directory
WORKDIR /app

# Copy the source code and Makefile into the container
COPY src ./src
COPY Makefile .
COPY src/properties/$PROPERTY_FILE .

# make stuff, clean just to be sure
RUN make clean 
RUN make

# set executable
ENV EXECUTABLE=./build/chat_client

# Expose the port the server is listening on
RUN echo "Exposing port: $PORT"

EXPOSE $PORT

# Specify the command to run the application
CMD sh -c "$EXECUTABLE $PROPERTY_FILE"
