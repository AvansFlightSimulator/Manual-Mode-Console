#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <nlohmann/json.hpp> // Include the nlohmann JSON library

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

using json = nlohmann::json;  // For convenience

class TCPServer {
public:
    // Constructor that initializes the server IP and port (default values can be provided)
    TCPServer(const std::string& server_ip = "192.168.137.123", int server_port = 32760);

    // Destructor to clean up resources
    ~TCPServer();

    // Start listening for incoming client connections
    void startListening();

    // Send data to the connected client
    void sendData(const std::string& data);

    // Receive data from the client and parse it
    void receiveData();

    // Close the connection
    void closeConnection();

    // Getter for currentPositions to access it outside of the class
    std::array<float, 6> getCurrentPositions() const;

    bool isConnected;           // Connection status

private:
    SOCKET server_sock;         // Server socket
    SOCKET client_sock;         // Client socket
    sockaddr_in server_address; // Server address
    sockaddr_in client_address; // Client address

    // array to store the current positions (6 floats)
    std::array<float, 6> currentPositions;

    // Accept an incoming client connection
    void acceptClient();

    // Enable TCP keep-alive on the client socket
    void enableKeepAlive(SOCKET sock, DWORD keepAliveTime, DWORD keepAliveInterval);
};

#endif // TCP_SERVER_H
