#include "stdafx.h"
#include "socket.h"

int server() {
     // server

    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD( 1, 1 );
    int res = WSAStartup( wVersionRequested, &wsaData );
    if ( res != 0 ) {
        return -1;
    }
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN addr;
    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    bind(sock,(SOCKADDR*)&addr,sizeof(SOCKADDR));
    listen(sock, 5);

    int len = sizeof(SOCKADDR);
    SOCKADDR_IN client;
    SOCKET conn;
    //int pos = 0;
    while(1) {
        // printf("%d\n",pos++);
        conn = accept(sock, (SOCKADDR*)&client, &len);
		char recvBuf[2048]; // 2M
		recv(conn, recvBuf, 10, 0);
		//printf("%s\n", recvBuf);
        char sendBuf[10];
        sprintf(sendBuf, "receive succeeded.", inet_ntoa(client.sin_addr));
        send(conn, sendBuf, strlen(sendBuf) + 1, 0);
		//char *name = "jie jue fang an";
		//client(id, name);
        closesocket(conn);
    }
    WSACleanup();
    return 0;
}
