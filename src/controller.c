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

#include <Eina.h>

#include "log.h"
#include "controller.h"

struct controller_s {
	Eina_List *event_cb_list;
};
static struct controller_s controller_info;

struct _controller_event_cb_info_s {
	char *event_name;
	controller_event_cb event_cb;
	void *data;
};
typedef struct _controller_event_cb_info_s controller_event_cb_info_s;


int controller_register_event_cb(const char *event_name, controller_event_cb event_cb, void *data)
{
	controller_event_cb_info_s *event_cb_info = NULL;

	retv_if(!event_name, -1);
	retv_if(!event_cb, -1);

	event_cb_info = calloc(1, sizeof(controller_event_cb_info_s));
	retv_if(!event_cb_info, -1);

	event_cb_info->event_name = strdup(event_name);
	goto_if(!event_cb_info->event_name, error);

	event_cb_info->event_cb = event_cb;
	event_cb_info->data = data;

	controller_info.event_cb_list = eina_list_append(controller_info.event_cb_list, event_cb_info);

	return 0;

error:
	if (event_cb_info) free(event_cb_info);

	return -1;
}

int controller_unregister_event_cb(const char *event_name, controller_event_cb event_cb)
{
	controller_event_cb_info_s *event_cb_info = NULL;
	const Eina_List *l = NULL;
	const Eina_List *ln = NULL;

	retv_if(!event_name, -1);
	retv_if(!event_cb, -1);

	EINA_LIST_FOREACH_SAFE(controller_info.event_cb_list, l, ln, event_cb_info) {
		if (event_cb_info->event_name 
			&& strcmp(event_cb_info->event_name, event_name)
			&& event_cb_info->event_cb == event_cb)
		{
			controller_info.event_cb_list = eina_list_remove(controller_info.event_cb_list, event_cb_info);
			break;
		}
	}

	return 0;
}

int controller_send_event(const char *event_name, void *event_info)
{
	controller_event_cb_info_s *event_cb_info = NULL;
	const Eina_List *l = NULL;
	const Eina_List *ln = NULL;

	retv_if(!event_name, -1);

	EINA_LIST_FOREACH_SAFE(controller_info.event_cb_list, l, ln, event_cb_info) {
		if (event_cb_info->event_name 
			&& strcmp(event_cb_info->event_name, event_name))
		{
			int ret = -1;
			ret = event_cb_info->event_cb(event_name, event_info, event_cb_info->data);
			if (ret < 0) _E("There were errors sending an event");
			break;
		}
	}

	return 0;
}
