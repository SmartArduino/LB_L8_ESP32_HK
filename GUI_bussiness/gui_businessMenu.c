/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>

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

#include "gui_businessMenu.h"
#include "gui_businessHome.h"

#include "dataTrans_remoteServer.h"
#include "dataTrans_localHandler.h"

#include "devDriver_manage.h"

LV_FONT_DECLARE(lv_font_dejavu_15)

LV_IMG_DECLARE(iconMenu_linkageConfig);
LV_IMG_DECLARE(iconMenu_timer);
LV_IMG_DECLARE(iconMenu_delayer);
LV_IMG_DECLARE(iconMenu_wifiConfig);
LV_IMG_DECLARE(iconMenu_other);
LV_IMG_DECLARE(iconMenu_setting);
LV_IMG_DECLARE(iconMenu_bkPR);
LV_IMG_DECLARE(iconMenu_bkREL);
LV_IMG_DECLARE(iconMenu_bkRELdk);
LV_IMG_DECLARE(iconMenu_bkPRSl);
LV_IMG_DECLARE(iconMenu_bkRELSl);
LV_IMG_DECLARE(iconMenu_bkRELSldk);
LV_IMG_DECLARE(iconMenu_funBack);
LV_IMG_DECLARE(iconMenu_funBackdk);
LV_IMG_DECLARE(imageBtn_feedBackNormal);

static lv_style_t *styleText_menuLevel_A = NULL;
static lv_style_t *styleText_menuLevel_B = NULL;
static lv_style_t *styleBtn_menuBack_press = NULL;

static lv_res_t funCb_btnActionClick_menuBtn_funBack(lv_obj_t *btn){

//	lvGui_usrSwitch(bussinessType_Home);

	lvGui_usrSwitch_withPrefunc(bussinessType_Home, NULL);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionPress_menuBtn_funBack(lv_obj_t *btn){

//	lv_obj_t *btnFeedBk = lv_img_create(btn, NULL);
//	lv_img_set_src(btnFeedBk, &imageBtn_feedBackNormal);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_menuBtn_other(lv_obj_t *btn){

//	lvGui_usrSwitch(bussinessType_menuPageOther);
	lvGui_usrSwitch_withPrefunc(bussinessType_menuPageOther, NULL);
	
	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_menuBtn_delayer(lv_obj_t *btn){

//	lvGui_usrSwitch(bussinessType_menuPageDelayer);
	lvGui_usrSwitch_withPrefunc(bussinessType_menuPageDelayer, NULL);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_menuBtn_timer(lv_obj_t *btn){

//	lvGui_usrSwitch(bussinessType_menuPageTimer);
	lvGui_usrSwitch_withPrefunc(bussinessType_menuPageTimer, NULL);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_menuBtn_linkageConfig(lv_obj_t *btn){

//	lvGui_usrSwitch(bussinessType_menuPageLinkageConfig);
	lvGui_usrSwitch_withPrefunc(bussinessType_menuPageLinkageConfig, NULL);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_menuBtn_setting(lv_obj_t *btn){

//	lvGui_usrSwitch(bussinessType_menuPageSetting);
	lvGui_usrSwitch_withPrefunc(bussinessType_menuPageSetting, NULL);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_menuBtn_wifiConfig(lv_obj_t *btn){

//	lvGui_usrSwitch(bussinessType_menuPageWifiConfig);
	lvGui_usrSwitch_withPrefunc(bussinessType_menuPageWifiConfig, NULL);
	
	return LV_RES_OK;
}

static void lvGuiMenu_styleMemoryInitialization(void){

	static bool memAlloced_flg = false;

	if(true == memAlloced_flg)return;
	else memAlloced_flg = true;

	styleText_menuLevel_A = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleText_menuLevel_B = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
	styleBtn_menuBack_press = (lv_style_t *)os_zalloc(sizeof(lv_style_t));
}

void lvGui_businessMenu(lv_obj_t * obj_Parent){

	lv_obj_t * menuBtnChoIcon_other;
	lv_obj_t * menuBtnChoIcon_delayer;
	lv_obj_t * menuBtnChoIcon_timer;
	lv_obj_t * menuBtnChoIcon_linkageConfig;
	lv_obj_t * menuBtnChoIcon_setting;
	lv_obj_t * menuBtnChoIcon_wifiConfig;
	
	lv_obj_t * menuBtnChoIcon_fun_back;

	lv_obj_t * menuIconCho_other;
	lv_obj_t * menuIconCho_delayer;
	lv_obj_t * menuIconCho_timer;
	lv_obj_t * menuIconCho_linkageConfig;
	lv_obj_t * menuIconCho_setting;
	lv_obj_t * menuIconCho_wifiConfig;

	lv_obj_t * text_Title;

	lv_obj_t * menuTextCho_other;
	lv_obj_t * menuTextCho_delayer;
	lv_obj_t * menuTextCho_timer;
	lv_obj_t * menuTextCho_linkageConfig;
	lv_obj_t * menuTextCho_setting;
	lv_obj_t * menuTextCho_wifiConfig;

	lv_img_dsc_t *iconMenuFuncBack = &iconMenu_funBack;
	lv_img_dsc_t *iconMenuBkRel = &iconMenu_bkREL;
	lv_img_dsc_t *iconMenuBkPre = &iconMenu_bkPR;

	devFireware_upgradeReserveCheck_trigByEvent(); //每次进入菜单check一次upgrade
	devDetailInfoList_request_trigByEvent();  //每次进入菜单请求一次设备列表

	lvGuiMenu_styleMemoryInitialization();

	lv_style_copy(styleText_menuLevel_A, &lv_style_plain);
	styleText_menuLevel_A->text.font = &lv_font_dejavu_30;
	styleText_menuLevel_A->text.color = LV_COLOR_WHITE;

	lv_style_copy(styleBtn_menuBack_press, &lv_style_plain);
	styleBtn_menuBack_press->image.color = LV_COLOR_SILVER;
	styleBtn_menuBack_press->image.intense = LV_OPA_50;

	switch(usrAppHomepageBgroundPicOrg_Get()){
	
		case bGroudImg_objInsert_usrPic:{

			styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x5d462b);
			
		}break;

		case bGroudImg_objInsert_figureA:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x5d462b);}break;
		case bGroudImg_objInsert_figureB:{styleText_menuLevel_A->text.color = LV_COLOR_MAKE(164, 128, 128);}break;
		case bGroudImg_objInsert_figureC:{styleText_menuLevel_A->text.color = LV_COLOR_MAKE(192, 168, 64);}break;
		case bGroudImg_objInsert_figureD:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x5d462b);}break;
		case bGroudImg_objInsert_figureE:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x5d462b);}break;
		case bGroudImg_objInsert_figureF:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x606060);}break;
		case bGroudImg_objInsert_figureG:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x606060);}break;
		case bGroudImg_objInsert_figureH:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x606060);}break;
		case bGroudImg_objInsert_figureI:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x606060);}break;
		case bGroudImg_objInsert_figureJ:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x606060);}break;
		case bGroudImg_objInsert_figureK:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x606060);}break;
		case bGroudImg_objInsert_figureL:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x606060);}break;
		case bGroudImg_objInsert_figureM:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x606060);}break;
		case bGroudImg_objInsert_figureN:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0xffffff);}break;
		case bGroudImg_objInsert_figureO:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x606060);}break;
		case bGroudImg_objInsert_figureP:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x606060);}break;
		case bGroudImg_objInsert_figureQ:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0xffffff);}break;
		case bGroudImg_objInsert_figureR:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x606060);}break;
		case bGroudImg_objInsert_figureS:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x606060);}break;
		case bGroudImg_objInsert_figureT:{styleText_menuLevel_A->text.color = LV_COLOR_MAKE(192, 192, 192);}break;

		case bGroudImg_objInsert_pureColor1:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0xffffff);}break;
		case bGroudImg_objInsert_pureColor2:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0xffffff);}break;
		case bGroudImg_objInsert_pureColor3:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0xffffff);}break;
		case bGroudImg_objInsert_pureColor4:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0xffffff);}break;
		case bGroudImg_objInsert_pureColor5:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0x2c2c2c);}break;
		case bGroudImg_objInsert_pureColor6:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0xffffff);}break;
		case bGroudImg_objInsert_pureColor7:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0xffffff);}break;
	
		default:{styleText_menuLevel_A->text.color = LV_COLOR_HEX(0xffffff);}break;
	}

	switch(usrAppHomepageBgroundPicOrg_Get()){

		case bGroudImg_objInsert_figureN:
		case bGroudImg_objInsert_figureQ:{

			iconMenuFuncBack = &iconMenu_funBack;
			if(devStatusDispMethod_landscapeIf_get()){

				iconMenuBkRel = &iconMenu_bkRELSl;
				iconMenuBkPre = &iconMenu_bkPRSl;
			}
			else
			{
				iconMenuBkRel = &iconMenu_bkREL;
				iconMenuBkPre = &iconMenu_bkPR;
			}
		
		}break;

		default:{

			iconMenuFuncBack = &iconMenu_funBackdk;
			iconMenuBkPre = &iconMenu_bkPR;
			if(devStatusDispMethod_landscapeIf_get()){

				iconMenuBkRel = &iconMenu_bkRELSldk;
				iconMenuBkPre = &iconMenu_bkPRSl;
			}
			else
			{
				iconMenuBkRel = &iconMenu_bkRELdk;
				iconMenuBkPre = &iconMenu_bkPR;
			}

		}break;
	}

	text_Title = lv_label_create(obj_Parent, NULL);
	lv_label_set_text(text_Title, "Menu");
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(text_Title, 135, 25)):
		(lv_obj_set_pos(text_Title, 90, 40));
	lv_obj_set_style(text_Title, styleText_menuLevel_A);

	menuBtnChoIcon_fun_back = lv_imgbtn_create(obj_Parent, NULL);
	lv_imgbtn_set_src(menuBtnChoIcon_fun_back, LV_BTN_STATE_REL, iconMenuFuncBack);
	lv_imgbtn_set_src(menuBtnChoIcon_fun_back, LV_BTN_STATE_PR, iconMenuFuncBack);

	lv_imgbtn_set_style(menuBtnChoIcon_fun_back, LV_BTN_STATE_PR, styleBtn_menuBack_press);
//	lv_imgbtn_set_style(menuBtnChoIcon_fun_back, LV_BTN_STATE_PR, &styleBtn_devMulitSw_statusOn);
	
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(menuBtnChoIcon_fun_back, 8, 20)):
		(lv_obj_set_pos(menuBtnChoIcon_fun_back, 8, 35));
	lv_btn_set_action(menuBtnChoIcon_fun_back, LV_BTN_ACTION_CLICK, funCb_btnActionClick_menuBtn_funBack);
	lv_btn_set_action(menuBtnChoIcon_fun_back, LV_BTN_ACTION_PR, funCb_btnActionPress_menuBtn_funBack);
//	lv_obj_set_click(menuBtnChoIcon_fun_back, false);

	lv_obj_animate(text_Title, 		 		LV_ANIM_FLOAT_LEFT, 100,   0, NULL);
	lv_obj_animate(menuBtnChoIcon_fun_back, LV_ANIM_FLOAT_LEFT, 100,  50, NULL);

	menuBtnChoIcon_other = lv_imgbtn_create(obj_Parent, NULL);
	lv_imgbtn_set_src(menuBtnChoIcon_other, LV_BTN_STATE_REL, iconMenuBkRel);
	lv_imgbtn_set_src(menuBtnChoIcon_other, LV_BTN_STATE_PR, iconMenuBkPre);
	if(devStatusDispMethod_landscapeIf_get()){

		lv_obj_set_pos(menuBtnChoIcon_other, 214, 150);
		menuBtnChoIcon_delayer = lv_imgbtn_create(obj_Parent, menuBtnChoIcon_other);
		lv_obj_set_pos(menuBtnChoIcon_delayer, 108, 60);
		menuBtnChoIcon_timer = lv_imgbtn_create(obj_Parent, menuBtnChoIcon_other);
		lv_obj_set_pos(menuBtnChoIcon_timer, 2, 60);
		menuBtnChoIcon_linkageConfig = lv_imgbtn_create(obj_Parent, menuBtnChoIcon_other);
		lv_obj_set_pos(menuBtnChoIcon_linkageConfig, 214, 60);
		menuBtnChoIcon_setting = lv_imgbtn_create(obj_Parent, menuBtnChoIcon_other);
		lv_obj_set_pos(menuBtnChoIcon_setting, 108, 150);
		menuBtnChoIcon_wifiConfig = lv_imgbtn_create(obj_Parent, menuBtnChoIcon_other);
		lv_obj_set_pos(menuBtnChoIcon_wifiConfig, 2, 150);
	}
	else
	{
		lv_obj_set_pos(menuBtnChoIcon_other, 121, 238);
		menuBtnChoIcon_delayer = lv_imgbtn_create(obj_Parent, menuBtnChoIcon_other);
		lv_obj_set_pos(menuBtnChoIcon_delayer, 121, 74);
		menuBtnChoIcon_timer = lv_imgbtn_create(obj_Parent, menuBtnChoIcon_other);
		lv_obj_set_pos(menuBtnChoIcon_timer, 5, 74);
		menuBtnChoIcon_linkageConfig = lv_imgbtn_create(obj_Parent, menuBtnChoIcon_other);
		lv_obj_set_pos(menuBtnChoIcon_linkageConfig, 5, 156);
		menuBtnChoIcon_setting = lv_imgbtn_create(obj_Parent, menuBtnChoIcon_other);
		lv_obj_set_pos(menuBtnChoIcon_setting, 121, 156);
		menuBtnChoIcon_wifiConfig = lv_imgbtn_create(obj_Parent, menuBtnChoIcon_other);
		lv_obj_set_pos(menuBtnChoIcon_wifiConfig, 5, 238);
	}
	
	lv_btn_set_action(menuBtnChoIcon_other, LV_BTN_ACTION_CLICK, funCb_btnActionClick_menuBtn_other);
	lv_btn_set_action(menuBtnChoIcon_delayer, LV_BTN_ACTION_CLICK, funCb_btnActionClick_menuBtn_delayer);
	lv_btn_set_action(menuBtnChoIcon_timer, LV_BTN_ACTION_CLICK, funCb_btnActionClick_menuBtn_timer);
	lv_btn_set_action(menuBtnChoIcon_linkageConfig, LV_BTN_ACTION_CLICK, funCb_btnActionClick_menuBtn_linkageConfig);
	lv_btn_set_action(menuBtnChoIcon_setting, LV_BTN_ACTION_CLICK, funCb_btnActionClick_menuBtn_setting);
	lv_btn_set_action(menuBtnChoIcon_wifiConfig, LV_BTN_ACTION_CLICK, funCb_btnActionClick_menuBtn_wifiConfig);
	
//	lv_obj_set_click(menuBtnChoIcon_other, 			false);
//	lv_obj_set_click(menuBtnChoIcon_delayer, 		false);
//	lv_obj_set_click(menuBtnChoIcon_timer, 			false);
//	lv_obj_set_click(menuBtnChoIcon_linkageConfig,  false);
//	lv_obj_set_click(menuBtnChoIcon_setting, 		false);
//	lv_obj_set_click(menuBtnChoIcon_wifiConfig, 	false);
	
	menuIconCho_other = lv_img_create(menuBtnChoIcon_other, NULL);
	lv_img_set_src(menuIconCho_other, &iconMenu_other);
	menuIconCho_delayer = lv_img_create(menuBtnChoIcon_delayer, NULL);
	lv_img_set_src(menuIconCho_delayer, &iconMenu_delayer);
	menuIconCho_timer = lv_img_create(menuBtnChoIcon_timer, NULL);
	lv_img_set_src(menuIconCho_timer, &iconMenu_timer);
	menuIconCho_linkageConfig = lv_img_create(menuBtnChoIcon_linkageConfig, NULL);
	lv_img_set_src(menuIconCho_linkageConfig, &iconMenu_linkageConfig);
	menuIconCho_setting = lv_img_create(menuBtnChoIcon_setting, NULL);
	lv_img_set_src(menuIconCho_setting, &iconMenu_setting);
	menuIconCho_wifiConfig = lv_img_create(menuBtnChoIcon_wifiConfig, NULL);
	lv_img_set_src(menuIconCho_wifiConfig, &iconMenu_wifiConfig);
	lv_obj_set_protect(menuIconCho_other, LV_PROTECT_POS);
	lv_obj_set_protect(menuIconCho_delayer, LV_PROTECT_POS);
	lv_obj_set_protect(menuIconCho_timer, LV_PROTECT_POS);
	lv_obj_set_protect(menuIconCho_linkageConfig, LV_PROTECT_POS);
	lv_obj_set_protect(menuIconCho_setting, LV_PROTECT_POS);
	lv_obj_set_protect(menuIconCho_wifiConfig, LV_PROTECT_POS);
	lv_obj_align(menuIconCho_other, NULL, LV_ALIGN_IN_LEFT_MID, 20, 0);
	lv_obj_align(menuIconCho_delayer, NULL, LV_ALIGN_IN_LEFT_MID, 5, 0);
	lv_obj_align(menuIconCho_timer, NULL, LV_ALIGN_IN_LEFT_MID, 5, 0);
	lv_obj_align(menuIconCho_linkageConfig, NULL, LV_ALIGN_IN_LEFT_MID, 5, 0);
	lv_obj_align(menuIconCho_setting, NULL, LV_ALIGN_IN_LEFT_MID, 5, 0);
	lv_obj_align(menuIconCho_wifiConfig, NULL, LV_ALIGN_IN_LEFT_MID, 5, 0);

	lv_style_copy(styleText_menuLevel_B, &lv_style_plain);
	styleText_menuLevel_B->text.font = &lv_font_dejavu_15;
	styleText_menuLevel_B->text.color = LV_COLOR_WHITE;
	
	menuTextCho_other = lv_label_create(menuBtnChoIcon_other, NULL);
	lv_label_set_text(menuTextCho_other, "System\ninfo");
	lv_obj_set_style(menuTextCho_other, styleText_menuLevel_B);
	lv_obj_set_protect(menuTextCho_other, LV_PROTECT_POS);
	lv_obj_align(menuTextCho_other, NULL, LV_ALIGN_IN_LEFT_MID, 55, 5);

	menuTextCho_setting = lv_label_create(menuBtnChoIcon_setting, NULL);
	lv_label_set_text(menuTextCho_setting, "Setting");
	lv_obj_set_style(menuTextCho_setting, styleText_menuLevel_B);
	lv_obj_set_protect(menuTextCho_setting, LV_PROTECT_POS);
	lv_obj_align(menuTextCho_setting, NULL, LV_ALIGN_IN_LEFT_MID, 55, 5);

	menuTextCho_linkageConfig = lv_label_create(menuBtnChoIcon_linkageConfig, NULL);
	lv_label_set_text(menuTextCho_linkageConfig, "Linkage\nconfig");
	lv_obj_set_style(menuTextCho_linkageConfig, styleText_menuLevel_B);
	lv_obj_set_protect(menuTextCho_linkageConfig, LV_PROTECT_POS);
	lv_obj_align(menuTextCho_linkageConfig, NULL, LV_ALIGN_IN_LEFT_MID, 55, 5);

	menuTextCho_timer = lv_label_create(menuBtnChoIcon_timer, NULL);
	lv_label_set_text(menuTextCho_timer, "Timer");
	lv_obj_set_style(menuTextCho_timer, styleText_menuLevel_B);
	lv_obj_set_protect(menuTextCho_timer, LV_PROTECT_POS);
	lv_obj_align(menuTextCho_timer, NULL, LV_ALIGN_IN_LEFT_MID, 55, 5);

	menuTextCho_delayer = lv_label_create(menuBtnChoIcon_delayer, NULL);
	lv_label_set_text(menuTextCho_delayer, "Delayer");
	lv_obj_set_style(menuTextCho_delayer, styleText_menuLevel_B);
	lv_obj_set_protect(menuTextCho_delayer, LV_PROTECT_POS);
	lv_obj_align(menuTextCho_delayer, NULL, LV_ALIGN_IN_LEFT_MID, 55, 5);

	menuTextCho_wifiConfig = lv_label_create(menuBtnChoIcon_wifiConfig, NULL);
	lv_label_set_text(menuTextCho_wifiConfig, "Wifi\nconfig");
	lv_obj_set_style(menuTextCho_wifiConfig, styleText_menuLevel_B);
	lv_obj_set_protect(menuTextCho_wifiConfig, LV_PROTECT_POS);
	lv_obj_align(menuTextCho_wifiConfig, NULL, LV_ALIGN_IN_LEFT_MID, 55, 5);

	if(devStatusDispMethod_landscapeIf_get()){

		lv_obj_align(menuIconCho_other, 		NULL, 	LV_ALIGN_CENTER, 0, -15);
		lv_obj_align(menuIconCho_delayer, 		NULL, 	LV_ALIGN_CENTER, 0, -15);
		lv_obj_align(menuIconCho_timer, 		NULL, 	LV_ALIGN_CENTER, 0, -15);
		lv_obj_align(menuIconCho_linkageConfig, NULL, 	LV_ALIGN_CENTER, 0, -15);
		lv_obj_align(menuIconCho_setting, 		NULL, 	LV_ALIGN_CENTER, 0, -15);
		lv_obj_align(menuIconCho_wifiConfig, 	NULL, 	LV_ALIGN_CENTER, 0, -15);

		lv_obj_align(menuTextCho_other, 		NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
		lv_obj_align(menuTextCho_setting, 		NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
		lv_obj_align(menuTextCho_linkageConfig, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
		lv_obj_align(menuTextCho_timer, 		NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
		lv_obj_align(menuTextCho_delayer, 		NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
		lv_obj_align(menuTextCho_wifiConfig, 	NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
	}

	if(devStatusDispMethod_landscapeIf_get()){

		lv_obj_animate(menuBtnChoIcon_timer, 		 LV_ANIM_FLOAT_LEFT, 80,   0, NULL);
		lv_obj_animate(menuBtnChoIcon_delayer, 		 LV_ANIM_FLOAT_LEFT, 80,  50, NULL);
		lv_obj_animate(menuBtnChoIcon_linkageConfig, LV_ANIM_FLOAT_LEFT, 80, 100, NULL);
		lv_obj_animate(menuBtnChoIcon_wifiConfig, 	 LV_ANIM_FLOAT_LEFT, 80,  50, NULL);
		lv_obj_animate(menuBtnChoIcon_setting, 		 LV_ANIM_FLOAT_LEFT, 80, 100, NULL);
		lv_obj_animate(menuBtnChoIcon_other, 		 LV_ANIM_FLOAT_LEFT, 80, 200, NULL);
	}
	else
	{
		lv_obj_animate(menuBtnChoIcon_timer,		 LV_ANIM_FLOAT_LEFT, 80,   0, NULL);
		lv_obj_animate(menuBtnChoIcon_linkageConfig, LV_ANIM_FLOAT_LEFT, 80,  50, NULL);
		lv_obj_animate(menuBtnChoIcon_wifiConfig,	 LV_ANIM_FLOAT_LEFT, 80, 100, NULL);
		lv_obj_animate(menuBtnChoIcon_delayer,		 LV_ANIM_FLOAT_LEFT, 80,  50, NULL);
		lv_obj_animate(menuBtnChoIcon_setting,		 LV_ANIM_FLOAT_LEFT, 80, 100, NULL);
		lv_obj_animate(menuBtnChoIcon_other,		 LV_ANIM_FLOAT_LEFT, 80, 200, NULL);
	}

	vTaskDelay(20 / portTICK_PERIOD_MS);
	lv_obj_refresh_style(obj_Parent);

//	lvglUsrApp_touchDisTrig(0, 1000);

//	lv_obj_set_click(menuBtnChoIcon_fun_back, true);

//	lv_obj_set_click(menuBtnChoIcon_other, 			true);
//	lv_obj_set_click(menuBtnChoIcon_delayer, 		true);
//	lv_obj_set_click(menuBtnChoIcon_timer, 			true);
//	lv_obj_set_click(menuBtnChoIcon_linkageConfig,  true);
//	lv_obj_set_click(menuBtnChoIcon_setting, 		true);
//	lv_obj_set_click(menuBtnChoIcon_wifiConfig, 	true);
}


