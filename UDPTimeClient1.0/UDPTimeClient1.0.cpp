#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string.h>

#define TIME_PORT	27015
float avgCalculate(unsigned long* arr);

void main()
{

	// Initialize Winsock (Windows Sockets).

	WSAData wsaData;
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Time Client: Error at WSAStartup()\n";
		return;
	}

	// Client side:
	// Create a socket and connect to an internet address.

	SOCKET connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connSocket)
	{
		cout << "Time Client: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// For a client to communicate on a network, it must connect to a server.

	// Need to assemble the required data for connection in sockaddr structure.

	// Create a sockaddr_in object called server. 
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(TIME_PORT);

	// Send and receive data.

	int bytesSent = 0;
	int bytesRecv = 0;
	char sendBuff[255] = "";
	char recvBuff[255];
	unsigned long  timeSentArr[100];
	unsigned long responseArr[100];
	int val = 1;

	char choice[255];
	char cityTime[255];
	bool finish = false;
	bool isCalc = false;
	while (!finish)
	{
		bool isValid = true;
		for (int i = 0; i < val; i++)
		{
			if (i == 0)
			{
				cout << "Please enter number of request: " << endl;
				cout << "1. Get Time " << endl;
				cout << "2. Get Time Without Date  " << endl;
				cout << "3. Get Time Since Epoch " << endl;
				cout << "4. Get Client To Server DelayEstimation " << endl;
				cout << "5. MeasureRTT " << endl;
				cout << "6. GetTimeWithoutDateOrSeconds " << endl;
				cout << "7. GetYear " << endl;
				cout << "8. Get Month and Year " << endl;
				cout << "9. Get Seconds Since Beginning of Month " << endl;
				cout << "10. Get Week of Year " << endl;
				cout << "11. Get DayLight Savings " << endl;
				cout << "12. Get Time Without Date in City " << endl;
				cout << "13. MeasureTimeLap " << endl;
				cout << "14. Exit " << endl;

				cin >> choice;
			}
				if (strcmp(choice, "1") == 0)
				{
					strcpy_s(sendBuff, "1");
					val = 1;
				}
				else if (strcmp(choice, "2") == 0)
				{
					strcpy_s(sendBuff, "2");
					val = 1;
				}
				else if (strcmp(choice, "3") == 0)
				{
					strcpy_s(sendBuff, "3");
					val = 1;
				}
				else if (strcmp(choice, "4") == 0 || strcmp(choice, "5") == 0)
				{
					val = 100;
					timeSentArr[i] = GetTickCount();
					strcpy_s(sendBuff, "1");
				}
				else if (strcmp(choice, "6") == 0)
				{
					strcpy_s(sendBuff, "6");
					val = 1;
				}
				else if (strcmp(choice, "7") == 0)
				{
					strcpy_s(sendBuff, "7");
					val = 1;
				}
				else if (strcmp(choice, "8") == 0)
				{
					strcpy_s(sendBuff, "8");
					val = 1;
				}
				else if (strcmp(choice, "9") == 0)
				{
					strcpy_s(sendBuff, "9");
					val = 1;
				}
				else if (strcmp(choice, "10") == 0)
				{
					strcpy_s(sendBuff, "10");
					val = 1;
				}
				else if (strcmp(choice, "11") == 0)
				{
					strcpy_s(sendBuff, "11");
					val = 1;
				}
				else if (strcmp(choice, "12") == 0)
				{
					val = 1;
					strcpy_s(sendBuff, "12");
					cout << "Please Choose City Number According The Menu:  " << endl;
					cout << "1. Tokyo (Japan) " << endl;
					cout << "2. Melbourne (Australia) " << endl;
					cout << "3. San-Francisco (USA)" << endl;
					cout << "4. Porto (Portugal)" << endl;
					cin >> cityTime;
					if (strcmp(cityTime, "1") == 0)
					{
						strcat_s(sendBuff, " 1");
					}
					else if (strcmp(cityTime, "2") == 0)
					{
						strcat_s(sendBuff, " 2");
					}
					else if (strcmp(cityTime, "3") == 0)
					{
						strcat_s(sendBuff, " 3");
					}
					else if (strcmp(cityTime, "4") == 0)
					{
						strcat_s(sendBuff, " 4");
					}
					else
					{
						strcat_s(sendBuff, " 5");
					}
				}
				else if (strcmp(choice, "13") == 0)
				{
					val = 1;
					strcpy_s(sendBuff, "13");
				}
				else if (strcmp(choice, "14") == 0)
				{
					val = 1;
					finish = true;
					break;
				}
			
			else
			{
				val = 1;
				cout << "Not valid key!" << endl;
				isValid = false;
			}
			if (isValid && !isCalc && !finish)
			{
				bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
				if (SOCKET_ERROR == bytesSent)
				{
					cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}
				if (i == 0)
					cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";
				if (strcmp(choice, "4") != 0)
				{
					bytesRecv = recv(connSocket, recvBuff, 255, 0);
					if (strcmp(choice, "5") == 0)
						responseArr[i] = GetTickCount() - timeSentArr[i];
					if (SOCKET_ERROR == bytesRecv)
					{
						cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
						closesocket(connSocket);
						WSACleanup();
						return;
					}
					recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
					if (i == 99 && strcmp(choice, "5") == 0)
					{
						float avrg = avgCalculate(responseArr);
						cout << "The average Round Trip Round (RTT) is:  " << avrg << " Miliseconds \n";
						isCalc = false;
					}
					
				}
			}
		}
		if (strcmp(choice, "4") == 0 && isValid)
		{
			for (int i = 0; i < val && !finish && isValid; i++)
			{
				// Gets the server's answer using simple recieve (no need to hold the server's address).
					bytesRecv = recv(connSocket, recvBuff, 255, 0);
					responseArr[i] = GetTickCount() - timeSentArr[i];
					if (SOCKET_ERROR == bytesRecv)
					{
						cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
						closesocket(connSocket);
						WSACleanup();
						return;
					}
					recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
			}
			isCalc = false;
			float avrg = avgCalculate(responseArr);
			cout << "The average delay between client to server is:  " << avrg << " Milliseconds \n";
		}
		else if(!finish && strcmp(choice, "4") != 0 && strcmp(choice, "5") != 0 && isValid)
			cout << "Time Client: Received: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n" << endl;
	}
}
float avgCalculate(unsigned long* arr)
{
	int sum = 0, i;
	for (i = 0; i < 100; i++)
		sum += arr[i];
	return sum / 100;
}
