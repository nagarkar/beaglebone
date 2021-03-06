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
#ifndef bsp_h
#define bsp_h

uint32_t const QP_CLOCK_PERIOD_MS			= static_cast<uint32_t>(1);
uint32_t const BSP_TICKS_PER_SEC			= QP_CLOCK_PERIOD_MS * 1000;	/* 1 sec = 1000 ms */

uint32_t const AHRS_CLOCK_PERIOD_MS			= static_cast<uint32_t>(100);	/* We want to measure IMU every 10 ms*/
uint32_t const AHRS_TICKS					= AHRS_CLOCK_PERIOD_MS/QP_CLOCK_PERIOD_MS;

uint32_t const BLINKY_CLOCK_PERIOD_MS		= static_cast<uint32_t>(1000);	/* We want to blink every sec */
uint32_t const BLINKY_TICKS					= BLINKY_CLOCK_PERIOD_MS / QP_CLOCK_PERIOD_MS;


typedef enum {
	DMP,
	SOFTWARE
} IMU_MODE;

extern IMU_MODE quaternion_calculation_mode;

typedef enum {
	ACC,
	GYRO,
	MAG,
	QUAT,
	DMPQUAT,
	STOP
} OUTPUT_MODE;

extern OUTPUT_MODE default_output_mode;

void BSP_init(void);
void BSP_ledOff(void);
void BSP_ledOn(void);
void BSP_PublishAttitude(void);
void BSP_SetupIMU(void);
void BSP_Cleanup(void);

void BSP_Toggle_a(void);
void BSP_Toggle_g(void);
void BSP_Toggle_m(void);
void BSP_Toggle_q(void);
void BSP_Toggle_d(void);
void BSP_Toggle_s(void);

uint64_t BSP_millis(void);
uint64_t BSP_nanos(void);


#endif // bsp_h
