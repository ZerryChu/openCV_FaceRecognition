#include "stdafx.h"
#include "socket.h"


// 改为使用socket发送HTTP请求
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
