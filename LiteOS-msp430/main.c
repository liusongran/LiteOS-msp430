
/** LiteOS includes. */
#include <msp430.h>
#include "los_base.h"
#include "los_task.h"
#include "los_typedef.h"
#include "los_sys.h"
/** Driver includes. */
#include <driverlib.h>
/** App includes here. */

/** Functions&variables. */
static void prv_HardwareInit();
static void prv_GPIOInit();
static void prv_CSInit();
UINT32 prv_DemoTaskCreate_All();

static UINT32 g_redblink_tskHandle;
static void redblink();

main()
{
    prv_HardwareInit();

    UINT32 uwRet = LOS_OK;
    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    uwRet =prv_DemoTaskCreate_All();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    (void)LOS_Start();

}

/**
 * Create all demo tasks.
 */
UINT32 prv_DemoTaskCreate_All(VOID)
{
    UINT32 uwRet = LOS_OK;

    TSK_INIT_PARAM_S task_init_param;
    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "redblink";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC) redblink;
    task_init_param.uwStackSize = 0x0600;

    uwRet = LOS_TaskCreate(&g_redblink_tskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

void redblink(void)
{
while(1)
{
    P1DIR |= 0x03;                          // Set P1.0 to output direction
    P1OUT = 0x02;

    UINT64 old,now;
    old = LOS_TickCountGet ();
    now = LOS_TickCountGet ();
    //LOS_TaskSuspend(1);
    while(now - old <300)
    {
        volatile unsigned int i;            // volatile to prevent optimization
        P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR
        i = 65535;                          // SW Delay
        do i--;
        while(i != 0);
        now = LOS_TickCountGet ();
    }
    P1OUT = 0x00;

    LOS_TaskDelay(500);
}
}


/**
 * hardware init.
 */
void prv_HardwareInit(void)
{
    /** stop watchdog */
    WDTCTL = WDTPW | WDTHOLD;

    /** Disable the GPIO power-on default high-impedance mode. */
    PM5CTL0 &= ~LOCKLPM5;
    P4DIR |= BIT1;      //P4.1
    P4DIR |= BIT2;      //P4.2
    P4OUT &= ~BIT1;     //p4.1 output 0
    P4OUT &= ~BIT2;

    /** Initialization */
    prv_GPIOInit();
    prv_CSInit();

}

/**
 * GPIO init.
 */
void prv_GPIOInit()
{
    /* Terminate all GPIO pins to Output LOW to minimize power consumption */
    GPIO_setAsOutputPin(GPIO_PORT_PA, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PB, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PC, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PD, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PE, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PF, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PA, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PB, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PC, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PD, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PE, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PF, GPIO_PIN_ALL16);
}

/**
 * Clock System init.
 */
void prv_CSInit()
{
    /** Set DCO frequency to 16MHz */
    CS_setDCOFreq(CS_DCORSEL_1, CS_DCOFSEL_4);

    /**
     * Configure one FRAM waitstate as required by the device datasheet for MCLK
     * operation beyond 8MHz _before_ configuring the clock system.
     */
    FRCTL0 = FRCTLPW | NWAITS_1;

    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK,CS_LFXTCLK_SELECT,CS_CLOCK_DIVIDER_1);
}
