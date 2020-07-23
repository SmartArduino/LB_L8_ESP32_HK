#include "devDataManage.h"

#include "os.h"

#include "mdf_common.h"
#include "mwifi.h"
#include "mlink.h"

#include "devDriver_manage.h"
#include "bussiness_timerSoft.h"
#include "bussiness_timerHard.h"

LV_IMG_DECLARE(homepageCtrlObjIcon_1size32);
LV_IMG_DECLARE(homepageCtrlObjIcon_2size32);
LV_IMG_DECLARE(homepageCtrlObjIcon_3size32);

LV_IMG_DECLARE(btnIconHome_meeting);
LV_IMG_DECLARE(homepageCtrlObjIcon_1);LV_IMG_DECLARE(homepageCtrlObjIcon_2);LV_IMG_DECLARE(homepageCtrlObjIcon_3);
LV_IMG_DECLARE(homepageCtrlObjIcon_4);LV_IMG_DECLARE(homepageCtrlObjIcon_5);LV_IMG_DECLARE(homepageCtrlObjIcon_6);
LV_IMG_DECLARE(homepageCtrlObjIcon_7);LV_IMG_DECLARE(homepageCtrlObjIcon_8);LV_IMG_DECLARE(homepageCtrlObjIcon_9);
LV_IMG_DECLARE(homepageCtrlObjIcon_10);LV_IMG_DECLARE(homepageCtrlObjIcon_11);LV_IMG_DECLARE(homepageCtrlObjIcon_12);
LV_IMG_DECLARE(homepageCtrlObjIcon_13);LV_IMG_DECLARE(homepageCtrlObjIcon_14);LV_IMG_DECLARE(homepageCtrlObjIcon_15);
LV_IMG_DECLARE(homepageCtrlObjIcon_16);LV_IMG_DECLARE(homepageCtrlObjIcon_17);LV_IMG_DECLARE(homepageCtrlObjIcon_18);
LV_IMG_DECLARE(homepageCtrlObjIcon_19);LV_IMG_DECLARE(homepageCtrlObjIcon_20);LV_IMG_DECLARE(homepageCtrlObjIcon_21);
LV_IMG_DECLARE(homepageCtrlObjIcon_22);LV_IMG_DECLARE(homepageCtrlObjIcon_23);LV_IMG_DECLARE(homepageCtrlObjIcon_24);
LV_IMG_DECLARE(homepageCtrlObjIcon_25);LV_IMG_DECLARE(homepageCtrlObjIcon_26);LV_IMG_DECLARE(homepageCtrlObjIcon_27);
LV_IMG_DECLARE(homepageCtrlObjIcon_28);LV_IMG_DECLARE(homepageCtrlObjIcon_29);LV_IMG_DECLARE(homepageCtrlObjIcon_30);
LV_IMG_DECLARE(homepageCtrlObjIcon_31);LV_IMG_DECLARE(homepageCtrlObjIcon_32);LV_IMG_DECLARE(homepageCtrlObjIcon_33);
LV_IMG_DECLARE(homepageCtrlObjIcon_34);LV_IMG_DECLARE(homepageCtrlObjIcon_35);
LV_IMG_DECLARE(homepageCtrlObjIcon2_1);LV_IMG_DECLARE(homepageCtrlObjIcon2_2);LV_IMG_DECLARE(homepageCtrlObjIcon2_3);
LV_IMG_DECLARE(homepageCtrlObjIcon2_4);LV_IMG_DECLARE(homepageCtrlObjIcon2_5);LV_IMG_DECLARE(homepageCtrlObjIcon2_6);
LV_IMG_DECLARE(homepageCtrlObjIcon2_7);LV_IMG_DECLARE(homepageCtrlObjIcon2_8);LV_IMG_DECLARE(homepageCtrlObjIcon2_9);
LV_IMG_DECLARE(homepageCtrlObjIcon2_10);LV_IMG_DECLARE(homepageCtrlObjIcon2_11);LV_IMG_DECLARE(homepageCtrlObjIcon2_12);
LV_IMG_DECLARE(homepageCtrlObjIcon3_1);LV_IMG_DECLARE(homepageCtrlObjIcon3_2);LV_IMG_DECLARE(homepageCtrlObjIcon3_3);
LV_IMG_DECLARE(homepageCtrlObjIcon3_4);LV_IMG_DECLARE(homepageCtrlObjIcon3_5);LV_IMG_DECLARE(homepageCtrlObjIcon3_6);
LV_IMG_DECLARE(homepageCtrlObjIcon3_7);LV_IMG_DECLARE(homepageCtrlObjIcon3_8);

stt_nodeDev_hbDataManage *listHead_nodeDevDataManage = NULL;
stt_nodeDev_detailInfoManage *listHead_nodeInfoDetailManage = NULL;
stt_nodeObj_listManageDevCtrlBase *listHead_nodeCtrlObjBlockBaseManage = NULL;

uint8_t devRunningTimeFromPowerUp_couter = 0; //设备启动时间 计时变量
uint8_t devRestartDelay_counter = COUNTER_DISENABLE_MASK_SPECIALVAL_U8; //设备重启，倒计时延时执行时间

#if(SCREENSAVER_RUNNING_ENABLE == 1)

 stt_screensaverDispAttr screensaverDispAttrParam = {

	.idlePeriod = SCREENSAVER_DISP_IDLE_TIME,
 };
 stt_epidemicReqRunningAttr epidemicDataRunningParam = {

	.reqTimePeriod = EPIDEMIC_DATA_REQ_PERIOD_TIME,
 };
#endif

xQueueHandle msgQh_dataManagementHandle = NULL; //用于通知数据已被修改成功

bool mutualCtrlTrigIf_A = false; 
bool mutualCtrlTrigIf_B = false;
bool mutualCtrlTrigIf_C = false;

const char meshPsd_default[20] = DEV_MESH_PSD_DEFAULT();

//uint8_t *dataPtr_btnTextImg_sw_A = NULL;
//uint8_t *dataPtr_btnTextImg_sw_B = NULL;
//uint8_t *dataPtr_btnTextImg_sw_C = NULL;

uint8_t bGroundPic_reserveFlg = 0;
uint8_t *dataPtr_bGroundPic = NULL;

static void devGuiBussinessHome_btnTextPic_save(uint8_t picIst, uint8_t *picData);

static const char *TAG = "lanbon_L8 - dataManage";

static const char *NVS_DATA_L8_PARTITION_NAME		= "L8_devDataRcord";

static const char *NVS_DATA_SYSINFO_RECORD			= "devSys_Info";   //nvs条目描述字符串限长16

static const char *DATA_SWSTATUS					= "devStatus";
static const char *DATA_DEV_STATUS_RECORD_IF		= "devStatusRecord";
static const char *DATA_INFO_TIMER_NORMAL			= "devTimerNormal";
static const char *DATA_INFO_TIMER_IRDATA			= "devTimerIRdat";
static const char *DATA_INFO_TIMER_NIGHTMODE		= "devTimerMnight";
static const char *DATA_INFO_TIMER_GREENMODE		= "devTimerMgreen";
static const char *DATA_INFO_DEVRUNNINGFLG			= "devRunningFlg";
static const char *DATA_TIMEZONE					= "devTimeZone";
static const char *DATA_DEV_ELECSUM					= "devElecSum";
static const char *DATA_DEV_TYPEDEF					= "devTypeDef";
static const char *DATA_DEV_ROUTER_BSSID			= "devRouterBssid";
static const char *DATA_DEV_ROUTER_BSSIDRCD			= "devRtBssidRcd";
static const char *DATA_DEV_MUTUALCTRL_INFO			= "devMutualInfo";
static const char *DATA_DEV_LINKAGE_CONFIG			= "devLinkageCfg";
static const char *DATA_DEV_GUIHOMEBTNTEXTDISP		= "devBtnTextDisp";
static const char *DATA_DEV_GUIHOMEBTNICONDISP		= "devBtnIconDisp";
static const char *DATA_DEV_GUIHOMEBTNTEXTPIC_A		= "devBtnTextPicA";
static const char *DATA_DEV_GUIHOMEBTNTEXTPIC_B		= "devBtnTextPicB";
static const char *DATA_DEV_GUIHOMEBTNTEXTPIC_C		= "devBtnTextPicC";
static const char *DATA_DEV_GUIHOME_BGROUDPIC		= "devBGoundPic";
static const char *DATA_DEV_GUIHOMETHEMEPARAM		= "devThemeParam";
static const char *DATA_DEV_DEVATMOSRUNPARAM		= "devAtmosParam";
static const char *DATA_DEV_TEMPRATURE_CALPARAM		= "devTempCalParam";	
static const char *DATA_DEVDRVIPT_RECALIBRAPARAM	= "devDrviptParam";		//屏幕重新校准使能参数
static const char *DATA_DEVSCREEN_CONFIGPARAM		= "devScreenParam"; 
static const char *DATA_DEVCURTAIN_RUNNINGPARAM		= "devCurtainParam";
static const char *DATA_DEVTHERMOSTATEX_SWPARAM		= "devThrExParam";
static const char *DATA_DEVINFRARED_DATA_PARAM		= "devInfDats_";
static const char *DATA_DEVSCENARIO_DATA_PARAM_0	= "devScenDats_0";
static const char *DATA_DEVSCENARIO_DATA_PARAM_1	= "devScenDats_1";
static const char *DATA_DEVSCENARIO_DATA_PARAM_2	= "devScenDats_2";
static const char *DATA_DTMQTT_CFGPARAM				= "dtMqttCfgParam";
static const char *DATA_HASERVER_CFGPARAM			= "dtHaServerParam";
static const char *DATA_ROUTER_CFG_INFO				= "routerCfg_info";
static const char *DATA_DEVHEATER_CUSTOMTIME		= "devHeaterCstTim";
static const char *DATA_GREENMODE_USRCFG			= "greenModeCfgDat";

static const char *DATA_EPIDCYLOCATION				= "epidCyLocation";
static const char *DATA_RELAYMAG_TESTPARAM			= "testDtMgRelay";

static SemaphoreHandle_t xSph_usrAppNvsOpreat = NULL;

static uint8_t meshAppOpt_rootFirstConNotice_count = 0;

static stt_dataDisp_guiBussinessHome_btnText dataBtnTextObjDisp_bussinessHome = {

//	.countryFlg = countryT_Arabic,
//	.dataBtnTextDisp[0] = "غرفة الاجتماعاتغرفة الاجتماعاتغرفة الاجتماعات",
//	.dataBtnTextDisp[1] = "غرفة النوم",
//	.dataBtnTextDisp[2] = "مرحاض",

	.countryFlg = countryT_EnglishSerail,
	.dataBtnTextDisp[0] = "light1",
	.dataBtnTextDisp[1] = "light2",
	.dataBtnTextDisp[2] = "light3",
};
static uint8_t dataBtnIconNumObjDisp_bussinessHome[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {1, 1, 1};

static bool listNodeDevStatus_opreatingFlg = false; //节点链表(开关状态)是否正在被进行管理操作
static bool listNodeDevDetailInfo_opreatingFlg = false; //节点链表(开关状态)是否正在被进行管理操作
static bool listNodeDevLvGblock_opreatingFlg = false; //节点链表(开关状态)是否正在被进行管理操作

static bool meshNetworkParamReserve_Flg = false;

static int8_t devSignalStrength2ParentVal = -127; //设备信号强度
static uint16_t devMesh_currentNodeNum = 0; //当前所在mesh网络内节点数量

static bool devRouterOrMeshConnect_flg = false;

static uint16_t devCurrentRunningFlg = 0;

static uint8_t routerConnect_bssid[6] = {0};
static uint8_t routerRecord_bssid[6] = {0};

static stt_devMutualGroupParam devMutualCtrl_group[DEVICE_MUTUAL_CTRL_GROUP_NUM] = {0};

static char devIptdrvParam_recalibration = 'D';

static stt_paramLinkageConfig devSysParam_linkageConfig = {0};

static stt_routerCfgInfo routerConfiguration_info = {0};

static stt_mqttCfgParam dtMqttParamInfo = {

	.host_domain = MQTT_REMOTE_DATATRANS_PARAM_HOST_DEF,
	.port_remote = MQTT_REMOTE_DATATRANS_PARAM_PORT_DEF,
};

static stt_mqttExServerCfgParam dtHaExMqttParamInfo = {

	.hostConnServer.host_domain = MQTT_REMOTE_DATATRANS_PARAM_HOST_DEF,
	.hostConnServer.port_remote = MQTT_REMOTE_DATATRANS_PARAM_PORT_DEF,
	.usrName = MQTT_REMOTE_DATATRANS_USERNAME_DEF,
	.usrPsd = MQTT_REMOTE_DATATRANS_PASSWORD_DEF,
};

static uint8_t epidCyLocationIst = 0;

uint8_t numCheckMethod_customLanbon(uint8_t *dats, uint8_t len){

	unsigned char loop		= 0;
	unsigned char val_Check = 0;

//	for(loop = 0; loop < len; loop ++){
//	
//		val_Check ^= dats[loop];
//	}

	for(loop = 0; loop < len; loop ++){
	
		val_Check += dats[loop];
	}
	
	val_Check  = ~val_Check;
	val_Check ^= 0xa7;

	return val_Check;
}

void dispApplication_epidCyLocation_set(uint8_t cyIst, bool nvsRecord_IF){

	epidCyLocationIst = cyIst;

	if(nvsRecord_IF){

		devSystemInfoLocalRecord_save(saveObj_epidCyLocation, &cyIst);
	}
}

uint8_t dispApplication_epidCyLocation_get(void){

	return epidCyLocationIst;
}

uint8_t systemDevice_startUpTime_get(void){

	return devRunningTimeFromPowerUp_couter;
}

uint16_t systemDevice_currentVersionGet(void){

	char verStr_temp[8] = {0};
	int verVal_a = 0,
		verVal_b = 0,
		verVal_c = 0;

	sscanf(L8_DEVICE_VERSION_REF_DISCRIPTION, "%*[^v]v%[^\[]", verStr_temp);
	sscanf(verStr_temp, "%d.%d.%d", &verVal_a, &verVal_b, &verVal_c);

	return verVal_a * 10000 + verVal_b * 100 + verVal_c;
}

void gui_bussinessHome_btnText_dataReales(uint8_t picIst, uint8_t *picDataBuf, uint8_t *dataLoad, uint16_t dataLoad_len, uint8_t dataBagIst, bool lastFrame_If){

	if(picDataBuf == NULL)return;

	printf("btnText_dataReales.\n");

	const uint16_t dataIstBase = MQTT_REMOTE_DATATRANS_PIC_DATABAG_LENGTH / 4 * 3 * (uint16_t)dataBagIst;
	const uint16_t dataOpUnit = 4;
	uint16_t opLoop = 0;
	struct _pixelRGB_discr{

		uint16_t pData_B:5;
		uint16_t pData_G:6;		
		uint16_t pData_R:5;
	}pixelRGB565_opUnitTemp = {0};

	if(dataIstBase > GUI_BUSSINESS_HOME_BTNTEXT_PIC_PIXEL_SIZE * 3)return;

	for(opLoop = 0; opLoop < (dataLoad_len / dataOpUnit); opLoop ++){

		picDataBuf[dataIstBase + (3 * opLoop) + 2] = dataLoad[(dataOpUnit * opLoop) + 3];
		memset(&pixelRGB565_opUnitTemp, 0xff, sizeof(uint16_t));
		pixelRGB565_opUnitTemp.pData_R = dataLoad[(dataOpUnit * opLoop) + 2] / 8; //R:5bit
		pixelRGB565_opUnitTemp.pData_G = dataLoad[(dataOpUnit * opLoop) + 1] / 4; //G:6bit
		pixelRGB565_opUnitTemp.pData_B = dataLoad[(dataOpUnit * opLoop) + 0] / 8; //B:5bit
		memcpy(&picDataBuf[dataIstBase + (3 * opLoop)], &pixelRGB565_opUnitTemp, sizeof(uint16_t));
	}

	if(lastFrame_If){

		printf("last frame trig picSave.\n");
		devGuiBussinessHome_btnTextPic_save(picIst, picDataBuf);
	}
}

void gui_bussinessHome_bGroundPic_dataReales(uint8_t *picDataBuf, uint8_t *dataLoad, uint16_t dataLoad_len, uint16_t dataBagIst, bool lastFrame_If){

	if(picDataBuf == NULL)return;

	printf("bGroundPic_dataReales.\n");

	const uint32_t dataIstBase = MQTT_REMOTE_DATATRANS_PIC_DATABAG_LENGTH_S / 4 * 3 * (uint32_t)dataBagIst;
	const uint16_t dataOpUnit = 4;
	uint16_t opLoop = 0;
	struct _pixelRGB_discr{

		uint16_t pData_B:5;
		uint16_t pData_G:6;		
		uint16_t pData_R:5;
	}pixelRGB565_opUnitTemp = {0};

	if(dataIstBase > GUI_BUSSINESS_HOME_BGROUND_PIC_PIXEL_SIZE * 3)return;

	for(opLoop = 0; opLoop < (dataLoad_len / dataOpUnit); opLoop ++){

		picDataBuf[dataIstBase + (3 * opLoop) + 2] = dataLoad[(dataOpUnit * opLoop) + 3];
		memset(&pixelRGB565_opUnitTemp, 0xff, sizeof(uint16_t));
		pixelRGB565_opUnitTemp.pData_R = dataLoad[(dataOpUnit * opLoop) + 2] / 8; //R:5bit
		pixelRGB565_opUnitTemp.pData_G = dataLoad[(dataOpUnit * opLoop) + 1] / 4; //G:6bit
		pixelRGB565_opUnitTemp.pData_B = dataLoad[(dataOpUnit * opLoop) + 0] / 8; //B:5bit
		memcpy(&picDataBuf[dataIstBase + (3 * opLoop)], &pixelRGB565_opUnitTemp, sizeof(uint16_t));
	}

	if(lastFrame_If){

		printf("last frame trig picSave.\n");
		devSystemInfoLocalRecord_save(saveObj_devGuiBussinessHome_bGroundPic, picDataBuf);
		bGroundPic_reserveFlg = 1;
	}
	
}

void usrAppHomepageBtnTextDisp_paramSet(stt_dataDisp_guiBussinessHome_btnText *param, bool nvsRecord_IF){

	stt_msgDats_dataManagementHandle sptr_msgQ_dmHandle = {0};

	//提前对比差异
	sptr_msgQ_dmHandle.msgType = dataManagement_msgType_homePageCtrlObjTextChg;
	for(uint8_t loop = 0; loop < GUIBUSSINESS_CTRLOBJ_MAX_NUM; loop ++){ //缓存更新前作比较

		if(memcmp(param->dataBtnTextDisp[loop], dataBtnTextObjDisp_bussinessHome.dataBtnTextDisp[loop], sizeof(uint8_t) * GUI_BUSSINESS_HOME_BTNTEXT_STR_UTF8_SIZE))
			sptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjTextChg.objChg_bitHold |= (1 << loop);
	}

	/*第一步更新数据*/
	memcpy(&dataBtnTextObjDisp_bussinessHome, param, sizeof(stt_dataDisp_guiBussinessHome_btnText));
	if(nvsRecord_IF)devSystemInfoLocalRecord_save(saveObj_devGuiBussinessHome_btnTextDisp, &dataBtnTextObjDisp_bussinessHome);

	/*第二步刷新UI显示*///顺序不能错，否则提前刷新
	xQueueSend(msgQh_dataManagementHandle, &sptr_msgQ_dmHandle, 1 / portTICK_PERIOD_MS);

	extern void devDetailInfoUploadTrig(void);
	devDetailInfoUploadTrig(); //节点信息改变，触发上报
}

void usrAppHomepageBtnTextDisp_paramSet_specified(uint8_t objNum, 
															  uint8_t nameTemp[GUI_BUSSINESS_HOME_BTNTEXT_STR_UTF8_SIZE], 
															  uint8_t nameLen, 
															  uint8_t cyFlg,
															  bool nvsRecord_IF){

	stt_dataDisp_guiBussinessHome_btnText dataTextObjDisp_temp = {0};

	usrAppHomepageBtnTextDisp_paramGet(&dataTextObjDisp_temp);

	switch(cyFlg){

		case countryT_Arabic:
		case countryT_Hebrew:{

			uint8_t dataChg_temp[GUI_BUSSINESS_HOME_BTNTEXT_STR_UTF8_SIZE] = {0};
			uint8_t dataTransIst_temp = 0;

			memset(dataTextObjDisp_temp.dataBtnTextDisp[objNum], 0, GUI_BUSSINESS_HOME_BTNTEXT_STR_UTF8_SIZE);
			memcpy(dataChg_temp, nameTemp, nameLen);
			for(uint8_t loop = 0; loop < (nameLen / 2); loop ++){ //字序倒置<utf8编码2字节长度>

				dataTransIst_temp = nameLen - (2 * (loop + 1));
				memcpy(&(dataTextObjDisp_temp.dataBtnTextDisp[objNum][loop * 2]), &(dataChg_temp[dataTransIst_temp]), 2);
			}

		}break;

		case countryT_EnglishSerail:
		default:{

			memset(dataTextObjDisp_temp.dataBtnTextDisp[objNum], 0, GUI_BUSSINESS_HOME_BTNTEXT_STR_UTF8_SIZE);
			memcpy(dataTextObjDisp_temp.dataBtnTextDisp[objNum], nameTemp, nameLen);

		}break;
	}

	usrAppHomepageBtnTextDisp_paramSet(&dataTextObjDisp_temp, true);
}

void usrAppHomepageBtnTextDisp_defaultLoad(devTypeDef_enum devType, bool nvsRecord_IF){

	const stt_dataDisp_guiBussinessHome_btnText dataBtnTextObjDispDefault_devTypeMulitSw = {

		.countryFlg = countryT_EnglishSerail,
		.dataBtnTextDisp[0] = "Light1",
		.dataBtnTextDisp[1] = "Light2",
		.dataBtnTextDisp[2] = "Light3",
		
	},dataBtnTextObjDispDefault_devTypeScene = {

		.countryFlg = countryT_EnglishSerail,
		.dataBtnTextDisp[0] = "Scene1",
		.dataBtnTextDisp[1] = "Scene2",
		.dataBtnTextDisp[2] = "Scene3",
		
	},dataBtnTextObjDispDefault_devTypeCurtain = {

		.countryFlg = countryT_EnglishSerail,
		.dataBtnTextDisp[0] = "Curtain",
		
	},dataBtnTextObjDispDefault_devTypeDimmer = {

		.countryFlg = countryT_EnglishSerail,
		.dataBtnTextDisp[0] = "Dimmer",
		
	},dataBtnTextObjDispDefault_devTypeSocket = {

		.countryFlg = countryT_EnglishSerail,
		.dataBtnTextDisp[0] = "Socket",
		
	},dataBtnTextObjDispDefault_devTypeInfrared = {

		.countryFlg = countryT_EnglishSerail,
		.dataBtnTextDisp[0] = "Infrared",
		
	},dataBtnTextObjDispDefault_devTypeHeater = {

		.countryFlg = countryT_EnglishSerail,
		.dataBtnTextDisp[0] = "Heater",
		
	},dataBtnTextObjDispDefault_devTypeThermostat = {

		.countryFlg = countryT_EnglishSerail,
		.dataBtnTextDisp[0] = "Thermostat",
		.dataBtnTextDisp[1] = "Light1",
		.dataBtnTextDisp[2] = "Light2",
		
	},dataBtnTextObjDispDefault_devTypeDefault = {

		.countryFlg = countryT_EnglishSerail,
		.dataBtnTextDisp[0] = "Switch1",
		.dataBtnTextDisp[1] = "Switch2",
		.dataBtnTextDisp[2] = "Switch3",
		
	};
	
	switch(devType){

		case devTypeDef_mulitSwOneBit:
		case devTypeDef_mulitSwTwoBit:
		case devTypeDef_mulitSwThreeBit:
		case devTypeDef_moudleSwOneBit:
		case devTypeDef_moudleSwTwoBit:
		case devTypeDef_moudleSwThreeBit:
			
			usrAppHomepageBtnTextDisp_paramSet(&dataBtnTextObjDispDefault_devTypeMulitSw, nvsRecord_IF);
			break;

		case devTypeDef_curtain:
		case devTypeDef_moudleSwCurtain:
			
			usrAppHomepageBtnTextDisp_paramSet(&dataBtnTextObjDispDefault_devTypeCurtain, nvsRecord_IF);
			break;

		case devTypeDef_dimmer:

			usrAppHomepageBtnTextDisp_paramSet(&dataBtnTextObjDispDefault_devTypeDimmer, nvsRecord_IF);
			break;

		case devTypeDef_scenario:

			usrAppHomepageBtnTextDisp_paramSet(&dataBtnTextObjDispDefault_devTypeScene, nvsRecord_IF);
			break;

		case devTypeDef_infrared:

			usrAppHomepageBtnTextDisp_paramSet(&dataBtnTextObjDispDefault_devTypeInfrared, nvsRecord_IF);
			break;
		
		case devTypeDef_socket:

			usrAppHomepageBtnTextDisp_paramSet(&dataBtnTextObjDispDefault_devTypeSocket, nvsRecord_IF);
			break;

		case devTypeDef_heater:

			usrAppHomepageBtnTextDisp_paramSet(&dataBtnTextObjDispDefault_devTypeHeater, nvsRecord_IF);
			break;

		case devTypeDef_thermostat:
		case devTypeDef_thermostatExtension:	

			usrAppHomepageBtnTextDisp_paramSet(&dataBtnTextObjDispDefault_devTypeThermostat, nvsRecord_IF);
			break;

		default:

			usrAppHomepageBtnTextDisp_paramSet(&dataBtnTextObjDispDefault_devTypeDefault, nvsRecord_IF);
			break;
	}
}

void usrAppHomepageBtnTextDisp_paramGet(stt_dataDisp_guiBussinessHome_btnText *param){

	memcpy(param, &dataBtnTextObjDisp_bussinessHome, sizeof(stt_dataDisp_guiBussinessHome_btnText));
}

uint8_t countryFlgGetByAbbre(char countryAbbre[DATAMANAGE_LANGUAGE_ABBRE_MAXLEN]){

	const char countryAbbre_tab[DATAMANAGE_LANGUAGE_TAB_MAXNUM][DATAMANAGE_LANGUAGE_ABBRE_MAXLEN] = {

		"en", // 00 英语
		"ar", // 01 阿拉伯语
		"de", // 02 德语
		"es", // 03 西班牙
		"fr", // 04 法文
		"it", // 05 意大利
		"ja", // 06 日语
		"pl", // 07 波兰文
		"pt", // 08 葡萄牙语
		"th", // 09 泰文
		"tr", // 10 土耳其
		"iw", // 11 希伯来语
		"ru", // 12 俄文
		"vi", // 13 越南语
	};
	uint8_t res_flg = countryT_EnglishSerail;

	for(uint8_t loop = 0; loop < DATAMANAGE_LANGUAGE_TAB_MAXNUM; loop ++){

		if(!strcmp(countryAbbre_tab[loop], countryAbbre)){

			res_flg = loop;
			break;
		}
	}

	switch(res_flg){

		case 1:{

			res_flg = countryT_Arabic;

		}break;
	
		case 11:{

			res_flg = countryT_Hebrew;

		}break;

		default:{

			res_flg = countryT_EnglishSerail;
		
		}break;
	}

	return res_flg;
}

void usrAppHomepageBtnIconNumDisp_paramSet(uint8_t param[GUIBUSSINESS_CTRLOBJ_MAX_NUM], bool nvsRecord_IF){

	stt_msgDats_dataManagementHandle sptr_msgQ_dmHandle = {0};

	//提前对比差异
	sptr_msgQ_dmHandle.msgType = dataManagement_msgType_homePageCtrlObjIconChg;
	for(uint8_t loop = 0; loop < GUIBUSSINESS_CTRLOBJ_MAX_NUM; loop ++){ //缓存更新前作比较

		if(param[loop] != dataBtnIconNumObjDisp_bussinessHome[loop])
			sptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjIconChg.objChg_bitHold |= (1 << loop);
	}

	/*第一步更新数据*/
	memcpy(dataBtnIconNumObjDisp_bussinessHome, param, sizeof(uint8_t) * GUIBUSSINESS_CTRLOBJ_MAX_NUM);
	if(nvsRecord_IF)devSystemInfoLocalRecord_save(saveObj_devGuiBussinessHome_btnIconDisp, dataBtnIconNumObjDisp_bussinessHome);

	/*第二部刷新UI显示*///顺序不能错，否则提前刷新
	xQueueSend(msgQh_dataManagementHandle, &sptr_msgQ_dmHandle, 1 / portTICK_PERIOD_MS);

	extern void devDetailInfoUploadTrig(void);
	devDetailInfoUploadTrig(); //节点信息改变，触发上报
}

void usrAppHomepageBtnIconNumDisp_defaultLoad(devTypeDef_enum devType, bool nvsRecord_IF){

	const uint8_t dataBtnIconNumObjDispDefault_devTypeMulitSw[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {1, 1, 1},
				  dataBtnIconNumObjDispDefault_devTypeScene[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {55, 55, 55},
				  dataBtnIconNumObjDispDefault_defSource[GUIBUSSINESS_CTRLOBJ_MAX_NUM]  = {28, 1, 1};

	switch(devType){

		case devTypeDef_mulitSwOneBit:
		case devTypeDef_mulitSwTwoBit:
		case devTypeDef_mulitSwThreeBit:
		case devTypeDef_moudleSwOneBit:
		case devTypeDef_moudleSwTwoBit:
		case devTypeDef_moudleSwThreeBit:{

			usrAppHomepageBtnIconNumDisp_paramSet(dataBtnIconNumObjDispDefault_devTypeMulitSw, nvsRecord_IF);

		}break;

		case devTypeDef_scenario:{

			usrAppHomepageBtnIconNumDisp_paramSet(dataBtnIconNumObjDispDefault_devTypeScene, nvsRecord_IF);

		}break;

		case devTypeDef_infrared:
		case devTypeDef_socket:
		case devTypeDef_heater:
		case devTypeDef_thermostat:
		case devTypeDef_thermostatExtension:{

			usrAppHomepageBtnIconNumDisp_paramSet(dataBtnIconNumObjDispDefault_defSource, nvsRecord_IF);

		}break;

		default:break;
	}
}

void usrAppHomepageBtnIconNumDisp_paramGet(uint8_t param[GUIBUSSINESS_CTRLOBJ_MAX_NUM]){

	memcpy(param, dataBtnIconNumObjDisp_bussinessHome, sizeof(uint8_t) * GUIBUSSINESS_CTRLOBJ_MAX_NUM);
}

lv_img_dsc_t *usrAppHomepageBtnIconDisp_dataGet(uint8_t iconNum){

	const lv_img_dsc_t *res = NULL;

	switch(iconNum){
		
		case 1:		res = &homepageCtrlObjIcon_21; 	break;
		case 2:		res = &homepageCtrlObjIcon_20; 	break;
		case 3:		res = &homepageCtrlObjIcon_18; 	break;
		case 4:		res = &homepageCtrlObjIcon_17; 	break;
		case 5:		res = &homepageCtrlObjIcon_19; 	break;
		case 6:		res = &homepageCtrlObjIcon_16; 	break;
		case 7:		res = &homepageCtrlObjIcon_15; 	break;
		case 8:		res = &homepageCtrlObjIcon_14; 	break;
		case 9:		res = &homepageCtrlObjIcon_13; 	break;		
		case 10:	res = &homepageCtrlObjIcon2_12; break;
		case 11:	res = &homepageCtrlObjIcon2_11; break;
		case 12:	res = &homepageCtrlObjIcon2_10; break;
		case 13:	res = &homepageCtrlObjIcon2_9; 	break;
		case 14:	res = &homepageCtrlObjIcon3_8;	break;
		case 15:	res = &homepageCtrlObjIcon3_7;	break;
		case 16:	res = &homepageCtrlObjIcon3_6;	break;
		case 17:	res = &homepageCtrlObjIcon3_5;	break;
		case 18:	res = &homepageCtrlObjIcon3_4;	break;
		case 19:	res = &homepageCtrlObjIcon_26;	break;
		case 20:	res = &homepageCtrlObjIcon_28;	break;
		case 21:	res = &homepageCtrlObjIcon_27;	break;
		case 22:	res = &homepageCtrlObjIcon_23;	break;
		case 23:	res = &homepageCtrlObjIcon_24;	break;
		case 24:	res = &homepageCtrlObjIcon_25;	break;
		case 25:	res = &homepageCtrlObjIcon_31;	break;
		case 26:	res = &homepageCtrlObjIcon_29;	break;
		case 27:	res = &homepageCtrlObjIcon_34;	break;
		case 28:	res = &homepageCtrlObjIcon_29;	break;
		case 29:	res = &homepageCtrlObjIcon_32;	break;
		case 30:	res = &homepageCtrlObjIcon_30;	break;
		case 31:	res = &homepageCtrlObjIcon_31;	break;
		case 32:	res = &homepageCtrlObjIcon_33;	break;
		case 33:	res = &homepageCtrlObjIcon2_7; 	break;
		case 34:	res = &homepageCtrlObjIcon_10; 	break;
		case 35:	res = &homepageCtrlObjIcon2_2; 	break;
		case 36:	res = &homepageCtrlObjIcon3_2; 	break;
		case 37:	res = &homepageCtrlObjIcon_9; 	break;
		case 38:	res = &homepageCtrlObjIcon_12; 	break;
		case 39:	res = &homepageCtrlObjIcon2_1; 	break;
		case 40:	res = &homepageCtrlObjIcon3_3; 	break;
		case 41:	res = &homepageCtrlObjIcon3_1; 	break;
		case 42:	res = &homepageCtrlObjIcon2_3; 	break;
		case 43:	res = &homepageCtrlObjIcon_22;	break;
		case 44:	res = &homepageCtrlObjIcon2_5; 	break;
		case 45:	res = &homepageCtrlObjIcon2_8;	break;
		case 46:	res = &homepageCtrlObjIcon_35;	break;
		case 47:	res = &homepageCtrlObjIcon2_4; 	break;
		case 48:	res = &homepageCtrlObjIcon2_6; 	break;
		case 49:	res = &homepageCtrlObjIcon_11; 	break;

		case 50:	res = &homepageCtrlObjIcon_5; 	break;
		case 51:	res = &homepageCtrlObjIcon_3; 	break;
		case 52:	res = &homepageCtrlObjIcon_1; 	break;
		case 53:	res = &homepageCtrlObjIcon_2; 	break;
		case 54:	res = &homepageCtrlObjIcon_4; 	break;
		case 55:	res = &homepageCtrlObjIcon_7; 	break;
		case 56:	res = &homepageCtrlObjIcon_8;	break;	
		
		default:{

			switch(currentDev_typeGet()){

				case devTypeDef_mulitSwOneBit:
				case devTypeDef_mulitSwTwoBit:
				case devTypeDef_mulitSwThreeBit:{
				
					res = &homepageCtrlObjIcon_21;
				
				}break;
				
				case devTypeDef_scenario:{
				
					res = &homepageCtrlObjIcon_7;
				
				}break;
				
				default:{

					res = &homepageCtrlObjIcon_21;

				}break;
			}
		}break;
	}

	return res;
}

void usrApp_devIptdrv_paramRecalibration_set(bool reCalibra_if){

	(reCalibra_if)?
		(devIptdrvParam_recalibration = 'Y'):
		(devIptdrvParam_recalibration = 'N');

	devSystemInfoLocalRecord_save(saveObj_devDriver_iptRecalibration_set, &devIptdrvParam_recalibration);
}

enum_touchCaliRsv usrApp_devIptdrv_paramRecalibration_get(void){

	enum_touchCaliRsv res = caliStatus_noCfm;

	switch(devIptdrvParam_recalibration){

		case 'Y':{res = caliStatus_Y;}break;

		case 'N':{res = caliStatus_N;}break;

		default:{res = caliStatus_noCfm;}break;
	}
	
	return res;
}

void meshNetwork_connectReserve_IF_set(bool param){

	meshNetworkParamReserve_Flg = param;
}

bool meshNetwork_connectReserve_IF_get(void){

	return meshNetworkParamReserve_Flg;
}

void flgSet_gotRouterOrMeshConnect(bool valSet){

	devRouterOrMeshConnect_flg = valSet;
}

bool flgGet_gotRouterOrMeshConnect(void){

	return devRouterOrMeshConnect_flg;
}

void devRouterRecordBssid_Set(uint8_t bssid[6], bool nvsRecord_IF){

	memcpy(routerRecord_bssid, bssid, 6);
	if(nvsRecord_IF)devSystemInfoLocalRecord_save(saveObj_routerBssidRcd, routerRecord_bssid);
}

void devRouterConnectBssid_Set(uint8_t bssid[6], bool nvsRecord_IF){

	memcpy(routerConnect_bssid, bssid, 6);
	if(nvsRecord_IF)devSystemInfoLocalRecord_save(saveObj_routerBssid, routerConnect_bssid);
}

void devRouterRecordBssid_Get(uint8_t bssid[6]){

	memcpy(bssid, routerRecord_bssid, 6);
}

void devRouterConnectBssid_Get(uint8_t bssid[6]){

	memcpy(bssid, routerConnect_bssid, 6);
}

void devMeshSignalVal_Reales(void){

	mesh_assoc_t mesh_assoc = {0x0};

	if(mwifi_is_connected()){

		esp_wifi_vnd_mesh_get(&mesh_assoc);

		devSignalStrength2ParentVal = mesh_assoc.rssi;
	}
	else
	{
		devSignalStrength2ParentVal = -127;
	}
}

int8_t devMeshSignalFromParentVal_Get(void){

	return devSignalStrength2ParentVal;
}

void devMeshNodeNum_Reales(void){

	devMesh_currentNodeNum = (uint16_t)esp_mesh_get_total_node_num();
}

uint16_t devMeshNodeNum_Get(void){

	return devMesh_currentNodeNum;
}

static void devGuiBussinessHome_btnTextPic_save(uint8_t picIst, uint8_t *picData){

	switch(picIst){

		case 0:{

			devSystemInfoLocalRecord_save(saveObj_devGuiBussinessHome_btnTextPic_A, picData);

		}break;

		case 1:{

			devSystemInfoLocalRecord_save(saveObj_devGuiBussinessHome_btnTextPic_B, picData);

		}break;

		case 2:{

			devSystemInfoLocalRecord_save(saveObj_devGuiBussinessHome_btnTextPic_C, picData);

		}break;

		default:break;
	}
}

void devMutualCtrlGroupInfo_Clr(void){

	memset(devMutualCtrl_group, 0, sizeof(stt_devMutualGroupParam) * DEVICE_MUTUAL_CTRL_GROUP_NUM);
	devSystemInfoLocalRecord_save(saveObj_mutualCtrlInfo, devMutualCtrl_group);
}

void devMutualCtrlGroupInfo_Set(stt_devMutualGroupParam *mutualGroupParam, uint8_t opreatBit, bool nvsRecord_IF){

	if(opreatBit & (1 << 0))memcpy(&devMutualCtrl_group[0], mutualGroupParam, sizeof(stt_devMutualGroupParam));
	if(opreatBit & (1 << 1))memcpy(&devMutualCtrl_group[1], mutualGroupParam, sizeof(stt_devMutualGroupParam));
	if(opreatBit & (1 << 2))memcpy(&devMutualCtrl_group[2], mutualGroupParam, sizeof(stt_devMutualGroupParam));
	if(nvsRecord_IF)devSystemInfoLocalRecord_save(saveObj_mutualCtrlInfo, devMutualCtrl_group);
}

void devMutualCtrlGroupInfo_Get(stt_devMutualGroupParam mutualGroupParam[DEVICE_MUTUAL_CTRL_GROUP_NUM]){

	memcpy(mutualGroupParam, devMutualCtrl_group, sizeof(stt_devMutualGroupParam) * DEVICE_MUTUAL_CTRL_GROUP_NUM);
}

void devMutualCtrlGroupInfo_groupInsertGet(uint8_t groupInsert[DEVICE_MUTUAL_CTRL_GROUP_NUM]){

	for(uint8_t loop = 0; loop < DEVICE_MUTUAL_CTRL_GROUP_NUM; loop ++){

		groupInsert[loop] = devMutualCtrl_group[loop].mutualCtrlGroup_insert;
	}
}

bool devMutualCtrlGroupInfo_unitCheckByInsert(stt_devMutualGroupParam *mutualGroupParamUnit, uint8_t *groupNum, uint8_t paramInsert){

	bool infoGet_res = false;

	if((paramInsert == DEVICE_MUTUALGROUP_INVALID_INSERT_A) ||
	   (paramInsert == DEVICE_MUTUALGROUP_INVALID_INSERT_B)){

		infoGet_res = false;
	}
	else
	{
		for(uint8_t loop = 0; loop < DEVICE_MUTUAL_CTRL_GROUP_NUM; loop ++){
		
			if(devMutualCtrl_group[loop].mutualCtrlGroup_insert == paramInsert){
		
				memcpy(mutualGroupParamUnit, &devMutualCtrl_group[loop], sizeof(stt_devMutualGroupParam));
				*groupNum = loop;
				infoGet_res = true;
			}
		}
	}

	return infoGet_res;
}

void devSystemOpration_linkageConfig_paramSet(stt_paramLinkageConfig *param, bool nvsRecord_IF){

	memcpy(&devSysParam_linkageConfig, param, sizeof(stt_paramLinkageConfig));

	if(nvsRecord_IF)
		devSystemInfoLocalRecord_save(saveObj_devDriver_linkageConfigParam_set, &devSysParam_linkageConfig);
}

void devSystemOpration_linkageConfig_paramGet(stt_paramLinkageConfig *param){

	memcpy(param, &devSysParam_linkageConfig, sizeof(stt_paramLinkageConfig));
}

void mqttRemoteConnectCfg_paramSet(stt_mqttCfgParam *param, bool nvsRecord_IF){

	memcpy(&dtMqttParamInfo, param, sizeof(stt_mqttCfgParam));
	if(nvsRecord_IF)
		devSystemInfoLocalRecord_save(saveObj_dtMqttCfgParam, &dtMqttParamInfo);
}

void mqttRemoteConnectCfg_paramGet(stt_mqttCfgParam *param){

	memcpy(param, &dtMqttParamInfo, sizeof(stt_mqttCfgParam));
}

void mqttHaMqttServer_paramSet(stt_mqttExServerCfgParam *param, bool nvsRecord_IF){

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED) || (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_SOCKET) || (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_MOUDLE)
	
	stt_devStatusRecord devStatusRecordFlg_temp = {0};

	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);
	devStatusRecordFlg_temp.homeassitant_En = 1;
	devStatusRecordIF_paramSet(&devStatusRecordFlg_temp, nvsRecord_IF);
	vTaskDelay(100 / portTICK_PERIOD_MS);
#endif

	memcpy(&dtHaExMqttParamInfo, param, sizeof(stt_mqttExServerCfgParam));
	if(nvsRecord_IF)
		devSystemInfoLocalRecord_save(saveObj_dtHaMqttCfgParam, &dtHaExMqttParamInfo);
}

void mqttHaMqttServer_paramGet(stt_mqttExServerCfgParam *param){

	memcpy(param, &dtHaExMqttParamInfo, sizeof(stt_mqttExServerCfgParam));
}

void currentRouterCfgInfo_paramSet(stt_routerCfgInfo *param, bool nvsRecord_IF){

	memcpy(&routerConfiguration_info, param, sizeof(stt_routerCfgInfo));

	if(nvsRecord_IF)
		devSystemInfoLocalRecord_save(saveObj_routerConfigInfo, &routerConfiguration_info);
}

void currentRouterCfgInfo_paramGet(stt_routerCfgInfo *param){

	memcpy(param, &routerConfiguration_info, sizeof(stt_routerCfgInfo));
}

uint16_t currentDevRunningFlg_paramGet(void){

	return devCurrentRunningFlg;
}

void currentDevRunningFlg_paramSet(uint16_t valFlg, bool nvsRecord_IF){

	devCurrentRunningFlg = valFlg;
	if(nvsRecord_IF)devSystemInfoLocalRecord_save(saveObj_devRunning_flg, &devCurrentRunningFlg);
}

void usrMeshApplication_rootFirstConNoticeTrig(void){

	const uint8_t noticePeriod = 3;

	if(MESH_ROOT == esp_mesh_get_layer()){

		meshAppOpt_rootFirstConNotice_count = noticePeriod;
	}
}

bool usrMeshApplication_rootFirstConNoticeActionRserveGet(void){

	bool res = false;

	if(meshAppOpt_rootFirstConNotice_count){

		meshAppOpt_rootFirstConNotice_count --;
		res = true;
	}

	return res;
}

stt_blufiConfigDevInfo_resp *devBlufiConfig_respInfoGet(void){

	uint8_t loop = 0;
	uint8_t strLogInfo_temp[64] = {0};
	uint8_t dataInfo_temp[16] = {0};
	stt_blufiConfigDevInfo_resp *devInfo = os_zalloc(sizeof(stt_blufiConfigDevInfo_resp));
	stt_devDataPonitTypedef devDataPoint = {0};

	currentDev_dataPointGet(&devDataPoint);

	devInfo->devTypedef = currentDev_typeGet();
	memcpy(&(devInfo->devStatus_valCurrent), &devDataPoint, sizeof(stt_devDataPonitTypedef)); //开关状态
	devInfo->devVersion = L8_DEVICE_VERSION; //版本
	devDriverBussiness_temperatureMeasure_getByHex(&(devInfo->devFuncInfo_temprature)); //温度
	devDriverBussiness_elecMeasure_valPowerGetByHex(&(devInfo->devFuncInfo_elecPower)); //功率

	memcpy(dataInfo_temp, devInfo, sizeof(stt_blufiConfigDevInfo_resp));
	for(loop = 0; loop < sizeof(stt_blufiConfigDevInfo_resp); loop ++){

		sprintf((char *)&strLogInfo_temp[loop * 3], "%02X ", dataInfo_temp[loop]);
	}
	printf("blufiCfg resp dats:%s.\n", strLogInfo_temp);

//	printf("blufi config dataResp length:%d.\n", sizeof(stt_blufiConfigDevInfo_resp));

	return devInfo;
}

bool usrAppMethod_mwifiMacAddrRemoveFromList(uint8_t *macAddrList, uint8_t macAddrList_num, uint8_t macAddrRemove[MWIFI_ADDR_LEN]){

	uint8_t loop = 0;
	bool res_macAddrSearch = false;

	for(loop = 0; loop < macAddrList_num; loop ++){

		if(!res_macAddrSearch){

			if(!memcmp(&macAddrList[loop * MWIFI_ADDR_LEN], macAddrRemove, MWIFI_ADDR_LEN)){

				memset(&macAddrList[loop * MWIFI_ADDR_LEN], 0, MWIFI_ADDR_LEN);
				res_macAddrSearch = true;
			}
		}
		else
		{
			memcpy(&macAddrList[(loop - 1) * MWIFI_ADDR_LEN], &macAddrList[loop * MWIFI_ADDR_LEN], MWIFI_ADDR_LEN);
		}
	}

	if(res_macAddrSearch)memset(&macAddrList[(macAddrList_num - 1) * MWIFI_ADDR_LEN], 0, MWIFI_ADDR_LEN);

	return res_macAddrSearch;
}

stt_mutualCtrlInfoResp *L8devMutualCtrlInfo_Get(stt_nodeDev_hbDataManage *pHead, uint8_t mutualCtrlGroupIst){

	stt_mutualCtrlInfoResp *devMutualInfo = (stt_mutualCtrlInfoResp *)os_zalloc(sizeof(stt_mutualCtrlInfoResp));
	uint8_t devMutualGroupIst_self[DEVICE_MUTUAL_CTRL_GROUP_NUM] = {0};
	uint8_t devSelfMac[MWIFI_ADDR_LEN] = {0};
	stt_nodeDev_hbDataManage *pAbove = pHead;
	uint8_t loopCount = 0;

	if(devRunningTimeFromPowerUp_couter <= L8_DEV_LISTMANAGE_REALES_CONFIRM){
	
		devMutualInfo->devNum = DEVLIST_MANAGE_LISTNUM_MASK_NULL; //数量掩码，链表数据尚未就绪
	
		return devMutualInfo;
	}

	listNodeDevStatus_opreatingFlg = true;

	esp_wifi_get_mac(ESP_IF_WIFI_STA, devSelfMac);
	devMutualCtrlGroupInfo_groupInsertGet(devMutualGroupIst_self); //自身互控获取处理
	for(uint8_t loopA = 0; loopA < DEVICE_MUTUAL_CTRL_GROUP_NUM; loopA ++){
	
		if(devMutualGroupIst_self[loopA] == mutualCtrlGroupIst){

			memcpy(devMutualInfo->unitDevMutualInfo[loopCount].devUnitMAC, 
				   devSelfMac,
				   MWIFI_ADDR_LEN * sizeof(uint8_t));
			devMutualInfo->unitDevMutualInfo[loopCount].devMutualBitIst = (1 << loopA);
			
			loopCount ++;

//			printf("self mCtrl found.\n");
		}
	}	

	while(pAbove->next != NULL){

		for(uint8_t loopA = 0; loopA < DEVICE_MUTUAL_CTRL_GROUP_NUM; loopA ++){

			if(pAbove->next->dataManage.nodeDev_mautualInfo[loopA] == mutualCtrlGroupIst){

				memcpy(devMutualInfo->unitDevMutualInfo[loopCount].devUnitMAC, 
					   pAbove->next->dataManage.nodeDev_Mac,
					   MWIFI_ADDR_LEN * sizeof(uint8_t));
				devMutualInfo->unitDevMutualInfo[loopCount].devMutualBitIst = (1 << loopA);

				loopCount ++;

//				printf("node mCtrl found.\n");
			}

//			if(loopA == 2){

//				printf("node[mac:%02X %02X %02X %02X %02X %02X] mCtrl{%d, %d, %d}.\n", 
//						MAC2STR(pAbove->next->dataManage.nodeDev_Mac),
//						pAbove->next->dataManage.nodeDev_mautualInfo[0],
//						pAbove->next->dataManage.nodeDev_mautualInfo[1],
//						pAbove->next->dataManage.nodeDev_mautualInfo[2]);
//			}
		}		
		
		pAbove = pAbove->next;

		if(loopCount >= MAXNUM_OF_DEVICE_IN_SINGLE_MUTUALGROUP)break;
	}

	devMutualInfo->devNum = loopCount; //数量确定
	devMutualInfo->mGroupIst = mutualCtrlGroupIst;

	listNodeDevStatus_opreatingFlg = false;

	return devMutualInfo; //谨记释放内存
}

uint8_t *L8DevListGet_Type1(stt_nodeDev_hbDataManage *pHead, uint8_t targetDevType){ //下标1为数量，后续为地址列表

	uint8_t *devList = NULL;
	uint8_t  devTargetVersion_temp = deviceTypeVersionJudge(targetDevType);

	if(devRunningTimeFromPowerUp_couter <= L8_DEV_LISTMANAGE_REALES_CONFIRM){

		devList = (uint8_t *)os_zalloc(sizeof(uint8_t) * MWIFI_ADDR_LEN + 1);
		memset(&devList[1], 0, sizeof(uint8_t) * MWIFI_ADDR_LEN);
		devList[0] = 1; 

		return devList;
	}
	else
	{
		uint8_t devList_num = (uint8_t)esp_mesh_get_total_node_num();
		uint8_t loopCount = 0;
		stt_nodeDev_hbDataManage *pAbove = pHead;
		uint8_t devSelfMac[MWIFI_ADDR_LEN] = {0};

		esp_wifi_get_mac(ESP_IF_WIFI_STA, devSelfMac);

		while(listNodeDevStatus_opreatingFlg)vTaskDelay(1 / portTICK_PERIOD_MS);		
		listNodeDevStatus_opreatingFlg = true;

		devList = (uint8_t *)os_zalloc(sizeof(uint8_t) * MWIFI_ADDR_LEN * (devList_num) + 1);

		if(devTargetVersion_temp == deviceTypeVersionJudge(currentDev_typeGet())){

			memcpy(&devList[loopCount * MWIFI_ADDR_LEN + 1], devSelfMac, sizeof(uint8_t) * MWIFI_ADDR_LEN);
			loopCount ++;
		}

		//子设备状态信息填装
		while(pAbove->next != NULL){

			if(devTargetVersion_temp == deviceTypeVersionJudge(pAbove->next->dataManage.nodeDev_Type)){

				memcpy(&devList[loopCount * MWIFI_ADDR_LEN + 1], pAbove->next->dataManage.nodeDev_Mac, sizeof(uint8_t) * MWIFI_ADDR_LEN);
				loopCount ++;
			}
		
			pAbove = pAbove->next;

			if(loopCount >= devList_num)break; //防止mesh内部调整时，心跳统计列表数量与mesh统计数量不一致导致内存申请数量不一致
		}

		devList[0] = loopCount;

		listNodeDevStatus_opreatingFlg = false;

		return devList;
	}
}

uint8_t *L8devStatusInfoGet(stt_nodeDev_hbDataManage *pHead){ //仅获取链表单位子节点信息内的 除了 互控信息 以外的其他属性

	uint8_t *devStatusInfo = NULL;

	if(devRunningTimeFromPowerUp_couter <= L8_DEV_LISTMANAGE_REALES_CONFIRM){

		devStatusInfo = (uint8_t *)os_zalloc(sizeof(uint8_t) * 1);
		*devStatusInfo = DEVLIST_MANAGE_LISTNUM_MASK_NULL;

		return devStatusInfo;
	}
	else
	{
		uint8_t devList_num = (uint8_t)esp_mesh_get_total_node_num();
		uint8_t loopCount = 0;
		stt_nodeDev_hbDataManage *pAbove = pHead;
		stt_devStatusInfoResp statusInfo_dataUnitTemp = {0};
		uint8_t devSelfMac[MWIFI_ADDR_LEN] = {0};

		while(listNodeDevStatus_opreatingFlg)vTaskDelay(1 / portTICK_PERIOD_MS);		
		listNodeDevStatus_opreatingFlg = true;

		devStatusInfo = (uint8_t *)os_zalloc((sizeof(uint8_t) * 1) + (sizeof(stt_devStatusInfoResp) * (devList_num + 1))); //设备数量描述占1 Byte，本身设备状态信息占对应结构体 size Byte
		
		esp_wifi_get_mac(ESP_IF_WIFI_STA, devSelfMac);

		//本机状态信息填装
		memcpy(statusInfo_dataUnitTemp.nodeDev_Mac, 
			   devSelfMac,
			   sizeof(uint8_t) * MWIFI_ADDR_LEN);
		currentDev_dataPointGet((stt_devDataPonitTypedef *)&(statusInfo_dataUnitTemp.nodeDev_Status));
		statusInfo_dataUnitTemp.nodeDev_Type = (uint8_t)currentDev_typeGet();
		statusInfo_dataUnitTemp.nodeDev_DevRunningFlg = currentDevRunningFlg_paramGet();
		devDriverBussiness_temperatureMeasure_getByHex(&(statusInfo_dataUnitTemp.nodeDev_dataTemprature));
		devDriverBussiness_elecMeasure_valPowerGetByHex(&(statusInfo_dataUnitTemp.nodeDev_dataPower));
		currentDev_extParamGet(statusInfo_dataUnitTemp.nodeDev_extFunParam); //扩展数据填装
		
		memcpy(&devStatusInfo[sizeof(stt_devStatusInfoResp) * loopCount + 1], //下标0为设备数量，所以从下标为 loopCount+1
			   &statusInfo_dataUnitTemp, 
			   sizeof(stt_devStatusInfoResp));

		loopCount ++;

		printf("mqtt statusQuery rootDev type:%02X.\n", statusInfo_dataUnitTemp.nodeDev_Type);

		//子设备状态信息填装
		while(pAbove->next != NULL){

			//单位缓存清空
			memset(&statusInfo_dataUnitTemp, 0, sizeof(stt_devStatusInfoResp));

			//单位缓存填装
			memcpy(statusInfo_dataUnitTemp.nodeDev_Mac, 
				   pAbove->next->dataManage.nodeDev_Mac,
				   sizeof(uint8_t) * MWIFI_ADDR_LEN);
			statusInfo_dataUnitTemp.nodeDev_Type = pAbove->next->dataManage.nodeDev_Type;
			statusInfo_dataUnitTemp.nodeDev_Status = pAbove->next->dataManage.nodeDev_Status;
			memcpy(&(statusInfo_dataUnitTemp.nodeDev_dataTemprature), 
				   &(pAbove->next->dataManage.nodeDev_dataTemprature),
				   sizeof(stt_devTempParam2Hex));
			memcpy(&(statusInfo_dataUnitTemp.nodeDev_dataPower), 
				   &(pAbove->next->dataManage.nodeDev_dataPower),
				   sizeof(stt_devPowerParam2Hex));
			memcpy(statusInfo_dataUnitTemp.nodeDev_extFunParam, 
				   pAbove->next->dataManage.nodeDev_extFunParam,
				   sizeof(uint8_t) * DEVPARAMEXT_DT_LEN);
			
			//单位缓存填装进总数据队列缓存
			memcpy(&devStatusInfo[sizeof(stt_devStatusInfoResp) * loopCount + 1], //下标0为设备数量，所以从下标为 loopCount+1
				   &statusInfo_dataUnitTemp, 
				   sizeof(stt_devStatusInfoResp));

			pAbove = pAbove->next;
			loopCount ++;

			if(loopCount >= devList_num)break; //防止mesh内部调整时，心跳统计列表数量与mesh统计数量不一致导致内存申请数量不一致
		}

		devStatusInfo[0] = loopCount;

		listNodeDevStatus_opreatingFlg = false;

		return devStatusInfo; //谨记释放内存
	}
}

uint8_t *L8devElecsumInfoGet(stt_nodeDev_hbDataManage *pHead){

	uint8_t *devElecsumInfo = NULL;

	if(devRunningTimeFromPowerUp_couter <= L8_DEV_LISTMANAGE_REALES_CONFIRM){

		devElecsumInfo= (uint8_t *)os_zalloc(sizeof(uint8_t) * 1);
		*devElecsumInfo = DEVLIST_MANAGE_LISTNUM_MASK_NULL;

		return devElecsumInfo;

	}else{

		const uint8_t dataPackHead_length = sizeof(uint8_t) * 1 + 
											sizeof(stt_timeZone) * 1 + 
											sizeof(uint8_t) * MWIFI_ADDR_LEN; //设备数量描述占1 Byte，时区占2bytes，BSSID/MESHID占6字节，本身设备状态信息占对应结构体 size Byte
		uint8_t devList_num = (uint8_t)esp_mesh_get_total_node_num();
		uint8_t loopCount = 0;
		uint8_t headInfo_loadIst = 0,
				headInfo_loadLenTemp = 0;
		stt_timeZone devTimeZone_paramTemp = {0};
		stt_nodeDev_hbDataManage *pAbove = pHead;
		stt_devUnitElecsumReport elecsumInfo_dataUnitTemp = {0};
		uint8_t devSelfMac[MWIFI_ADDR_LEN] = {0};
		uint8_t devRouterBssid[6] = {0};

		while(listNodeDevStatus_opreatingFlg)vTaskDelay(1 / portTICK_PERIOD_MS);		
		listNodeDevStatus_opreatingFlg = true;

		devElecsumInfo = (uint8_t *)os_zalloc((sizeof(uint8_t) * dataPackHead_length) + (sizeof(stt_devUnitElecsumReport) * (devList_num + 1))); //数据包头 + 所有数量节点的属性数据

		esp_wifi_get_mac(ESP_IF_WIFI_STA, devSelfMac);

		//本机状态信息填装
		memcpy(elecsumInfo_dataUnitTemp.nodeDev_Mac, 
			   devSelfMac,
			   sizeof(uint8_t) * MWIFI_ADDR_LEN);
		devDriverBussiness_elecMeasure_valElecsumGetByHex(&(elecsumInfo_dataUnitTemp.nodeDev_dataElecsum));
		
		memcpy(&devElecsumInfo[sizeof(stt_devUnitElecsumReport) * loopCount + dataPackHead_length], //数据对应下标偏移量，即数据包头长度dataPackHead_length
			   &elecsumInfo_dataUnitTemp, 
			   sizeof(stt_devUnitElecsumReport));

		loopCount ++;

		//子设备状态信息填装
		while(pAbove->next != NULL){

			//单位缓存清空
			memset(&elecsumInfo_dataUnitTemp, 0, sizeof(stt_devUnitElecsumReport));

			//单位缓存填装
			memcpy(elecsumInfo_dataUnitTemp.nodeDev_Mac, 
				   pAbove->next->dataManage.nodeDev_Mac,
				   sizeof(uint8_t) * MWIFI_ADDR_LEN);
			memcpy(&(elecsumInfo_dataUnitTemp.nodeDev_dataElecsum),
				   &(pAbove->next->dataManage.nodeDev_dataElecsum),
				   sizeof(stt_devUnitElecsumReport));
			
			//单位缓存填装进总数据队列缓存
			memcpy(&devElecsumInfo[sizeof(stt_devUnitElecsumReport) * loopCount + dataPackHead_length], //数据对应下标偏移量，即数据包头长度dataPackHead_length
				   &elecsumInfo_dataUnitTemp, 
				   sizeof(stt_devUnitElecsumReport));

			pAbove = pAbove->next;
			loopCount ++;

			if(loopCount >= devList_num)break; //防止mesh内部调整时，心跳统计列表数量与mesh统计数量不一致导致内存申请数量不一致
		}

		deviceParamGet_timeZone(&devTimeZone_paramTemp);
		devRouterConnectBssid_Get(devRouterBssid);

		headInfo_loadLenTemp = sizeof(uint8_t) * 1;
		devElecsumInfo[headInfo_loadIst] = loopCount; //设备数量

		headInfo_loadIst += headInfo_loadLenTemp;	
		
		headInfo_loadLenTemp = sizeof(stt_timeZone) * 1;
		memcpy(&devElecsumInfo[headInfo_loadIst], &devTimeZone_paramTemp, headInfo_loadLenTemp); //设备时区

		headInfo_loadIst += headInfo_loadLenTemp;
		
		headInfo_loadLenTemp = sizeof(uint8_t) * MWIFI_ADDR_LEN;
		memcpy(&devElecsumInfo[headInfo_loadIst], devRouterBssid, headInfo_loadLenTemp); //设备当前bssid/meshid

		listNodeDevStatus_opreatingFlg = false;

		return devElecsumInfo;
	}
}

void L8devHeartbeatFunctionParamLoad(stt_hbDataUpload *nodeDev_dataTemp){

	stt_hbDataUpload nodeDev_hbDataTemp = {0};
	stt_devDataPonitTypedef devDataPoint_temp = {0};

	esp_wifi_get_mac(ESP_IF_WIFI_STA, nodeDev_dataTemp->nodeDev_Mac); //mac地址填装
	nodeDev_dataTemp->nodeDev_Type = currentDev_typeGet(); //设备类型填装
	currentDev_dataPointGet(&devDataPoint_temp); //设备状态获取
	memcpy(&(nodeDev_dataTemp->nodeDev_Status), &devDataPoint_temp, sizeof(stt_devDataPonitTypedef)); //设备状态填装
	devMutualCtrlGroupInfo_groupInsertGet(nodeDev_dataTemp->nodeDev_mautualInfo); //互控信息填装
	nodeDev_dataTemp->nodeDev_runningFlg = currentDevRunningFlg_paramGet(); //设备运行状态填装
	devDriverBussiness_temperatureMeasure_getByHex(&(nodeDev_dataTemp->nodeDev_dataTemprature)); //设备温度数据填装 
	devDriverBussiness_elecMeasure_valPowerGetByHex(&(nodeDev_dataTemp->nodeDev_dataPower)); //设备功率数据填装
	devDriverBussiness_elecMeasure_valElecsumGetByHex(&(nodeDev_dataTemp->nodeDev_dataElecsum)); //设备电量数据填装
	currentDev_extParamGet(nodeDev_dataTemp->nodeDev_extFunParam);
}

void L8devDetailInfoParamLoad(stt_devInfoDetailUpload *nodeDev_dataTemp){

	uint16_t devVerTemp = systemDevice_currentVersionGet();
	stt_devDataPonitTypedef devStatus_temp = {0};
	stt_dataDisp_guiBussinessHome_btnText dataTextObjDisp_temp = {0};

	esp_wifi_get_mac(ESP_IF_WIFI_STA, nodeDev_dataTemp->nodeDev_Mac); //mac地址填装
	nodeDev_dataTemp->data2Server.nodeDev_Version[0] = (uint8_t)((devVerTemp & 0xff00) >> 8);
	nodeDev_dataTemp->data2Server.nodeDev_Version[1] = (uint8_t)((devVerTemp & 0x00ff) >> 0);

	nodeDev_dataTemp->data2Root.devType = currentDev_typeGet();
	currentDev_dataPointGet(&devStatus_temp);
	memcpy(&nodeDev_dataTemp->data2Root.devSelf_status, &devStatus_temp, sizeof(uint8_t));
	usrAppHomepageBtnIconNumDisp_paramGet(nodeDev_dataTemp->data2Root.devSelf_iconIst);
	usrAppHomepageBtnTextDisp_paramGet(&dataTextObjDisp_temp);
	memcpy(nodeDev_dataTemp->data2Root.devSelf_name[0], (char *)dataTextObjDisp_temp.dataBtnTextDisp[0], sizeof(char) * DEV_CTRLOBJ_NAME_DETAILUD_LEN);
	memcpy(nodeDev_dataTemp->data2Root.devSelf_name[1], (char *)dataTextObjDisp_temp.dataBtnTextDisp[1], sizeof(char) * DEV_CTRLOBJ_NAME_DETAILUD_LEN);
	memcpy(nodeDev_dataTemp->data2Root.devSelf_name[2], (char *)dataTextObjDisp_temp.dataBtnTextDisp[2], sizeof(char) * DEV_CTRLOBJ_NAME_DETAILUD_LEN);
}	

void L8devHbDataManageList_delSame(stt_nodeDev_hbDataManage *pHead){

    stt_nodeDev_hbDataManage *p,*q,*r;
    p = pHead->next; 

	listNodeDevStatus_opreatingFlg = true;
	
    while(p != NULL)    
    {
        q = p;
        while(q->next != NULL) 
        {
            if(!memcmp(q->next->dataManage.nodeDev_Mac, p->dataManage.nodeDev_Mac, MWIFI_ADDR_LEN)) 
            {
                r = q->next; 
                q->next = r->next;   

				memcpy(&(q->dataManage), &(r->dataManage), sizeof(stt_hbDataUpload)); //后到优先更新
				q->nodeDevKeepAlive_counter = r->nodeDevKeepAlive_counter;
				
				free(r);
            }
            else
            {
            	q = q->next;
			}
        }

        p = p->next;
    }

	listNodeDevStatus_opreatingFlg = false;
}

uint8_t L8devHbDataManageList_nodeNumDetect(stt_nodeDev_hbDataManage *pHead){

	stt_nodeDev_hbDataManage *pAbove = pHead;
	stt_nodeDev_hbDataManage *pFollow;
	uint8_t loop = 0;

	listNodeDevStatus_opreatingFlg = true;

	while(pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;

		loop ++;
	}

	listNodeDevStatus_opreatingFlg = false;

	return loop;
}

uint8_t *L8devHbDataManageList_listGet(stt_nodeDev_hbDataManage *pHead){

	uint8_t listLen = L8devHbDataManageList_nodeNumDetect(pHead) + 1;
	uint8_t *listInfo = (uint8_t *)os_zalloc(sizeof(stt_hbDataUpload) * listLen);
	stt_nodeDev_hbDataManage *pAbove = pHead;
	uint8_t loop = 1;

	listNodeDevStatus_opreatingFlg = true;

	listInfo[0] = listLen;
	memset(listInfo, 0, sizeof(stt_hbDataUpload) * listLen);

	while(pAbove->next != NULL){

		memcpy(&listInfo[sizeof(stt_hbDataUpload) * loop + 1], &(pAbove->next->dataManage), sizeof(stt_hbDataUpload));
		pAbove = pAbove->next;
		loop ++;
	}

	listNodeDevStatus_opreatingFlg = false;

	return listInfo;
}

void L8devHbDataManageList_listDestory(stt_nodeDev_hbDataManage *pHead){

	stt_nodeDev_hbDataManage *pAbove = pHead;
	stt_nodeDev_hbDataManage *pFollow = pAbove;

	stt_nodeDev_hbDataManage *pTemp = NULL;

	listNodeDevStatus_opreatingFlg = true;

	while(pAbove->next != NULL){

		pTemp 	= pAbove->next;
		pFollow->next = pTemp->next;
		free(pTemp);
		pAbove 	= pFollow;
		pFollow = pAbove;
	}

	listNodeDevStatus_opreatingFlg = false;
}

uint8_t L8devHbDataManageList_nodeCreat(stt_nodeDev_hbDataManage *pHead, stt_nodeDev_hbDataManage *pNew){
	
	stt_nodeDev_hbDataManage *pAbove = pHead;
	stt_nodeDev_hbDataManage *pFollow = NULL;
	uint8_t nCount = 0;

	listNodeDevStatus_opreatingFlg = true;
	
	stt_nodeDev_hbDataManage *pNew_temp = (stt_nodeDev_hbDataManage *)os_zalloc(sizeof(stt_nodeDev_hbDataManage));
	memcpy(&(pNew_temp->dataManage), &(pNew->dataManage), sizeof(stt_hbDataUpload));
	pNew_temp->nodeDevKeepAlive_counter = pNew->nodeDevKeepAlive_counter;
	pNew_temp->next = NULL;
	
	while(pAbove->next != NULL){
	
		pFollow = pAbove;
		pAbove	= pFollow->next;

		nCount ++;
	}
	
	pAbove->next = pNew_temp;

	listNodeDevStatus_opreatingFlg = false;
	
	return ++nCount;
}

stt_nodeDev_hbDataManage *L8devHbDataManageList_nodeGet(stt_nodeDev_hbDataManage *pHead, uint8_t nodeDev_Mac[MWIFI_ADDR_LEN], bool method){	//method = 1,源节点地址返回，操作返回内存影响源节点信息; method = 0,映射信息地址返回，操作返回内存，不影响源节点信息.
	
	stt_nodeDev_hbDataManage *pAbove = pHead;
	stt_nodeDev_hbDataManage *pFollow = NULL;
	
	stt_nodeDev_hbDataManage *pTemp = (stt_nodeDev_hbDataManage *)os_zalloc(sizeof(stt_nodeDev_hbDataManage));
	pTemp->next = NULL;

	listNodeDevStatus_opreatingFlg = true;
	
	while((0 != memcmp(pAbove->dataManage.nodeDev_Mac, nodeDev_Mac, MWIFI_ADDR_LEN)) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(!memcmp(pAbove->dataManage.nodeDev_Mac, nodeDev_Mac, MWIFI_ADDR_LEN)){
		
		if(!method){
			
			memcpy(&(pTemp->dataManage), &(pAbove->dataManage), sizeof(stt_hbDataUpload));
			pTemp->nodeDevKeepAlive_counter = pAbove->nodeDevKeepAlive_counter;
		
		}else{
			
			free(pTemp);
			pTemp = pAbove;	
		}

		listNodeDevStatus_opreatingFlg = false;
		
		return pTemp;
		
	}else{
		
		free(pTemp);

		listNodeDevStatus_opreatingFlg = false;
		
		return NULL;
	}	
} 

bool L8devHbDataManageList_nodeRemove(stt_nodeDev_hbDataManage *pHead, uint8_t nodeDev_Mac[MWIFI_ADDR_LEN]){
	
	stt_nodeDev_hbDataManage *pAbove = pHead;
	stt_nodeDev_hbDataManage *pFollow = NULL;
	
	stt_nodeDev_hbDataManage *pTemp = NULL;

	listNodeDevStatus_opreatingFlg = true;
	
	while((0 != memcmp(pAbove->dataManage.nodeDev_Mac, nodeDev_Mac, MWIFI_ADDR_LEN)) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(!memcmp(pAbove->dataManage.nodeDev_Mac, nodeDev_Mac, MWIFI_ADDR_LEN)){
		
		pTemp = pAbove;
		pFollow->next = pAbove->next;
		free(pTemp);

		listNodeDevStatus_opreatingFlg = false;

		return true;
		 
	}else{

		listNodeDevStatus_opreatingFlg = false;

		return false;
	}
}

void L8devHbDataManageList_bussinessKeepAliveManagePeriod1s(stt_nodeDev_hbDataManage *pHead_hb, 
																					   stt_nodeDev_detailInfoManage *pHead_di,
																					   stt_nodeObj_listManageDevCtrlBase *pHead_gb){

	extern uint8_t datatransOpreation_heartbeatHold_get(void);

	stt_nodeDev_hbDataManage *pAbove = pHead_hb;
	stt_nodeDev_hbDataManage *pFollow = pAbove;

	stt_nodeDev_hbDataManage *pTemp = NULL;

	if(datatransOpreation_heartbeatHold_get())return;
	if(listNodeDevStatus_opreatingFlg)return;

	if(mwifi_is_connected()){

		if(esp_mesh_get_layer() == MESH_ROOT){
		
			while(pAbove->next != NULL){

				if(pAbove->next->nodeDevKeepAlive_counter){

					pAbove->next->nodeDevKeepAlive_counter --;
//					printf("nodeMac:"MACSTR"-lifeTime:%d.\n", MAC2STR(pAbove->next->dataManage.nodeDev_Mac), //释放前打印，否则读空指针，要崩
//															  pAbove->next->nodeDevKeepAlive_counter);

					pAbove  = pAbove->next;
					pFollow = pAbove;
				}
				else
				{
					bool dInfoNodeDel_res = L8devInfoDetailManageList_nodeRemove(pHead_di, pAbove->next->dataManage.nodeDev_Mac),
						 gBlockNodeDel_res = lvglUsrApp_devCtrlBlockBaseManageList_nodeRemove(pHead_gb, pAbove->next->dataManage.nodeDev_Mac);
				
					printf("nodeMac:"MACSTR"-remove cause heartbeat miss, Dinfo node del res:%d, Gblock node del res:%d\n", MAC2STR(pAbove->next->dataManage.nodeDev_Mac), dInfoNodeDel_res, gBlockNodeDel_res);  //释放前打印，否则读空指针，要崩

					pTemp = pAbove->next;
					pFollow->next = pTemp->next;
					free(pTemp);

					pAbove = pFollow;
				}
			
			}
		}
		else
		{
			L8devHbDataManageList_listDestory(pHead_hb);
			L8devInfoDetailManageList_listDestory(pHead_di);
		}
	}
}

uint8_t L8devInfoDetailManageList_nodeNumDetect(stt_nodeDev_detailInfoManage *pHead){

	stt_nodeDev_detailInfoManage *pAbove = pHead;
	stt_nodeDev_detailInfoManage *pFollow;
	uint8_t loop = 0;

	listNodeDevDetailInfo_opreatingFlg = true;

	while(pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;

		loop ++;
	}

	listNodeDevDetailInfo_opreatingFlg = false;

	return loop;
}

uint8_t L8devInfoDetailManageList_nodeCreat(stt_nodeDev_detailInfoManage *pHead, stt_nodeDev_detailInfoManage *pNew){
	
	stt_nodeDev_detailInfoManage *pAbove = pHead;
	stt_nodeDev_detailInfoManage *pFollow = NULL;
	uint8_t nCount = 0;

	listNodeDevDetailInfo_opreatingFlg = true;
	
	stt_nodeDev_detailInfoManage *pNew_temp = (stt_nodeDev_detailInfoManage *)os_zalloc(sizeof(stt_nodeDev_detailInfoManage));
	memcpy(&(pNew_temp->dataManage), &(pNew->dataManage), sizeof(stt_devInfoDetailUpload));
	pNew_temp->next = NULL;
	
	while(pAbove->next != NULL){
	
		pFollow = pAbove;
		pAbove	= pFollow->next;

		nCount ++;
	}
	
	pAbove->next = pNew_temp;

	listNodeDevDetailInfo_opreatingFlg = false;
	
	return ++nCount;
}

stt_nodeDev_detailInfoManage * L8devInfoDetailManageList_nodeGet(stt_nodeDev_detailInfoManage *pHead, uint8_t nodeDev_Mac[MWIFI_ADDR_LEN], bool method){	//method = 1,源节点地址返回，操作返回内存影响源节点信息; method = 0,映射信息地址返回，操作返回内存，不影响源节点信息.
	
	stt_nodeDev_detailInfoManage *pAbove = pHead;
	stt_nodeDev_detailInfoManage *pFollow = NULL;
	
	stt_nodeDev_detailInfoManage *pTemp = (stt_nodeDev_detailInfoManage *)os_zalloc(sizeof(stt_nodeDev_detailInfoManage));
	pTemp->next = NULL;

	listNodeDevDetailInfo_opreatingFlg = true;
	
	while((0 != memcmp(pAbove->dataManage.nodeDev_Mac, nodeDev_Mac, MWIFI_ADDR_LEN)) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(!memcmp(pAbove->dataManage.nodeDev_Mac, nodeDev_Mac, MWIFI_ADDR_LEN)){
		
		if(!method){
			
			memcpy(&(pTemp->dataManage), &(pAbove->dataManage), sizeof(stt_devInfoDetailUpload));
		
		}else{
			
			free(pTemp);
			pTemp = pAbove;	
		}

		listNodeDevDetailInfo_opreatingFlg = false;
		
		return pTemp;
		
	}else{
		
		free(pTemp);

		listNodeDevDetailInfo_opreatingFlg = false;
		
		return NULL;
	}	
} 

bool L8devInfoDetailManageList_nodeRemove(stt_nodeDev_detailInfoManage *pHead, uint8_t nodeDev_Mac[MWIFI_ADDR_LEN]){
	
	stt_nodeDev_detailInfoManage *pAbove = pHead;
	stt_nodeDev_detailInfoManage *pFollow = NULL;
	
	stt_nodeDev_detailInfoManage *pTemp = NULL;

	listNodeDevDetailInfo_opreatingFlg = true;
	
	while(!(!memcmp(pAbove->dataManage.nodeDev_Mac, nodeDev_Mac, MWIFI_ADDR_LEN)) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(!memcmp(pAbove->dataManage.nodeDev_Mac, nodeDev_Mac, MWIFI_ADDR_LEN)){
		
		pTemp = pAbove;
		pFollow->next = pAbove->next;
		free(pTemp);

		listNodeDevDetailInfo_opreatingFlg = false;

		return true;
		 
	}else{

		listNodeDevDetailInfo_opreatingFlg = false;

		return false;
	}
}

void L8devInfoDetailManageList_listDestory(stt_nodeDev_detailInfoManage *pHead){

	stt_nodeDev_detailInfoManage *pAbove = pHead;
	stt_nodeDev_detailInfoManage *pFollow = pAbove;

	stt_nodeDev_detailInfoManage *pTemp = NULL;

	listNodeDevDetailInfo_opreatingFlg = true;

	while(pAbove->next != NULL){

		pTemp 	= pAbove->next;
		pFollow->next = pTemp->next;
		free(pTemp);
		pAbove 	= pFollow;
		pFollow = pAbove;
	}

	listNodeDevDetailInfo_opreatingFlg = false;
}

uint8_t lvglUsrApp_devCtrlBlockBaseManageList_nodeNumDetect(stt_nodeObj_listManageDevCtrlBase *pHead){

	stt_nodeObj_listManageDevCtrlBase *pAbove = pHead;
	stt_nodeObj_listManageDevCtrlBase *pFollow;
	uint8_t loop = 0;

	listNodeDevLvGblock_opreatingFlg = true;

	while(pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;

		loop ++;
	}

	listNodeDevLvGblock_opreatingFlg = false;

	return loop;
}

uint8_t lvglUsrApp_devCtrlBlockBaseManageList_nodeAdd(stt_nodeObj_listManageDevCtrlBase *pHead, stt_nodeObj_listManageDevCtrlBase *pNew){
	
	stt_nodeObj_listManageDevCtrlBase *pAbove = pHead;
	stt_nodeObj_listManageDevCtrlBase *pFollow = NULL;
	uint8_t nCount = 0;
	bool nodeRepeated_flg = false;

	listNodeDevLvGblock_opreatingFlg = true;
	
	stt_nodeObj_listManageDevCtrlBase *pNew_temp = (stt_nodeObj_listManageDevCtrlBase *) os_zalloc(sizeof(stt_nodeObj_listManageDevCtrlBase));
	memcpy(&(pNew_temp->nodeData), &(pNew->nodeData), sizeof(stt_devCtrlObjGraphBlockDataBase));
	pNew_temp->next = NULL;
	
	while(pAbove->next != NULL){

		if(0 == memcmp(pAbove->next->nodeData.ctrlObj_devMac, pNew->nodeData.ctrlObj_devMac, sizeof(uint8_t) * MWIFI_ADDR_LEN)){

			memcpy(&pAbove->next->nodeData, &pNew_temp->nodeData, sizeof(stt_devCtrlObjGraphBlockDataBase)); //相同MAC设备则进行数据更新
			nodeRepeated_flg = true;

//			printf("gBlock[devType:%02x, mac:%02X%02X%02X%02X%02X%02X] reapt, creat give up.\n",
//					pNew->nodeData.ctrlObj_devType,
//					MAC2STR(pNew->nodeData.ctrlObj_devMac));
		}
	
		pFollow = pAbove;
		pAbove	= pFollow->next;

		nCount ++;
	}

	if(false == nodeRepeated_flg){ //发生重复节点

		pAbove->next = pNew_temp;
		nCount ++;
	}
	else
	{
		free(pNew_temp);
	}

	listNodeDevLvGblock_opreatingFlg = false;
	
	return nCount;
}

stt_nodeObj_listManageDevCtrlBase * lvglUsrApp_devCtrlBlockBaseManageList_nodeGet(stt_nodeObj_listManageDevCtrlBase *pHead, uint8_t nodeDev_Mac[MWIFI_ADDR_LEN], bool method){	//method = 1,源节点地址返回，操作返回内存影响源节点信息; method = 0,映射信息地址返回，操作返回内存，不影响源节点信息.
	
	stt_nodeObj_listManageDevCtrlBase *pAbove = pHead;
	stt_nodeObj_listManageDevCtrlBase *pFollow = NULL;
	
	stt_nodeObj_listManageDevCtrlBase *pTemp = (stt_nodeObj_listManageDevCtrlBase *)os_zalloc(sizeof(stt_nodeObj_listManageDevCtrlBase));
	pTemp->next = NULL;

	listNodeDevLvGblock_opreatingFlg = true;
	
	while((0 != memcmp(pAbove->nodeData.ctrlObj_devMac, nodeDev_Mac, MWIFI_ADDR_LEN)) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(!memcmp(pAbove->nodeData.ctrlObj_devMac, nodeDev_Mac, MWIFI_ADDR_LEN)){
		
		if(!method){
			
			memcpy(&(pTemp->nodeData), &(pAbove->nodeData), sizeof(stt_devCtrlObjGraphBlockDataBase));
		
		}else{
			
			free(pTemp);
			pTemp = pAbove;	
		}

		listNodeDevLvGblock_opreatingFlg = false;
		
		return pTemp;
		
	}else{
		
		free(pTemp);

		listNodeDevLvGblock_opreatingFlg = false;
		
		return NULL;
	}	
} 

stt_nodeObj_listManageDevCtrlBase * lvglUsrApp_devCtrlBlockBaseManageList_nodeGetByLvObjFreenum(stt_nodeObj_listManageDevCtrlBase *pHead, LV_OBJ_FREE_NUM_TYPE fNumHead, bool method){	//method = 1,源节点地址返回，操作返回内存影响源节点信息; method = 0,映射信息地址返回，操作返回内存，不影响源节点信息.
	
	stt_nodeObj_listManageDevCtrlBase *pAbove = pHead->next;
	stt_nodeObj_listManageDevCtrlBase *pFollow = NULL;
	
	stt_nodeObj_listManageDevCtrlBase *pTemp = NULL;

	if(NULL == pAbove)return NULL;

	listNodeDevLvGblock_opreatingFlg = true;
	
	while((pAbove->nodeData.lv_fNum_objImgCtrl[0] != fNumHead) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if((pAbove->nodeData.lv_fNum_objImgCtrl[0] == fNumHead)){
		
		if(!method){

			pTemp = (stt_nodeObj_listManageDevCtrlBase *)os_zalloc(sizeof(stt_nodeObj_listManageDevCtrlBase));
			pTemp->next = NULL;
			memcpy(&(pTemp->nodeData), &(pAbove->nodeData), sizeof(stt_devCtrlObjGraphBlockDataBase));
		
		}else{
			
			free(pTemp);
			pTemp = pAbove;	
		}

		listNodeDevLvGblock_opreatingFlg = false;
		
		return pTemp;
		
	}else{
		
		free(pTemp);

		listNodeDevLvGblock_opreatingFlg = false;
		
		return NULL;
	}	
} 

bool lvglUsrApp_devCtrlBlockBaseManageList_nodeRemove(stt_nodeObj_listManageDevCtrlBase *pHead, uint8_t nodeDev_Mac[MWIFI_ADDR_LEN]){
	
	stt_nodeObj_listManageDevCtrlBase *pAbove = pHead;
	stt_nodeObj_listManageDevCtrlBase *pFollow = NULL;
	
	stt_nodeObj_listManageDevCtrlBase *pTemp = NULL;

	listNodeDevLvGblock_opreatingFlg = true;
	
	while((0 != memcmp(pAbove->nodeData.ctrlObj_devMac, nodeDev_Mac, MWIFI_ADDR_LEN)) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(!memcmp(pAbove->nodeData.ctrlObj_devMac, nodeDev_Mac, MWIFI_ADDR_LEN)){
		
		pTemp = pAbove;
		pFollow->next = pAbove->next;
		free(pTemp);

		listNodeDevLvGblock_opreatingFlg = false;

		return true;
		 
	}else{

		listNodeDevLvGblock_opreatingFlg = false;

		return false;
	}
}

void lvglUsrApp_devCtrlBlockBaseManageList_listDestory(stt_nodeObj_listManageDevCtrlBase *pHead){

	stt_nodeObj_listManageDevCtrlBase *pAbove = pHead;
	stt_nodeObj_listManageDevCtrlBase *pFollow = pAbove;

	stt_nodeObj_listManageDevCtrlBase *pTemp = NULL;

	listNodeDevLvGblock_opreatingFlg = true;

	while(pAbove->next != NULL){

		pTemp 	= pAbove->next;
		pFollow->next = pTemp->next;
		free(pTemp);
		pAbove 	= pFollow;
		pFollow = pAbove;
	}

	listNodeDevLvGblock_opreatingFlg = false;
}

uint8_t *L8devInfoDetailManageList_data2ServerTabGet(stt_nodeDev_detailInfoManage *pHead){

	uint8_t *listDataTab = NULL;

	if(devRunningTimeFromPowerUp_couter <= L8_DEV_LISTMANAGE_REALES_CONFIRM){

		listDataTab= (uint8_t *)os_zalloc(sizeof(uint8_t) * 1);
		*listDataTab = DEVLIST_MANAGE_LISTNUM_MASK_NULL;

	}else{	

		struct tabHead_dataDiscrip{

			uint8_t devNodeNum;
			
		}dataTabHead = {0};
		uint8_t dataTabHead_Len = sizeof(struct tabHead_dataDiscrip);
		uint8_t dataTabUnit_Len = MWIFI_ADDR_LEN + sizeof(stt_devInfoDetailUpload_2Server);
		uint8_t listNodeNum = L8devInfoDetailManageList_nodeNumDetect(pHead) + 1;
		stt_nodeDev_detailInfoManage *pAbove = pHead;
		uint8_t loopCount = 0;

		uint8_t devSelfMac[MWIFI_ADDR_LEN] = {0};
		stt_devInfoDetailUpload_2Server selfData_temp = {0};
		uint16_t devVerVal_temp = 0;

		listDataTab = (uint8_t *)os_zalloc(dataTabUnit_Len * listNodeNum + dataTabHead_Len);

		memset(&listDataTab[0], 0, dataTabHead_Len);
		memset(&listDataTab[dataTabHead_Len], 0, dataTabUnit_Len * listNodeNum);
		
		esp_wifi_get_mac(ESP_IF_WIFI_STA, devSelfMac);
		devVerVal_temp = systemDevice_currentVersionGet();
		selfData_temp.nodeDev_Version[0] = (uint8_t)((devVerVal_temp & 0xff00) >> 8);
		selfData_temp.nodeDev_Version[1] = (uint8_t)((devVerVal_temp & 0x00ff) >> 0);

		memcpy(&listDataTab[dataTabHead_Len + dataTabUnit_Len * loopCount + 0], devSelfMac, MWIFI_ADDR_LEN); //MAC装填
		memcpy(&listDataTab[dataTabHead_Len + dataTabUnit_Len * loopCount + MWIFI_ADDR_LEN], &selfData_temp, sizeof(stt_devInfoDetailUpload_2Server)); //数据装填

		loopCount ++;

		while(pAbove->next != NULL){

			uint8_t ist = 0;
			
			memcpy(&listDataTab[dataTabHead_Len + dataTabUnit_Len * loopCount + ist], pAbove->next->dataManage.nodeDev_Mac, MWIFI_ADDR_LEN); //MAC装填
			ist += MWIFI_ADDR_LEN;
			memcpy(&listDataTab[dataTabHead_Len + dataTabUnit_Len * loopCount + ist], &pAbove->next->dataManage.data2Server, sizeof(stt_devInfoDetailUpload_2Server)); //数据装填
			
			pAbove = pAbove->next;
			loopCount ++;
		}

		dataTabHead.devNodeNum = listNodeNum;
		memcpy(listDataTab, &dataTabHead, dataTabHead_Len);
	}

	return listDataTab;
}

uint8_t *L8devInfoDetailManageList_data2RootTabGet(stt_nodeDev_detailInfoManage *pHead){

	uint8_t *listDataTab = NULL;

	if(devRunningTimeFromPowerUp_couter <= L8_DEV_LISTMANAGE_REALES_CONFIRM){

		listDataTab= (uint8_t *)os_zalloc(sizeof(uint8_t) * 1);
		*listDataTab = DEVLIST_MANAGE_LISTNUM_MASK_NULL;

	}else{	

		struct tabHead_dataDiscrip{

			uint16_t dataTabCompleteLen;
			uint8_t dataNullForExt;
			uint8_t devNodeNum;
			
		}dataTabHead = {0};
		uint8_t dataTabHead_Len = sizeof(struct tabHead_dataDiscrip);
		uint8_t dataTabUnit_Len = MWIFI_ADDR_LEN + sizeof(stt_devInfoDetailUpload_2Root);
		uint8_t listNodeNum = L8devInfoDetailManageList_nodeNumDetect(pHead) + 1;
		stt_nodeDev_detailInfoManage *pAbove = pHead;
		uint8_t loopCount = 0;

		uint8_t devSelfMac[MWIFI_ADDR_LEN] = {0};
		stt_devInfoDetailUpload_2Root selfData_temp = {0};
		stt_dataDisp_guiBussinessHome_btnText dataTextObjDisp_temp = {0};

		dataTabHead.dataTabCompleteLen = dataTabUnit_Len * listNodeNum + dataTabHead_Len;
		listDataTab = (uint8_t *)os_zalloc(dataTabHead.dataTabCompleteLen);

		memset(&listDataTab[0], 0, dataTabHead_Len);
		memset(&listDataTab[dataTabHead_Len], 0, dataTabUnit_Len * listNodeNum);
		
		esp_wifi_get_mac(ESP_IF_WIFI_STA, devSelfMac);
		selfData_temp.devType = currentDev_typeGet();
		usrAppHomepageBtnTextDisp_paramGet(&dataTextObjDisp_temp);
		usrAppHomepageBtnIconNumDisp_paramGet(selfData_temp.devSelf_iconIst);
//		memcpy(selfData_temp.devSelf_name, dataTextObjDisp_temp.dataBtnTextDisp, sizeof(uint8_t) * GUIBUSSINESS_CTRLOBJ_MAX_NUM * DEV_CTRLOBJ_NAME_DETAILUD_LEN);
		memcpy(selfData_temp.devSelf_name[0], dataTextObjDisp_temp.dataBtnTextDisp[0], sizeof(uint8_t) * DEV_CTRLOBJ_NAME_DETAILUD_LEN);
		memcpy(selfData_temp.devSelf_name[1], dataTextObjDisp_temp.dataBtnTextDisp[1], sizeof(uint8_t) * DEV_CTRLOBJ_NAME_DETAILUD_LEN);
		memcpy(selfData_temp.devSelf_name[2], dataTextObjDisp_temp.dataBtnTextDisp[2], sizeof(uint8_t) * DEV_CTRLOBJ_NAME_DETAILUD_LEN);
		currentDev_dataPointGet((stt_devDataPonitTypedef *)&selfData_temp.devSelf_status);

		memcpy(&listDataTab[dataTabHead_Len + dataTabUnit_Len * loopCount + 0], devSelfMac, MWIFI_ADDR_LEN); //MAC装填
		memcpy(&listDataTab[dataTabHead_Len + dataTabUnit_Len * loopCount + MWIFI_ADDR_LEN], &selfData_temp, sizeof(stt_devInfoDetailUpload_2Root)); //数据装填

		loopCount ++;

		// while(pAbove->next != NULL){
		while((pAbove->next != NULL) && loopCount < 15){ //limit the num

			uint8_t ist = 0;
			
			memcpy(&listDataTab[dataTabHead_Len + dataTabUnit_Len * loopCount + ist], pAbove->next->dataManage.nodeDev_Mac, MWIFI_ADDR_LEN); //MAC装填
			ist += MWIFI_ADDR_LEN;
			memcpy(&listDataTab[dataTabHead_Len + dataTabUnit_Len * loopCount + ist], &pAbove->next->dataManage.data2Root, sizeof(stt_devInfoDetailUpload_2Root)); //数据装填
			
			pAbove = pAbove->next;
			loopCount ++;
		}

		dataTabHead.dataNullForExt = 0;
		dataTabHead.devNodeNum = listNodeNum;
		memcpy(listDataTab, &dataTabHead, dataTabHead_Len);
	}

	return listDataTab;
}

void usrApplication_systemRestartTrig(uint8_t trigDelay){

	devRestartDelay_counter = trigDelay;
}

void devSystemInfoLocalRecord_preSaveTest(void){

	nvs_handle handle;

	stt_devDataPonitTypedef dataTemp_devInfo_swStatus = {0}; //数据缓存：开关状态
	usrApp_trigTimer dataTemp_trigTimerGroup[USRAPP_VALDEFINE_TRIGTIMER_NUM] = {0}; //数据缓存：普通定时

	ESP_ERROR_CHECK( nvs_flash_init_partition(NVS_DATA_L8_PARTITION_NAME));
	ESP_ERROR_CHECK( nvs_open_from_partition(NVS_DATA_L8_PARTITION_NAME, NVS_DATA_SYSINFO_RECORD, NVS_READWRITE, &handle) );
	
	ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_SWSTATUS, &dataTemp_devInfo_swStatus, sizeof(stt_devDataPonitTypedef)) );
	ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_INFO_TIMER_NORMAL, dataTemp_trigTimerGroup, sizeof(usrApp_trigTimer) * USRAPP_VALDEFINE_TRIGTIMER_NUM) );

	ESP_ERROR_CHECK( nvs_commit(handle) );

	nvs_close(handle);
	ESP_ERROR_CHECK( nvs_flash_deinit_partition(NVS_DATA_L8_PARTITION_NAME));
}

void nvsDataOpreation_devInfraredParam_set(uint8_t irDats[DEVINFRARED_HXD019D_IIC_DATA_BUF_LEN], uint8_t cmdInsert){

	stt_devInfraredNvsOpreatParam paramTemp = {0};

	sprintf(paramTemp.nvsKeyWord, "%s%d", DATA_DEVINFRARED_DATA_PARAM, cmdInsert);
	memcpy(paramTemp.dats.ir_dataOpreat_buf, irDats, sizeof(uint8_t) * DEVINFRARED_HXD019D_IIC_DATA_BUF_LEN);

	devSystemInfoLocalRecord_save(saveObj_devInfrared_paramDats, &paramTemp);
}

stt_infraredSwitchData_nvsOpreat *nvsDataOpreation_devInfraredParam_get(uint8_t cmdInsert){

	nvs_handle handle;

	char nvsOpreat_key[16] = {0};

	uint32_t dataLength = 0;
	esp_err_t err;

	stt_infraredSwitchData_nvsOpreat *dataParam = (stt_infraredSwitchData_nvsOpreat *)os_zalloc(sizeof(stt_infraredSwitchData_nvsOpreat));

	sprintf(nvsOpreat_key, "%s%d", DATA_DEVINFRARED_DATA_PARAM, cmdInsert);

	ESP_ERROR_CHECK( nvs_flash_init_partition(NVS_DATA_L8_PARTITION_NAME));
    ESP_ERROR_CHECK( nvs_open_from_partition(NVS_DATA_L8_PARTITION_NAME, NVS_DATA_SYSINFO_RECORD, NVS_READWRITE, &handle) );

	dataLength = sizeof(stt_infraredSwitchData_nvsOpreat);
	err = nvs_get_blob(handle, nvsOpreat_key, dataParam, &dataLength);
	if(err == ESP_OK){

		ESP_LOGI(TAG,"nvs_data devInfrared dataParam read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devInfrared dataParam not found, maybe first running, err:0x%04X.\n", err);
	}

    nvs_close(handle);
	ESP_ERROR_CHECK( nvs_flash_deinit_partition(NVS_DATA_L8_PARTITION_NAME));

	return dataParam;
}

stt_scenarioSwitchData_nvsOpreat *nvsDataOpreation_devScenarioParam_get(uint8_t scenarioIst){

	nvs_handle handle;

	uint32_t dataLength = 0;
	esp_err_t err;

	stt_scenarioSwitchData_nvsOpreat *dataParam = (stt_scenarioSwitchData_nvsOpreat *)os_zalloc(sizeof(stt_scenarioSwitchData_nvsOpreat));
	static const char *nvsOpreat_key = NULL;

	switch(scenarioIst){

		case 1:nvsOpreat_key = DATA_DEVSCENARIO_DATA_PARAM_0;break;
		case 2:nvsOpreat_key = DATA_DEVSCENARIO_DATA_PARAM_1;break;
		case 3:
		default:nvsOpreat_key = DATA_DEVSCENARIO_DATA_PARAM_2;break;
	}

	ESP_ERROR_CHECK( nvs_flash_init_partition(NVS_DATA_L8_PARTITION_NAME));
    ESP_ERROR_CHECK( nvs_open_from_partition(NVS_DATA_L8_PARTITION_NAME, NVS_DATA_SYSINFO_RECORD, NVS_READWRITE, &handle) );

//	printf("nvsOpreat_key:%s.\n", nvsOpreat_key);
	
	dataLength = sizeof(stt_scenarioSwitchData_nvsOpreat);
	err = nvs_get_blob(handle, nvsOpreat_key, dataParam, &dataLength);
	if(err == ESP_OK){

		ESP_LOGI(TAG,"nvs_data devScenario dataParam read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devScenario dataParam not found, maybe first running, err:0x%04X.\n", err);
	}

    nvs_close(handle);
	ESP_ERROR_CHECK( nvs_flash_deinit_partition(NVS_DATA_L8_PARTITION_NAME));

	return dataParam;
}

void devSystemInfoLocalRecord_normalClear(void){ //一般擦除

	devMutualCtrlGroupInfo_Clr(); //互控数据清空
	usrAppActTrigTimer_paramClrReset(); //定时清空
	usrAppNightModeTimeTab_paramClrReset(); //夜间模式清空
	usrAppParamClrReset_devGreenMode(); //绿色模式清空
	usrAppParamClrReset_devDelayTrig(); //延时清空
}

void devSystemInfoLocalRecord_allErase(void){ //全域擦除

	nvs_handle handle;

	ESP_ERROR_CHECK( nvs_flash_init_partition(NVS_DATA_L8_PARTITION_NAME) );
    ESP_ERROR_CHECK( nvs_open_from_partition(NVS_DATA_L8_PARTITION_NAME, NVS_DATA_SYSINFO_RECORD, NVS_READWRITE, &handle) );
	ESP_ERROR_CHECK( nvs_erase_all(handle) );
    nvs_close(handle);
	ESP_ERROR_CHECK( nvs_flash_deinit_partition(NVS_DATA_L8_PARTITION_NAME) );
}

void devSystemInfoLocalRecord_initialize(void){

	nvs_handle handle;

	uint32_t dataLength = 0;
	esp_err_t err;

	stt_devDataPonitTypedef 				dataTemp_devInfo_swStatus 										= {0}; 	//数据缓存：开关状态
	usrApp_trigTimer 						dataTemp_trigTimerGroup[USRAPP_VALDEFINE_TRIGTIMER_NUM] 		= {0}; 	//数据缓存：普通定时
	uint8_t 								dataTemp_devInfraTimerUpIstTab[USRAPP_VALDEFINE_TRIGTIMER_NUM]	= {0};  //数据缓存：红外转发器专用定时响应数据
	uint8_t 								dataTemp_greenModePeriod[2] 									= {0}; 	//数据缓存：绿色模式
	usrApp_trigTimer 						dataTemp_nightModeTimeTab[2] 									= {0}; 	//数据缓存：夜间模式
	uint16_t 								dataTemp_devRunningFlg 											= 0; 	//数据缓存：设备运行标志
	stt_timeZone 							dataTemp_devTimeZone 											= {0};  //数据缓存：设备时区
	uint32_t								dataTemp_devElecSum												= 0;
	devTypeDef_enum 						dataTemp_devTypeDef												= L8_DEVICE_TYPE_DEFULT;
	stt_devStatusRecord 					dataTemp_devStatusRecordIF 										= {0};
	uint8_t 								dataTemp_devRouterConnectBssid[DEVICE_MAC_ADDR_APPLICATION_LEN] = {0};
	uint8_t 								dataTemp_devRouterRecordBssid[DEVICE_MAC_ADDR_APPLICATION_LEN] 	= {0};
	stt_devMutualGroupParam 				dataTemp_devMutaulCtrlInfo[DEVICE_MUTUAL_CTRL_GROUP_NUM] 		= {0};
	stt_paramLinkageConfig					dataTemp_devLinkageCfg											= {0};
	stt_devCurtain_runningParam				dataTemp_devCurtainParam										= {0};
	uint32_t 								dataTemp_devHeaterGearCustomTime								= 0;
	uint8_t 								dataTemp_devThermostatExSwStatus								= 0;
	stt_dataDisp_guiBussinessHome_btnText	dataTemp_homepageBtnTextDisp									= {0};
	uint8_t									dataTemp_homepageBtnIconNumDisp[GUIBUSSINESS_CTRLOBJ_MAX_NUM]	= {0};
	stt_bGroundThemeParam					dataTemp_homepageThemeParam										= {0};
	stt_devAtmosLightRunningParam			dataTemp_devAtmosRunningParam									= {0};
	uint8_t									dataTemp_devTempratureCalParam									= 0;
	char 									dataTemp_devDrviptRecalibraParam								= 0;
	stt_devScreenRunningParam				dataTemp_devScreenRunningConfigParam							= {0};
	stt_gModeOpFunc							dataTemp_greenModeUsrCfgDat										= {0};
	stt_routerCfgInfo						dataTemp_routerCfgInfoParam										= {0};
	stt_mqttCfgParam 						dataTemp_dtMqttParamInfo 										= {0};
	stt_mqttExServerCfgParam 				dataTemp_haServerParamInfo 										= {0};	
	uint8_t									dataTemp_epidCyAbbreIst											= 0;

#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SLAVE_MCU)
 #if(DRVMETHOD_BY_SLAVE_MCU_RELAY_TEST == 1)
	stt_RMTest_pRcd							dataTemp_paramMagRelayTest										= {0};	
 #endif		
#endif

	/*NVS用户数据存储操作互斥所创建*/
	xSph_usrAppNvsOpreat = xSemaphoreCreateMutex();

	/*子设备管理表，单链初始化*/
	listHead_nodeDevDataManage = (stt_nodeDev_hbDataManage *)os_zalloc(sizeof(stt_nodeDev_hbDataManage));
	listHead_nodeInfoDetailManage = (stt_nodeDev_detailInfoManage *)os_zalloc(sizeof(stt_nodeDev_detailInfoManage));
	listHead_nodeCtrlObjBlockBaseManage = (stt_nodeObj_listManageDevCtrlBase *)os_zalloc(sizeof(stt_nodeObj_listManageDevCtrlBase));

	/*图片RAM，内存初始化*/
//	dataPtr_btnTextImg_sw_A = (uint8_t *)os_zalloc(GUI_BUSSINESS_HOME_BTNTEXT_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE);
//	dataPtr_btnTextImg_sw_B = (uint8_t *)os_zalloc(GUI_BUSSINESS_HOME_BTNTEXT_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE);
//	dataPtr_btnTextImg_sw_C = (uint8_t *)os_zalloc(GUI_BUSSINESS_HOME_BTNTEXT_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE);

	dataPtr_bGroundPic = (uint8_t *)os_zalloc(GUI_BUSSINESS_HOME_BGROUND_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE);

	msgQh_dataManagementHandle = xQueueCreate(10, sizeof(stt_msgDats_dataManagementHandle));

	ESP_ERROR_CHECK( nvs_flash_init_partition(NVS_DATA_L8_PARTITION_NAME));
    ESP_ERROR_CHECK( nvs_open_from_partition(NVS_DATA_L8_PARTITION_NAME, NVS_DATA_SYSINFO_RECORD, NVS_READWRITE, &handle) );
	
	/*掉电数据更新 --普通定时数据*/
	dataLength = sizeof(usrApp_trigTimer) * USRAPP_VALDEFINE_TRIGTIMER_NUM;
	err = nvs_get_blob(handle, DATA_INFO_TIMER_NORMAL, dataTemp_trigTimerGroup, &dataLength);
	if(err == ESP_OK){

		usrAppActTrigTimer_paramSet(dataTemp_trigTimerGroup, false);
		ESP_LOGI(TAG,"nvs_data devAppTrigTimer info read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devAppTrigTimer info not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --夜间模式定时数据*/
	dataLength = sizeof(usrApp_trigTimer) * 2;
	err = nvs_get_blob(handle, DATA_INFO_TIMER_NIGHTMODE, dataTemp_nightModeTimeTab, &dataLength);
	if(err == ESP_OK){

		usrAppNightModeTimeTab_paramSet(dataTemp_nightModeTimeTab, false);
		ESP_LOGI(TAG,"nvs_data nightModeTimeTab info read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data nightModeTimeTab info not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --绿色模式数据*/
	dataLength = sizeof(uint8_t) * 2;
	err = nvs_get_blob(handle, DATA_INFO_TIMER_GREENMODE, dataTemp_greenModePeriod, &dataLength);
	if(err == ESP_OK){

		uint8_t temp = 0;

		//16bits数据转8bits数组数据，倒序处理
		temp = dataTemp_greenModePeriod[1];
		dataTemp_greenModePeriod[1] = dataTemp_greenModePeriod[0];
		dataTemp_greenModePeriod[0] = temp;

		usrAppParamSet_devGreenMode(dataTemp_greenModePeriod, false);
		ESP_LOGI(TAG,"nvs_data devGreenModePeriod info read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data evGreenModePeriod info not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --绿色模式用户配置数据*/
	dataLength = sizeof(stt_gModeOpFunc);
	err = nvs_get_blob(handle, DATA_GREENMODE_USRCFG, &dataTemp_greenModeUsrCfgDat, &dataLength);
	if(err == ESP_OK){
	
		usrAppParamSet_devGreenMode_actOption(&dataTemp_greenModeUsrCfgDat, false);
		ESP_LOGI(TAG,"nvs_data devGreenModePeriod usrCfg data read success.\n");
		
	}else{
	
		ESP_LOGI(TAG,"nvs_data evGreenModePeriod usrCfg data not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --设备运行标志*/
	dataLength = sizeof(uint16_t);
	err = nvs_get_blob(handle, DATA_INFO_DEVRUNNINGFLG, &dataTemp_devRunningFlg, &dataLength);
	if(err == ESP_OK){

		currentDevRunningFlg_paramSet(dataTemp_devRunningFlg, false);
		ESP_LOGI(TAG,"nvs_data devRunningFlg info read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devRunningFlg info not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --设备时区*/
	dataLength = sizeof(stt_timeZone);
	err = nvs_get_blob(handle, DATA_TIMEZONE, &dataTemp_devTimeZone, &dataLength);
	if(err == ESP_OK){

		deviceParamSet_timeZone(&dataTemp_devTimeZone, false);
		ESP_LOGI(TAG,"nvs_data devTimeZone info read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devTimeZone info not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --电量数据*/
	dataLength = sizeof(uint32_t);
	err = nvs_get_blob(handle, DATA_DEV_ELECSUM, &dataTemp_devElecSum, &dataLength);
	if(err == ESP_OK){
	
		devDriverBussiness_elecMeasure_elecSumRealesFromFlash(dataTemp_devElecSum);
		ESP_LOGI(TAG,"nvs_data devElecSum info read success.\n");
		
	}else{
	
		ESP_LOGI(TAG,"nvs_data devElecSum info not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --设备类型*/
#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_MOUDLE)
	extern void deviceTypeDefineByDcode_preScanning(void);
	deviceTypeDefineByDcode_preScanning(); //拨码预读取
	dataTemp_devTypeDef = currentDev_typeGet();
#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_SHARE_MIX) ||\ 
	 (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_MULIT_THERMO) ||\ 
	 (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_ALL_RESERVE)
	dataLength = sizeof(devTypeDef_enum);
	err = nvs_get_blob(handle, DATA_DEV_TYPEDEF, &dataTemp_devTypeDef, &dataLength);
	if(err == ESP_OK){

		currentDev_typeSet(dataTemp_devTypeDef, false);

		ESP_LOGI(TAG,"nvs_data devTypeDef info read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devTypeDef info not found, maybe first running, err:0x%04X.\n", err);
	}
#endif

	/*掉电数据更新 --开关状态（数据恢复必须在设备类型之后）*/
	dataLength = sizeof(stt_devDataPonitTypedef);
    err = nvs_get_blob(handle, DATA_SWSTATUS, &dataTemp_devInfo_swStatus, &dataLength);
	if(err == ESP_OK){

//		currentDev_dataPointSet(&dataTemp_devInfo_swStatus, false, false, false, false, false); //已改为在驱动初始化时进行
		ESP_LOGI(TAG,"nvs_data devStatus info read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devStatus info not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --设备状态掉电记忆使能*/
#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_MOUDLE) //模块仅需要掉电记忆标志数据，在拨码内读取
	devStatusRecordIF_paramGet(&dataTemp_devStatusRecordIF);
#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_SOCKET) //插座强制记忆
	dataTemp_devStatusRecordIF.devStatusOnOffRecord_IF = 1;
	devStatusRecordIF_paramSet(&dataTemp_devStatusRecordIF, false);
#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED) //红外转发器强制不记忆
	dataTemp_devStatusRecordIF.devStatusOnOffRecord_IF = 0;
	devStatusRecordIF_paramSet(&dataTemp_devStatusRecordIF, false);
#else

	dataLength = sizeof(stt_devStatusRecord);
	err = nvs_get_blob(handle, DATA_DEV_STATUS_RECORD_IF, &dataTemp_devStatusRecordIF, &dataLength);
	if(err == ESP_OK){

		devStatusRecordIF_paramSet(&dataTemp_devStatusRecordIF, false);
		
		ESP_LOGI(TAG,"nvs_data devStatusRecordIF info read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devStatusRecordIF info not found, maybe first running, err:0x%04X.\n", err);
	}
#endif

	if(dataTemp_devStatusRecordIF.devStatusOnOffRecord_IF){ //设备状态存储使能，则进行恢复操作
	
		currentDev_dataPointRecovery(&dataTemp_devInfo_swStatus); //记忆使能则数据保持，否则初始化数据不做更改（特别是恒温器，数据不能做更改）
//		currentDev_dataPointSet(&dataTemp_devInfo_swStatus, false, false, true, false, false); //由于时序问题改为在驱动初始化时执行操作
	}	
	else //特殊设备类型默认初始化数据加载
	{
		switch(currentDev_typeGet()){

			case devTypeDef_thermostat:
			case devTypeDef_thermostatExtension:{

				devDriverBussiness_datapointDefaultParamLoad();

			}break;

			default:break;
		}
	}

	/*掉电数据更新 --路由器BSSID*/
	dataLength = sizeof(uint8_t) * 6;
	err = nvs_get_blob(handle, DATA_DEV_ROUTER_BSSID, dataTemp_devRouterConnectBssid, &dataLength);
	if(err == ESP_OK){

		devRouterConnectBssid_Set(dataTemp_devRouterConnectBssid, false);
		ESP_LOGI(TAG,"nvs_data devRouterConnectBssid info read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devRouterConnectBssid info not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --历史路由器BSSID*/
	dataLength = sizeof(uint8_t) * 6;
	err = nvs_get_blob(handle, DATA_DEV_ROUTER_BSSIDRCD, dataTemp_devRouterRecordBssid, &dataLength);
	if(err == ESP_OK){

		devRouterRecordBssid_Set(dataTemp_devRouterRecordBssid, false);
		ESP_LOGI(TAG,"nvs_data devRouterRecordBssid info read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devRouterRecordBssid info not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --互控设置数据*/
	dataLength = sizeof(stt_devMutualGroupParam) * DEVICE_MUTUAL_CTRL_GROUP_NUM;
	err = nvs_get_blob(handle, DATA_DEV_MUTUALCTRL_INFO, dataTemp_devMutaulCtrlInfo, &dataLength);
	if(err == ESP_OK){

		devMutualCtrlGroupInfo_Set(&dataTemp_devMutaulCtrlInfo[0], 0x01, false);
		devMutualCtrlGroupInfo_Set(&dataTemp_devMutaulCtrlInfo[1], 0x02, false);
		devMutualCtrlGroupInfo_Set(&dataTemp_devMutaulCtrlInfo[2], 0x04, false);
		ESP_LOGI(TAG,"nvs_data devMutaulCtrl info read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devMutaulCtrl info not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --联动配置数据*/
	dataLength = sizeof(stt_paramLinkageConfig);
	err = nvs_get_blob(handle, DATA_DEV_LINKAGE_CONFIG, &dataTemp_devLinkageCfg, &dataLength);
	if(err == ESP_OK){

		devSystemOpration_linkageConfig_paramSet(&dataTemp_devLinkageCfg, false);
		ESP_LOGI(TAG,"nvs_data devLinkage config data read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devLinkage config data not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --窗帘开关设备运行参数（轨道时间）*/
	dataLength = sizeof(stt_Curtain_motorAttr);
	err = nvs_get_blob(handle, DATA_DEVCURTAIN_RUNNINGPARAM, &dataTemp_devCurtainParam, &dataLength);
	if(err == ESP_OK){

		devCurtain_runningParamSet(&dataTemp_devCurtainParam, false);
		
		ESP_LOGI(TAG,"nvs_data devCurtain running param read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devCurtain running param not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --热水器开关自定义档位 延时时间（单位：s）*/
	dataLength = sizeof(uint32_t);
	err = nvs_get_blob(handle, DATA_DEVHEATER_CUSTOMTIME, &dataTemp_devHeaterGearCustomTime, &dataLength);
	if(err == ESP_OK){

		devDriverBussiness_heaterSwitch_closePeriodCustom_Set(dataTemp_devHeaterGearCustomTime, false);
		
		ESP_LOGI(TAG,"nvs_data devHeater gearCustom time read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devHeater gearCustom time not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --恒温器扩展类型额外继电器状态恢复 延时时间（单位：s）*/
	dataLength = sizeof(uint8_t);
	err = nvs_get_blob(handle, DATA_DEVTHERMOSTATEX_SWPARAM, &dataTemp_devThermostatExSwStatus, &dataLength);
	if(err == ESP_OK){

		if(dataTemp_devStatusRecordIF.devStatusOnOffRecord_IF){ //记忆使能才进行状态恢复

			devDriverBussiness_thermostatSwitch_exSwitchParamReales(dataTemp_devThermostatExSwStatus);
		}
		
		ESP_LOGI(TAG,"nvs_data devThermostat exSwRly data read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devThermostat exSwRly data not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --红外转发器定时响应数据*/
	dataLength = sizeof(uint8_t) * USRAPP_VALDEFINE_TRIGTIMER_NUM;
	err = nvs_get_blob(handle, DATA_INFO_TIMER_IRDATA, dataTemp_devInfraTimerUpIstTab, &dataLength);
	if(err == ESP_OK){

		devDriverBussiness_infraredSwitch_timerUpTrigIstTabSet(dataTemp_devInfraTimerUpIstTab, false);
		
		ESP_LOGI(TAG,"nvs_data devInfrared timerUpIstTab data read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devInfrared timerUpIstTab data not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --home界面按键显示文字*///-顺序必须在设备类型更新之后
	dataLength = sizeof(stt_dataDisp_guiBussinessHome_btnText);
	err = nvs_get_blob(handle, DATA_DEV_GUIHOMEBTNTEXTDISP, &dataTemp_homepageBtnTextDisp, &dataLength);
	if(err == ESP_OK){

		usrAppHomepageBtnTextDisp_paramSet(&dataTemp_homepageBtnTextDisp, false);
		
		ESP_LOGI(TAG,"nvs_data homepage btnTextDisp param read success.\n");
		
	}else{

		usrAppHomepageBtnTextDisp_defaultLoad(dataTemp_devTypeDef, false);

		ESP_LOGI(TAG,"nvs_data homepage btnTextDisp param not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --home界面按键显示图标索引*/ //-顺序必须在设备类型更新之后
	dataLength = sizeof(uint8_t) * GUIBUSSINESS_CTRLOBJ_MAX_NUM;
	err = nvs_get_blob(handle, DATA_DEV_GUIHOMEBTNICONDISP, dataTemp_homepageBtnIconNumDisp, &dataLength);
	if(err == ESP_OK){

		usrAppHomepageBtnIconNumDisp_paramSet(dataTemp_homepageBtnIconNumDisp, false);
		
		ESP_LOGI(TAG,"nvs_data homepage btnIconNumDisp param read success.\n");
		
	}else{

		usrAppHomepageBtnIconNumDisp_defaultLoad(dataTemp_devTypeDef, false);

		ESP_LOGI(TAG,"nvs_data homepage btnIconNumDisp param not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --home界面主题相关参数*/
	dataLength = sizeof(stt_bGroundThemeParam);
	err = nvs_get_blob(handle, DATA_DEV_GUIHOMETHEMEPARAM, &dataTemp_homepageThemeParam, &dataLength);
	if(err == ESP_OK){

		extern void usrAppHomepageThemeType_Set(const uint8_t themeType_flg, bool recommendBpic_if, bool nvsRecord_IF);
		extern void usrAppHomepageBgroundPicOrg_Set(const uint8_t picIst, bool nvsRecord_IF, bool refresh_IF);

		usrAppHomepageThemeType_Set(dataTemp_homepageThemeParam.bGround_keyTheme_ist, false, false);
		usrAppHomepageBgroundPicOrg_Set(dataTemp_homepageThemeParam.bGround_picOrg_ist, false, false);
		
		ESP_LOGI(TAG,"nvs_data homepage theme param read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data homepage theme param not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --边框氛围灯相关运行参数*/
	dataLength = sizeof(stt_devAtmosLightRunningParam);
	err = nvs_get_blob(handle, DATA_DEV_DEVATMOSRUNPARAM, &dataTemp_devAtmosRunningParam, &dataLength);
	if(err == ESP_OK){

		devAtmosLight_runningParam_set(&dataTemp_devAtmosRunningParam, false);
		
		ESP_LOGI(TAG,"nvs_data atmosphereLight running param read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data atmosphereLight running param not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --温度人工校准数据*/
	dataLength = sizeof(uint8_t);
	err = nvs_get_blob(handle, DATA_DEV_TEMPRATURE_CALPARAM, &dataTemp_devTempratureCalParam, &dataLength);
	if(err == ESP_OK){	

		devTempratureSensor_dataCal_set(dataTemp_devTempratureCalParam, false);
		
		ESP_LOGI(TAG,"nvs_data devTempratureCal read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devTempratureCal not found, maybe first running, err:0x%04X.\n", err);
	}


	/*掉电数据更新 --屏幕校准使能*/
	dataLength = sizeof(char);
	err = nvs_get_blob(handle, DATA_DEVDRVIPT_RECALIBRAPARAM, &dataTemp_devDrviptRecalibraParam, &dataLength);
	if(err == ESP_OK){	

		devIptdrvParam_recalibration = dataTemp_devDrviptRecalibraParam;
		if(dataTemp_devDrviptRecalibraParam == 'Y'){ //当此生效，生效后参数复位

			dataTemp_devDrviptRecalibraParam = 'N';
			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEVDRVIPT_RECALIBRAPARAM, &dataTemp_devDrviptRecalibraParam, sizeof(char)) );
		}
		
		ESP_LOGI(TAG,"nvs_data devDrvInpt paramRecalibra read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data  devDrvInpt paramRecalibra not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --屏幕运行相关参数*/
	dataLength = sizeof(stt_devScreenRunningParam);
	err = nvs_get_blob(handle, DATA_DEVSCREEN_CONFIGPARAM, &dataTemp_devScreenRunningConfigParam, &dataLength);
	if(err == ESP_OK){	

		devScreenDriver_configParam_set(&dataTemp_devScreenRunningConfigParam, false);
		
		ESP_LOGI(TAG,"nvs_data devScreen runningParam read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data devScreen runningParam not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --MQTT远程连接配置数据*/
	dataLength = sizeof(stt_mqttCfgParam);
	err = nvs_get_blob(handle, DATA_DTMQTT_CFGPARAM, &dataTemp_dtMqttParamInfo, &dataLength);
	if(err == ESP_OK){	

		mqttRemoteConnectCfg_paramSet(&dataTemp_dtMqttParamInfo, false);
		
		ESP_LOGI(TAG,"nvs_data mqtt cfgInfoParam read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data mqtt cfgInfoParam not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --homeassistant server配置数据*/
	dataLength = sizeof(stt_mqttExServerCfgParam);
	err = nvs_get_blob(handle, DATA_HASERVER_CFGPARAM, &dataTemp_haServerParamInfo, &dataLength);
	if(err == ESP_OK){	

		mqttHaMqttServer_paramSet(&dataTemp_haServerParamInfo, false);
		
		ESP_LOGI(TAG,"nvs_data ha server cfgInfoParam read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data ha server cfgInfoParam not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --路由器配置数据*/
	dataLength = sizeof(stt_routerCfgInfo);
	err = nvs_get_blob(handle, DATA_ROUTER_CFG_INFO, &dataTemp_routerCfgInfoParam, &dataLength);
	if(err == ESP_OK){	

		currentRouterCfgInfo_paramSet(&dataTemp_routerCfgInfoParam, false);
		
		ESP_LOGI(TAG,"nvs_data router cfgInfoParam read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data router cfgInfoParam not found, maybe first running, err:0x%04X.\n", err);
	}

	/*掉电数据更新 --疫情国家选择数据*/
	dataLength = sizeof(uint8_t);
	err = nvs_get_blob(handle, DATA_EPIDCYLOCATION, &dataTemp_epidCyAbbreIst, &dataLength);
	if(err == ESP_OK){	

		dispApplication_epidCyLocation_set(dataTemp_epidCyAbbreIst, false);
		
		ESP_LOGI(TAG,"nvs_data epidCyLocation read success.\n");
		
	}else{

		ESP_LOGI(TAG,"nvs_data epidCyLocation not found, maybe first running, err:0x%04X.\n", err);
	}

//	/*掉电数据更新 --home界面背景图*/
//	if(dataPtr_bGroundPic){

//		dataLength = sizeof(uint8_t) * GUI_BUSSINESS_HOME_BGROUND_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE;
//		err = nvs_get_blob(handle, DATA_DEV_GUIHOME_BGROUDPIC, dataPtr_bGroundPic, &dataLength);
//		if(err == ESP_OK){

//			ESP_LOGI(TAG,"nvs_data guiHome bGroundPic info read success.\n");
//			
//		}else{

//			ESP_LOGI(TAG,"nvs_data guiHome bGroundPic info not found, maybe first running, err:0x%04X.\n", err);
//		}
//	}	

//	/*掉电数据更新 --home界面按键文件图片<pic_A>*/
//	if(dataPtr_btnTextImg_sw_A){

//		dataLength = sizeof(uint8_t) * GUI_BUSSINESS_HOME_BTNTEXT_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE;
//		err = nvs_get_blob(handle, DATA_DEV_GUIHOMEBTNTEXTPIC_A, dataPtr_btnTextImg_sw_A, &dataLength);
//		if(err == ESP_OK){

//			ESP_LOGI(TAG,"nvs_data guiHome btnTextImg_sw_A info read success.\n");
//			
//		}else{

//			ESP_LOGI(TAG,"nvs_data guiHome btnTextImg_sw_A info not found, maybe first running, err:0x%04X.\n", err);
//		}
//	}

//	/*掉电数据更新 --home界面按键文件图片<pic_B>*/
//	if(dataPtr_btnTextImg_sw_B){

//		dataLength = sizeof(uint8_t) * GUI_BUSSINESS_HOME_BTNTEXT_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE;
//		err = nvs_get_blob(handle, DATA_DEV_GUIHOMEBTNTEXTPIC_B, dataPtr_btnTextImg_sw_B, &dataLength);
//		if(err == ESP_OK){

//			ESP_LOGI(TAG,"nvs_data guiHome btnTextImg_sw_B info read success.\n");
//			
//		}else{

//			ESP_LOGI(TAG,"nvs_data guiHome btnTextImg_sw_B info not found, maybe first running, err:0x%04X.\n", err);
//		}
//	}

//	/*掉电数据更新 --home界面按键文件图片<pic_C>*/
//	if(dataPtr_btnTextImg_sw_C){

//		dataLength = sizeof(uint8_t) * GUI_BUSSINESS_HOME_BTNTEXT_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE;
//		err = nvs_get_blob(handle, DATA_DEV_GUIHOMEBTNTEXTPIC_C, dataPtr_btnTextImg_sw_C, &dataLength);
//		if(err == ESP_OK){

//			ESP_LOGI(TAG,"nvs_data guiHome btnTextImg_sw_C info read success.\n");
//			
//		}else{

//			ESP_LOGI(TAG,"nvs_data guiHome btnTextImg_sw_C info not found, maybe first running, err:0x%04X.\n", err);
//		}
//	}

	/*掉电数据更新 --磁保持继电器测试数据*/
#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SLAVE_MCU)
 #if(DRVMETHOD_BY_SLAVE_MCU_RELAY_TEST == 1)
	
	 dataLength = sizeof(stt_RMTest_pRcd);
	 err = nvs_get_blob(handle, DATA_RELAYMAG_TESTPARAM, &dataTemp_paramMagRelayTest, &dataLength);
	 if(err == ESP_OK){  
	 
		 debugTestMagRelay_paramSet(&dataTemp_paramMagRelayTest, false);
		 
		 ESP_LOGI(TAG,"nvs_data relayMag testData read success.\n");
		 
	 }else{
	 
		 ESP_LOGI(TAG,"nvs_data relayMag testData not found, maybe first running, err:0x%04X.\n", err);
	 }
 #endif		
#endif

    nvs_close(handle);
	ESP_ERROR_CHECK( nvs_flash_deinit_partition(NVS_DATA_L8_PARTITION_NAME));

	devCurrentRunningFlg &= ~DEV_RUNNING_FLG_BIT_DELAY; //延时模式所有数据掉电清空，包括运行标志
}

void devSystemInfoLocalRecord_save(enum_dataSaveObj obj, void *dataSave){

    nvs_handle handle;

	xSemaphoreTake(xSph_usrAppNvsOpreat, 1000 / portTICK_RATE_MS);

	ESP_ERROR_CHECK( nvs_flash_init_partition(NVS_DATA_L8_PARTITION_NAME));
    ESP_ERROR_CHECK( nvs_open_from_partition(NVS_DATA_L8_PARTITION_NAME, NVS_DATA_SYSINFO_RECORD, NVS_READWRITE, &handle) );

	switch(obj){

		case saveObj_swStatus:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_SWSTATUS, dataSave, sizeof(stt_devDataPonitTypedef)) );
		
		}break;

		case saveObj_infoTimer_normal:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_INFO_TIMER_NORMAL, dataSave, sizeof(usrApp_trigTimer) * USRAPP_VALDEFINE_TRIGTIMER_NUM) );

		}break;

		case saveObj_infoTimer_nightMode:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_INFO_TIMER_NIGHTMODE, dataSave, sizeof(usrApp_trigTimer) * 2) );

		}break;

		case saveObj_infoTimer_greenMode:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_INFO_TIMER_GREENMODE, dataSave, sizeof(uint8_t) * 2) );

		}break;

		case saveObj_devRunning_flg:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_INFO_DEVRUNNINGFLG, dataSave, sizeof(uint16_t)) );

		}break;

		case saveObj_devTimeZone:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_TIMEZONE, dataSave, sizeof(stt_timeZone)) );

		}break;

		case saveObj_devEelcSum:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_ELECSUM, dataSave, sizeof(uint32_t)) );

		}break;

		case saveObj_devTypeDef:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_TYPEDEF, dataSave, sizeof(devTypeDef_enum)) );

		}break;

		case saveObj_devStatusRecordIF:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_STATUS_RECORD_IF, dataSave, sizeof(stt_devStatusRecord)) );

		}break;

		case saveObj_routerBssidRcd:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_ROUTER_BSSIDRCD, dataSave, sizeof(uint8_t) * DEVICE_MAC_ADDR_APPLICATION_LEN) );

		}break;

		case saveObj_routerBssid:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_ROUTER_BSSID, dataSave, sizeof(uint8_t) * DEVICE_MAC_ADDR_APPLICATION_LEN) );

		}break;

		case saveObj_mutualCtrlInfo:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_MUTUALCTRL_INFO, dataSave, sizeof(stt_devMutualGroupParam) * DEVICE_MUTUAL_CTRL_GROUP_NUM) );

		}break;

		case saveObj_devDriver_linkageConfigParam_set:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_LINKAGE_CONFIG, dataSave, sizeof(stt_paramLinkageConfig)) );

		}break;

		case saveObj_devGuiBussinessHome_themeParam:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_GUIHOMETHEMEPARAM, dataSave, sizeof(stt_bGroundThemeParam)) );

		}break;

		case saveObj_devDriver_atmosLightRunningParam_set:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_DEVATMOSRUNPARAM, dataSave, sizeof(stt_devAtmosLightRunningParam)) );
		
		}break;

		case saveObj_devDriver_tempratureCal_valDat:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_TEMPRATURE_CALPARAM, dataSave, sizeof(uint8_t)) );
		
		}break;

		case saveObj_devDriver_iptRecalibration_set:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEVDRVIPT_RECALIBRAPARAM, dataSave, sizeof(char)) );

		}break;

		case saveObj_devDriver_screenRunningParam_set:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEVSCREEN_CONFIGPARAM, dataSave, sizeof(stt_devScreenRunningParam)) );

		}break;

		case saveObj_devGuiBussinessHome_btnTextDisp:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_GUIHOMEBTNTEXTDISP, dataSave, sizeof(stt_dataDisp_guiBussinessHome_btnText)) );

		}break;

		case saveObj_devGuiBussinessHome_btnIconDisp:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_GUIHOMEBTNICONDISP, dataSave, sizeof(uint8_t) * GUIBUSSINESS_CTRLOBJ_MAX_NUM) );

		}break;

		case saveObj_devCurtain_runningParam:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEVCURTAIN_RUNNINGPARAM, dataSave, sizeof(stt_devCurtain_runningParam)) );
		
		}break;

		case saveObj_devInfrared_paramDats:{

			stt_devInfraredNvsOpreatParam paramTemp = {0};

			memcpy(&paramTemp, dataSave, sizeof(stt_devInfraredNvsOpreatParam));

			ESP_ERROR_CHECK( nvs_set_blob( handle, paramTemp.nvsKeyWord, &paramTemp.dats, sizeof(stt_infraredSwitchData_nvsOpreat)) );

		}break;

		case saveObj_devInfrared_timerUpIstTab:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_INFO_TIMER_IRDATA, dataSave, sizeof(uint8_t) * USRAPP_VALDEFINE_TRIGTIMER_NUM) );

		}break;

		case saveObj_devThermostatExSw_statusDats:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEVTHERMOSTATEX_SWPARAM, dataSave, sizeof(uint8_t)) );

		}break;

		case saveObj_devScenario_paramDats_0:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEVSCENARIO_DATA_PARAM_0, dataSave, sizeof(stt_scenarioSwitchData_nvsOpreat)) );

		}break;

		case saveObj_devScenario_paramDats_1:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEVSCENARIO_DATA_PARAM_1, dataSave, sizeof(stt_scenarioSwitchData_nvsOpreat)) );

		}break;

		case saveObj_devScenario_paramDats_2:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEVSCENARIO_DATA_PARAM_2, dataSave, sizeof(stt_scenarioSwitchData_nvsOpreat)) );

		}break;

		case saveObj_dtMqttCfgParam:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DTMQTT_CFGPARAM, dataSave, sizeof(stt_mqttCfgParam)) );

		}break;

		case saveObj_dtHaMqttCfgParam:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_HASERVER_CFGPARAM, dataSave, sizeof(stt_mqttExServerCfgParam)) );

		}break;

		case saveObj_routerConfigInfo:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_ROUTER_CFG_INFO, dataSave, sizeof(stt_routerCfgInfo)) );

		}break;

		case saveObj_devHeater_customTimeParam:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEVHEATER_CUSTOMTIME, dataSave, sizeof(uint32_t)) );

		}break;

		case saveObj_greenMode_usrCfg:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_GREENMODE_USRCFG, dataSave, sizeof(stt_gModeOpFunc)) );

		}break;

		case saveObj_epidCyLocation:{

			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_EPIDCYLOCATION, dataSave, sizeof(uint8_t)) );

		}break;

		case saveObj_devGuiBussinessHome_bGroundPic:{

//			char 	 	   nvsStr_temp[16] = {0};
//			const uint32_t loopPeriod = GUI_BUSSINESS_HOME_BGROUND_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE / 3072;
//				  uint32_t loopCount  = 0;

//			for(loopCount = 0; loopCount < loopPeriod; loopCount ++){

//				snprintf(nvsStr_temp, 16, "%s%02X", DATA_DEV_GUIHOME_BGROUDPIC, loopCount);
//				ESP_ERROR_CHECK( nvs_set_blob( handle, nvsStr_temp, &dataSave[3072 * loopCount], sizeof(uint8_t) * 3072) );
//			}

//			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_GUIHOME_BGROUDPIC, dataSave, sizeof(uint8_t) * GUI_BUSSINESS_HOME_BGROUND_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE) );

		}break;

//		case saveObj_devGuiBussinessHome_btnTextPic_A:{

//			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_GUIHOMEBTNTEXTPIC_A, dataSave, sizeof(uint8_t) * GUI_BUSSINESS_HOME_BTNTEXT_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE) );
//		
//		}break;

//		case saveObj_devGuiBussinessHome_btnTextPic_B:{

//			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_GUIHOMEBTNTEXTPIC_B, dataSave, sizeof(uint8_t) * GUI_BUSSINESS_HOME_BTNTEXT_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE) );

//		}break;

//		case saveObj_devGuiBussinessHome_btnTextPic_C:{

//			ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_DEV_GUIHOMEBTNTEXTPIC_C, dataSave, sizeof(uint8_t) * GUI_BUSSINESS_HOME_BTNTEXT_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE) );

//		}break;

#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SLAVE_MCU)
 #if(DRVMETHOD_BY_SLAVE_MCU_RELAY_TEST == 1)
		
		 case saveObj_paramMagRelayTest:{
		 
			 ESP_ERROR_CHECK( nvs_set_blob( handle, DATA_RELAYMAG_TESTPARAM, dataSave, sizeof(stt_RMTest_pRcd)) );
		 
		 }break;
 #endif
#endif

		default:break;
	}

    ESP_ERROR_CHECK( nvs_commit(handle) );

	nvs_close(handle);
	ESP_ERROR_CHECK( nvs_flash_deinit_partition(NVS_DATA_L8_PARTITION_NAME));

	xSemaphoreGive(xSph_usrAppNvsOpreat);
}

void functionSpecialUsrApp_floatToHex(stt_devElecsumParam2Hex *param, float fData){

	const float decimal_prtCoefficient = 100.0F; //小数计算偏移倍数 --100倍对应十进制两位

	uint16_t dataInteger_prt = (uint16_t)fData & 0xFFFF;
	uint8_t dataDecimal_prt = (uint8_t)((fData - (float)dataInteger_prt) * decimal_prtCoefficient);

	//只可能为正数，不做负数处理

	param->integer_h8bit = (uint8_t)((dataInteger_prt & 0xFF00) >> 8);
	param->integer_l8bit = (uint8_t)((dataInteger_prt & 0x00FF) >> 0);
	param->decimal_8bit = dataDecimal_prt;
}

void functionEndianSwap(uint8_t *pData, int startIndex, int length){

    int i = 0,cnt = 0,end = 0,start = 0;
    cnt = length / 2;
    start = startIndex;
    end  = startIndex + length - 1;
    uint8_t tmp = 0;
	
    for (i = 0; i < cnt; i++)
    {
        tmp            = pData[start+i];
        pData[start+i] = pData[end-i];
        pData[end-i]   = tmp;
    }
}

void functionStrPsdToHidden(char *psdStr){

	uint8_t strLen_temp = 0;

	strLen_temp = strlen(psdStr);
	if(strLen_temp > 3)memset(&psdStr[2], '*', strLen_temp - 3);
	else memset(&psdStr[1], '*', strLen_temp - 1);
}


