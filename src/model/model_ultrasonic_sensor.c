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

struct _model_ultrasonic_s {
	peripheral_gpio_h trig_gpio;
	peripheral_gpio_h echo_gpio;
};
typedef struct _model_ultrasonic_s model_ultrasonic_s;

void model_fini_ultrasonic_sensor(void *peripheral_info)
{
	model_ultrasonic_s *info = peripheral_info;

	ret_if(!info);

	if (info->echo_gpio)
		peripheral_gpio_close(info->echo_gpio);

	if (info->trig_gpio)
		peripheral_gpio_close(info->trig_gpio);

	free(info);
}

int model_init_ultrasonic_sensor(int gpio_num1, int gpio_num2, void **peripheral_info)
{
	model_ultrasonic_s *info = NULL;
	int ret = 0;

	_I("Ultrasonic Sensor is initializing...");

	info = calloc(1, sizeof(model_ultrasonic_s));
	retv_if(!info, -1);
	*peripheral_info = info;

	ret = peripheral_gpio_open(gpio_num1, &info->trig_gpio);
	goto_if(ret != 0, error);
	goto_if(!info->trig_gpio, error);

	ret = peripheral_gpio_set_direction(info->trig_gpio, PERIPHERAL_GPIO_DIRECTION_OUT);
	goto_if(ret != 0, error);

	ret = peripheral_gpio_open(gpio_num2, &info->echo_gpio);
	goto_if(ret != 0, error);
	goto_if(!info->echo_gpio, error);

	ret = peripheral_gpio_set_direction(info->echo_gpio, PERIPHERAL_GPIO_DIRECTION_IN);
	goto_if(ret != 0, error);

	return 0;

error:
	model_fini_ultrasonic_sensor(info);
	free(info);
	*peripheral_info = NULL;
	return -1;
}

static int _get_echo_value(model_ultrasonic_s *info)
{
	int ret = 0;
	int value = 0;

	ret = peripheral_gpio_read(info->echo_gpio, &value);
	retv_if(ret < 0, -1);

	return value;
}

int model_read_ultrasonic_sensor(void *peripheral_info, double *value)
{
	model_ultrasonic_s *info = peripheral_info;
	int ret = 0;
	double duration = 0.0;
	struct timeval start_time, end_time, temp_start_time, temp_end_time;

	ret = peripheral_gpio_write(info->trig_gpio, 0);
	retv_if(ret < 0, -1);

	sleep(1);

	ret = peripheral_gpio_write(info->trig_gpio, 1);
	retv_if(ret < 0, -1);

	usleep(10);

	ret = peripheral_gpio_write(info->trig_gpio, 0);
	retv_if(ret < 0, -1);

	_D("Count the distance");
	gettimeofday(&temp_start_time, NULL);

	while (_get_echo_value(info) == 0) {
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

	while (_get_echo_value(info) == 1);
	gettimeofday(&end_time, NULL);

	_D("After checking #2");

	duration = (double)end_time.tv_sec + (double)(end_time.tv_usec / 1000000.0)
		- (double)start_time.tv_sec - (double)(start_time.tv_usec / 1000000.0);
	*value = duration / 2 * 340.0;

	return 0;
}
