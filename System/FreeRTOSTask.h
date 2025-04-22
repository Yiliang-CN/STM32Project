#ifndef __FREERTOSTASK_H
#define __FREERTOSTASK_H

#include "FreeRTOS.h"
#include "task.h"

// 是否使用任务 1使用0不使用
// 如果需要使用任务 请打开此宏定义
#define isUseTask 0

#if isUseTask

void LED_Task(void *pvParameters);
void Buzzer_Task(void *pvParameters);

void RD03_Task(void *pvParameters);
void MPU6050_Task(void *pvParameters);

#endif

#endif
