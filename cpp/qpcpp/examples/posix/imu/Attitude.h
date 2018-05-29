#pragma once
#include<cinttypes>

typedef struct _Attitude {
	float Qc;
	float Qx;
	float Qy;
	float Qz;
	uint64_t timestamp;
} Attitude;