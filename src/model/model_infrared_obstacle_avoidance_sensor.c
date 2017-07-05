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
#include "model_internal.h"

void model_close_infrared_obstacle_avoidance_sensor(int pin_num)
{
	ret_if(!model_get_info(pin_num)->opened);

	_I("Infrared Obstacle Avoidance Sensor is finishing...");
	peripheral_gpio_close(model_get_info(pin_num)->sensor_h);
	model_get_info(pin_num)->opened = 0;
}

int model_read_infrared_obstacle_avoidance_sensor(int pin_num, int *out_value)
{
	int ret = PERIPHERAL_ERROR_NONE;

	if (!model_get_info(pin_num)->opened) {
		ret = peripheral_gpio_open(pin_num, &model_get_info(pin_num)->sensor_h);
		retv_if(!model_get_info(pin_num)->sensor_h, -1);

		ret = peripheral_gpio_set_direction(model_get_info(pin_num)->sensor_h, PERIPHERAL_GPIO_DIRECTION_IN);
		retv_if(ret != 0, -1);

		model_get_info(pin_num)->opened = 1;
	}

	ret = peripheral_gpio_read(model_get_info(pin_num)->sensor_h, out_value);
	retv_if(ret < 0, -1);

	_I("Infrared Obstacle Avoidance Sensor Value : %d", *out_value);

	return 0;
}
