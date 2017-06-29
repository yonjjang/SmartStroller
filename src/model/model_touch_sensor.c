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
#include "model/model_touch_sensor.h"

#define GPIO_NUM 4

struct model_touch_sensor {
	peripheral_gpio_h gpio;
};
static struct model_touch_sensor model_touch_sensor_s;

void model_fini_touch_sensor(void)
{
	_I("Touch Sensor is finishing...");

	if (model_touch_sensor_s.gpio)
		peripheral_gpio_close(model_touch_sensor_s.gpio);
}

int model_init_touch_sensor(void)
{
	int ret = 0;

	_I("Touch is initializing...");

	/* GPIO for Ultrasonic Sensor's Transmit */
	ret = peripheral_gpio_open(GPIO_NUM, &model_touch_sensor_s.gpio);
	retv_if(ret != 0, -1);
	retv_if(!model_touch_sensor_s.gpio, -1);

	ret = peripheral_gpio_set_direction(model_touch_sensor_s.gpio, PERIPHERAL_GPIO_DIRECTION_IN);
	goto_if(ret != 0, error);

	return 0;

error:
	model_fini_touch_sensor();
	return -1;
}

int model_read_touch_sensor(int *out_value)
{
	int ret = 0;

	ret = peripheral_gpio_read(model_touch_sensor_s.gpio, out_value);
	retv_if(ret < 0, -1);

	_I("Touch Sensor Value : %d", *out_value);

	return 0;
}
