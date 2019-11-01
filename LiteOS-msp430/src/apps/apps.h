/*
 * ------------------------------------
 * Last Modified: Friday, 1st November 2019 4:01:13 pm
 * Modified By: srLiu 
 * ------------------------------------
 * CGLog:
 * ------------------------------------
 * TODO:
 * ------------------------------------
 */


#ifndef SRC_APPS_APPS_H_
#define SRC_APPS_APPS_H_

#define delay_ms(x)     __delay_cycles(x*16*1000)       //16MHz
#define TSK1_PERIOD         50
#define TSK2_PERIOD         50
#define TSK3_PERIOD         500

#define Q_MSG_SIZE          16

//Task 1: collection
UINT32 creat_data_collection_task(VOID);

//Task 2: calculation
UINT32 creat_data_calculation_task(VOID);

//Task 3: report
UINT32 creat_data_report_task(VOID);

#endif /* SRC_APPS_APPS_H_ */
