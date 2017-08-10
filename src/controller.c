/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Contact: Jin Yoon <jinny.yoon@samsung.com>
 *          Geunsun Lee <gs86.lee@samsung.com>
 *          Eunyoung Lee <ey928.lee@samsung.com>
 *          Junkyu Han <junkyu.han@samsung.com>
 *
 * Licensed under the Flora License, Version 1.1 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://floralicense.org/license/
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <unistd.h>
#include <glib.h>
#include <Ecore.h>
#include <tizen.h>
#include <service_app.h>

#include <iotcon.h> // Please remove this after test

#include "log.h"
#include "resource.h"
#include "connectivity.h"
#include "controller.h"

#define GPIO_ULTRASONIC_TRIG_NUM_1 20
#define GPIO_ULTRASONIC_ECHO_NUM_1 21
#define MULTIPLE_SENSOR_NUMBER 5
#define CONNECTIVITY_KEY "opened"

typedef struct app_data_s {
	Ecore_Timer *getter_timer;
	connectivity_resource_s *resource_info;
} app_data;

static Eina_Bool control_read_sensors_cb(void *data)
{
	int value[MULTIPLE_SENSOR_NUMBER] = { 0, };
	int total = 0;
	int gpio_num[MULTIPLE_SENSOR_NUMBER] = { 5, 6, 13, 19, 26 };
	int i = 0;
	app_data *ad = data;

	for (i = 0; i < MULTIPLE_SENSOR_NUMBER; i++) {
		if (resource_read_infrared_motion_sensor(gpio_num[i], &value[i]) == -1) {
			_E("Failed to get Infrared Motion value [GPIO:%d]", gpio_num[i]);
			continue;
		}
		total |= value[i];
	}
	_I("[5:%d] | [6:%d] | [13:%d] | [19:%d] | [26:%d] = [Total:%d]", value[0], value[1], value[2], value[3], value[4], total);

	if (connectivity_notify_bool(ad->resource_info, CONNECTIVITY_KEY, total) == -1)
		_E("Cannot notify message");

	return ECORE_CALLBACK_RENEW;
}

static bool service_app_create(void *data)
{
	app_data *ad = data;
	int ret = -1;

	controller_init_internal_functions();
	ret = connectivity_set_resource("/door/1", "org.tizen.door", &ad->resource_info);
	if (ret == -1) _E("Cannot broadcast resource");

	ad->getter_timer = ecore_timer_add(0.5f, control_read_sensors_cb, ad);
	if (!ad->getter_timer) {
		_E("Failed to add infrared motion getter timer");
		return false;
	}

    return true;
}

static void service_app_terminate(void *data)
{
	app_data *ad = (app_data *)data;

	for (int i = 0; i < PIN_MAX; i++) {
		if (ad->getter_timer) {
			ecore_timer_del(ad->getter_timer);
		}
	}

	connectivity_unset_resource(ad->resource_info);
	controller_fini_internal_functions();

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
	retv_if(!ad, -1);

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
