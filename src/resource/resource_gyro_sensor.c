/*
 *
 *
 * Ewha Womans University, Computer Science & Engineering
 *
 * 1515029 Jeong-min Seo <chersoul@gmail.com>
 * 1515013 Seung-Yun Kim <fic1214@gmail.com>
 *
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <peripheral_io.h>
#include "log.h"
#include "resource/resource_gyro_sensor.h"

#define RPI3_I2C_BUS 1

/* Registers/etc: */
#define MPU6050_Address 0x68	/*Device Address/Identifier for MPU6050*/
#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define INT_ENABLE   0x38
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H  0x43
#define GYRO_YOUT_H  0x45
#define GYRO_ZOUT_H  0x47

/* Bits: */
#define RESTART            0x80
#define SLEEP              0x10
#define ALLCALL            0x01
#define INVRT              0x10
#define OUTDRV             0x04

static peripheral_i2c_h g_i2c_h = NULL;
static unsigned int ref_count = 0;
float Angle_x=0;
<<<<<<< HEAD
=======
float Angle_y=0;
float Angle_z=0;
>>>>>>> f6c0b9fc0f891b6a91218b89fe6c3db8e596ac69

int resource_gyro_sensor_init()
{
	uint8_t mode1 = 0;
	int ret = PERIPHERAL_ERROR_NONE;

	if (g_i2c_h == NULL)
		ret = peripheral_i2c_open(RPI3_I2C_BUS, MPU6050_Address , &g_i2c_h);

	ret = peripheral_i2c_write_register_byte(g_i2c_h,SMPLRT_DIV, 7);  //write to sample rate register
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("failed to write register");
		goto ERROR;
	}

	ret = peripheral_i2c_write_register_byte(g_i2c_h,PWR_MGMT_1, 1);  //Write to power management register
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("failed to write register");
		goto ERROR;
	}

	ret = peripheral_i2c_write_register_byte(g_i2c_h,CONFIG, 0);  //Write to Configuration register
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("failed to write register");
		goto ERROR;
	}

	ret = peripheral_i2c_write_register_byte(g_i2c_h,GYRO_CONFIG, 24);  //Write to Gyro configuration register
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("failed to write register");
		goto ERROR;
	}

	ret = peripheral_i2c_write_register_byte(g_i2c_h,INT_ENABLE, 1);  //Write to interrupt enable register
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("failed to write register");
		goto ERROR;
	}

	return 0;


ERROR:
	if (g_i2c_h)
		peripheral_i2c_close(g_i2c_h);

	g_i2c_h = NULL;
	return -1;
}

short read_raw_data(int addr){
	
	int ret = PERIPHERAL_ERROR_NONE;
	short high_byte,low_byte,value;

	ret = peripheral_i2c_read_register_byte(g_i2c_h, addr , &high_byte);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("failed to read register");

	}

	ret = peripheral_i2c_read_register_byte(g_i2c_h, addr+1 ,&low_byte);
	if (ret != PERIPHERAL_ERROR_NONE) {
		_E("failed to read register");

	}
	value = (high_byte << 8) + low_byte;
	if (value > 32768)
		value=value-65536;
	return value;
}

int resource_calculate_tilt(float rate_Gx, float interval){

	Angle_x += (rate_Gx)*interval;
	_D("\n Angle_x = %.1f", Angle_x);

	return Angle_x;

}

int resource_read_gyro_sensor(float interval, float *tilt){

	int ret = PERIPHERAL_ERROR_NONE;
	float Acc_x,Acc_y,Acc_z;
	float Gyro_x,Gyro_y,Gyro_z;
	float Ax=0, Ay=0, Az=0;
	float Gx=0, Gy=0, Gz=0;


	ret = resource_gyro_sensor_init();

	Acc_x = read_raw_data(ACCEL_XOUT_H);
	Acc_y = read_raw_data(ACCEL_YOUT_H);
	Acc_z = read_raw_data(ACCEL_ZOUT_H);

	Gyro_x = read_raw_data(GYRO_XOUT_H);
	Gyro_y = read_raw_data(GYRO_YOUT_H);
	Gyro_z = read_raw_data(GYRO_ZOUT_H);

	Ax = Acc_x/16384.0;
	Ay = Acc_y/16384.0;
	Az = Acc_z/16384.0;

	Gx = Gyro_x/131;
	Gy = Gyro_y/131;
	Gz = Gyro_z/131;

<<<<<<< HEAD

	_D("\n Gx=%d °/s\tGy=%.3f °/s\tGz=%.3f °/s", (int)Gx+1, Gy+1, Gz);
	_D("\n Ax=%.1f g\tAy=%.1f g\tAz=%.1f g \n", Ax, Ay, Az);

	*tilt= resource_calculate_tilt((int)Gx+1, interval);


	return 0;
=======
	_D("\n Gx=%.3f °/s\tGy=%.3f °/s\tGz=%.3f °/s",Gx+1,Gy+1,Gz+1); //보정을 위한 +1
	_D("\n Ax=%.1f g\tAy=%.1f g\tAz=%.1f g \n",Ax,Ay,Az);
>>>>>>> f6c0b9fc0f891b6a91218b89fe6c3db8e596ac69

	Angle_x += (Gx+1) * 0.1;
	Angle_y += (Gy+1) * 0.1;
	Angle_z += (Gz+1) * 0.1;

<<<<<<< HEAD


=======
	_D("\n Angle_x = %.1f\tAngle_x = %.1fAngle_x = %.1f",Angle_x,Angle_y,Angle_z);

}
>>>>>>> f6c0b9fc0f891b6a91218b89fe6c3db8e596ac69
