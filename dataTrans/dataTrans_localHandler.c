#include "dataTrans_localHandler.h"

#include "os.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_freertos_hooks.h"

#include "dataTrans_meshUpgrade.h"

#include "bussiness_timerSoft.h"
#include "bussiness_timerHard.h"

#include "devDriver_manage.h"

extern stt_nodeDev_hbDataManage *listHead_nodeDevDataManage;
extern stt_nodeDev_detailInfoManage *listHead_nodeInfoDetailManage;
extern stt_nodeObj_listManageDevCtrlBase *listHead_nodeCtrlObjBlockBaseManage;

extern EventGroupHandle_t xEventGp_devApplication;

extern uint8_t listNum_nodeDevDataManage;

extern void lvGui_wifiConfig_bussiness_configComplete_tipsTrig(void);
extern void lvGui_usrAppBussinessRunning_block(uint8_t iconType, const char *strTips, uint8_t timeOut);

static const uint8_t L8_meshDataCmdLen = 1; //L8 mesh内部数据传输 头命令 长度

static const char *TAG = "lanbon_L8 - meshDataHandler";

static TimerHandle_t timerHandler_dataTransPeriod = NULL;

static uint16_t usrApp_heartBeathold_counter = 0;

void dataHandler_devNodeMeshData(const uint8_t *src_addr, const mlink_httpd_type_t *type,
                       					 const void *dataRx, size_t dataLen){

	extern void lvGuiLinkageConfig_devGraphCtrlBlock_listNodeUnitRefresh(uint8_t devMac[MWIFI_ADDR_LEN], uint8_t devState);

	const uint8_t MACADDR_INSRT_START_CMDCONTROL		 = DEV_HEX_PROTOCOL_APPLEN_CONTROL; 	 //MAC地址起始下标:普通控制
	const uint8_t MACADDR_INSRT_START_CMDTIMERSET		 = DEV_HEX_PROTOCOL_APPLEN_TIMERSET;	 //MAC地址起始下标:普通定时设置
	const uint8_t MACADDR_INSRT_START_CMDDELAYSET		 = DEV_HEX_PROTOCOL_APPLEN_DELAYSET;	 //MAC地址起始下标:延时设置
	const uint8_t MACADDR_INSRT_START_CMDGREENMODESET	 = DEV_HEX_PROTOCOL_APPLEN_GREENMODESET; //MAC地址起始下标:绿色模式设置
	const uint8_t MACADDR_INSRT_START_CMDNIGHTMODESET	 = DEV_HEX_PROTOCOL_APPLEN_NIGHTMODESET; //MAC地址起始下标:夜间模式设置

	const uint8_t devMacAddr_root[DEVICE_MAC_ADDR_APPLICATION_LEN] = MWIFI_ADDR_ROOT;

	uint8_t *dataRcv_temp = (uint8_t *)dataRx;

	uint8_t dataRespond_temp[DEVMQTT_MESH_DATAREQ_TEMPLEN] = {0};
	uint16_t dataRespond_tempLen = 0;
	uint8_t devMacBuff[DEVICE_MAC_ADDR_APPLICATION_LEN] = {0};

	switch(dataRcv_temp[0]){

		/*case R2N*--------------------------------------------------------------------------*///主机转发至子设备处理
		case L8DEV_MESH_CMD_CONTROL:{

			stt_devDataPonitTypedef dataVal_set = {0};
			
			memcpy(&dataVal_set, &dataRcv_temp[1], sizeof(uint8_t));
			currentDev_dataPointSet(&dataVal_set, true, true, true, true, true);

//			printf(">>>>>>>>>>>>>>>ctrl cmd rcv:%02X.\n", dataRcv_temp[1]);

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdControl.\n");

		}break;

		case L8DEV_MESH_CMD_DEVLOCK:{

			uint16_t devRunningFlg_temp = currentDevRunningFlg_paramGet();
			
			(dataRcv_temp[1])?
				(devRunningFlg_temp |= DEV_RUNNING_FLG_BIT_DEVLOCK):
				(devRunningFlg_temp &= (~DEV_RUNNING_FLG_BIT_DEVLOCK));
			
			currentDevRunningFlg_paramSet(devRunningFlg_temp, true);

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdDevLock.\n");

		}break;
		
		case L8DEV_MESH_CMD_SET_TIMER:{

			uint8_t *dataHandle = &dataRcv_temp[1];

			usrAppActTrigTimer_paramSet((usrApp_trigTimer *)&dataHandle[0], true);

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED) //红外定时数据另外存储
		
			const uint8_t dataIst = DEV_HEX_PROTOCOL_APPLEN_TIMERSET;

			if(dataLen >= (dataIst + USRAPP_VALDEFINE_TRIGTIMER_NUM)){

				devDriverBussiness_infraredSwitch_timerUpTrigIstTabSet((uint8_t *)(&dataHandle[dataIst]), true);
				
//				printf("ir timerUpIstTab get:%d, %d, %d, %d, %d, %d, %d, %d.\n", (int)dataHandle[dataIst + 0],
//																				 (int)dataHandle[dataIst + 1],
//																				 (int)dataHandle[dataIst + 2],
//																				 (int)dataHandle[dataIst + 3],
//																				 (int)dataHandle[dataIst + 4],
//																				 (int)dataHandle[dataIst + 5],
//																				 (int)dataHandle[dataIst + 6],
//																				 (int)dataHandle[dataIst + 7]);
			}
#endif

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdTimerSet.\n");

		}break;	
		
		case L8DEV_MESH_CMD_SET_DELAY:{

			usrAppParamSet_devDelayTrig(&dataRcv_temp[1]);

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdDelaySet.\n");

		}break;		
		
		case L8DEV_MESH_CMD_SET_GREENMODE:{

			usrAppParamSet_devGreenMode(&dataRcv_temp[1], true);

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdGreenmodeSet.\n");

		}break;

		case L8DEV_MESH_CMD_SET_NIGHTMODE:{

			uint8_t dataTemp[6] = {0};
			uint8_t *dataHandle = &dataRcv_temp[1];

			//数据处理调换
			dataTemp[0] = dataHandle[0]; //全天
			dataTemp[1] = dataHandle[2]; //时段1：时
			dataTemp[2] = dataHandle[3]; //时段1：分
			dataTemp[3] = dataHandle[1]; //非全天
			dataTemp[4] = dataHandle[4]; //时段2：时
			dataTemp[5] = dataHandle[5]; //时段2：分
			usrAppNightModeTimeTab_paramSet((usrApp_trigTimer *)&dataTemp, true);

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdNightmodeSet.\n");

		}break;

		case L8DEV_MESH_CMD_SET_MUTUALCTRL:{
		
			uint8_t deviceMutualNum = (uint8_t)dataRcv_temp[1];
			uint8_t loop = 0;
			uint8_t cpyist = 0;
			
			struct stt_devMutualCtrlParam{
			
				uint8_t opreat_bit;
				uint8_t mutualGroup_insert[DEVICE_MUTUAL_CTRL_GROUP_NUM];
				uint8_t devMac[MWIFI_ADDR_LEN];
				
			}mutualGroupParamRcv_temp = {0};
			stt_devMutualGroupParam mutualGroupParamSet_temp = {0},
									mutualGroupParamSet_empty = {0};
			
			uint8_t dataParamUintTemp_length = sizeof(struct stt_devMutualCtrlParam);

			for(loop = 0; loop < deviceMutualNum; loop ++){ //互控设备Maclist获取

				memset(&mutualGroupParamRcv_temp, 0, dataParamUintTemp_length);
				memcpy(&mutualGroupParamRcv_temp, 
					   &(dataRcv_temp[loop * dataParamUintTemp_length + 2]), 
					   dataParamUintTemp_length);

				for(uint8_t loopA = 0; loopA < DEVICE_MUTUAL_CTRL_GROUP_NUM; loopA ++){

					if(mutualGroupParamRcv_temp.opreat_bit & (1 << loopA)){

						if((mutualGroupParamRcv_temp.mutualGroup_insert[loopA] != DEVICE_MUTUALGROUP_INVALID_INSERT_A) &&
						   (mutualGroupParamRcv_temp.mutualGroup_insert[loopA] != DEVICE_MUTUALGROUP_INVALID_INSERT_B)){

							mutualGroupParamSet_temp.mutualCtrlGroup_insert = mutualGroupParamRcv_temp.mutualGroup_insert[loopA]; //单次设置之内，组号非无效，必相同
							memcpy(&(mutualGroupParamSet_temp.mutualCtrlDevMacList[(cpyist ++) * MWIFI_ADDR_LEN]),
								   mutualGroupParamRcv_temp.devMac,
								   MWIFI_ADDR_LEN);

							break;
						}
					}
				}		
			}
			mutualGroupParamSet_temp.mutualCtrlDevNum = cpyist;	

			esp_wifi_get_mac(ESP_IF_WIFI_STA, devMacBuff);
			for(loop = 0; loop < deviceMutualNum; loop ++){ //互控设置参数自身存储更新 或 分发

				memset(&mutualGroupParamRcv_temp, 0, dataParamUintTemp_length);
				memcpy(&mutualGroupParamRcv_temp, 
					   &(dataRcv_temp[loop * dataParamUintTemp_length + 2]), 
					   dataParamUintTemp_length);

				if(!memcmp(devMacBuff, mutualGroupParamRcv_temp.devMac, MWIFI_ADDR_LEN)){

					for(uint8_t loopA = 0; loopA < DEVICE_MUTUAL_CTRL_GROUP_NUM; loopA ++){

						if(mutualGroupParamRcv_temp.opreat_bit & (1 << loopA)){

							if((mutualGroupParamRcv_temp.mutualGroup_insert[loopA] != DEVICE_MUTUALGROUP_INVALID_INSERT_A) &&
							   (mutualGroupParamRcv_temp.mutualGroup_insert[loopA] != DEVICE_MUTUALGROUP_INVALID_INSERT_B)){

								devMutualCtrlGroupInfo_Set(&mutualGroupParamSet_temp, 1 << loopA, true);
							}
							else
							{
								devMutualCtrlGroupInfo_Set(&mutualGroupParamSet_empty, 1 << loopA, true);
							}
						}
					}
				}
			}
		
			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdMutualCtrlSet.\n");
		
		}break; 

		case L8DEV_MESH_CMD_EXT_PARAM_SET:{

			currentDev_extParamSet(&dataRcv_temp[1]);
			
			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdExtParamSet.\n");

		}break;

		case L8DEV_MESH_CMD_SCENARIO_SET:{

			const uint8_t *dataRcv_kernel = &dataRcv_temp[1];

			uint8_t scenarioParam_ist = dataRcv_kernel[1];
			stt_scenarioSwitchData_nvsOpreat *scenarioParamData = nvsDataOpreation_devScenarioParam_get(scenarioParam_ist);
			uint8_t dataParamHalf_flg = dataRcv_kernel[2];
			
			if(dataParamHalf_flg == 0xA1){
			
				memset(scenarioParamData, 0, sizeof(stt_scenarioSwitchData_nvsOpreat));
				scenarioParamData->dataRef.scenarioDevice_sum = dataRcv_kernel[0];
				scenarioParamData->dataRef.scenarioInsert_num = scenarioParam_ist;
				if(scenarioParamData->dataRef.scenarioDevice_sum > DEVSCENARIO_NVSDATA_HALFOPREAT_NUM)
					memcpy(scenarioParamData->dataHalf_A, &(dataRcv_kernel[9]), sizeof(stt_scenarioUnitOpreatParam) * DEVSCENARIO_NVSDATA_HALFOPREAT_NUM);
				else
					memcpy(scenarioParamData->dataHalf_A, &(dataRcv_kernel[9]), sizeof(stt_scenarioUnitOpreatParam) * scenarioParamData->dataRef.scenarioDevice_sum);
			
				devDriverBussiness_scnarioSwitch_dataParam_save(scenarioParamData);
			}
			else
			if(dataParamHalf_flg == 0xA2){
			
				uint8_t scenarioDeviceSum_reserve = scenarioParamData->dataRef.scenarioDevice_sum - DEVSCENARIO_NVSDATA_HALFOPREAT_NUM;
			
				memcpy(scenarioParamData->dataHalf_B, &(dataRcv_kernel[9]), sizeof(stt_scenarioUnitOpreatParam) * scenarioDeviceSum_reserve);
			
				devDriverBussiness_scnarioSwitch_dataParam_save(scenarioParamData);
			}
			
			printf("scenario set cmd coming! sum:%d, ist:%d, part:%02X.\n", scenarioParamData->dataRef.scenarioDevice_sum,
																		 	scenarioParamData->dataRef.scenarioInsert_num,
																		 	dataParamHalf_flg);
			
			os_free(scenarioParamData);

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdScenarioSet.\n");

		}break;

		case L8DEV_MESH_CMD_SCENARIO_CTRL:{

			stt_devDataPonitTypedef dataPointScenCtrl_temp = {0};
			
			memcpy(&dataPointScenCtrl_temp, &dataRcv_temp[1], sizeof(stt_devDataPonitTypedef));
	
			switch(currentDev_typeGet()){

				case devTypeDef_thermostat:{

					stt_devDataPonitTypedef devDataPointCurrent_temp = {0};

					currentDev_dataPointGet(&devDataPointCurrent_temp); //只改运行使能参数，其他参数不变
					dataPointScenCtrl_temp.devType_thermostat.devThermostat_nightMode_en =\
					devDataPointCurrent_temp.devType_thermostat.devThermostat_nightMode_en;
					dataPointScenCtrl_temp.devType_thermostat.devThermostat_tempratureTarget =\
					devDataPointCurrent_temp.devType_thermostat.devThermostat_tempratureTarget;

				}break;

				case devTypeDef_thermostatExtension:{

					stt_devDataPonitTypedef devDataPointCurrent_temp = {0};
					uint8_t devThermostatExSwStatus_temp = 0;

					memcpy(&devThermostatExSwStatus_temp, &dataPointScenCtrl_temp, sizeof(uint8_t));
					currentDev_dataPointGet(&devDataPointCurrent_temp); //只改运行使能参数，其他参数不变

					/*bit0 -恒温器是否开启*/
					(devThermostatExSwStatus_temp & (1 >> 0))?
						(dataPointScenCtrl_temp.devType_thermostat.devThermostat_running_en = 1):
						(dataPointScenCtrl_temp.devType_thermostat.devThermostat_running_en = 0);
					dataPointScenCtrl_temp.devType_thermostat.devThermostat_nightMode_en =\
					devDataPointCurrent_temp.devType_thermostat.devThermostat_nightMode_en;
					dataPointScenCtrl_temp.devType_thermostat.devThermostat_tempratureTarget =\
					devDataPointCurrent_temp.devType_thermostat.devThermostat_tempratureTarget;

					/*bit2 -第二位开关值
					  bit1 -第一位开关值*/
					devThermostatExSwStatus_temp >>= 1;
					devThermostatExSwStatus_temp &= 0x03;
					devDriverBussiness_thermostatSwitch_exSwitchParamSet(devThermostatExSwStatus_temp);

				}break;

				default:break;
			}

			currentDev_dataPointSet(&dataPointScenCtrl_temp, true, false, false, false, false);
	
			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdScenarioCtrl.\n");

		}break;

		case L8DEV_MESH_CMD_FWARE_CHECK:{

			extern void appUiElementSet_upgradeAvailable(bool val);

			uint8_t firewareVersionNum = dataRcv_temp[1];

			if(firewareVersionNum > L8_DEVICE_VERSION){

				appUiElementSet_upgradeAvailable(true);
			}
			else
			{
				appUiElementSet_upgradeAvailable(false);
			}

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdFirewareCheck.\n");

		}break;

		case L8DEV_MESH_CMD_DETAILDEVINFO_NOTICE:{

			const uint8_t dataUnitIst_start = 1,
						  dataUnitLen = MWIFI_ADDR_LEN + sizeof(stt_devInfoDetailUpload_2Root);

			uint8_t *dataHandle = &dataRcv_temp[1];

			uint16_t dataIst_temp = 0;
			uint8_t devNum = dataHandle[0];
			uint8_t loop = 0;
			stt_devInfoDetailUpload_2Root *detailInfoUnit_ptr = NULL;
			stt_nodeObj_listManageDevCtrlBase gNode_new = {0};

			printf("devInfo list rcv, devNum:%d.\n", devNum);

			for(loop = 0; loop < devNum; loop ++){

				memset(&gNode_new, 0, sizeof(stt_nodeObj_listManageDevCtrlBase));
				dataIst_temp = dataUnitIst_start + dataUnitLen * loop;
				memcpy(gNode_new.nodeData.ctrlObj_devMac, &dataHandle[dataIst_temp], sizeof(uint8_t) * MWIFI_ADDR_LEN);
				dataIst_temp += MWIFI_ADDR_LEN;
				detailInfoUnit_ptr = (stt_devInfoDetailUpload_2Root *)&dataHandle[dataIst_temp];
				memcpy(&gNode_new.nodeData.ctrlObj_devType, &detailInfoUnit_ptr->devType, sizeof(uint8_t));
				memcpy(&gNode_new.nodeData.devStatusVal, &detailInfoUnit_ptr->devSelf_status, sizeof(uint8_t));
				memcpy(gNode_new.nodeData.objIcon_ist, detailInfoUnit_ptr->devSelf_iconIst, sizeof(uint8_t) * GUIBUSSINESS_CTRLOBJ_MAX_NUM);
				memcpy(gNode_new.nodeData.objCtrl_name, detailInfoUnit_ptr->devSelf_name, sizeof(char) * GUIBUSSINESS_CTRLOBJ_MAX_NUM * DEV_CTRLOBJ_NAME_DETAILUD_LEN);
//				printf("listGblock creat, listNum:%d.\n", lvglUsrApp_devCtrlBlockBaseManageList_nodeAdd(listHead_nodeCtrlObjBlockBaseManage, &gNode_new));
				lvglUsrApp_devCtrlBlockBaseManageList_nodeAdd(listHead_nodeCtrlObjBlockBaseManage, &gNode_new);
			}

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdDetailInfo list get.\n");

		}break;

		case L8DEV_MESH_CMD_DEVINFO_GET:{

			mdf_err_t ret				= MDF_OK;
			mwifi_data_type_t data_type = {
				
				.compression = true,
				.communicate = MWIFI_COMMUNICATE_UNICAST,
			};
			mlink_httpd_type_t type_L8mesh_cst = {
			
				.format = MLINK_HTTPD_FORMAT_HEX,
			};

			esp_wifi_get_mac(ESP_IF_WIFI_STA, devMacBuff);
			memcpy(&data_type.custom, &type_L8mesh_cst, sizeof(uint32_t));

			switch(dataRcv_temp[1]){
				
				case L8DEV_MESH_CMD_SET_TIMER:{

					uint8_t dataLoad_ist = 0;

					dataRespond_temp[dataLoad_ist] = L8DEV_MESH_CMD_SET_TIMER;
					dataLoad_ist += 1;
					usrAppActTrigTimer_paramGet((usrApp_trigTimer *)&dataRespond_temp[dataLoad_ist]);
					dataLoad_ist += MACADDR_INSRT_START_CMDTIMERSET;
					memcpy(&dataRespond_temp[dataLoad_ist], devMacBuff, DEVICE_MAC_ADDR_APPLICATION_LEN);
					dataLoad_ist += DEVICE_MAC_ADDR_APPLICATION_LEN;

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED) //红外定时数据另外获取并添加
					devDriverBussiness_infraredSwitch_timerUpTrigIstTabGet(&dataRespond_temp[dataLoad_ist]);
					dataLoad_ist += USRAPP_VALDEFINE_TRIGTIMER_NUM;
#endif
					
					dataRespond_tempLen = dataLoad_ist;

					ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdTimerGet.\n");
				
				}break;
				
				case L8DEV_MESH_CMD_SET_DELAY:{

					dataRespond_temp[0] = L8DEV_MESH_CMD_SET_DELAY;
					usrAppParamGet_devDelayTrig(&dataRespond_temp[1]);
					memcpy(&dataRespond_temp[MACADDR_INSRT_START_CMDDELAYSET + 1], devMacBuff, DEVICE_MAC_ADDR_APPLICATION_LEN);
					dataRespond_tempLen = MACADDR_INSRT_START_CMDDELAYSET + DEVICE_MAC_ADDR_APPLICATION_LEN + 1;

					ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdDelayGet.\n");
				
				}break;
				
				case L8DEV_MESH_CMD_SET_GREENMODE:{

					dataRespond_temp[0] = L8DEV_MESH_CMD_SET_GREENMODE;
					usrAppParamGet_devGreenMode(&dataRespond_temp[1]);
					memcpy(&dataRespond_temp[MACADDR_INSRT_START_CMDGREENMODESET + 1], devMacBuff, DEVICE_MAC_ADDR_APPLICATION_LEN);
					dataRespond_tempLen = MACADDR_INSRT_START_CMDGREENMODESET + DEVICE_MAC_ADDR_APPLICATION_LEN + 1;
				
					ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdGreenmodeGet.\n");
				
				}break;
				
				case L8DEV_MESH_CMD_SET_NIGHTMODE:{

					uint8_t dataTemp[6] = {0};

					dataRespond_temp[0] = L8DEV_MESH_CMD_SET_NIGHTMODE;
					usrAppNightModeTimeTab_paramGet((usrApp_trigTimer *)&dataTemp);
					dataRespond_temp[1] = dataTemp[0]; //全天
					dataRespond_temp[2] = dataTemp[3]; //非全天
					dataRespond_temp[3] = dataTemp[1]; //时段1：时
					dataRespond_temp[4] = dataTemp[2]; //时段1：分
					dataRespond_temp[5] = dataTemp[4]; //时段2：时
					dataRespond_temp[6] = dataTemp[5]; //时段2：分
						
					memcpy(&dataRespond_temp[MACADDR_INSRT_START_CMDNIGHTMODESET + 1], devMacBuff, DEVICE_MAC_ADDR_APPLICATION_LEN);
					dataRespond_tempLen = MACADDR_INSRT_START_CMDNIGHTMODESET + DEVICE_MAC_ADDR_APPLICATION_LEN + 1;

					ESP_LOGI(TAG, "<R2N>mesh data rcv, cmdNightmodeGet.\n");
				
				}break;

			}

			if(dataRespond_tempLen){

				ret = mwifi_write(devMacAddr_root, &data_type, dataRespond_temp, dataRespond_tempLen, true);
				MDF_ERROR_BREAK(ret != MDF_OK, "<%s> mqtt mwifi_root_write", mdf_err_to_name(ret));
			}

		}break;

		case L8DEV_MESH_CMD_CTRLOBJ_TEXTSET:{

			const uint8_t *dataRcv_kernel = &dataRcv_temp[1];

			uint8_t dataChg_temp[GUI_BUSSINESS_HOME_BTNTEXT_STR_UTF8_SIZE] = {0};
			char countryAbbreTemp[DATAMANAGE_LANGUAGE_ABBRE_MAXLEN] = {0};
			uint8_t cyFlg_temp = 0;
			uint8_t objNum =  dataRcv_kernel[6];
			uint8_t textDataLen = dataRcv_kernel[7];
			const uint8_t dataTextCodeIstStart = 14;
			
			memcpy(countryAbbreTemp, &(dataRcv_kernel[0]), sizeof(char) * DATAMANAGE_LANGUAGE_ABBRE_MAXLEN);
			cyFlg_temp = countryFlgGetByAbbre(countryAbbreTemp);
			memcpy(dataChg_temp, &(dataRcv_kernel[dataTextCodeIstStart]), textDataLen);

			usrAppHomepageBtnTextDisp_paramSet_specified(objNum, dataChg_temp, textDataLen, cyFlg_temp, true);

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmd btnTextSet.\n");

		}break;

		case L8DEV_MESH_CMD_CTRLOBJ_ICONSET:{

			const uint8_t *dataRcv_kernel = &dataRcv_temp[1];

			usrAppHomepageBtnIconNumDisp_paramSet((uint8_t *)dataRcv_kernel, true);

			printf("dataRcv:%02x, %02x, %02x.\n", dataRcv_kernel[0],
												  dataRcv_kernel[1],
												  dataRcv_kernel[2]);

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmd btnIconSet.\n");

		}break;

		case L8DEV_MESH_CMD_BGROUND_ISTSET:{

			extern void usrAppHomepageBgroundPicOrg_Set(const uint8_t picIst, bool nvsRecord_IF, bool refresh_IF);

			uint8_t *dataHandleTemp = &dataRcv_temp[1];
			uint8_t  bGroundPicIst = dataHandleTemp[0];

			usrAppHomepageBgroundPicOrg_Set((const uint8_t)bGroundPicIst, true, true);

		}break;

		case L8DEV_MESH_CMD_UISET_THEMESTYLE:{

			uint8_t uiThemeStyleFlg_set = dataRcv_temp[1];

			extern void usrAppHomepageThemeType_Set(const uint8_t themeType_flg, bool recommendBpic_if, bool nvsRecord_IF);
			
			usrAppHomepageThemeType_Set(uiThemeStyleFlg_set, true, true);

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmd guiHome themeSet.\n");

		}break;

		case L8DEV_MESH_CMD_NEIWORK_PARAM_CHG:{

			if(esp_mesh_get_layer() != MESH_ROOT){ //非根节点响应

				mwifi_config_t ap_config = {0x0};
				struct stt_paramWifiConfig{
				
					char router_ssid[32];
					char router_password[64];
					uint8_t router_bssid[6];
					
				}param_wifiConfig = {0};

				//wifi信息更改
				memcpy(&param_wifiConfig, &dataRcv_temp[1], sizeof(struct stt_paramWifiConfig));
				mdf_info_load("ap_config", &ap_config, sizeof(mwifi_config_t));
				memcpy(ap_config.router_ssid, param_wifiConfig.router_ssid, sizeof(char) * 32);
				memcpy(ap_config.router_password, param_wifiConfig.router_password, sizeof(char) * 64);
				memcpy(ap_config.router_bssid, param_wifiConfig.router_bssid, sizeof(uint8_t) * 6);
				memcpy(ap_config.mesh_id, param_wifiConfig.router_bssid, sizeof(uint8_t) * 6);
				mdf_info_save("ap_config", &ap_config, sizeof(mwifi_config_t));

				//倒计时重启触发
				usrApplication_systemRestartTrig(10);
			}

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmd wifi change.\n");

		}break;

		case L8DEV_MESH_CMD_SERVER_PARAM_CHG:{

				if(esp_mesh_get_layer() != MESH_ROOT){ //非根节点响应

					stt_mqttCfgParam dtMqttParamTemp = {
					
						.host_domain = MQTT_REMOTE_DATATRANS_PARAM_HOST_DEF,
						.port_remote = MQTT_REMOTE_DATATRANS_PARAM_PORT_DEF,
					};


					//自身IP等信息修改
					memcpy(&dtMqttParamTemp, &dataRcv_temp[1], sizeof(dtMqttParamTemp));
					mqttRemoteConnectCfg_paramSet(&dtMqttParamTemp, true);

					lvGui_usrAppBussinessRunning_block(2, "\nserver changed", 6);
				}
				
			}break;

		case L8DEV_MESH_CMD_MQTTUSR_PARAM_CHG:{

				if(esp_mesh_get_layer() != MESH_ROOT){ //非根节点响应

				}
				
			}break;

		case L8DEV_MESH_CMD_MQTTHA_PARAM_CHG:{
			
				if(esp_mesh_get_layer() != MESH_ROOT){ //非根节点响应
				
					stt_mqttExServerCfgParam dtMqttParamTemp = {0};
					
					//自身IP等信息修改
					memcpy(&dtMqttParamTemp, &dataRcv_temp[1], sizeof(stt_mqttExServerCfgParam));
					mqttHaMqttServer_paramSet(&dtMqttParamTemp, true);
					
					lvGui_usrAppBussinessRunning_block(2, "\nHA server changed", 6);	
				}

			}break;

		case L8DEV_MESH_CMD_SYSTEM_PARAM_CHG:{

			if(esp_mesh_get_layer() != MESH_ROOT){

				stt_timeZone timeZone_temp = {0};
				stt_mqttCfgParam mqttCfg_temp = {0};
				uint8_t *dataHandleTemp = &dataRcv_temp[1];
				char hostDmain_temp[MQTT_HOST_DOMAIN_STRLEN] = {0};

				//自身时区修改
				timeZone_temp.timeZone_H = dataHandleTemp[0];
				timeZone_temp.timeZone_M = dataHandleTemp[1];
				deviceParamSet_timeZone(&timeZone_temp, true);

				//自身mqtt配置信息修改
				sprintf(hostDmain_temp, "%d.%d.%d.%d", dataHandleTemp[2],
													   dataHandleTemp[3],
													   dataHandleTemp[4],
													   dataHandleTemp[5]);
				strcpy((char *)mqttCfg_temp.host_domain, hostDmain_temp);
				mqttCfg_temp.port_remote  = ((uint16_t)(dataHandleTemp[6]) << 8) & 0xff00;
				mqttCfg_temp.port_remote |= ((uint16_t)(dataHandleTemp[7]) << 0) & 0x00ff;
				if((dataHandleTemp[2] | dataHandleTemp[3] | dataHandleTemp[4] | dataHandleTemp[5]) != 0)
					mqttRemoteConnectCfg_paramSet(&mqttCfg_temp, true);
			}
			
			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmd sysParam cfg.\n");

		}break;

#if(SCREENSAVER_RUNNING_ENABLE == 1)

		case L8DEV_MESH_CMD_EPID_DATA_ISSUE:{

			if(esp_mesh_get_layer() == MESH_ROOT)break;

			struct stt_dataUint32tFormat{

				uint32_t sumData_cure;
				uint32_t sumData_confirmed;
				uint32_t sumData_deaths;
			}epidDataTemp = {0};

			uint8_t cyIst_temp = dataRcv_temp[1];
			uint8_t *dataHandleTemp = &dataRcv_temp[2];

			dispApplication_epidCyLocation_set(cyIst_temp, true);

			memcpy(&epidDataTemp, dataHandleTemp, sizeof(struct stt_dataUint32tFormat));
//			printf("mqtt epidemic notic from root, cure:0x%08X, confirmed:0x%08X, deaths:0x%08X.\n", epidDataTemp.sumData_cure,
//																									 epidDataTemp.sumData_confirmed,
//																									 epidDataTemp.sumData_deaths);

			epidemicDataRunningParam.epidData_cure = epidDataTemp.sumData_cure;
			epidemicDataRunningParam.epidData_confirmed = epidDataTemp.sumData_confirmed;
			epidemicDataRunningParam.epidData_deaths = epidDataTemp.sumData_deaths;

			screensaverDispAttrParam.flg_screensaverDataRefresh = 1;

			ESP_LOGI(TAG, "<R2N>mesh data rcv, epidemic data issue.\n");
		
		}break;

		case L8DEV_MESH_CMD_EPID_DATA_REQ:{ 

			if(esp_mesh_get_layer() != MESH_ROOT)break;

			epidemicDataRunningParam.reqTimeCounter =\
				epidemicDataRunningParam.reqTimePeriod - 3; //收到子节点疫情数据请求，触发提前获取疫情数据

			ESP_LOGI(TAG, "<N2R>mesh data rcv, epidemic data req.\n");

		}break;
#endif
		
		case L8DEV_MESH_CMD_DEVRESET:{}break;	

		case L8DEV_MESH_SYSTEMTIME_BOARDCAST:{

			if(esp_mesh_get_layer() != MESH_ROOT){ //根节点不理会广播时间数据

				deviceParamSet_timeZone((stt_timeZone *)&dataRcv_temp[L8_meshDataCmdLen], true); //时区设置
				usrAppDevCurrentSystemTime_paramSet((stt_localTime *)&dataRcv_temp[sizeof(stt_timeZone) + L8_meshDataCmdLen]); //时间设置

				ESP_LOGI(TAG, "<R2N>mesh data rcv, rootdev systime boardcast data.\n");
			}

		}break;

		case L8DEV_MESH_CMD_SPEQUERY_NOTICE:{
			
#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED)||\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_SOCKET)||\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_MOUDLE)||\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_RELAY_BOX)	
			
			devBeepTips_trig(4, 8, 100, 40, 2); //beeps提示
#else
			
			lvGui_wifiConfig_bussiness_configComplete_tipsTrig();
#endif 

			ESP_LOGI(TAG, "<R2N>mesh data rcv, cmd specified query notice.\n");

		}break;

		case L8DEV_MESH_CMD_ROOT_FIRST_CONNECT:{

			if(esp_mesh_get_layer() != MESH_ROOT){ //非根节点响应

				devDetailInfoUploadTrig(); //根节点首次上线，触发子节点详情更新上传

				ESP_LOGI(TAG, "<R2N>mesh data rcv, cmd first connect notice.\n");
			}
		
		}break;

		/*case N2N || N2R*----------------------------------------------------------------------*///MDF内部数据传输
		case L8DEV_MESH_CMD_MUTUAL_CTRL:{

			/*
				byte1:互控组号
				byte2:互控数据
			*/

			uint8_t mutualCtrlGroup_insert = dataRcv_temp[1]; //互控组号
			uint8_t mutualCtrlGroupParam_data = dataRcv_temp[2];
			stt_devMutualGroupParam mutualCtrlGroupParam_temp = {0};
			uint8_t mutualCtrlGroupNum = 0; //互控数据数组索引
			bool mutualCtrlGroupCheck_res = false;

			mutualCtrlGroupCheck_res = devMutualCtrlGroupInfo_unitCheckByInsert(&mutualCtrlGroupParam_temp, &mutualCtrlGroupNum, mutualCtrlGroup_insert);

			if(mutualCtrlGroupCheck_res){

				devTypeDef_enum currentDevType = currentDev_typeGet();

				switch(currentDevType){
				
					case devTypeDef_mulitSwOneBit:
					case devTypeDef_mulitSwTwoBit:
					case devTypeDef_mulitSwThreeBit:
					case devTypeDef_moudleSwOneBit:
					case devTypeDef_relayBox_1bit:
					case devTypeDef_moudleSwTwoBit:
					case devTypeDef_relayBox_2bit:
					case devTypeDef_moudleSwThreeBit:{

						bool mutualCtrl_reserveIf = false;

						switch(mutualCtrlGroupNum){

							case 0:

								if(mutualCtrlTrigIf_A)
									mutualCtrl_reserveIf = true;

							break;
							
							case 1:

								if(mutualCtrlTrigIf_B)
									mutualCtrl_reserveIf = true;

							break;
							
							case 2:

								if(mutualCtrlTrigIf_C)
									mutualCtrl_reserveIf = true;

							break;

							default:break;
						}

						if(mutualCtrl_reserveIf){

							uint8_t devDataPoint_hex = 0;
							stt_devDataPonitTypedef devDataPoint = {0};

							currentDev_dataPointGet(&devDataPoint);
							memcpy(&devDataPoint_hex, &devDataPoint, sizeof(uint8_t));
							devDataPoint_hex &= 0x07; //仅多位开关类型有效，同时数值仅仅低三位有效
							
							if(mutualCtrlGroupParam_data){
							
								devDataPoint_hex |= (1 << mutualCtrlGroupNum);
							}
							else
							{
								devDataPoint_hex &= ~(1 << mutualCtrlGroupNum);
							}

							currentDev_dataPointSet((stt_devDataPonitTypedef *)&devDataPoint_hex, true, false, true, false, false);
						}
						
					}break;

					case devTypeDef_thermostatExtension:{

						bool mutualCtrl_reserveIf = false;
						
						switch(mutualCtrlGroupNum){
						
							case 0:
						
								if(mutualCtrlTrigIf_A)
									mutualCtrl_reserveIf = true;
						
							break;
							
							case 1:
						
								if(mutualCtrlTrigIf_B)
									mutualCtrl_reserveIf = true;
						
							break;
						
							default:break;
						}
						
						if(mutualCtrl_reserveIf){
						
							uint8_t devThermostatExSwStatus_temp = devDriverBussiness_thermostatSwitch_exSwitchParamGet();
							
							if(mutualCtrlGroupParam_data){
							
								devThermostatExSwStatus_temp |= (1 << mutualCtrlGroupNum);
							}
							else
							{
								devThermostatExSwStatus_temp &= ~(1 << mutualCtrlGroupNum);
							}
						
							devDriverBussiness_thermostatSwitch_exSwitchParamSet(devThermostatExSwStatus_temp);
							devDriverParamChg_dataRealesTrig(true, false, true, false, false);
						}

					}break;

					case devTypeDef_curtain:
					case devTypeDef_moudleSwCurtain:
					case devTypeDef_relayBox_curtain:
					case devTypeDef_dimmer:{

						if(mutualCtrlTrigIf_A)
							currentDev_dataPointSet((stt_devDataPonitTypedef *)&mutualCtrlGroupParam_data, true, false, true, false, false);

					}break;

					case devTypeDef_socket:{}break;
					case devTypeDef_fans:{}break;
					case devTypeDef_scenario:{}break;
					case devTypeDef_heater:{}break;
				
					default:break;
				}

				usrAppParamSet_hbRealesInAdvance(true); //立即向主机更新互控信息
			}

			ESP_LOGI(TAG, "<R2N>mesh data rcv, mutualCtrl Trig.\n");
			
		}break;
		
		case L8DEV_MESH_CMD_SUPER_CTRL:{

			uint8_t *dataHandle = &dataRcv_temp[1];
			uint8_t devSelfMac[MWIFI_ADDR_LEN] = {0};
			stt_superCtrl_dtTransParam *superCtrl_data = (stt_superCtrl_dtTransParam *)dataHandle;
			
			esp_wifi_get_mac(ESP_IF_WIFI_STA, devSelfMac);
			if(0 == memcmp(superCtrl_data->targetDevMac, devSelfMac, sizeof(uint8_t) * MWIFI_ADDR_LEN)){

				currentDev_dataPointSet((stt_devDataPonitTypedef *)&superCtrl_data->devStateSet, true, true, true, true, false);
				lvGuiLinkageConfig_devGraphCtrlBlock_listNodeUnitRefresh(superCtrl_data->targetDevMac, superCtrl_data->devStateSet);
			}
			else{

				if(0 != memcmp(src_addr, devSelfMac, sizeof(uint8_t) * MWIFI_ADDR_LEN)){ //是自己发的就不要再二次刷新了

					lvGuiLinkageConfig_devGraphCtrlBlock_listNodeUnitRefresh(superCtrl_data->targetDevMac, superCtrl_data->devStateSet);
				}
			}

			ESP_LOGI(TAG, "<R2N>mesh data rcv, superCtrl Trig.\n");

		}break;

		case L8DEV_MESH_CMD_SUPER_SYNC:{

			uint8_t *dataHandle = &dataRcv_temp[1];
			uint8_t devSelfMac[MWIFI_ADDR_LEN] = {0};
			stt_superCtrl_dtTransParam *superCtrl_data = (stt_superCtrl_dtTransParam *)dataHandle;

			lvGuiLinkageConfig_devGraphCtrlBlock_listNodeUnitRefresh(superCtrl_data->targetDevMac, superCtrl_data->devStateSet);
			
			esp_wifi_get_mac(ESP_IF_WIFI_STA, devSelfMac);
			if(0 == memcmp(superCtrl_data->targetDevMac, devSelfMac, sizeof(uint8_t) * MWIFI_ADDR_LEN)){

				
			}
			else{


			}

			ESP_LOGI(TAG, "<R2N>mesh data rcv, superSycn Trig.\n");

		}break;

		/*case N2R*--------------------------------------------------------------------------*///子设备上报至网关处理
		case L8DEV_MESH_HEARTBEAT_REQ:{

			if(esp_mesh_get_layer() == MESH_ROOT){ //根节点根据子节点上报信息进行链表管理

				stt_hbDataUpload devNode_hbDataTemp = {0};
				stt_nodeDev_hbDataManage *devNode = NULL;
				stt_nodeDev_hbDataManage devNode_temp = {0};

				memcpy(&devNode_hbDataTemp, (stt_hbDataUpload *)&dataRcv_temp[L8_meshDataCmdLen], sizeof(stt_hbDataUpload)); //数据加载
				devNode = L8devHbDataManageList_nodeGet(listHead_nodeDevDataManage, devNode_hbDataTemp.nodeDev_Mac, true); //链表节点获取
				if(NULL == devNode){

					printf("newNodeMac[heartBeat]:"MACSTR"-creat.\n", MAC2STR(devNode_hbDataTemp.nodeDev_Mac)); 

					memcpy(&(devNode_temp.dataManage), &devNode_hbDataTemp, sizeof(stt_hbDataUpload));
					devNode_temp.nodeDevKeepAlive_counter = L8_NODEDEV_KEEPALIVE_PERIOD;
				
					L8devHbDataManageList_nodeCreat(listHead_nodeDevDataManage, &devNode_temp);	
				}
				else
				{
					printf("nodeMac[heartBeat]:"MACSTR"-keepalive reales.\n", MAC2STR(devNode->dataManage.nodeDev_Mac));
					memcpy(&(devNode->dataManage), &devNode_hbDataTemp, sizeof(stt_hbDataUpload)); //节点状态信息更新
					devNode->nodeDevKeepAlive_counter = L8_NODEDEV_KEEPALIVE_PERIOD; //生命周期刷新
				}
			}

			ESP_LOGI(TAG, "<N2R>mesh data rcv, heartbeat data.\n");
		
		}break;

		case L8DEV_MESH_CMD_DETAIL_INFO_REPORT:{

			if(esp_mesh_get_layer() == MESH_ROOT){

				stt_devInfoDetailUpload devNode_detailInfoTemp = {0};
				stt_nodeDev_detailInfoManage *devNode = NULL;
				stt_nodeDev_detailInfoManage devNode_temp = {0};

				memcpy(&devNode_detailInfoTemp, (stt_devInfoDetailUpload *)&dataRcv_temp[L8_meshDataCmdLen], sizeof(stt_devInfoDetailUpload)); //数据加载
				devNode = L8devInfoDetailManageList_nodeGet(listHead_nodeInfoDetailManage, devNode_detailInfoTemp.nodeDev_Mac, true); //链表节点获取
				if(NULL == devNode){

					printf("newNodeMac[detailInfo]:"MACSTR"-creat.\n", MAC2STR(devNode_detailInfoTemp.nodeDev_Mac)); 

					memcpy(&(devNode_temp.dataManage), &devNode_detailInfoTemp, sizeof(stt_devInfoDetailUpload));
					L8devInfoDetailManageList_nodeCreat(listHead_nodeInfoDetailManage, &devNode_temp); 
				}
				else
				{
					printf("nodeMac[detailInfo]:"MACSTR"-report reales.\n", MAC2STR(devNode->dataManage.nodeDev_Mac));
				
					memcpy(&(devNode->dataManage), &devNode_detailInfoTemp, sizeof(stt_devInfoDetailUpload)); //节点信息更新
				}

				devDetailInfoUploadTrig(); //触发节点详情更新上传
			}

			ESP_LOGI(TAG, "<N2R>mesh data rcv, detail info report data.\n");

		}break;

		case L8DEV_MESH_CMD_DEVINFO_LIST_REQ:{

			if(esp_mesh_get_layer() == MESH_ROOT){ //仅root设备有效

				devDetailParamManageList_rootNoticeTrig(src_addr);
			}

			ESP_LOGI(TAG, "<N2R>mesh data rcv, device info list req.\n");

		}break;

		case L8DEV_MESH_FWARE_UPGRADE:{

			uint8_t *dataHandleTemp = &dataRcv_temp[1];
			uint8_t  targetUpgradeDevType = dataHandleTemp[0];
			bool	 mulitUpgradeFlg = false; 

			usrAppUpgrade_targetDevaddrSet(src_addr);

			if(dataLen > 1){

				if(DEV_TYPES_PANEL_DEF_NULL != deviceTypeVersionJudge(targetUpgradeDevType)){ //类型版本可识别

					mulitUpgradeFlg = true;
				}
			}

			if(mulitUpgradeFlg){

				usrApp_firewareUpgrade_trig(true, targetUpgradeDevType); //批量升级
			}
			else
			{
				usrApp_firewareUpgrade_trig(false, 0); //单独升级
			}

			printf("upgrade dtrx devType:%02X.\n", dataRcv_temp[1]);

		}break;
		
		default:break;
	}
}

void usrApp_devNodeStatusSynchronousInitiative(void){

	const uint8_t devMacAddr_root[DEVICE_MAC_ADDR_APPLICATION_LEN] = MWIFI_ADDR_ROOT;

	mdf_err_t ret				= MDF_OK;
	mwifi_data_type_t data_type = {
		
		.compression = true,
		.communicate = MWIFI_COMMUNICATE_UNICAST,
	};
	mlink_httpd_type_t type_L8mesh_cst = {
	
		.format = MLINK_HTTPD_FORMAT_HEX,
	};

	uint8_t dataTrans_temp[1 + 1 + DEVPARAMEXT_DT_LEN + DEVICE_MAC_ADDR_APPLICATION_LEN] = {0};
	uint16_t dataTrans_tempLen = 1 + 1 + DEVPARAMEXT_DT_LEN + DEVICE_MAC_ADDR_APPLICATION_LEN;
	
	memcpy(&data_type.custom, &type_L8mesh_cst, sizeof(uint32_t));

	dataTrans_temp[0] = L8DEV_MESH_STATUS_SYNCHRO; //第一字节永远是命令
	currentDev_dataPointGetwithRecord((stt_devDataPonitTypedef *)&dataTrans_temp[1]); //payload ist0：状态
	currentDev_extParamGet(&dataTrans_temp[2]); //payload ist1 - 4：扩展状态
	esp_wifi_get_mac(ESP_IF_WIFI_STA, &dataTrans_temp[6]);  //payload ist5 - 10：MAC地址

	ret = mwifi_write(devMacAddr_root, &data_type, dataTrans_temp, dataTrans_tempLen, true);
	USR_ERROR_CHECK(ret != MDF_OK, ret, "<%s> mqtt mwifi_root_write", mdf_err_to_name(ret));
}

void usrApp_devNodeHomeassistantOnlineNotice(void){

	const uint8_t devMacAddr_root[DEVICE_MAC_ADDR_APPLICATION_LEN] = MWIFI_ADDR_ROOT;

	mdf_err_t ret				= MDF_OK;
	mwifi_data_type_t data_type = {
		
		.compression = true,
		.communicate = MWIFI_COMMUNICATE_UNICAST,
	};
	mlink_httpd_type_t type_L8mesh_cst = {
	
		.format = MLINK_HTTPD_FORMAT_HEX,
	};

	uint16_t dataTrans_tempLen = 1 + DEVICE_MAC_ADDR_APPLICATION_LEN;
	uint8_t dataTrans_temp[1 + DEVICE_MAC_ADDR_APPLICATION_LEN] = {0};
	
	memcpy(&data_type.custom, &type_L8mesh_cst, sizeof(uint32_t));

	dataTrans_temp[0] = L8DEV_MESH_CMD_HOMEASSISTANT_ONLINE; //第一字节永远是命令
	esp_wifi_get_mac(ESP_IF_WIFI_STA, &dataTrans_temp[1]);  //payload ist5 - 10：MAC地址

	ret = mwifi_write(devMacAddr_root, &data_type, dataTrans_temp, dataTrans_tempLen, true);
	USR_ERROR_CHECK(ret != MDF_OK, ret, "<%s> mqtt mwifi_root_write", mdf_err_to_name(ret));
}

void devDetailParamManageList_rootNoticeTrig(uint8_t destMac[MWIFI_ADDR_LEN]){

	mwifi_data_type_t data_type = {
		
		.compression = true,
		.communicate = MWIFI_COMMUNICATE_UNICAST,
	};
	const mlink_httpd_type_t type_L8mesh_cst = {

		.format = MLINK_HTTPD_FORMAT_HEX,
	};
	mdf_err_t ret = MDF_OK;

	const uint8_t dataTab_headLen = 4; //数据tab描述头长度
	uint8_t dtPagHead_len = 1 + 1; //单帧描述头长度， ist1: mesh命令， ist2: 单帧node数量 
	uint8_t nodeUnitDtPag_len = MWIFI_ADDR_LEN + sizeof(stt_devInfoDetailUpload_2Root);
	uint8_t devUnitNum_temp = 0;
	uint16_t devUnitNumLimit_perPack = (USRAPP_LOCAL_MESH_DATATRANS_PERPACK_MAX_LEN - dtPagHead_len) / nodeUnitDtPag_len; //mqtt单包包含设备信息 数量限制
	uint16_t meshData_pbLen = 0;

	if(mwifi_is_connected() && esp_mesh_get_layer() == MESH_ROOT); //仅根节点有效
	else return;

	memcpy(&(data_type.custom), &type_L8mesh_cst, sizeof(uint32_t));

	uint8_t *dataNotice_rootManageList = L8devInfoDetailManageList_data2RootTabGet(listHead_nodeInfoDetailManage);
	if(dataNotice_rootManageList[0] != DEVLIST_MANAGE_LISTNUM_MASK_NULL){

		uint8_t *dataRespPerPackBuff = (uint8_t *)os_zalloc(sizeof(uint8_t) * USRAPP_LOCAL_MESH_DATATRANS_PERPACK_MAX_LEN);
		uint16_t tabData_len = 0;

		devUnitNum_temp = dataNotice_rootManageList[3]; 
		memcpy(&tabData_len, &dataNotice_rootManageList[0], sizeof(uint16_t));
		printf("L8devInfoDetailManageList_data2RootTabGet, devNum:%d, tabLen:%d.\n", devUnitNum_temp, tabData_len);
	
		if(dataRespPerPackBuff != NULL){

			uint8_t dataRespLoop = devUnitNum_temp / devUnitNumLimit_perPack; //商
			uint8_t dataRespLastPack_devNum = devUnitNum_temp % devUnitNumLimit_perPack; //余
			uint16_t dataRespPerPackInfoLen = 0;
			uint16_t dataRespLoadInsert = dataTab_headLen;

			//总信息长度超过一个包则拆分发送 -商包
			if(dataRespLoop){

				uint8_t loop = 0;

				for(loop = 0; loop < dataRespLoop; loop ++){

					dataRespPerPackInfoLen = nodeUnitDtPag_len * devUnitNumLimit_perPack;
					dataRespPerPackBuff[0] = L8DEV_MESH_CMD_DETAILDEVINFO_NOTICE; //mesh命令
					dataRespPerPackBuff[1] = devUnitNumLimit_perPack; //node数量
					memcpy(&dataRespPerPackBuff[dtPagHead_len], &dataNotice_rootManageList[dataRespLoadInsert], dataRespPerPackInfoLen);
					dataRespLoadInsert += dataRespPerPackInfoLen;
					meshData_pbLen = dataRespPerPackInfoLen + dtPagHead_len;

					ret = mwifi_root_write(destMac, 
										   1, 
										   &data_type, 
										   dataRespPerPackBuff,
										   meshData_pbLen, 
										   true);
					USR_ERROR_CHECK(ret != MDF_OK, ret, "<%s> devDetail info list notice from root mwifi_translate", mdf_err_to_name(ret));
					memset(dataRespPerPackBuff, 0, sizeof(uint8_t) * USRAPP_LOCAL_MESH_DATATRANS_PERPACK_MAX_LEN); //数据发送缓存清空
					printf("devDetail list respond loop%d res:%08X.\n", loop, ret);
				}
			}

			//设备列表收尾数据包 -余包
			if(dataRespLastPack_devNum){

				dataRespPerPackInfoLen = nodeUnitDtPag_len * dataRespLastPack_devNum;
				dataRespPerPackBuff[0] = L8DEV_MESH_CMD_DETAILDEVINFO_NOTICE; //mesh命令
				dataRespPerPackBuff[1] = dataRespLastPack_devNum; //node数量
				memcpy(&dataRespPerPackBuff[dtPagHead_len], &dataNotice_rootManageList[dataRespLoadInsert], dataRespPerPackInfoLen);
				dataRespLoadInsert += dataRespPerPackInfoLen;
				meshData_pbLen = dataRespPerPackInfoLen + dtPagHead_len;

				ret = mwifi_root_write(destMac, 
									   1, 
									   &data_type, 
									   dataRespPerPackBuff,
									   meshData_pbLen, 
									   true);
				USR_ERROR_CHECK(ret != MDF_OK, ret, "<%s> devDetail info list notice from root mwifi_translate", mdf_err_to_name(ret));
				memset(dataRespPerPackBuff, 0, sizeof(uint8_t) * USRAPP_LOCAL_MESH_DATATRANS_PERPACK_MAX_LEN); //数据发送缓存清空
				printf("devDetail list respond last res:%08X.\n", ret);
			}
			
			free(dataRespPerPackBuff);
		}
		else{

			MDF_LOGW("devDetail info list notice, respond malloc fail!");
		}
	}

	if(dataNotice_rootManageList != NULL)free(dataNotice_rootManageList);
}

void deviceDetailInfoListRequest_bussinessTrig(void){

	if(mwifi_is_connected()){

		lvglUsrApp_devCtrlBlockBaseManageList_listDestory(listHead_nodeCtrlObjBlockBaseManage); //子设备清空设备列表缓存，便于刷新

		if(esp_mesh_get_layer() == MESH_ROOT){ //root节点无需发送获取请求，从自己的内存获取info list即可

			uint8_t *dataNotice_rootManageList = L8devInfoDetailManageList_data2RootTabGet(listHead_nodeInfoDetailManage);
			if(dataNotice_rootManageList[0] != DEVLIST_MANAGE_LISTNUM_MASK_NULL){

				const uint8_t dataUnitIst_start = 1,
							  dataUnitLen = MWIFI_ADDR_LEN + sizeof(stt_devInfoDetailUpload_2Root);
	
				uint8_t *dataHandle = &dataNotice_rootManageList[3];
							  
				uint16_t dataIst_temp = 0;
				uint8_t devNum = dataHandle[0];
				uint8_t loop = 0;
				stt_devInfoDetailUpload_2Root *detailInfoUnit_ptr = NULL;
				stt_nodeObj_listManageDevCtrlBase gNode_new = {0};
	
				for(loop = 0; loop < devNum; loop ++){
	
					memset(&gNode_new, 0, sizeof(stt_nodeObj_listManageDevCtrlBase));
					dataIst_temp = dataUnitIst_start + dataUnitLen * loop;
					memcpy(gNode_new.nodeData.ctrlObj_devMac, &dataHandle[dataIst_temp], sizeof(uint8_t) * MWIFI_ADDR_LEN);
					dataIst_temp += MWIFI_ADDR_LEN;
					detailInfoUnit_ptr = (stt_devInfoDetailUpload_2Root *)&dataHandle[dataIst_temp];
					memcpy(&gNode_new.nodeData.ctrlObj_devType, &detailInfoUnit_ptr->devType, sizeof(uint8_t));
					memcpy(&gNode_new.nodeData.devStatusVal, &detailInfoUnit_ptr->devSelf_status, sizeof(uint8_t));
					memcpy(gNode_new.nodeData.objIcon_ist, detailInfoUnit_ptr->devSelf_iconIst, sizeof(uint8_t) * GUIBUSSINESS_CTRLOBJ_MAX_NUM);
					memcpy(gNode_new.nodeData.objCtrl_name, detailInfoUnit_ptr->devSelf_name, sizeof(char) * GUIBUSSINESS_CTRLOBJ_MAX_NUM * DEV_CTRLOBJ_NAME_DETAILUD_LEN);
//					printf("listGblock creat, listNum:%d.\n", lvglUsrApp_devCtrlBlockBaseManageList_nodeAdd(listHead_nodeCtrlObjBlockBaseManage, &gNode_new));
					lvglUsrApp_devCtrlBlockBaseManageList_nodeAdd(listHead_nodeCtrlObjBlockBaseManage, &gNode_new);
				}
			}

			free(dataNotice_rootManageList);
		}
		else{ //子节点向root节点请求info list
			
			const uint8_t dataRequest_temp = L8DEV_MESH_CMD_DEVINFO_LIST_REQ;
			const uint8_t meshRootAddr[MWIFI_ADDR_LEN] = MWIFI_ADDR_ROOT;
			mdf_err_t ret = MDF_OK;
			mwifi_data_type_t data_type = {
				
				.compression = true,
				.communicate = MWIFI_COMMUNICATE_UNICAST,
			};
			const mlink_httpd_type_t type_L8mesh_cst = {
			
				.format = MLINK_HTTPD_FORMAT_HEX,
			};
			
			memcpy(&(data_type.custom), &type_L8mesh_cst, sizeof(uint32_t));
		
			ret = mwifi_write(meshRootAddr, &data_type, &dataRequest_temp, 1, true);
			USR_ERROR_CHECK(ret != MDF_OK, ret, "<%s> mwifi_node_write", mdf_err_to_name(ret));			
		}
	}
}

void devDetailInfoList_request_trigByEvent(void){

	xEventGroupSetBits(xEventGp_devApplication, DEVAPPLICATION_FLG_DEVINFO_LIST_REQ);
}

void devHeartbeat_dataTrans_bussinessTrig(void){

	uint8_t dataRequest_temp[32] = {0}; //缓存大小要给够，不然数据传输会出问题
	mdf_err_t ret = MDF_OK;
	mwifi_data_type_t data_type = {
		
		.compression = true,
		.communicate = MWIFI_COMMUNICATE_UNICAST,
	};
	const mlink_httpd_type_t type_L8mesh_cst = {
	
		.format = MLINK_HTTPD_FORMAT_HEX,
	};

	if(usrApp_heartBeathold_counter)return;
	
	memcpy(&(data_type.custom), &type_L8mesh_cst, sizeof(uint32_t));

	if(mwifi_is_connected() && esp_mesh_get_layer() != MESH_ROOT){ //是子节点则发心跳

		static uint8_t meshApp_notice_count = 3;

		stt_hbDataUpload nodeDev_hbDataTemp = {0};
		const uint8_t meshRootAddr[MWIFI_ADDR_LEN] = MWIFI_ADDR_ROOT;

		L8devHeartbeatFunctionParamLoad(&nodeDev_hbDataTemp); //数据加载

		dataRequest_temp[0] = L8DEV_MESH_HEARTBEAT_REQ;
		memcpy(&dataRequest_temp[L8_meshDataCmdLen], &nodeDev_hbDataTemp, sizeof(stt_hbDataUpload));

		ret = mwifi_write(meshRootAddr, &data_type, dataRequest_temp, sizeof(stt_hbDataUpload) + 1, true);
		USR_ERROR_CHECK(ret != MDF_OK, ret, "<%s> heartbeat mwifi_root_write", mdf_err_to_name(ret));

//		printf("n2r hb trig.\n");
	}
	else
	if(mwifi_is_connected() && esp_mesh_get_layer() == MESH_ROOT){ //是根节点则广播时间

		const uint8_t boardcastAddr[MWIFI_ADDR_LEN] = MWIFI_ADDR_BROADCAST;

		deviceParamGet_timeZone((stt_timeZone *)&dataRequest_temp[L8_meshDataCmdLen]); //设备时区填装
		usrAppDevCurrentSystemTime_paramGet((stt_localTime *)&dataRequest_temp[sizeof(stt_timeZone) + L8_meshDataCmdLen]); //设备系统时间填装

		dataRequest_temp[0] = L8DEV_MESH_SYSTEMTIME_BOARDCAST;

		ret = mwifi_root_write(boardcastAddr, 1, &data_type, dataRequest_temp, sizeof(stt_timeZone) + sizeof(stt_localTime) + L8_meshDataCmdLen, true);
        USR_ERROR_CHECK(ret != MDF_OK, ret, "<%s> mwifi_root_write", mdf_err_to_name(ret));

		if(true == usrMeshApplication_rootFirstConNoticeActionRserveGet()){
			
			memset(dataRequest_temp, 0, sizeof(dataRequest_temp));
			dataRequest_temp[0] = L8DEV_MESH_CMD_ROOT_FIRST_CONNECT;
			ret = mwifi_root_write(boardcastAddr, 1, &data_type, dataRequest_temp, 1, true);
			USR_ERROR_CHECK(ret != MDF_OK, ret, "<%s> root firstConnect mwifi_root_write", mdf_err_to_name(ret));
		}

//		printf("r2n systime boradcast trig.\n");
	}
}

void devDetailInfoUploadTrig(void){

	uint8_t cycCount = 0;

	(MESH_ROOT == esp_mesh_get_layer())?
		(cycCount = 9):
		(cycCount = 18);
	
	usrAppAllNodeDevDetailInfoReport_cycCountSet(cycCount);

//	printf("dev detail info upload trig.\n");
}

void datatransOpreation_heartbeatHold_realesRunning(void){

	if(usrApp_heartBeathold_counter)usrApp_heartBeathold_counter --;
}

void datatransOpreation_heartbeatHold_set(uint16_t valSet){

	usrApp_heartBeathold_counter = valSet;
}

void datatransOpreation_heartbeatHold_auto(void){

	uint8_t meshNodeNum = (uint8_t)esp_mesh_get_total_node_num();

	if(meshNodeNum < 30)usrApp_heartBeathold_counter = 180;
	else{

		usrApp_heartBeathold_counter = meshNodeNum / 10 * 120; //每10个两分钟
	}
}

uint16_t datatransOpreation_heartbeatHold_get(void){

	return usrApp_heartBeathold_counter;
}





