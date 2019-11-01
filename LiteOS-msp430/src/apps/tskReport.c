/*
 * ------------------------------------
 * Last Modified: Friday, 1st November 2019 4:04:49 pm
 * Modified By: srLiu 
 * ------------------------------------
 * CGLog:
 * ------------------------------------
 * TODO:
 * 1. nb driver
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

static UINT32 g_datareport_tskHandle;
extern UINT32 g_uwRefinedQueue;
static VOID data_report_task(VOID);

UINT32 creat_data_report_task(VOID)
{
    UINT32 uwRet = LOS_OK;

    TSK_INIT_PARAM_S task_init_param;
    task_init_param.usTaskPrio = 10;
    task_init_param.pcName = "data_report_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC) data_report_task;
    task_init_param.uwStackSize = 0x200;

    uwRet = LOS_TaskCreate(&g_datareport_tskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;

}

VOID data_report_task(VOID)
{
    //UINT32 uwRet = LOS_OK;
    UINT32 uwInfoPack;
    CHAR   cInfoPack[16];
    QUEUE_INFO_S xRefinedQueue;
    UINT16 usNumPack = 0;

    while(1)
    {
        (VOID)LOS_QueueInfoGet(g_uwRefinedQueue, &xRefinedQueue);
        usNumPack = xRefinedQueue.usReadableCnt;
        while(usNumPack!=0)
        {
            (VOID)LOS_QueueRead(g_uwRefinedQueue, &uwInfoPack, Q_MSG_SIZE, 0);
            sprintf(cInfoPack, "%s", (char *)uwInfoPack);
            usNumPack--;
#if 0
            //TODO: nb driver & send msg in cInfoPack.
#endif
        }
        LOS_TaskDelay(TSK3_PERIOD);
    }
}
