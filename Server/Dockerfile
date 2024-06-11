# Use an official Ubuntu as a parent image
FROM gcc:latest

ENV DIR=remote
ENV PROPERTY_FILE=$DIR.properties
ENV EXECUTABLE=./build/chat_server
ENV PORT=1110

# Create and set the working directory
WORKDIR /app

# Copy the source code and Makefile into the container
COPY src ./src
COPY Makefile .
COPY src/properties/$DIR/$DIR.properties .

# Build the application
RUN make

# Conditionally set the port based on the directory
RUN if [ "$DIR" = "local" ]; then export PORT=5000; fi

# Expose the port
EXPOSE $PORT

# Specify the command to run the application
CMD sh -c "$EXECUTABLE $PROPERTY_FILE"
RUN echo "Booting up server on port $PORT..."