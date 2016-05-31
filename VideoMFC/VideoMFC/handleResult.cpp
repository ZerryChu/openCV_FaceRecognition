#include "stdafx.h"
#include "handleResult.h"
#include "detect_recog.h"

#define LABEL_SIZE 10

// if new_target = 1 ,reset the num array
int NEW_TARGET = 0;
int PREDICT_TIMES = 0;
int RESULT_LABEL = -1;
int num[LABEL_SIZE];

int handleResult(int label) {
	PREDICT_TIMES++;
	if (PREDICT_TIMES == 30) {
		NEW_TARGET = 1;
		PREDICT_TIMES = 0;
		// no found, check again
		return -1;
	}
	if (NEW_TARGET == 1) {
		for (int i = 0; i < LABEL_SIZE; i++) {
			num[i] = 0;
		}
		NEW_TARGET = 0;
	}
	num[label]++;
	if (num[label] == 28) {
		NEW_TARGET = 1;
		PREDICT_TIMES = 0;
		// result get
		return true;
	}
	else
		// checking...
		return 0;
}

//Timer() 设定两秒定时器