# Use an official Ubuntu as a parent image
FROM gcc:latest

ENV PROPERTY_FILE=server.properties
ENV EXECUTABLE=./build/chat_server
ENV PORT=1110

# Create and set the working directory
WORKDIR /app

# Copy the source code and Makefile into the container
COPY src ./src
COPY Makefile .
COPY src/properties/sever.properties .

# Build the application
RUN make

# Expose the port
EXPOSE $PORT

# Specify the command to run the application
CMD sh -c "$EXECUTABLE $PROPERTY_FILE"
RUN echo "Booting up server on port $PORT..."