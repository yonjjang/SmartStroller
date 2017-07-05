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
#include <Eina.h>

#include "log.h"
#include "model.h"
#include "model/model_ultrasonic_sensor.h"
#include "model/model_infrared_motion_sensor.h"
#include "model/model_infrared_obstacle_avoidance_sensor.h"
#include "model/model_touch_sensor.h"

struct _model_sensor_s {
	char *id;
	sensor_type_e sensor_type;
	int gpio_num[2];
	void *peripheral_info;
};
static struct {
	Eina_List *list;
} model_s;

int model_init(const char *id, sensor_type_e sensor_type, int gpio_num1, int gpio_num2, model_sensor_h *out_info)
{
	model_sensor_h info = NULL;
	int ret = -1;

	retv_if(!id, -1);

	info = calloc(1, sizeof(model_sensor_s));
	retv_if(!info, -1);
	*out_info = info;

	info->sensor_type = sensor_type;
	info->id = strdup(id);
	goto_if(!info->id, error);

	info->gpio_num[0] = gpio_num1;
	info->gpio_num[1] = gpio_num2;
	goto_if(gpio_num1 == -1, error);

	switch (sensor_type) {
	case SENSOR_TYPE_ULTRASONIC:
		goto_if(gpio_num2 == -1, error);
		ret = model_init_ultrasonic_sensor(gpio_num1, gpio_num2, &info->peripheral_info);
		break;
	case SENSOR_TYPE_INFRARED_MOTION:
		ret = model_init_infrared_motion_sensor(gpio_num1, &info->peripheral_info);
		break;
	case SENSOR_TYPE_INFRARED_OBSTACLE_AVOIDANCE:
		ret = model_init_infrared_obstacle_avoidance_sensor(gpio_num1, &info->peripheral_info);
		break;
	case SENSOR_TYPE_TOUCH:
		ret = model_init_touch_sensor(gpio_num1, &info->peripheral_info);
		break;
	default:
		break;
	}

	goto_if(ret != 0, error);

	return 0;

error:
	if (info->id) free(info->id);
	if (info) free(info);
	*out_info = NULL;
	return -1;
}

void model_fini(model_sensor_h info)
{
	ret_if(!info);

	switch (info->sensor_type) {
	case SENSOR_TYPE_ULTRASONIC:
		model_fini_ultrasonic_sensor(info->peripheral_info);
		break;
	case SENSOR_TYPE_INFRARED_MOTION:
		model_fini_infrared_motion_sensor(info->peripheral_info);
		break;
	case SENSOR_TYPE_INFRARED_OBSTACLE_AVOIDANCE:
		model_fini_infrared_obstacle_avoidance_sensor(info->peripheral_info);
		break;
	case SENSOR_TYPE_TOUCH:
		model_fini_touch_sensor(info->peripheral_info);
		break;
	default:
		break;
	}

	free(info->id);
	free(info);
}

int model_read_int_value(model_sensor_h info, int *out_value)
{
	int ret = 0;

	switch (info->sensor_type) {
	case SENSOR_TYPE_ULTRASONIC:
		ret = model_read_infrared_obstacle_avoidance_sensor(info, out_value);
		break;
	case SENSOR_TYPE_INFRARED_MOTION:
		ret = model_read_infrared_motion_sensor(info, out_value);
		break;
	case SENSOR_TYPE_TOUCH:
		ret = model_read_touch_sensor(info, out_value);
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

int model_read_double_value(model_sensor_h info, double *out_value)
{
	int ret = 0;

	switch (info->sensor_type) {
	case SENSOR_TYPE_ULTRASONIC:
		ret = model_read_ultrasonic_sensor(info, out_value);
		break;
	default:
		_E("There is no data to retrieve");
		break;
	}

	if (ret < 0) {
		_E("Something wrong in the result[%d]", ret);
		return -1;
	}

	return 0;
}

int model_list_add_sensor(model_sensor_h info)
{
	Eina_List *l, *ln;
	model_sensor_h temp = NULL;

	retv_if(!info, -1);
	retv_if(!info->id, -1);

	EINA_LIST_FOREACH_SAFE(model_s.list, l, ln, temp) {
		retv_if(!temp->id, -1);
		if (!strcmp(temp->id, info->id)) {
			_E("That id[%s] already exists.", info->id);
			return -1;
		}
	}
	model_s.list = eina_list_append(model_s.list, info);
	return 0;
}

int model_list_remove_sensor(model_sensor_h info)
{
	retv_if(!info, -1);
	model_s.list = eina_list_remove(model_s.list, info);
	return 0;
}

int model_list_get_sensor(const char *id, model_sensor_h *out_info)
{
	Eina_List *l, *ln;
	model_sensor_h temp = NULL;

	retv_if(!id, -1);

	EINA_LIST_FOREACH_SAFE(model_s.list, l, ln, temp) {
		retv_if(!temp->id, -1);
		if (!strcmp(temp->id, id)) {
			_E("That id[%s] already exists.", id);
			*out_info = temp;
			return 0;
		}
	}

	*out_info = NULL;
	return -1;
}

int model_list_foreach(void (*cb)(model_sensor_h info))
{
	Eina_List *l, *ln;
	model_sensor_h temp = NULL;

	retv_if(!cb, -1);

	EINA_LIST_FOREACH_SAFE(model_s.list, l, ln, temp) {
		cb(temp);
	}

	return 0;
}
