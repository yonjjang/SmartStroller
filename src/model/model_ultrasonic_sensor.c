/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Contact: Jin Yoon <jinny.yoon@samsung.com>
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

#include <unistd.h>
#include <peripheral_io.h>
#include <sys/time.h>

#include "log.h"

#define GPIO_TRAN_NUM 20
#define GPIO_ECHO_NUM 21

struct _model_s {
	peripheral_gpio_h tran_gpio;
	peripheral_gpio_h echo_gpio;
};
static struct _model_s model_s;

void model_fini_ultrasonic_sensor(void)
{
	if (model_s.echo_gpio)
		peripheral_gpio_close(model_s.echo_gpio);

	if (model_s.tran_gpio)
		peripheral_gpio_close(model_s.tran_gpio);
}

int model_init_ultrasonic_sensor(void)
{
	int ret = 0;

	/* GPIO for Ultrasonic Sensor's Transmit */
	ret = peripheral_gpio_open(GPIO_TRAN_NUM, &model_s.tran_gpio);
	retv_if(ret != 0, -1);
	retv_if(!model_s.tran_gpio, -1);

	ret = peripheral_gpio_set_direction(model_s.tran_gpio, PERIPHERAL_GPIO_DIRECTION_OUT);
	goto_if(ret != 0, error);

	/* GPIO for Ultrasonic Sensor's Echo */
	ret = peripheral_gpio_open(GPIO_ECHO_NUM, &model_s.echo_gpio);
	goto_if(ret != 0, error);
	goto_if(!model_s.echo_gpio, error);

	ret = peripheral_gpio_set_direction(model_s.echo_gpio, PERIPHERAL_GPIO_DIRECTION_IN);
	goto_if(ret != 0, error);

	return 0;

error:
	model_fini_ultrasonic_sensor();
	return -1;
}

static int _get_echo_value(void)
{
	int ret = 0;
	int value = 0;

	ret = peripheral_gpio_read(model_s.echo_gpio, &value);
	retv_if(ret < 0, -1);

	return value;
}

int model_read_ultrasonic_sensor(double *value)
{
	int ret = 0;
	double duration = 0.0;
	struct timeval start_time, end_time, temp_start_time, temp_end_time;

	ret = peripheral_gpio_write(model_s.tran_gpio, 0);
	retv_if(ret < 0, -1);

	sleep(1);

	ret = peripheral_gpio_write(model_s.tran_gpio, 1);
	retv_if(ret < 0, -1);

	usleep(10);

	ret = peripheral_gpio_write(model_s.tran_gpio, 0);
	retv_if(ret < 0, -1);

	_D("Count the distance");
	gettimeofday(&temp_start_time, NULL);

	while (_get_echo_value() == 0) {
		gettimeofday(&temp_end_time, NULL);
		duration = (double)temp_end_time.tv_sec + (double)(temp_end_time.tv_usec / 1000000.0)
			- (double)temp_start_time.tv_sec - (double)(temp_start_time.tv_usec / 1000000.0);
		if (duration > 1.0f) {
			_E("Cannot get the echo value[%d]", _get_echo_value());
			return -1;
		}
	}
	gettimeofday(&start_time, NULL);

	_D("After checking #1");

	while (_get_echo_value() == 1);
	gettimeofday(&end_time, NULL);

	_D("After checking #2");

	duration = (double)end_time.tv_sec + (double)(end_time.tv_usec / 1000000.0)
		- (double)start_time.tv_sec - (double)(start_time.tv_usec / 1000000.0);
	*value = duration / 2 * 340.0;

	return 0;
}
