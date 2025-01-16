#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <random>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main() 
{
    int InstanceAmount = 1;

    std::cout << "Enter Amount of instances" << std::endl;
    std::cin >> InstanceAmount;
    std::cout << std::endl;

    std::vector<SOCKET*> Sockets;

    for(int i = 0;i<InstanceAmount;i++)
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "WSAStartup failed!" << std::endl;
            return -1;
        }

        const char* server_ip = "192.168.1.109";
        const int server_port = 12345;

        SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (client_socket == INVALID_SOCKET)
        {
            std::cerr << "Socket creation failed!" << std::endl;
            WSACleanup();
            return -1;
        }

        sockaddr_in server_address{};
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(server_port);
        inet_pton(AF_INET, server_ip, &server_address.sin_addr);

        if (connect(client_socket, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
        {
            std::cerr << "Connection failed!" << std::endl;
            closesocket(client_socket);
            WSACleanup();
            return -1;
        }

        std::cout << "Connected to the server!" << std::endl;

        Sockets.push_back(&client_socket);

        int64_t Numbers = std::rand();
        std::string message = std::to_string(Numbers);

        send(client_socket, (message.append("S")).c_str(), strlen(message.c_str()), 0);
    }

    while (true)
    {
        for (auto& Sok : Sockets)
        {
            fd_set Set = {};
            char buffer[1024];
            int bytes_rec = recv(*Sok, buffer, sizeof(buffer), 0);

            if (bytes_rec > 0)
            {
                std::cout << atoi(buffer) << std::endl;
            }
        }
    }

    /*closesocket(client_socket);
    WSACleanup();*/
    return 0;
}