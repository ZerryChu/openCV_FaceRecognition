#include "stdafx.h"
#include <stdio.h>
#include <WinSock2.h>
#define PORT 8080
#define SERIES "FB0F88715B56" // 改为由加密文件读取

extern int LESSON_ID;

int server();

char* startWorking();

bool attend(int student_id);
