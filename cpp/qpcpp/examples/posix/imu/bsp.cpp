#include "qpcpp.h"
#include "bsp.h"
#include "active_objects.h"
#include <ahrs/MahonyAHRS.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>      // for memcpy() and memset()
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>
#include <telemetry_service.grpc.pb.h>
#include "TelemetryServiceImpl.h"
#include "Attitude.h"
#include "CircularBuffer.h"

// This extern "C" is required because roboticscape.h does not wrap it's declarations in extern "C"
extern "C"
{
	#include <roboticscape.h>
}

using namespace std;
using namespace telemetry;

Q_DEFINE_THIS_FILE

#ifdef Q_SPY
    #error Simple Blinky Application does not provide Spy build configuration
#endif

//............................................................................
void BSP_init(void) {
    cout << "Simple Blinky example" << endl
         << "QP version: " << QP_VERSION_STR << endl
         << "Press ESC to quit..." << endl;
}
//............................................................................
void BSP_ledOff(void) {
	rc_set_led(GREEN, 0);    
}
//............................................................................
void BSP_ledOn(void) {
	rc_set_led(GREEN, 1);
}

static rc_imu_data_t imu_data;
static rc_imu_config_t imu_config;
static Q_cxyz complementary_filter = {1.0f, 0.0f, 0.0f, 0.0f};
static Server * server;

void BSP_SetupIMU() {
	cout << "Setting up IMU" << endl;
	imu_config = rc_default_imu_config();
	imu_config.enable_magnetometer = 1;
	imu_config.show_warnings = 1;
	rc_initialize_imu(&imu_data, imu_config);
	cout << "IMU Setup done" << endl;	
}

void BSP_IMUCleanup() {
	if (server != NULL) {
		server->Shutdown();
	}
}

static void normalizeQ(Q_cxyz &q) {
	float norm = q.c*q.c + q.x*q.x + q.y*q.y + q.z*q.z;
	q.c = q.c / sqrtf(norm);
	q.x = q.x / sqrtf(norm);
	q.y = q.y / sqrtf(norm);
	q.z = q.z / sqrtf(norm);
}

void BSP_PublishAttitude(void) {	
	//cout << "." << flush;
	static int counter = 0;
	if (rc_read_accel_data(&imu_data) < 0) {
		cout << "can't read acc" << endl;
	}
	if (rc_read_gyro_data(&imu_data) < 0) {
		cout << "Can't read gyro" << endl;
	}
	if (counter % 10 == 0) {		
		if (rc_read_mag_data(&imu_data) < 0) {
			cout << "can't read mag" << endl;
		}
	}
	
	float degToRad = 0.0174532925199;
	MahonyAHRSupdate(&complementary_filter,
		imu_data.gyro[0] * degToRad,
		imu_data.gyro[1] * degToRad,
		imu_data.gyro[2] * degToRad,
		imu_data.accel[0],
		imu_data.accel[1],
		imu_data.accel[2],
		imu_data.mag[0],
		imu_data.mag[1],
		imu_data.mag[2]);
	
	normalizeQ(complementary_filter);
	
	attitudeBuffer->put({ complementary_filter.c, complementary_filter.x, complementary_filter.y, complementary_filter.z, 0 });

	if (counter % 100 == 0) {
		counter = 0;		
		cout << "AHRS:" << complementary_filter.c << "," << complementary_filter.x << "," << complementary_filter.y << "," << complementary_filter.z << "," << endl;
		cout << "AttitudeBuffer Size: " << attitudeBuffer->currentSize() << endl;
		cout << "AttitudeBuffer isEmpty: " << attitudeBuffer->empty() << endl;
		cout << "AttitudeBuffer isFull: " << attitudeBuffer->full() << endl;
		Attitude att = attitudeBuffer->peek();
		cout << "AttitudeBuffer top: Qc:" << att.Qc << ",Qx:" << att.Qx << endl;
		/*
		cout << "Acc:" << imu_data.accel[0] << "," << imu_data.accel[1] << ", " << imu_data.accel[2] << endl;
		cout << "Gyr:" << imu_data.gyro[0] << "," << imu_data.gyro[1] << ", " << imu_data.gyro[2] << endl;
		cout << "Mag:" << imu_data.mag[0] << "," << imu_data.mag[1] << ", " << imu_data.mag[2] << endl;*/
	}
	
	counter++;
	//cout << "*" << flush;
}
