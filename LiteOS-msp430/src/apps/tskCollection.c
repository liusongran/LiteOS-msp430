/*
 * ------------------------------------
 * Last Modified: Friday, 1st November 2019 4:03:27 pm
 * Modified By: srLiu 
 * ------------------------------------
 * CGLog:
 * ------------------------------------
 * TODO:
 * 1. sensor_read()
 * 2. Temp_Rh()
 * ------------------------------------
 */


/** LiteOS includes. */
#include <msp430.h>
#include "los_base.h"
#include "los_task.h"
#include "los_queue.h"
#include "los_typedef.h"
#include "los_sys.h"
/** Driver includes. */
#include <driverlib.h>
/** App includes here. */
#include "apps.h"

static UINT32 g_datacollect_tskHandle;
static VOID data_collection_task(VOID);
static VOID sensor_read(UINT8 Order);
static VOID Temp_Rh(VOID);

extern UINT32 g_uwRawQueue;

CHAR cTemperature[] = "t=41.14";        //fake input
CHAR cHumidity[] = "h=53.35";           //fake input

UINT32 creat_data_collection_task(VOID)
{
    UINT32 uwRet = LOS_OK;

    TSK_INIT_PARAM_S task_init_param;
    task_init_param.usTaskPrio = 2;
    task_init_param.pcName = "data_collection_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC) data_collection_task;
    task_init_param.uwStackSize = 0x200;

    uwRet = LOS_TaskCreate(&g_datacollect_tskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

VOID data_collection_task(VOID)
{
    int count = 1;
    unsigned char i;
    CHAR cInfoPack[16];
    UINT32 uwRet = 0;

    while(1)
    {
#if 0
        sprintf(str, "collect = %d\r\n", count++);
        printf(str);
#endif
        //read humiturel
        Temp_Rh();
        sensor_read(0xE3);      //read temperature
        delay_ms(10);
        sensor_read(0xE5);      //read humidity
        delay_ms(10);
#if 0
        sprintf(str, "temperature = %d%d%c%d%d\r\n", buf[0], buf[1], '.', buf[2], buf[3]);
        printf(str);
        sprintf(str, "humidity = %d%d%c%d%d\r\n", buf[4], buf[5], '.', buf[6], buf[7]);
        printf(str);
#endif

        //put the sensor data to message queue.
        sprintf(cInfoPack, "%s%s", cTemperature, cHumidity);
        uwRet = LOS_QueueWrite(g_uwRawQueue, cInfoPack, Q_MSG_SIZE, 0);
        if(uwRet != LOS_OK)
        {
            //printf("send message failure,error:%x\n",uwRet);
        }

        LOS_TaskDelay(TSK1_PERIOD);
    }
}

VOID sensor_read(UINT8 Order)
{
    //TODO:
}

VOID Temp_Rh(VOID)
{
    //TODO:
}
