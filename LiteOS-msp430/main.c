/*
 * ------------------------------------
 * Last Modified: Friday, 1st November 2019 4:06:24 pm
 * Modified By: srLiu 
 * ------------------------------------
 * CGLog:
 * 1. creat three tasks(under `./src/apps`)
 * 2. those three tasks communicate through two non-blocking queues (`rawData` and `refinedData`)
 *      tskCollection--->(queue: rawData)--->tskCalculation--->(queue: refinedData)--->tskReport
 * 3. message size and task-related info are defined in `./src/apps/apps.h`
 * ------------------------------------
 * TODO:
 * 1. driver init code.
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

/** Functions&variables. */
static void prv_HardwareInit();
static void prv_GPIOInit();
static void prv_CSInit();
static void prv_IICInit();      //TODO: please add the driver init codes.
static void prv_UartInit();     //TODO: please add the driver init codes.
UINT32 prv_DemoTaskCreate_All();

UINT32 g_uwRawQueue;
UINT32 g_uwRefinedQueue;
#define SIZE_QUEUE   20          //queue size
#define SIZE_MSG     15           //message length: (*)bytes
/**
 * main entry;
 */
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

    //Tasks creation:
    uwRet = creat_data_collection_task();
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    uwRet = creat_data_calculation_task();
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    uwRet = creat_data_report_task();
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }

    //Queues creation:
    uwRet = LOS_QueueCreate("rawData", SIZE_QUEUE, &g_uwRawQueue, 0, SIZE_MSG);
    if (uwRet != LOS_OK)
    {
        return uwRet;
    }
    uwRet = LOS_QueueCreate("refinedData", SIZE_QUEUE, &g_uwRefinedQueue, 0, SIZE_MSG);
    if (uwRet != LOS_OK)
    {
        return uwRet;
    }

    return uwRet;
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
    prv_IICInit();
    prv_UartInit();
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

/**
 * IIC init.
 */
void prv_IICInit()
{

}

/**
 * Uart init.
 */
void prv_UartInit()
{

}
