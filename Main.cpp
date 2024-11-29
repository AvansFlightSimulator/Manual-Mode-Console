#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "TCPServer.h"
#include <thread>
#include <stdlib.h>
#include <iostream>


using json = nlohmann::json;
TCPServer server;

const int SPEED_LIMIT = 400;

void listeningThreadMethod() {
    while (server.isConnected)
    {
        server.receiveData();
    }
}

std::string CheckDigitCount(int value)
{
    if (value >= 100)
        return std::to_string(value);
    else if (value < 100 && value >= 10)
        return "0" + std::to_string(value);
    else
        return "00" + std::to_string(value);
}

int main() {
    // Define server IP and port
    

    // Start listening for client connections
    server.startListening();
    std::thread listeningThread(&listeningThreadMethod);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while (true)
    {
        if (server.isConnected)
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            // Define variables
            std::array<int, 6> positions; // Desired positions for actuators
            std::array<float, 6> currentLegLengths = server.getCurrentPositions();
            std::cout << "Current positions: " << std::endl;
            std::cout << currentLegLengths[0] << std::endl;
            std::cout << currentLegLengths[1] << std::endl;
            std::cout << currentLegLengths[2] << std::endl;
            std::cout << currentLegLengths[3] << std::endl;
            std::cout << currentLegLengths[4] << std::endl;
            std::cout << currentLegLengths[5] << std::endl;

            float hertz = 1; // Default hertz (updates per second)
            float time = 1 / hertz;

            // Get the desired positions from the user
            std::cout << "Enter positions for the 6 actuators (space-separated integers): ";
            for (int i = 0; i < 6; ++i) {
                std::cin >> positions[i];
            }

            // Calculate the required speeds for each actuator
            std::array<int, 6> speeds{ 0, 0, 0, 0, 0, 0 };
            for (int i = 0; i < 6; ++i) {
                // Calculate the absolute speed
                int calculatedSpeed = std::abs((positions[i] - currentLegLengths[i]) / time);

                // Enforce the speed limit
                if (calculatedSpeed > SPEED_LIMIT) {
                    speeds[i] = SPEED_LIMIT;
                }
                else if(calculatedSpeed <= SPEED_LIMIT && calculatedSpeed > 0)
                {
                    speeds[i] = calculatedSpeed;
                }
                else if (calculatedSpeed <= 0) {
                    speeds[i] = 1;
                }
                             
                std::cout << "leg speed " << i << ": " << calculatedSpeed << std::endl;
                //std::cout << positions[0] << std::endl;
                //std::cout << currentLegLengths[0] << std::endl;
            }


            // Prepare the JSON object with positions and speeds
            //nlohmann::json data;
            //data["positions"] = positions;
            //data["speeds"] = speeds;

            // Send the JSON object to the client
            std::cout << "Sending data to client..." << std::endl;

            std::string tempSendingData = "{\"positions\" : [";

            tempSendingData += CheckDigitCount(positions[0]);

            for (size_t i = 1; i < positions.size(); i++)
                tempSendingData += ", " + CheckDigitCount(positions[i]);

            tempSendingData += "], \"speeds\" : [";

            std::cout << CheckDigitCount(speeds[0]) << std::endl;
            tempSendingData += CheckDigitCount(speeds[0]);

            for (size_t i = 1; i < speeds.size(); i++)
            {
                std::cout << CheckDigitCount(speeds[i])  << std::endl;
                tempSendingData += ", " + CheckDigitCount(speeds[i]);
            }

            tempSendingData += "]}";

            server.sendData(tempSendingData); // Replace with your server's actual send function
        }
    }

    

    // Close the connection after sending
    server.closeConnection();

    return 0;
}