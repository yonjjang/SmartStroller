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

#include <stdlib.h>
#include <unistd.h>
#include <peripheral_io.h>
#include <sys/time.h>

#include "log.h"
#include "resource/resource_adc_mcp3008.h"

static bool initialized = false;

void resource_close_pressure_sensor(void)
{
	resource_adc_mcp3008_fini();
	initialized = false;
}

int resource_read_pressure_sensor(int ch_num, unsigned int *out_value)
{
	unsigned int read_value = 0;
	int ret = 0;

	if (!initialized) {
		ret = resource_adc_mcp3008_init();
		retv_if(ret != 0, -1);
		initialized = true;
	}
	ret = resource_read_adc_mcp3008(ch_num, &read_value);
	retv_if(ret != 0, -1);

	*out_value = read_value;

	return 0;
}

