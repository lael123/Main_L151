/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-06-05     RT-Thread    first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "radio.h"
#include "radio_config.h"
#include "ax5043.h"
#include "device.h"
#include "led.h"
#include "work.h"
#include "easyflash.h"
#include "flashwork.h"
#include "key.h"
#include "moto.h"
#include "RTCWork.h"
#include "status.h"
#include "dog.h"
#include "adcwork.h"
#include "radio_decoder.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

int main(void)
{
    //cm_backtrace_init("1","2","3");
    led_Init();
    Key_Reponse();
    button_Init();
    flash_Init();
    easyflash_init();
    LoadDevice2Memory();
    Moto_Init();
    wdt_sample();
    Delay_Timer_Init();
    RTC_Init();
    WarningInit();
    WaterScan_Init();
    Radio_Task_Init();
    ADC_Init();
    while (1)
    {
        FeedDog();
        rt_thread_mdelay(1000);
    }
    return RT_EOK;
}
