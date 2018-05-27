//****************************************************************************
// Product: Simple Blinky example, POSIX
// Last updated for version 5.6.0
// Last updated on  2015-12-26
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Contact information:
// https://state-machine.com
// mailto:info@state-machine.com
//****************************************************************************
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

void BSP_SetupIMU() {
	cout << "Setting up IMU" << endl;
	imu_config = rc_default_imu_config();
	imu_config.enable_magnetometer = 1;
	imu_config.show_warnings = 1;
	rc_initialize_imu(&imu_data, imu_config);
	cout << "IMU Setup done" << endl;
}

static void normalizeQ(Q_cxyz &q) {
	float norm = q.c*q.c + q.x*q.x + q.y*q.y + q.z*q.z;
	q.c = q.c / sqrtf(norm);
	q.x = q.x / sqrtf(norm);
	q.y = q.y / sqrtf(norm);
	q.z = q.z / sqrtf(norm);
}

void BSP_PublishAttitude(void) {	
	//cout << "Publishing Attitude" << endl;
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
	
	if (counter % 100 == 0) {
		counter = 0;
		/*cout << "AHRS:" << complementary_filter.c << "," << complementary_filter.x << "," << complementary_filter.y << "," << complementary_filter.z << "," << endl;
		cout << "Acc:" << imu_data.accel[0] << "," << imu_data.accel[1] << ", " << imu_data.accel[2] << endl;
		cout << "Gyr:" << imu_data.gyro[0] << "," << imu_data.gyro[1] << ", " << imu_data.gyro[2] << endl;
		cout << "Mag:" << imu_data.mag[0] << "," << imu_data.mag[1] << ", " << imu_data.mag[2] << endl;*/
	}
	counter++;
	//cout << "Publishing Attitude Done" << endl;
	//cout << "*" << flush;
}
