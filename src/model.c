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
#include "model.h"
#include "model/model_infrared_motion_sensor.h"
#include "model/model_infrared_obstacle_avoidance_sensor.h"

struct _model_s {
	sensor_type_e sensor_type;
};
static struct _model_s model_s;

void model_fini(void)
{
	switch (model_s.sensor_type) {
	case SENSOR_TYPE_ULTRASONIC:
		break;
	case SENSOR_TYPE_INFRARED_MOTION:
		model_fini_infrared_motion_sensor();
		break;
	case SENSOR_TYPE_INFRARED_OBSTACLE_AVOIDANCE:
		model_fini_infrared_obstacle_avoidance_sensor();
		break;
	default:
		break;
	}
}

int model_init(sensor_type_e sensor_type)
{
	int ret = 0;
	model_s.sensor_type = sensor_type;

	switch (sensor_type) {
	case SENSOR_TYPE_ULTRASONIC:
		break;
	case SENSOR_TYPE_INFRARED_MOTION:
		ret = model_init_infrared_motion_sensor();
		break;
	case SENSOR_TYPE_INFRARED_OBSTACLE_AVOIDANCE:
		ret = model_init_infrared_obstacle_avoidance_sensor();
		break;
	default:
		break;
	}

	goto_if(ret != 0, error);

	return 0;

error:
	model_fini();
	return -1;
}

int model_alloc(void **data)
{
	switch (model_s.sensor_type) {
	case SENSOR_TYPE_ULTRASONIC:
		break;
	case SENSOR_TYPE_INFRARED_MOTION:
	case SENSOR_TYPE_INFRARED_OBSTACLE_AVOIDANCE:
		_E("No function for allocation");
		break;
	default:
		break;
	}

	return 0;
}

int model_read_int_value(int *out_value)
{
	int ret = 0;

	switch (model_s.sensor_type) {
	case SENSOR_TYPE_ULTRASONIC:
		ret = model_read_infrared_obstacle_avoidance_sensor(out_value);
		break;
	case SENSOR_TYPE_INFRARED_MOTION:
		ret = model_read_infrared_motion_sensor(out_value);
		break;
	default:
		break;
	}

	if (ret < 0) {
		_E("Something wrong in the result[%d]", ret);
		return -1;
	}

	return 0;
}

int model_write(void *data)
{
	switch (model_s.sensor_type) {
	case SENSOR_TYPE_ULTRASONIC:
	case SENSOR_TYPE_INFRARED_MOTION:
		_E("No function for writing");
		break;
	default:
		break;
	}

	return 0;
}
