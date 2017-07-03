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

#include <stdlib.h>
#include <unistd.h>
#include <peripheral_io.h>
#include <sys/time.h>

#include "log.h"
#include "model/model_infrared_motion_sensor.h"

struct _model_infrared_motion_sensor_s {
	peripheral_gpio_h gpio;
};
typedef struct _model_infrared_motion_sensor_s model_infrared_motion_sensor_s;

void model_fini_infrared_motion_sensor(void *peripheral_info)
{
	model_infrared_motion_sensor_s *info = peripheral_info;

	ret_if(!info);

	_I("Infrared Motion Sensor is finishing...");


	if (info->gpio)
		peripheral_gpio_close(info->gpio);

	free(info);
}

int model_init_infrared_motion_sensor(int gpio_num, void **peripheral_info)
{
	model_infrared_motion_sensor_s *info = NULL;
	int ret = 0;

	_I("Infrared Motion Sensor is initializing...");

	info = calloc(1, sizeof(model_infrared_motion_sensor_s));
	retv_if(!info, -1);
	*peripheral_info = info;

	ret = peripheral_gpio_open(gpio_num, &info->gpio);
	retv_if(ret != 0, -1);
	retv_if(!info->gpio, -1);

	ret = peripheral_gpio_set_direction(info->gpio, PERIPHERAL_GPIO_DIRECTION_IN);
	goto_if(ret != 0, error);

	return 0;

error:
	model_fini_infrared_motion_sensor(info);
	free(info);
	*peripheral_info = NULL;
	return -1;
}

int model_read_infrared_motion_sensor(void *peripheral_info, int *out_value)
{
	model_infrared_motion_sensor_s *info = peripheral_info;
	int ret = 0;

	ret = peripheral_gpio_read(info->gpio, out_value);
	retv_if(ret < 0, -1);

	_I("Infrared Motion Sensor Value : %d", *out_value);

	return 0;
}
