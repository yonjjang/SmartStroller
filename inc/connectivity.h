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

#ifndef __POSITION_FINDER_CONNECTIVITY_H__
#define __POSITION_FINDER_CONNECTIVITY_H__

typedef struct connectivity_resource connectivity_resource_s;
extern int connectivity_notify(connectivity_resource_s *user_data, int value);

extern int connectivity_init(const char *device_name);
extern int connectivity_fini(void);

extern void connectivity_destroy_resource(connectivity_resource_s *resource);
extern int connectivity_create_resource(const char *uri_path, const char *type, connectivity_resource_s **out_resource);

#endif /* __POSITION_FINDER_CONNECTIVITY_H__ */
