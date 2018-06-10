#include "qpcpp.h"
#include "bsp.h"
#include "active_objects.h"
#include <bbahrs/MahonyAHRS.h>
#include <bbahrs/MadgwickAHRS.h>
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

#define SAMPLE_RATE_HZ 100;

// Private methods
static void updateAttitudeBuffer();
static void imu_interrupt_handler();
static void normalizeQ(Q_cxyz &q);
static void print_options();

IMU_MODE quaternion_calculation_mode(SOFTWARE);

//............................................................................
void BSP_init(void) {
	cout	<< "IMU Streaming Data..." << endl
			<< "QP version: " << QP_VERSION_STR << endl			
			<< "Press ESC to quit..." << endl;
	print_options();
}
//............................................................................
void BSP_ledOff(void) {
	rc_led_set(RC_LED_GREEN, 0);    
}
//............................................................................
void BSP_ledOn(void) {
	rc_led_set(RC_LED_GREEN, 1);
}

static rc_mpu_data_t imu_data;
static rc_mpu_config_t imu_config;
static Q_cxyz complementary_filter = {1.0f, 0.0f, 0.0f, 0.0f};
static Server * server;

// < 0 indicates failure.
int BSP_InitProcess() {	
	/*
	if (rc_initialize() < 0) {
		cout << "RC Initialization Failed" << endl;
		return -1;
	}
	*/
	return 0;
}

static void imu_interrupt_handler() {
	updateAttitudeBuffer();
}


// DMP version
void BSP_SetupIMU_DMP() {	
	cout << "Setting up IMU" << endl;
	imu_config = rc_mpu_default_config();
	imu_config.dmp_sample_rate = SAMPLE_RATE_HZ;
	imu_config.enable_magnetometer = 1;	
#ifdef SHOW_WARNINGS
		imu_config.show_warnings = 1;
#endif
	imu_config.orient = ORIENTATION_Y_UP;
	if (rc_mpu_initialize_dmp(&imu_data, imu_config) < 0) {
		fprintf(stderr, "ERROR: can't talk to IMU DMP, all hope is lost\n");
		//return -1;
	}
	if (rc_mpu_set_dmp_callback(&imu_interrupt_handler) < 0) {
		fprintf(stderr, "ERROR: can't setup interrupt handler for imu.\n");
	}
}

void BSP_SetupIMU_Software() {
	sampleFreq = BSP_TICKS_PER_SEC/AHRS_CLOCK_PERIOD_MS;
	beta = beta * 2;
	cout << "Setting up IMU with SOFTWARE configuration" << endl;
	imu_config = rc_mpu_default_config();
	imu_config.enable_magnetometer = 1;
#ifdef SHOW_WARNINGS
	imu_config.show_warnings = 1;
#endif
	imu_config.orient = ORIENTATION_Y_UP;
	if (rc_mpu_initialize(&imu_data, imu_config) < 0 /* zero on success */) {
		cout << "IMU setup failed" << endl;
	} else {
		cout << "IMU Setup done" << endl;
	}	
	if (rc_mpu_set_dmp_callback(&imu_interrupt_handler) < 0) {
		fprintf(stderr, "ERROR: can't setup interrupt handler for imu.\n");
	}	
}

void print_options() {
	cout << "IMU Calculation Mode:" << (quaternion_calculation_mode == DMP ? "DMP" : "SOFTWARE") << "\n";
	cout << "Options:\n";
	cout << "\t\tESC key" << "Exit" << "\n";
	cout << "\t\ts:" << "Stop data print" << "\n";
	cout << "\t\ta:" << "Show Acc" << "\n";
	cout << "\t\tg:" << "Show Gyro" << "\n";
	cout << "\t\tq:" << "Show software computed quaternion" << "\n";
	cout << "\t\td:" << "Show DMP Quaternion" << "\n";
	cout << "\t\tm:" << "Show Magnetogmeter readings" << "\n";
}

void BSP_SetupIMU(){	
	if (quaternion_calculation_mode == DMP) {
		BSP_SetupIMU_DMP();
	}
	else if (quaternion_calculation_mode == SOFTWARE) {
		BSP_SetupIMU_Software();
	}
}

void BSP_Cleanup() {
	rc_mpu_power_off();
	rc_led_cleanup();
	if (server != NULL) {
		server->Shutdown();
	}
}

static void normalizeQ(Q_cxyz &q) {
	float norm = q.c*q.c + q.x*q.x + q.y*q.y + q.z*q.z;
	float norm_root = sqrtf(norm);
	q.c = q.c / norm_root;
	q.x = q.x / norm_root;
	q.y = q.y / norm_root;
	q.z = q.z / norm_root;
}

static void updateAttitudeBuffer() {
	uint64_t timestamp = BSP_nanos(); 
	if (quaternion_calculation_mode == SOFTWARE) {
		float degToRad = 0.0174532925199;
		//for (uint16_t i = 0; i < AHRS_CLOCK_PERIOD_MS; i++) {
			MadgwickAHRSupdate(&complementary_filter,
				imu_data.gyro[0] * degToRad,
				imu_data.gyro[1] * degToRad,
				imu_data.gyro[2] * degToRad,
				imu_data.accel[0],
				imu_data.accel[1],
				imu_data.accel[2],
				imu_data.mag[0],
				imu_data.mag[1],
				imu_data.mag[2]);
		//}

		normalizeQ(complementary_filter);
		attitudeBuffer->put({ complementary_filter.c, complementary_filter.x, complementary_filter.y, complementary_filter.z, timestamp });	
		//cout << "(a)" << flush;
	}
	else if (quaternion_calculation_mode == DMP) {
		float c = imu_data.fused_quat[0];
		float x = imu_data.fused_quat[1];
		float y = imu_data.fused_quat[2];
		float z = imu_data.fused_quat[3];
		float norm = c*c + x*x + y*y + z*z;
		float sqrt_norm = sqrtf(norm);
		c = c / sqrt_norm;
		x = x / sqrt_norm;
		y = y / sqrt_norm;
		z = z / sqrt_norm;		
		attitudeBuffer->put({ c, x, y, z, timestamp });
	}
	
	switch (default_output_mode) {
	case ACC:
		cout << "acc," << imu_data.accel[0] << "," << imu_data.accel[1] << "," << imu_data.accel[2] << endl;		
		break;
	case GYRO:
		cout << "gyro," << imu_data.gyro[0] << "," << imu_data.gyro[1] << "," << imu_data.gyro[2] << endl;
		break;
	case MAG:
		cout << "mag," << imu_data.mag[0] << "," << imu_data.mag[1] << "," << imu_data.mag[2] << endl;
		break;
	case QUAT:
		cout << "orientation," << complementary_filter.c << "," << complementary_filter.x << "," << complementary_filter.y << "," << complementary_filter.z << "," << timestamp << endl;
		break;
	case DMPQUAT:
		cout << "dmp," << imu_data.fused_quat[0]<< "," << imu_data.fused_quat[1] << "," << imu_data.fused_quat[1] << "," << imu_data.fused_quat[2] << "," << timestamp << endl;
		break;
	case STOP:
	default:
		break;
	}
	//cout << "*" << flush;
}

void BSP_PublishAttitude(void) {
	static int mag_skip_counter = 0;
	//cout << "." << flush;
	if (rc_mpu_read_accel(&imu_data) != 0) {
		cout << "can't read acc" << endl;
	}
	if (rc_mpu_read_gyro(&imu_data) != 0) {
		cout << "Can't read gyro" << endl;
	}
	if (mag_skip_counter % 10 == 0) {
		if (rc_mpu_read_mag(&imu_data) != 0) {
			cout << "can't read mag" << endl;
		}		
	}
	updateAttitudeBuffer();
}

void BSP_Toggle_a(void) { default_output_mode = ACC; }
void BSP_Toggle_g(void) { default_output_mode = GYRO; }
void BSP_Toggle_m(void) { default_output_mode = MAG; }
void BSP_Toggle_q(void) { default_output_mode = QUAT; }
void BSP_Toggle_d(void) { default_output_mode = DMPQUAT; }
void BSP_Toggle_s(void) { default_output_mode = STOP; }

OUTPUT_MODE default_output_mode(QUAT);

uint64_t BSP_millis() {
	return BSP_nanos() / 1.0e6;
	/*
	uint64_t		ms; // Milliseconds
	time_t          s;  // Seconds
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	s = spec.tv_sec;
	ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
	if (ms > 999) {
	s++;
	ms = 0;
	}
	return s * 1000 + ms;
	*/
}

uint64_t BSP_nanos() {
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	return spec.tv_sec * 1.0e9 + spec.tv_nsec;
	/*
	time_t s = spec.tv_sec;
	uint64_t nanos = spec.tv_nsec;
	if (nanos > 999999999) {
	s++;
	nanos = 0;
	}
	return (s * 1.0e9 + nanos);
	*/
}
