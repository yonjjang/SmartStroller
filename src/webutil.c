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

#include <stdbool.h>
#include <curl/curl.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>
#include "log.h"
#include "webutil.h"

#define URI_PATH_LEN 64
#define REQ_CON_TIMEOUT 5L

typedef struct _wu_json_handle {
	JsonBuilder *builder;
	bool is_begin;
	bool is_end;
} wu_json_handle;

static wu_json_handle Json_h = {NULL, false, false};

static size_t _post_response_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t res_size = 0;

	res_size = size*nmemb;

	if (res_size > 0)
		_I("POST response : %s", ptr);
	/* What should we do here, if response body has negative message? */

	return res_size;
}

static size_t _get_response_write_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t res_size = 0;
	char **received = (char **)data;

	res_size = size*nmemb;

	if (received && res_size > 0)
		*received = strndup((char *)ptr, size*nmemb);
	else
		_E("fail to get response [res size : %d]", res_size);

	return res_size;
}

int web_util_noti_init(void)
{
	int ret = 0;
	CURLcode result;
	result = curl_global_init(CURL_GLOBAL_DEFAULT);
	if (result != CURLE_OK) {
		_E("curl_global_init() failed: %s",
			curl_easy_strerror(result));
		ret = -1;
	}
	return ret;
}

void web_util_noti_fini(void)
{
	curl_global_cleanup();
	return;
}

int web_util_noti_post(const char *resource, const char *json_data)
{
	int ret = 0;
	CURL *curl = NULL;
	CURLcode response = CURLE_OK;
	struct curl_slist *headers = NULL;

	retv_if(resource == NULL, -1);
	retv_if(json_data == NULL, -1);

	_I("server : %s", resource);
	_I("json_data : %s", json_data);

	curl = curl_easy_init();

	if (!curl) {
		_E("fail to init curl");
		return -1;
	}

	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_URL, resource);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _post_response_write_callback);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, REQ_CON_TIMEOUT);

	response = curl_easy_perform(curl);

	if (response != CURLE_OK) {
		_E("curl_easy_perform() failed: %s",
			curl_easy_strerror(response));
		/* What should we do here, if response is kind of errors? */
		ret = -1;
	}

	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);

	return ret;
}

int web_util_noti_get(const char *resource, char **res)
{
	int ret = 0;
	CURL *curl = NULL;
	CURLcode response = CURLE_OK;

	retv_if(resource == NULL, -1);

	_I("GET to [%s]", resource);

	curl = curl_easy_init();

	if (!curl) {
		_E("fail to init curl");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, resource);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _get_response_write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)res);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "tizen-iot-agent/1.0");
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, REQ_CON_TIMEOUT);

	response = curl_easy_perform(curl);

	if (response != CURLE_OK) {
		_E("curl_easy_perform() failed: %s",
		curl_easy_strerror(response));
		/* What should we do here, if response is kind of errors? */
		ret = -1;
	}

	curl_easy_cleanup(curl);

	return ret;
}

int web_util_json_init(void)
{
	if (Json_h.builder)
		g_object_unref(Json_h.builder);

	Json_h.is_begin = false;
	Json_h.is_end = false;
	Json_h.builder = json_builder_new();
	retv_if(Json_h.builder == NULL, -1);

	return 0;
}

int web_util_json_fini(void)
{

	if (Json_h.builder) {
		g_object_unref(Json_h.builder);
		Json_h.builder = NULL;
	}

	Json_h.is_begin = false;
	Json_h.is_end = false;

	return 0;
}

int web_util_json_begin(void)
{
	retv_if(Json_h.builder == NULL, -1);
	retv_if(Json_h.is_begin == true, -1);
	retv_if(Json_h.is_end == true, -1);

	Json_h.is_begin = true;

	json_builder_begin_object(Json_h.builder);

	return 0;
}

int web_util_json_end(void)
{
	retv_if(Json_h.builder == NULL, -1);
	retv_if(Json_h.is_begin == false, -1);
	retv_if(Json_h.is_end == true, -1);

	json_builder_end_object(Json_h.builder);
	Json_h.is_end = true;

	return 0;
}

int web_util_json_add_int(const char* key, long long int value)
{
	retv_if(!key, -1);

	if (Json_h.builder == NULL) {
		_E("Handle for json is not initialized, call web_util_json_init() first");
		return -1;
	}

	if (Json_h.is_begin == false) {
		_E("json object has not begun, call web_util_json_begin() first");
		return -1;
	}

	if (Json_h.is_end == true) {
		_E("json object has already ended, call web_util_json_begin() first");
		return -1;
	}

	json_builder_set_member_name(Json_h.builder, key);
	json_builder_add_int_value(Json_h.builder, value);

	return 0;
}

int web_util_json_add_double(const char* key, double value)
{
	retv_if(!key, -1);

	if (Json_h.builder == NULL) {
		_E("Handle for json is not initialized, call web_util_json_init() first");
		return -1;
	}

	if (Json_h.is_begin == false) {
		_E("json object has not begun, call web_util_json_begin() first");
		return -1;
	}

	if (Json_h.is_end == true) {
		_E("json object has already ended, call web_util_json_begin() first");
		return -1;
	}

	json_builder_set_member_name(Json_h.builder, key);
	json_builder_add_double_value(Json_h.builder, value);

	return 0;
}

int web_util_json_add_boolean(const char* key, bool value)
{
	retv_if(!key, -1);

	if (Json_h.builder == NULL) {
		_E("Handle for json is not initialized, call web_util_json_init() first");
		return -1;
	}

	if (Json_h.is_begin == false) {
		_E("json object has not begun, call web_util_json_begin() first");
		return -1;
	}

	if (Json_h.is_end == true) {
		_E("json object has already ended, call web_util_json_begin() first");
		return -1;
	}

	json_builder_set_member_name(Json_h.builder, key);
	json_builder_add_boolean_value(Json_h.builder, value);

	return 0;
}

int web_util_json_add_string(const char* key, const char *value)
{
	retv_if(!key, -1);

	if (Json_h.builder == NULL) {
		_E("Handle for json is not initialized, call web_util_json_init() first");
		return -1;
	}

	if (Json_h.is_begin == false) {
		_E("json object has not begun, call web_util_json_begin() first");
		return -1;
	}

	if (Json_h.is_end == true) {
		_E("json object has already ended, call web_util_json_begin() first");
		return -1;
	}

	json_builder_set_member_name(Json_h.builder, key);
	json_builder_add_string_value(Json_h.builder, value);

	return 0;
}

int web_util_json_data_array_begin(void)
{
	int ret = 0;
	retv_if(Json_h.builder == NULL, -1);

	ret = web_util_json_begin();
	retv_if(ret, -1);

	json_builder_set_member_name(Json_h.builder, "SensorDataList");
	json_builder_begin_array(Json_h.builder);

	return 0;
}

int web_util_json_data_array_end(void)
{
	retv_if(Json_h.builder == NULL, -1);
	retv_if(Json_h.is_begin == false, -1);
	retv_if(Json_h.is_end == true, -1);

	json_builder_end_array(Json_h.builder);
	web_util_json_end();

	return 0;
}

int web_util_json_add_sensor_data(const char* sensorpi_id, web_util_sensor_data_s *sensor_data)
{
	const char n_id[] = "SensorPiID";
	const char n_motion[] = "Motion";
	const char n_flame[] = "Flame";
	const char n_hum[] = "Humidity";
	const char n_temp[] = "Temperature";
	const char n_vib[] = "Vibration";
	const char n_co2[] = "CO2";
	const char n_sound[] = "SoundLevel";
	const char n_tilt[] = "Tilt";
	const char n_light[] = "Light";
	const char n_collision[] = "Collision";
	const char n_obstacle[] = "Obstacle";
	const char n_distance[] = "Distance";
	const char n_rain[] = "Rain";
	const char n_touch[] = "Touch";
	const char n_gas[] = "Gas";
	const char n_e_sensor[] = "SensorEnabled";
	const char n_hash[] = "Hash";

	retv_if(!sensorpi_id, -1);
	retv_if(Json_h.builder == NULL, -1);
	retv_if(Json_h.is_begin == false, -1);
	retv_if(Json_h.is_end == true, -1);
	retv_if(sensor_data == NULL, -1);

	/* JSON structure :
	{
		SensorPiID: string,
		Motion: boolean,
		Flame: boolean,
		Humidity: double,
		Temperature: double,
		Vibration: boolean,
		CO2: double,
		SoundLevel: int,
		Tilt: int,
		Light: int,
		Collision: int,
		Obstacle: int,
		Distance: double,
		Rain: int,
		Touch: int,
		Gas: int,
		SensorEnabled: [Motion, ],
		Hash: string,
	}
	*/


	json_builder_begin_object(Json_h.builder);

	json_builder_set_member_name(Json_h.builder, n_id);
	json_builder_add_string_value(Json_h.builder, sensorpi_id);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_MOTION) {
		json_builder_set_member_name(Json_h.builder, n_motion);
		json_builder_add_int_value(Json_h.builder, sensor_data->motion);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_FLAME) {
		json_builder_set_member_name(Json_h.builder, n_flame);
		json_builder_add_int_value(Json_h.builder, sensor_data->flame);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_HUMIDITY) {
		json_builder_set_member_name(Json_h.builder, n_hum);
		json_builder_add_double_value(Json_h.builder, sensor_data->humidity);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_TEMPERATURE) {
		json_builder_set_member_name(Json_h.builder, n_temp);
		json_builder_add_double_value(Json_h.builder, sensor_data->temperature);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_VIB) {
		json_builder_set_member_name(Json_h.builder, n_vib);
		json_builder_add_int_value(Json_h.builder, sensor_data->virbration);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_CO2) {
		json_builder_set_member_name(Json_h.builder, n_co2);
		json_builder_add_double_value(Json_h.builder, sensor_data->co2);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_SOUND) {
		json_builder_set_member_name(Json_h.builder, n_sound);
		json_builder_add_int_value(Json_h.builder, sensor_data->soundlevel);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_TILT) {
		json_builder_set_member_name(Json_h.builder, n_tilt);
		json_builder_add_int_value(Json_h.builder, sensor_data->tilt);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_LIGHT) {
		json_builder_set_member_name(Json_h.builder, n_light);
		json_builder_add_int_value(Json_h.builder, sensor_data->light);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_COLLISION) {
		json_builder_set_member_name(Json_h.builder, n_collision);
		json_builder_add_int_value(Json_h.builder, sensor_data->collision);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_OBSTACLE) {
		json_builder_set_member_name(Json_h.builder, n_obstacle);
		json_builder_add_int_value(Json_h.builder, sensor_data->obstacle);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_ULTRASONIC_DISTANCE) {
		json_builder_set_member_name(Json_h.builder, n_distance);
		json_builder_add_double_value(Json_h.builder, sensor_data->distance);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_RAIN) {
		json_builder_set_member_name(Json_h.builder, n_rain);
		json_builder_add_int_value(Json_h.builder, sensor_data->rain);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_TOUCH) {
		json_builder_set_member_name(Json_h.builder, n_touch);
		json_builder_add_int_value(Json_h.builder, sensor_data->touch);
	}

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_GAS) {
		json_builder_set_member_name(Json_h.builder, n_gas);
		json_builder_add_int_value(Json_h.builder, sensor_data->gas);
	}

	json_builder_set_member_name(Json_h.builder, n_e_sensor);
	json_builder_begin_array(Json_h.builder);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_MOTION)
		json_builder_add_string_value(Json_h.builder, n_motion);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_FLAME)
		json_builder_add_string_value(Json_h.builder, n_flame);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_HUMIDITY)
		json_builder_add_string_value(Json_h.builder, n_hum);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_TEMPERATURE)
		json_builder_add_string_value(Json_h.builder, n_temp);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_VIB)
		json_builder_add_string_value(Json_h.builder, n_vib);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_CO2)
		json_builder_add_string_value(Json_h.builder, n_co2);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_SOUND)
		json_builder_add_string_value(Json_h.builder, n_sound);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_TILT)
		json_builder_add_string_value(Json_h.builder, n_tilt);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_LIGHT)
		json_builder_add_string_value(Json_h.builder, n_light);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_COLLISION)
		json_builder_add_string_value(Json_h.builder, n_collision);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_OBSTACLE)
		json_builder_add_string_value(Json_h.builder, n_obstacle);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_ULTRASONIC_DISTANCE)
		json_builder_add_string_value(Json_h.builder, n_distance);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_RAIN)
		json_builder_add_string_value(Json_h.builder, n_rain);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_TOUCH)
		json_builder_add_string_value(Json_h.builder, n_touch);

	if (sensor_data->enabled_sensor & WEB_UTIL_SENSOR_GAS)
		json_builder_add_string_value(Json_h.builder, n_gas);

	json_builder_end_array(Json_h.builder);

	if (sensor_data->hash) {
		json_builder_set_member_name(Json_h.builder, n_hash);
		json_builder_add_string_value(Json_h.builder, sensor_data->hash);
	}

	json_builder_end_object(Json_h.builder);

	return 0;
}

char *web_util_get_json_string(void)
{
	JsonGenerator *gen = NULL;
	JsonNode *root = NULL;
	char *str = NULL;

	retv_if(Json_h.builder == NULL, NULL);
	retv_if(Json_h.is_begin == false, NULL);
	retv_if(Json_h.is_end == false, NULL);

	root = json_builder_get_root(Json_h.builder);
	retv_if(root == NULL, NULL);

	gen = json_generator_new();
	goto_if(gen == NULL, error_release_all);
	json_generator_set_root(gen, root);

	str = json_generator_to_data(gen, NULL);

	return str;

error_release_all:
	if (root)
		json_node_free(root);

	if (gen)
		g_object_unref(gen);

	return NULL;
}

