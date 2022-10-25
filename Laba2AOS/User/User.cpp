#include <stdio.h>;
#include "winsock2.h";
#include "Ws2tcpip.h";
#include <tchar.h>;
#include <fstream>
#include <iostream>

using namespace std;

#define SOCKET_PORT_NUMBER 60000
char recvbuf[320000] = "";
void main(int argc, char* argv[])
{
	char filename[3200] = "user.txt";
	wchar_t ip[32] = _T("127.0.0.1");
	if (argc == 1)
	{
		filename[0] = '\0';
	}
	else if (argc == 2)
	{
		filename[0] = '\0';
		const size_t cSize = strlen(argv[1]) + 1;
		size_t a = 0;
		mbstowcs_s(&a, ip, argv[1], cSize);
	}
	else if (argc == 3)
	{
		const size_t cSize = strlen(argv[1]) + 1;
		size_t a = 0;
		mbstowcs_s(&a, ip, argv[1], cSize);
		strcpy_s(filename, argv[2]);
	}
	//----------------------
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2),  &wsaData);
	if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n ");
	//----------------------
	// Create a SOCKET for connecting to server
	SOCKET ConnectSocket;
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket() : % ld\n " , WSAGetLastError());
		WSACleanup();
		return;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;


		
	//clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	InetPton(AF_INET, ip, &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(SOCKET_PORT_NUMBER);
	//----------------------
	// Connect to server.
	if (connect(ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService)) == SOCKET_ERROR) {
		printf("Failed to connect.\n " );
		WSACleanup();
		return;
	}
	//----------------------
	// Declare and initialize variables.
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	//char sendbuf[3200] = "dir\ndir\ndir";
	//char sendbuf[3200] = "dir";
	
	//----------------------
	// Send and receive data.

	char command[32000];
	char end[2] = "e";
	//string a;
	//cout << "Chose file(f) or command(c): ";
	//cin >> a;
	if (strlen(filename) != 0)
	{
		fstream file;
		file.open(filename);
		while (!file.eof())
		{
			file >> command;
			if (strlen(command) == 0)
			{
				break;
			}
			bytesSent = send(ConnectSocket, command, strlen(command) + 1, 0);
			if (bytesSent != strlen(command) + 1)
			{
				printf("Error of sending bytes: %ld\n ", bytesSent);
				break;
			}
			printf("Bytes Sent : % ld\n ", bytesSent);
			bytesRecv = recv(ConnectSocket, recvbuf, sizeof(recvbuf), 0);
			if (bytesRecv == 0 || bytesRecv == WSAECONNRESET) {
				printf("Connection Closed.\n ");
				break;
			}
			printf("Bytes Recv : % ld\n ", bytesRecv);
			printf("recvbuf: % s\n ", recvbuf);
		}
	}	
	else
	{
		while (true)
		{
			cout << "Enter command: ";
			cout << "To end press (e)" << endl;
			cin >> command;
			if (strcmp(command, end) == 0)
			{
				break;
			}
			bytesSent = send(ConnectSocket, command, strlen(command) + 1, 0);
			if (bytesSent != strlen(command) + 1)
			{
				printf("Error of sending bytes: %ld\n ", bytesSent);
				break;
			}
			printf("Bytes Sent : % ld\n ", bytesSent);
			bytesRecv = recv(ConnectSocket, recvbuf, sizeof(recvbuf), 0);
			if (bytesRecv == 0 || bytesRecv == WSAECONNRESET) {
				printf("Connection Closed.\n ");
				break;
			}
			printf("Bytes Recv : % ld\n ", bytesRecv);
			printf("recvbuf: % s\n ", recvbuf);
		}
	}
	WSACleanup();
	return;
}