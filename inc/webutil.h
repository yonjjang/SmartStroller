/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Contact: Jin Yoon <jinny.yoon@samsung.com>
 *          Geunsun Lee <gs86.lee@samsung.com>
 *          Eunyoung Lee <ey928.lee@samsung.com>
 *          Junkyu Han <junkyu.han@samsung.com>
 *          Jeonghoon Park <jh1979.park@samsung.com>
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


#ifndef __POSITION_FINDER_WEBUTIL_H__
#define __POSITION_FINDER_WEBUTIL_H__

typedef enum {
	WEB_UTIL_SENSOR_NONE = 0,
	WEB_UTIL_SENSOR_MOTION = (1 << 0), /* IR motion sensor */
	WEB_UTIL_SENSOR_FLAME = (1 << 1), /* flame sensor */
	WEB_UTIL_SENSOR_HUMIDITY = (1 << 2), /* humidity sensor */
	WEB_UTIL_SENSOR_TEMPERATURE = (1 << 3), /* temperature sensor */
	WEB_UTIL_SENSOR_VIB = (1 << 4), /* vibration sensor */
	WEB_UTIL_SENSOR_CO2 = (1 << 5), /* CO2 sensor */
	WEB_UTIL_SENSOR_SOUND = (1 << 6), /* noise sensor */
} web_util_sensor_type_e;

typedef struct _web_util_sensor_data_s web_util_sensor_data_s;
struct _web_util_sensor_data_s {
	int motion;
	int flame;
	double humidity;
	double temperature;
	int virbration;
	double co2;
	int soundlevel;
	web_util_sensor_type_e enabled_sensor;
	char *hash;
};

int web_util_noti_init(void);
void web_util_noti_fini(void);
int web_util_noti_post(const char *resource, const char *json_data);

int web_util_json_init(void);
int web_util_json_fini(void);
int web_util_json_data_array_begin(void);
int web_util_json_data_array_end(void);
int web_util_json_add_sensor_data(const char* sensorpi_id, web_util_sensor_data_s *sensor_data);
char *web_util_get_json_string(void);

#endif /* __POSITION_FINDER_WEBUTIL_H__ */