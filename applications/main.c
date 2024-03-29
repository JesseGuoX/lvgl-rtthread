/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"

/* defined the LED0 pin: PI8 */
#define LED0_PIN    GET_PIN(I, 8)
#define USER_KEY    GET_PIN(H, 4)

int main(void)
{
    rt_uint32_t count = 1;

    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(USER_KEY, PIN_MODE_INPUT_PULLDOWN);

    while(count++)
    {
        if(rt_pin_read(USER_KEY)){
            rt_pin_write(LED0_PIN, PIN_HIGH);
            rt_thread_mdelay(500);
            rt_pin_write(LED0_PIN, PIN_LOW);
            rt_thread_mdelay(500);
        }
        rt_thread_mdelay(10);
    }
    return RT_EOK;
}

#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);


