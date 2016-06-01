#include "handleResult.h"

#define LABEL_SIZE 10

// if new_target = 1 ,reset the num array
int NEW_TARGET = 0;
int PREDICT_TIMES = 0;
int RESULT_LABEL = -1;
int num[LABEL_SIZE];

bool handleResult(int label) {
	PREDICT_TIMES++;
	if (PREDICT_TIMES == 50) {
		NEW_TARGET = 1;
	}
	if (NEW_TARGET == 1) {
		for (int i = 0; i < LABEL_SIZE; i++) {
			num[i] = 0;
		}
		NEW_TARGET = 0;
	}
	num[label]++;
	if (num[label] == 40) {
		return true;
	}
	else
		return false;
}