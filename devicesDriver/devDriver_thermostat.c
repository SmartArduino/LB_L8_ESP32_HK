#include "devDriver_thermostat.h"

#include "mlink.h"
#include "mwifi.h"
#include "mdf_common.h"

#include "devDriver_manage.h"

#define DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY1     	12
#define DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY2     	14
#define DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY3     	27

#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SLAVE_MCU)
 #if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)
  #define DEVDRIVER_THERMOSTAT_ADJUST_HEAT_START()		devDriverApp_statusExexuteBySlaveMcu_byBit(2, 1);devDriverApp_statusExexuteBySlaveMcu_byBit(1, 0)
  #define DEVDRIVER_THERMOSTAT_ADJUST_COOL_START()		devDriverApp_statusExexuteBySlaveMcu_byBit(2, 0);devDriverApp_statusExexuteBySlaveMcu_byBit(1, 1)
  #define DEVDRIVER_THERMOSTAT_ADJUST_STOP()			devDriverApp_statusExexuteBySlaveMcu_byBit(2, 0);devDriverApp_statusExexuteBySlaveMcu_byBit(1, 0)
  #define DEVDRIVER_THERMOSTAT_VENT_FAN_OPT(x)			devDriverApp_statusExexuteBySlaveMcu_byBit(0, x)

  #define DEVDRIVER_THERM_EX_RELAY1_STATUS_SET(a)		
  #define DEVDRIVER_THERM_EX_RELAY2_STATUS_SET(a)		
 #else
  #define DEVDRIVER_THERMOSTAT_ADJUST_START() 		   	devDriverApp_statusExexuteBySlaveMcu_byBit(0, 1)
  #define DEVDRIVER_THERMOSTAT_ADJUST_START_MX()		devDriverApp_statusExexuteBySlaveMcu(0x07)
  #define DEVDRIVER_THERMOSTAT_ADJUST_STOP()			devDriverApp_statusExexuteBySlaveMcu(0x00)
  #define DEVDRIVER_THERMOSTAT_ADJUST_STOP_IX()		   	devDriverApp_statusExexuteBySlaveMcu_byBit(0, 0)
														
  #define DEVDRIVER_THERM_EX_RELAY1_STATUS_SET(a) 	   	devDriverApp_statusExexuteBySlaveMcu_byBit(1, a)
  #define DEVDRIVER_THERM_EX_RELAY2_STATUS_SET(a) 	   	devDriverApp_statusExexuteBySlaveMcu_byBit(2, a)												
  #if(DEVICE_THERMOSTAT_PEDESTAL_HEATER_RESERVE == 1)													
   #define DEVDRIVER_THERMOSTAT_ACTION_POWERSTART()    	devDriverApp_statusExexuteBySlaveMcu(0x01)
   #define DEVDRIVER_THERMOSTAT_ACTION_POWERSYNCH()    	devDriverApp_statusExexuteBySlaveMcu(0x07)
   #define DEVDRIVER_THERMOSTAT_ACTION_POWEROFF()	  	devDriverApp_statusExexuteBySlaveMcu(0x00)
  #endif
 #endif
#else
 #if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)
  #define DEVDRIVER_THERMOSTAT_ADJUST_HEAT_START()		gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY3, (uint32_t)1);gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY2, (uint32_t)0)
  #define DEVDRIVER_THERMOSTAT_ADJUST_COOL_START()		gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY3, (uint32_t)0);gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY2, (uint32_t)1)
  #define DEVDRIVER_THERMOSTAT_ADJUST_STOP()			gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY3, (uint32_t)0);gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY2, (uint32_t)0)
  #define DEVDRIVER_THERMOSTAT_VENT_FAN_OPT(x)			gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY1, (uint32_t)x)													

  #define DEVDRIVER_THERM_EX_RELAY1_STATUS_SET(a) 	  
  #define DEVDRIVER_THERM_EX_RELAY2_STATUS_SET(a)   
 #else
  #define DEVDRIVER_THERMOSTAT_ADJUST_START() 		   	gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY1, (uint32_t)1)
  #define DEVDRIVER_THERMOSTAT_ADJUST_START_MX()		gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY1, (uint32_t)1);\
														gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY2, (uint32_t)1);\
														gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY3, (uint32_t)1)
  #define DEVDRIVER_THERMOSTAT_ADJUST_STOP()			gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY1, (uint32_t)0);\
														gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY2, (uint32_t)0);\
														gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY3, (uint32_t)0)
  #define DEVDRIVER_THERMOSTAT_ADJUST_STOP_IX()		   	gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY1, (uint32_t)0)
	
  #define DEVDRIVER_THERM_EX_RELAY1_STATUS_SET(a) 	   	gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY2, (uint32_t)a)
  #define DEVDRIVER_THERM_EX_RELAY2_STATUS_SET(a) 	   	gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY3, (uint32_t)a)
	
  #if(DEVICE_THERMOSTAT_PEDESTAL_HEATER_RESERVE == 1)
	
   #define DEVDRIVER_THERMOSTAT_ACTION_POWERSTART()    	gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY3, (uint32_t)1)
   #define DEVDRIVER_THERMOSTAT_ACTION_POWERSYNCH()    	gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY2, (uint32_t)1);\
														gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY1, (uint32_t)1)
   #define DEVDRIVER_THERMOSTAT_ACTION_POWEROFF()	   	gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY1, (uint32_t)0);\
														gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY2, (uint32_t)0);\
														gpio_set_level(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY3, (uint32_t)0)
  #endif
 #endif
#endif

#if(DEVICE_THERMOSTAT_PEDESTAL_HEATER_RESERVE == 1)

	enum{

		thermostatSycnStep_null = 0,
		thermostatSycnStep_close,
		thermostatSycnStep_openStdBy,
		thermostatSycnStep_open,
	};

	static struct{

		uint8_t thermostatPedestalHeater_relaySycnCounter;
		uint8_t actStep;
		
	}thermostatSycnAct_param = {

		.thermostatPedestalHeater_relaySycnCounter = COUNTER_DISENABLE_MASK_SPECIALVAL_U8,
		.actStep = thermostatSycnStep_close,
	};
#endif

static bool devDriver_moudleInitialize_Flg = false;

static struct{

	uint8_t swVal_bit1:1; 
	uint8_t swVal_bit2:1; 
	
	uint8_t rsv:6;
}thermostatExSwParam_rcd = {0};

static stt_thermostat_actAttr devParam_thermostat = {

	.workModeInNight_IF = 0,
	.temperatureVal_target = DEVICE_THERMOSTAT_TEMPTARGET_DEFAULT,
	.temperatureVal_current = 16,
};

static void devDriverBussiness_thermostatSwitch_periphInit(void){

	devTypeDef_enum swCurrentDevType = currentDev_typeGet();
	gpio_config_t io_conf = {0};

	if((swCurrentDevType != devTypeDef_thermostat) &&
	   (swCurrentDevType != devTypeDef_thermostatExtension))return;
	
	//disable interrupt
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set
	io_conf.pin_bit_mask = (1ULL << DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY1) |
						   (1ULL << DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY2) |
						   (1ULL << DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY3);
	//disable pull-down mode
	io_conf.pull_down_en = 0;
	//disable pull-up mode
	io_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&io_conf);
}

static void devDriverBussiness_thermostatSwitch_periphDeinit(void){

	DEVDRIVER_THERMOSTAT_ADJUST_STOP();
	gpio_reset_pin(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY1);
	gpio_reset_pin(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY2);
	gpio_reset_pin(DEVDRIVER_THERMOSTAT_GPIO_OUTPUT_IO_RLY3);
}

void devDriverBussiness_datapointDefaultParamLoad(void){

	stt_devDataPonitTypedef devDatapoint_temp = {

		.devType_thermostat.devThermostat_tempratureTarget =\
			DEVICE_THERMOSTAT_TEMPTARGET_DEFAULT,
	};

	currentDev_dataPointRecovery(&devDatapoint_temp, false, false, false, false, false);
}

void devDriverBussiness_thermostatSwitch_moudleInit(void){

	devTypeDef_enum swCurrentDevType = currentDev_typeGet();

	if((swCurrentDevType != devTypeDef_thermostat) &&
	   (swCurrentDevType != devTypeDef_thermostatExtension))return;
	if(devDriver_moudleInitialize_Flg)return;

#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SLAVE_MCU)
					
#else

	devDriverBussiness_thermostatSwitch_periphInit();		
#endif

	devDriver_moudleInitialize_Flg = true;
}

void devDriverBussiness_thermostatSwitch_moudleDeinit(void){

	if(!devDriver_moudleInitialize_Flg)return;

	//数据清空
	thermostatExSwParam_rcd.swVal_bit1 =\
	thermostatExSwParam_rcd.swVal_bit2 =\
	devParam_thermostat.deviceExSwstatus_rly1 =\
	devParam_thermostat.deviceExSwstatus_rly2 = 0;

#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SLAVE_MCU)
									
#else

	devDriverBussiness_thermostatSwitch_periphDeinit();
#endif
	
	devDriver_moudleInitialize_Flg = false;
}

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)
void devDriverBussiness_thermostatSwitch_runningDetectLoop(void){

	uint8_t adjustTrig_shortVal = 0;
	float tempratureRead_temp = devDriverBussiness_temperatureMeasure_get();

	static bool enableRecord = false;

	devParam_thermostat.temperatureVal_current = (uint16_t)tempratureRead_temp;

	DEVDRIVER_THERMOSTAT_VENT_FAN_OPT(devParam_thermostat.deviceExSwstatus_fans);

	if(thermostatRunningStatus_mode_stop == devParam_thermostat.deviceRunning_MODE){

		if(enableRecord){
		
			enableRecord = false;
			
			switch(currentDev_typeGet()){

				case devTypeDef_thermostat:{

					DEVDRIVER_THERMOSTAT_ADJUST_STOP();

				}break;

				default:break;
			}
		}

		return;
	}

	enableRecord = true;

	switch(devParam_thermostat.deviceRunning_MODE){

		case thermostatRunningStatus_mode_heat:{

			(devParam_thermostat.workModeInNight_IF)?
				(adjustTrig_shortVal = 5):
				(adjustTrig_shortVal = 2);
			switch(currentDev_typeGet()){

				case devTypeDef_thermostat:{

					if(devParam_thermostat.temperatureVal_current <= (devParam_thermostat.temperatureVal_target - adjustTrig_shortVal)){

						DEVDRIVER_THERMOSTAT_ADJUST_HEAT_START();
					}
					else
					if(devParam_thermostat.temperatureVal_current > devParam_thermostat.temperatureVal_target){

						DEVDRIVER_THERMOSTAT_ADJUST_STOP();
					}

				}break;

				default:break;
			}

		}break;
		
		case thermostatRunningStatus_mode_cool:{

			(devParam_thermostat.workModeInNight_IF)?
				(adjustTrig_shortVal = 5):
				(adjustTrig_shortVal = 2);
			switch(currentDev_typeGet()){

				case devTypeDef_thermostat:{

					if(devParam_thermostat.temperatureVal_current >= (devParam_thermostat.temperatureVal_target + adjustTrig_shortVal)){
					
						DEVDRIVER_THERMOSTAT_ADJUST_COOL_START();
					}
					else
					if(devParam_thermostat.temperatureVal_current < devParam_thermostat.temperatureVal_target){
					
						DEVDRIVER_THERMOSTAT_ADJUST_STOP();
					}

				}break;
			
				default:break;
			}

		}break;
		
		case thermostatRunningStatus_mode_auto:{

			uint8_t adjustTrig_valThreshold_A = 0,
					adjustTrig_valThreshold_B = 0;

			(devParam_thermostat.workModeInNight_IF)?
				(adjustTrig_shortVal = 3):
				(adjustTrig_shortVal = 2);
			switch(currentDev_typeGet()){

				case devTypeDef_thermostat:{

					if(devParam_thermostat.temperatureVal_current <= (devParam_thermostat.temperatureVal_target - adjustTrig_shortVal)){
					
						DEVDRIVER_THERMOSTAT_ADJUST_HEAT_START();
					}
					else
					if(devParam_thermostat.temperatureVal_current >= (devParam_thermostat.temperatureVal_target + adjustTrig_shortVal)){
					
						DEVDRIVER_THERMOSTAT_ADJUST_COOL_START();
					}
					else
					if(devParam_thermostat.temperatureVal_current == devParam_thermostat.temperatureVal_target){

						DEVDRIVER_THERMOSTAT_ADJUST_STOP();
					}

				}break;
			
				default:break;
			}

		}break;

		default:break;
	}
}

void devDriverBussiness_thermostatSwitch_devParam_runMode_set(stt_devThermostat_runningStatus workMode){

	devParam_thermostat.deviceRunning_MODE = workMode;
}

void devDriverBussiness_thermostatSwitch_devParam_exFansState_set(bool openIf){

	devParam_thermostat.deviceExSwstatus_fans = openIf;
}

#else
void devDriverBussiness_thermostatSwitch_runningDetectLoop(void){

	uint8_t adjustTrig_shortVal = 0;
	float tempratureRead_temp = devDriverBussiness_temperatureMeasure_get();

	static bool enableRecord = false;

	devParam_thermostat.temperatureVal_current = (uint16_t)tempratureRead_temp;

	if(0 == devParam_thermostat.deviceRunning_EN){

		if(enableRecord){

			enableRecord = false;

			switch(currentDev_typeGet()){

				case devTypeDef_thermostat:{

 #if(DEVICE_THERMOSTAT_PEDESTAL_HEATER_RESERVE == 1)
									
					DEVDRIVER_THERMOSTAT_ACTION_POWEROFF();
					thermostatSycnAct_param.actStep = thermostatSycnStep_close;
					thermostatSycnAct_param.thermostatPedestalHeater_relaySycnCounter = COUNTER_DISENABLE_MASK_SPECIALVAL_U8;
 #else
	
					DEVDRIVER_THERMOSTAT_ADJUST_STOP();
 #endif
				}break;

				case devTypeDef_thermostatExtension:{

					DEVDRIVER_THERMOSTAT_ADJUST_STOP_IX();

				}break;

				default:break;
			}
		}

		return;
	} //设备启用失能，直接返回
	
	enableRecord = true;
	
	(devParam_thermostat.workModeInNight_IF)?
		(adjustTrig_shortVal = 5):
		(adjustTrig_shortVal = 2);

	if(devParam_thermostat.temperatureVal_current <= (devParam_thermostat.temperatureVal_target - adjustTrig_shortVal)){

		switch(currentDev_typeGet()){
		
			case devTypeDef_thermostat:{

 #if(DEVICE_THERMOSTAT_PEDESTAL_HEATER_RESERVE == 1)

				if(thermostatSycnAct_param.actStep == thermostatSycnStep_close){

					thermostatSycnAct_param.actStep = thermostatSycnStep_openStdBy;
					thermostatSycnAct_param.thermostatPedestalHeater_relaySycnCounter = 1; //继电器延迟跟随时间设定
					DEVDRIVER_THERMOSTAT_ACTION_POWERSTART();
				}
 #else

				DEVDRIVER_THERMOSTAT_ADJUST_START_MX();
 #endif
			}break;
		
			case devTypeDef_thermostatExtension:{

				DEVDRIVER_THERMOSTAT_ADJUST_START();

			}break;

			default:break;
		}
//		printf("on.\n");
	}
	else //施密特触发器
	if(devParam_thermostat.temperatureVal_current >= devParam_thermostat.temperatureVal_target){

		switch(currentDev_typeGet()){
		
			case devTypeDef_thermostat:{

 #if(DEVICE_THERMOSTAT_PEDESTAL_HEATER_RESERVE == 1)

				if(thermostatSycnAct_param.actStep != thermostatSycnStep_close){

					thermostatSycnAct_param.actStep = thermostatSycnStep_close;
					thermostatSycnAct_param.thermostatPedestalHeater_relaySycnCounter = COUNTER_DISENABLE_MASK_SPECIALVAL_U8;
					DEVDRIVER_THERMOSTAT_ACTION_POWEROFF();
				}
 #else

				DEVDRIVER_THERMOSTAT_ADJUST_STOP();
 #endif
			}break;
		
			case devTypeDef_thermostatExtension:{
		
				DEVDRIVER_THERMOSTAT_ADJUST_STOP_IX();
		
			}break;
		
			default:break;
		}
//		printf("off.\n");
	}

 #if(DEVICE_THERMOSTAT_PEDESTAL_HEATER_RESERVE == 1)

	switch(currentDev_typeGet()){

		case devTypeDef_thermostat:{

//			printf("fhasoihfoiasho counter:%d.\n", thermostatSycnAct_param.thermostatPedestalHeater_relaySycnCounter);

			if(thermostatSycnAct_param.thermostatPedestalHeater_relaySycnCounter != COUNTER_DISENABLE_MASK_SPECIALVAL_U8){
			
				if(thermostatSycnAct_param.thermostatPedestalHeater_relaySycnCounter )thermostatSycnAct_param.thermostatPedestalHeater_relaySycnCounter --;
				else{
			
					thermostatSycnAct_param.thermostatPedestalHeater_relaySycnCounter = COUNTER_DISENABLE_MASK_SPECIALVAL_U8;
					thermostatSycnAct_param.actStep = thermostatSycnStep_open;
					DEVDRIVER_THERMOSTAT_ACTION_POWERSYNCH();
				}
			}
			
		}break;

		default:break;
	}
 #endif
//	printf("temperatureVal_current:%d, temperatureVal_target:%d.\n", devParam_thermostat.temperatureVal_current,
//									  								 devParam_thermostat.temperatureVal_target);
}
#endif

void devDriverBussiness_thermostatSwitch_exSwitchParamReales(uint8_t param){

	stt_thermostatExSwParam devStatus_temp = {0};

	memcpy(&devStatus_temp, &param, sizeof(uint8_t));

	thermostatExSwParam_rcd.swVal_bit1 = devParam_thermostat.deviceExSwstatus_rly1;
	thermostatExSwParam_rcd.swVal_bit2 = devParam_thermostat.deviceExSwstatus_rly2;

	devParam_thermostat.deviceExSwstatus_rly1 = devStatus_temp.swVal_bit1;
	devParam_thermostat.deviceExSwstatus_rly2 = devStatus_temp.swVal_bit2;
}

void devDriverBussiness_thermostatSwitch_exSwitchParamSet(uint8_t param){

	stt_thermostatExSwParam devStatus_temp = {0};
	uint8_t statusVal_opreat = 0;

	memcpy(&devStatus_temp, &param, sizeof(uint8_t));
	memcpy(&statusVal_opreat, &param, sizeof(uint8_t));

	thermostatExSwParam_rcd.swVal_bit1 = devParam_thermostat.deviceExSwstatus_rly1;
	thermostatExSwParam_rcd.swVal_bit2 = devParam_thermostat.deviceExSwstatus_rly2;

	if(statusVal_opreat & 0xE0){

		if(devStatus_temp.opCtrl_bit1)
			devParam_thermostat.deviceExSwstatus_rly1 = devStatus_temp.swVal_bit1;
		if(devStatus_temp.opCtrl_bit2)
			devParam_thermostat.deviceExSwstatus_rly2 = devStatus_temp.swVal_bit2;
	}
	else
	{
		devParam_thermostat.deviceExSwstatus_rly1 = devStatus_temp.swVal_bit1;
		devParam_thermostat.deviceExSwstatus_rly2 = devStatus_temp.swVal_bit2;
	}

	DEVDRIVER_THERM_EX_RELAY1_STATUS_SET(devParam_thermostat.deviceExSwstatus_rly1);
	DEVDRIVER_THERM_EX_RELAY2_STATUS_SET(devParam_thermostat.deviceExSwstatus_rly2);
}

uint8_t devDriverBussiness_thermostatSwitch_exSwitchParamGet(void){

	stt_thermostatExSwParam paramGet_temp = {0};
	uint8_t paramGet_tempHex = 0;

	paramGet_temp.swVal_bit1 = devParam_thermostat.deviceExSwstatus_rly1;
	paramGet_temp.swVal_bit2 = devParam_thermostat.deviceExSwstatus_rly2;

	memcpy(&paramGet_tempHex, &paramGet_temp, sizeof(uint8_t));

	return paramGet_tempHex;
}

uint8_t devDriverBussiness_thermostatSwitch_exSwitchRcdParamGet(void){

	uint8_t paramGet_tempHex = 0;

	memcpy(&paramGet_tempHex, &thermostatExSwParam_rcd, sizeof(uint8_t));

	return paramGet_tempHex;
}

uint8_t devDriverBussiness_thermostatSwitch_exSwitchParamGetWithRcd(void){

	stt_thermostatExSwParam paramGet_temp = {0};
	uint8_t paramGet_tempHex = 0;

	paramGet_temp.swVal_bit1 = devParam_thermostat.deviceExSwstatus_rly1;
	paramGet_temp.swVal_bit2 = devParam_thermostat.deviceExSwstatus_rly2;

	//操作位 区分
	memcpy(&paramGet_tempHex, &paramGet_temp, sizeof(uint8_t));
	(thermostatExSwParam_rcd.swVal_bit1 != devParam_thermostat.deviceExSwstatus_rly1)?
		(paramGet_tempHex |=  (1 << 6)):
		(paramGet_tempHex &= ~(1 << 6));
	(thermostatExSwParam_rcd.swVal_bit2 != devParam_thermostat.deviceExSwstatus_rly2)?
		(paramGet_tempHex |=  (1 << 7)):
		(paramGet_tempHex &= ~(1 << 7));

	return paramGet_tempHex;
}

void devDriverBussiness_thermostatSwitch_periphStatusReales(stt_devDataPonitTypedef *param){

	devTypeDef_enum swCurrentDevType = currentDev_typeGet();

	if((swCurrentDevType == devTypeDef_thermostat) ||
	   (swCurrentDevType == devTypeDef_thermostatExtension)){

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)
		if(0 == param->devType_thermostat.devThermostat_running_en){

			devParam_thermostat.deviceRunning_MODE = thermostatRunningStatus_mode_stop;
		}
		else{

			// if(devParam_thermostat.deviceRunning_MODE == thermostatRunningStatus_mode_stop)
			// 	devParam_thermostat.deviceRunning_MODE = thermostatRunningStatus_mode_auto;
		}
#else
		devParam_thermostat.deviceRunning_EN = 
			param->devType_thermostat.devThermostat_running_en;
#endif

		devParam_thermostat.workModeInNight_IF = 
			param->devType_thermostat.devThermostat_nightMode_en;

		devParam_thermostat.temperatureVal_target = 
			param->devType_thermostat.devThermostat_tempratureTarget;

		if(devParam_thermostat.temperatureVal_target < 16)
			devParam_thermostat.temperatureVal_target = 16;
	}
}

void devDriverBussiness_thermostatSwitch_devParam_get(stt_thermostat_actAttr *param){

	memcpy(param, &devParam_thermostat, sizeof(stt_thermostat_actAttr));
}



