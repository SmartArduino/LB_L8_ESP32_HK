#include "devDriver_temperatureMeasure.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_freertos_hooks.h"

#include "driver/periph_ctrl.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "soc/gpio_sig_map.h"

#include "mlink.h"
#include "mwifi.h"
#include "mdf_common.h"

#include "devDriver_manage.h"

#define DEVDRIVER_TEMPERATUREMEASURE_FILTER_COEFFIE			0.05F

#define DEVDRIVER_TEMPERATUREMEASURE_DEFAULT_VREF       	1100
#define DEVDRIVER_TEMPERATUREMEASURE_NO_OF_SAMPLES			64

#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SELF_HARDWARE)
 #define DEVDRIVER_TEMPERATUREMEASURE_DATA_COMPENSATION_IF	1 //是否进行温度负补偿
#else
 #define DEVDRIVER_TEMPERATUREMEASURE_DATA_COMPENSATION_IF	0 //是否进行温度负补偿
#endif

EventGroupHandle_t xEventGp_tempratureActDetect = NULL;

static uint16_t devParam_reduceTimeCount = 0;
static float 	devParam_reduceTempRecord = 0.0F;
static float 	devParam_temprature = 0.0F;

static uint8_t 	devParam_tempratureCaliOpreat = DEV_TEMPRATURE_DATA_CAL_MAX_RANGE / 2; //温度校准值，用于补偿，实际值等于当前值除10，精度0.1
static uint8_t  devTempratureCalData_nvsSave_timeDelay_count = 0;	//存储延迟计时变量

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_3;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;

const struct _stt_ntcCheckTab{ //NTC对照表

	int16_t tempratureVal;
	uint16_t voltageDectectVal;
	
}ntcResTemperature_checkTab[DEVDRIVER_TEMPERATUREDETECT_ADC_CHECKTAB_LEN] = {

//	/*1k分压电阻对应Tab --num:96*/
//	{-30, 0x0049}, {-29, 0x004D}, {-28, 0x0052}, {-27, 0x0057}, {-26, 0x005D}, {-25, 0x0063}, {-24, 0x0069}, {-23, 0x006F}, {-22, 0x0076}, {-21, 0x007D}, {-20, 0x0084}, {-19, 0x008C},
//	{-18, 0x0094}, {-17, 0x009D}, {-16, 0x00A6}, {-15, 0x00AF}, {-14, 0x00B9}, {-13, 0x00C4}, {-12, 0x00CE}, {-11, 0x00DA}, {-10, 0x00E6}, {-9,  0x00F2}, {-8,  0x00FF}, {-7,  0x010D},
//	{-6,  0x011B}, {-5,  0x012A}, {-4,  0x013A}, {-3,  0x014A}, {-2,  0x015B}, {-1,  0x016D}, {0,   0x017F}, {1,   0x0193}, {2,   0x01A7}, {3,   0x01BC}, {4,   0x01D1}, {5,   0x01E8},
//	{6,   0x01FF}, {7,   0x0218}, {8,   0x0231}, {9,   0x024C}, {10,  0x0267}, {11,  0x0283}, {12,  0x02A1}, {13,  0x02BF}, {14,  0x02DF}, {15,  0x0300}, {16,  0x0321}, {17,  0x0344},
//	{18,  0x0369}, {19,  0x038E}, {20,  0x03B5}, {21,  0x03DD}, {22,  0x0406}, {23,  0x0430}, {24,  0x045C}, {25,  0x0489}, {26,  0x04B8}, {27,  0x04E8}, {28,  0x0519}, {29,  0x054C},
//	{30,  0x0580}, {31,  0x05B5}, {32,  0x05EC}, {33,  0x0625}, {34,  0x065F}, {35,  0x069A}, {36,  0x06D6}, {37,  0x0715}, {38,  0x0754}, {39,  0x0795}, {40,  0x07D8}, {41,  0x081C},
//	{42,  0x0861}, {43,  0x08A8}, {44,  0x08F0}, {45,  0x093A}, {46,  0x0985}, {47,  0x09D1}, {48,  0x0A1F}, {49,  0x0A6D}, {50,  0x0ABE}, {51,  0x0B0F}, {52,  0x0B62}, {53,  0x0BB6},
//	{54,  0x0C0B}, {55,  0x0C61}, {56,  0x0CB8}, {57,  0x0D10}, {58,  0x0D6A}, {59,  0x0DC4}, {60,  0x0E1F}, {61,  0x0E7B}, {62,  0x0ED8}, {63,  0x0F36}, {64,  0x0F94}, {65,  0x0FF3},
	{-30, 19}, {-29, 20}, {-28, 21}, {-27, 22}, {-26, 24}, {-25, 25}, {-24, 27}, {-23, 28}, {-22, 30}, {-21, 32}, {-20, 34}, {-19, 36}, {-18, 38}, {-17, 40}, {-16, 42},
	{-15, 45}, {-14, 47}, {-13, 50}, {-12, 53}, {-11, 56}, {-10, 59}, {-9, 62},  {-8, 65}, {-7, 69},   {-6, 72},  {-5, 76},  {-4, 80},  {-3, 84},  {-2, 89},  {-1, 93}, 
	{0, 98},   {1, 103},  {2, 108},  {3, 114},  {4, 119},  {5, 125},  {6, 131},  {7, 138}, {8, 144},   {9, 151},  {10, 158}, {11, 165}, {12, 173}, {13, 181}, {14, 189}, 
	{15, 198}, {16, 206}, {17, 215}, {18, 225}, {19, 235}, {20, 245}, {21, 255}, {22, 266}, {23, 277}, {24, 288}, {25, 300}, {26, 312}, {27, 325}, {28, 337}, {29, 351},
	{30, 364}, {31, 378}, {32, 393}, {33, 407}, {34, 423}, {35, 438}, {36, 454}, {37, 470}, {38, 487}, {39, 504}, {40, 522}, {41, 539}, {42, 558}, {43, 576}, {44, 595},
	{45, 615}, {46, 635}, {47, 655}, {48, 675}, {49, 696}, {50, 717}, {51, 739}, {52, 761}, {53, 783}, {54, 805}, {55, 828}, {56, 851}, {57, 875}, {58, 899}, {59, 923},
	{60, 947}, {61, 971}, {62, 996}, {63, 1021},{64, 1046},{65, 1071},
};

//static float temperatureParsing_fromNTCtab(uint16_t adcDectectVal){

//	float temperatureRes = 0.0F;
//	uint16_t insertTemp = DEVDRIVER_TEMPERATUREDETECT_ADC_CHECKTAB_LEN / 2;
//	uint16_t insertHalfTemp = DEVDRIVER_TEMPERATUREDETECT_ADC_CHECKTAB_LEN / 2;
//	uint16_t loopOp = 0;

//	//范围限定
//	if(adcDectectVal < ntcResTemperature_checkTab[3].adcDectectVal)return -273.15F;
//	if(adcDectectVal > ntcResTemperature_checkTab[DEVDRIVER_TEMPERATUREDETECT_ADC_CHECKTAB_LEN - 3].adcDectectVal)return 1000.0F;

//	//二分查表
//	do{
//	
//		loopOp ++;

//		insertHalfTemp /= 2;

//		if(adcDectectVal < ntcResTemperature_checkTab[insertTemp].adcDectectVal){

//			insertTemp -= insertHalfTemp;
//		}
//		else
//		{
//			insertTemp += insertHalfTemp;
//		}

//	}while(insertHalfTemp);

//	//区间线性计算
//	if(adcDectectVal == ntcResTemperature_checkTab[insertTemp].adcDectectVal){
//		
//		temperatureRes = (float)ntcResTemperature_checkTab[insertTemp].tempratureVal;
//	}
//	else
//	{
//		
//		float divisorTemp = 0.0F;
//		
//		if(adcDectectVal < ntcResTemperature_checkTab[insertTemp].adcDectectVal){
//			
//			if(adcDectectVal == ntcResTemperature_checkTab[insertTemp - 1].adcDectectVal)
//				temperatureRes = (float)ntcResTemperature_checkTab[insertTemp - 1].tempratureVal;
//			else{
//				
//				divisorTemp = (float)(ntcResTemperature_checkTab[insertTemp].tempratureVal - ntcResTemperature_checkTab[insertTemp - 1].tempratureVal) /
//							  (float)(ntcResTemperature_checkTab[insertTemp].adcDectectVal - ntcResTemperature_checkTab[insertTemp - 1].adcDectectVal);
//				temperatureRes = (float)(adcDectectVal - ntcResTemperature_checkTab[insertTemp - 1].adcDectectVal) *  divisorTemp + (float)ntcResTemperature_checkTab[insertTemp - 1].tempratureVal;
//			}
//		}
//		else
//		{
//			if(adcDectectVal == ntcResTemperature_checkTab[insertTemp + 1].adcDectectVal)
//				temperatureRes = (float)ntcResTemperature_checkTab[insertTemp + 1].tempratureVal;
//			else{
//				
//				divisorTemp = (float)(ntcResTemperature_checkTab[insertTemp + 1].tempratureVal - ntcResTemperature_checkTab[insertTemp].tempratureVal) /
//							  (float)(ntcResTemperature_checkTab[insertTemp + 1].adcDectectVal - ntcResTemperature_checkTab[insertTemp].adcDectectVal);
//				temperatureRes = (float)(adcDectectVal - ntcResTemperature_checkTab[insertTemp].adcDectectVal) *  divisorTemp + (float)ntcResTemperature_checkTab[insertTemp].tempratureVal;
//			}
//		}
//	}
//	
//	//	printf("insert check:%d.\n", insertTemp);
//	//	printf("loopOp:%d.\n", loopOp);
//	//	printf("insertHalf check:%d.\n", insertHalfTemp);
//	
//	return temperatureRes;
//}

static float temperatureParsing_fromNTCtab(uint16_t voltageDectectVal){

	float temperatureRes = 0.0F;
	uint16_t insertTemp = DEVDRIVER_TEMPERATUREDETECT_ADC_CHECKTAB_LEN / 2;
	uint16_t insertHalfTemp = DEVDRIVER_TEMPERATUREDETECT_ADC_CHECKTAB_LEN / 2;
	uint16_t loopOp = 0;

	//范围限定
	if(voltageDectectVal < ntcResTemperature_checkTab[3].voltageDectectVal)return -273.15F;
	if(voltageDectectVal > ntcResTemperature_checkTab[DEVDRIVER_TEMPERATUREDETECT_ADC_CHECKTAB_LEN - 3].voltageDectectVal)return 1000.0F;

	//二分查表
	do{
	
		loopOp ++;

		insertHalfTemp /= 2;

		if(voltageDectectVal < ntcResTemperature_checkTab[insertTemp].voltageDectectVal){

			insertTemp -= insertHalfTemp;
		}
		else
		{
			insertTemp += insertHalfTemp;
		}

	}while(insertHalfTemp);

	//区间线性计算
	if(voltageDectectVal == ntcResTemperature_checkTab[insertTemp].voltageDectectVal){
		
		temperatureRes = (float)ntcResTemperature_checkTab[insertTemp].tempratureVal;
	}
	else
	{
		
		float divisorTemp = 0.0F;
		
		if(voltageDectectVal < ntcResTemperature_checkTab[insertTemp].voltageDectectVal){
			
			if(voltageDectectVal == ntcResTemperature_checkTab[insertTemp - 1].voltageDectectVal)
				temperatureRes = (float)ntcResTemperature_checkTab[insertTemp - 1].tempratureVal;
			else{
				
				divisorTemp = (float)(ntcResTemperature_checkTab[insertTemp].tempratureVal - ntcResTemperature_checkTab[insertTemp - 1].tempratureVal) /
							  (float)(ntcResTemperature_checkTab[insertTemp].voltageDectectVal - ntcResTemperature_checkTab[insertTemp - 1].voltageDectectVal);
				temperatureRes = (float)(voltageDectectVal - ntcResTemperature_checkTab[insertTemp - 1].voltageDectectVal) *  divisorTemp + (float)ntcResTemperature_checkTab[insertTemp - 1].tempratureVal;
			}
		}
		else
		{
			if(voltageDectectVal == ntcResTemperature_checkTab[insertTemp + 1].voltageDectectVal)
				temperatureRes = (float)ntcResTemperature_checkTab[insertTemp + 1].tempratureVal;
			else{
				
				divisorTemp = (float)(ntcResTemperature_checkTab[insertTemp + 1].tempratureVal - ntcResTemperature_checkTab[insertTemp].tempratureVal) /
							  (float)(ntcResTemperature_checkTab[insertTemp + 1].voltageDectectVal - ntcResTemperature_checkTab[insertTemp].voltageDectectVal);
				temperatureRes = (float)(voltageDectectVal - ntcResTemperature_checkTab[insertTemp].voltageDectectVal) *  divisorTemp + (float)ntcResTemperature_checkTab[insertTemp].tempratureVal;
			}
		}
	}
	
//	printf("insert check:%d.\n", insertTemp);
//	printf("loopOp:%d.\n", loopOp);
//	printf("insertHalf check:%d.\n", insertHalfTemp);

	return temperatureRes;
}

static void check_efuse(void){
	
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}

static void print_char_val_type(esp_adc_cal_value_t val_type){

    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}

static void devDriverBussiness_temperatureMeasure_adcInit(void){

	//Check if Two Point or Vref are burned into eFuse
	check_efuse();

	//Configure ADC
	if (unit == ADC_UNIT_1) {
		adc1_config_width(ADC_WIDTH_BIT_12);
		adc1_config_channel_atten(channel, atten);
	} else {
		adc2_config_channel_atten((adc2_channel_t)channel, atten);
	}

	//Characterize ADC
	adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
	esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEVDRIVER_TEMPERATUREMEASURE_DEFAULT_VREF, adc_chars);
	print_char_val_type(val_type);
}

float devDriverBussiness_temperatureMeasure_temperatureReales(void){

	const float dataDef_tempratureAbsoluteErr = 0.0F;
	const uint16_t localTimeLoop = DEVDRIVER_TEMPERATUREDETECT_PERIODLOOP_TIME;

	stt_paramLinkageConfig linkageConfigParamGet_temp = {0};

	static bool tempMeasure_startFlg = false;
	static float tempSample_record = 0.0F;
	float tempSample_new = 0.0F;
	uint32_t adc_reading = 0;

	//温度联动业务
	devSystemOpration_linkageConfig_paramGet(&linkageConfigParamGet_temp);
	if(linkageConfigParamGet_temp.linkageRunning_temprature_en){

		if(devParam_temprature == linkageConfigParamGet_temp.linkageCondition_tempratureVal){

			currentDev_dataPointSet(&(linkageConfigParamGet_temp.linkageReaction_temprature_swVal), true, true, true, false, false);
		}
	}

//	//温度负补偿 //暂时不用定值校准，使用人工校准
//	if(devParam_reduceTimeCount < 2400){

//		devParam_reduceTimeCount += localTimeLoop; //计时

//		if(devParam_reduceTimeCount < 1200){ //前1200s 机身增加4.5℃

//			devParam_reduceTempRecord += 0.0334F;
//		}
//		else //后1200s 机身增加2℃
//		{
//			devParam_reduceTempRecord += 0.0167F;
//		}
//	}
	
	//Multisampling
	for(int i = 0; i < DEVDRIVER_TEMPERATUREMEASURE_NO_OF_SAMPLES; i++){

		int raw;
		
		if (unit == ADC_UNIT_1) {

			raw = adc1_get_raw((adc1_channel_t)channel);
			adc_reading += raw;
			
		} else {
		
			adc2_get_raw((adc2_channel_t)channel, ADC_WIDTH_BIT_12, &raw);
			adc_reading += raw;
		}
	}
	adc_reading /= DEVDRIVER_TEMPERATUREMEASURE_NO_OF_SAMPLES;
	//Convert adc_reading to voltage in mV
	uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);

	tempSample_new = temperatureParsing_fromNTCtab((uint16_t)voltage);

	if(!tempMeasure_startFlg){ //起始采样

		if(tempSample_new < 45.0F){ //首次采样为样本值，必须相对标准

			tempMeasure_startFlg = true;

			tempSample_record = tempSample_new;
		}
	}
	else
	{
		if(((tempSample_new - tempSample_record) > 10.0F) || //±10.0F大波动废值丢弃
		   ((tempSample_record - tempSample_new) > 10.0F)){

			printf("temprature data not qualified.\n");

			return devParam_temprature;
		}
	}

//	printf("Raw: %d<%04X>, Voltage: %dmV, dReduce:-%f\n", adc_reading, adc_reading, voltage, devParam_reduceTempRecord);

	//一阶滞后
	tempSample_record = DEVDRIVER_TEMPERATUREMEASURE_FILTER_COEFFIE * tempSample_record + (1.0F - DEVDRIVER_TEMPERATUREMEASURE_FILTER_COEFFIE) * tempSample_new;
	
#if(DEVDRIVER_TEMPERATUREMEASURE_DATA_COMPENSATION_IF == 1)

 #if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED)

 	devParam_temprature = tempSample_record;
 #else
 
//	devParam_temprature = tempSample_record - devParam_reduceTempRecord; //暂时不用定值校准，使用人工校准

	devParam_temprature = tempSample_record + devTempratureSensor_dataCal_hanldeGet(devParam_tempratureCaliOpreat);
 #endif
#else

	devParam_temprature = tempSample_record;
#endif

	devParam_temprature += dataDef_tempratureAbsoluteErr; //绝对误差合并

	return devParam_temprature;
}

float devDriverBussiness_temperatureMeasure_get(void){

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED) && (INDEP_TEMPERATURE_MEARSURE_BY_DS18B20 == 1)
	
	return devDriverBussiness_tempMeasureByDs18b20_get();
#else
	
	return devParam_temprature;		
#endif
}

static void devTempratureSensor_configParamSave_actionTrig(void){

	devSystemInfoLocalRecord_save(saveObj_devDriver_tempratureCal_valDat, &devParam_tempratureCaliOpreat);
}

void devTempratureSensor_configParamSave_actionDetect(void){

	if(devTempratureCalData_nvsSave_timeDelay_count != COUNTER_DISENABLE_MASK_SPECIALVAL_U8){

		if(devTempratureCalData_nvsSave_timeDelay_count)devTempratureCalData_nvsSave_timeDelay_count --;
		else{

			devTempratureCalData_nvsSave_timeDelay_count = COUNTER_DISENABLE_MASK_SPECIALVAL_U8;
			devTempratureSensor_configParamSave_actionTrig();
		}
	}
}

float devTempratureSensor_dataCal_hanldeGet(uint8_t param){

	float tempCal_maxRange = DEV_TEMPRATURE_DATA_CAL_MAX_RANGE / 10.0f;
	float ret = (float)param / 10.0f - (tempCal_maxRange / 2.0f); //中间范围

	return ret;
}

void devTempratureSensor_dataCal_set(uint8_t dataCal, bool nvsRecord_IF){

	printf("temprature caliVal set val:%f.\n", devTempratureSensor_dataCal_hanldeGet(dataCal));

	devParam_tempratureCaliOpreat = dataCal;
	
	if(nvsRecord_IF){

		devTempratureCalData_nvsSave_timeDelay_count = DEVDRIVER_TEMPERATUREDETECT_CAL_PARAMSAVE_TIMEDELAY;
		devDriverBussiness_temperatureMeasure_temperatureReales(); //设置完成后，超前更新一次		
	}
}

uint8_t devTempratureSensor_dataCal_get(void){

	return devParam_tempratureCaliOpreat;
}

void devDriverBussiness_temperatureMeasure_getByHex(stt_devTempParam2Hex *param){
	
#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED) && (INDEP_TEMPERATURE_MEARSURE_BY_DS18B20 == 1)

	devDriverBussiness_tempMeasureByDs18b20_getByHex(param);
#else
	
	const float decimal_prtCoefficient = 100.0F; //小数计算偏移倍数 --100倍对应十进制两位
	float tempratureCaculate_temp = devParam_temprature + DEVDRIVER_TEMPERATUREMEASURE_NEGATIVE_BOUND;
	
	uint16_t dataInteger_prt = (uint16_t)tempratureCaculate_temp & 0xFFFF;
	uint8_t dataDecimal_prt = (uint8_t)((tempratureCaculate_temp - (float)dataInteger_prt) * decimal_prtCoefficient);
	
	if(devParam_temprature < 0.0F)dataDecimal_prt = 99 - dataDecimal_prt; //针对负数处理
	
	param->integer_h8bit = (uint8_t)((dataInteger_prt & 0xFF00) >> 8);
	param->integer_l8bit = (uint8_t)((dataInteger_prt & 0x00FF) >> 0);
	param->decimal_8bit = dataDecimal_prt;
#endif
}

int16_t devDriverBussiness_temperatureRevoveInteger_fromHex(stt_devTempParam2Hex *param){

	int16_t tempVal_temp = 0;
	const int16_t valHandle = (int16_t)DEVDRIVER_TEMPERATUREMEASURE_NEGATIVE_BOUND;

	if(NULL != param){

		tempVal_temp = param->integer_h8bit * 256 + param->integer_l8bit;
		tempVal_temp -= valHandle;
	}

	return tempVal_temp;
}

void devDriverBussiness_temperatureMeasure_periphInit(void){

	devDriverBussiness_temperatureMeasure_adcInit();

	xEventGp_tempratureActDetect = xEventGroupCreate();
}


