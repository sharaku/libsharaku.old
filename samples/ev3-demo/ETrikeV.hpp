/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2014-2015 abe takafumi All Rights Reserved.
@file	
@brief	
******************************************************************************/

#include <stdio.h>
#include <sharaku/sm/ev3dev/ev3dev-touch.hpp>
#include <sharaku/sm/ev3dev/ev3dev-gyro.hpp>
#include <sharaku/sm/ev3dev/ev3dev-color.hpp>
#include <sharaku/sm/ev3dev/ev3dev-ultrasonic.hpp>
#include <sharaku/sm/ev3dev/ev3dev-large-motor.hpp>
#include <sharaku/sm/sm-balancer.hpp>
#include <sharaku/sm/sm-motors.hpp>
#include <sharaku/sm/sm-linetrace.hpp>
#include <sharaku/sm/sm-gearretio.hpp>
#include <sharaku/sm/sm-trike.hpp>
#include <sharaku/sm/sm.hpp>

// モジュールの構成と接続関係
//                               +------------- _gear_s - _motor_steer
//                               |           +- _gear_r - _motor_r
// API - _linetrace -+-  _trike -+- _motors -+
//                   |                       +- _gear_l - _motor_l
//                   +----------------------------------- _color
