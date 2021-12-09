/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-18     Meco Man      First version
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_USE_PERF_MONITOR     1
#define LV_COLOR_DEPTH          16

#define LV_USE_GPU_STM32_DMA2D      1
#define LV_GPU_DMA2D_CMSIS_INCLUDE  "stm32h750xx.h"

#define LV_HOR_RES_MAX          (1024)
#define LV_VER_RES_MAX          (600)

#ifdef LVGL_USING_DEMO_BENCHMARK
#define LV_USE_DEMO_BENCHMARK       1
#endif

#ifdef LVGL_USING_DEMO_MUSIC
#define LV_USE_DEMO_MUSIC           1
#endif

#ifdef LVGL_USING_DEMO_STRESS
#define LV_USE_DEMO_STRESS          1
#endif

#ifdef LVGL_USING_DEMO_KEYPAD_ENCODER
#define LV_USE_DEMO_KEYPAD_AND_ENCODER   1
#endif

#ifdef LVGL_USING_DEMO_WIDGETS
#define LV_USE_DEMO_WIDGETS         1
#endif

#define LV_FONT_MONTSERRAT_12       1
#define LV_FONT_MONTSERRAT_16       1
#define LV_FONT_MONTSERRAT_28       1

#endif
