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


#include <tizen.h>
#include <Ecore.h>
#include <service_app.h>
#include <unistd.h>
#include <glib.h>

#include "log.h"
#include "controller.h"
#include "model.h"

#define GPIO_NOT_USED -1
#define GPIO_ULTRASONIC_TRIG_NUM 20
#define GPIO_ULTRASONIC_ECHO_NUM 21
#define GPIO_INFRARED_MOTION_NUM 4

typedef struct app_data_s {
	model_sensor_h sensor_info;
	Ecore_Timer *getter_timer;
	void *event;
} app_data;

static Eina_Bool _ultrasonic_getter_timer(void *data)
{
	app_data *ad = data;

#if 1
	int value = 0;
	retv_if(model_read_int_value(ad->sensor_info, &value) == -1, ECORE_CALLBACK_CANCEL);
	_I("Ultrasonic Value is [%d]", value);
#else
	double value = 0.0;
	retv_if(model_read_double_value(ad->sensor_info, &value) == -1, ECORE_CALLBACK_RENEW);
	_I("Value is [%f]", value);
#endif

	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _infrared_motion_getter_timer(void *data)
{
	app_data *ad = data;

#if 1
	int value = 0;
	retv_if(model_read_int_value(ad->sensor_info, &value) == -1, ECORE_CALLBACK_CANCEL);
	_I("Infrared Motion Value is [%d]", value);
#else
	double value = 0.0;
	retv_if(model_read_double_value(ad->sensor_info, &value) == -1, ECORE_CALLBACK_RENEW);
	_I("Value is [%f]", value);
#endif

	return ECORE_CALLBACK_RENEW;
}

static bool service_app_create(void *data)
{
	model_sensor_h sensor_info = NULL;
	app_data *ad = data;

	retv_if(model_init("Ultrasonic", SENSOR_TYPE_ULTRASONIC, GPIO_ULTRASONIC_TRIG_NUM, GPIO_ULTRASONIC_ECHO_NUM, &sensor_info) == -1, false);
	ad->sensor_info = sensor_info;

	ad->getter_timer = ecore_timer_add(3.0, _ultrasonic_getter_timer, ad);
	if (!ad->getter_timer) {
		_D("Failed to add getter timer");
		return false;
	}

	retv_if(model_init("Infrared_motion", SENSOR_TYPE_INFRARED_MOTION, GPIO_INFRARED_MOTION_NUM, GPIO_NOT_USED, &sensor_info) == -1, false);
	ad->sensor_info = sensor_info;

	ad->getter_timer = ecore_timer_add(3.0, _infrared_motion_getter_timer, ad);
	if (!ad->getter_timer) {
		_D("Failed to add getter timer");
		return false;
	}

    return true;
}

static void service_app_terminate(void *data)
{
	app_data *ad = (app_data *)data;
	ecore_timer_del(ad->getter_timer);
	model_fini(ad->sensor_info);
	free(ad);
}

static void service_app_control(app_control_h app_control, void *data)
{
    // Todo: add your code here.
}

static void service_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
}

static void service_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void service_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void service_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[])
{
	app_data *ad = NULL;
	int ret = 0;
	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = {NULL, };

	ad = calloc(1, sizeof(app_data));

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);

	ret = service_app_main(argc, argv, &event_callback, ad);

	return ret;
}
