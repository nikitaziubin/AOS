#include <stdio.h>;
#include "winsock2.h";
#include "Ws2tcpip.h";
#include <tchar.h>;
#include <fstream>
#include <iostream>

using namespace std;


char recvbuf[320000] = "";
void main() 
{
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
	InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(60000);
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
	char command1[320];
	
	fstream file;
	file.open("user.txt");
	
	while (!file.eof())
	{
		file >> command;
		//strcat_s(command, command1);
		//strcat_s(command, ">>C:/tmp/file2004.log 2>>&1\n");
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
	
	
	
	/*printf("Bytes Sent : % ld\n " , bytesSent);
	while (bytesRecv == SOCKET_ERROR) {
		bytesRecv = recv(ConnectSocket, recvbuf, 32, 0);
		if (bytesRecv == 0 || bytesRecv == WSAECONNRESET) {
			printf("Connection Closed.\n ");
			break;
		}
		printf("Bytes Recv : % ld\n " , bytesRecv);
	}*/
	WSACleanup();
	return;
}