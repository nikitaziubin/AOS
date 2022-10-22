#include <stdio.h>;
#include "winsock2.h";
#include "Ws2tcpip.h";
#include <tchar.h>;
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
	char sendbuf[3200] = "dir";
	char recvbuf[32] = "";
	//----------------------
	// Send and receive data.
	bytesSent = send(ConnectSocket, sendbuf, strlen(sendbuf) + 1, 0);
	printf("Bytes Sent : % ld\n " , bytesSent);
	while (bytesRecv == SOCKET_ERROR) {
		bytesRecv = recv(ConnectSocket, recvbuf, 32, 0);
		if (bytesRecv == 0 || bytesRecv == WSAECONNRESET) {
			printf("Connection Closed.\n ");
			break;
		}
		printf("Bytes Recv : % ld\n " , bytesRecv);
	}
	WSACleanup();
	return;
}