/*
 * arch/arm/mach-tegra/board-ventana-sensors.c
 *
 * Copyright (c) 2010, NVIDIA, All Rights Reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <linux/i2c.h>
#include <mach/gpio.h>
#include <linux/i2c/nct1008.h>
#include <linux/akm8975.h>

#include "gpio-names.h"

#define ISL29018_IRQ_GPIO	TEGRA_GPIO_PZ2
#define AKM8975_IRQ_GPIO	TEGRA_GPIO_PN5

static void ventana_isl29018_init(void)
{
	tegra_gpio_enable(ISL29018_IRQ_GPIO);
	gpio_request(ISL29018_IRQ_GPIO, "isl29018");
	gpio_direction_input(ISL29018_IRQ_GPIO);
}

static void ventana_akm8975_init(void)
{
	tegra_gpio_enable(AKM8975_IRQ_GPIO);
	gpio_request(AKM8975_IRQ_GPIO, "akm8975");
	gpio_direction_input(AKM8975_IRQ_GPIO);
}

struct nct1008_platform_data ventana_nct1008_pdata = {
	.conv_rate = 5,
	.config = NCT1008_CONFIG_ALERT_DISABLE,
	.thermal_threshold = 110,
};

static const struct i2c_board_info ventana_i2c0_board_info[] = {
	{
		I2C_BOARD_INFO("isl29018", 0x44),
		.irq = TEGRA_GPIO_TO_IRQ(TEGRA_GPIO_PZ2),
	},
};

static const struct i2c_board_info ventana_i2c2_board_info[] = {
	{
		I2C_BOARD_INFO("bq20z75-battery", 0x0B),
	},
};

static struct i2c_board_info ventana_i2c4_board_info[] = {
	{
		I2C_BOARD_INFO("nct1008", 0x4C),
		.platform_data = &ventana_nct1008_pdata,
	},
	{
		I2C_BOARD_INFO("akm8975", 0x0C),
		.irq = TEGRA_GPIO_TO_IRQ(AKM8975_IRQ_GPIO),
	}
};

int __init ventana_sensors_init(void)
{
	ventana_isl29018_init();
	ventana_akm8975_init();

	i2c_register_board_info(0, ventana_i2c0_board_info,
		ARRAY_SIZE(ventana_i2c0_board_info));

	i2c_register_board_info(2, ventana_i2c2_board_info,
		ARRAY_SIZE(ventana_i2c2_board_info));

	i2c_register_board_info(4, ventana_i2c4_board_info,
		ARRAY_SIZE(ventana_i2c4_board_info));

	return 0;
}