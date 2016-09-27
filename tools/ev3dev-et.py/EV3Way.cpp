/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
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
#include <sharaku/task.h>
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include "EV3way.hpp"

using namespace std;

static pthread_t		_th;
static struct sharaku_job	_job_init;
static struct sharaku_job	_job_exit;
static EV3way			_ev3way;

//////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------
// メイン制御
// ---------------------------------------------------------------------
static void job_init(struct sharaku_job *job)
{
}

static void job_exit_end(struct sharaku_job *job)
{
	// 終了する。
	// これにより、sharaku_entryが終了する。
	sharaku_exit_message(0);
}

static void job_exit(struct sharaku_job *job)
{
	// スケジュールされているjobをキャンセルする
	sharaku_cancel_message(&_job_init);

	// 終了するまで100ms待つ。その間にモータ等を終了させる
	sharaku_timer_message(&_job_exit, 100, job_exit_end);
}


static void* start_routine(void *arg)
{
	sharaku_init_job(&_job_init);
	sharaku_async_message(&_job_init, job_init);
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
	sharaku_init_job(&_job_exit);
	sharaku_async_message(&_job_exit, job_exit);
	pthread_join(_th, NULL);
	return 0;
}

static void version(void)
{
	printf("%s.%s\n", __DATE__, __TIME__);
	return;
}

// ---------------------------------------------------------------------
// Sensors
// ---------------------------------------------------------------------
namespace Gyro {

static int32_t GetAngle(void)
{
	return ENOTSUP;
}

static int32_t GetRate(void)
{
	return ENOTSUP;
}

static int32_t Reset(void)
{
	return ENOTSUP;
}

};
// ---------------------------------------------------------------------
namespace Color {

static int32_t SetMode(std::string mode)
{
	return ENOTSUP;
}

static int32_t GetValue(void)
{
	return 0;
}

static int32_t GetRed(void)
{
	return 0;
}

static int32_t GetGreen(void)
{
	return 0;
}

static int32_t GetBlue(void)
{
	return 0;
}

};
// ---------------------------------------------------------------------
namespace USonic {

static int32_t GetMM(void)
{
	return ENOTSUP;
}

static int32_t GetInch(void)
{
	return ENOTSUP;
}

};
// ---------------------------------------------------------------------
namespace Touch {

static bool IsPress(void)
{
	return false;
}

static bool IsPush(void)
{
	return false;
}

static bool IsRelease(void)
{
	return false;
}

};
// ---------------------------------------------------------------------
namespace Tail {


static int32_t GetDegree(void)
{
	return ENOTSUP;
}

static int32_t SetDegree(int32_t deg)
{
	return ENOTSUP;
}

static int32_t WaitDegree(void)
{
	return ENOTSUP;
}

};
// ---------------------------------------------------------------------
namespace Motors {

static int32_t SetMotorsRatio(float motor_l, float motor_r)
{
	return ENOTSUP;
}

static int32_t SetFwdSpeed(int32_t speed)
{
	return ENOTSUP;
}

static int32_t GetFwdSpeed(void)
{
	return ENOTSUP;
}

static int32_t SetSteerDegree(int32_t steer)
{
	return ENOTSUP;
}

static int32_t GetSteerDegree(void)
{
	return ENOTSUP;
}

static int32_t SetSteerRho(float rho)
{
	return ENOTSUP;
}

static int32_t GetSteerRho(void)
{
	return ENOTSUP;
}

static int32_t SetStopMode(void)
{
	return ENOTSUP;
}

};
// ---------------------------------------------------------------------
namespace Odo {


static int32_t GetDistance(void)
{
	return ENOTSUP;
}

static int32_t GetPositionX(void)
{
	return ENOTSUP;
}

static int32_t GetPositionY(void)
{
	return ENOTSUP;
}

static int32_t GetPositionRot(void)
{
	return ENOTSUP;
}

static int32_t SetDistance(int32_t distance)
{
	return ENOTSUP;
}

static int32_t SetRotation(int32_t deg)
{
	return ENOTSUP;
}

static int32_t SetPosition(float x, float y)
{
	return ENOTSUP;
}

static int32_t WaitDistance(void)
{
	return ENOTSUP;
}

};
// ---------------------------------------------------------------------
namespace Linetrace {


static int32_t CalibrationWhite(uint32_t value)
{
	return ENOTSUP;
}

static int32_t CalibrationBlack(uint32_t value)
{
	return ENOTSUP;
}

static int32_t SetOnOff(bool onoff)
{
	return ENOTSUP;
}

static int32_t SetEdge(uint32_t edge)
{
	return ENOTSUP;
}

static int32_t SetParam(float Kp, float Ki, float Kd, float q, int in_angle, int out_angle)
{
	return ENOTSUP;
}

static int32_t GetUncertainTime(void)
{
	return ENOTSUP;
}

};
// ---------------------------------------------------------------------
namespace TargetMove {


static int32_t SetOnOff(bool onoff)
{
	return ENOTSUP;
}

static int32_t SetAutoSpeedOnOff(bool onoff)
{
	return ENOTSUP;
}

static int32_t SetSpeedMinMax(int min_speed, int max_speed)
{
	return ENOTSUP;
}

static int32_t SetProximityArrival(int proximity, int nearness, int arrival)
{
	return ENOTSUP;
}

static int32_t SetProximityArrivalDeg(int proximity, int arrival)
{
	return ENOTSUP;
}

static int32_t SetPID(float Kp, float Ki, float Kd)
{
	return ENOTSUP;
}

static int32_t GetStatus(void)
{
	return ENOTSUP;
}

static int32_t SetTargetPosition(int mm_x, int mm_y, int deg_r)
{
	return ENOTSUP;
}

static int32_t SetTargetDistance(int mm)
{
	return ENOTSUP;
}

static int32_t SetTurnDegree(int deg, int rho_mm=0)
{
	return ENOTSUP;
}

static int32_t WaitStatus(void)
{
	return ENOTSUP;
}

};
// ---------------------------------------------------------------------
namespace Baranser {


static int32_t SetOnOff(bool onoff)
{
	return ENOTSUP;
}

static int32_t Reset(void)
{
	return ENOTSUP;
}

};


//////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------
// センサー
// ---------------------------------------------------------------------
void export_submodule_gyro() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("HackEV.Gyro"))));
	// from mainmodule import gyro を使えるようにする
	scope().attr("Gyro") = module;
	// スコープを設定
	scope gyro_scope = module;

	def("GetAngle", &Gyro::GetAngle);
	def("GetRate", &Gyro::GetRate);
	def("Reset", &Gyro::Reset);
}

void export_submodule_color() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("HackEV.Color"))));
	// from mainmodule import color を使えるようにする
	scope().attr("Color") = module;
	// スコープを設定
	scope gyro_scope = module;

	def("SetMode", &Color::SetMode);
	def("GetValue", &Color::GetValue);
	def("GetRed", &Color::GetRed);
	def("GetGreen", &Color::GetGreen);
	def("GetBlue", &Color::GetBlue);
}

void export_submodule_usonic() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("HackEV.USonic"))));
	// from mainmodule import usonic を使えるようにする
	scope().attr("USonic") = module;
	// スコープを設定
	scope gyro_scope = module;

	def("GetMM", &USonic::GetMM);
	def("GetInch", &USonic::GetInch);
}

void export_submodule_touch() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("HackEV.Touch"))));
	// from mainmodule import usonic を使えるようにする
	scope().attr("Touch") = module;
	// スコープを設定
	scope gyro_scope = module;

	def("IsPress", &Touch::IsPress);
	def("IsPush", &Touch::IsPush);
	def("IsRelease", &Touch::IsRelease);
}

// ---------------------------------------------------------------------
// センサー情報
// ---------------------------------------------------------------------
void export_submodule_tail() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("HackEV.Tail"))));
	// from mainmodule import usonic を使えるようにする
	scope().attr("Tail") = module;
	// スコープを設定
	scope gyro_scope = module;

	def("GetDegree", &Tail::GetDegree);
	def("SetDegree", &Tail::SetDegree);
	def("WaitDegree", &Tail::WaitDegree);
}


// ---------------------------------------------------------------------
// モジュール
// ---------------------------------------------------------------------
void export_submodule_motors() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("HackEV.Motors"))));
	// from mainmodule import odo を使えるようにする
	scope().attr("Odo") = module;
	// スコープを設定
	scope odo_scope = module;

	def("SetMotorsRatio", &Motors::SetMotorsRatio);
	def("SetFwdSpeed", &Motors::SetFwdSpeed);
	def("GetFwdSpeed", &Motors::GetFwdSpeed);
	def("SetSteerDegree", &Motors::SetSteerDegree);
	def("GetSteerDegree", &Motors::GetSteerDegree);
	def("SetSteerRho", &Motors::SetSteerRho);
	def("GetSteerRho", &Motors::GetSteerRho);
	def("SetStopMode", &Motors::SetStopMode);
}

void export_submodule_odo() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("HackEV.Odo"))));
	// from mainmodule import odo を使えるようにする
	scope().attr("Odo") = module;
	// スコープを設定
	scope odo_scope = module;

	def("GetDistance", &Odo::GetDistance);
	def("GetPositionX", &Odo::GetPositionX);
	def("GetPositionY", &Odo::GetPositionY);
	def("GetPositionRot", &Odo::GetPositionRot);
	def("SetDistance", &Odo::SetDistance);
	def("SetPosition", &Odo::SetPosition);
	def("WaitDistance", &Odo::WaitDistance);
}

void export_submodule_linetrace() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("HackEV.Linetrace"))));
	// from mainmodule import linetrace を使えるようにする
	scope().attr("Linetrace") = module;
	// スコープを設定
	scope linetrace_scope = module;

	def("CalibrationWhite", &Linetrace::CalibrationWhite);
	def("CalibrationBlack", &Linetrace::CalibrationBlack);
	def("SetOnOff", &Linetrace::SetOnOff);
	def("SetEdge", &Linetrace::SetEdge);
	def("SetParam", &Linetrace::SetParam);
	def("GetUncertainTime", &Linetrace::GetUncertainTime);
}

void export_submodule_targetmove() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("HackEV.TargetMove"))));
	// from mainmodule import linetrace を使えるようにする
	scope().attr("Linetrace") = module;
	// スコープを設定
	scope linetrace_scope = module;

	def("SetOnOff", &TargetMove::SetOnOff);
	def("SetAutoSpeedOnOff", &TargetMove::SetAutoSpeedOnOff);
	def("SetSpeedMinMax", &TargetMove::SetSpeedMinMax);
	def("SetProximityArrival", &TargetMove::SetProximityArrival);
	def("SetProximityArrivalDeg", &TargetMove::SetProximityArrivalDeg);
	def("SetPID", &TargetMove::SetPID);
	def("GetStatus", &TargetMove::GetStatus);
	def("SetTargetPosition", &TargetMove::SetTargetPosition);
	def("SetTargetDistance", &TargetMove::SetTargetDistance);
	def("SetTurnDegree", &TargetMove::SetTurnDegree);
	def("WaitStatus", &TargetMove::WaitStatus);
}

void export_submodule_baranser() {
	using namespace boost::python;

	object module(handle<>(borrowed(PyImport_AddModule("HackEV.Baranser"))));
	// from mainmodule import linetrace を使えるようにする
	scope().attr("Baranser") = module;
	// スコープを設定
	scope linetrace_scope = module;

	def("SetOnOff", &Baranser::SetOnOff);
	def("Reset", &Baranser::Reset);
}

// ---------------------------------------------------------------------
// モジュール定義
// ---------------------------------------------------------------------
BOOST_PYTHON_MODULE(EV3Way)
{
	using namespace boost::python;

	object package = scope();
	package.attr("__path__") = "EV3Way";
	def("Initialize", &initialize);
	def("Finalize", &finalize);
	def("Version", &version);

	export_submodule_gyro();
	export_submodule_color();
	export_submodule_usonic();
	export_submodule_touch();

	export_submodule_tail();
	export_submodule_motors();
	export_submodule_odo();
	export_submodule_linetrace();
	export_submodule_targetmove();
	export_submodule_baranser();
}


