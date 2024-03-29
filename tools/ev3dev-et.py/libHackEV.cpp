/* --
 *
 * MIT License
 * 
 * Copyright (c) 2016 Abe Takafumi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <pthread.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <sys/resource.h>
#include <sharaku/sched.h>
#include <sharaku/system.h>
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include "libHackEV.hpp"

using namespace std;

static const char libhackev_version[] = "1.0";

static pthread_t	_th;
static job_t		_job_init;
static job_t		_job_exit;
static HackEV		_hackev;

//////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------
// メイン制御
// ---------------------------------------------------------------------
static void job_init(job_t *job)
{
	// HackEVを開始する
	_hackev.start();
}

static void job_exit_end(job_t *job)
{
	// 終了する。
	// これにより、sharaku_entryが終了する。
	sharaku_exit_message(0);
}

static void job_exit(job_t *job)
{
	// HackEVを終了する
	_hackev.stop();

	// スケジュールされているjobをキャンセルする
	job_cancel_sched(&_job_init);

	// 終了するまで100ms待つ。その間にモータ等を終了させる
	job_timer_sched(&_job_exit, 100, job_exit_end);
}


static void* start_routine(void *arg)
{
	init_job(&_job_init);
	job_async_sched(&_job_init, job_init);
	sharaku_entry();
	return NULL;
}

// ---------------------------------------------------------------------
// Global
// ---------------------------------------------------------------------
static int32_t initialize(void)
{
	pthread_create(&_th, NULL, start_routine, NULL);

	struct sched_param	param;
	param.sched_priority = 1;
	pthread_setschedparam(_th, SCHED_RR, &param);

	sleep(3);
	return 0;
}

static int32_t finalize(void)
{
	init_job(&_job_exit);
	job_async_sched(&_job_exit, job_exit);
	pthread_join(_th, NULL);
	return 0;
}

static void version(void)
{
	printf("%s\n", libhackev_version);
	return;
}

// ---------------------------------------------------------------------
// Sensors
// ---------------------------------------------------------------------
namespace Gyro {

static int32_t SetZeroAngle(int32_t angle)
{
	return _hackev.gyro.set_offset(angle);
}

static int32_t GetAngle(void)
{
	return _hackev.gyro.get_angle();
}

static int32_t GetRate(void)
{
	return _hackev.gyro.get_rate();
}

static int32_t Reset(void)
{
	return _hackev.gyro.reset();
}

};
// ---------------------------------------------------------------------
namespace Color {

static int32_t SetMode(std::string mode)
{
	if (mode == "REFLECTED") {
		return _hackev.color.set_mode(sharaku::color_operations::MODE_REFLECTED);
	} else if (mode == "AMBIENT") {
		return _hackev.color.set_mode(sharaku::color_operations::MODE_AMBIENT);
	} else if (mode == "CORRECTION") {
		return _hackev.color.set_mode(sharaku::color_operations::MODE_CORRECTION);
	} else if (mode == "FULLCOLOR") {
		return _hackev.color.set_mode(sharaku::color_operations::MODE_FULLCOLOR);
	} else {
		return ENOTSUP;
	}
	return ENOTSUP;
}

static int32_t GetValue(void)
{
	return _hackev.color.get_value();
}

static int32_t GetRed(void)
{
	return _hackev.color.get_red();
}

static int32_t GetGreen(void)
{
	return _hackev.color.get_green();
}

static int32_t GetBlue(void)
{
	return _hackev.color.get_blue();
}

};
// ---------------------------------------------------------------------
namespace USonic {

static int32_t GetMM(void)
{
	_hackev.usonic.set_unit(sharaku::usonic_operations::USONIC_MM);
	return _hackev.usonic.get_dist();
}

static int32_t GetInch(void)
{
	_hackev.usonic.set_unit(sharaku::usonic_operations::USONIC_IN);
	return _hackev.usonic.get_dist();
}

};
// ---------------------------------------------------------------------
namespace Touch {

static bool IsPress(void)
{
	return _hackev.touch.is_press() != 0;
}

static bool IsPush(void)
{
	return _hackev.touch.is_push() != 0;
}

static bool IsRelease(void)
{
	return _hackev.touch.is_release() != 0;
}

};
// ---------------------------------------------------------------------
namespace Arm {

static int32_t _target_deg = 0;

static int32_t GetDegree(void)
{
	return _hackev.motor_a.get_position();
}

static int32_t SetDegree(int32_t deg)
{
	_target_deg = deg;
	return _hackev.motor_a.set_position_sp(deg);
}

static int32_t WaitDegree(void)
{
	int32_t now_deg;
	const struct timespec sleep_time = {0, 100000};
	int32_t count = 0;

	// 指定位置に来るまで待つ。
	// 動作上、指定位置に来るはずであるが、パワー不足や障害物に引っかかって
	// 目標に到達できない可能性がある。
	for (;;) {
		now_deg = GetDegree();
		if (now_deg == _target_deg) {
			count ++;
			if (count > 10) {
				break;
			}
		} else {
			count = 0;
		}
		nanosleep(&sleep_time, NULL);
	}

	return now_deg;
}

};
// ---------------------------------------------------------------------
namespace Tail {

static int32_t _target_deg = 0;

static int32_t GetDegree(void)
{
	return _hackev.motor_t.get_position();
}

static int32_t SetDegree(int32_t deg)
{
	_target_deg = deg;
	return _hackev.motor_t.set_position_sp(deg);
}

static int32_t WaitDegree(void)
{
	int32_t now_deg;
	const struct timespec sleep_time = {0, 100000};
	int32_t count = 0;

	// 指定位置に来るまで待つ。
	// 動作上、指定位置に来るはずであるが、パワー不足や障害物に引っかかって
	// 目標に到達できない可能性がある。
	for (;;) {
		now_deg = GetDegree();
		if (now_deg == _target_deg) {
			count ++;
			if (count > 10) {
				break;
			}
		} else {
			count = 0;
		}
		nanosleep(&sleep_time, NULL);
	}

	return now_deg;
}

};
// ---------------------------------------------------------------------
namespace Motors {

static int32_t SetAutoCorrectionOnOff(bool onoff)
{
	if (onoff) {
		_hackev.motors.auto_correction_on();
	} else {
		_hackev.motors.auto_correction_off();
	}
	return 0;
}

static int32_t SetMotorsRatio(float ratio_l, float ratio_r)
{
	_hackev.gear_motor_r.set_gear_ratio(ratio_r);
	_hackev.gear_motor_l.set_gear_ratio(ratio_l);
	return 0;
}

static int32_t SetFwdSpeed(int32_t speed)
{
	_hackev.target_move.set_speed_sp(speed);
	return 0;
}

static int32_t GetFwdSpeed(void)
{
	return _hackev.target_move.get_speed_sp();
}

static int32_t SetSteerDegree(int32_t steer)
{
	_hackev.target_move.set_steer_sp(steer);
	return 0;
}

static int32_t GetSteerDegree(void)
{
	return _hackev.target_move.get_steer_sp();
}

static int32_t SetStopMode(std::string mode)
{
	if (mode == "HOLD") {
		_hackev.motor_r.set_stop_mode(sharaku::motor_ctrl_operations::STOPMODE_HOLD);
		_hackev.motor_l.set_stop_mode(sharaku::motor_ctrl_operations::STOPMODE_HOLD);
	} else if (mode == "COAST") {
		_hackev.motor_r.set_stop_mode(sharaku::motor_ctrl_operations::STOPMODE_COAST);
		_hackev.motor_l.set_stop_mode(sharaku::motor_ctrl_operations::STOPMODE_COAST);
	} else if (mode == "BRAKE") {
		_hackev.motor_r.set_stop_mode(sharaku::motor_ctrl_operations::STOPMODE_BRAKE);
		_hackev.motor_l.set_stop_mode(sharaku::motor_ctrl_operations::STOPMODE_BRAKE);
	}
	return 0;
}

};
// ---------------------------------------------------------------------
namespace Odo {
static int32_t _start_dist = 0;
static int32_t _target_dist = 0;

static int32_t GetDistance(void)
{
	return _hackev.odo.get_dist();
}

static int32_t GetPositionX(void)
{
	position3 pos;
	pos = _hackev.odo.get_position();
	return pos.x;
}

static int32_t GetPositionY(void)
{
	position3 pos;
	pos = _hackev.odo.get_position();
	return pos.y;
}

static int32_t GetPositionRot(void)
{
	rotation3 rot;
	rot = _hackev.odo.get_rotation();
	return rot.z;
}

static int32_t SetDistance(int32_t distance)
{
	_start_dist = GetDistance();
	_target_dist = distance;
	return _hackev.odo.set_dist(distance);
}

static int32_t SetRotation(int32_t deg)
{
	rotation3 rot;
	rot(0.0f, 0.0f, deg);
	return _hackev.odo.set_rotation(rot);
}

static int32_t SetPosition(float x, float y)
{
	position3 pos;
	pos(x, y, 0.0f);
	return _hackev.odo.set_position(pos);
}

static int32_t WaitDistance(void)
{
	int32_t now_dist;

	for (;;) {
		now_dist = GetDistance();
		if (_start_dist > _target_dist) {
			if (now_dist <= _target_dist) {
				break;
			}
		} else {
			if (now_dist >= _target_dist) {
				break;
			}
		}
	}
	return 0;
}

};
// ---------------------------------------------------------------------
namespace Linetrace {

static int32_t CalibrationWhite(uint32_t value)
{
	_hackev.linetrace.set_white(value);
	return 0;
}

static int32_t CalibrationBlack(uint32_t value)
{
	_hackev.linetrace.set_black(value);
	return 0;
}

static int32_t SetOnOff(bool onoff)
{
	if (onoff) {
		_hackev.linetrace.trace_on();
	} else {
		_hackev.linetrace.trace_off();
	}
	return 0;
}

static int32_t SetEdge(uint32_t edge)
{
	sharaku::sd_linetrace::edge	e;
	if (edge) {
		e = sharaku::sd_linetrace::LINETRACE_EDGE_RIGHT;
	} else {
		e = sharaku::sd_linetrace::LINETRACE_EDGE_LEFT;
	}
	_hackev.linetrace.set_edge(e);
	return 0;
}

static int32_t SetParam(float Kp, float Ki, float Kd, float q, int in_angle, int out_angle)
{
	float lt_q = _hackev.linetrace.get_lowpass();
	if (lt_q != q) {
		// 更新がある場合のみ設定する。
		_hackev.linetrace.set_lowpass(q);
	}
	float lt_Kp, lt_Ki, lt_Kd;
	_hackev.linetrace.get_trace_pid(lt_Kp, lt_Ki, lt_Kd);
	if (lt_Kp != Kp || lt_Ki != Ki || lt_Kd != Kd) {
		// 更新がある場合のみ設定する。
		// リセットの必要があればリセットする
		_hackev.linetrace.set_trace_pid(Kp, Ki, Kd);
		_hackev.linetrace.clear_pid();
	}
	// 旋回角度だけは常に更新する。
	_hackev.linetrace.set_turn_angle(in_angle, out_angle);
	return 0;
}

static int32_t GetUncertainTime(void)
{
	return _hackev.linetrace.get_uncertain_time();
}

};
// ---------------------------------------------------------------------
namespace TargetMove {

static int32_t SetOnOff(bool onoff)
{
	if (onoff) {
		_hackev.target_move.move_on();
	} else {
		_hackev.target_move.move_off();
	}
	return 0;
}

static int32_t SetAutoSpeedOnOff(bool onoff)
{
	if (onoff) {
		_hackev.target_move.set_autospeed_on();
	} else {
		_hackev.target_move.set_autospeed_off();
	}
	return 0;
}

static int32_t SetSpeedMinMax(int min_speed, int max_speed)
{
	_hackev.target_move.set_limit_speed(min_speed, max_speed);
	return 0;
}

static int32_t SetProximityArrival(int proximity, int nearness, int arrival)
{
	_hackev.target_move.set_proximity_arrival(proximity,
						    nearness, arrival);
	return 0;
}

static int32_t SetProximityArrivalDeg(int proximity, int arrival)
{
	_hackev.target_move.set_proximity_arrival_deg(proximity, arrival);
	return 0;
}

static int32_t SetPID(float Kp, float Ki, float Kd)
{
	_hackev.target_move.set_pid(Kp, Ki, Kd);
	return 0;
}

static std::string GetStatus(void)
{
	std::string status;
	sharaku::sd_position_move::status st = _hackev.target_move.get_status();
	switch (st) {
	case sharaku::sd_position_move::STATUS_STOP:
		status = "STOP";
		break;
	case sharaku::sd_position_move::STATUS_MOVING:
		status = "MOVING";
		break;
	case sharaku::sd_position_move::STATUS_PROXIMITY:
		status = "PROXIMITY";
		break;
	case sharaku::sd_position_move::STATUS_ARRIVAL:
		status = "ARRIVAL";
		break;
	case sharaku::sd_position_move::STATUS_PASSING:
		status = "PASSING";
		break;
	}
	return status;
}

static int32_t SetTargetPosition(int mm_x, int mm_y, int deg_r)
{
	position3 pos;
	pos.x = mm_x;
	pos.y = mm_y;
	_hackev.target_move.set_position_sp(pos);
	return 0;
}

static int32_t SetTargetDistance(int mm)
{
	_hackev.target_move.set_trget_distance_sp(mm);
	return 0;
}

static int32_t SetTurnDegree(int deg, int rho_mm=0)
{
	_hackev.target_move.set_trget_distance_deg_sp(0, deg);
	return 0;
}

static int32_t WaitStatus(std::string status1, std::string status2 = "",
			  std::string status3 = "", std::string status4 = "",
			  std::string status5 = "", std::string status6 = "")
{
	int32_t i;
	const struct timespec sleep_time = {0, 100000};
	std::string status[6];
	status[0] = status1;
	status[1] = status2;
	status[2] = status3;
	status[3] = status4;
	status[4] = status5;
	status[5] = status6;

	for (i = 0; i < 6; i++) {
		if (status[i] != "STOP" &&
		    status[i] != "MOVING" &&
		    status[i] != "PROXIMITY" &&
		    status[i] != "ARRIVAL" &&
		    status[i] != "PASSING" &&
		    status[i] != "") {
			return -1;
		}
	}

	std::string now_status;
	for (;;) {
		for (i = 0; i < 6; i++) {
			now_status = GetStatus();
			if (now_status == status[i]) {
				break;
			}
		}
		nanosleep(&sleep_time, NULL);
	}
	return 0;
}

};


//////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------
// センサー
// ---------------------------------------------------------------------
void export_submodule_gyro() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("libHackEV.Gyro"))));
	// from mainmodule import gyro を使えるようにする
	scope().attr("Gyro") = module;
	// スコープを設定
	scope gyro_scope = module;

	def("SetZeroAngle", Gyro::SetZeroAngle);
	def("GetAngle", Gyro::GetAngle);
	def("GetRate", Gyro::GetRate);
	def("Reset", Gyro::Reset);
}

void export_submodule_color() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("libHackEV.Color"))));
	// from mainmodule import color を使えるようにする
	scope().attr("Color") = module;
	// スコープを設定
	scope gyro_scope = module;

	def("SetMode", Color::SetMode);
	def("GetValue", Color::GetValue);
	def("GetRed", Color::GetRed);
	def("GetGreen", Color::GetGreen);
	def("GetBlue", Color::GetBlue);
}

void export_submodule_usonic() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("libHackEV.USonic"))));
	// from mainmodule import usonic を使えるようにする
	scope().attr("USonic") = module;
	// スコープを設定
	scope gyro_scope = module;

	def("GetMM", USonic::GetMM);
	def("GetInch", USonic::GetInch);
}

void export_submodule_touch() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("libHackEV.Touch"))));
	// from mainmodule import usonic を使えるようにする
	scope().attr("Touch") = module;
	// スコープを設定
	scope gyro_scope = module;

	def("IsPress", Touch::IsPress);
	def("IsPush", Touch::IsPush);
	def("IsRelease", Touch::IsRelease);
}

// ---------------------------------------------------------------------
// センサー情報
// ---------------------------------------------------------------------
void export_submodule_arm() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("libHackEV.Arm"))));
	// from mainmodule import usonic を使えるようにする
	scope().attr("Arm") = module;
	// スコープを設定
	scope gyro_scope = module;

	def("GetDegree", Arm::GetDegree);
	def("SetDegree", Arm::SetDegree);
	def("WaitDegree", Arm::WaitDegree);
}

void export_submodule_tail() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("libHackEV.Tail"))));
	// from mainmodule import usonic を使えるようにする
	scope().attr("Tail") = module;
	// スコープを設定
	scope gyro_scope = module;

	def("GetDegree", Tail::GetDegree);
	def("SetDegree", Tail::SetDegree);
	def("WaitDegree", Tail::WaitDegree);
}


// ---------------------------------------------------------------------
// モジュール
// ---------------------------------------------------------------------
void export_submodule_motors() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("libHackEV.Motors"))));
	// from mainmodule import odo を使えるようにする
	scope().attr("Motors") = module;
	// スコープを設定
	scope odo_scope = module;

	def("SetAutoCorrectionOnOff", Motors::SetAutoCorrectionOnOff);
	def("SetMotorsRatio", Motors::SetMotorsRatio);
	def("SetFwdSpeed", Motors::SetFwdSpeed);
	def("GetFwdSpeed", Motors::GetFwdSpeed);
	def("SetSteerDegree", Motors::SetSteerDegree);
	def("GetSteerDegree", Motors::GetSteerDegree);
	def("SetStopMode", Motors::SetStopMode);
}

void export_submodule_odo() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("libHackEV.Odo"))));
	// from mainmodule import odo を使えるようにする
	scope().attr("Odo") = module;
	// スコープを設定
	scope odo_scope = module;

	def("GetDistance", Odo::GetDistance);
	def("GetPositionX", Odo::GetPositionX);
	def("GetPositionY", Odo::GetPositionY);
	def("GetPositionRot", Odo::GetPositionRot);
	def("SetDistance", Odo::SetDistance);
	def("SetPosition", Odo::SetPosition);
	def("WaitDistance", Odo::WaitDistance);
}

void export_submodule_linetrace() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("libHackEV.Linetrace"))));
	// from mainmodule import linetrace を使えるようにする
	scope().attr("Linetrace") = module;
	// スコープを設定
	scope linetrace_scope = module;

	def("CalibrationWhite", Linetrace::CalibrationWhite);
	def("CalibrationBlack", Linetrace::CalibrationBlack);
	def("SetOnOff", Linetrace::SetOnOff);
	def("SetEdge", Linetrace::SetEdge);
	def("SetParam", Linetrace::SetParam);
	def("GetUncertainTime", Linetrace::GetUncertainTime);
}

void export_submodule_targetmove() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("libHackEV.TargetMove"))));
	// from mainmodule import linetrace を使えるようにする
	scope().attr("TargetMove") = module;
	// スコープを設定
	scope linetrace_scope = module;

	def("SetOnOff", TargetMove::SetOnOff);
	def("SetAutoSpeedOnOff", TargetMove::SetAutoSpeedOnOff);
	def("SetSpeedMinMax", TargetMove::SetSpeedMinMax);
	def("SetProximityArrival", TargetMove::SetProximityArrival);
	def("SetProximityArrivalDeg", TargetMove::SetProximityArrivalDeg);
	def("SetPID", TargetMove::SetPID);
	def("GetStatus", TargetMove::GetStatus);
	def("SetTargetPosition", TargetMove::SetTargetPosition);
	def("SetTargetDistance", TargetMove::SetTargetDistance);
	def("SetTurnDegree", TargetMove::SetTurnDegree);
	def("WaitStatus", TargetMove::WaitStatus);
}

// ---------------------------------------------------------------------
// モジュール定義
// ---------------------------------------------------------------------
BOOST_PYTHON_MODULE(libHackEV)
{
	using namespace boost::python;

	object package = scope();
	package.attr("__path__") = "libHackEV";
	def("Initialize", initialize);
	def("Finalize", finalize);
	def("Version", version);

	export_submodule_gyro();
	export_submodule_color();
	export_submodule_usonic();
	export_submodule_touch();

	export_submodule_arm();
	export_submodule_tail();
	export_submodule_motors();
	export_submodule_odo();
	export_submodule_linetrace();
	export_submodule_targetmove();
}


