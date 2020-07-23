/**
 * @file tips_bussinessAcoustoOptic.h
 *
 */

#ifndef TIPS_BUSSINESSACOUSTOOPTIC_H
#define TIPS_BUSSINESSACOUSTOOPTIC_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "esp_types.h"

/*********************
 *      DEFINES
 *********************/
#define DEVDRIVER_EX_LEDTIPS_REFRESH_PERIOD			0.00005F	//LED提示驱动更新时间分量 -hTime

/**********************
 *      TYPEDEFS
 **********************/
typedef enum{

	socketTypeSpecifi_Safrica = 0, //南非，0
	socketTypeSpecifi_General, //常规，1
	socketTypeSpecifi_British, //英规，2
	socketTypeSpecifi_America  //美规，3
}enumSpecification_socketType;

typedef enum{

   ntStatus_noneNet = 0,
   ntStatus_offline,
   ntStatus_online,
}enum_tipsNetworkStatus;

typedef enum{

   tipsRunningStatus_normal = 0,
   tipsRunningStatus_upgrading,
   tipsRunningStatus_funcTrig,
}enum_tipsDevNoneScrRunningStatus;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void devTipsByLed_driverReales(void);

void devStatusRunning_tipsRefresh(void);
void devTipsAcoustoOpticPeriphInit(void);

void devTipsStatusRunning_abnormalTrig(enum_tipsDevNoneScrRunningStatus status, uint16_t time);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*TIPS_BUSSINESSACOUSTOOPTIC_H*/




