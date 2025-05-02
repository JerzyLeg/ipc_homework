# Simple Client-Server File Transfer

## Description

This project implements a simple client-server application in C++ that transfers a file over the network. The client sends a file to the server.

## Requirements

- C++ compiler (e.g., `g++`)
- Boost.Asio library

## Building

1. Install Boost.Asio (on Ubuntu/Debian):

   ```bash
   sudo apt-get install libboost-all-dev
   ```

2. Compile the server and client:

   ```bash
   g++ server.cpp -std=c++20 -o server -lboost_system -lpthread
   g++ client.cpp -std=c++20 -o client -lboost_system -lpthread
   ```

## Running

1. Start the server:

   ```bash
   ./server
   ```

2. Run the client, providing the path to the file as an argument:

   ```bash
   ./client path/to/file.txt
   ```

## Notes

- The server saves the received file as `received_file`.
- Communication happens over `localhost` on port `8080`.
