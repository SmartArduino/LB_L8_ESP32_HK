/**
 * @file devDriver_temperature.h
 *
 */

#ifndef DEVDRIVER_TEMPERATUREMEASURE_H
#define DEVDRIVER_TEMPERATUREMEASURE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "esp_types.h"

#include "devDataManage.h"

/*********************
 *      DEFINES
 *********************/
#define DEVDRIVER_TEMPERATUREMEASURE_NEGATIVE_BOUND				32767.0F	//数据转HEX传输时正负转换界限值

#define DEVDRIVER_TEMPERATUREDETECT_PERIODLOOP_TIME				2.0F

#define DEVDRIVER_TEMPERATUREDETECT_ADC_CHECKTAB_LEN			96

#define DEV_TEMPRATURE_DATA_CAL_MAX_RANGE						100			//最大操作调节范围

#define DEVDRIVER_TEMPERATUREDETECT_CAL_PARAMSAVE_TIMEDELAY		3			//屏幕运行参数存储 动作延迟 时间 单位：s
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
 * GLOBAL PROTOTYPES
 **********************/
void devDriverBussiness_temperatureMeasure_periphInit(void);
float devDriverBussiness_temperatureMeasure_temperatureReales(void);
float devDriverBussiness_temperatureMeasure_get(void);
void devTempratureSensor_configParamSave_actionDetect(void);
float devTempratureSensor_dataCal_hanldeGet(uint8_t param);
void devTempratureSensor_dataCal_set(uint8_t dataCal, bool nvsRecord_IF);
uint8_t devTempratureSensor_dataCal_get(void);
void devDriverBussiness_temperatureMeasure_getByHex(stt_devTempParam2Hex *param);
int16_t devDriverBussiness_temperatureRevoveInteger_fromHex(stt_devTempParam2Hex *param);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DEVDRIVER_TEMPERATUREMEASURE_H*/






