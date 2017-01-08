/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <devices/power/ev3rt-powersupply.hpp>
#include <devices/touch/ev3rt-touch.hpp>
#include <devices/usonic/ev3rt-ultrasonic.hpp>
#include <devices/gyro/ev3rt-gyro.hpp>
#include <devices/color/ev3rt-color.hpp>
#include <devices/motor/ev3rt-motor.hpp>
#include <devices/motor/dps2duty-motor.hpp>
#include <devices/sd/ev3way/ev3way-balancer.hpp>
#include <devices/sd/sd-linetrace-pid.hpp>
#include <devices/sd/sd-wheel-odometry.hpp>
#include <modules/mod-update.hpp>
#include <modules/mod-devupdate.hpp>

class EV3way
 : public  sharaku::mod_update {
 public:
 	EV3way() :
 	mod_update(4),				// updateは4ms
	power(),				// 電源制御
	touch(EV3_PORT_1),			// IN1
	usonic(EV3_PORT_2),			// IN2
	color(EV3_PORT_3, sharaku::color_operations::MODE_REFLECTED),
						// IN3
	gyro(EV3_PORT_4),			// IN4
	motor_l(EV3_PORT_B),
	motor_r(EV3_PORT_C),
	motor_tail(EV3_PORT_D),
	linetrace(1.0f, 0, 0)
	{
	}

	void start() {
		// 情報を読み込む。
		// 最初の1回は時間がかかるはず...
		power.get_voltage();
		power.get_current();
		touch.is_press();
		usonic.get_dist();
		gyro.get_angle();
		gyro.get_rate();
		motor_l.get_position();
		motor_l.set_stop_mode(sharaku::ev3rt_motor::STOPMODE_BRAKE);
		motor_r.get_position();
		motor_r.set_stop_mode(sharaku::ev3rt_motor::STOPMODE_BRAKE);
		motor_tail.get_position();
		motor_tail.set_stop_mode(sharaku::ev3rt_motor::STOPMODE_BRAKE);

		balancer.out_duty_motor_l = motor_l;
		balancer.out_duty_motor_r = motor_r;
		balancer.in_gyro = gyro;
		balancer.in_power = power;
		balancer.set_max_dps(1020);
		balancer.balancer_off();
		linetrace.set_edge(sharaku::sd_linetrace::LINETRACE_EDGE_LEFT);
		linetrace.set_turn_angle(30, 30);
		linetrace.set_lowpass(1.0f);
		linetrace.clear_pid();
//		mod_update::register_update(balancer);
//		mod_update::start();
	}

	void stop() {
		mod_update::stop();
	}

 public:
	sharaku::ev3rt_powersupply	power;		// 電源制御
	sharaku::ev3rt_touch		touch;		// タッチセンサー
	sharaku::ev3rt_ultrasonic	usonic;		// 超音波センサー
	sharaku::ev3rt_gyro		gyro;		// ジャイロセンサー
	sharaku::ev3rt_color		color;		// カラーセンサー
	sharaku::ev3rt_motor		motor_l;	// 
	sharaku::ev3rt_motor		motor_r;	// 
	sharaku::ev3rt_motor		motor_tail;	// 
	sharaku::ev3way_balancer	balancer;
	sharaku::sd_linetrace_pid	linetrace;
};

#define DISP_FONT_WIDTH		(6)
#define DISP_FONT_HEIGHT	(8)


static inline void
sharaku_dev_disp_touch(sharaku::ev3rt_touch * tc, char *port, int h)
{
	static char	msg[128];
	sprintf(msg,
		"touch (%s): touch=%d,%d,%d", port,
		tc->is_press(),
		tc->is_push(),
		tc->is_release());
	ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * h);
}

static inline void
sharaku_dev_disp_gyro(sharaku::ev3rt_gyro *gy, char *port, int h)
{
	static char	msg[128];
	sprintf(msg,
		"gyro  (%s): rate=%-03d agl=%-03d", port,
		gy->get_rate(),
		gy->get_angle());
	ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * h);
}

static inline void
sharaku_dev_disp_color(sharaku::ev3rt_color *cl, char *port, int h)
{
	static char	msg[128];
	char *mode = "";
	switch (cl->get_mode()) {
	case sharaku::color_operations::MODE_REFLECTED:
		mode = "REFLECTED";
		break;
	case sharaku::color_operations::MODE_AMBIENT:
		mode = "AMBIENT";
		break;
	case sharaku::color_operations::MODE_CORRECTION:
		mode = "CORRECTION";
		break;
	case sharaku::color_operations::MODE_FULLCOLOR:
		mode = "FULLCOLOR";
		break;
	}

	sprintf(msg,
		"color (%s): mode=%s", port, mode);
	ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * h);

	switch (cl->get_mode()) {
	case sharaku::color_operations::MODE_REFLECTED:
	case sharaku::color_operations::MODE_AMBIENT:
	case sharaku::color_operations::MODE_CORRECTION:
		sprintf(msg,
			"value=%s", cl->get_value());
		ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * (h + 1));
		break;
	case sharaku::color_operations::MODE_FULLCOLOR:
		sprintf(msg,
			"r=%03d g=%03d b=%03d",
			cl->get_red(),
			cl->get_green(),
			cl->get_blue());
		ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * (h + 1));
		break;
	}

}

static inline void
sharaku_dev_disp_usonic(sharaku::ev3rt_ultrasonic *us, char *port, int h)
{
	static char	msg[128];
	sprintf(msg,
		"usonic(%s): %05dmm", port,
		us->get_dist());
	ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * h);
}

static inline void
sharaku_dev_disp_motor(sharaku::ev3rt_motor *mt, char *port, int h)
{
	static char	msg[128];
	sprintf(msg,
		"motor(%s): pos=%-05d duty=%-03d", port,
		mt->get_position(),
		mt->get_duty_cycle());
	ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * h);
}

static inline void
sharaku_dev_disp_power(sharaku::ev3rt_powersupply *pp, int h)
{
	static char	msg[128];
	sprintf(msg,
		"power     : %dmV %dmA",
		pp->get_voltage(),
		pp->get_current());
	ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * h);
}

static inline void
sharaku_dev_disp(EV3way *ev3)
{
	static char	msg[128];
	sprintf(msg, "[time] %u", sharaku_get_usec());
	ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * 2);
	ev3_lcd_draw_string("[sensors]", 0, DISP_FONT_HEIGHT * 4);
	sharaku_dev_disp_power(&ev3->power, 5);
	sharaku_dev_disp_touch(&ev3->touch, "in1", 6);
	sharaku_dev_disp_gyro(&ev3->gyro, "in4", 7);
	sharaku_dev_disp_usonic(&ev3->usonic, "in2", 8);
//	sharaku_dev_disp_color(&ev3->color, "in3", 9);

	ev3_lcd_draw_string("[motors]", 0, DISP_FONT_HEIGHT * 12);
	sharaku_dev_disp_motor(&ev3->motor_l, "outB", 13);
	sharaku_dev_disp_motor(&ev3->motor_r, "outC", 14);
	sharaku_dev_disp_motor(&ev3->motor_tail, "outD", 15);
}

static inline void
sharaku_prof_disp(EV3way *ev3)
{
	static char	msg[128];
	sprintf(msg, "[time] %u", sharaku_get_usec());
	ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * 2);
	sprintf(msg,
		"[update interval]  %u us",
		 ev3->get_prof_usec_interval() / ev3->get_prof_count_interval());
	ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * 4);
	sprintf(msg,
		"[update processing]  %u us",
		 ev3->get_prof_usec_processing() / ev3->get_prof_count_processing());
	ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * 5);
	sprintf(msg,
		"[balancer interval]  %f us", ev3->balancer.get_interval() * 1000000);
	ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * 6);
	sprintf(msg,
		"[balancer period]  %f us", ev3->balancer.get_exec_period() * 1000000);
	ev3_lcd_draw_string(msg, 0, DISP_FONT_HEIGHT * 7);


}

extern "C" void job_entry(struct sharaku_job *job);

