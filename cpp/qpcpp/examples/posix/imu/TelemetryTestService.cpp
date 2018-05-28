#include "TelemetryServiceImpl.h"

static CircularBuffer<Attitude> dummy(1);

int main(int argn, char ** args) {	
	TelemetryServiceImpl::RunServer(&dummy, true);
	return 0;
}