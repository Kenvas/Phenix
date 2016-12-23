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
    SOCKADDR_IN server_address;

    autox argv = args;
    autox argc = args.size();

    char message[32];
    int string_length = -1;

    if (argc < 3)
    {
        printf("usage: %s <IP> <port>\n", argv[0].c_str());
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
    server_address.sin_addr.s_addr = inet_addr(argv[1].c_str());
    server_address.sin_port = htons(atoi(argv[2].c_str()));

    if (connect(server_socket, (SOCKADDR *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        error_handler("connect() error.");
    }

    string_length = recv(server_socket, message, sizeof(message) - 1, 0);
    if (string_length < 0)
    {
        error_handler("read() error.");
    }

    printf("message from server: %s \n", message);

    closesocket(server_socket);
    WSACleanup();

    system("pause");
    return 0;
};
