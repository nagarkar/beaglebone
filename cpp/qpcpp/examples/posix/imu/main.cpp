//****************************************************************************
// Product: Simple Blinky example
// Last Updated for Version: 5.4.0
// Date of the Last Update:  2015-05-04
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
// Web  : https://state-machine.com
// Email: info@state-machine.com
//****************************************************************************
#include "qpcpp.h"
#include "bsp.h"
#include "active_objects.h"
#include "CircularBuffer.h"
#include "Attitude.h"
#include <unistd.h>

using namespace std;

void * runTServer(void * ptr);

void setOptions(int argn, char **argc) {
	int c;
	while ((c = getopt(argn, argc, "s::d::")) != -1) {
		switch (c)
		{
		case 's':
			quaternion_calculation_mode = SOFTWARE;
			break;
		case 'd':
			quaternion_calculation_mode = DMP;
			break;
		default:
			cout << "Unknown option" << endl;
			abort();
		}
	}
}

int main(int argn, char ** argc) {
	//setOptions(argn, argc);
	quaternion_calculation_mode = SOFTWARE;
	// Event queue storage
    static QEvt const *blinkyQSto[10];
	static QEvt const *ahrsQSto[10]; 
	//static QEvt const *tserverQSto[10];

    BSP_init(); // initialize the Board Support Package
    QF::init(); // initialize the framework and the underlying RT kernel

    // publish-subscribe not used, no call to QF::psInit()
    // dynamic event allocation not used, no call to QF::poolInit()

    // instantiate and start the active objects...

	/*
	AO_TServer->start(3U,					// priority (must be unique)
		tserverQSto, Q_DIM(tserverQSto),			// event queue
		(void *)0, 0U);								// stack (unused)
	*/

	AO_Blinky->start(1U,                            // priority (must be unique)
                     blinkyQSto, Q_DIM(blinkyQSto), // event queue
                     (void *)0, 0U);                // stack (unused)

	AO_AHRS->start(2U,								// priority (must be unique)
					ahrsQSto, Q_DIM(ahrsQSto),		// event queue
					(void *)0, 0U);					// stack (unused)

	// Create a thread (https://goo.gl/ybJAeM) to run grpc.
	pthread_t tserver;
	int tserver_ret = pthread_create(&tserver, NULL, runTServer, (void*)attitudeBuffer);
	cout << "Server Started" << tserver_ret << endl;
    return QF::run(); // run the QF application
}

void * runTServer(void * ptr) {
	CircularBuffer<Attitude> * buffer = (CircularBuffer<Attitude> *)ptr;
	TelemetryServiceImpl::RunServer(buffer, true /* wait */);
	return 0;
}
