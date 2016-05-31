#include "stdafx.h"
#include "socket.h"
#include <string.h>
#include <fstream>
using namespace std;
//#define savePath "C:\\Users\\Administrator\\Desktop\\openCV_FaceRecognition-master\\FaceRecognition\\VideoMFC\\sourcePic\\result.jpg"

int LESSON_ID = -1;

int client() {
    //client
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD( 1, 1 );
    int res = WSAStartup( wVersionRequested, &wsaData );
    if ( res != 0 ) {
        return -1;
    }
    SOCKADDR_IN addr;
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //自环
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    //while(1) {
        //Sleep(1000);
        SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
        connect(sockClient, (SOCKADDR*)&addr, sizeof(SOCKADDR));
        send(sockClient, "hello", strlen("hello")+1, 0);
        char recvBuf[50];
        recv(sockClient, recvBuf, 50, 0);
		//if (strcmp(recvBuf, "return msg") {
		//	return -1;
		//}
        closesocket(sockClient);
   // }
    WSACleanup();
}

/*
char* startWorking() {
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1, 1);
	int res = WSAStartup(wVersionRequested, &wsaData);
	if (res != 0) {
		return "";
		//return LESSON_ID;
	}	
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(sockClient, (SOCKADDR*)&addr, sizeof(SOCKADDR)) < 0) {
		// conn errors
		return "1";
	}
	char buffersend[] = "GET /presence/attendence/attend?student_id=1&lesson_id=1&series=FB0F88715B56 HTTP/1.1\r\nHost:localhost\r\nAccept-Charset:unicode\r\nAccept-Language:zh-cn\r\n\r\n";
	AfxMessageBox(buffersend);
	send(sockClient, buffersend, strlen(buffersend), 0);
	char recvBuf[1024];
	//memset(recvBuf, 0, sizeof(recvBuf));
	int num = recv(sockClient, recvBuf, 1024, 0);
	if (num > 0) {
		//LESSON_ID = atoi(recvBuf);
	}
	else
		return "2";
	AfxMessageBox(recvBuf);
	closesocket(sockClient);
	WSACleanup();
	return recvBuf;
	//return LESSON_ID;
}
*/

// 签到 利用socket发送http报文向服务器提出请求
bool attend(int student_id) {
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1, 1);
	int res = WSAStartup(wVersionRequested, &wsaData);
	if (res != 0) {
		return false;
	}
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(sockClient, (SOCKADDR*)&addr, sizeof(SOCKADDR)) < 0) {
		// conn errors
		return false;
	}

	// construct a http resquest
	char* buffersend = "GET /presence/attendence/attend?student_id=";
	char* student_idInChar;
	buffersend = strcat(buffersend, itoa(student_id, student_idInChar, 10));
	buffersend = strcat(buffersend, "&series=");
	buffersend = strcat(buffersend, SERIES);
	buffersend = strcat(buffersend, " HTTP / 1.1\r\nHost:localhost\r\nAccept - Charset:unicode\r\nAccept - Language:zh - cn\r\n\r\n");
	AfxMessageBox(buffersend);
	send(sockClient, buffersend, strlen(buffersend), 0);
	//	no recv
	closesocket(sockClient);
	WSACleanup();
	return true;
	//return LESSON_ID;
}