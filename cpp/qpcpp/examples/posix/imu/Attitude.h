#pragma once
typedef struct _Attitude {
	float Qc;
	float Qx;
	float Qy;
	float Qz;
	long timestamp;
} Attitude;