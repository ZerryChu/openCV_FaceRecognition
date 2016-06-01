#include "stdafx.h"
#include "socket.h"
#include <fstream>
#include "detect_recog.h"
#include "map_and_path.h"
using namespace std;
#define savePath "C:\\Users\\Administrator\\Desktop\\openCV_FaceRecognition-master\\FaceRecognition\\VideoMFC\\sourcePic\\target.jpg"

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
	parseXML();
    //int pos = 0;
    while(1) {
        // printf("%d\n",pos++);
        conn = accept(sock, (SOCKADDR*)&client, &len);
		char recvBuf[1024]; // 1M
		fstream rfile;
		fstream wfile;
		wfile.open(savePath, ios::out | ios::binary | ios::trunc);
		while (recv(conn, recvBuf, 10, 0) > 0) {
			memset(recvBuf, 0, sizeof(recvBuf));
			wfile.write(recvBuf, sizeof(recvBuf));
		}
		//printf("%s\n", recvBuf);
        char sendBuf[10];
        //sprintf(sendBuf, "receive succeeded.", inet_ntoa(client.sin_addr));
		int number = recog_and_draw2(SOURCEPIC);
		string target_name = mp[number].name.c_str();
		sprintf(sendBuf, target_name.c_str(), inet_ntoa(client.sin_addr));
        send(conn, sendBuf, strlen(sendBuf) + 1, 0);
		//client(id, name);
        closesocket(conn);
    }
    WSACleanup();
    return 0;
}
