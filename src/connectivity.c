/*
 * Copyright (c) 2015 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
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
#include <stdbool.h>
#include <glib.h>
#include <Eina.h>

#include <iotcon.h>

#include "log.h"
#include "connectivity.h"

#define ULTRASONIC_RESOURCE_1_URI "/door/1"
#define ULTRASONIC_RESOURCE_2_URI "/door/2"
#define ULTRASONIC_RESOURCE_TYPE "org.tizen.door"

static void _request_resource_handler(iotcon_resource_h resource, iotcon_request_h request, void *user_data);

static int _send_response(iotcon_request_h request, iotcon_representation_h representation, iotcon_response_result_e result)
{
	int ret = -1;
	iotcon_response_h response;

	ret = iotcon_response_create(request, &response);
	retv_if(IOTCON_ERROR_NONE != ret, -1);

	ret = iotcon_response_set_result(response, result);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_response_set_representation(response, representation);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_response_send(response);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	iotcon_response_destroy(response);

	return 0;

error:
	iotcon_response_destroy(response);
	return -1;
}

static void _destroy_representation(iotcon_representation_h representation)
{
	ret_if(!representation);
	iotcon_representation_destroy(representation);
}

static iotcon_representation_h _create_representation_with_bool(iotcon_resource_h res, const char *key, bool value)
{
	iotcon_attributes_h attributes = NULL;
	iotcon_representation_h representation = NULL;
	char *uri_path = NULL;
	int ret = -1;

	ret = iotcon_resource_get_uri_path(res, &uri_path);
	retv_if(IOTCON_ERROR_NONE != ret, NULL);

	ret = iotcon_representation_create(&representation);
	retv_if(IOTCON_ERROR_NONE != ret, NULL);

	ret = iotcon_attributes_create(&attributes);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_representation_set_uri_path(representation, uri_path);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_attributes_add_bool(attributes, "opened", value);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_representation_set_attributes(representation, attributes);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	iotcon_attributes_destroy(attributes);

	return representation;

error:
	if (attributes) iotcon_attributes_destroy(attributes);
	if (representation) iotcon_representation_destroy(representation);

	return NULL;
}

static iotcon_representation_h _create_representation_with_int(iotcon_resource_h res, const char *key, int value)
{
	iotcon_attributes_h attributes = NULL;
	iotcon_representation_h representation = NULL;
	char *uri_path = NULL;
	int ret = -1;

	ret = iotcon_resource_get_uri_path(res, &uri_path);
	retv_if(IOTCON_ERROR_NONE != ret, NULL);

	ret = iotcon_representation_create(&representation);
	retv_if(IOTCON_ERROR_NONE != ret, NULL);

	ret = iotcon_attributes_create(&attributes);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_representation_set_uri_path(representation, uri_path);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_attributes_add_int(attributes, "opened", value);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_representation_set_attributes(representation, attributes);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	iotcon_attributes_destroy(attributes);

	return representation;

error:
	if (attributes) iotcon_attributes_destroy(attributes);
	if (representation) iotcon_representation_destroy(representation);

	return NULL;
}

static iotcon_representation_h _create_representation_with_double(iotcon_resource_h res, const char *key, double value)
{
	iotcon_attributes_h attributes = NULL;
	iotcon_representation_h representation = NULL;
	char *uri_path = NULL;
	int ret = -1;

	ret = iotcon_resource_get_uri_path(res, &uri_path);
	retv_if(IOTCON_ERROR_NONE != ret, NULL);

	ret = iotcon_representation_create(&representation);
	retv_if(IOTCON_ERROR_NONE != ret, NULL);

	ret = iotcon_attributes_create(&attributes);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_representation_set_uri_path(representation, uri_path);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_attributes_add_double(attributes, "opened", value);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_representation_set_attributes(representation, attributes);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	iotcon_attributes_destroy(attributes);

	return representation;

error:
	if (attributes) iotcon_attributes_destroy(attributes);
	if (representation) iotcon_representation_destroy(representation);

	return NULL;
}

int connectivity_notify_bool(connectivity_resource_s *resource_info, const char *key, bool value)
{
	iotcon_representation_h representation;
	int ret = -1;

	retv_if(!resource_info, -1);
	retv_if(!resource_info->observers, -1);

	_D("Notify the value[%d]", value);

	representation = _create_representation_with_bool(resource_info->res, key, value);
	retv_if(!representation, -1);

	ret = iotcon_resource_notify(resource_info->res, representation, resource_info->observers, IOTCON_QOS_HIGH);
	retv_if(IOTCON_ERROR_NONE != ret, -1);

	_destroy_representation(representation);

	return 0;
}

int connectivity_notify_int(connectivity_resource_s *resource_info, const char *key, int value)
{
	iotcon_representation_h representation;
	int ret = -1;

	retv_if(!resource_info, -1);
	retv_if(!resource_info->observers, -1);

	_D("Notify the value[%d]", value);

	representation = _create_representation_with_int(resource_info->res, key, value);
	retv_if(!representation, -1);

	ret = iotcon_resource_notify(resource_info->res, representation, resource_info->observers, IOTCON_QOS_HIGH);
	retv_if(IOTCON_ERROR_NONE != ret, -1);

	_destroy_representation(representation);

	return 0;
}

int connectivity_notify_double(connectivity_resource_s *resource_info, const char *key, double value)
{
	iotcon_representation_h representation;
	int ret = -1;

	retv_if(!resource_info, -1);
	retv_if(!resource_info->observers, -1);

	_D("Notify the value[%f]", value);

	representation = _create_representation_with_double(resource_info->res, key, value);
	retv_if(!representation, -1);

	ret = iotcon_resource_notify(resource_info->res, representation, resource_info->observers, IOTCON_QOS_HIGH);
	retv_if(IOTCON_ERROR_NONE != ret, -1);

	_destroy_representation(representation);

	return 0;
}

static bool _query_cb(const char *key, const char *value, void *user_data)
{
	_D("Key : [%s], Value : [%s]", key, value);

	return IOTCON_FUNC_CONTINUE;
}

static int _handle_query(iotcon_request_h request)
{
	iotcon_query_h query = NULL;
	int ret = -1;

	ret = iotcon_request_get_query(request, &query);
	retv_if(IOTCON_ERROR_NONE != ret, -1);

	if (query) iotcon_query_foreach(query, _query_cb, NULL);

	return 0;
}

static int _handle_request_by_crud_type(iotcon_request_h request, connectivity_resource_s *resource_info)
{
	iotcon_request_type_e type;
	int ret = -1;

	ret = iotcon_request_get_request_type(request, &type);
	retv_if(IOTCON_ERROR_NONE != ret, -1);

	switch (type) {
	case IOTCON_REQUEST_GET:
		_I("Do not support 'get' query");
		break;
	case IOTCON_REQUEST_PUT:
		_I("Do not support 'put' query");
		break;
	case IOTCON_REQUEST_POST:
		_I("Do not support 'post' query");
		break;
	case IOTCON_REQUEST_DELETE:
		_I("Do not support 'delete' query");
		break;
	default:
		_E("Cannot reach here");
		ret = -1;
		break;
	}
	retv_if(0 != ret, -1);

	return 0;
}

static int _handle_observer(iotcon_request_h request, iotcon_observers_h observers)
{
	iotcon_observe_type_e observe_type;
	int ret = -1;
	int observe_id = -1;

	ret = iotcon_request_get_observe_type(request, &observe_type);
	retv_if(IOTCON_ERROR_NONE != ret, -1);

	if (IOTCON_OBSERVE_REGISTER == observe_type) {
		ret = iotcon_request_get_observe_id(request, &observe_id);
		retv_if(IOTCON_ERROR_NONE != ret, -1);

		ret = iotcon_observers_add(observers, observe_id);
		retv_if(IOTCON_ERROR_NONE != ret, -1);
	} else if (IOTCON_OBSERVE_DEREGISTER == observe_type) {
		ret = iotcon_request_get_observe_id(request, &observe_id);
		retv_if(IOTCON_ERROR_NONE != ret, -1);

		ret = iotcon_observers_remove(observers, observe_id);
		retv_if(IOTCON_ERROR_NONE != ret, -1);
	}

	return 0;
}

static void _request_resource_handler(iotcon_resource_h resource, iotcon_request_h request, void *user_data)
{
	connectivity_resource_s *resource_info = user_data;
	int ret = -1;
	char *host_address = NULL;

	ret_if(!request);

	ret = iotcon_request_get_host_address(request, &host_address);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	_D("Host address : %s", host_address);

	ret = _handle_query(request);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = _handle_request_by_crud_type(request, resource_info);
	goto_if(0 != ret, error);

	ret = _handle_observer(request, resource_info->observers);
	goto_if(0 != ret, error);

	return;

error:
	_send_response(request, NULL, IOTCON_RESPONSE_ERROR);
}

int connectivity_init(void)
{
	int ret = -1;

	ret = iotcon_initialize("/home/owner/apps_rw/org.tizen.position-finder-server/data/iotcon-test-svr-db-server.dat");
	retv_if(IOTCON_ERROR_NONE != ret, -1);

	ret = iotcon_set_device_name(ULTRASONIC_RESOURCE_TYPE);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_start_presence(10);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	return 0;

error:
	iotcon_deinitialize();
	return -1;
}

int connectivity_fini(void)
{
	iotcon_deinitialize();
	return 0;
}

void connectivity_unset_resource(connectivity_resource_s *resource_info)
{
	ret_if(!resource_info);
	if (resource_info->observers) iotcon_observers_destroy(resource_info->observers);
	if (resource_info->res) iotcon_resource_destroy(resource_info->res);
	free(resource_info);
}

int connectivity_set_resource(const char *uri_path, const char *type, connectivity_resource_s **out_resource_info)
{
	iotcon_resource_types_h resource_types = NULL;
	iotcon_resource_interfaces_h ifaces = NULL;
	connectivity_resource_s *resource_info = NULL;
	uint8_t policies;
	int ret = -1;

	resource_info = calloc(1, sizeof(connectivity_resource_s));
	retv_if(!resource_info, -1);
	*out_resource_info = resource_info;

	ret = iotcon_resource_types_create(&resource_types);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_resource_types_add(resource_types, type);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_resource_interfaces_create(&ifaces);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_resource_interfaces_add(ifaces, IOTCON_INTERFACE_DEFAULT);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_resource_interfaces_add(ifaces, IOTCON_INTERFACE_BATCH);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	policies =
		IOTCON_RESOURCE_DISCOVERABLE |
		IOTCON_RESOURCE_OBSERVABLE |
		IOTCON_RESOURCE_SECURE;

	ret = iotcon_resource_create(uri_path,
			resource_types,
			ifaces,
			policies,
			_request_resource_handler,
			resource_info,
			&resource_info->res);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_observers_create(&resource_info->observers);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	iotcon_resource_types_destroy(resource_types);
	iotcon_resource_interfaces_destroy(ifaces);

	return 0;

error:
	if (ifaces) iotcon_resource_interfaces_destroy(ifaces);
	if (resource_types) iotcon_resource_types_destroy(resource_types);
	if (resource_info->res) iotcon_resource_destroy(resource_info->res);
	if (resource_info) free(resource_info);

	return -1;
}
