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

#ifndef __TIZEN_SYSTEM_PERIPHERAL_IO_H__
#define __TIZEN_SYSTEM_PERIPHERAL_IO_H__

#include <stdint.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_MODULE
 * @{
 */

/**
 * @file peripheral_io.h
 * @brief This file contains the peripheral-io API.
 */

/**
 * @brief Enumeration for peripheral-io error.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_ERROR_NONE                = TIZEN_ERROR_NONE,                /**< Successful */
	PERIPHERAL_ERROR_IO_ERROR            = TIZEN_ERROR_IO_ERROR,            /**< I/O error */
	PERIPHERAL_ERROR_NO_DEVICE           = TIZEN_ERROR_NO_SUCH_DEVICE,      /**< No such device */
	PERIPHERAL_ERROR_TRY_AGAIN           = TIZEN_ERROR_TRY_AGAIN,           /**< Try again */
	PERIPHERAL_ERROR_OUT_OF_MEMORY       = TIZEN_ERROR_OUT_OF_MEMORY,       /**< Out of memory */
	PERIPHERAL_ERROR_PERMISSION_DENIED   = TIZEN_ERROR_PERMISSION_DENIED,   /**< Permission denied */
	PERIPHERAL_ERROR_RESOURCE_BUSY       = TIZEN_ERROR_RESOURCE_BUSY,       /**< Device or resource busy */
	PERIPHERAL_ERROR_INVALID_PARAMETER   = TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
	PERIPHERAL_ERROR_NOT_SUPPORTED       = TIZEN_ERROR_NOT_SUPPORTED,       /**< Not supported */
	PERIPHERAL_ERROR_UNKNOWN             = TIZEN_ERROR_UNKNOWN,             /**< Unknown error */
} peripheral_error_e;

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_GPIO_MODULE
 * @{
 */

/**
 * @brief Enumeration of GPIO direction options.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_GPIO_DIRECTION_IN = 0,              /**< Input Mode */
	PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_HIGH,  /**< Output mode with high value */
	PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW,   /**< Output mode with low value */
} peripheral_gpio_direction_e;

/**
 * @brief Enumeration of edge types for the GPIO interrupt.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_GPIO_EDGE_NONE = 0,  /**< No interrupt on GPIO */
	PERIPHERAL_GPIO_EDGE_RISING,    /**< Interrupt on rising only */
	PERIPHERAL_GPIO_EDGE_FALLING,   /**< Interrupt on falling only */
	PERIPHERAL_GPIO_EDGE_BOTH,      /**< Interrupt on rising & falling */
} peripheral_gpio_edge_e;

/**
 * @brief The handle of a GPIO pin.
 * @since_tizen 4.0
 */
typedef struct _peripheral_gpio_s *peripheral_gpio_h;

/**
 * @platform
 * @brief Opens a GPIO pin.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 * @remarks @a gpio should be released with peripheral_gpio_close()
 *
 * @param[in] gpio_pin The GPIO pin number
 * @param[out] gpio The GPIO handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_gpio_close()
 */
int peripheral_gpio_open(int gpio_pin, peripheral_gpio_h *gpio);

/**
 * @platform
 * @brief Closes a GPIO pin.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] gpio The GPIO handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @pre peripheral_gpio_open()
 */
int peripheral_gpio_close(peripheral_gpio_h gpio);

/**
 * @platform
 * @brief Sets the GPIO direction.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] gpio The GPIO handle
 * @param[in] direction The direction of the GPIO pin
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_gpio_direction_e
 */
int peripheral_gpio_set_direction(peripheral_gpio_h gpio, peripheral_gpio_direction_e direction);

/**
 * @platform
 * @brief Sets the GPIO edge mode.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] gpio The GPIO handle
 * @param[in] edge The edge mode of the GPIO pin
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_gpio_edge_e
 */
int peripheral_gpio_set_edge_mode(peripheral_gpio_h gpio, peripheral_gpio_edge_e edge);

/**
 * @platform
 * @brief The GPIO interrupted callback called when the GPIO interrupt is triggered.
 * @since_tizen 4.0
 *
 * @param[in] gpio The GPIO handle
 * @param[in] error The GPIO error
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see peripheral_gpio_set_interrupted_cb()
 */
typedef void(*peripheral_gpio_interrupted_cb)(peripheral_gpio_h gpio, peripheral_error_e error, void *user_data);

/**
 * @platform
 * @brief Sets the GPIO interrupted callback to be invoked when the GPIO interrupt is triggered.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] gpio The GPIO handle
 * @param[in] callback The GPIO interrupted callback function to set
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 *
 * @post peripheral_gpio_unset_interrupted_cb()
 * @see peripheral_gpio_set_edge_mode()
 */
int peripheral_gpio_set_interrupted_cb(peripheral_gpio_h gpio, peripheral_gpio_interrupted_cb callback, void *user_data);

/**
 * @platform
 * @brief Unsets the GPIO interrupted callback.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] gpio The GPIO handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre peripheral_gpio_set_interrupted_cb()
 */
int peripheral_gpio_unset_interrupted_cb(peripheral_gpio_h gpio);

/**
 * @platform
 * @brief Gets the current value of the GPIO pin.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] gpio The GPIO handle
 * @param[out] value The value to get
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_gpio_write()
 */
int peripheral_gpio_read(peripheral_gpio_h gpio, uint32_t *value);

/**
 * @platform
 * @brief Sets the value of the GPIO pin.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] gpio The GPIO handle
 * @param[in] value The value to set (must be 0 or 1)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_gpio_read()
 */
int peripheral_gpio_write(peripheral_gpio_h gpio, uint32_t value);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_I2C_MODULE
 * @{
 */

/**
 * @brief The handle of the I2C slave device.
 * @since_tizen 4.0
 */
typedef struct _peripheral_i2c_s *peripheral_i2c_h;

/**
 * @platform
 * @brief Opens an I2C slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 * @remarks @a i2c should be released with peripheral_i2c_close()
 *
 * @param[in] bus The I2C bus number that the slave device is connected
 * @param[in] address The address of the slave device
 * @param[out] i2c The I2C handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_i2c_close()
 */
int peripheral_i2c_open(int bus, int address, peripheral_i2c_h *i2c);

/**
 * @platform
 * @brief Closes an I2C slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @pre peripheral_i2c_open()
 */
int peripheral_i2c_close(peripheral_i2c_h i2c);

/**
 * @platform
 * @brief Reads the bytes data from the I2C slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 * @param[out] data The data buffer to read
 * @param[in] length The size of data buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_write()
 */
int peripheral_i2c_read(peripheral_i2c_h i2c, uint8_t *data, uint32_t length);

/**
 * @platform
 * @brief Writes the bytes data to the I2C slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 * @param[in] data The data buffer to write
 * @param[in] length The size of data buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_read()
 */
int peripheral_i2c_write(peripheral_i2c_h i2c, uint8_t *data, uint32_t length);

/**
 * @platform
 * @brief Reads single byte data from the register of the I2C slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 * @param[in] reg The register address of the I2C slave device to read
 * @param[out] data The single byte data to read
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_write_register_byte()
 */
int peripheral_i2c_read_register_byte(peripheral_i2c_h i2c, uint8_t reg, uint8_t *data);

/**
 * @platform
 * @brief Writes single byte data to the register of the I2C slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 * @param[in] reg The register address of the I2C slave device to write
 * @param[in] data The single byte data to write
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_read_register_byte()
 */
int peripheral_i2c_write_register_byte(peripheral_i2c_h i2c, uint8_t reg, uint8_t data);

/**
 * @platform
 * @brief Reads word data from the register of the I2C slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 * @param[in] reg The register address of the I2C slave device to read
 * @param[out] data The word(2 bytes) data to read
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_write_register_word()
 */
int peripheral_i2c_read_register_word(peripheral_i2c_h i2c, uint8_t reg, uint16_t *data);

/**
 * @platform
 * @brief Writes word data to the register of the I2C slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] i2c The I2C handle
 * @param[in] reg The register address of the I2C slave device to write
 * @param[in] data The word(2 bytes) data to write
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_i2c_read_register_word()
 */
int peripheral_i2c_write_register_word(peripheral_i2c_h i2c, uint8_t reg, uint16_t data);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_PWM_MODULE
 * @{
 */

/**
 * @brief The handle of the PWM device.
 * @since_tizen 4.0
 */
typedef struct _peripheral_pwm_s *peripheral_pwm_h;

/**
 * @brief Enumeration for Polarity.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_PWM_POLARITY_ACTIVE_HIGH = 0, /**< PWM signal start in the active high state (Normal) */
	PERIPHERAL_PWM_POLARITY_ACTIVE_LOW,      /**< PWM signal start in the active low state (Inversed) */
} peripheral_pwm_polarity_e;

/**
 * @platform
 * @brief Opens the PWM pin.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 * @remarks @a pwm should be released with peripheral_pwm_close()
 *
 * @param[in] chip The PWM chip number
 * @param[in] pin The PWM pin(channel) number to control
 * @param[out] pwm The PWM handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_pwm_close()
 */
int peripheral_pwm_open(int chip, int pin, peripheral_pwm_h *pwm);

/**
 * @platform
 * @brief Closes the PWM pin.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] pwm The PWM handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @pre peripheral_pwm_open()
 */
int peripheral_pwm_close(peripheral_pwm_h pwm);

/**
 * @platform
 * @brief Sets period of the PWM pin.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] pwm The PWM handle
 * @param[in] period_ns The total period of the PWM pin (in nanoseconds)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_pwm_set_period(peripheral_pwm_h pwm, uint32_t period_ns);

/**
 * @platform
 * @brief Sets duty cycle of the PWM pin.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] pwm The PWM handle
 * @param[in] duty_cycle_ns The duty cycle of the PWM pin (in nanoseconds)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_pwm_set_duty_cycle(peripheral_pwm_h pwm, uint32_t duty_cycle_ns);

/**
 * @platform
 * @brief Sets polarity of the PWM pin.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] pwm The PWM handle
 * @param[in] polarity The polarity of the PWM pin
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_pwm_polarity_e
 */
int peripheral_pwm_set_polarity(peripheral_pwm_h pwm, peripheral_pwm_polarity_e polarity);

/**
 * @platform
 * @brief Enables the PWM pin.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] pwm The PWM handle
 * @param[in] enabled Enable/disable the PWM pin
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_pwm_set_enabled(peripheral_pwm_h pwm, bool enabled);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_UART_MODULE
 * @{
 */

/**
 * @brief The handle to the UART device.
 * @since_tizen 4.0
 */
typedef struct _peripheral_uart_s *peripheral_uart_h;

/**
 * @brief Enumeration for baud rate.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_UART_BAUD_RATE_0 = 0,  /**< The number of signal in one second is 0 */
	PERIPHERAL_UART_BAUD_RATE_50,     /**< The number of signal in one second is 50 */
	PERIPHERAL_UART_BAUD_RATE_75,     /**< The number of signal in one second is 75 */
	PERIPHERAL_UART_BAUD_RATE_110,    /**< The number of signal in one second is 110 */
	PERIPHERAL_UART_BAUD_RATE_134,    /**< The number of signal in one second is 134 */
	PERIPHERAL_UART_BAUD_RATE_150,    /**< The number of signal in one second is 150 */
	PERIPHERAL_UART_BAUD_RATE_200,    /**< The number of signal in one second is 200 */
	PERIPHERAL_UART_BAUD_RATE_300,    /**< The number of signal in one second is 300 */
	PERIPHERAL_UART_BAUD_RATE_600,    /**< The number of signal in one second is 600 */
	PERIPHERAL_UART_BAUD_RATE_1200,   /**< The number of signal in one second is 1200 */
	PERIPHERAL_UART_BAUD_RATE_1800,   /**< The number of signal in one second is 1800 */
	PERIPHERAL_UART_BAUD_RATE_2400,   /**< The number of signal in one second is 2400 */
	PERIPHERAL_UART_BAUD_RATE_4800,   /**< The number of signal in one second is 4800 */
	PERIPHERAL_UART_BAUD_RATE_9600,   /**< The number of signal in one second is 9600 */
	PERIPHERAL_UART_BAUD_RATE_19200,  /**< The number of signal in one second is 19200 */
	PERIPHERAL_UART_BAUD_RATE_38400,  /**< The number of signal in one second is 38400 */
	PERIPHERAL_UART_BAUD_RATE_57600,  /**< The number of signal in one second is 57600 */
	PERIPHERAL_UART_BAUD_RATE_115200, /**< The number of signal in one second is 115200 */
	PERIPHERAL_UART_BAUD_RATE_230400, /**< The number of signal in one second is 230400 */
} peripheral_uart_baud_rate_e;

/**
 * @brief Enumeration for byte size.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_UART_BYTE_SIZE_5BIT = 0, /**< 5 data bits */
	PERIPHERAL_UART_BYTE_SIZE_6BIT,     /**< 6 data bits */
	PERIPHERAL_UART_BYTE_SIZE_7BIT,     /**< 7 data bits */
	PERIPHERAL_UART_BYTE_SIZE_8BIT,     /**< 8 data bits */
} peripheral_uart_byte_size_e;

/**
 * @brief Enumeration for parity bit.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_UART_PARITY_NONE = 0, /**< No parity is used */
	PERIPHERAL_UART_PARITY_EVEN,     /**< Even parity is used */
	PERIPHERAL_UART_PARITY_ODD,      /**< ODD parity is used */
} peripheral_uart_parity_e;

/**
 * @brief Enumeration for stop bits.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_UART_STOP_BITS_1BIT = 0, /**< One stop bit */
	PERIPHERAL_UART_STOP_BITS_2BIT,     /**< Two stop bits */
} peripheral_uart_stop_bits_e;

/**
 * @brief Enumeration for hardware flow control.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_UART_HARDWARE_FLOW_CONTROL_NONE = 0,    /**< No hardware flow control */
	PERIPHERAL_UART_HARDWARE_FLOW_CONTROL_AUTO_RTSCTS, /**< Automatic RTS/CTS hardware flow control*/
} peripheral_uart_hardware_flow_control_e;

/**
 * @brief Enumeration for software flow control.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_UART_SOFTWARE_FLOW_CONTROL_NONE = 0,    /**< No software flow control */
	PERIPHERAL_UART_SOFTWARE_FLOW_CONTROL_XONXOFF,     /**< XON/XOFF software flow control */
} peripheral_uart_software_flow_control_e;

/**
 * @platform
 * @brief Opens the UART slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 * @remarks @a uart should be released with peripheral_uart_close()
 *
 * @param[in] port The UART port number that the slave device is connected
 * @param[out] uart The UART handle is created on success
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_uart_close()
 */
int peripheral_uart_open(int port, peripheral_uart_h *uart);

/**
 * @platform
 * @brief Closes the UART slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @pre peripheral_uart_open()
 */
int peripheral_uart_close(peripheral_uart_h uart);

/**
 * @platform
 * @brief Sets baud rate of the UART slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[in] baud Baud rate of the UART device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_baud_rate_e
 */
int peripheral_uart_set_baud_rate(peripheral_uart_h uart, peripheral_uart_baud_rate_e baud);

/**
 * @platform
 * @brief Sets byte size of the UART slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[in] byte_size Byte size of the UART device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_byte_size_e
 */
int peripheral_uart_set_byte_size(peripheral_uart_h uart, peripheral_uart_byte_size_e byte_size);

/**
 * @platform
 * @brief Sets parity bit of the UART slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[in] parity Parity bit of the UART device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_parity_e
 */
int peripheral_uart_set_parity(peripheral_uart_h uart, peripheral_uart_parity_e parity);

/**
 * @platform
 * @brief Sets stop bits of the UART slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[in] stop_bits Stop bits of the UART device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_stop_bits_e
 */
int peripheral_uart_set_stop_bits(peripheral_uart_h uart, peripheral_uart_stop_bits_e stop_bits);

/**
 * @platform
 * @brief Sets flow control of the UART slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[in] sw_flow_control Software flow control (Turns a transmitter on or off)
 * @param[in] hw_flow_control Hardware flow control (Turns "Request to Send/Clear to Send" on or off)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_software_flow_control_e
 * @see peripheral_uart_hardware_flow_control_e
 */
int peripheral_uart_set_flow_control(peripheral_uart_h uart,
			peripheral_uart_software_flow_control_e sw_flow_control,
			peripheral_uart_hardware_flow_control_e hw_flow_control);

/**
 * @platform
 * @brief Reads data from the UART slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[out] data The buffer to read
 * @param[out] length The size of buffer (in bytes)
 *
 * @return the number of bytes read on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_TRY_AGAIN Try again
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_write()
 */
int peripheral_uart_read(peripheral_uart_h uart, uint8_t *data, uint32_t length);

/**
 * @platform
 * @brief Writes data to the UART slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] uart The UART handle
 * @param[in] data The buffer to write
 * @param[in] length The size of buffer (in bytes)
 *
 * @return the number of bytes write on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_TRY_AGAIN Try again
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_uart_read()
 */
int peripheral_uart_write(peripheral_uart_h uart, uint8_t *data, uint32_t length);

/**
* @}
*/

/**
 * @addtogroup CAPI_SYSTEM_PERIPHERAL_IO_SPI_MODULE
 * @{
 */

/**
 * @brief The handle of a SPI device.
 * @since_tizen 4.0
 */
typedef struct _peripheral_spi_s *peripheral_spi_h;

/**
 * @brief Enumeration of SPI transfer modes.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_SPI_MODE_0 = 0,   /**< CPOL = 0, CPHa = 0 Mode */
	PERIPHERAL_SPI_MODE_1,       /**< CPOL = 0, CPHa = 1 Mode */
	PERIPHERAL_SPI_MODE_2,       /**< CPOL = 1, CPHa = 0 Mode */
	PERIPHERAL_SPI_MODE_3,       /**< CPOL = 1, CPHa = 1 Mode */
} peripheral_spi_mode_e;

/**
 * @brief Enumeration of bit orders.
 * @since_tizen 4.0
 */
typedef enum {
	PERIPHERAL_SPI_BIT_ORDER_MSB = 0, /**< Use most siginificant bit first */
	PERIPHERAL_SPI_BIT_ORDER_LSB,     /**< Use least significant bit first */
} peripheral_spi_bit_order_e;

/**
 * @platform
 * @brief Opens a SPI slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 * @remarks @a spi should be released with peripheral_spi_close()
 *
 * @param[in] bus The SPI bus number
 * @param[in] cs The SPI chip select number
 * @param[out] spi The SPI device handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_OUT_OF_MEMORY Memory allocation failed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_RESOURCE_BUSY Device is in use
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @post peripheral_spi_close()
 */
int peripheral_spi_open(int bus, int cs, peripheral_spi_h *spi);

/**
 * @platform
 * @brief Closes the SPI slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI device handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_open()
 */
int peripheral_spi_close(peripheral_spi_h spi);

/**
 * @platform
 * @brief Sets the SPI transfer mode.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI device handle
 * @param[in] mode The SPI transfer mode
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_mode_e
 */
int peripheral_spi_set_mode(peripheral_spi_h spi, peripheral_spi_mode_e mode);

/**
 * @platform
 * @brief Sets the SPI bit order.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI device handle
 * @param[in] bit_order The transfer bit order
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_bit_order_e
 */
int peripheral_spi_set_bit_order(peripheral_spi_h spi, peripheral_spi_bit_order_e bit_order);

/**
 * @platform
 * @brief Sets the number of bits per word.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI device handle
 * @param[in] bits The number of bits per word (in bits)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_spi_set_bits_per_word(peripheral_spi_h spi, uint8_t bits);

/**
 * @platform
 * @brief Sets the frequency of the SPI bus.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 * @remarks The frequencies supported are board dependent.
 *
 * @param[in] spi The SPI device handle
 * @param[in] freq_hz Frequency to set (in Hz)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 */
int peripheral_spi_set_frequency(peripheral_spi_h spi, uint32_t freq_hz);

/**
 * @platform
 * @brief Reads the bytes data from the SPI slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI device handle
 * @param[out] data The data buffer to read
 * @param[in] length The size of data buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_write()
 */
int peripheral_spi_read(peripheral_spi_h spi, uint8_t *data, uint32_t length);

/**
 * @platform
 * @brief Writes the bytes data to the SPI slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI device handle
 * @param[in] data The data buffer to write
 * @param[in] length The size of data buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_read()
 */
int peripheral_spi_write(peripheral_spi_h spi, uint8_t *data, uint32_t length);

/**
 * @platform
 * @brief Exchanges the bytes data to the SPI slave device.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege http://tizen.org/privilege/peripheralio
 *
 * @param[in] spi The SPI device handle
 * @param[in] txdata The data buffer to write
 * @param[out] rxdata The data buffer to read
 * @param[in] length The size of txdata and rxdata buffer (in bytes)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #PERIPHERAL_ERROR_NONE Successful
 * @retval #PERIPHERAL_ERROR_IO_ERROR I/O operation failed
 * @retval #PERIPHERAL_ERROR_NO_DEVICE Device does not exist or is removed
 * @retval #PERIPHERAL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PERIPHERAL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PERIPHERAL_ERROR_NOT_SUPPORTED Not supported
 * @retval #PERIPHERAL_ERROR_UNKNOWN Unknown internal error
 *
 * @see peripheral_spi_read()
 * @see peripheral_spi_write()
 */
int peripheral_spi_transfer(peripheral_spi_h spi, uint8_t *txdata, uint8_t *rxdata, uint32_t length);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_PERIPHERAL_IO_H__ */
