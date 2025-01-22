#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <random>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    std::vector<SOCKET*> Sockets;

    while (true)
    {
        for (int i = 0;i<300 ;i++)
        {
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            {
                std::cerr << "WSAStartup failed!" << std::endl;
                return -1;
            }

            const char* server_ip = "192.168.1.106";
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

            SOCKET* NewSock = new SOCKET(client_socket);

            Sockets.push_back(NewSock);
        }

        for (SOCKET* Sok : Sockets)
        {
            int64_t Numbers = std::rand();
            std::string message = std::to_string(Numbers);

            send(*Sok, (message.append("S")).c_str(), strlen(message.c_str()), 0);

            char buffer[1024];
            int bytes_rec = recv(*Sok, buffer, sizeof(buffer), 0);

            std::string Rec = buffer;
            Rec.resize(bytes_rec);

            if (bytes_rec > 0)
            {
                std::cout << Rec << std::endl;
                closesocket(*Sok);
                WSACleanup();
            }
        }

        for (SOCKET* Sok : Sockets)
        {
            delete Sok;
            Sok = nullptr;
        }

        Sockets.clear();
    }

    return 0;
}