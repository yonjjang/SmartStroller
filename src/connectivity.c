/*
 * Copyright (c) 2015 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
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
#include <glib.h>

#include <iotcon.h>
#include "log.h"

#define ULTRASONIC_RESOURCE_1_URI "/door/1"
#define ULTRASONIC_RESOURCE_2_URI "/door/2"
#define ULTRASONIC_RESOURCE_TYPE "org.tizen.door"

struct connectivity_resource {
	iotcon_resource_h res;
	iotcon_observers_h observers;
	iotcon_representation_h repr;
};
typedef struct connectivity_resource connectivity_resource_s;

static bool _resource_created;

static void _request_handler(iotcon_resource_h resource, iotcon_request_h request, void *user_data);

static int _send_response(iotcon_request_h request, iotcon_representation_h repr,
		iotcon_response_result_e result)
{
	int ret;
	iotcon_response_h response;

	ret = iotcon_response_create(request, &response);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_response_create() Fail(%d)", ret);
		return -1;
	}

	ret = iotcon_response_set_result(response, result);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_response_set_result() Fail(%d)", ret);
		iotcon_response_destroy(response);
		return -1;
	}

	ret = iotcon_response_set_representation(response, repr);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_response_set_representation() Fail(%d)", ret);
		iotcon_response_destroy(response);
		return -1;
	}

	/* send Representation to the client */
	ret = iotcon_response_send(response);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_response_send() Fail(%d)", ret);
		iotcon_response_destroy(response);
		return -1;
	}

	iotcon_response_destroy(response);

	return 0;
}

static iotcon_representation_h _create_representation(connectivity_resource_s *door, bool value)
{
	iotcon_attributes_h attributes = NULL;
	iotcon_representation_h repr = NULL;
	char *uri_path = NULL;
	int ret;

	ret = iotcon_resource_get_uri_path(resource->res, &uri_path);
	retv_if(IOTCON_ERROR_NONE != ret, NULL);

	ret = iotcon_representation_create(&repr);
	retv_if(IOTCON_ERROR_NONE != ret, NULL);

	ret = iotcon_attributes_create(&attributes);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_representation_set_uri_path(repr, uri_path);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_attributes_add_bool(attributes, "opened", value);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_representation_set_attributes(repr, attributes);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	iotcon_attributes_destroy(attributes);

	return repr;

error:
	if (attributes) iotcon_attributes_destroy(attributes);
	if (repr) iotcon_representation_destroy(repr);

	return NULL;
}

static int _request_handler_get(connectivity_resource_s *door, iotcon_request_h request)
{
	int ret;
	iotcon_representation_h resp_repr;
	_D("GET request");

	resp_repr = _create_representation(door);
	if (NULL == resp_repr) {
		_E("_create_representation() Fail");
		return -1;
	}

	ret = _send_response(request, resp_repr, IOTCON_RESPONSE_OK);
	if (0 != ret) {
		_E("_send_response() Fail(%d)", ret);
		iotcon_representation_destroy(resp_repr);
		return -1;
	}

	iotcon_representation_destroy(resp_repr);

	return 0;
}

static int _set_door_representation(connectivity_resource_s *door,
		iotcon_representation_h repr)
{
	int ret;
	bool bval;
	iotcon_attributes_h attributes;

	ret = iotcon_representation_get_attributes(repr, &attributes);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_representation_get_attributes() Fail(%d)", ret);
		return -1;
	}

	ret = iotcon_attributes_get_bool(attributes, "opened", &bval);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_attributes_get_bool() Fail(%d)", ret);
		return -1;
	}

	door->attributes = bval;

	return 0;
}

static int _request_handler_put(connectivity_resource_s *door, iotcon_request_h request)
{
	int ret;
	iotcon_representation_h req_repr, resp_repr;
	_D("PUT request");

	ret = iotcon_request_get_representation(request, &req_repr);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_request_get_representation() Fail(%d)", ret);
		return -1;
	}

	ret = _set_door_representation(door, req_repr);
	if (0 != ret) {
		_E("_set_door_representation() Fail(%d)", ret);
		return -1;
	}

	/* FIXME : We need to check the sensor here */

	resp_repr = _create_representation(door);
	if (NULL == resp_repr) {
		_E("_create_representation() Fail");
		return -1;
	}

	ret = _send_response(request, resp_repr, IOTCON_RESPONSE_OK);
	if (0 != ret) {
		_E("_send_response() Fail(%d)", ret);
		iotcon_representation_destroy(resp_repr);
		return -1;
	}

	/* notify */
	ret = iotcon_resource_notify(door->handle, resp_repr, door->observers, IOTCON_QOS_HIGH);
	if (IOTCON_ERROR_NONE != ret)
		_E("iotcon_resource_notify() Fail(%d)", ret);

	iotcon_representation_destroy(resp_repr);

	return 0;
}

int connectivity_notify(connectivity_resource_s *resource, int value)
{
	int ret;
	static int i = 0;
	iotcon_representation_h repr;
	connectivity_resource_s *door = user_data;

	retv_if(!resource, -1);
	retv_if(!resource->observers, -1);

	_D("Notify the value[%d]", value);

	repr = _create_representation(door);
	if (NULL == repr) {
		_E("_create_representation() Fail");
		return G_SOURCE_REMOVE;
	}

	ret = iotcon_resource_notify(door->handle, repr, door->observers, IOTCON_QOS_HIGH);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_resource_notify() Fail(%d)", ret);
		iotcon_representation_destroy(repr);
		return G_SOURCE_REMOVE;
	}

	iotcon_representation_destroy(repr);
	return G_SOURCE_CONTINUE;
}

static int _request_handler_post(connectivity_resource_s *door, iotcon_request_h request)
{
	int ret;
	iotcon_attributes_h resp_attributes;
	iotcon_representation_h resp_repr = NULL;
	iotcon_resource_h new_door_handle;
	_D("POST request");

	if (_resource_created) {
		_E("Resource(%s) is already created", ULTRASONIC_RESOURCE_2_URI);
		return -1;
	}

	/* FIXME */
	ret = connectivity_create_resource(ULTRASONIC_RESOURCE_2_URI, door->type, NULL, &new_door_handle);
	if (NULL == new_door_handle) {
		_E("_create_door_resource() Fail");
		return -1;
	}
	_resource_created = true;

	/* send information that new resource was created */
	ret = iotcon_representation_create(&resp_repr);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_representation_create() Fail(%d)", ret);
		return -1;
	}

	ret = iotcon_attributes_create(&resp_attributes);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_attributes_create() Fail(%d)", ret);
		iotcon_representation_destroy(resp_repr);
		return -1;
	}

	ret = iotcon_attributes_add_str(resp_attributes, "createduripath", ULTRASONIC_RESOURCE_2_URI);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_attributes_add_str() Fail(%d)", ret);
		iotcon_attributes_destroy(resp_attributes);
		iotcon_representation_destroy(resp_repr);
		return -1;
	}

	ret = iotcon_representation_set_attributes(resp_repr, resp_attributes);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_representation_set_attributes() Fail(%d)", ret);
		iotcon_attributes_destroy(resp_attributes);
		iotcon_representation_destroy(resp_repr);
		return -1;
	}

	iotcon_attributes_destroy(resp_attributes);

	ret = _send_response(request, resp_repr, IOTCON_RESPONSE_RESOURCE_CREATED);
	if (0 != ret) {
		_E("_send_response() Fail(%d)", ret);
		iotcon_representation_destroy(resp_repr);
		return -1;
	}

	iotcon_representation_destroy(resp_repr);

	return 0;
}

static int _request_handler_delete(iotcon_resource_h resource,
		iotcon_request_h request)
{
	int ret;
	_D("DELETE request");

	ret = iotcon_resource_destroy(resource);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_resource_destroy() Fail(%d)", ret);
		return -1;
	}

	ret = _send_response(request, NULL, IOTCON_RESPONSE_RESOURCE_DELETED);
	if (0 != ret) {
		_E("_send_response() Fail(%d)", ret);
		return -1;
	}

	return 0;
}

static bool _query_cb(const char *key, const char *value, void *user_data)
{
	_D("key : %s, value : %s", key, value);

	return IOTCON_FUNC_CONTINUE;
}

static void _request_handler(iotcon_resource_h resource, iotcon_request_h request,
		void *user_data)
{
	connectivity_resource_s *door = user_data;
	iotcon_query_h query;
	int ret, observe_id;
	iotcon_request_type_e type;
	iotcon_observe_type_e observe_type;
	char *host_address;

	ret_if(NULL == request);

	ret = iotcon_request_get_host_address(request, &host_address);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_request_get_host_address() Fail(%d)", ret);
		_send_response(request, NULL, IOTCON_RESPONSE_ERROR);
		return;
	}
	_D("host_address : %s", host_address);

	ret = iotcon_request_get_query(request, &query);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_request_get_query() Fail(%d)", ret);
		_send_response(request, NULL, IOTCON_RESPONSE_ERROR);
		return;
	}
	if (query)
		iotcon_query_foreach(query, _query_cb, NULL);

	ret = iotcon_request_get_request_type(request, &type);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_request_get_request_type() Fail(%d)", ret);
		_send_response(request, NULL, IOTCON_RESPONSE_ERROR);
		return;
	}

	if (IOTCON_REQUEST_GET == type)
		ret = _request_handler_get(door, request);

	else if (IOTCON_REQUEST_PUT == type)
		ret = _request_handler_put(door, request);

	else if (IOTCON_REQUEST_POST == type)
		ret = _request_handler_post(door, request);

	else if (IOTCON_REQUEST_DELETE == type)
		ret = _request_handler_delete(resource, request);

	if (0 != ret) {
		_send_response(request, NULL, IOTCON_RESPONSE_ERROR);
		return;
	}

	ret = iotcon_request_get_observe_type(request, &observe_type);
	if (IOTCON_ERROR_NONE != ret) {
		_E("iotcon_request_get_observe_type() Fail(%d)", ret);
		return;
	}

	if (IOTCON_OBSERVE_REGISTER == observe_type) {
		ret = iotcon_request_get_observe_id(request, &observe_id);
		if (IOTCON_ERROR_NONE != ret) {
			_E("iotcon_request_get_observe_id() Fail(%d)", ret);
			return;
		}

		ret = iotcon_observers_add(door->observers, observe_id);
		if (IOTCON_ERROR_NONE != ret) {
			_E("iotcon_observers_add() Fail(%d)", ret);
			return;
		}
	} else if (IOTCON_OBSERVE_DEREGISTER == observe_type) {
		ret = iotcon_request_get_observe_id(request, &observe_id);
		if (IOTCON_ERROR_NONE != ret) {
			_E("iotcon_request_get_observe_id() Fail(%d)", ret);
			return;
		}
		ret = iotcon_observers_remove(door->observers, observe_id);
		if (IOTCON_ERROR_NONE != ret) {
			_E("iotcon_observers_remove() Fail(%d)", ret);
			return;
		}
	}
}


int connectivity_init(void)
{
	int ret = 0;

	ret = iotcon_initialize("../res/iotcon-test-svr-db-server.dat");
	retv_if(IOTCON_ERROR_NONE != ret, -1);

	ret = iotcon_set_device_name("iotcon-test-basic-server");
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_start_presence(10);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	return 0;

error:
	iotcon_deinitialize();
	return -1;
}

int connectivity_fini()
{
	iotcon_deinitialize();
}

void connectivity_destroy_resource(connectivity_resource_s *resource)
{
	iotcon_observers_destroy(resource->observers);
}

int connectivity_create_resource(const char *uri_path, const char *type, void *data, connectivity_resource_s *resource)
{
	iotcon_resource_types_h resource_types = NULL;
	iotcon_resource_interfaces_h ifaces = NULL;
	iotcon_resource_h handle = NULL;
	uint8_t policies;
	int ret;

	ret = iotcon_resource_types_create(&resource_types);
	retv_if(IOTCON_ERROR_NONE != ret, error);

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

	ret = iotcon_resource_create(uri_path, resource_types, ifaces, policies, _request_handler, data, &resource->res);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	ret = iotcon_observers_create(&resource->observers);
	goto_if(IOTCON_ERROR_NONE != ret, error);

	iotcon_resource_types_destroy(resource_types);
	iotcon_resource_interfaces_destroy(ifaces);

	return 0;

error:
	if (resource->res) iotcon_resource_destroy(my_door.handle);
	if (ifaces) iotcon_resource_interfaces_destroy(ifaces);
	if (resource_types) iotcon_resource_types_destroy(resource_types);
	return -1;
}



int main(int argc, char **argv)
{
	connectivity_resource_s resource = {0, };

	ret = connectivity_init();
	retv_if(0 != ret, -1);

	ret = connectivity_create_resource(ULTRASONIC_RESOURCE_1_URI, ULTRASONIC_RESOURCE_TYPE, NULL, &resource)
	retv_if(0 != ret, -1);

	/* FIXME */
	g_timeout_add_seconds(5, connectivity_notify, &my_door);

	/* Enter the mainloop */

	connectivity_destroy_resource(&my_door);
	connectivity_fini();

	return 0;
}
