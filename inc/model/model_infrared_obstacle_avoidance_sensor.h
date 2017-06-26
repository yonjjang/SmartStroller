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

#ifndef __POSITION_FINDER_MODEL_INFRARED_OBSTACLE_AVOIDANCE_SENSOR_H__
#define __POSITION_FINDER_MODEL_INFRARED_OBSTACLE_AVOIDANCE_SENSOR_H__

typedef struct infrared_obstacle_avoidance_event infrared_obstacle_avoidance_event_s;

extern int model_init_infrared_obstacle_avoidance_sensor(void);
extern void model_fini_infrared_obstacle_avoidance_sensor(void);

extern int model_read_infrared_obstacle_avoidance_sensor(int *out_value);

#endif /* __POSITION_FINDER_MODEL_INFRARED_OBSTACLE_AVOIDANCE_SENSOR_H__ */
