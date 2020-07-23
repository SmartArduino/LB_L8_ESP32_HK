/**
 * @file appLaunch_HT.h
 *
 */

#ifndef GUI_APPLAUNCH_HT_H
#define GUI_APPLAUNCH_HT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "iot_lvgl.h"

#include "devDataManage.h"

/*********************
 *      DEFINES
 *********************/
#define HTAPPLICATION_FLG_BITHOLD_RESERVE		 		(0xFFFF)

#define HTAPPLICATION_FLG_BITHOLD_ACCESSORY_REG			(1 << 0) //accessory register
#define HTAPPLICATION_FLG_BITHOLD_DATAUPLD_PERIOD		(1 << 1) 
#define HTAPPLICATION_FLG_BITHOLD_DATAUPLD_TRIG         (1 << 2)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void appLaunchHT_generate(void);
void deviceStatusUpdateToHT(stt_devDataPonitTypedef *staVal);
void deviceDestoryPreventChange(devTypeDef_enum devType);
void homekitDevice_setupPayloadStr_get(char str[]);
void homekitDevice_setupCodeStr_get(char str[]);
bool hkL8_nwkWifiConnected_statusGet(void);

void htAccessoryDeviceRegisterByEvt_trig(void);
void htAccessoryDeviceDataPeriodUploadByEvt_trig(void);
void htAccessoryDeviceDataUploadTrigByEvt_trig(void);
void htAccessoryDeviceDataPeriodUploadBussiness_loopReales(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*GUI_APPLAUNCH_HT_H*/


