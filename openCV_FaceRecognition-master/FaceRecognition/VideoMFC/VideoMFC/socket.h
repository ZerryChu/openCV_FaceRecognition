#include "stdafx.h"
#include <stdio.h>
#include <WinSock2.h>
#define PORT 8080
#define SERIES "FB0F88715B56" // ��Ϊ�ɼ����ļ���ȡ

extern int LESSON_ID;

int server();

char* startWorking();

bool attend(int student_id);
