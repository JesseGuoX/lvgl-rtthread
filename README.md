# 基于 ART-PI 的 LVGL-Demos 移植

## 简介

本例程主要功能是在 ART-PI 上运行 LVGL-Demos.

## 硬件说明

本工程主要硬件为 RGB 显示屏，以及 ART-Pi.

## 软件说明

LVGL-Demos 源码位于 libraries/lvgl/demo，共有 5 个 LVGL 官方提供的例程。LVGL 所用版本为 [LVGL](https://github.com/lvgl/lvgl.git) 官方最新代码。

在 `lvgl_thread` 线程中，根据不同的宏定义使用不同的 demo。

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
​  
 while(1)  
 {  
 lv_task_handler();  
 rt_thread_mdelay(1);  
 }  
}

## 使用说明

1. 使用 git 工具 clone 本工程：`https://github.com/liukangcc/ART-PI-LVGL-DEMO.git`

2. 在工程目录下打开 env 工具，输入命令 `menuconfig`

3. 选择一个 demo，保存并退出

4. 输入命令 `pkgs --update` 拉取 LVGL 软件包

5. 输入命令 `scons --target=mdk5` 生成 KEIL 工程

6. 打开 keil，编译下载代码；

## 演示效果

### benchmark

## keypad_encoder

## music

## stress

## widgets


