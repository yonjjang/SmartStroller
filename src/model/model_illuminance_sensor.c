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

#include <stdlib.h>
#include <unistd.h>
#include <peripheral_io.h>
#include <sys/time.h>

#include "log.h"
#include "model_internal.h"

#define I2C_PIN_MAX 28
/* I2C */
#define GY30_ADDR 0x23 /* Address of GY30 light sensor */
#define GY30_CONT_HIGH_RES_MODE 0x10 /* Start measurement at 11x resolution. Measurement time is approx 120mx */
#define GY30_CONSTANT_NUM (1.2)

static struct {
	int opened;
	peripheral_i2c_h sensor_h;
} model_sensor_s;

void model_close_illuminance_sensor(void)
{
	ret_if(!model_sensor_s.opened);

	_I("Infrared Motion Sensor is finishing...");
	peripheral_i2c_close(model_sensor_s.sensor_h);
	model_sensor_s.opened = 0;
}

/* You have to use this illuminance sensor ONLY ONE in the pi board */
int model_read_illuminance_sensor(int i2c_bus, int *out_value)
{
	int ret = PERIPHERAL_ERROR_NONE;
	unsigned char buf[10] = { 0, };

	if (!model_sensor_s.opened) {
		ret = peripheral_i2c_open(i2c_bus, GY30_ADDR, &model_sensor_s.sensor_h);
		retv_if(!model_sensor_s.sensor_h, -1);
		model_sensor_s.opened = 1;
	}

	ret = peripheral_i2c_write_byte(model_sensor_s.sensor_h, GY30_CONT_HIGH_RES_MODE);
	retv_if(ret < 0, -1);

	ret = peripheral_i2c_read(model_sensor_s.sensor_h, buf, 2);
	retv_if(ret < 0, -1);

	*out_value = (buf[0] << 8 | buf[1]) / GY30_CONSTANT_NUM; // Just Sum High 8bit and Low 8bit

	_I("Infrared Motion Sensor Value : %d", *out_value);

	return 0;
}
