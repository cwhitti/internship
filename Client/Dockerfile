# Use an official Ubuntu as a parent image
FROM gcc:latest

# set variables
ARG USERNAME
ENV DIR=remote
ENV PROPERTY_FILE=$USERNAME.properties
ENV EXECUTABLE=./build/chat_client
ENV PORT=1110

# Create and set the working directory
WORKDIR /app

# Copy the source code and Makefile into the container
COPY src ./src
COPY Makefile .
COPY src/properties/$DIR/$PROPERTY_FILE .

# Build the application
RUN make

# Conditionally set the port based on the directory
RUN if [ "$DIR" = "local" ]; then export PORT=5000; fi

# Expose the port the server is listening on
EXPOSE $PORT

# Specify the command to run the application
CMD sh -c "$EXECUTABLE $PROPERTY_FILE"
