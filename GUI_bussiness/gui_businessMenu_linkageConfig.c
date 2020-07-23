/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>

#include "os.h"

// /* freertos includes */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_freertos_hooks.h"

#include "sdkconfig.h"

/* lvgl includes */
#include "iot_lvgl.h"

#include "gui_businessMenu_linkageConfig.h"
#include "gui_businessHome.h"

#include "gui_businessReuse_reactionObjPage.h"

#include "devDriver_manage.h"

#include "bussiness_timerSoft.h"

#include "dataTrans_localHandler.h"

#define PAGEREACTION_REUSE_LINKAGETEMPRATURE_THRESHOLDADJ_FNUM		(PAGEREACTION_REUSE_BUSSINESS_RESERVE_MAX + 1)

LV_FONT_DECLARE(lv_font_consola_13);
LV_FONT_DECLARE(lv_font_dejavu_15);
LV_FONT_DECLARE(lv_font_consola_16);
LV_FONT_DECLARE(lv_font_consola_17);
LV_FONT_DECLARE(lv_font_consola_19);
LV_FONT_DECLARE(lv_font_ariblk_18);

LV_IMG_DECLARE(iconMenu_funBack_arrowLeft);
LV_IMG_DECLARE(iconMenu_funBack_homePage);
LV_IMG_DECLARE(imageBtn_feedBackNormal);

LV_IMG_DECLARE(scrsaverStatusIconBk);

extern EventGroupHandle_t xEventGp_devApplication;

extern stt_nodeObj_listManageDevCtrlBase *listHead_nodeCtrlObjBlockBaseManage;

static const char *TAG = "lvUsrPage_linkage";

static const uint8_t L8_meshDataCmdLen = 1; //L8 mesh内部数据传输 头命令 长度

static lv_style_t *stylePage_funSetOption = NULL;
static lv_style_t *stylePage_linkageDetailSetting = NULL;
static lv_style_t *styleText_menuLevel_A = NULL;
static lv_style_t *styleText_menuLevel_B = NULL;
static lv_style_t *styleText_menuLevel_C = NULL;
static lv_style_t *styleCb_proximityDetection_screenLight = NULL;
static lv_style_t *styleCb_proximityDetection_switchTrigEn = NULL;
static lv_style_t *styleBtn_specialTransparent = NULL;
static lv_style_t *styleTextBtn_linkageDetailSetting = NULL;
static lv_style_t *styleTextTitle_linkageDetailSetting = NULL;
static lv_style_t *styleRoller_bg_tempThresholdAdj = NULL;
static lv_style_t *styleRoller_sel_tempThresholdAdj = NULL;
static lv_style_t *styleImg_menuFun_btnFun = NULL;

static lv_style_t *styleStdGblockCtrlObj_baseBlock_SELF = NULL;
static lv_style_t *styleStdGblockCtrlObj_baseBlock_A = NULL;
static lv_style_t *styleStdGblockCtrlObj_baseBlock_B = NULL;
static lv_style_t *styleStdGblockCtrlObj_baseBlock_C = NULL;
static lv_style_t *styleStdGblockCtrlObj_baseBlock_D = NULL;

static lv_style_t *styleStdGblockCtrlObj_imgbtn_pr = NULL;
static lv_style_t *styleStdGblockCtrlObj_imgbtn_rel = NULL;
static lv_style_t *styleStdGblockCtrlObj_icon_pr = NULL;
static lv_style_t *styleStdGblockCtrlObj_icon_rel = NULL;
static lv_style_t *styleStdGblockCtrlObj_labSelf = NULL;
static lv_style_t *styleStdGblockCtrlObj_labOther = NULL;

static lv_style_t *styleStdGblockCtrlObjLabel_ptr = NULL;

static lv_obj_t *menuBtnChoIcon_fun_back = NULL;
static lv_obj_t *menuBtnChoIcon_fun_home = NULL;
static lv_obj_t *imgMenuBtnChoIcon_fun_home = NULL;
static lv_obj_t *imgMenuBtnChoIcon_fun_back = NULL;
static lv_obj_t *page_funSetOption = NULL;
static lv_obj_t *bGround_obj = NULL;
static lv_obj_t *text_Title = NULL;
static lv_obj_t *textSettingA_proximityDetection = NULL;
static lv_obj_t	*textSettingA_tempratureDetection = NULL;
static lv_obj_t	*swReserveSet_proximityDetection = NULL;
static lv_obj_t	*swReserveSet_tempratureDetection = NULL;

static lv_obj_t	*cbProximityDetection_screenLight = NULL;
static lv_obj_t	*btnProximityDetection_switchTrig = NULL;
static lv_obj_t	*textBtnProximityDetection_switchTrig = NULL;
static lv_obj_t	*pageBtnProximityDetection_switchTrig = NULL;
static lv_obj_t	*cbProximityDetection_switchTrig_en = NULL;

static lv_obj_t	*btnTempratureDetection_tempThresholdAdj = NULL;
static lv_obj_t	*textBtnTempratureDetection_tempThresholdAdj = NULL;
static lv_obj_t	*btnTempratureDetection_switchTrig = NULL;
static lv_obj_t	*textBtnTempratureDetection_switchTrig = NULL;

static lv_obj_t	*pageLinkage_detailSetting = NULL;
static lv_obj_t	*labelTitle_pageLinkageDetailSetting = NULL;
static lv_obj_t	*btnConfirm_pageLinkageDetailSetting = NULL;
static lv_obj_t	*textBtnConfirm_pageLinkageDetailSetting = NULL;
static lv_obj_t	*btnCancel_pageLinkageDetailSetting = NULL;
static lv_obj_t	*textBtnCancel_pageLinkageDetailSetting = NULL;

static lv_obj_t	*rollerThresholdTempSet_tempratureDetection = NULL;
static lv_obj_t	*textRollerThresholdTempSet_tempratureDetection = NULL;

static lv_obj_t	*lvDevGblockListGenerate_prePage = NULL;

static bool pageCreated_flg = false;

static char strTemp_textBtnTempratureDetection_tempThresholdAdj[60] = {0};

static uint8_t cfgDataTemp_linkageCondition_temprature = 0;
static bool cfgDataScrLight_linkageReaction_proximity = false;

static uint8_t screenLandscapeCoordinate_objOffset = 0;

static struct{

	uint8_t *dats;
	uint16_t dataLen;
}dataTemp_swDev_superCtrl = {0};

static const uint8_t devOpt_gBlockBase_sizeX = 230,
					 devOpt_gBlockBase_sizeY = 90;

static void lvGuiLinkageConfig_childOptionObjCreat_proximityDetection(void);
static void lvGuiLinkageConfig_childOptionObjDelete_proximityDetection(void);
static void lvGuiLinkageConfig_childOptionObjCreat_tempratureDetection(void);
static void lvGuiLinkageConfig_childOptionObjDelete_tempratureDetection(void);

static void dataTransBussiness_pageLinkageCfg_superCtrlActionTrig(uint8_t devMac_dest[MWIFI_ADDR_LEN], uint8_t devStateSet){

	uint8_t devSelfMac[MWIFI_ADDR_LEN] = {0};
	uint8_t dataIst = 0;
	stt_superCtrl_dtTransParam dtSuperCtrlTemp = {0};

	esp_wifi_get_mac(ESP_IF_WIFI_STA, devSelfMac);

	if(0 == memcmp(devMac_dest, devSelfMac, sizeof(uint8_t) * MWIFI_ADDR_LEN)){

		currentDev_dataPointSet((stt_devDataPonitTypedef *)&devStateSet, true, true, true, true, true);
	}
	else{

		dataTemp_swDev_superCtrl.dataLen = L8_meshDataCmdLen + sizeof(stt_superCtrl_dtTransParam);
		dataTemp_swDev_superCtrl.dats = (uint8_t *)os_zalloc(dataTemp_swDev_superCtrl.dataLen);

		memcpy(dtSuperCtrlTemp.targetDevMac, devMac_dest, MWIFI_ADDR_LEN);
		dtSuperCtrlTemp.devStateSet = devStateSet;

		dataTemp_swDev_superCtrl.dats[dataIst] = L8DEV_MESH_CMD_SUPER_CTRL;
		dataIst += L8_meshDataCmdLen;
		memcpy(&dataTemp_swDev_superCtrl.dats[dataIst], &dtSuperCtrlTemp, sizeof(stt_superCtrl_dtTransParam));

		xEventGroupSetBits(xEventGp_devApplication, DEVAPPLICATION_FLG_DEVLOCAL_SUPER_CTRL);		
	}
}

static void currentGui_elementClear(void){

	if(pageLinkage_detailSetting){

		lv_obj_del(pageLinkage_detailSetting);
		pageLinkage_detailSetting = NULL;
	}

	lv_obj_del(page_funSetOption);
}

void guiDispTimeOut_pageLinkageCfg(void){

//	lvGui_usrSwitch(bussinessType_Home);

//	currentGui_elementClear();

	lvGui_usrSwitch_withPrefunc(bussinessType_Home, currentGui_elementClear);
}

static lv_res_t funCb_btnActionClick_menuBtn_funBack(lv_obj_t *btn){

	LV_OBJ_FREE_NUM_TYPE btnFreeNum = lv_obj_get_free_num(btn);
	usrGuiBussiness_type guiChg_temp = bussinessType_Menu;

	switch(btnFreeNum){

		case LV_OBJ_FREENUM_BTNNUM_DEF_MENUHOME:

			guiChg_temp = bussinessType_Home;

		break;

		case LV_OBJ_FREENUM_BTNNUM_DEF_MENUBACK:
		default:

			guiChg_temp = bussinessType_Menu;

		break;
	}

//	lvGui_usrSwitch(guiChg_temp);

//	currentGui_elementClear();

	lvGui_usrSwitch_withPrefunc(guiChg_temp, currentGui_elementClear);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionPress_menuBtn_funBack(lv_obj_t *btn){

	LV_OBJ_FREE_NUM_TYPE btnFreeNum = lv_obj_get_free_num(btn);
	lv_obj_t *objImg_colorChg = NULL;

	switch(btnFreeNum){

		case LV_OBJ_FREENUM_BTNNUM_DEF_MENUHOME:

			objImg_colorChg = imgMenuBtnChoIcon_fun_home;

		break;

		case LV_OBJ_FREENUM_BTNNUM_DEF_MENUBACK	:
		default:

			objImg_colorChg = imgMenuBtnChoIcon_fun_back;

		break;
	}

	lv_img_set_style(objImg_colorChg, styleImg_menuFun_btnFun);
	lv_obj_refresh_style(objImg_colorChg);

	return LV_RES_OK;
}

static lv_res_t funCb_swOpreat_proximityDetection_reserveEn(lv_obj_t *sw){

	bool opVal_get = lv_sw_get_state(sw);
	stt_paramLinkageConfig linkageConfigParamSet_temp = {0};

	devSystemOpration_linkageConfig_paramGet(&linkageConfigParamSet_temp);

	if(opVal_get){ 

		lvGuiLinkageConfig_childOptionObjCreat_proximityDetection();
	}
	else
	{
		lvGuiLinkageConfig_childOptionObjDelete_proximityDetection();
	}

	(opVal_get)?
		(linkageConfigParamSet_temp.linkageRunning_proxmity_en = 1):
		(linkageConfigParamSet_temp.linkageRunning_proxmity_en = 0);
	devSystemOpration_linkageConfig_paramSet(&linkageConfigParamSet_temp, true);
	
	return LV_RES_OK;
}

static lv_res_t funCb_swOpreat_tempratureDetection_reserveEn(lv_obj_t *sw){
	
	bool opVal_get = lv_sw_get_state(sw);
	stt_paramLinkageConfig linkageConfigParamSet_temp = {0};

	devSystemOpration_linkageConfig_paramGet(&linkageConfigParamSet_temp);

	if(opVal_get){

		lvGuiLinkageConfig_childOptionObjCreat_tempratureDetection();	
	}
	else
	{
		lvGuiLinkageConfig_childOptionObjDelete_tempratureDetection();
	}

	(opVal_get)?
		(linkageConfigParamSet_temp.linkageRunning_temprature_en = 1):
		(linkageConfigParamSet_temp.linkageRunning_temprature_en = 0);
	devSystemOpration_linkageConfig_paramSet(&linkageConfigParamSet_temp, true);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClickDetailSet_pageChoiceSelect_confirm(lv_obj_t *btn){

	LV_OBJ_FREE_NUM_TYPE btnmFreeNum = lv_obj_get_free_num(btn);

	switch(btnmFreeNum){
	 
//		case PAGEREACTION_REUSE_BUSSINESS_TIMERSET_IST:{}break;
//		
//		case PAGEREACTION_REUSE_BUSSINESS_DELAYSET_IST:{}break;
		
		case PAGEREACTION_REUSE_BUSSINESS_LINKAGESET_TEMP:{

			stt_devDataPonitTypedef dataPointReaction_valTemp = {0};
			stt_paramLinkageConfig linkageConfigParamSet_temp = {0};
			
			devSystemOpration_linkageConfig_paramGet(&linkageConfigParamSet_temp);
			lvGui_businessReuse_reactionObjPageElement_funValConfig_get(PAGEREACTION_REUSE_BUSSINESS_LINKAGESET_TEMP, 
																		&dataPointReaction_valTemp);
			
			memcpy(&linkageConfigParamSet_temp.linkageReaction_temprature_swVal,
				   &dataPointReaction_valTemp,
				   sizeof(stt_devDataPonitTypedef));
			devSystemOpration_linkageConfig_paramSet(&linkageConfigParamSet_temp, true);		

		}break;
		
		case PAGEREACTION_REUSE_BUSSINESS_LINKAGESET_PROX:{

			stt_devDataPonitTypedef dataPointReaction_valTemp = {0};
			stt_paramLinkageConfig linkageConfigParamSet_temp = {0};

			devSystemOpration_linkageConfig_paramGet(&linkageConfigParamSet_temp);
			lvGui_businessReuse_reactionObjPageElement_funValConfig_get(PAGEREACTION_REUSE_BUSSINESS_LINKAGESET_PROX, 
																		&dataPointReaction_valTemp);

			memcpy(&linkageConfigParamSet_temp.linkageReaction_proxmity_swVal,
				   &dataPointReaction_valTemp,
				   sizeof(stt_devDataPonitTypedef));
			devSystemOpration_linkageConfig_paramSet(&linkageConfigParamSet_temp, true);		

		}break;

		case PAGEREACTION_REUSE_LINKAGETEMPRATURE_THRESHOLDADJ_FNUM:{

			stt_paramLinkageConfig linkageConfigParamSet_temp = {0};
			
			devSystemOpration_linkageConfig_paramGet(&linkageConfigParamSet_temp);

			linkageConfigParamSet_temp.linkageCondition_tempratureVal = cfgDataTemp_linkageCondition_temprature;
			devSystemOpration_linkageConfig_paramSet(&linkageConfigParamSet_temp, true);

			sprintf(strTemp_textBtnTempratureDetection_tempThresholdAdj, " #00A2E8 > temprature set##C0C0FF [%d\"C]#", cfgDataTemp_linkageCondition_temprature); //温度设定值显示更新
			lv_label_set_text(textBtnTempratureDetection_tempThresholdAdj, strTemp_textBtnTempratureDetection_tempThresholdAdj);	

		}break;

		default:break;
	}

	if(pageLinkage_detailSetting){

		lv_obj_del(pageLinkage_detailSetting);
		pageLinkage_detailSetting = NULL;
	}

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClickDetailSet_pageChoiceSelect_cancel(lv_obj_t *btn){

	if(pageLinkage_detailSetting){

		lv_obj_del(pageLinkage_detailSetting);
		pageLinkage_detailSetting = NULL;
	}

	return LV_RES_OK;
}

static lv_res_t funCb_imgbtnActionClick_CtrlObjBaseOpreat(lv_obj_t *imgbtn){

	LV_OBJ_FREE_NUM_TYPE btnmFreeNum = lv_obj_get_free_num(imgbtn);
	LV_OBJ_FREE_NUM_TYPE btnmFreeNumIndex = btnmFreeNum - (btnmFreeNum % GUIBUSSINESS_CTRLOBJ_MAX_NUM);					
	LV_OBJ_FREE_NUM_TYPE objFnum_ist = btnmFreeNum % GUIBUSSINESS_CTRLOBJ_MAX_NUM;
	lv_btn_state_t btnState = lv_imgbtn_get_state(imgbtn);
	stt_devDataPonitTypedef devStateSet_temp = {0};

	stt_nodeObj_listManageDevCtrlBase *objNode_data = lvglUsrApp_devCtrlBlockBaseManageList_nodeGetByLvObjFreenum(listHead_nodeCtrlObjBlockBaseManage, btnmFreeNumIndex, true);

//	printf("%s: fNum:%d, ist:%d, btnState:%d.\n", TAG, btnmFreeNum, objFnum_ist, btnState);

	if(NULL != objNode_data){

		switch(objNode_data->nodeData.ctrlObj_devType){

			case devTypeDef_mulitSwOneBit:
			case devTypeDef_mulitSwTwoBit:
			case devTypeDef_mulitSwThreeBit:
			case devTypeDef_moudleSwOneBit:
			case devTypeDef_moudleSwTwoBit:
			case devTypeDef_moudleSwThreeBit:{

				if(LV_BTN_STATE_TGL_REL == btnState){

					objNode_data->nodeData.devStatusVal |= (1 << objFnum_ist);
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[objFnum_ist], styleStdGblockCtrlObj_icon_pr);
				}
				else{

					objNode_data->nodeData.devStatusVal &= ~(1 << objFnum_ist);
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[objFnum_ist], styleStdGblockCtrlObj_icon_rel);
				}
				
			}break;

			case devTypeDef_infrared:{

				devStateSet_temp.devType_infrared.devInfrared_actCmd = 0;
				devStateSet_temp.devType_infrared.devInfrared_irIst = 1;

				memcpy(&objNode_data->nodeData.devStatusVal, &devStateSet_temp, sizeof(uint8_t));

			}break;
			
			case devTypeDef_socket:{

				if(LV_BTN_STATE_TGL_REL == btnState){

					devStateSet_temp.devType_socket.devSocket_opSw = 1;
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[objFnum_ist], styleStdGblockCtrlObj_icon_pr);
				}
				else{

					devStateSet_temp.devType_socket.devSocket_opSw = 0;
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[objFnum_ist], styleStdGblockCtrlObj_icon_rel);
				}
				
				memcpy(&objNode_data->nodeData.devStatusVal, &devStateSet_temp, sizeof(uint8_t));
				
			}break;
			
			case devTypeDef_thermostat:{

				if(LV_BTN_STATE_TGL_REL == btnState){

					devStateSet_temp.devType_thermostat.devThermostat_running_en = 1;
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[objFnum_ist], styleStdGblockCtrlObj_icon_pr);
				}
				else{

					devStateSet_temp.devType_thermostat.devThermostat_running_en = 0;
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[objFnum_ist], styleStdGblockCtrlObj_icon_rel);
				}

				memcpy(&objNode_data->nodeData.devStatusVal, &devStateSet_temp, sizeof(uint8_t));

			}break;
			
			case devTypeDef_heater:{

				if(LV_BTN_STATE_TGL_REL == btnState){
				
					devStateSet_temp.devType_heater.devHeater_swEnumVal = heaterOpreatAct_open;
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[objFnum_ist], styleStdGblockCtrlObj_icon_pr);
				}
				else{
				
					devStateSet_temp.devType_heater.devHeater_swEnumVal = heaterOpreatAct_close;
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[objFnum_ist], styleStdGblockCtrlObj_icon_rel);
				}
		
				memcpy(&objNode_data->nodeData.devStatusVal, &devStateSet_temp, sizeof(uint8_t));

			}break;

//			case devTypeDef_fans:{}break;			
			case devTypeDef_thermostatExtension:{

				(LV_BTN_STATE_TGL_REL == btnState)?
					(lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[objFnum_ist], styleStdGblockCtrlObj_icon_pr)):
					(lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[objFnum_ist], styleStdGblockCtrlObj_icon_rel));

				switch(objFnum_ist){

					case 0:{

						if(LV_BTN_STATE_TGL_REL == btnState){
						
							devStateSet_temp.devType_thermostat.devThermostat_running_en = 1;
						}
						else{
						
							devStateSet_temp.devType_thermostat.devThermostat_running_en = 0;
						}

					}break;
					
					case 1:{}break;
					case 2:{}break;

					default:break;
				}

				memcpy(&objNode_data->nodeData.devStatusVal, &devStateSet_temp, sizeof(uint8_t));

			}break;	
			
			case devTypeDef_scenario:{

				lv_style_t *objStyle_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {NULL};
				lv_btn_state_t objState_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM];
		
				switch(objFnum_ist){
				
					case 0:{

						devStateSet_temp.devType_scenario.devScenario_opNum = 1;
						objStyle_temp[0] = styleStdGblockCtrlObj_icon_pr;
						objStyle_temp[1] = styleStdGblockCtrlObj_icon_rel;
						objStyle_temp[2] = styleStdGblockCtrlObj_icon_rel;
						objState_temp[0] = LV_BTN_STATE_TGL_REL;
						objState_temp[1] = LV_BTN_STATE_REL;
						objState_temp[2] = LV_BTN_STATE_REL;
					}break;

					case 1:{

						devStateSet_temp.devType_scenario.devScenario_opNum = 2;
						objStyle_temp[0] = styleStdGblockCtrlObj_icon_rel;
						objStyle_temp[1] = styleStdGblockCtrlObj_icon_pr;
						objStyle_temp[2] = styleStdGblockCtrlObj_icon_rel;
						objState_temp[0] = LV_BTN_STATE_REL;
						objState_temp[1] = LV_BTN_STATE_TGL_REL;
						objState_temp[2] = LV_BTN_STATE_REL;
					}break;
					
					case 2:{

						devStateSet_temp.devType_scenario.devScenario_opNum = 4;
						objStyle_temp[0] = styleStdGblockCtrlObj_icon_rel;
						objStyle_temp[1] = styleStdGblockCtrlObj_icon_rel;
						objStyle_temp[2] = styleStdGblockCtrlObj_icon_pr;
						objState_temp[0] = LV_BTN_STATE_REL;
						objState_temp[1] = LV_BTN_STATE_REL;
						objState_temp[2] = LV_BTN_STATE_TGL_REL;
					}break;
					
					default:break;
				}

				memcpy(&objNode_data->nodeData.devStatusVal, &devStateSet_temp, sizeof(uint8_t));
				lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[0], objStyle_temp[0]);
				lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[1], objStyle_temp[1]);
				lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[2], objStyle_temp[2]);
				lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[0], objState_temp[0]);
				lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[1], objState_temp[1]);
				lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[2], objState_temp[2]);
				lv_obj_refresh_style(objNode_data->nodeData.ctrlObjIcon[0]); //控件刷新
				lv_obj_refresh_style(objNode_data->nodeData.ctrlObjIcon[1]); //控件刷新
				lv_obj_refresh_style(objNode_data->nodeData.ctrlObjIcon[2]); //控件刷新
				lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[0]); //控件刷新
				lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[1]); //控件刷新
				lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[2]); //控件刷新

			}break;		

			default:break;
		}
			
		lv_obj_refresh_style(objNode_data->nodeData.ctrlObjIcon[objFnum_ist]); //界面刷新
		dataTransBussiness_pageLinkageCfg_superCtrlActionTrig(objNode_data->nodeData.ctrlObj_devMac, objNode_data->nodeData.devStatusVal); //超控触发
	}
	else
	{
		ESP_LOGW(TAG, "imgbtnActionClick_CtrlObjBaseOpreat freenum[%d] not found", btnmFreeNum);
	}

	return LV_RES_OK;
}

static lv_res_t funCb_sliderActionSlide_CtrlObjBaseOpreat(lv_obj_t *slider){

	LV_OBJ_FREE_NUM_TYPE sliderFreeNum = lv_obj_get_free_num(slider);
	LV_OBJ_FREE_NUM_TYPE sliderFreeNumIndex = sliderFreeNum - (sliderFreeNum % GUIBUSSINESS_CTRLOBJ_MAX_NUM);

	stt_devDataPonitTypedef devState_temp = {0};
	uint8_t devState_hex = 0;
	int16_t sliderVal = lv_slider_get_value(slider);

	stt_nodeObj_listManageDevCtrlBase *objNode_data = lvglUsrApp_devCtrlBlockBaseManageList_nodeGetByLvObjFreenum(listHead_nodeCtrlObjBlockBaseManage, sliderFreeNumIndex, true);

	if(NULL != objNode_data){

		switch(objNode_data->nodeData.ctrlObj_devType){
			
			case devTypeDef_curtain:
			case devTypeDef_moudleSwCurtain:

				devState_temp.devType_curtain.devCurtain_actMethod = 1;
				devState_temp.devType_curtain.devCurtain_actEnumVal = sliderVal;

				break;
			
			case devTypeDef_dimmer:

				devState_temp.devType_dimmer.devDimmer_brightnessVal = sliderVal;

				break;

			default:break;
		}

		memcpy(&devState_hex, &devState_temp, sizeof(uint8_t));
		dataTransBussiness_pageLinkageCfg_superCtrlActionTrig(objNode_data->nodeData.ctrlObj_devMac, devState_hex);
	}

	return LV_RES_OK;
}

static lv_obj_t *lvGui_pageFunctionDetailSet_creat(const char * pageTitleName, uint8_t pageObjIst){

	if(pageLinkage_detailSetting == NULL)
		pageLinkage_detailSetting = lv_page_create(lv_scr_act(), NULL);

	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(pageLinkage_detailSetting, 280, 200)):
		(lv_obj_set_size(pageLinkage_detailSetting, 200, 240));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(pageLinkage_detailSetting, 20, 20)):
		(lv_obj_set_pos(pageLinkage_detailSetting, 20, 60));		
	lv_page_set_style(pageLinkage_detailSetting, LV_PAGE_STYLE_SB, stylePage_linkageDetailSetting);
	lv_page_set_style(pageLinkage_detailSetting, LV_PAGE_STYLE_BG, stylePage_linkageDetailSetting);
	lv_page_set_sb_mode(pageLinkage_detailSetting, LV_SB_MODE_HIDE);	
	lv_page_set_scrl_fit(pageLinkage_detailSetting, false, true); //key opration
	lv_page_set_scrl_layout(pageLinkage_detailSetting, LV_LAYOUT_PRETTY);

	labelTitle_pageLinkageDetailSetting = lv_label_create(pageLinkage_detailSetting, NULL);
	lv_obj_set_style(labelTitle_pageLinkageDetailSetting, styleTextTitle_linkageDetailSetting);
	lv_label_set_recolor(labelTitle_pageLinkageDetailSetting, true);
	lv_label_set_text(labelTitle_pageLinkageDetailSetting, pageTitleName);
	lv_obj_set_protect(labelTitle_pageLinkageDetailSetting, LV_PROTECT_POS);
	lv_obj_align(labelTitle_pageLinkageDetailSetting, pageLinkage_detailSetting, LV_ALIGN_IN_TOP_MID, 0, -5);

	btnConfirm_pageLinkageDetailSetting = lv_btn_create(pageLinkage_detailSetting, NULL);
	lv_btn_set_style(btnConfirm_pageLinkageDetailSetting, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
	lv_btn_set_style(btnConfirm_pageLinkageDetailSetting, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
	lv_btn_set_style(btnConfirm_pageLinkageDetailSetting, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
	lv_btn_set_style(btnConfirm_pageLinkageDetailSetting, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
	lv_btn_set_fit(btnConfirm_pageLinkageDetailSetting, false, false);
	lv_obj_set_size(btnConfirm_pageLinkageDetailSetting, 70, 25);
	lv_page_glue_obj(btnConfirm_pageLinkageDetailSetting, true);
	lv_obj_set_protect(btnConfirm_pageLinkageDetailSetting, LV_PROTECT_FOLLOW);
	lv_obj_set_protect(btnConfirm_pageLinkageDetailSetting, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(btnConfirm_pageLinkageDetailSetting, pageLinkage_detailSetting, LV_ALIGN_IN_BOTTOM_LEFT, 45, -1)):
		(lv_obj_align(btnConfirm_pageLinkageDetailSetting, pageLinkage_detailSetting, LV_ALIGN_IN_BOTTOM_LEFT, 25, -1));
	lv_btn_set_action(btnConfirm_pageLinkageDetailSetting, LV_BTN_ACTION_CLICK, funCb_btnActionClickDetailSet_pageChoiceSelect_confirm);
	lv_obj_set_free_num(btnConfirm_pageLinkageDetailSetting, pageObjIst);
	btnCancel_pageLinkageDetailSetting = lv_btn_create(pageLinkage_detailSetting, btnConfirm_pageLinkageDetailSetting);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(btnCancel_pageLinkageDetailSetting, pageLinkage_detailSetting, LV_ALIGN_IN_BOTTOM_RIGHT, -45, -1)):
		(lv_obj_align(btnCancel_pageLinkageDetailSetting, btnConfirm_pageLinkageDetailSetting, LV_ALIGN_CENTER, 85, 0));
	lv_btn_set_action(btnCancel_pageLinkageDetailSetting, LV_BTN_ACTION_CLICK, funCb_btnActionClickDetailSet_pageChoiceSelect_cancel);
	lv_obj_set_free_num(btnCancel_pageLinkageDetailSetting, pageObjIst);

	textBtnConfirm_pageLinkageDetailSetting = lv_label_create(btnConfirm_pageLinkageDetailSetting, NULL);
	lv_obj_set_style(textBtnConfirm_pageLinkageDetailSetting, styleTextBtn_linkageDetailSetting);
	lv_label_set_recolor(textBtnConfirm_pageLinkageDetailSetting, true);
	lv_label_set_text(textBtnConfirm_pageLinkageDetailSetting, "confirm");
	textBtnCancel_pageLinkageDetailSetting = lv_label_create(btnCancel_pageLinkageDetailSetting, textBtnConfirm_pageLinkageDetailSetting);
	lv_label_set_text(textBtnCancel_pageLinkageDetailSetting, "cancel");

	return pageLinkage_detailSetting;
}

static lv_res_t funCb_cbActionCheckDetailSet_swTrigEnable_proximity(lv_obj_t *cb){

	bool opVal_get = lv_cb_is_checked(cb);
	stt_paramLinkageConfig linkageConfigParamSet_temp = {0};
	
	devSystemOpration_linkageConfig_paramGet(&linkageConfigParamSet_temp);
	
	linkageConfigParamSet_temp.linkageReaction_proxmity_trigEn = opVal_get;
	devSystemOpration_linkageConfig_paramSet(&linkageConfigParamSet_temp, true);

	return LV_RES_OK;
}

static lv_res_t funCb_cbActionCheckDetailSet_trigScrLight_proximity(lv_obj_t *cb){

	bool opVal_get = lv_cb_is_checked(cb);
	stt_paramLinkageConfig linkageConfigParamSet_temp = {0};
	
	devSystemOpration_linkageConfig_paramGet(&linkageConfigParamSet_temp);

	cfgDataScrLight_linkageReaction_proximity = opVal_get;
	linkageConfigParamSet_temp.linkageReaction_proxmity_scrLightTrigIf = opVal_get;
	devSystemOpration_linkageConfig_paramSet(&linkageConfigParamSet_temp, true);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClickDetailSet_switchTrigSet_proximity(lv_obj_t *btn){

	stt_paramLinkageConfig devLinkageCfg_dataTemp = {0};
	lv_coord_t cy_base = 0;

	devSystemOpration_linkageConfig_paramGet(&devLinkageCfg_dataTemp);
	
	lv_obj_t *pageDetailSet = lvGui_pageFunctionDetailSet_creat("switch set", PAGEREACTION_REUSE_BUSSINESS_LINKAGESET_PROX);

	cbProximityDetection_switchTrig_en = lv_cb_create(pageDetailSet, NULL);
	lv_cb_set_action(cbProximityDetection_switchTrig_en, funCb_cbActionCheckDetailSet_swTrigEnable_proximity);
	lv_obj_set_protect(cbProximityDetection_switchTrig_en, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(cbProximityDetection_switchTrig_en, 50, 120)):
		(lv_obj_set_pos(cbProximityDetection_switchTrig_en, 40, 165));
	lv_obj_set_width(cbProximityDetection_switchTrig_en, 130);
	lv_cb_set_style(cbProximityDetection_switchTrig_en, LV_CB_STYLE_BG, styleCb_proximityDetection_switchTrigEn);
	lv_cb_set_text(cbProximityDetection_switchTrig_en, " trig enable");
	lv_cb_set_checked(cbProximityDetection_switchTrig_en, devLinkageCfg_dataTemp.linkageReaction_proxmity_trigEn);

	(devStatusDispMethod_landscapeIf_get())?
		(cy_base = 30):
		(cy_base = 60);

	lvGui_businessReuse_reactionObjPageElement_creat(pageDetailSet, PAGEREACTION_REUSE_BUSSINESS_LINKAGESET_PROX, cy_base, &(devLinkageCfg_dataTemp.linkageReaction_proxmity_swVal));

	return LV_RES_OK;
}

static lv_res_t funCb_rollerActionTrigDetailSet_conditionValSet_temprature(lv_obj_t *roller){

	uint8_t opVal_get = lv_roller_get_selected(roller);

	cfgDataTemp_linkageCondition_temprature = opVal_get;

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClickDetailSet_valThresholdAdj_temprature(lv_obj_t *btn){

	lv_obj_t *pageSetting = lvGui_pageFunctionDetailSet_creat("temprature set", PAGEREACTION_REUSE_LINKAGETEMPRATURE_THRESHOLDADJ_FNUM);

	rollerThresholdTempSet_tempratureDetection = lv_roller_create(pageSetting, NULL);
	lv_roller_set_action(rollerThresholdTempSet_tempratureDetection, funCb_rollerActionTrigDetailSet_conditionValSet_temprature);
	lv_roller_set_options(rollerThresholdTempSet_tempratureDetection, "00\n""01\n""02\n""03\n""04\n""05\n"
																	  "06\n""07\n""08\n""09\n""10\n""11\n"
																	  "12\n""13\n""14\n""15\n""16\n""17\n"
																	  "18\n""19\n""20\n""21\n""22\n""23\n"
																	  "24\n""25\n""26\n""27\n""28\n""29\n"
																	  "30\n""31\n""32\n""33\n""34\n""35\n"
																	  "36\n""37\n""38\n""39\n""40");
	lv_obj_set_protect(rollerThresholdTempSet_tempratureDetection, LV_PROTECT_POS);
	lv_roller_set_hor_fit(rollerThresholdTempSet_tempratureDetection, false);
	lv_obj_set_width(rollerThresholdTempSet_tempratureDetection, 40);
	lv_obj_align(rollerThresholdTempSet_tempratureDetection, pageSetting, LV_ALIGN_CENTER, 0, -25);
	lv_page_glue_obj(rollerThresholdTempSet_tempratureDetection, false);
	lv_roller_set_selected(rollerThresholdTempSet_tempratureDetection, 5, false);
	lv_roller_set_visible_row_count(rollerThresholdTempSet_tempratureDetection, 4);
	lv_roller_set_style(rollerThresholdTempSet_tempratureDetection, LV_ROLLER_STYLE_BG, styleRoller_bg_tempThresholdAdj);
	lv_roller_set_style(rollerThresholdTempSet_tempratureDetection, LV_ROLLER_STYLE_SEL, styleRoller_sel_tempThresholdAdj);
	lv_roller_set_selected(rollerThresholdTempSet_tempratureDetection, 
						   cfgDataTemp_linkageCondition_temprature, 
						   true);

	textRollerThresholdTempSet_tempratureDetection = lv_label_create(pageSetting, NULL);
	lv_label_set_recolor(textRollerThresholdTempSet_tempratureDetection, true);
	lv_label_set_text(textRollerThresholdTempSet_tempratureDetection, "#8000FF \"C#");
	lv_obj_set_protect(textRollerThresholdTempSet_tempratureDetection, LV_PROTECT_POS);
	lv_obj_align(textRollerThresholdTempSet_tempratureDetection, rollerThresholdTempSet_tempratureDetection, LV_ALIGN_CENTER, 33, 0);
	lv_obj_set_style(textRollerThresholdTempSet_tempratureDetection, styleTextTitle_linkageDetailSetting);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClickDetailSet_switchTrigSet_temprature(lv_obj_t *btn){

	stt_paramLinkageConfig devLinkageCfg_dataTemp = {0};
	lv_obj_t *pageDetailSet = lvGui_pageFunctionDetailSet_creat("switch set", PAGEREACTION_REUSE_BUSSINESS_LINKAGESET_TEMP);
	uint8_t cy_base = 0;

	devSystemOpration_linkageConfig_paramGet(&devLinkageCfg_dataTemp);

	(devStatusDispMethod_landscapeIf_get())?
		(cy_base = 45):
		(cy_base = 80);

	lvGui_businessReuse_reactionObjPageElement_creat(pageDetailSet, PAGEREACTION_REUSE_BUSSINESS_LINKAGESET_TEMP, cy_base, &(devLinkageCfg_dataTemp.linkageReaction_temprature_swVal));

	return LV_RES_OK;
}

static void lvGuiLinkageConfig_animationChildOptionActivity(lv_obj_t *optionObj, int32_t coordX_a, int32_t coordX_b){

	lv_anim_t a;	

	a.var = optionObj;												
	a.start = coordX_a;														
	a.end = coordX_b;								 
	a.fp = (lv_anim_fp_t)lv_obj_set_y;	   
	a.path = lv_anim_path_linear;					
	a.end_cb = NULL;							  
	a.act_time = 0; 								
	a.time = 300;								  
	a.playback = 0; 								
	a.playback_pause = 0;								  
	a.repeat = 0;								  
	a.repeat_pause = 0; 

	lv_anim_create(&a);
}

static void lvGuiLinkageConfig_childOptionObjCreat_proximityDetection(void){

	lv_anim_t a,b,c,d;	
	stt_paramLinkageConfig linkageConfigParamSet_temp = {0};
	
	devSystemOpration_linkageConfig_paramGet(&linkageConfigParamSet_temp);

	cfgDataScrLight_linkageReaction_proximity = linkageConfigParamSet_temp.linkageReaction_proxmity_scrLightTrigIf;

	lvGuiLinkageConfig_animationChildOptionActivity(textSettingA_tempratureDetection, 
													lv_obj_get_y(textSettingA_proximityDetection) + 30,
													lv_obj_get_y(textSettingA_proximityDetection) + 100);

	lvGuiLinkageConfig_animationChildOptionActivity(swReserveSet_tempratureDetection,
													lv_obj_get_y(swReserveSet_proximityDetection) + 30,
													lv_obj_get_y(swReserveSet_proximityDetection) + 100);

	if(btnTempratureDetection_tempThresholdAdj){

		lvGuiLinkageConfig_animationChildOptionActivity(btnTempratureDetection_tempThresholdAdj,
														lv_obj_get_y(swReserveSet_proximityDetection) + 30 + 15 + 15,
														lv_obj_get_y(swReserveSet_proximityDetection) + 100 + 15 + 15);

	}

	if(btnTempratureDetection_switchTrig){

		lvGuiLinkageConfig_animationChildOptionActivity(btnTempratureDetection_switchTrig,
														lv_obj_get_y(swReserveSet_proximityDetection) + 30 + 15 + 50,
														lv_obj_get_y(swReserveSet_proximityDetection) + 100 + 15 + 50);
	}

	cbProximityDetection_screenLight = lv_cb_create(page_funSetOption, NULL);
	lv_cb_set_action(cbProximityDetection_screenLight, funCb_cbActionCheckDetailSet_trigScrLight_proximity);
	lv_obj_set_protect(cbProximityDetection_screenLight, LV_PROTECT_POS);
	lv_obj_align(cbProximityDetection_screenLight, textSettingA_proximityDetection, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 17, 15);
	lv_obj_set_width(cbProximityDetection_screenLight, 175);
	lv_cb_set_style(cbProximityDetection_screenLight, LV_CB_STYLE_BG, styleCb_proximityDetection_screenLight);
	lv_cb_set_text(cbProximityDetection_screenLight, " light the screen");
	lv_cb_set_checked(cbProximityDetection_screenLight, cfgDataScrLight_linkageReaction_proximity);

	btnProximityDetection_switchTrig = lv_btn_create(page_funSetOption, NULL);
	lv_obj_set_size(btnProximityDetection_switchTrig, 200, 20);
	lv_obj_set_protect(btnProximityDetection_switchTrig, LV_PROTECT_POS);
	lv_btn_set_action(btnProximityDetection_switchTrig, LV_BTN_ACTION_CLICK, funCb_btnActionClickDetailSet_switchTrigSet_proximity);
	lv_obj_align(btnProximityDetection_switchTrig, textSettingA_proximityDetection, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + (-8), 50);
	lv_btn_set_style(btnProximityDetection_switchTrig, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
	lv_btn_set_style(btnProximityDetection_switchTrig, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
	lv_btn_set_style(btnProximityDetection_switchTrig, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
	lv_btn_set_style(btnProximityDetection_switchTrig, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
	textBtnProximityDetection_switchTrig = lv_label_create(btnProximityDetection_switchTrig, NULL);
	lv_label_set_recolor(textBtnProximityDetection_switchTrig, true);
	lv_label_set_text(textBtnProximityDetection_switchTrig, " #B5E61D > trig the switch#");
	lv_obj_set_size(textBtnProximityDetection_switchTrig, 200, 20);
	lv_label_set_align(textBtnProximityDetection_switchTrig, LV_LABEL_ALIGN_LEFT);
	lv_obj_set_style(textBtnProximityDetection_switchTrig, styleText_menuLevel_C);
	lv_obj_set_protect(textBtnProximityDetection_switchTrig, LV_PROTECT_POS);
	lv_obj_align(textBtnProximityDetection_switchTrig, btnProximityDetection_switchTrig, LV_ALIGN_CENTER, 0, 0);

	lv_obj_animate(cbProximityDetection_screenLight, LV_ANIM_FLOAT_RIGHT, 200,	0, 		NULL);
	lv_obj_animate(btnProximityDetection_switchTrig, LV_ANIM_FLOAT_RIGHT, 200,	100, 	NULL);

	lv_obj_refresh_style(page_funSetOption);
}

static void lvGuiLinkageConfig_childOptionObjDelete_proximityDetection(void){

	lv_obj_del(cbProximityDetection_screenLight);
	cbProximityDetection_screenLight = NULL;
	lv_obj_del(btnProximityDetection_switchTrig);
	btnProximityDetection_switchTrig = NULL;

	lvGuiLinkageConfig_animationChildOptionActivity(textSettingA_tempratureDetection,
													lv_obj_get_y(textSettingA_proximityDetection) + 100,
													lv_obj_get_y(textSettingA_proximityDetection) + 30);

	lvGuiLinkageConfig_animationChildOptionActivity(swReserveSet_tempratureDetection,
													lv_obj_get_y(swReserveSet_proximityDetection) + 100,
													lv_obj_get_y(swReserveSet_proximityDetection) + 30);

	if(btnTempratureDetection_tempThresholdAdj){

		lvGuiLinkageConfig_animationChildOptionActivity(btnTempratureDetection_tempThresholdAdj,
														lv_obj_get_y(swReserveSet_proximityDetection) + 100 + 15 + 15,
														lv_obj_get_y(swReserveSet_proximityDetection) + 30 + 15 + 15);
	}

	if(btnTempratureDetection_switchTrig){

		lvGuiLinkageConfig_animationChildOptionActivity(btnTempratureDetection_switchTrig,
														lv_obj_get_y(swReserveSet_proximityDetection) + 100 + 15 + 50,
														lv_obj_get_y(swReserveSet_proximityDetection) + 30 + 15 + 50);
	}

	lv_obj_refresh_style(page_funSetOption);
}

static void lvGuiLinkageConfig_childOptionObjCreat_tempratureDetection(void){

	stt_paramLinkageConfig linkageConfigParamSet_temp = {0};
	
	devSystemOpration_linkageConfig_paramGet(&linkageConfigParamSet_temp);

	cfgDataTemp_linkageCondition_temprature = linkageConfigParamSet_temp.linkageCondition_tempratureVal;

	btnTempratureDetection_tempThresholdAdj = lv_btn_create(page_funSetOption, NULL);
	lv_obj_set_size(btnTempratureDetection_tempThresholdAdj, 200, 20);
	lv_obj_set_protect(btnTempratureDetection_tempThresholdAdj, LV_PROTECT_POS);
	lv_btn_set_action(btnTempratureDetection_tempThresholdAdj, LV_BTN_ACTION_CLICK, funCb_btnActionClickDetailSet_valThresholdAdj_temprature);
	lv_obj_align(btnTempratureDetection_tempThresholdAdj, textSettingA_tempratureDetection, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 16, 15);
	lv_btn_set_style(btnTempratureDetection_tempThresholdAdj, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
	lv_btn_set_style(btnTempratureDetection_tempThresholdAdj, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
	lv_btn_set_style(btnTempratureDetection_tempThresholdAdj, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
	lv_btn_set_style(btnTempratureDetection_tempThresholdAdj, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
	textBtnTempratureDetection_tempThresholdAdj = lv_label_create(btnTempratureDetection_tempThresholdAdj, NULL);
	lv_label_set_recolor(textBtnTempratureDetection_tempThresholdAdj, true);
	sprintf(strTemp_textBtnTempratureDetection_tempThresholdAdj, " #00A2E8 > temprature set##C0C0FF [%d\"C]#", cfgDataTemp_linkageCondition_temprature);
	lv_label_set_text(textBtnTempratureDetection_tempThresholdAdj, strTemp_textBtnTempratureDetection_tempThresholdAdj);
	lv_obj_set_size(textBtnTempratureDetection_tempThresholdAdj, 200, 20);
	lv_label_set_align(textBtnTempratureDetection_tempThresholdAdj, LV_LABEL_ALIGN_LEFT);
	lv_obj_set_style(textBtnTempratureDetection_tempThresholdAdj, styleText_menuLevel_C);
	lv_obj_set_protect(textBtnTempratureDetection_tempThresholdAdj, LV_PROTECT_POS);
	lv_obj_align(textBtnTempratureDetection_tempThresholdAdj, btnTempratureDetection_tempThresholdAdj, LV_ALIGN_CENTER, 0, 0);

	btnTempratureDetection_switchTrig = lv_btn_create(page_funSetOption, NULL);
	lv_obj_set_size(btnTempratureDetection_switchTrig, 200, 20);
	lv_obj_set_protect(btnTempratureDetection_switchTrig, LV_PROTECT_POS);
	lv_btn_set_action(btnTempratureDetection_switchTrig, LV_BTN_ACTION_CLICK, funCb_btnActionClickDetailSet_switchTrigSet_temprature);
	lv_obj_align(btnTempratureDetection_switchTrig, textSettingA_tempratureDetection, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + (-8), 50);
	lv_btn_set_style(btnTempratureDetection_switchTrig, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
	lv_btn_set_style(btnTempratureDetection_switchTrig, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
	lv_btn_set_style(btnTempratureDetection_switchTrig, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
	lv_btn_set_style(btnTempratureDetection_switchTrig, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
	textBtnTempratureDetection_switchTrig = lv_label_create(btnTempratureDetection_switchTrig, NULL);
	lv_label_set_recolor(textBtnTempratureDetection_switchTrig, true);
	lv_label_set_text(textBtnTempratureDetection_switchTrig, " #B5E61D > trig the switch#");
	lv_obj_set_size(textBtnTempratureDetection_switchTrig, 200, 20);
	lv_label_set_align(textBtnTempratureDetection_switchTrig, LV_LABEL_ALIGN_LEFT);
	lv_obj_set_style(textBtnTempratureDetection_switchTrig, styleText_menuLevel_C);
	lv_obj_set_protect(textBtnTempratureDetection_switchTrig, LV_PROTECT_POS);
	lv_obj_align(textBtnTempratureDetection_switchTrig, btnTempratureDetection_switchTrig, LV_ALIGN_CENTER, 0, 0);

	lv_obj_animate(btnTempratureDetection_tempThresholdAdj, LV_ANIM_FLOAT_RIGHT, 200,	0, 		NULL);
	lv_obj_animate(btnTempratureDetection_switchTrig, 		LV_ANIM_FLOAT_RIGHT, 200,	100, 	NULL);
}

static void lvGuiLinkageConfig_childOptionObjDelete_tempratureDetection(void){

	lv_obj_del(btnTempratureDetection_tempThresholdAdj);
	btnTempratureDetection_tempThresholdAdj = NULL;
	lv_obj_del(btnTempratureDetection_switchTrig);
	btnTempratureDetection_switchTrig = NULL;
}

void lvGuiLinkageConfig_devGraphCtrlBlock_listLoadInitialization(stt_nodeObj_listManageDevCtrlBase *devNodeListHead, lv_obj_t *obj_Parent){

	const uint8_t coordErrObj_x = 0,
				  coordErrObj_y = 8,
				  coordErrObj_icon_x = 0,
				  coordErrObj_icon_y = 0,
				  coordErrObj_name_x = 0,
				  coordErrObj_name_y = 2;

	struct stt_devStatis{

		uint8_t sum_devSwit;
		uint8_t sum_devCur;
		uint8_t sum_devDim;
		uint8_t sum_devScen;
		uint8_t sum_devFan;
		uint8_t sum_devHeat;
		uint8_t sum_devTher;
		uint8_t sum_devSoc;
		uint8_t sum_devInfr;
	}devStatistical = {0};
	uint8_t devInfoObjAdd_ist = 0;
	lv_obj_t *label_devListStatisInfo = NULL;
	char strDevListStatisInfo[256] = {0},
		 strStatisInfoHandle[32] = {0};
	
	stt_nodeObj_listManageDevCtrlBase *pTemp = devNodeListHead;
	uint8_t devSelfMac[MWIFI_ADDR_LEN] = {0}; 
	uint8_t gBlockCreat_count = 0;
	uint8_t gBlockList_num = lvglUsrApp_devCtrlBlockBaseManageList_nodeNumDetect(devNodeListHead);

	label_devListStatisInfo = lv_label_create(obj_Parent, NULL);
	lv_label_set_align(label_devListStatisInfo, LV_LABEL_ALIGN_LEFT);
	lv_label_set_recolor(label_devListStatisInfo, true);
	lv_obj_set_protect(label_devListStatisInfo, LV_PROTECT_POS);

	if(false == mwifi_is_connected()){

		lv_label_set_align(label_devListStatisInfo, LV_LABEL_ALIGN_CENTER);
		lv_label_set_text(label_devListStatisInfo, "#FF2020 OFFLINE#");
		lv_obj_align(label_devListStatisInfo, obj_Parent, LV_ALIGN_CENTER, 0, -20);
	
		return;
	}
	
	lvGui_usrAppBussinessRunning_block(0, "\nlist generateing", (gBlockList_num > 3)?(gBlockList_num / 3):(1));

//	stt_nodeObj_listManageDevCtrlBase gNode_new = {

//		.nodeData.ctrlObj_devMac = {0xbc, 0xdd, 0xc2, 0xc6, 0xfb, 0x50},
//		.nodeData.ctrlObj_devType = devTypeDef_mulitSwThreeBit,
//			
//		.nodeData.devStatusVal = 2,

//		.nodeData.objIcon_ist = {1, 2, 3},
//		.nodeData.objCtrl_name = {"abc", "def", "ghi"},
//	};

//	lvglUsrApp_devCtrlBlockBaseManageList_nodeAdd(listHead_nodeCtrlObjBlockBaseManage, &gNode_new);

	esp_wifi_get_mac(ESP_IF_WIFI_STA, devSelfMac);

	if(styleStdGblockCtrlObj_baseBlock_SELF == NULL)styleStdGblockCtrlObj_baseBlock_SELF = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	if(styleStdGblockCtrlObj_baseBlock_A == NULL)styleStdGblockCtrlObj_baseBlock_A = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	if(styleStdGblockCtrlObj_baseBlock_B == NULL)styleStdGblockCtrlObj_baseBlock_B = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	if(styleStdGblockCtrlObj_baseBlock_C == NULL)styleStdGblockCtrlObj_baseBlock_C = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	if(styleStdGblockCtrlObj_baseBlock_D == NULL)styleStdGblockCtrlObj_baseBlock_D = (lv_style_t *)os_zalloc(sizeof(lv_style_t));

	if(styleStdGblockCtrlObj_imgbtn_pr == NULL)styleStdGblockCtrlObj_imgbtn_pr = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	if(styleStdGblockCtrlObj_imgbtn_rel == NULL)styleStdGblockCtrlObj_imgbtn_rel = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	if(styleStdGblockCtrlObj_icon_pr == NULL)styleStdGblockCtrlObj_icon_pr = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	if(styleStdGblockCtrlObj_icon_rel == NULL)styleStdGblockCtrlObj_icon_rel = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	if(styleStdGblockCtrlObj_labSelf == NULL)styleStdGblockCtrlObj_labSelf = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	if(styleStdGblockCtrlObj_labOther == NULL)styleStdGblockCtrlObj_labOther = (lv_style_t *)os_zalloc(sizeof(lv_style_t));

	lv_style_copy(styleStdGblockCtrlObj_baseBlock_A, &lv_style_plain);
	styleStdGblockCtrlObj_baseBlock_A->body.border.part = LV_BORDER_NONE;
	styleStdGblockCtrlObj_baseBlock_A->body.radius = 20;
	styleStdGblockCtrlObj_baseBlock_A->body.main_color = LV_COLOR_GRAY;
	styleStdGblockCtrlObj_baseBlock_A->body.grad_color = LV_COLOR_MAKE(0xE0, 0xE0, 0xE0);
	lv_style_copy(styleStdGblockCtrlObj_baseBlock_B, styleStdGblockCtrlObj_baseBlock_A);
	styleStdGblockCtrlObj_baseBlock_B->body.grad_color = LV_COLOR_MAKE(0x99, 0xED, 0xEA);
	lv_style_copy(styleStdGblockCtrlObj_baseBlock_C, styleStdGblockCtrlObj_baseBlock_A);
	styleStdGblockCtrlObj_baseBlock_C->body.grad_color = LV_COLOR_MAKE(0xE0, 0xFF, 0xFF);
	lv_style_copy(styleStdGblockCtrlObj_baseBlock_D, styleStdGblockCtrlObj_baseBlock_A);
	styleStdGblockCtrlObj_baseBlock_D->body.grad_color = LV_COLOR_MAKE(0xE0, 0xFF, 0xE0);

	lv_style_copy(styleStdGblockCtrlObj_imgbtn_pr, &lv_style_plain);
	styleStdGblockCtrlObj_imgbtn_pr->image.intense = LV_OPA_70;
	styleStdGblockCtrlObj_imgbtn_pr->image.opa = LV_OPA_80;
	styleStdGblockCtrlObj_imgbtn_pr->image.color = LV_COLOR_MAKE(0x00, 0x80, 0xFF);
	lv_style_copy(styleStdGblockCtrlObj_imgbtn_rel, styleStdGblockCtrlObj_imgbtn_pr);
	styleStdGblockCtrlObj_imgbtn_rel->image.color = LV_COLOR_WHITE;

	lv_style_copy(styleStdGblockCtrlObj_icon_pr, &lv_style_plain);
	styleStdGblockCtrlObj_icon_pr->image.intense = LV_OPA_COVER;
	styleStdGblockCtrlObj_icon_pr->image.opa = LV_OPA_80;
	styleStdGblockCtrlObj_icon_pr->image.color = LV_COLOR_WHITE;
	lv_style_copy(styleStdGblockCtrlObj_icon_rel, styleStdGblockCtrlObj_icon_pr);
	styleStdGblockCtrlObj_icon_rel->image.color = LV_COLOR_GRAY;

	lv_style_copy(styleStdGblockCtrlObj_labSelf, &lv_style_plain);
	styleStdGblockCtrlObj_labSelf->text.font = &lv_font_consola_13;
	styleStdGblockCtrlObj_labSelf->text.color = LV_COLOR_MAKE(0x60, 0x60, 0x60);
	lv_style_copy(styleStdGblockCtrlObj_labOther, &lv_style_plain);
	styleStdGblockCtrlObj_labOther->text.font = &lv_font_consola_13;
	styleStdGblockCtrlObj_labOther->text.color = LV_COLOR_MAKE(0xE0, 0xE0, 0xE0);

	while(pTemp->next != NULL){

//		printf("gBlock unit devType:%02X.\n", pTemp->next->nodeData.ctrlObj_devType);

		styleStdGblockCtrlObjLabel_ptr = styleStdGblockCtrlObj_labOther;

		pTemp->next->nodeData.graphBaseBlock = lv_obj_create(obj_Parent, NULL);
		lv_page_glue_obj(pTemp->next->nodeData.graphBaseBlock, true);
		lv_obj_set_size(pTemp->next->nodeData.graphBaseBlock, devOpt_gBlockBase_sizeX, devOpt_gBlockBase_sizeY);
		switch(pTemp->next->nodeData.ctrlObj_devType){

			case devTypeDef_mulitSwOneBit:
			case devTypeDef_mulitSwTwoBit:
			case devTypeDef_mulitSwThreeBit:
			case devTypeDef_moudleSwOneBit:
			case devTypeDef_moudleSwTwoBit:
			case devTypeDef_moudleSwThreeBit:
					lv_obj_set_style(pTemp->next->nodeData.graphBaseBlock, styleStdGblockCtrlObj_baseBlock_A);
					break;

			case devTypeDef_dimmer:
			case devTypeDef_curtain:
			case devTypeDef_moudleSwCurtain:
					lv_obj_set_style(pTemp->next->nodeData.graphBaseBlock, styleStdGblockCtrlObj_baseBlock_B);
					break;

			case devTypeDef_socket:
			case devTypeDef_heater:
			case devTypeDef_infrared:
					lv_obj_set_style(pTemp->next->nodeData.graphBaseBlock, styleStdGblockCtrlObj_baseBlock_C);
					break;

			default:lv_obj_set_style(pTemp->next->nodeData.graphBaseBlock, styleStdGblockCtrlObj_baseBlock_D);
					break;
		}
		
		if(0 == memcmp(pTemp->next->nodeData.ctrlObj_devMac, devSelfMac, sizeof(uint8_t) * MWIFI_ADDR_LEN)){ //本机设备block标记

			lv_style_t *styleTemp = lv_obj_get_style(pTemp->next->nodeData.graphBaseBlock);
			
			lv_style_copy(styleStdGblockCtrlObj_baseBlock_SELF, styleTemp);
			styleStdGblockCtrlObj_baseBlock_SELF->body.main_color = LV_COLOR_MAKE(0xFF, 0xE0, 0xE0);
			lv_obj_set_style(pTemp->next->nodeData.graphBaseBlock, styleStdGblockCtrlObj_baseBlock_SELF);

			styleStdGblockCtrlObjLabel_ptr = styleStdGblockCtrlObj_labSelf;
		}
	
		switch(pTemp->next->nodeData.ctrlObj_devType){

			case devTypeDef_mulitSwOneBit:
			case devTypeDef_mulitSwTwoBit:
			case devTypeDef_mulitSwThreeBit:
			case devTypeDef_moudleSwOneBit:
			case devTypeDef_moudleSwTwoBit:
			case devTypeDef_moudleSwThreeBit:{

				uint8_t loopCreat = 0,
						countCreat = 0;
				
				switch(pTemp->next->nodeData.ctrlObj_devType){

					case devTypeDef_mulitSwTwoBit:
					case devTypeDef_moudleSwTwoBit:loopCreat = 2;break;
					case devTypeDef_mulitSwThreeBit:
					case devTypeDef_moudleSwThreeBit:loopCreat = 3;break;

					default:loopCreat = 1;break;
				}

				for(countCreat = 0; countCreat < loopCreat; countCreat ++){

					if(0 == countCreat){

						pTemp->next->nodeData.imgCtrlObj[countCreat] = lv_imgbtn_create(pTemp->next->nodeData.graphBaseBlock, NULL);
						lv_imgbtn_set_toggle(pTemp->next->nodeData.imgCtrlObj[countCreat], true);
						lv_obj_set_protect(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_PROTECT_POS);
						lv_imgbtn_set_src(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_REL, &scrsaverStatusIconBk);
						lv_imgbtn_set_src(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_PR, &scrsaverStatusIconBk);
						lv_imgbtn_set_src(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_TGL_REL, &scrsaverStatusIconBk);
						lv_imgbtn_set_src(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_TGL_PR, &scrsaverStatusIconBk);
						lv_imgbtn_set_style(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_IMGBTN_STYLE_REL, styleStdGblockCtrlObj_imgbtn_rel);	
						lv_imgbtn_set_style(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_IMGBTN_STYLE_TGL_REL, styleStdGblockCtrlObj_imgbtn_pr);
						pTemp->next->nodeData.lv_fNum_objImgCtrl[countCreat] = gBlockCreat_count * GUIBUSSINESS_CTRLOBJ_MAX_NUM + countCreat;
						lv_obj_set_free_num(pTemp->next->nodeData.imgCtrlObj[countCreat], pTemp->next->nodeData.lv_fNum_objImgCtrl[countCreat]);
						lv_imgbtn_set_action(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_ACTION_CLICK, funCb_imgbtnActionClick_CtrlObjBaseOpreat);
						(pTemp->next->nodeData.devStatusVal & (1 << countCreat))?
							lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_TGL_REL):
							lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_REL);
						
						pTemp->next->nodeData.ctrlObjIcon[countCreat] = lv_img_create(pTemp->next->nodeData.imgCtrlObj[countCreat], NULL);
						lv_obj_set_protect(pTemp->next->nodeData.ctrlObjIcon[countCreat], LV_PROTECT_POS);
						lv_img_set_src(pTemp->next->nodeData.ctrlObjIcon[countCreat], usrAppHomepageBtnIconDisp_dataGet(pTemp->next->nodeData.objIcon_ist[countCreat]));
						(pTemp->next->nodeData.devStatusVal & (1 << countCreat))?
							lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[countCreat], styleStdGblockCtrlObj_icon_pr):
							lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[countCreat], styleStdGblockCtrlObj_icon_rel);

						pTemp->next->nodeData.nameTextDispObj[countCreat] = lv_label_create(pTemp->next->nodeData.graphBaseBlock, NULL);
						lv_label_set_align(pTemp->next->nodeData.nameTextDispObj[countCreat], LV_LABEL_ALIGN_CENTER);
						lv_label_set_long_mode(pTemp->next->nodeData.nameTextDispObj[countCreat], LV_LABEL_LONG_DOT);
						lv_obj_set_size(pTemp->next->nodeData.nameTextDispObj[countCreat], 60, 20);
						lv_label_set_text(pTemp->next->nodeData.nameTextDispObj[countCreat], (const char*)pTemp->next->nodeData.objCtrl_name[countCreat]);
						lv_obj_set_protect(pTemp->next->nodeData.nameTextDispObj[countCreat], LV_PROTECT_POS);
						lv_label_set_style(pTemp->next->nodeData.nameTextDispObj[countCreat], styleStdGblockCtrlObjLabel_ptr);
					}
					else{

						pTemp->next->nodeData.imgCtrlObj[countCreat] = lv_imgbtn_create(pTemp->next->nodeData.graphBaseBlock, pTemp->next->nodeData.imgCtrlObj[0]);
						pTemp->next->nodeData.lv_fNum_objImgCtrl[countCreat] = gBlockCreat_count * GUIBUSSINESS_CTRLOBJ_MAX_NUM + countCreat;
						lv_obj_set_free_num(pTemp->next->nodeData.imgCtrlObj[countCreat], pTemp->next->nodeData.lv_fNum_objImgCtrl[countCreat]);
						(pTemp->next->nodeData.devStatusVal & (1 << countCreat))?
							lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_TGL_REL):
							lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_REL);

						pTemp->next->nodeData.ctrlObjIcon[countCreat] = lv_img_create(pTemp->next->nodeData.imgCtrlObj[countCreat], pTemp->next->nodeData.ctrlObjIcon[0]);
						lv_img_set_src(pTemp->next->nodeData.ctrlObjIcon[countCreat], usrAppHomepageBtnIconDisp_dataGet(pTemp->next->nodeData.objIcon_ist[countCreat]));
						(pTemp->next->nodeData.devStatusVal & (1 << countCreat))?
							lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[countCreat], styleStdGblockCtrlObj_icon_pr):
							lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[countCreat], styleStdGblockCtrlObj_icon_rel);
						
						pTemp->next->nodeData.nameTextDispObj[countCreat] = lv_label_create(pTemp->next->nodeData.graphBaseBlock, pTemp->next->nodeData.nameTextDispObj[0]);
						lv_label_set_text(pTemp->next->nodeData.nameTextDispObj[countCreat], (const char*)pTemp->next->nodeData.objCtrl_name[countCreat]);
					}
				}

				switch(pTemp->next->nodeData.ctrlObj_devType){

					case devTypeDef_mulitSwTwoBit:
					case devTypeDef_moudleSwTwoBit:{

						lv_obj_align(pTemp->next->nodeData.imgCtrlObj[0], pTemp->next->nodeData.graphBaseBlock, LV_ALIGN_CENTER, -50, coordErrObj_y);
						lv_obj_align(pTemp->next->nodeData.imgCtrlObj[1], pTemp->next->nodeData.graphBaseBlock, LV_ALIGN_CENTER, 50, coordErrObj_y);

					}break;
					
					case devTypeDef_mulitSwThreeBit:
					case devTypeDef_moudleSwThreeBit:{

						lv_obj_align(pTemp->next->nodeData.imgCtrlObj[1], pTemp->next->nodeData.graphBaseBlock, LV_ALIGN_CENTER, 0, coordErrObj_y);
						lv_obj_align(pTemp->next->nodeData.imgCtrlObj[0], pTemp->next->nodeData.imgCtrlObj[1], LV_ALIGN_OUT_LEFT_BOTTOM, -20, 0);
						lv_obj_align(pTemp->next->nodeData.imgCtrlObj[2], pTemp->next->nodeData.imgCtrlObj[1], LV_ALIGN_OUT_RIGHT_BOTTOM, 20, 0);

					}break;
					
					default:{

						lv_obj_align(pTemp->next->nodeData.imgCtrlObj[0], pTemp->next->nodeData.graphBaseBlock, LV_ALIGN_CENTER, 0, coordErrObj_y);

					}break;
				}

				for(countCreat = 0; countCreat < loopCreat; countCreat ++){

					lv_obj_align(pTemp->next->nodeData.ctrlObjIcon[countCreat], pTemp->next->nodeData.imgCtrlObj[countCreat], LV_ALIGN_CENTER, coordErrObj_icon_x, coordErrObj_icon_y);
					lv_obj_align(pTemp->next->nodeData.nameTextDispObj[countCreat], pTemp->next->nodeData.imgCtrlObj[countCreat], LV_ALIGN_OUT_TOP_MID, coordErrObj_name_x, coordErrObj_name_y);
				}

			}break;

			case devTypeDef_curtain:
			case devTypeDef_moudleSwCurtain:
			case devTypeDef_dimmer:{

				stt_devDataPonitTypedef devStateTemp = {0};

				memcpy(&devStateTemp, &pTemp->next->nodeData.devStatusVal, sizeof(stt_devDataPonitTypedef));

				pTemp->next->nodeData.imgCtrlObj[0] = lv_slider_create(pTemp->next->nodeData.graphBaseBlock, NULL);
				lv_obj_set_size(pTemp->next->nodeData.imgCtrlObj[0], 175, 35);
				lv_obj_set_protect(pTemp->next->nodeData.imgCtrlObj[0], LV_PROTECT_POS);
				lv_obj_align(pTemp->next->nodeData.imgCtrlObj[0], pTemp->next->nodeData.graphBaseBlock, LV_ALIGN_CENTER, 0, coordErrObj_y);
				lv_slider_set_action(pTemp->next->nodeData.imgCtrlObj[0], funCb_sliderActionSlide_CtrlObjBaseOpreat);
				lv_slider_set_range(pTemp->next->nodeData.imgCtrlObj[0], 0, 100);
				pTemp->next->nodeData.lv_fNum_objImgCtrl[0] = gBlockCreat_count * GUIBUSSINESS_CTRLOBJ_MAX_NUM + 0;
				lv_obj_set_free_num(pTemp->next->nodeData.imgCtrlObj[0], pTemp->next->nodeData.lv_fNum_objImgCtrl[0]);				

				pTemp->next->nodeData.nameTextDispObj[0] = lv_label_create(pTemp->next->nodeData.graphBaseBlock, NULL);
				lv_label_set_align(pTemp->next->nodeData.nameTextDispObj[0], LV_LABEL_ALIGN_CENTER);
				lv_label_set_long_mode(pTemp->next->nodeData.nameTextDispObj[0], LV_LABEL_LONG_DOT);
				lv_obj_set_size(pTemp->next->nodeData.nameTextDispObj[0], 120, 20);
				lv_label_set_text(pTemp->next->nodeData.nameTextDispObj[0], (const char*)pTemp->next->nodeData.objCtrl_name[0]);
				lv_obj_set_protect(pTemp->next->nodeData.nameTextDispObj[0], LV_PROTECT_POS);
				lv_obj_align(pTemp->next->nodeData.nameTextDispObj[0], pTemp->next->nodeData.imgCtrlObj[0], LV_ALIGN_OUT_TOP_MID, coordErrObj_name_x, coordErrObj_name_y - 5);
				lv_label_set_style(pTemp->next->nodeData.nameTextDispObj[0], styleStdGblockCtrlObjLabel_ptr);

				pTemp->next->nodeData.ctrlObjIcon[0] = lv_label_create(pTemp->next->nodeData.graphBaseBlock, pTemp->next->nodeData.nameTextDispObj[0]);
				lv_label_set_recolor(pTemp->next->nodeData.ctrlObjIcon[0], true);
				lv_obj_set_protect(pTemp->next->nodeData.ctrlObjIcon[0], LV_PROTECT_POS);
				lv_obj_align(pTemp->next->nodeData.ctrlObjIcon[0], pTemp->next->nodeData.graphBaseBlock, LV_ALIGN_IN_TOP_LEFT, -25, 5);

				switch(pTemp->next->nodeData.ctrlObj_devType){

					case devTypeDef_curtain:
					case devTypeDef_moudleSwCurtain:
						lv_slider_set_value(pTemp->next->nodeData.imgCtrlObj[0], devStateTemp.devType_curtain.devCurtain_actEnumVal);
						lv_label_set_text(pTemp->next->nodeData.ctrlObjIcon[0], "#FFFF80 Cur#");
						break;
					
					case devTypeDef_dimmer:
						lv_slider_set_value(pTemp->next->nodeData.imgCtrlObj[0], devStateTemp.devType_dimmer.devDimmer_brightnessVal);
						lv_label_set_text(pTemp->next->nodeData.ctrlObjIcon[0], "#FFFF80 Dim#");
						break;

					default:break;
				}

			}break;

			case devTypeDef_infrared:
			case devTypeDef_socket:
			case devTypeDef_thermostat:
			case devTypeDef_heater:{

				stt_devDataPonitTypedef devStateTemp = {0};
				
				memcpy(&devStateTemp, &pTemp->next->nodeData.devStatusVal, sizeof(stt_devDataPonitTypedef));

				pTemp->next->nodeData.imgCtrlObj[0] = lv_imgbtn_create(pTemp->next->nodeData.graphBaseBlock, NULL);
				lv_imgbtn_set_toggle(pTemp->next->nodeData.imgCtrlObj[0], true);
				lv_obj_set_protect(pTemp->next->nodeData.imgCtrlObj[0], LV_PROTECT_POS);
				lv_imgbtn_set_src(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_REL, &scrsaverStatusIconBk);
				lv_imgbtn_set_src(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_PR, &scrsaverStatusIconBk);
				lv_imgbtn_set_src(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_TGL_REL, &scrsaverStatusIconBk);
				lv_imgbtn_set_src(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_TGL_PR, &scrsaverStatusIconBk);
				lv_imgbtn_set_style(pTemp->next->nodeData.imgCtrlObj[0], LV_IMGBTN_STYLE_REL, styleStdGblockCtrlObj_imgbtn_rel);	
				lv_imgbtn_set_style(pTemp->next->nodeData.imgCtrlObj[0], LV_IMGBTN_STYLE_TGL_REL, styleStdGblockCtrlObj_imgbtn_pr);
				pTemp->next->nodeData.lv_fNum_objImgCtrl[0] = gBlockCreat_count * GUIBUSSINESS_CTRLOBJ_MAX_NUM + 0;
				lv_obj_set_free_num(pTemp->next->nodeData.imgCtrlObj[0], pTemp->next->nodeData.lv_fNum_objImgCtrl[0]);
				lv_imgbtn_set_action(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_ACTION_CLICK, funCb_imgbtnActionClick_CtrlObjBaseOpreat);

				pTemp->next->nodeData.nameTextDispObj[0] = lv_label_create(pTemp->next->nodeData.graphBaseBlock, NULL);
				lv_label_set_align(pTemp->next->nodeData.nameTextDispObj[0], LV_LABEL_ALIGN_CENTER);
				lv_label_set_long_mode(pTemp->next->nodeData.nameTextDispObj[0], LV_LABEL_LONG_DOT);
				lv_obj_set_size(pTemp->next->nodeData.nameTextDispObj[0], 120, 20);
				lv_label_set_text(pTemp->next->nodeData.nameTextDispObj[0], (const char*)pTemp->next->nodeData.objCtrl_name[0]);
				lv_obj_set_protect(pTemp->next->nodeData.nameTextDispObj[0], LV_PROTECT_POS);
				lv_obj_align(pTemp->next->nodeData.nameTextDispObj[0], pTemp->next->nodeData.imgCtrlObj[0], LV_ALIGN_OUT_TOP_MID, coordErrObj_name_x, coordErrObj_name_y);
				lv_label_set_style(pTemp->next->nodeData.nameTextDispObj[0], styleStdGblockCtrlObjLabel_ptr);

				pTemp->next->nodeData.ctrlObjIcon[0] = lv_img_create(pTemp->next->nodeData.imgCtrlObj[0], NULL);
				lv_obj_set_protect(pTemp->next->nodeData.ctrlObjIcon[0], LV_PROTECT_POS);
				lv_img_set_src(pTemp->next->nodeData.ctrlObjIcon[0], usrAppHomepageBtnIconDisp_dataGet(pTemp->next->nodeData.objIcon_ist[0]));

				pTemp->next->nodeData.ctrlObjIcon[1] = lv_label_create(pTemp->next->nodeData.graphBaseBlock, pTemp->next->nodeData.nameTextDispObj[0]);
				lv_label_set_recolor(pTemp->next->nodeData.ctrlObjIcon[1], true);
				lv_obj_set_protect(pTemp->next->nodeData.ctrlObjIcon[1], LV_PROTECT_POS);
				lv_obj_align(pTemp->next->nodeData.ctrlObjIcon[1], pTemp->next->nodeData.graphBaseBlock, LV_ALIGN_IN_TOP_LEFT, -25, 5);

				switch(pTemp->next->nodeData.ctrlObj_devType){

					case devTypeDef_socket:{
					
						(devStateTemp.devType_socket.devSocket_opSw)?
							lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_TGL_REL):
							lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_REL);
						(devStateTemp.devType_socket.devSocket_opSw)?
							lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_pr):
							lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_rel);

						lv_label_set_text(pTemp->next->nodeData.ctrlObjIcon[1], "#FFFF80 Soc#");
	
					}break;
					
					case devTypeDef_thermostat:{

						(devStateTemp.devType_thermostat.devThermostat_running_en)?
							lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_TGL_REL):
							lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_REL);
						(devStateTemp.devType_thermostat.devThermostat_running_en)?
							lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_pr):
							lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_rel);

						lv_label_set_text(pTemp->next->nodeData.ctrlObjIcon[1], "#FFFF80 Therm#");

					}break;
					
					case devTypeDef_heater:{

						(heaterOpreatAct_close != devStateTemp.devType_heater.devHeater_swEnumVal)?
							lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_TGL_REL):
							lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_REL);
						(heaterOpreatAct_close != devStateTemp.devType_heater.devHeater_swEnumVal)?
							lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_pr):
							lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_rel);

						lv_label_set_text(pTemp->next->nodeData.ctrlObjIcon[1], "#FFFF80 Hea#");

					}break;
					
					case devTypeDef_infrared:{

						lv_imgbtn_set_toggle(pTemp->next->nodeData.imgCtrlObj[0], false);
						lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_REL);
						lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_rel);

						lv_label_set_text(pTemp->next->nodeData.ctrlObjIcon[1], "#FFFF80 Infra#");

					}break;

					default:break;
				}

				lv_obj_align(pTemp->next->nodeData.imgCtrlObj[0], pTemp->next->nodeData.graphBaseBlock, LV_ALIGN_CENTER, 0, coordErrObj_y);
				lv_obj_align(pTemp->next->nodeData.ctrlObjIcon[0], pTemp->next->nodeData.imgCtrlObj[0], LV_ALIGN_CENTER, coordErrObj_icon_x, coordErrObj_icon_y);
				lv_obj_align(pTemp->next->nodeData.nameTextDispObj[0], pTemp->next->nodeData.imgCtrlObj[0], LV_ALIGN_OUT_TOP_MID, coordErrObj_name_x, coordErrObj_name_y);

			}break;

//			case devTypeDef_fans:				
			case devTypeDef_thermostatExtension:
			case devTypeDef_scenario:{

				uint8_t countCreat = 0;
				stt_devDataPonitTypedef devStateTemp = {0};
				
				memcpy(&devStateTemp, &pTemp->next->nodeData.devStatusVal, sizeof(stt_devDataPonitTypedef));

				for(countCreat = 0; countCreat < 3; countCreat ++){

					pTemp->next->nodeData.imgCtrlObj[countCreat] = lv_imgbtn_create(pTemp->next->nodeData.graphBaseBlock, NULL);
					lv_imgbtn_set_toggle(pTemp->next->nodeData.imgCtrlObj[countCreat], true);
					lv_obj_set_protect(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_PROTECT_POS);
					lv_imgbtn_set_src(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_REL, &scrsaverStatusIconBk);
					lv_imgbtn_set_src(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_PR, &scrsaverStatusIconBk);
					lv_imgbtn_set_src(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_TGL_REL, &scrsaverStatusIconBk);
					lv_imgbtn_set_src(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_TGL_PR, &scrsaverStatusIconBk);
					lv_imgbtn_set_style(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_IMGBTN_STYLE_REL, styleStdGblockCtrlObj_imgbtn_rel);	
					lv_imgbtn_set_style(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_IMGBTN_STYLE_TGL_REL, styleStdGblockCtrlObj_imgbtn_pr);
					pTemp->next->nodeData.lv_fNum_objImgCtrl[countCreat] = gBlockCreat_count * GUIBUSSINESS_CTRLOBJ_MAX_NUM + countCreat;
					lv_obj_set_free_num(pTemp->next->nodeData.imgCtrlObj[countCreat], pTemp->next->nodeData.lv_fNum_objImgCtrl[countCreat]);
					lv_imgbtn_set_action(pTemp->next->nodeData.imgCtrlObj[countCreat], LV_BTN_ACTION_CLICK, funCb_imgbtnActionClick_CtrlObjBaseOpreat);
					
					pTemp->next->nodeData.ctrlObjIcon[countCreat] = lv_img_create(pTemp->next->nodeData.imgCtrlObj[countCreat], NULL);
					lv_obj_set_protect(pTemp->next->nodeData.ctrlObjIcon[countCreat], LV_PROTECT_POS);
					lv_img_set_src(pTemp->next->nodeData.ctrlObjIcon[countCreat], usrAppHomepageBtnIconDisp_dataGet(pTemp->next->nodeData.objIcon_ist[countCreat]));
					(pTemp->next->nodeData.devStatusVal & (1 << countCreat))?
						lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[countCreat], styleStdGblockCtrlObj_icon_pr):
						lv_img_set_style(pTemp->next->nodeData.ctrlObjIcon[countCreat], styleStdGblockCtrlObj_icon_rel);
					
					pTemp->next->nodeData.nameTextDispObj[countCreat] = lv_label_create(pTemp->next->nodeData.graphBaseBlock, NULL);
					lv_label_set_align(pTemp->next->nodeData.nameTextDispObj[countCreat], LV_LABEL_ALIGN_CENTER);
					lv_label_set_long_mode(pTemp->next->nodeData.nameTextDispObj[countCreat], LV_LABEL_LONG_DOT);
					lv_obj_set_size(pTemp->next->nodeData.nameTextDispObj[countCreat], 60, 20);
					lv_label_set_text(pTemp->next->nodeData.nameTextDispObj[countCreat], (const char*)pTemp->next->nodeData.objCtrl_name[countCreat]);
					lv_obj_set_protect(pTemp->next->nodeData.nameTextDispObj[countCreat], LV_PROTECT_POS);
					lv_label_set_style(pTemp->next->nodeData.nameTextDispObj[countCreat], styleStdGblockCtrlObjLabel_ptr);
				}

				switch(pTemp->next->nodeData.ctrlObj_devType){

					case devTypeDef_thermostatExtension:{

						(devStateTemp.devType_thermostat.devThermostat_running_en)?
							lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_TGL_REL):
							lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[0], LV_BTN_STATE_REL);

					}break;
					
					case devTypeDef_scenario:{

						lv_btn_state_t objState_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM];

						switch(devStateTemp.devType_scenario.devScenario_opNum){

							case 1:{

								objState_temp[0] = LV_BTN_STATE_TGL_REL;
								objState_temp[1] = LV_BTN_STATE_REL;
								objState_temp[2] = LV_BTN_STATE_REL;
							}break;

							case 2:{

								objState_temp[0] = LV_BTN_STATE_REL;
								objState_temp[1] = LV_BTN_STATE_TGL_REL;
								objState_temp[2] = LV_BTN_STATE_REL;
							}break;
	
							case 4:{

								objState_temp[0] = LV_BTN_STATE_REL;
								objState_temp[1] = LV_BTN_STATE_REL;
								objState_temp[2] = LV_BTN_STATE_TGL_REL;
							}break;

							default:{

								objState_temp[0] = LV_BTN_STATE_REL;
								objState_temp[1] = LV_BTN_STATE_REL;
								objState_temp[2] = LV_BTN_STATE_REL;
							}break;
						}
						
						lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[0], objState_temp[0]);
						lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[1], objState_temp[1]);
						lv_imgbtn_set_state(pTemp->next->nodeData.imgCtrlObj[2], objState_temp[2]);

					}break;
				}

				lv_obj_align(pTemp->next->nodeData.imgCtrlObj[1], pTemp->next->nodeData.graphBaseBlock, LV_ALIGN_CENTER, 0, coordErrObj_y);
				lv_obj_align(pTemp->next->nodeData.imgCtrlObj[0], pTemp->next->nodeData.imgCtrlObj[1], LV_ALIGN_OUT_LEFT_BOTTOM, -20, 0);
				lv_obj_align(pTemp->next->nodeData.imgCtrlObj[2], pTemp->next->nodeData.imgCtrlObj[1], LV_ALIGN_OUT_RIGHT_BOTTOM, 20, 0);

				for(countCreat = 0; countCreat < 3; countCreat ++){

					lv_obj_align(pTemp->next->nodeData.ctrlObjIcon[countCreat], pTemp->next->nodeData.imgCtrlObj[countCreat], LV_ALIGN_CENTER, coordErrObj_icon_x, coordErrObj_icon_y);
					lv_obj_align(pTemp->next->nodeData.nameTextDispObj[countCreat], pTemp->next->nodeData.imgCtrlObj[countCreat], LV_ALIGN_OUT_TOP_MID, coordErrObj_name_x, coordErrObj_name_y);
				}

			}break;

			default:break;
		}

		lv_obj_align(pTemp->next->nodeData.graphBaseBlock, obj_Parent, LV_ALIGN_IN_TOP_MID, 0, (devOpt_gBlockBase_sizeY + 10) * gBlockCreat_count - 100);

		switch(pTemp->next->nodeData.ctrlObj_devType){

			case devTypeDef_mulitSwOneBit:
			case devTypeDef_moudleSwOneBit:
				devStatistical.sum_devSwit += 1;
				break;
			
			case devTypeDef_mulitSwTwoBit:
			case devTypeDef_moudleSwTwoBit:
				devStatistical.sum_devSwit += 2;
				break;

			case devTypeDef_mulitSwThreeBit:
			case devTypeDef_moudleSwThreeBit:
				devStatistical.sum_devSwit += 3;
				break;

			case devTypeDef_curtain:
			case devTypeDef_moudleSwCurtain:
				devStatistical.sum_devCur += 1;
				break;

			case devTypeDef_thermostat:
				devStatistical.sum_devTher += 1;
				break;
			
			case devTypeDef_thermostatExtension:
				devStatistical.sum_devTher += 1;
				devStatistical.sum_devSwit += 2;
				break;

			case devTypeDef_dimmer:
				devStatistical.sum_devDim += 1;
				break;
				
			case devTypeDef_heater:
				devStatistical.sum_devHeat += 1;
				break;

			case devTypeDef_fans:
				devStatistical.sum_devFan += 1;
				break;

			case devTypeDef_scenario:
				devStatistical.sum_devScen += 1;
				break;

			case devTypeDef_infrared:
				devStatistical.sum_devInfr += 1;
				break;

			case devTypeDef_socket:
				devStatistical.sum_devSoc += 1;
				break;

			default:break;
		}

		gBlockCreat_count ++;

		pTemp = pTemp->next;
	}
//	styleStdGblockCtrlObj_baseBlock_A->body.grad_color = LV_COLOR_MAKE(0xE0, 0xE0, 0xE0);
//	lv_style_copy(styleStdGblockCtrlObj_baseBlock_B, styleStdGblockCtrlObj_baseBlock_A);
//	styleStdGblockCtrlObj_baseBlock_B->body.grad_color = LV_COLOR_MAKE(0x99, 0xED, 0xEA);
//	lv_style_copy(styleStdGblockCtrlObj_baseBlock_C, styleStdGblockCtrlObj_baseBlock_A);
//	styleStdGblockCtrlObj_baseBlock_C->body.grad_color = LV_COLOR_MAKE(0xE0, 0xFF, 0xFF);
//	lv_style_copy(styleStdGblockCtrlObj_baseBlock_D, styleStdGblockCtrlObj_baseBlock_A);
//	styleStdGblockCtrlObj_baseBlock_D->body.grad_color = LV_COLOR_MAKE(0xE0, 0xFF, 0xE0);

	if(devStatistical.sum_devSwit){sprintf(strStatisInfoHandle, "#FFC080 Swit:%d,#", devStatistical.sum_devSwit);strcat(strDevListStatisInfo, strStatisInfoHandle);memset(strStatisInfoHandle, 0, sizeof(char) * 16);devInfoObjAdd_ist ++;if((devInfoObjAdd_ist % 3) == 0)strcat(strDevListStatisInfo, "\n");}
	if(devStatistical.sum_devCur){sprintf(strStatisInfoHandle, 	"#99EDEA Cur:%d,#",  devStatistical.sum_devCur); strcat(strDevListStatisInfo, strStatisInfoHandle);memset(strStatisInfoHandle, 0, sizeof(char) * 16);devInfoObjAdd_ist ++;if((devInfoObjAdd_ist % 3) == 0)strcat(strDevListStatisInfo, "\n");}
	if(devStatistical.sum_devDim){sprintf(strStatisInfoHandle, 	"#99EDEA Dim:%d,#",  devStatistical.sum_devDim); strcat(strDevListStatisInfo, strStatisInfoHandle);memset(strStatisInfoHandle, 0, sizeof(char) * 16);devInfoObjAdd_ist ++;if((devInfoObjAdd_ist % 3) == 0)strcat(strDevListStatisInfo, "\n");}
	if(devStatistical.sum_devScen){sprintf(strStatisInfoHandle, "#E0FFE0 Scen:%d,#", devStatistical.sum_devScen);strcat(strDevListStatisInfo, strStatisInfoHandle);memset(strStatisInfoHandle, 0, sizeof(char) * 16);devInfoObjAdd_ist ++;if((devInfoObjAdd_ist % 3) == 0)strcat(strDevListStatisInfo, "\n");}
	if(devStatistical.sum_devFan){sprintf(strStatisInfoHandle, 	"#E0FFE0 Fan:%d,#",  devStatistical.sum_devFan); strcat(strDevListStatisInfo, strStatisInfoHandle);memset(strStatisInfoHandle, 0, sizeof(char) * 16);devInfoObjAdd_ist ++;if((devInfoObjAdd_ist % 3) == 0)strcat(strDevListStatisInfo, "\n");}
	if(devStatistical.sum_devHeat){sprintf(strStatisInfoHandle, "#E0FFFF Heat:%d,#", devStatistical.sum_devHeat);strcat(strDevListStatisInfo, strStatisInfoHandle);memset(strStatisInfoHandle, 0, sizeof(char) * 16);devInfoObjAdd_ist ++;if((devInfoObjAdd_ist % 3) == 0)strcat(strDevListStatisInfo, "\n");}
	if(devStatistical.sum_devTher){sprintf(strStatisInfoHandle, "#E0FFE0 Ther:%d,#", devStatistical.sum_devTher);strcat(strDevListStatisInfo, strStatisInfoHandle);memset(strStatisInfoHandle, 0, sizeof(char) * 16);devInfoObjAdd_ist ++;if((devInfoObjAdd_ist % 3) == 0)strcat(strDevListStatisInfo, "\n");}
	if(devStatistical.sum_devSoc){sprintf(strStatisInfoHandle, 	"#E0FFFF Soc:%d,#",  devStatistical.sum_devSoc); strcat(strDevListStatisInfo, strStatisInfoHandle);memset(strStatisInfoHandle, 0, sizeof(char) * 16);devInfoObjAdd_ist ++;if((devInfoObjAdd_ist % 3) == 0)strcat(strDevListStatisInfo, "\n");}
	if(devStatistical.sum_devInfr){sprintf(strStatisInfoHandle, "#E0FFFF Infr:%d,#", devStatistical.sum_devInfr);strcat(strDevListStatisInfo, strStatisInfoHandle);memset(strStatisInfoHandle, 0, sizeof(char) * 16);devInfoObjAdd_ist ++;if((devInfoObjAdd_ist % 3) == 0)strcat(strDevListStatisInfo, "\n");}
	if(strlen(strDevListStatisInfo) > 1)strDevListStatisInfo[strlen(strDevListStatisInfo) - 2] = '.';
	lv_label_set_text(label_devListStatisInfo, strDevListStatisInfo);
	if(devInfoObjAdd_ist)devInfoObjAdd_ist --;
	lv_obj_align(label_devListStatisInfo, obj_Parent, LV_ALIGN_IN_TOP_MID, 0, 10 * (3 - (devInfoObjAdd_ist / 3)));
	printf("gBlock creat cout:%d.\n", gBlockCreat_count);

	pageCreated_flg = true;
}


void lvGuiLinkageConfig_devGraphCtrlBlock_listLoadPrePage(lv_obj_t * obj_Parent){

	uint8_t deviceNum = lvglUsrApp_devCtrlBlockBaseManageList_nodeNumDetect(listHead_nodeCtrlObjBlockBaseManage);

	lvDevGblockListGenerate_prePage = lv_page_create(obj_Parent, NULL);
	lv_page_set_scrl_fit(lvDevGblockListGenerate_prePage, false, false);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(lvDevGblockListGenerate_prePage, 330, 240)):
		(lv_obj_set_size(lvDevGblockListGenerate_prePage, 251, 280));
	(devStatusDispMethod_landscapeIf_get())?
		lv_obj_set_pos(lvDevGblockListGenerate_prePage, 9, 30):
		lv_obj_set_pos(lvDevGblockListGenerate_prePage, 9, 41);
	lv_page_set_style(lvDevGblockListGenerate_prePage, LV_PAGE_STYLE_SB, &lv_style_transp);
	lv_page_set_style(lvDevGblockListGenerate_prePage, LV_PAGE_STYLE_BG, &lv_style_transp);
	lv_page_set_style(lvDevGblockListGenerate_prePage, LV_PAGE_STYLE_SCRL, &lv_style_transp);
	lv_page_set_sb_mode(lvDevGblockListGenerate_prePage, LV_SB_MODE_HIDE);
	lv_page_set_scrl_fit(lvDevGblockListGenerate_prePage, false, false); //key opration
	lv_page_set_scrl_layout(lvDevGblockListGenerate_prePage, LV_LAYOUT_CENTER);

	if(devStatusDispMethod_landscapeIf_get()){

		lv_page_set_scrl_width(lvDevGblockListGenerate_prePage, 310);
		lv_page_set_scrl_height(lvDevGblockListGenerate_prePage, deviceNum * (devOpt_gBlockBase_sizeY + 10) + 150);
	}
	else
	{
		lv_page_set_scrl_width(lvDevGblockListGenerate_prePage, 230);
		lv_page_set_scrl_height(lvDevGblockListGenerate_prePage, deviceNum * (devOpt_gBlockBase_sizeY + 10) + 150);
	}

	lvGuiLinkageConfig_devGraphCtrlBlock_listLoadInitialization(listHead_nodeCtrlObjBlockBaseManage, lvDevGblockListGenerate_prePage);
}

static void lvGuiMenuLinkage_styleMemoryInitialization(void){

	static bool memAlloced_flg = false;

	if(true == memAlloced_flg)return;
	else memAlloced_flg = true;

	stylePage_funSetOption = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	stylePage_linkageDetailSetting = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleText_menuLevel_A = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleText_menuLevel_B = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleText_menuLevel_C = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleCb_proximityDetection_screenLight = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleCb_proximityDetection_switchTrigEn = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleBtn_specialTransparent = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleTextBtn_linkageDetailSetting = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleTextTitle_linkageDetailSetting = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleRoller_bg_tempThresholdAdj = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleRoller_sel_tempThresholdAdj = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleImg_menuFun_btnFun = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
}

static void lvGuiLinkageConfig_objStyle_Init(void){

	lvGuiMenuLinkage_styleMemoryInitialization();

	lv_style_copy(styleText_menuLevel_A, &lv_style_plain);
	styleText_menuLevel_A->text.font = &lv_font_dejavu_20;
	styleText_menuLevel_A->text.color = LV_COLOR_WHITE;

	lv_style_copy(stylePage_funSetOption, &lv_style_plain);
	stylePage_funSetOption->body.main_color = LV_COLOR_GRAY;
	stylePage_funSetOption->body.grad_color = LV_COLOR_GRAY;

	lv_style_copy(styleTextBtn_linkageDetailSetting, &lv_style_plain);
	styleTextBtn_linkageDetailSetting->text.font = &lv_font_ariblk_18;
	styleTextBtn_linkageDetailSetting->text.color = LV_COLOR_MAKE(0, 128, 255);

	lv_style_copy(styleTextTitle_linkageDetailSetting, &lv_style_plain);
	styleTextTitle_linkageDetailSetting->text.font = &lv_font_consola_19;
	styleTextTitle_linkageDetailSetting->text.color = LV_COLOR_BLACK;

	lv_style_copy(stylePage_linkageDetailSetting, &lv_style_plain_color);
	stylePage_linkageDetailSetting->body.main_color = LV_COLOR_SILVER;
	stylePage_linkageDetailSetting->body.grad_color = LV_COLOR_SILVER;
	stylePage_linkageDetailSetting->body.border.part = LV_BORDER_NONE;
	stylePage_linkageDetailSetting->body.radius = 6;
	stylePage_linkageDetailSetting->body.opa = LV_OPA_90;
	stylePage_linkageDetailSetting->body.padding.hor = 0;
	stylePage_linkageDetailSetting->body.padding.inner = 0;	

	lv_style_copy(styleText_menuLevel_B, &lv_style_plain);
	styleText_menuLevel_B->text.font = &lv_font_dejavu_15;
	styleText_menuLevel_B->text.color = LV_COLOR_WHITE;

	lv_style_copy(styleText_menuLevel_C, &lv_style_plain);
	styleText_menuLevel_C->text.font = &lv_font_consola_16;
	styleText_menuLevel_C->text.color = LV_COLOR_WHITE;

	lv_style_copy(styleCb_proximityDetection_screenLight, &lv_style_plain);
	styleCb_proximityDetection_screenLight->body.main_color = LV_COLOR_GRAY;
	styleCb_proximityDetection_screenLight->body.grad_color = LV_COLOR_GRAY;
	styleCb_proximityDetection_screenLight->body.border.part = LV_BORDER_NONE;
	styleCb_proximityDetection_screenLight->body.radius = 6;
	styleCb_proximityDetection_screenLight->body.opa = LV_OPA_100;
	styleCb_proximityDetection_screenLight->body.padding.hor = 0;
	styleCb_proximityDetection_screenLight->body.padding.inner = 0;	
	styleCb_proximityDetection_screenLight->text.color = LV_COLOR_MAKE(0xB5, 0xE6, 0x1D);
	styleCb_proximityDetection_screenLight->text.font = &lv_font_consola_16;
	
	lv_style_copy(styleCb_proximityDetection_switchTrigEn, styleCb_proximityDetection_screenLight);
	styleCb_proximityDetection_switchTrigEn->body.empty = 1;
	styleCb_proximityDetection_switchTrigEn->text.color = LV_COLOR_BLACK;

    lv_style_copy(styleBtn_specialTransparent, &lv_style_btn_rel);
	styleBtn_specialTransparent->body.main_color = LV_COLOR_TRANSP;
	styleBtn_specialTransparent->body.grad_color = LV_COLOR_TRANSP;
	styleBtn_specialTransparent->body.border.part = LV_BORDER_NONE;
    styleBtn_specialTransparent->body.opa = LV_OPA_TRANSP;
	styleBtn_specialTransparent->body.radius = 0;
    styleBtn_specialTransparent->body.shadow.width = 0;
		
	lv_style_copy(styleRoller_bg_tempThresholdAdj, &lv_style_plain);
	styleRoller_bg_tempThresholdAdj->body.main_color = LV_COLOR_SILVER;
	styleRoller_bg_tempThresholdAdj->body.grad_color = LV_COLOR_SILVER;
	styleRoller_bg_tempThresholdAdj->body.opa = LV_OPA_100;
	styleRoller_bg_tempThresholdAdj->text.font = &lv_font_consola_19;
	styleRoller_bg_tempThresholdAdj->text.line_space = 5;
	styleRoller_bg_tempThresholdAdj->text.opa = LV_OPA_40;
	lv_style_copy(styleRoller_sel_tempThresholdAdj, &lv_style_plain);
	styleRoller_sel_tempThresholdAdj->body.main_color = LV_COLOR_SILVER;
	styleRoller_sel_tempThresholdAdj->body.grad_color = LV_COLOR_SILVER;
	styleRoller_sel_tempThresholdAdj->body.opa = LV_OPA_100;
	styleRoller_sel_tempThresholdAdj->body.radius = 30;
	styleRoller_sel_tempThresholdAdj->text.color = LV_COLOR_MAKE(128, 0, 255);
	styleRoller_sel_tempThresholdAdj->text.font = &lv_font_consola_19;

	lv_style_copy(styleImg_menuFun_btnFun, &lv_style_plain);
	styleImg_menuFun_btnFun->image.intense = LV_OPA_COVER;
	styleImg_menuFun_btnFun->image.color = LV_COLOR_MAKE(200, 191, 231);
}

static void lvGuiLinkageConfig_devGraphCtrlBlock_listGenerate(stt_nodeObj_listManageDevCtrlBase *devNodeListHead, lv_obj_t *obj_Parent){

	
}

void lvGuiLinkageConfig_devGraphCtrlBlock_listNodeUnitRefresh(uint8_t devMac[MWIFI_ADDR_LEN], uint8_t devState){

	stt_nodeObj_listManageDevCtrlBase *objNode_data = lvglUsrApp_devCtrlBlockBaseManageList_nodeGet(listHead_nodeCtrlObjBlockBaseManage, devMac, true);
	stt_devDataPonitTypedef devStateTemp = {0};
	
	if(NULL != objNode_data){

		objNode_data->nodeData.devStatusVal = devState; //状态数据更新
		
		if(false == pageCreated_flg)return; //若UI可用，才进行UI更新

		lv_page_focus(lvDevGblockListGenerate_prePage, objNode_data->nodeData.graphBaseBlock, 500);

		switch(objNode_data->nodeData.ctrlObj_devType){

			case devTypeDef_mulitSwOneBit:
			case devTypeDef_mulitSwTwoBit:
			case devTypeDef_mulitSwThreeBit:
			case devTypeDef_moudleSwOneBit:
			case devTypeDef_moudleSwTwoBit:
			case devTypeDef_moudleSwThreeBit:{

				uint8_t loopCreat = 0,
						countCreat = 0;
				
				switch(objNode_data->nodeData.ctrlObj_devType){

					case devTypeDef_mulitSwTwoBit:
					case devTypeDef_moudleSwTwoBit:loopCreat = 2;break;
					case devTypeDef_mulitSwThreeBit:
					case devTypeDef_moudleSwThreeBit:loopCreat = 3;break;

					default:loopCreat = 1;break;
				}

				for(countCreat = 0; countCreat < loopCreat; countCreat ++){

					if(devState & (1 << countCreat)){

						lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_TGL_REL);
						lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[countCreat], styleStdGblockCtrlObj_icon_pr);
					}
					else{
						
						lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[countCreat], LV_BTN_STATE_REL);
						lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[countCreat], styleStdGblockCtrlObj_icon_rel);
					}

					lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[countCreat]);
					lv_obj_refresh_style(objNode_data->nodeData.ctrlObjIcon[countCreat]);
				}

			}break;

			case devTypeDef_curtain:
			case devTypeDef_moudleSwCurtain:{

				memcpy(&devStateTemp, &devState, sizeof(stt_devDataPonitTypedef));
				lv_slider_set_value(objNode_data->nodeData.imgCtrlObj[0], devStateTemp.devType_curtain.devCurtain_actEnumVal);

				lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[0]);

			}break;
			
			case devTypeDef_dimmer:{
				
				memcpy(&devStateTemp, &devState, sizeof(stt_devDataPonitTypedef));
				lv_slider_set_value(objNode_data->nodeData.imgCtrlObj[0], devStateTemp.devType_dimmer.devDimmer_brightnessVal);

				lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[0]);

			}break;

			case devTypeDef_infrared:{}break;
			
			case devTypeDef_socket:{

				memcpy(&devStateTemp, &devState, sizeof(stt_devDataPonitTypedef));
				(devStateTemp.devType_socket.devSocket_opSw)?
					lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[0], LV_BTN_STATE_TGL_REL):
					lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[0], LV_BTN_STATE_REL);
				(devStateTemp.devType_socket.devSocket_opSw)?
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_pr):
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_rel);

				lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[0]);

			}break;

			case devTypeDef_heater:{

				memcpy(&devStateTemp, &devState, sizeof(stt_devDataPonitTypedef));
				(heaterOpreatAct_close != devStateTemp.devType_heater.devHeater_swEnumVal)?
					lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[0], LV_BTN_STATE_TGL_REL):
					lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[0], LV_BTN_STATE_REL);
				(heaterOpreatAct_close != devStateTemp.devType_heater.devHeater_swEnumVal)?
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_pr):
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_rel);

				lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[0]);

			}break;

			case devTypeDef_thermostat:{

				memcpy(&devStateTemp, &devState, sizeof(stt_devDataPonitTypedef));
				(devStateTemp.devType_thermostat.devThermostat_running_en)?
					lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[0], LV_BTN_STATE_TGL_REL):
					lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[0], LV_BTN_STATE_REL);
				(devStateTemp.devType_thermostat.devThermostat_running_en)?
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_pr):
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_rel);

				lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[0]);

			}break;

			case devTypeDef_fans:{}break;

			case devTypeDef_scenario:{

				lv_style_t *objStyle_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {NULL};
				lv_btn_state_t objState_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM];

				memcpy(&devStateTemp, &devState, sizeof(stt_devDataPonitTypedef));
				switch(devStateTemp.devType_scenario.devScenario_opNum){

					case 1:{

						objStyle_temp[0] = styleStdGblockCtrlObj_icon_pr;
						objStyle_temp[1] = styleStdGblockCtrlObj_icon_rel;
						objStyle_temp[2] = styleStdGblockCtrlObj_icon_rel;
						objState_temp[0] = LV_BTN_STATE_TGL_REL;
						objState_temp[1] = LV_BTN_STATE_REL;
						objState_temp[2] = LV_BTN_STATE_REL;
					}break;
					
					case 2:{
					
						objStyle_temp[0] = styleStdGblockCtrlObj_icon_rel;
						objStyle_temp[1] = styleStdGblockCtrlObj_icon_pr;
						objStyle_temp[2] = styleStdGblockCtrlObj_icon_rel;
						objState_temp[0] = LV_BTN_STATE_REL;
						objState_temp[1] = LV_BTN_STATE_TGL_REL;
						objState_temp[2] = LV_BTN_STATE_REL;
					}break;
					
					case 4:{
					
						objStyle_temp[0] = styleStdGblockCtrlObj_icon_rel;
						objStyle_temp[1] = styleStdGblockCtrlObj_icon_rel;
						objStyle_temp[2] = styleStdGblockCtrlObj_icon_pr;
						objState_temp[0] = LV_BTN_STATE_REL;
						objState_temp[1] = LV_BTN_STATE_REL;
						objState_temp[2] = LV_BTN_STATE_TGL_REL;
					}break;
					
					default:{
					
						objStyle_temp[0] = styleStdGblockCtrlObj_icon_rel;
						objStyle_temp[1] = styleStdGblockCtrlObj_icon_rel;
						objStyle_temp[2] = styleStdGblockCtrlObj_icon_rel;
						objState_temp[0] = LV_BTN_STATE_REL;
						objState_temp[1] = LV_BTN_STATE_REL;
						objState_temp[2] = LV_BTN_STATE_REL;
					}break;
				}
				
				lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[0], objStyle_temp[0]);
				lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[1], objStyle_temp[1]);
				lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[2], objStyle_temp[2]);
				lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[0], objState_temp[0]);
				lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[1], objState_temp[1]);
				lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[2], objState_temp[2]);
				lv_obj_refresh_style(objNode_data->nodeData.ctrlObjIcon[0]); //控件刷新
				lv_obj_refresh_style(objNode_data->nodeData.ctrlObjIcon[1]); //控件刷新
				lv_obj_refresh_style(objNode_data->nodeData.ctrlObjIcon[2]); //控件刷新
				lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[0]); //控件刷新
				lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[1]); //控件刷新
				lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[2]); //控件刷新

			}break;
			
			case devTypeDef_thermostatExtension:{

				(devStateTemp.devType_thermostat.devThermostat_running_en)?
					lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[0], LV_BTN_STATE_TGL_REL):
					lv_imgbtn_set_state(objNode_data->nodeData.imgCtrlObj[0], LV_BTN_STATE_REL);
				(devStateTemp.devType_thermostat.devThermostat_running_en)?
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_pr):
					lv_img_set_style(objNode_data->nodeData.ctrlObjIcon[0], styleStdGblockCtrlObj_icon_rel);

				lv_obj_refresh_style(objNode_data->nodeData.imgCtrlObj[0]);

			}break;

			default:{}break;
		}
	}
}

void lvGuiLinkageConfig_devGraphCtrlBlock_listLoadFlg_set(bool flg){

	pageCreated_flg = flg;
}

void dataTransBussiness_pageLinkageCfg_superCtrlActivityFunction(void){

	const uint8_t devMacDest[MWIFI_ADDR_LEN] = MWIFI_ADDR_ANY;
	mwifi_data_type_t data_type = {
		
		.compression = true,
		.communicate = MWIFI_COMMUNICATE_UNICAST,
	};
	const mlink_httpd_type_t type_L8mesh_cst = {

		.format = MLINK_HTTPD_FORMAT_HEX,
	};

	mdf_err_t ret = MDF_OK;

	memcpy(&data_type.custom, &type_L8mesh_cst, sizeof(uint32_t));
	
	if(NULL != dataTemp_swDev_superCtrl.dats){

		if(esp_mesh_get_layer() == MESH_ROOT){
		
			ret = mwifi_root_write(devMacDest, 
								   1, 
								   &data_type, 
								   dataTemp_swDev_superCtrl.dats, 
								   dataTemp_swDev_superCtrl.dataLen, 
								   true);
			MDF_ERROR_CHECK(ret != MDF_OK, ret, "<%s> superCtrl mwifi_translate", mdf_err_to_name(ret));
		}
		else
		{
			ret = mwifi_write(devMacDest, 
							  &data_type, 
							  dataTemp_swDev_superCtrl.dats, 
							  dataTemp_swDev_superCtrl.dataLen,  
							  true);
			MDF_ERROR_CHECK(ret != MDF_OK, ret, "<%s> superCtrl mwifi_translate", mdf_err_to_name(ret)); 
		}

		free(dataTemp_swDev_superCtrl.dats);
		dataTemp_swDev_superCtrl.dats = NULL;
	}
	else{

		ESP_LOGW(TAG, "super ctrl data is null");
	}
}

void dataTransBussiness_pageLinkageCfg_superSycnActivityFunction(void){

	const uint8_t devMacDest[MWIFI_ADDR_LEN] = MWIFI_ADDR_ANY;
	mwifi_data_type_t data_type = {
		
		.compression = true,
		.communicate = MWIFI_COMMUNICATE_UNICAST,
	};
	const mlink_httpd_type_t type_L8mesh_cst = {

		.format = MLINK_HTTPD_FORMAT_HEX,
	};

	mdf_err_t ret = MDF_OK;

	uint16_t dataTx_len = 0;
	uint8_t *dataTx_ptr = NULL;
	stt_superCtrl_dtTransParam dtSuperCtrlTemp = {0};

	memcpy(&data_type.custom, &type_L8mesh_cst, sizeof(uint32_t));

	esp_wifi_get_mac(ESP_IF_WIFI_STA, dtSuperCtrlTemp.targetDevMac);
	currentDev_dataPointGet((stt_devDataPonitTypedef *)&dtSuperCtrlTemp.devStateSet);
	
	dataTx_len = L8_meshDataCmdLen + sizeof(stt_superCtrl_dtTransParam);
	dataTx_ptr = (uint8_t *)os_zalloc(dataTx_len);
	if(NULL != dataTx_ptr){

		dataTx_ptr[0] = L8DEV_MESH_CMD_SUPER_SYNC;
		memcpy(&dataTx_ptr[1], &dtSuperCtrlTemp, sizeof(stt_superCtrl_dtTransParam));

		if(esp_mesh_get_layer() == MESH_ROOT){
		
			ret = mwifi_root_write(devMacDest, 
								   1, 
								   &data_type, 
								   dataTx_ptr, 
								   dataTx_len, 
								   true);
			MDF_ERROR_CHECK(ret != MDF_OK, ret, "<%s> superSycn mwifi_translate", mdf_err_to_name(ret));
		}
		else
		{
			ret = mwifi_write(devMacDest, 
							  &data_type, 
							  dataTx_ptr, 
							  dataTx_len, 
							  true);
			MDF_ERROR_CHECK(ret != MDF_OK, ret, "<%s> superSycn mwifi_translate", mdf_err_to_name(ret)); 
		}

		free(dataTx_ptr);
	}
	else{

		ESP_LOGW(TAG, "super sycn heap alloc null");
	}
}

void lvGui_businessMenu_linkageConfig(lv_obj_t * obj_Parent){

	stt_paramLinkageConfig linkageConfigParamSet_temp = {0};
	uint8_t deviceNum = lvglUsrApp_devCtrlBlockBaseManageList_nodeNumDetect(listHead_nodeCtrlObjBlockBaseManage);

	devSystemOpration_linkageConfig_paramGet(&linkageConfigParamSet_temp);

	lvGuiLinkageConfig_objStyle_Init();

	(devStatusDispMethod_landscapeIf_get())?
		(screenLandscapeCoordinate_objOffset = 35):
		(screenLandscapeCoordinate_objOffset = 0);

	text_Title = lv_label_create(obj_Parent, NULL);
	lv_label_set_text(text_Title, "linkage config");
	lv_obj_align(text_Title, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -7);
	lv_obj_set_style(text_Title, styleText_menuLevel_A);

	menuBtnChoIcon_fun_home = lv_imgbtn_create(obj_Parent, NULL);
	lv_obj_set_size(menuBtnChoIcon_fun_home, 100, 50);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(menuBtnChoIcon_fun_home, 220, 23)):
		(lv_obj_set_pos(menuBtnChoIcon_fun_home, 140, 23));
	lv_imgbtn_set_src(menuBtnChoIcon_fun_home, LV_BTN_STATE_REL, &iconMenu_funBack_homePage);
	lv_imgbtn_set_src(menuBtnChoIcon_fun_home, LV_BTN_STATE_PR, &iconMenu_funBack_homePage);
	lv_imgbtn_set_style(menuBtnChoIcon_fun_home, LV_BTN_STATE_PR, styleImg_menuFun_btnFun);
	lv_btn_set_action(menuBtnChoIcon_fun_home, LV_BTN_ACTION_CLICK, funCb_btnActionClick_menuBtn_funBack);
	lv_obj_set_free_num(menuBtnChoIcon_fun_home, LV_OBJ_FREENUM_BTNNUM_DEF_MENUHOME);

	menuBtnChoIcon_fun_back = lv_imgbtn_create(obj_Parent, menuBtnChoIcon_fun_home);
	lv_obj_set_pos(menuBtnChoIcon_fun_back, 0, 20);
	lv_imgbtn_set_src(menuBtnChoIcon_fun_back, LV_BTN_STATE_REL, &iconMenu_funBack_arrowLeft);
	lv_imgbtn_set_src(menuBtnChoIcon_fun_back, LV_BTN_STATE_PR, &iconMenu_funBack_arrowLeft);
	lv_btn_set_action(menuBtnChoIcon_fun_back, LV_BTN_ACTION_CLICK, funCb_btnActionClick_menuBtn_funBack);
	lv_obj_set_free_num(menuBtnChoIcon_fun_back, LV_OBJ_FREENUM_BTNNUM_DEF_MENUBACK);

//	menuBtnChoIcon_fun_home = lv_btn_create(obj_Parent, NULL);
//	lv_obj_set_size(menuBtnChoIcon_fun_home, 100, 50);
//	(devStatusDispMethod_landscapeIf_get())?
//		(lv_obj_set_pos(menuBtnChoIcon_fun_home, 240, 25)):
//		(lv_obj_set_pos(menuBtnChoIcon_fun_home, 160, 25));
//	lv_obj_set_top(menuBtnChoIcon_fun_home, true);
//	lv_obj_set_free_num(menuBtnChoIcon_fun_home, LV_OBJ_FREENUM_BTNNUM_DEF_MENUHOME);
//	lv_btn_set_style(menuBtnChoIcon_fun_home, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
//	lv_btn_set_style(menuBtnChoIcon_fun_home, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
//	lv_btn_set_style(menuBtnChoIcon_fun_home, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
//	lv_btn_set_style(menuBtnChoIcon_fun_home, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
//	lv_btn_set_action(menuBtnChoIcon_fun_home, LV_BTN_ACTION_CLICK, funCb_btnActionClick_menuBtn_funBack);
//	lv_btn_set_action(menuBtnChoIcon_fun_home, LV_BTN_ACTION_PR, funCb_btnActionPress_menuBtn_funBack);
//	imgMenuBtnChoIcon_fun_home = lv_img_create(obj_Parent, NULL);
//	lv_img_set_src(imgMenuBtnChoIcon_fun_home, &iconMenu_funBack_homePage);
//	lv_obj_set_protect(imgMenuBtnChoIcon_fun_home, LV_PROTECT_POS);
//	lv_obj_align(imgMenuBtnChoIcon_fun_home, menuBtnChoIcon_fun_home, LV_ALIGN_IN_RIGHT_MID, -25, 4);
//	lv_obj_set_top(menuBtnChoIcon_fun_home, true);

//	menuBtnChoIcon_fun_back = lv_btn_create(obj_Parent, menuBtnChoIcon_fun_home);
//	lv_obj_set_pos(menuBtnChoIcon_fun_back, 0, 25);
//	lv_obj_set_free_num(menuBtnChoIcon_fun_back, LV_OBJ_FREENUM_BTNNUM_DEF_MENUBACK);
//	lv_btn_set_action(menuBtnChoIcon_fun_back, LV_BTN_ACTION_CLICK, funCb_btnActionClick_menuBtn_funBack);
//	lv_btn_set_action(menuBtnChoIcon_fun_back, LV_BTN_ACTION_PR, funCb_btnActionPress_menuBtn_funBack);
//	imgMenuBtnChoIcon_fun_back = lv_img_create(obj_Parent, NULL);
//	lv_img_set_src(imgMenuBtnChoIcon_fun_back, &iconMenu_funBack_arrowLeft);
//	lv_obj_set_protect(imgMenuBtnChoIcon_fun_back, LV_PROTECT_POS);
//	lv_obj_align(imgMenuBtnChoIcon_fun_back, menuBtnChoIcon_fun_back, LV_ALIGN_IN_LEFT_MID, 5, 4);
//	lv_obj_set_top(menuBtnChoIcon_fun_back, true);

	page_funSetOption = lv_page_create(lv_scr_act(), NULL);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(page_funSetOption, 320, 165)):
		(lv_obj_set_size(page_funSetOption, 240, 245));
	lv_obj_set_pos(page_funSetOption, 0, 75);
	lv_page_set_style(page_funSetOption, LV_PAGE_STYLE_SB, stylePage_funSetOption);
	lv_page_set_style(page_funSetOption, LV_PAGE_STYLE_BG, stylePage_funSetOption);
	lv_page_set_style(page_funSetOption, LV_PAGE_STYLE_SCRL, stylePage_funSetOption);
	lv_page_set_sb_mode(page_funSetOption, LV_SB_MODE_HIDE);
	lv_page_set_scrl_fit(page_funSetOption, false, false); //key opration

	if(devStatusDispMethod_landscapeIf_get()){

		lv_page_set_scrl_width(page_funSetOption, 310);
		lv_page_set_scrl_height(page_funSetOption, deviceNum * (devOpt_gBlockBase_sizeY + 10) + 150);
	}
	else
	{
		lv_page_set_scrl_width(page_funSetOption, 230);
		lv_page_set_scrl_height(page_funSetOption, deviceNum * (devOpt_gBlockBase_sizeY + 10) + 150);
	}
	lv_page_set_scrl_layout(page_funSetOption, LV_LAYOUT_CENTER);

	lv_obj_t *label_funcNoSuppert = lv_label_create(page_funSetOption, NULL);
	lv_label_set_recolor(label_funcNoSuppert, true);
	lv_label_set_text(label_funcNoSuppert, "#FFFFFF Function unsupported#");
	lv_obj_set_style(label_funcNoSuppert, styleText_menuLevel_B);
	lv_obj_set_protect(label_funcNoSuppert, LV_PROTECT_POS);
	lv_obj_align(label_funcNoSuppert, NULL, LV_ALIGN_IN_TOP_MID, 0, 40);

//	lvGuiLinkageConfig_devGraphCtrlBlock_listLoadInitialization(listHead_nodeCtrlObjBlockBaseManage, page_funSetOption);

//	textSettingA_proximityDetection = lv_label_create(page_funSetOption, NULL);
//	lv_label_set_recolor(textSettingA_proximityDetection, true);
//	lv_label_set_text(textSettingA_proximityDetection, "#FFFFFF proximity detection#:");
//	lv_obj_set_style(textSettingA_proximityDetection, styleText_menuLevel_B);
//	lv_obj_set_protect(textSettingA_proximityDetection, LV_PROTECT_POS);
//	lv_obj_align(textSettingA_proximityDetection, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 20);

//	swReserveSet_proximityDetection = lv_sw_create(page_funSetOption, NULL);
//	lv_sw_set_action(swReserveSet_proximityDetection, funCb_swOpreat_proximityDetection_reserveEn);
//	lv_obj_set_size(swReserveSet_proximityDetection, 45, 15);
//	lv_obj_set_protect(swReserveSet_proximityDetection, LV_PROTECT_POS);
//	lv_obj_align(swReserveSet_proximityDetection, textSettingA_proximityDetection, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

//	textSettingA_tempratureDetection = lv_label_create(page_funSetOption, NULL);
//	lv_label_set_recolor(textSettingA_tempratureDetection, true);
//	lv_label_set_text(textSettingA_tempratureDetection, "#FFFFFF temprature detection#:");
//	lv_obj_set_style(textSettingA_tempratureDetection, styleText_menuLevel_B);
//	lv_obj_set_protect(textSettingA_tempratureDetection, LV_PROTECT_POS);
//	lv_obj_align(textSettingA_tempratureDetection, textSettingA_proximityDetection, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 30);

//	swReserveSet_tempratureDetection = lv_sw_create(page_funSetOption, NULL);
//	lv_sw_set_action(swReserveSet_tempratureDetection, funCb_swOpreat_tempratureDetection_reserveEn);
//	lv_obj_set_size(swReserveSet_tempratureDetection, 45, 15);
//	lv_obj_set_protect(swReserveSet_tempratureDetection, LV_PROTECT_POS);
//	lv_obj_align(swReserveSet_tempratureDetection, swReserveSet_proximityDetection, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);

//	if(linkageConfigParamSet_temp.linkageRunning_temprature_en){

//		lv_sw_on(swReserveSet_tempratureDetection);
//		lvGuiLinkageConfig_childOptionObjCreat_tempratureDetection();
//	}

//	if(linkageConfigParamSet_temp.linkageRunning_proxmity_en){

//		lv_sw_on(swReserveSet_proximityDetection);
//		lvGuiLinkageConfig_childOptionObjCreat_proximityDetection();
//	}

//	lv_obj_animate(textSettingA_proximityDetection,  LV_ANIM_FLOAT_RIGHT, 200,	  0,	NULL);
//	lv_obj_animate(swReserveSet_proximityDetection,  LV_ANIM_FLOAT_RIGHT, 200,	100,	NULL);
//	lv_obj_animate(textSettingA_tempratureDetection, LV_ANIM_FLOAT_RIGHT, 200,	200,	NULL);
//	lv_obj_animate(swReserveSet_tempratureDetection, LV_ANIM_FLOAT_RIGHT, 200,	300,	NULL);

	vTaskDelay(20 / portTICK_PERIOD_MS);
	lv_obj_refresh_style(page_funSetOption);
	lv_obj_refresh_style(obj_Parent);
}


