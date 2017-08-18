/*
 * Copyright (c) 2016-2017 Samsung Electronics Co., Ltd.
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

#ifndef __PERIPHERAL_IO_H__
#define __PERIPHERAL_IO_H__

#include <stdint.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file peripheral_io.h
 * @brief This file contains the peripheral-io API
 */

/**
 * @brief Enumeration for peripheral-io error.
 * @since_tizen
 */
typedef enum {
	PERIPHERAL_ERROR_NONE                = TIZEN_ERROR_NONE,                /**< Successful */
	PERIPHERAL_ERROR_IO_ERROR            = TIZEN_ERROR_IO_ERROR,            /**< I/O error */
	PERIPHERAL_ERROR_NO_DEVICE           = TIZEN_ERROR_NO_SUCH_DEVICE,      /**< No such device */
	PERIPHERAL_ERROR_OUT_OF_MEMORY       = TIZEN_ERROR_OUT_OF_MEMORY,       /**< Out of memory */
	PERIPHERAL_ERROR_PERMISSION_DENIED   = TIZEN_ERROR_PERMISSION_DENIED,   /**< Permission denied */
	PERIPHERAL_ERROR_RESOURCE_BUSY       = TIZEN_ERROR_RESOURCE_BUSY,       /**< Device or resource busy */
	PERIPHERAL_ERROR_INVALID_PARAMETER   = TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
	PERIPHERAL_ERROR_NO_DATA             = TIZEN_ERROR_NO_DATA,             /**< No data available */
	PERIPHERAL_ERROR_INVALID_OPERATION   = TIZEN_ERROR_INVALID_OPERATION,   /**< Function not implemented */
	PERIPHERAL_ERROR_TIMED_OUT           = TIZEN_ERROR_TIMED_OUT,           /**< Time out */
	PERIPHERAL_ERROR_NOT_SUPPORTED       = TIZEN_ERROR_NOT_SUPPORTED,       /**< Not supported */
	PERIPHERAL_ERROR_UNKNOWN             = TIZEN_ERROR_UNKNOWN,             /**< Unknown error */
} peripheral_error_e;

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_GPIO_MODULE
 * @{
 */

/**
 * @brief Enumeration of gpio direction
 */
typedef enum {
	PERIPHERAL_GPIO_DIRECTION_IN = 0,    /**< Input Mode */
	PERIPHERAL_GPIO_DIRECTION_OUT,       /**< Output mode with low value */
	PERIPHERAL_GPIO_DIRECTION_OUT_LOW = PERIPHERAL_GPIO_DIRECTION_OUT, /**< Same as above */
	PERIPHERAL_GPIO_DIRECTION_OUT_HIGH,  /**< Output mode with high value */
} peripheral_gpio_direction_e;

/**
 * @brief Enumeration of edge type for gpio interrupt
 */
typedef enum {
	PERIPHERAL_GPIO_EDGE_NONE = 0,  /**< No interrupt on Gpio */
	PERIPHERAL_GPIO_EDGE_RISING,    /**< Interrupt on rising only */
	PERIPHERAL_GPIO_EDGE_FALLING,   /**< Interrupt on falling only */
	PERIPHERAL_GPIO_EDGE_BOTH,      /**< Interrupt on rising & falling */
} peripheral_gpio_edge_e;

/**
 * @brief Gpio isr callback data delivered via gpio_isr_cb().
 * @details A gpio isr callback data is delivered as a structure, which contains
 *          the pin number, the pin value, and the timestamp of the gpio interrupt
 *          in microseconds.
 * @since_tizen 4.0
 */
typedef struct {
	int pin;
	int value;
	unsigned long long timestamp;
} gpio_isr_cb_s;

/**
 * @brief The handle to the gpio pin
 * @since_tizen 4.0
 */
typedef struct _peripheral_gpio_s* peripheral_gpio_h;

/**
 * @brief Initializes(export) gpio pin and creates gpio handle.
 * @since_tizen 4.0
 *
 * @param[in] gpio_pin The gpio pin number
 * @param[out] gpio The gpio handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 *
 * @see peripheral_gpio_close()
 */
int peripheral_gpio_open(int gpio_pin, peripheral_gpio_h *gpio);

/**
 * @brief Releases the gpio handle and finalize(unexport) the gpio pin.
 * @since_tizen 4.0
 *
 * @param[in] gpio The handle to the gpio pin to release
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 *
 * @see peripheral_gpio_open()
 */
int peripheral_gpio_close(peripheral_gpio_h gpio);

/**
 * @brief Gets direction of the gpio.
 * @since_tizen 4.0
 *
 * @param[in] gpio The handle to the gpio pin
 * @param[out] value The direction(value) type of the gpio
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 *
 * @see peripheral_gpio_set_direction()
 */
int peripheral_gpio_get_direction(peripheral_gpio_h gpio, peripheral_gpio_direction_e *direction);

/**
 * @brief Sets direction of the gpio pin.
 * @since_tizen 4.0
 *
 * @param[in] gpio The handle to the gpio pin to set
 * @param[in] direction Direction(value) type of the gpio pin
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_gpio_set_direction(peripheral_gpio_h gpio, peripheral_gpio_direction_e direction);

/**
 * @brief Reads value of the gpio.
 * @since_tizen 4.0
 *
 * @param[in] gpio The handle to the gpio pin
 * @param[out] value The value of the gpio (zero or non-zero)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 *
 * @see peripheral_gpio_write()
 */
int peripheral_gpio_read(peripheral_gpio_h gpio, int *value);

/**
 * @brief Writes value to the gpio.
 * @since_tizen 4.0
 *
 * @param[in] gpio The handle to the gpio pin
 * @param[in] value Value to be written to the gpio (muse be zero or non-zero)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 *
 * @see peripheral_gpio_read()
 */
int peripheral_gpio_write(peripheral_gpio_h gpio, int value);

/**
 * @brief Gets the edge mode of the gpio.
 * @since_tizen 4.0
 *
 * @param[in] gpio The handle to the gpio pin
 * @param[out] gpio_pin The edge mode of the gpio
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 *
 * @see peripheral_gpio_set_edge_mode()
 */
int peripheral_gpio_get_edge_mode(peripheral_gpio_h gpio, peripheral_gpio_edge_e *edge);

/**
 * @brief Sets the edge mode of the gpio pin.
 * @since_tizen 4.0
 *
 * @param[in] gpio The handle to the gpio pin to set
 * @param[in] edge The edge mode of the gpio pin
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 *
 * @see peripheral_gpio_get_edge_mode()
 */
int peripheral_gpio_set_edge_mode(peripheral_gpio_h gpio, peripheral_gpio_edge_e edge);

/**
 * @brief Called when the gpio interrupt is triggered.
 * @since_tizen 4.0
 *
 * @param[in] data The gpio isr callback data
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see peripheral_gpio_register_cb()
 * @see peripheral_gpio_unregister_cb()
 */
typedef void(*gpio_isr_cb)(gpio_isr_cb_s *data, void *user_data);

/**
 * @brief Registers a callback function to be invoked when the gpio interrupt is triggered.
 * @since_tizen 4.0
 *
 * @param[in] gpio The handle to the gpio pin to set
 * @param[in] edge The edge type of the gpio pin
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see peripheral_gpio_set_edge_mode()
 * @see peripheral_gpio_unregister_cb()
 */
int peripheral_gpio_register_cb(peripheral_gpio_h gpio, gpio_isr_cb callback, void *user_data);

/**
 * @brief Unregisters the callback function for the gpio handler.
 * @since_tizen 4.0
 *
 * @param[in] gpio The handle to the gpio pin
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see peripheral_gpio_register_cb()
 */
int peripheral_gpio_unregister_cb(peripheral_gpio_h gpio);

/**
 * @brief Gets pin number of the gpio handle.
 * @since_tizen 4.0
 *
 * @param[in] gpio The handle to the gpio pin
 * @param[out] gpio_pin The pin number of the gpio
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 */
int peripheral_gpio_get_pin(peripheral_gpio_h gpio, int *gpio_pin);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_I2C_MODULE
 * @{
 */

/**
 * @brief The handle to the i2c device
 * @since_tizen 4.0
 */
typedef struct _peripheral_i2c_s *peripheral_i2c_h;

/**
 * @brief Initializes i2c communication and creates i2c handle.
 * @since_tizen 4.0
 *
 * @param[in] bus The i2c bus number that the slave device is connected
 * @param[in] address The address of the slave device
 * @param[out] i2c The i2c handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 *
 * @see peripheral_i2c_close()
 */
int peripheral_i2c_open(int bus, int address, peripheral_i2c_h *i2c);

/**
 * @brief Destory the i2c handle and release the communication.
 * @since_tizen 4.0
 *
 * @param[in] i2c The i2c handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_open()
 */
int peripheral_i2c_close(peripheral_i2c_h i2c);

/**
 * @brief Reads data from the i2c slave device.
 * @since_tizen 4.0
 *
 * @param[in] i2c The handle to the i2c device
 * @param[out] data The address of data buffer to read
 * @param[in] length The size of data buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_i2c_read(peripheral_i2c_h i2c, uint8_t *data, int length);

/**
 * @brief Write data to the i2c slave device.
 * @since_tizen 4.0
 *
 * @param[in] i2c The handle to the i2c device
 * @param[in] data The address of data buffer to write
 * @param[in] length The size of data buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_i2c_write(peripheral_i2c_h i2c, uint8_t *data, int length);

/**
 * @brief Reads single byte data from the i2c slave device.
 * @since_tizen 4.0
 *
 * @param[in] i2c The handle to the i2c device
 * @param[out] data The address of data buffer to read
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_i2c_read_byte(peripheral_i2c_h i2c, uint8_t *data);

/**
 * @brief Write single byte data to the i2c slave device.
 * @since_tizen 4.0
 *
 * @param[in] i2c The handle to the i2c device
 * @param[in] data The byte value to write
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_i2c_write_byte(peripheral_i2c_h i2c, uint8_t data);

/**
 * @brief Reads byte data from the register of i2c slave device.
 * @since_tizen 4.0
 *
 * @param[in] i2c The handle to the i2c device
 * @param[in] address The register address of the i2c slave device to read
 * @param[out] data The byte output of slave device(register value)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_i2c_read_register_byte(peripheral_i2c_h i2c, uint8_t address, uint8_t *data);

/**
 * @brief Write byte data to the register of i2c slave device.
 * @since_tizen 4.0
 *
 * @param[in] i2c The handle to the i2c device
 * @param[in] address The register address of the i2c slave device to write
 * @param[in] data The byte value to write
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_i2c_write_register_byte(peripheral_i2c_h i2c, uint8_t address, uint8_t data);

/**
 * @brief Reads word data from the register of i2c slave device.
 * @since_tizen 4.0
 *
 * @param[in] i2c The handle to the i2c device
 * @param[in] address The register address of the i2c slave device to read
 * @param[out] data The word output(2 bytes) of slave device(register value)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_i2c_read_register_word(peripheral_i2c_h i2c, uint8_t address, uint16_t *data);

/**
 * @brief Write byte data to the register of i2c slave device.
 * @since_tizen 4.0
 *
 * @param[in] i2c The handle to the i2c device
 * @param[in] address The register address of the i2c slave device to write
 * @param[in] data The word(2 bytes) value to write
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_i2c_write_register_word(peripheral_i2c_h i2c, uint8_t address, uint16_t data);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_PWM_MODULE
 * @{
 */

/**
 * @brief The handle to the pwm device
 * @since_tizen 4.0
 */
typedef struct _peripheral_pwm_s *peripheral_pwm_h;

/**
 * @brief Enumeration for Polarity.
 */
typedef enum {
	PERIPHERAL_PWM_POLARITY_NORMAL = 0,
	PERIPHERAL_PWM_POLARITY_INVERSED,
} peripheral_pwm_polarity_e;

/**
 * @brief Initializes(export) pwm device and creates pwm handle.
 * @since_tizen 4.0
 *
 * @param[in] device The pwm chip number
 * @param[in] channel The pwm channel number to control
 * @param[out] pwm The pwm handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 *
 * @see peripheral_pwm_close()
 */
int peripheral_pwm_open(int device, int channel, peripheral_pwm_h *pwm);

/**
 * @brief Destory the pwm handle and finalize(unexport) the pwm device.
 * @since_tizen 4.0
 *
 * @param[in] pwm The handle to the pwm device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_INVALID_OPERATION Invalid access
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 *
 * @see peripheral_pwm_open()
 */
int peripheral_pwm_close(peripheral_pwm_h pwm);

/**
 * @brief Sets Period of the pwm device.
 * @since_tizen 4.0
 *
 * @param[in] pwm The handle to the pwm device
 * @param[in] period The total period of the PWM signal (in nanoseconds)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_INVALID_OPERATION Invalid access
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_pwm_set_period(peripheral_pwm_h pwm, int period);

/**
 * @brief Gets Period of the pwm device.
 * @since_tizen 4.0
 *
 * @param[in] pwm The handle to the pwm device
 * @param[out] period The total period of the PWM signal (in nanoseconds)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_INVALID_OPERATION Invalid access
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_pwm_get_period(peripheral_pwm_h pwm, int *period);

/**
 * @brief Sets Duty Cycle of the pwm device.
 * @since_tizen 4.0
 *
 * @param[in] pwm The handle to the pwm device
 * @param[in] duty_cycle The active time of the pwm signal (in nanoseconds)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_INVALID_OPERATION Invalid access
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_pwm_set_duty_cycle(peripheral_pwm_h pwm, int duty_cycle);

/**
 * @brief Gets Duty Cycle of the pwm device.
 * @since_tizen 4.0
 *
 * @param[in] pwm The handle to the pwm device
 * @param[out] duty_cycle The active time of the pwm signal (in nanoseconds)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_INVALID_OPERATION Invalid access
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_pwm_get_duty_cycle(peripheral_pwm_h pwm, int *duty_cycle);

/**
 * @brief Sets Polarity of the pwm device.
 * @since_tizen 4.0
 *
 * @param[in] pwm The handle to the pwm device
 * @param[in] polarity The polarity of the pwm signal
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_INVALID_OPERATION Invalid access
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_pwm_set_polarity(peripheral_pwm_h pwm, peripheral_pwm_polarity_e polarity);

/**
 * @brief Gets Polarity of the pwm device.
 * @since_tizen 4.0
 *
 * @param[in] pwm The handle to the pwm device
 * @param[out] polarity The polarity of the pwm signal
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_INVALID_OPERATION Invalid access
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_pwm_get_polarity(peripheral_pwm_h pwm, peripheral_pwm_polarity_e *polarity);

/**
 * @brief Enable of the pwm device.
 * @since_tizen 4.0
 *
 * @param[in] pwm The handle to the pwm device
 * @param[in] enable Enable/disable the pwm signal
 *            true - enable
 *            false - disable
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_INVALID_OPERATION Invalid access
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_pwm_set_enable(peripheral_pwm_h pwm, bool enable);

/**
 * @brief Gets Enable status of the pwm device.
 * @since_tizen 4.0
 *
 * @param[in] pwm The handle to the pwm device
 * @param[out] enable Enable/disable the pwm signal
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_INVALID_OPERATION Invalid access
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_pwm_get_enable(peripheral_pwm_h pwm, bool *enable);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_ADC_MODULE
 * @{
 */

/**
 * @brief Reads data from the adc device.
 * @since_tizen 4.0
 *
 * @param[in] device The device number of the adc device
 * @param[in] channel The channel number to read
 * @param[out] data The address of buffer to read
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_adc_read(unsigned int device, unsigned int channel, int *data);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_UART_MODULE
 * @{
 */

/**
 * @brief The handle to the uart device
 * @since_tizen 4.0
 */
typedef struct _peripheral_uart_s *peripheral_uart_h;

/**
 * @brief Enumeration for Baud Rate.
 */
typedef enum {
	PERIPHERAL_UART_BAUDRATE_0 = 0,
	PERIPHERAL_UART_BAUDRATE_50,
	PERIPHERAL_UART_BAUDRATE_75,
	PERIPHERAL_UART_BAUDRATE_110,
	PERIPHERAL_UART_BAUDRATE_134,
	PERIPHERAL_UART_BAUDRATE_150,
	PERIPHERAL_UART_BAUDRATE_200,
	PERIPHERAL_UART_BAUDRATE_300,
	PERIPHERAL_UART_BAUDRATE_600,
	PERIPHERAL_UART_BAUDRATE_1200,
	PERIPHERAL_UART_BAUDRATE_1800,
	PERIPHERAL_UART_BAUDRATE_2400,
	PERIPHERAL_UART_BAUDRATE_4800,
	PERIPHERAL_UART_BAUDRATE_9600,
	PERIPHERAL_UART_BAUDRATE_19200,
	PERIPHERAL_UART_BAUDRATE_38400,
	PERIPHERAL_UART_BAUDRATE_57600,
	PERIPHERAL_UART_BAUDRATE_115200,
	PERIPHERAL_UART_BAUDRATE_230400
} peripheral_uart_baudrate_e;

/**
 * @brief Enumeration for Byte Size.
 */
typedef enum {
	PERIPHERAL_UART_BYTESIZE_5BIT = 0,
	PERIPHERAL_UART_BYTESIZE_6BIT,
	PERIPHERAL_UART_BYTESIZE_7BIT,
	PERIPHERAL_UART_BYTESIZE_8BIT
} peripheral_uart_bytesize_e;

/**
 * @brief Enumeration for Parity Bit.
 */
typedef enum {
	PERIPHERAL_UART_PARITY_NONE = 0,
	PERIPHERAL_UART_PARITY_EVEN,
	PERIPHERAL_UART_PARITY_ODD
} peripheral_uart_parity_e;

/**
 * @brief Enumeration for Stop Bits.
 */
typedef enum {
	PERIPHERAL_UART_STOPBITS_1BIT = 0,
	PERIPHERAL_UART_STOPBITS_2BIT
} peripheral_uart_stopbits_e;

/**
 * @brief Initializes uart communication and creates uart handle.
 * @since_tizen 4.0
 *
 * @param[in] port The uart port number that the slave device is connected
 * @param[out] uart The uart handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 *
 * @see peripheral_uart_close()
 */
int peripheral_uart_open(int port, peripheral_uart_h *uart);

/**
 * @brief Destory the uart handle and release the communication.
 * @since_tizen 4.0
 *
 * @param[in] uart The handle to the uart device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_open()
 */
int peripheral_uart_close(peripheral_uart_h uart);

/**
 * @brief Flush all input that has received but not yet been read by the uart
 *        device, or all output written but not transmitted to the uart device.
 * @since_tizen 4.0
 *
 * @param[in] uart The uart handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_uart_flush(peripheral_uart_h uart);

/**
 * @brief Sets baudrate of the uart device.
 * @since_tizen 4.0
 *
 * @param[in] uart The handle to the uart device to set
 * @param[in] baud Baudrate of the uart device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_uart_set_baudrate(peripheral_uart_h uart, peripheral_uart_baudrate_e baud);

/**
 * @brief Sets mode of the uart device.
 * @since_tizen 4.0
 *
 * @param[in] uart The handle to the uart device to set
 * @param[in] bytesize Byte size of the uart device
 * @param[in] parity Parity bits of the uart device
 * @param[in] stopbits Stop bits of the uart device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_uart_set_mode(peripheral_uart_h uart, peripheral_uart_bytesize_e bytesize, peripheral_uart_parity_e parity, peripheral_uart_stopbits_e stopbits);

/**
 * @brief Sets flow control of the uart device.
 * @since_tizen 4.0
 *
 * @param[in] uart The handle to the uart device to set
 * @param[in] xonxoff Turns a transmitter on or off
 * @param[in] rtscts Turns "Request to Send/Clear to Send" on or off
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_uart_set_flowcontrol(peripheral_uart_h uart, bool xonxoff, bool rtscts);

/**
 * @brief Reads data from the uart device.
 * @since_tizen 4.0
 *
 * @param[in] uart The handle to the uart device
 * @param[out] data The address of read buffer
 * @param[out] length The size of data buffer (in bytes)
 *
 * @return the number of bytes read on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_uart_read(peripheral_uart_h uart, uint8_t *data, int length);

/**
 * @brief Write data to the uart device.
 * @since_tizen 4.0
 *
 * @param[in] uart The handle to the uart device
 * @param[in] data The address of buffer to write
 * @param[in] length The size of data (in bytes)
 *
 * @return the number of bytes write on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_uart_write(peripheral_uart_h uart, uint8_t *data, int length);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_SPI_MODULE
 * @{
 */

/**
 * @brief The handle to the spi device
 * @since_tizen 4.0
 */
typedef struct _peripheral_spi_s* peripheral_spi_h;

/**
 * @brief Enumeration for SPI mode.
 */
typedef enum {
	PERIPHERAL_SPI_MODE_0 = 0,
	PERIPHERAL_SPI_MODE_1,
	PERIPHERAL_SPI_MODE_2,
	PERIPHERAL_SPI_MODE_3
} peripheral_spi_mode_e;

/**
 * @brief Initializes spi communication and creates spi handle.
 * @since_tizen 4.0
 *
 * @param[in] bus The spi bus number that the slave device is connected
 * @param[in] cs The spi chip select number that the slave device is connected
 * @param[out] spi The spi handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 *
 * @see peripheral_spi_close()
 */
int peripheral_spi_open(int bus, int cs, peripheral_spi_h *spi);

/**
 * @brief Destory the spi handle and release the communication.
 * @since_tizen 4.0
 *
 * @param[in] spi The handle to the spi device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_open()
 */
int peripheral_spi_close(peripheral_spi_h spi);

/**
 * @brief Sets mode of the spi device.
 * @since_tizen 4.0
 *
 * @param[in] spi The handle to the spi device
 * @param[in] mode The mode of the spi device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_spi_set_mode(peripheral_spi_h spi, peripheral_spi_mode_e mode);

/**
 * @brief Gets mode of the spi device.
 * @since_tizen 4.0
 *
 * @param[in] spi The handle to the spi device
 * @param[out] mode The mode of the spi device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_spi_get_mode(peripheral_spi_h spi, peripheral_spi_mode_e *mode);

/**
 * @brief Sets bits justification of the spi device.
 * @since_tizen 4.0
 *
 * @param[in] spi The handle to the spi device
 * @param[in] lsb The bit position to be transmitted first
 *            true - LSB first
 *            false - MSB first
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_spi_set_lsb_first(peripheral_spi_h spi, bool lsb);

/**
 * @brief Gets bits justification of the spi device.
 * @since_tizen 4.0
 *
 * @param[in] spi The handle to the spi device
 * @param[out] lsb The bit position to be transmitted first
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_spi_get_lsb_first(peripheral_spi_h spi, bool *lsb);

/**
 * @brief Sets the number of bits per word of the spi device
 * @since_tizen 4.0
 *
 * @param[in] spi The handle to the spi device
 * @param[in] bits The number of bits per word (in bits)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_spi_set_bits_per_word(peripheral_spi_h spi, unsigned char bits);

/**
 * @brief Gets the number of bits per word of the spi device
 * @since_tizen 4.0
 *
 * @param[in] spi The handle to the spi device
 * @param[out] bits The number of bits per word (in bits)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_spi_get_bits_per_word(peripheral_spi_h spi, unsigned char *bits);

/**
 * @brief Sets default max speed of the spi device.
 * @since_tizen 4.0
 *
 * @param[in] spi The handle to the spi device
 * @param[in] freq Max speed (in hz)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */
int peripheral_spi_set_frequency(peripheral_spi_h spi, unsigned int freq);

/**
 * @brief Gets default max speed of the spi device.
 * @since_tizen 4.0
 *
 * @param[in] spi The handle to the spi device
 * @param[out] freq Max speed (in hz)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device is not exist or removed
 */

int peripheral_spi_get_frequency(peripheral_spi_h spi, unsigned int *freq);

/**
 * @brief Reads data from the slave device.
 * @since_tizen 4.0
 *
 * @param[in] spi The handle to the spi device
 * @param[out] data The address of buffer to read
 * @param[in] length The size of data buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_spi_read(peripheral_spi_h spi, unsigned char *data, int length);

/**
 * @brief Write data to the slave device.
 * @since_tizen 4.0
 *
 * @param[in] spi The handle to the spi device
 * @param[in] data The address of buffer to write
 * @param[in] length The size of data (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_spi_write(peripheral_spi_h spi, unsigned char *data, int length);

/**
 * @brief Exchange data with the slave device.
 * @since_tizen 4.0
 *
 * @param[in] spi The handle to the spi device
 * @param[in] txdata The address of buffer to write
 * @param[out] rxdata The address of buffer to read
 * @param[in] length The size of data (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_spi_read_write(peripheral_spi_h spi, unsigned char *txdata, unsigned char *rxdata, int length);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __PERIPHERAL_IO_H__ */
