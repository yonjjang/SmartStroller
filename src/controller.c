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

#include "log.h"
#include "resource.h"
#include "connectivity.h"
#include "controller.h"

typedef struct app_data_s {
	Ecore_Timer *getter_timer;
	connectivity_resource_s *resource_info;
} app_data;

static void _ultrasonic_sensor_read_cb(float value, void *data)
{
	app_data *ad = data;

	/**
	 * TODO: Send the value of the ultrasonic sensor to the Client.
	 */

	/*
	if (value < 0) {
		_E("OUT OF RANGE");
	} else {
		_D("Measured Distance : %0.2fcm", value);
	}
	*/
}

static Eina_Bool _control_sensors_cb(void *data)
{
	app_data *ad = data;
	int ret = 0;

	/**
	 * TODO: Prepare to read the value of the ultrasonic sensor.
	 */

    /*
	ret = resource_read_ultrasonic_sensor(TRIG_PIN_NUMBER, ECHO_PIN_NUMBER, _ultrasonic_sensor_read_cb, NULL);
	*/

	if (ret < 0) {
		_E("Failed to read from ultrasonic sensor");
	}

	/*
	 * Notifies specific clients that resource's attributes have changed.
	 */
	/*
	if (connectivity_notify_double(ad->resource_info, "distance", value) == -1)
		_E("Cannot notify message");
	*/


	return ECORE_CALLBACK_RENEW;
}

static bool service_app_create(void *data)
{
	app_data *ad = data;
	int ret = -1;

	/**
	 * No modification required!!!
	 * Access only when modifying internal functions.
	 */
	controller_init_internal_functions();

	/**
	 * Create a connectivity resource and registers the resource in server.
	 */
	/*
	ret = connectivity_set_resource("/door/1", "org.tizen.door", &ad->resource_info);
	if (ret == -1) _E("Cannot broadcast resource");
	*/

	/**
	 * Creates a timer to call the given function in the given period of time.
	 * In the control_sensors_cb(), each sensor reads the measured value or writes a specific value to the sensor.
	 */
	/*
	ad->getter_timer = ecore_timer_add( Duration , _control_sensors_cb, ad);
	if (!ad->getter_timer) {
		_E("Failed to add infrared motion getter timer");
		return false;
	}
	*/

    return true;
}

static void service_app_terminate(void *data)
{
	app_data *ad = (app_data *)data;

	if (ad->getter_timer) {
		ecore_timer_del(ad->getter_timer);
	}

	/**
	 * Releases the resource about connectivity.
	 */
	connectivity_unset_resource(ad->resource_info);

	/**
	 * No modification required!!!
	 * Access only when modifying internal functions.
	 */
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
