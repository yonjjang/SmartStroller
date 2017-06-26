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

#ifndef __POSITION_FINDER_MODEL_H__
#define __POSITION_FINDER_MODEL_H__

enum sensor_type {
	SENSOR_TYPE_ULTRASONIC,
	SENSOR_TYPE_INFRARED_MOTION, /* HC_SR501 */
	SENSOR_TYPE_INFRARED_OBSTACLE_AVOIDANCE, /* HC_SR501 */
};
typedef enum sensor_type sensor_type_e;

extern int model_init(sensor_type_e sensor_type);
extern void model_fini(void);

extern int model_alloc(void **data);

extern int model_read_int_value(int *out_value);
extern int model_write(void *data);

#endif /* __POSITION_FINDER_MODEL_H__ */
