/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-18     Meco Man     The first version
 */
#include <lvgl.h>
#include <lcd_port.h>

//#define DRV_DEBUG
#define LOG_TAG             "lvgl.disp"
#include <drv_log.h>

/*A static or global variable to store the buffers*/
static lv_disp_draw_buf_t disp_buf;

rt_device_t lcd_device = 0;
static struct rt_device_graphic_info info;

static lv_disp_drv_t disp_drv;  /*Descriptor of a display driver*/

static DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef LtdcHandle;
volatile uint8_t g_gpu_state = 0;

static void lvgl_dma2d_config(void)
{
      hdma2d.Instance = DMA2D;
      hdma2d.Init.Mode = DMA2D_M2M;
      hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
      hdma2d.Init.OutputOffset = 0;

      hdma2d.LayerCfg[1].InputOffset = 0;
      hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
      hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
      hdma2d.LayerCfg[1].InputAlpha = 255;
      hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;
      hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR;
      hdma2d.LayerCfg[1].ChromaSubSampling = DMA2D_NO_CSS;
      if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
      {
        Error_Handler();
      }
      if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
      {
        Error_Handler();
      }
}

void DMA2D_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    if ((DMA2D->ISR & DMA2D_FLAG_TC) != 0U)
    {
        if ((DMA2D->CR & DMA2D_IT_TC) != 0U)
        {

            DMA2D->CR &= ~DMA2D_IT_TC;
            DMA2D->IFCR = DMA2D_FLAG_TC;

            if (g_gpu_state == 1)
            {
                g_gpu_state = 0;
                lv_disp_flush_ready(&disp_drv);
            }
        }
    }

    rt_interrupt_leave();
}

static void lcd_fb_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    uint32_t OffLineSrc = LV_HOR_RES_MAX - (area->x2 - area->x1 + 1);
    uint32_t addr = (uint32_t) LtdcHandle.LayerCfg[0].FBStartAdress + 2 * (LV_HOR_RES_MAX * area->y1 + area->x1);

    DMA2D->CR = 0x00000000UL | (1 << 9);

    DMA2D->FGMAR = (uint32_t) (uint16_t*) (color_p);

    DMA2D->OMAR = (uint32_t) addr;

    DMA2D->FGOR = 0;

    DMA2D->OOR = OffLineSrc;

    DMA2D->FGPFCCR = DMA2D_OUTPUT_RGB565;
    DMA2D->OPFCCR = DMA2D_OUTPUT_RGB565;

    DMA2D->NLR = (area->y2 - area->y1 + 1) | ((area->x2 - area->x1 + 1) << 16);

    DMA2D->CR |= DMA2D_IT_TC | DMA2D_IT_TE | DMA2D_IT_CE;

    DMA2D->CR |= DMA2D_CR_START;

    g_gpu_state = 1;
}

#define COLOR_BUFFER  (LV_HOR_RES_MAX * LV_VER_RES_MAX)
void lv_port_disp_init(void)
{
    rt_err_t result;
    void* buf_1 = RT_NULL;
    
    lcd_device = rt_device_find("lcd");
    if (lcd_device == 0)
    {
        LOG_E("error!");
        return;
    }
    result = rt_device_open(lcd_device, 0);
    if (result != RT_EOK)
    {
        LOG_E("error!");
        return;
    }

    /* get framebuffer address */
    result = rt_device_control(lcd_device, RTGRAPHIC_CTRL_GET_INFO, &info);
    if (result != RT_EOK)
    {
        LOG_E("error!");
        /* get device information failed */
        return;
    }

    RT_ASSERT (info.bits_per_pixel == 8 || info.bits_per_pixel == 16 ||
              info.bits_per_pixel == 24 || info.bits_per_pixel == 32);

    lvgl_dma2d_config();

    buf_1 = rt_malloc_align(COLOR_BUFFER * sizeof(lv_color_t), COLOR_BUFFER * sizeof(lv_color_t));
    if (buf_1 == RT_NULL)
    {
        LOG_E("malloc memory failed");
        return;
    }
    rt_memset(buf_1, 0, COLOR_BUFFER * sizeof(lv_color_t));

    /*Initialize `disp_buf` with the buffer(s).*/
    lv_disp_draw_buf_init(&disp_buf, buf_1, NULL, COLOR_BUFFER);

    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = info.width;
    disp_drv.ver_res = info.height;
    LOG_I("width: %d, height:%d", info.width, info.height);
    /*Set a display buffer*/
    disp_drv.draw_buf = &disp_buf;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = lcd_fb_flush;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}
