/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __POSITION_FINDER_MODEL_H__
#define __POSITION_FINDER_MODEL_H__

enum sensor_type {
	SENSOR_TYPE_ULTRASONIC,
	SENSOR_TYPE_INFRARED_MOTION, /* HC_SR501 */
	SENSOR_TYPE_INFRARED_OBSTACLE_AVOIDANCE,
	SENSOR_TYPE_TOUCH,
};
typedef enum sensor_type sensor_type_e;

typedef struct _model_sensor_s model_sensor_s;
typedef model_sensor_s *model_sensor_h;

extern int model_init(const char *id, sensor_type_e sensor_type, int gpio_num1, int gpio_num2, model_sensor_h *out_info);
extern void model_fini(model_sensor_h info);

extern int model_read_int_value(model_sensor_h info, int *out_value);
extern int model_read_double_value(model_sensor_h info, double *out_value);

extern int model_list_add_sensor(model_sensor_h info);
extern int model_list_remove_sensor(model_sensor_h info);
extern int model_list_get_sensor(const char *id, model_sensor_h *out_info);
extern int model_list_foreach(void (*cb)(model_sensor_h info));

#endif /* __POSITION_FINDER_MODEL_H__ */
