/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-17     Meco Man      First version
 */
#include <rtthread.h>
#include <lvgl.h>
#define DBG_TAG    "LVGL"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>
#include <lcd_port.h>
#include <lv_port_indev.h>

#ifndef LV_THREAD_STACK_SIZE
#define LV_THREAD_STACK_SIZE 4096
#endif

#ifndef LV_THREAD_PRIO
#define LV_THREAD_PRIO (RT_THREAD_PRIORITY_MAX*2/3)
#endif

static void lvgl_thread(void *parameter)
{
#if LV_USE_DEMO_BENCHMARK
    extern void lv_demo_benchmark(void);
    lv_demo_benchmark();
#elif LV_USE_DEMO_STRESS
    void lv_demo_stress(void);
    lv_demo_stress();
#elif LV_USE_DEMO_MUSIC
    extern void lv_demo_music(void);
    lv_demo_music();
#elif LV_USE_DEMO_KEYPAD_AND_ENCODER
    extern void lv_demo_keypad_encoder(void);
    lv_demo_keypad_encoder();
#elif LV_USE_DEMO_WIDGETS
    extern void lv_demo_widgets(void);
    lv_demo_widgets();
#endif

    while(1)
    {
        lv_task_handler();
        rt_thread_mdelay(1);
    }
}

static int lvgl_demo_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("LVGL", lvgl_thread, RT_NULL, LV_THREAD_STACK_SIZE, LV_THREAD_PRIO, 10);
    if(tid == RT_NULL)
    {
        LOG_E("Fail to create 'LVGL' thread");
    }
    rt_thread_startup(tid);

    return 0;
}
INIT_APP_EXPORT(lvgl_demo_init);
