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

#ifndef __POSITION_FINDER_CONNECTIVITY_H__
#define __POSITION_FINDER_CONNECTIVITY_H__

#include "connectivity_internal.h"

struct connectivity_resource {
	iotcon_resource_h res;
	iotcon_observers_h observers;
};

typedef struct connectivity_resource connectivity_resource_s;

extern int connectivity_set_resource(const char *uri_path, const char *type, connectivity_resource_s **out_resource_info);
extern void connectivity_unset_resource(connectivity_resource_s *resource);

extern int connectivity_notify_bool(connectivity_resource_s *resource_info, const char *key, bool value);
extern int connectivity_notify_int(connectivity_resource_s *resource_info, const char *key, int value);
extern int connectivity_notify_double(connectivity_resource_s *resource_info, const char *key, double value);

#endif /* __POSITION_FINDER_CONNECTIVITY_H__ */
