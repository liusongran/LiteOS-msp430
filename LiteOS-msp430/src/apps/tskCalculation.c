/*
 * ------------------------------------
 * Last Modified: Friday, 1st November 2019 4:01:41 pm
 * Modified By: srLiu 
 * ------------------------------------
 * CGLog:
 * ------------------------------------
 * TODO:
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

static UINT32 g_datacalculation_tskHandle;
static VOID data_calculation_task(VOID);
static VOID redBlinkTest(VOID);

extern UINT32 g_uwRawQueue;
extern UINT32 g_uwRefinedQueue;


UINT32 creat_data_calculation_task(VOID)
{
    UINT32 uwRet = LOS_OK;

    TSK_INIT_PARAM_S task_init_param;
    task_init_param.usTaskPrio = 5;
    task_init_param.pcName = "data_calculation_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC) data_calculation_task;
    task_init_param.uwStackSize = 0x200;

    uwRet = LOS_TaskCreate(&g_datacalculation_tskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;

}

VOID data_calculation_task(VOID)
{
    //UINT32 uwRet = LOS_OK;
    UINT32 uwInfoPack;
    CHAR   cInfoPack[16];
    QUEUE_INFO_S xRawQueue;
    UINT16 usNumPack = 0;

    while(1)
    {
        redBlinkTest();
        //Get information of rawQueue.
        (VOID)LOS_QueueInfoGet(g_uwRawQueue, &xRawQueue);
        //check the number of available packages in queue.
        usNumPack = xRawQueue.usReadableCnt;        
        while(usNumPack!=0)
        {
            (VOID)LOS_QueueRead(g_uwRawQueue, &uwInfoPack, Q_MSG_SIZE, 0);
            sprintf(cInfoPack, "%s", (char *)uwInfoPack);
            //TODO: add some math computation here.
            //cal(cInfoPack);
            (VOID)LOS_QueueWrite(g_uwRefinedQueue, cInfoPack, Q_MSG_SIZE, 0);
            usNumPack--;
        }
        LOS_TaskDelay(TSK2_PERIOD);
    }
}

void redBlinkTest(void)
{
    P1DIR |= 0x03;                          // Set P1.0 to output direction
    P1OUT = 0x02;

    UINT64 old,now;
    old = LOS_TickCountGet ();
    now = LOS_TickCountGet ();
    //LOS_TaskSuspend(1);
    while(now - old <10)
    {
        volatile unsigned int i;            // volatile to prevent optimization
        P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR
        i = 65535;                          // SW Delay
        do i--;
        while(i != 0);
        now = LOS_TickCountGet ();
    }
    P1OUT = 0x00;
}
