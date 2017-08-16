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

/**
 * @brief Create connectivity resource and registers the resource in server.
 * @param[in] uri_path The URI path of the resource
 * @param[in] type The string data to insert into the resource types (e.g. "org.tizen.light")
 * @param[out] out_resource_info A structure containing information about connectivity resource
 * @return 0 on success, otherwise a negative error value
 * @see uri_path length must be less than 128.
 * @see You must destroy resource by calling connectivity_unset_resource() if resource is no longer needed.
 */
extern int connectivity_set_resource(const char *uri_path, const char *type, connectivity_resource_s **out_resource_info);

/**
 * @brief Releases all resource about connectivity.
 * @param[in] resource_info A structure containing information about connectivity resource
 */
extern void connectivity_unset_resource(connectivity_resource_s *resource);

/**
 * @brief Notifies specific clients that resource's attributes have changed with boolean vaule.
 * @param[in] resource_info A structure containing information about connectivity resource
 * @param[in] key A new key to be added into attributes
 * @param[in] value A boolean value to be added into attributes
 * @return 0 on success, otherwise a negative error value
 * @see If key is already exists, current value will be replaced with new value.
 */
extern int connectivity_notify_bool(connectivity_resource_s *resource_info, const char *key, bool value);

/**
 * @brief Notifies specific clients that resource's attributes have changed with int vaule.
 * @param[in] resource_info A structure containing information about connectivity resource
 * @param[in] key A new key to be added into attributes
 * @param[in] value A int value to be added into attributes
 * @return 0 on success, otherwise a negative error value
 * @see If key is already exists, current value will be replaced with new value.
 */
extern int connectivity_notify_int(connectivity_resource_s *resource_info, const char *key, int value);

/**
 * @brief Notifies specific clients that resource's attributes have changed with double vaule.
 * @param[in] resource_info A structure containing information about connectivity resource
 * @param[in] key A new key to be added into attributes
 * @param[in] value A double value to be added into attributes
 * @return 0 on success, otherwise a negative error value
 * @see If key is already exists, current value will be replaced with new value.
 */
extern int connectivity_notify_double(connectivity_resource_s *resource_info, const char *key, double value);

#endif /* __POSITION_FINDER_CONNECTIVITY_H__ */
