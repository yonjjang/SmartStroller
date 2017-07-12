/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Contact: Jin Yoon <jinny.yoon@samsung.com>
 *          Geunsun Lee <gs86.lee@samsung.com>
 *          Eunyoung Lee <ey928.lee@samsung.com>
 *          Junkyu Han <junkyu.han@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <unistd.h>
#include <peripheral_io.h>
#include <sys/time.h>

#include "log.h"
#include "resource_internal.h"

void resource_close_ultrasonic_sensor(int echo_pin_num, int trig_pin_num)
{
	ret_if(!resource_get_info(echo_pin_num)->opened);
	ret_if(!resource_get_info(trig_pin_num)->opened);

	_I("Ultrasonic sensor is finishing...");

	peripheral_gpio_close(resource_get_info(echo_pin_num)->sensor_h);
	peripheral_gpio_close(resource_get_info(trig_pin_num)->sensor_h);

	resource_get_info(echo_pin_num)->opened = 0;
	resource_get_info(trig_pin_num)->opened = 0;
}

static int _get_echo_value(int echo_pin_num)
{
	int ret = 0;
	int value = 0;

	ret = peripheral_gpio_read(resource_get_info(echo_pin_num)->sensor_h, &value);
	retv_if(ret < 0, -1);

	return value;
}

int resource_read_ultrasonic_sensor(int echo_pin_num, int trig_pin_num, double *out_value)
{
	int ret = 0;
	double duration = 0.0;
	struct timeval start_time, end_time, temp_start_time, temp_end_time;

	if (!resource_get_info(echo_pin_num)->opened) {
		_I("Ultrasonic sensor is initializing...");

		ret = peripheral_gpio_open(echo_pin_num, &resource_get_info(echo_pin_num)->sensor_h);
		retv_if(!resource_get_info(echo_pin_num)->sensor_h, -1);

		ret = peripheral_gpio_set_direction(resource_get_info(echo_pin_num)->sensor_h, PERIPHERAL_GPIO_DIRECTION_IN);
		retv_if(ret != 0, -1);

		resource_get_info(echo_pin_num)->opened = 1;
	}

	if (!resource_get_info(trig_pin_num)->opened) {
		_I("Ultrasonic sensor is initializing...");

		ret = peripheral_gpio_open(trig_pin_num, &resource_get_info(trig_pin_num)->sensor_h);
		retv_if(!resource_get_info(trig_pin_num)->sensor_h, -1);

		ret = peripheral_gpio_set_direction(resource_get_info(trig_pin_num)->sensor_h, PERIPHERAL_GPIO_DIRECTION_IN);
		retv_if(ret != 0, -1);

		resource_get_info(trig_pin_num)->opened = 1;
	}

	ret = peripheral_gpio_write(resource_get_info(trig_pin_num)->sensor_h, 0);
	retv_if(ret < 0, -1);

	sleep(1);

	ret = peripheral_gpio_write(resource_get_info(trig_pin_num)->sensor_h, 1);
	retv_if(ret < 0, -1);

	usleep(10);

	ret = peripheral_gpio_write(resource_get_info(trig_pin_num)->sensor_h, 0);
	retv_if(ret < 0, -1);

	_D("Count the distance");
	gettimeofday(&temp_start_time, NULL);

	while (_get_echo_value(echo_pin_num) == 0) {
		gettimeofday(&temp_end_time, NULL);
		duration = (double)temp_end_time.tv_sec + (double)(temp_end_time.tv_usec / 1000000.0)
			- (double)temp_start_time.tv_sec - (double)(temp_start_time.tv_usec / 1000000.0);
		if (duration > 1.0f) {
			_E("Cannot get the echo value.");
			return -1;
		}
	}
	gettimeofday(&start_time, NULL);

	_D("After checking #1");

	while (_get_echo_value(echo_pin_num) == 1);
	gettimeofday(&end_time, NULL);

	_D("After checking #2");

	duration = (double)end_time.tv_sec + (double)(end_time.tv_usec / 1000000.0)
		- (double)start_time.tv_sec - (double)(start_time.tv_usec / 1000000.0);
	*out_value = duration / 2 * 340.0;

	_I("Ultrasonic Sensor Value : %f", *out_value);

	return 0;
}
