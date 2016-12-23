#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#include "phenix/entry/Entry.hpp"

static void error_handler(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    return;
}

//int main(int argc, char ** argv)
FNX_QuickAddEntry
{
    WSAData     wsa_data;
    SOCKET      server_socket;
    SOCKET      client_socket;
    SOCKADDR_IN server_address;
    SOCKADDR_IN client_address;
    int         client_address_size;

    autox argv = args;
    autox argc = args.size();

    char message[] = "hello world";

    if (argc < 2)
    {
        printf("usage: %s <port>\n", argv[0].c_str());
        //exit(1);
        return 0;
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        error_handler("WSAStartup() error.");
    }

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET)
    {
        error_handler("socket() error.");
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1].c_str()));
    if (::bind(server_socket, (SOCKADDR *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        error_handler("bind() error.");
    }

    if (listen(server_socket, 5) == SOCKET_ERROR)
    {
        error_handler("listen() error.");
    }

    client_address_size = sizeof(client_address);
    client_socket = accept(server_socket, (SOCKADDR *)&client_address, &client_address_size);
    if (client_socket == INVALID_SOCKET)
    {
        error_handler("accept() error.");
    }

    send(client_socket, message, sizeof(message), 0);
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    system("pause");

    return 0;
};