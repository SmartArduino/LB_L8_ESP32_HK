/*********************
 *      INCLUDES	 *
 *********************/
#include <stdio.h>

#include "sdkconfig.h"

#include "os.h"

/* lvgl includes */
#include "iot_lvgl.h"

#include "gui_businessHome.h"

#include "gui_businessMenu.h"
#include "gui_businessMenu_other.h"
#include "gui_businessMenu_wifiConfig.h"
#include "gui_businessMenu_delayer.h"
#include "gui_businessMenu_timer.h"
#include "gui_businessMenu_linkageConfig.h"
#include "gui_businessMenu_setting.h"
#include "gui_businessMenu_settingSet.h"
#include "gui_businessMenu_timerSet.h"
#include "gui_businessMenu_delayerSet.h"
#include "gui_businessMenu_timer.h"
#include "gui_businessMenu_delayer.h"
#include "tips_bussinessAcoustoOptic.h"

#include "devDriver_manage.h"

#include "bussiness_timerSoft.h"

#include "dataTrans_remoteServer.h"
#include "dataTrans_localHandler.h"

typedef void (*guiHomeFun_local_bussiness)(lv_obj_t * obj_Parent);

LV_FONT_DECLARE(lv_font_arialNum_100);
LV_FONT_DECLARE(lv_font_dejavu_40);
LV_FONT_DECLARE(lv_font_dejavu_30);
LV_FONT_DECLARE(lv_font_dejavu_20);
LV_FONT_DECLARE(lv_font_dejavu_15);
LV_FONT_DECLARE(lv_font_consola_13);
LV_FONT_DECLARE(lv_font_consola_16);
LV_FONT_DECLARE(lv_font_arial_12);
LV_FONT_DECLARE(lv_font_arial_15);
LV_FONT_DECLARE(lv_font_consola_17);
LV_FONT_DECLARE(lv_font_consola_19);
//LV_FONT_DECLARE(lv_font_arabic_20);
LV_FONT_DECLARE(lv_font_arabic_17);
//LV_FONT_DECLARE(lv_font_arabic_15);
//LV_FONT_DECLARE(lv_font_hebrew_20);
LV_FONT_DECLARE(lv_font_hebrew_17);
//LV_FONT_DECLARE(lv_font_hebrew_15);

LV_IMG_DECLARE(testPic);
LV_IMG_DECLARE(testPic_P);
LV_IMG_DECLARE(ttA);
LV_IMG_DECLARE(night_Sight);
LV_IMG_DECLARE(ll_longxia);
//LV_IMG_DECLARE(homepage_bkPic_danYa);
LV_IMG_DECLARE(homepage_bkPic_jiJian);
LV_IMG_DECLARE(homepage_bkPic_jianJie);
LV_IMG_DECLARE(homepage_bkPic_keAi);
LV_IMG_DECLARE(homepage_bkPic_palace);
LV_IMG_DECLARE(homepage_bkPic_jijianSl);
LV_IMG_DECLARE(homepage_bkPic_jianJieSl);
LV_IMG_DECLARE(homepage_bkPic_keAiSl);
LV_IMG_DECLARE(homepage_bkPic_palaceSl);

//LV_IMG_DECLARE(homepage_bkPic_ouZhou);

LV_IMG_DECLARE(iconHeader_wifi_A);
LV_IMG_DECLARE(iconHeader_wifi_B);
LV_IMG_DECLARE(iconHeader_wifi_C);
LV_IMG_DECLARE(iconHeader_wifi_D);
LV_IMG_DECLARE(iconHeader_wifi_offline);
LV_IMG_DECLARE(iconHeader_wifi_master);
LV_IMG_DECLARE(iconHeader_alarm);
LV_IMG_DECLARE(iconHeader_greenMode);
LV_IMG_DECLARE(iconHeader_node);
LV_IMG_DECLARE(iconHeader_apple);
LV_IMG_DECLARE(iconHeader_elec);
LV_IMG_DECLARE(iconAlarm_tips);
LV_IMG_DECLARE(iconHeader_nightMoon);
LV_IMG_DECLARE(iconHeader_lock);
LV_IMG_DECLARE(iconHeader_tempra_A);
LV_IMG_DECLARE(iconHeader_tempra_B);
LV_IMG_DECLARE(button_aera);
LV_IMG_DECLARE(button_area);
LV_IMG_DECLARE(homepage_buttonPicRel_fenSe);
LV_IMG_DECLARE(homepage_buttonPicPre_fenSe);
LV_IMG_DECLARE(homepage_buttonPicRel_jianJie);
LV_IMG_DECLARE(homepage_buttonPicPre_jianJie);
LV_IMG_DECLARE(homepage_buttonPicRel_ouZhou);
LV_IMG_DECLARE(homepage_buttonPicPre_ouZhou);
LV_IMG_DECLARE(homepage_buttonPicRel_jiJian);
LV_IMG_DECLARE(homepage_buttonPicPre_jiJian);

LV_IMG_DECLARE(homepage_buttonPicRel_keAiSl);
LV_IMG_DECLARE(homepage_buttonPicPre_keAiSl);
LV_IMG_DECLARE(homepage_buttonPicRel_jianJieSl);
LV_IMG_DECLARE(homepage_buttonPicPre_jianJieSl);
LV_IMG_DECLARE(homepage_buttonPicRel_jiJianSl);
LV_IMG_DECLARE(homepage_buttonPicPre_jiJianSl);

LV_IMG_DECLARE(bGroundPrev_picFigure_pic);
LV_IMG_DECLARE(bGroundPrev_picFigure_1);
LV_IMG_DECLARE(bGroundPrev_picFigure_2);
LV_IMG_DECLARE(bGroundPrev_picFigure_3);
LV_IMG_DECLARE(bGroundPrev_picFigure_4);
LV_IMG_DECLARE(bGroundPrev_picFigure_5);
LV_IMG_DECLARE(bGroundPrev_picFigure_6);
LV_IMG_DECLARE(bGroundPrev_picFigure_7);
LV_IMG_DECLARE(bGroundPrev_picFigure_8);
LV_IMG_DECLARE(bGroundPrev_picFigure_9);
LV_IMG_DECLARE(bGroundPrev_picFigure_10);
LV_IMG_DECLARE(bGroundPrev_picFigure_11);
LV_IMG_DECLARE(bGroundPrev_picFigure_12);
LV_IMG_DECLARE(bGroundPrev_picFigure_13);
LV_IMG_DECLARE(bGroundPrev_picFigure_14);
LV_IMG_DECLARE(bGroundPrev_picFigure_15);
LV_IMG_DECLARE(bGroundPrev_picFigure_16);
LV_IMG_DECLARE(bGroundPrev_picFigure_17);
LV_IMG_DECLARE(bGroundPrev_picFigure_18);
LV_IMG_DECLARE(bGroundPrev_picFigure_19);
LV_IMG_DECLARE(bGroundPrev_picFigure_20);
LV_IMG_DECLARE(bGroundPrev_picColor_1);
LV_IMG_DECLARE(bGroundPrev_picColor_2);
LV_IMG_DECLARE(bGroundPrev_picColor_3);
LV_IMG_DECLARE(bGroundPrev_picColor_4);
LV_IMG_DECLARE(bGroundPrev_picColor_5);
LV_IMG_DECLARE(bGroundPrev_picColor_6);
LV_IMG_DECLARE(bGroundPrev_picColor_7);
LV_IMG_DECLARE(bGroundPrev_picFigure_null);

LV_IMG_DECLARE(bGround_picFigure_0); //
LV_IMG_DECLARE(bGround_picFigure_1); //
LV_IMG_DECLARE(bGround_picFigure_2); //
LV_IMG_DECLARE(bGround_picFigure_3); //
LV_IMG_DECLARE(bGround_picFigure_4); //
LV_IMG_DECLARE(bGround_picFigure_5); //
LV_IMG_DECLARE(bGround_picFigure_6); //
LV_IMG_DECLARE(bGround_picFigure_7); //
LV_IMG_DECLARE(bGround_picFigure_8); //
LV_IMG_DECLARE(bGround_picFigure_9); //
LV_IMG_DECLARE(bGround_picFigure_10); //
LV_IMG_DECLARE(bGround_picFigure_11); //
LV_IMG_DECLARE(bGround_picFigure_12); //
LV_IMG_DECLARE(bGround_picFigure_13); //
LV_IMG_DECLARE(bGround_picFigure_14); //
LV_IMG_DECLARE(bGround_picFigure_15); //
LV_IMG_DECLARE(bGround_picFigure_16); //
LV_IMG_DECLARE(bGround_picFigure_17); //
LV_IMG_DECLARE(bGround_picFigure_18); //
LV_IMG_DECLARE(bGround_picFigure_19); //
LV_IMG_DECLARE(bGround_picFigure_20); //

LV_IMG_DECLARE(iconHome_menu);
LV_IMG_DECLARE(btnIconHome_meeting);
LV_IMG_DECLARE(btnIconHome_sleeping);
LV_IMG_DECLARE(btnIconHome_toilet);
LV_IMG_DECLARE(iconPage_curtainClose_rel);
LV_IMG_DECLARE(iconPage_curtainPluse_rel);
LV_IMG_DECLARE(iconPage_curtainOpen_rel);
LV_IMG_DECLARE(iconPage_binding);
LV_IMG_DECLARE(iconPage_binding_x);
LV_IMG_DECLARE(iconPage_unbinding);
LV_IMG_DECLARE(imageCurtain_gan);
LV_IMG_DECLARE(imageCurtain_body);
LV_IMG_DECLARE(specifyIcon_thermPageReturn);
LV_IMG_DECLARE(specifyIcon_thermostat);

LV_IMG_DECLARE(iconUiBlock_warning);
LV_IMG_DECLARE(iconUiBlock_success);
LV_IMG_DECLARE(iconUiBlock_preload);

LV_IMG_DECLARE(lanbonSealPic_A);
LV_IMG_DECLARE(lanbonSealPic_B);
LV_IMG_DECLARE(serPic_meridsmart);

LV_IMG_DECLARE(iconLightA_HomePageDeviceDimmer);
LV_IMG_DECLARE(iconLightB_HomePageDeviceDimmer);
LV_IMG_DECLARE(iconFans_HomePageDeviceFans);
LV_IMG_DECLARE(iconCurtain_HomePageDeviceCurtain);
LV_IMG_DECLARE(iconHourglass_HomePageDeviceHeater);
LV_IMG_DECLARE(iconHeater_HomePageDeviceHeater);
LV_IMG_DECLARE(iconSet_HomePageDeviceHeater);
LV_IMG_DECLARE(iconDevThermostat_ventFan);

LV_IMG_DECLARE(epidemicIcon_cure);
LV_IMG_DECLARE(epidemicIcon_confirmed);
LV_IMG_DECLARE(epidemicIcon_deaths);

LV_IMG_DECLARE(scrsaverStatusIconBk);

//业务变量
extern xQueueHandle msgQh_systemRestartDelayCounterTips;
extern EventGroupHandle_t xEventGp_tipsLoopTimer;
extern uint8_t devRunningTimeFromPowerUp_couter;

extern stt_nodeObj_listManageDevCtrlBase *listHead_nodeCtrlObjBlockBaseManage;

SemaphoreHandle_t xSph_lvglOpreat; //lvgl控件互斥信号量

stt_scrDisActAttr cntrDat_scrDis = {0};

xQueueHandle msgQh_wifiConfigCompleteTips = NULL;
EventGroupHandle_t xEventGp_screenTouch = NULL;
stt_touchEveInfo devTouchGetInfo = {0};

uint16_t ctrlObj_slidingCalmDownCounter = 0; //界面控件滑动冷却计时变量
uint8_t  homepageRecovery_timeoutCounter = 0; //非home界面显示超时跳转计时变量
uint16_t pageRefreshTrig_counter = COUNTER_DISENABLE_MASK_SPECIALVAL_U16; //界面刷新计时值（防马赛克）

stt_kLongPreKeepParam kPreReaptParam_unlock = {0};

static struct
{
    lv_img_header_t header;
    uint32_t data_size;
    uint8_t * data;
	
}testImg_data = {

	.header.always_zero = 0,
	.header.w = 120,
	.header.h = 20,
	.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
	.data_size = GUI_BUSSINESS_HOME_BTNTEXT_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE,
	.data = NULL,
	
},imgData_bGroudPic = {

	.header.always_zero = 0,
	.header.w = 240,
	.header.h = 320,
	.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
	.data_size = GUI_BUSSINESS_HOME_BGROUND_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE,
	.data = NULL,
};

static xQueueHandle msgQh_uiHp_devCtrlObjValChg = NULL;

static lv_obj_t *imageBK = NULL; 
static lv_obj_t *screenNull_refresher = NULL;

//static lv_obj_t *imageTips_timer = NULL;

static usrGuiBussiness_type guiPage_current = bussinessType_Home;
static usrGuiBussiness_type guiPage_record = bussinessType_null;

static bool uiBlockFlg_systemInit = false;
static bool uiBlockFlg_newVersionRunningConfirm = false;

static struct _stt_elecParam{

	float elecParam_power;
	float elecParam_elecsum;
}elecParamDisp_record = {99999.123F, 99999.123F};

static lv_obj_t *objFigure_ptr[64] = {NULL};
static lv_obj_t *objPicBg_ptr = NULL;

//home界面页眉显示信息对象
static lv_obj_t *iconHeaderObj_wifi = NULL;
static lv_obj_t *iconHeaderObj_node = NULL;
static lv_obj_t *iconHeaderObj_elec = NULL;
static lv_obj_t *iconHeaderObj_tempraA = NULL;
static lv_obj_t *iconHeaderObj_tempraB = NULL;

static lv_obj_t *iconHeaderObj_alarm = NULL;
static lv_obj_t *iconHeaderObj_greenMode = NULL;
static lv_obj_t *iconHeaderObj_nightMode = NULL;
static lv_obj_t *iconHeaderObj_devLock = NULL;

static lv_obj_t *textHeaderObj_time = NULL;
static lv_obj_t *textHeaderObj_elec = NULL;
static lv_obj_t *textHeaderObj_meshNodeNum = NULL;
static lv_obj_t *textHeaderObj_meshRole = NULL;
static lv_obj_t *textHeaderObj_temperature = NULL;

static lv_obj_t *text_loopTimerTips = NULL;
static lv_obj_t *icon_loopTimerTips = NULL;
static bool		trigFlg_loopTimerTips = false;

static lv_obj_t *label_sysRestartTips_ref = NULL;
static lv_obj_t *label_sysRestartTips_Counter = NULL;
static lv_obj_t *page_sysRestartTips = NULL;
static lv_obj_t *page_guiBlockTips = NULL;
static lv_obj_t *labelPage_guiBlockTips = NULL;
//static lv_obj_t *preloadPage_guiBlockTips = NULL;
static lv_obj_t *iconPage_guiBlockTips = NULL;
static lv_obj_t	*pageReminder_screenFull = NULL;
static lv_obj_t	*labelReminder_screenFull = NULL;

static lv_obj_t *label_bk_devDimmer = NULL;
static lv_obj_t *label_bk_devCurtain_positionCur = NULL;
static lv_obj_t *label_bk_devCurtain_positionAdj = NULL;

#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SLAVE_MCU)
 #if(DRVMETHOD_BY_SLAVE_MCU_RELAY_TEST == 1)

	static lv_obj_t *text_timeRecord = NULL;
	static lv_obj_t *text_loopCounter = NULL;

	extern volatile stt_relayMagTestParam paramMagRelayTest;
 #endif
#endif

//home界面开关按键对象
static lv_obj_t *btn_bk_devMulitSw_A = NULL;
static lv_obj_t *btn_bk_devMulitSw_B = NULL;
static lv_obj_t *btn_bk_devMulitSw_C = NULL;
static lv_obj_t *iconBtn_meeting = NULL;
static lv_obj_t *textBtn_meeting = NULL;
static lv_obj_t *iconBtn_sleeping = NULL;
static lv_obj_t *textBtn_sleeping = NULL;
static lv_obj_t *iconBtn_toilet = NULL;
static lv_obj_t *textBtn_toilet = NULL;
static lv_obj_t *iconBtn_binding_A = NULL;
static lv_obj_t *iconBtn_binding_B = NULL;
static lv_obj_t *iconBtn_binding_C = NULL;
static lv_obj_t *slider_bk_devDimmer = NULL;
static lv_obj_t *photoA_bk_devDimmer = NULL;
static lv_obj_t *photoB_bk_devDimmer = NULL;
static lv_obj_t *btn_bk_devCurtain_open = NULL;
static lv_obj_t *btn_bk_devCurtain_stop = NULL;
static lv_obj_t *btn_bk_devCurtain_close = NULL;
//static lv_obj_t *cont_bk_devCurtainImage = NULL;
//static lv_obj_t *image_bk_devCurtain_gan = NULL;
static lv_obj_t *image_bk_devCurtain_body = NULL;
static lv_obj_t *slider_bk_devCurtain = NULL;
static lv_obj_t *btnm_bk_devFans = NULL;
static lv_obj_t *textInstract_devFans = NULL;
static lv_obj_t *icomImage_devFans = NULL;
static lv_obj_t *btnm_bk_devHeater = NULL;
static lv_obj_t *imgBtn_devHeater_timeSet = NULL;
static lv_obj_t *page_timeSet_devHeater = NULL;
static lv_obj_t *rollerH_timeSetPage_devHeater = NULL;
static lv_obj_t *rollerM_timeSetPage_devHeater = NULL;
static lv_obj_t *rollerS_timeSetPage_devHeater = NULL;
static lv_obj_t *labelH_timeSetPage_devHeater = NULL;
static lv_obj_t *labelM_timeSetPage_devHeater = NULL;
static lv_obj_t *labelS_timeSetPage_devHeater = NULL;
static lv_obj_t *labelTitle_timeSetPage_devHeater = NULL;
static lv_obj_t *labelConfirm_timeSetPage_devHeater = NULL;
static lv_obj_t *labelCancel_timeSetPage_devHeater = NULL;
static lv_obj_t *btnConfirm_timeSetPage_devHeater = NULL;
static lv_obj_t *btnCancel_timeSetPage_devHeater = NULL;
static lv_obj_t *textTimeInstract_target_devHeater = NULL;
static lv_obj_t *textTimeInstract_current_devHeater = NULL;
static lv_obj_t *icomImageA_devHeater = NULL;
static lv_obj_t *lmeterTempInstTarget_devThermostat = NULL;
static lv_obj_t *lmeterTempInstCurrent_devThermostat = NULL;
static lv_obj_t *labelTempInstTarget_devThermostat = NULL;
static lv_obj_t *labelTempInstCurrent_devThermostat = NULL;
static lv_obj_t *slider_tempAdj_devThermostat = NULL;
static lv_obj_t *btn_tempAdjAdd_devThermostat = NULL;
static lv_obj_t *textBtn_tempAdjAdd_devThermostat = NULL;
static lv_obj_t *btn_tempAdjCut_devThermostat = NULL;
static lv_obj_t *textBtn_tempAdjCut_devThermostat = NULL;
static lv_obj_t *sw_devRunningEnable_devThermostat = NULL;
static lv_obj_t *roller_devRunningMode_devThermostat = NULL;
static lv_obj_t *btn_devRunningModeSw_devThermostat = NULL;
static lv_obj_t *btn_ventilationFan_devThermostat = NULL;
static lv_obj_t *cb_devEcoEnable_devThermostat = NULL;
static lv_obj_t *preload_driverCalmDown_devScenario = NULL;
static lv_obj_t *btn_bk_thermostatEx_fold = NULL;
static lv_obj_t *iconBtn_thermostatEx_fold = NULL;
static lv_obj_t *textBtn_thermostatExFold_temp = NULL;
static lv_obj_t *textBtn_thermostatExFold_eco = NULL;
static lv_obj_t *iconBtn_thermostatEx_pageReturn = NULL;

//home界面进入菜单按键对象
static lv_obj_t *btn_homeMenu = NULL;
static lv_obj_t *tv_homeLayout = NULL;
static lv_obj_t *tb_homePage_A = NULL;
static lv_obj_t *tb_homePage_B = NULL;
//home界面取消上锁按键对象
static lv_obj_t *btn_unlockMenu = NULL;

//疫情屏保显示界面相关控件
static lv_obj_t *objPage_epidemicDisp = NULL;
static lv_obj_t *objLabel_epidTitle = NULL;
static lv_obj_t *objLabel_epidCurDate = NULL;
static lv_obj_t *objLabel_epidLocalRef = NULL;
static lv_obj_t *objLabel_epidLocation = NULL;
static lv_obj_t *objIcon_epidCure = NULL;
static lv_obj_t *objIcon_epidConfirmed = NULL;
static lv_obj_t *objIcon_epidDeaths = NULL;
static lv_obj_t *objLabel_epidCure = NULL;
static lv_obj_t *objLabel_epidConfirmed = NULL;
static lv_obj_t *objLabel_epidDeaths = NULL;
static lv_obj_t *objLabelRef_epidCure = NULL;
static lv_obj_t *objLabelRef_epidConfirmed = NULL;
static lv_obj_t *objLabelRef_epidDeaths = NULL;

static lv_style_t *styleLabel_epidTitle = NULL;
static lv_style_t *styleLabel_epidCurDate = NULL;
static lv_style_t *styleLabel_epidNum = NULL;
static lv_style_t *styleLabel_epidRef = NULL;
static lv_style_t *styleLabel_epidLocation = NULL;

//常规屏保A显示界面相关控件
static lv_obj_t *objPage_screensaver_A = NULL;
static lv_obj_t *objLabel_scrsaverA_time_H = NULL;
static lv_obj_t *objLabel_scrsaverA_time_M = NULL;
static lv_obj_t *objLabel_scrsaverA_timeDot = NULL;
static lv_obj_t *objLabel_scrsaverA_date = NULL;

static lv_obj_t *objBase_ssr_swCtrlStatus[3] = {NULL};
static lv_obj_t *objIcon_ssr_swCtrlStatus[3] = {NULL};
static lv_obj_t *objLabel_ssr_swNanme[3] = {NULL};

static lv_style_t *styleLabel_scrsaverA_time = NULL;
static lv_style_t *styleLabel_scrsaverA_timeDot = NULL;
static lv_style_t *styleLabel_scrsaverA_date = NULL;
static lv_style_t *styleSsrBaseObj[3] = {NULL};
static lv_style_t *styleSsrBaseIcon[3] = {NULL};
static lv_style_t *styleSsrBaseLabel[3] = {NULL};

//home界面相关对象风格对象
static lv_style_t *styleImg_iconHeater = NULL;
static lv_style_t *styleImg_iconWifi = NULL;
static lv_style_t *styleText_versionDiscrib = NULL;
static lv_style_t *styleText_elecAnodenum = NULL;
static lv_style_t *styleText_temperature = NULL;
static lv_style_t *styleText_loopTimerTips = NULL;
static lv_style_t *styleText_time = NULL;
static lv_style_t *styleIcon_meshNode = NULL;
static lv_style_t *styleBtn_Text = NULL;
static lv_style_t *styleBgPic_colorStyle = NULL;
static lv_style_t *styleBk_secMenu = NULL;
static lv_style_t *styleBtn_devMulitSw_statusOn = NULL;
static lv_style_t *styleBtn_devMulitSw_statusOff = NULL;
static lv_style_t *styleIconvText_devMulitSw_statusOn = NULL;
static lv_style_t *styleIconvText_devMulitSw_statusOff = NULL;
static lv_style_t *styleBtnImg_icon = NULL;
static lv_style_t *styleSliderBk_devDimmer_bg = NULL;
static lv_style_t *styleSliderBk_devDimmer_indic = NULL;
static lv_style_t *styleSliderBk_devDimmer_knob = NULL;
static lv_style_t *stylePhotoAk_devDimmer = NULL;
static lv_style_t *stylePhotoBk_devDimmer = NULL;
static lv_style_t *styleText_devDimmer_SliderBk = NULL;
static lv_style_t *styleText_devHeater_Bk_timeRemind = NULL;
static lv_style_t *styleText_devCurtain_Bk_positionCurr = NULL;
static lv_style_t *styleText_devCurtain_Bk_positionTips = NULL;
static lv_style_t *styleBtn_devCurtain_statusPre = NULL;
static lv_style_t *styleBtn_devCurtain_statusRel = NULL;
static lv_style_t *styleImageBk_devCurtain_bkImgBody = NULL;
static lv_style_t *styleSliderBk_devCurtain_bg = NULL;
static lv_style_t *styleSliderBk_devCurtain_indic = NULL;
static lv_style_t *styleSliderBk_devCurtain_knob = NULL;
static lv_style_t *styleImage_devFans_icon = NULL;
static lv_style_t *styleText_devFans_instract = NULL;
static lv_style_t *styleBtnm_devFans_btnBg = NULL;
static lv_style_t *styleBtnm_devFans_btnRel = NULL;
static lv_style_t *styleBtnm_devFans_btnPre = NULL;
static lv_style_t *styleBtnm_devFans_btnTglRel = NULL;
static lv_style_t *styleBtnm_devFans_btnTglPre = NULL;
static lv_style_t *styleBtnm_devFans_btnIna = NULL;
static lv_style_t *styleImage_devHeater_icon = NULL;
static lv_style_t *styleText_devHeater_timeInstTar = NULL;
static lv_style_t *styleText_devHeater_timeInstCur = NULL;
static lv_style_t *styleBtnm_devHeater_btnBg = NULL;
static lv_style_t *styleBtnm_devHeater_btnRel = NULL;
static lv_style_t *styleBtnm_devHeater_btnPre = NULL;
static lv_style_t *styleBtnm_devHeater_btnTglRel = NULL;
static lv_style_t *styleBtnm_devHeater_btnTglPre = NULL;
static lv_style_t *styleBtnm_devHeater_btnIna = NULL;
static lv_style_t *stylePage_devHeater_timeSet = NULL;
static lv_style_t *styleTextBtn_devHeater_timeSetPage = NULL;
static lv_style_t *styleTextRoller_devHeater_timeSetPage_bg = NULL;
static lv_style_t *styleTextRoller_devHeater_timeSetPage_sel = NULL;
static lv_style_t *styleTextRollerTips_devHeater_timeSetPage = NULL;
static lv_style_t *styleLmeter_devThermostat_tempTarget = NULL;
static lv_style_t *styleLmeter_devThermostat_tempCurrent = NULL;
static lv_style_t *styleLabel_devThermostat_tempTarget = NULL;
static lv_style_t *styleLabel_devThermostat_tempCurrent = NULL;
static lv_style_t *styleSliderBk_devThermostat_bg = NULL;
static lv_style_t *styleSliderBk_devThermostat_indic = NULL;
static lv_style_t *styleSliderBk_devThermostat_knob = NULL;
static lv_style_t *styleTextBtnBk_devThermostat_tempAdj = NULL;
static lv_style_t *styleCb_devThermostat_EcoEnText = NULL;
static lv_style_t *styleCb_devThermostat_EcoEnBoxRel = NULL;
static lv_style_t *styleCb_devThermostat_EcoEnBoxTgRel = NULL;
static lv_style_t *styleRoller_thermostatGearSel_bg = NULL;
static lv_style_t *styleRoller_thermostatGearSel_sel = NULL;
static lv_style_t *styleBtn_thermostatVentFan_rel = NULL;
static lv_style_t *styleBtn_thermostatVentFan_pre = NULL;
static lv_style_t *styleText_thermostatEx_btnFold = NULL;
static lv_style_t *stylePreload_devScenario_driverCalmDown = NULL;
static lv_style_t *styleBtn_devScenario_driverCalmDown = NULL;
static lv_style_t *styleImgBk_underlying = NULL;
static lv_style_t *stylebtnBk_transFull = NULL;
static lv_style_t *stylePage_sysRestartTips = NULL;
static lv_style_t *styleLabelCounter_sysRestartTips = NULL;
static lv_style_t *styleLabelRef_sysRestartTips = NULL;
static lv_style_t *styleIconBinding_reserveIf = NULL;
static lv_style_t *stylePage_guiBlockTips = NULL;
static lv_style_t *styleLabelRef_guiBlockTips = NULL;
static lv_style_t *stylePreload_guiBlockTips = NULL;

static lv_style_t *styleBroundPicOrg_prevStyle = NULL;

//其他本地变量
static bool screenFullReminder_flg = false;

static bool	localMemory_lvObjStyle_mallocedFlg = false;

static lv_color_t scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0xffffff);

static stt_bGroundThemeParam bGroundImg_themeParam = {

	.bGround_keyTheme_ist = homepageThemeType_ouZhou,
	.bGround_picOrg_ist = bGroudImg_objInsert_figureT, 
};
//static uint8_t homepageThemeType_typeFlg = homepageThemeType_ouZhou;
//static uint8_t bGroundPic_formatInsert = bGroudImg_objInsert_usrPic;

static uint8_t lvGuiRefreshCounter_bussinessBk = 0;
static uint8_t guiInfoRefresh_calmDn_counter = 0;

static char textStr_time[16] = {0};
static char textStr_nodeNum[8] = {0}; 
static char textStr_elecSum[32] = {0};
static char textStr_temperature[16] = {0}; 

static char str_sliderBkVal_devDimmer[8] = {0};
static char str_devRunningRemind_devHeater[16] = {0};
static char str_devParamPositionCur_devCurtain[8] = {0};
static char str_devParamPositionAdj_devCurtain[8] = {0};

static char 	str_pageGuiBlockTips[64] = "block demo...";
static char		str_tipsFullScreen[64] = "tips full scr";
static uint8_t 	icon_pageGuiBlockType = 0;

static struct _guiSwitchParamAttr{

	usrGuiBussiness_type guiTarget;
	funcPtr_lvUsrGuiSwitchPreAction usrAppGuiSwitch_preFunc;
	bool usrAppGuiSwitch_flg;
	
}usrApp_guiSwitchParam;

static uint8_t btnBindingStatus_record[DEVICE_MUTUAL_CTRL_GROUP_NUM] = {0}; //按钮绑定图标本地比较记录缓存

const char *btnm_str_devHeater_X[DEVICE_HEATER_OPREAT_ACTION_NUM + 2] = {"Off", "On", "30min", "\n", "60min", SYMBOL_EDIT"Custom", ""};
const char *btnm_str_devHeater_Y[DEVICE_HEATER_OPREAT_ACTION_NUM + 1] = {"Off", "On", "30m", "60m", SYMBOL_EDIT, ""};
const char *btnm_str_devFans[DEVICE_FANS_OPREAT_ACTION_NUM + 1] = {"C", "1", "2", "3", ""};
const char rollerGearStr_devThermostat[] = {"OFF\n""HEAT\n""COOL\n""AUTO"};

//不同类型开关所需的参照常量
static const struct _rollerDispConferenceTab_devFans{

	stt_devFans_opratAct opreatActCurrent;
	uint16_t optionSelect;

}objDevFans_btnmDispConferenceTab[DEVICE_FANS_OPREAT_ACTION_NUM] = { //风扇开关不同档位对应ui显示值参照表

	{fansOpreatAct_stop,		0},
	{fansOpreatAct_firstGear,	1},
	{fansOpreatAct_secondGear,	2},
	{fansOpreatAct_thirdGear,	3},
};

static const struct _rollerDispConferenceTab_devHeater{

	stt_devHeater_opratAct opreatActCurrent;
	uint16_t optionSelect;

}objDevHeater_btnmDispConferenceTab[DEVICE_HEATER_OPREAT_ACTION_NUM] = { //风扇开关不同档位对应ui显示值参照表

	{heaterOpreatAct_close,					0},
	{heaterOpreatAct_open,					1},
	{heaterOpreatAct_closeAfter30Min,		2},
	{heaterOpreatAct_closeAfter60Min,		3},
	{heaterOpreatAct_closeAfterTimeCustom,	4},
};

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)
static const struct _rollerDispModeSel_devThermostat{

	stt_devThermostat_runningStatus opreatActCurrent;
	uint16_t optionSelect;

}objDevThermostat_rollerDispModeSel[DEVICE_THERMOSTAT_OPREAT_ACTION_NUM] = { //风扇开关不同档位对应ui显示值参照表

	{thermostatRunningStatus_mode_stop,		0},
	{thermostatRunningStatus_mode_heat,		1},
	{thermostatRunningStatus_mode_cool,		2},
	{thermostatRunningStatus_mode_auto,		3},
};
#endif

static char textDispTemp_thermostatExBtnFold_temp[64] = {0};
static bool dispFlg_devThermostat_firstPage = false;

static void lvGuiHome_styleApplicationInit(void);
static void lvGui_usrAppPowerUp_Animation(void);
static lv_res_t funCb_btnActionClick_devThermoEx_fold(lv_obj_t *btn);
static void local_guiHomeBussiness_thermostat(lv_obj_t * obj_Parent);
static void local_guiHomeBussiness_thermostatExtension(lv_obj_t * obj_Parent);
static void lvGui_homepageRefresh(void);
static void lvglUsrApp_epidemicSituation_disp(bool creatIf);
static bool usrApp_temperatureCtrlObj_reserveGet(void);
static void lvGui_usrSwitch(usrGuiBussiness_type guiPage, bool corpse_if);
static void usrApp_temperatureCtrlObj_generate(void);

#if(SCREENSAVER_RUNNING_ENABLE == 1)

 static void lvglUsrApp_epidemicSituationDisp_allObjRefresh(void);
#endif

static void usrApp_ctrlObjSlidlingTrig(void){

	ctrlObj_slidingCalmDownCounter = 1500;
}

static void usrAppHomepageRecovery_dispTimeoutCount_refresh(void){

	homepageRecovery_timeoutCounter = 120;
}

void usrAppHomepageThemeType_Set(const uint8_t themeType_flg, bool recommendBpic_if, bool nvsRecord_IF){

	bGroundImg_themeParam.bGround_keyTheme_ist = themeType_flg;
	stt_msgDats_dataManagementHandle sptr_msgQ_dmHandle = {0};

	if(false == localMemory_lvObjStyle_mallocedFlg)return;

	switch(bGroundImg_themeParam.bGround_keyTheme_ist){

		case homepageThemeType_jianJie:{

			styleIconvText_devMulitSw_statusOn->image.color = LV_COLOR_MAKE(166, 126, 79);
			styleIconvText_devMulitSw_statusOn->image.intense = LV_OPA_COVER;
			styleIconvText_devMulitSw_statusOn->text.color = LV_COLOR_MAKE(166, 126, 79);
			styleIconvText_devMulitSw_statusOff->image.color = LV_COLOR_WHITE;
			styleIconvText_devMulitSw_statusOff->image.intense = LV_OPA_COVER;
			styleIconvText_devMulitSw_statusOff->text.color = LV_COLOR_WHITE;
			
			if(true == recommendBpic_if)bGroundImg_themeParam.bGround_picOrg_ist = 1;

		}break;
		
		case homepageThemeType_keAi:{
			
			styleIconvText_devMulitSw_statusOn->image.color = LV_COLOR_MAKE(204, 151, 150);
			styleIconvText_devMulitSw_statusOn->image.intense = LV_OPA_COVER;
			styleIconvText_devMulitSw_statusOn->text.color = LV_COLOR_MAKE(204, 151, 150);
			styleIconvText_devMulitSw_statusOff->image.color = LV_COLOR_WHITE;
			styleIconvText_devMulitSw_statusOff->image.intense = LV_OPA_COVER;
			styleIconvText_devMulitSw_statusOff->text.color = LV_COLOR_WHITE;

			if(true == recommendBpic_if)bGroundImg_themeParam.bGround_picOrg_ist = 2;

		}break;
		
		default:{

			styleIconvText_devMulitSw_statusOn->image.color = LV_COLOR_GRAY;
			styleIconvText_devMulitSw_statusOn->image.intense = LV_OPA_COVER;
			styleIconvText_devMulitSw_statusOn->text.color = LV_COLOR_BLACK;
			styleIconvText_devMulitSw_statusOff->image.color = LV_COLOR_WHITE;
			styleIconvText_devMulitSw_statusOff->image.intense = LV_OPA_COVER;
			styleIconvText_devMulitSw_statusOff->text.color = LV_COLOR_WHITE;

			if(true == recommendBpic_if)bGroundImg_themeParam.bGround_picOrg_ist = 20;

		}break;
	}

	styleIconvText_devMulitSw_statusOn->text.font = styleBtn_Text->text.font;
	styleIconvText_devMulitSw_statusOff->text.font = styleBtn_Text->text.font;

	if(nvsRecord_IF){

		devSystemInfoLocalRecord_save(saveObj_devGuiBussinessHome_themeParam, &bGroundImg_themeParam);
	}

	/*UI刷新通知最后进行*/
	sptr_msgQ_dmHandle.msgType = dataManagement_msgType_homePageThemeTypeChg;
	sptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpThemeTypeChg.themeTypeChg_notice = 1;
	xQueueSend(msgQh_dataManagementHandle, &sptr_msgQ_dmHandle, 1 / portTICK_PERIOD_MS);

	if(true == recommendBpic_if){

		sptr_msgQ_dmHandle.msgType = dataManagement_msgType_homePagePicGroundChg;
		sptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpPicGroundChg.picGroundChg_notice = 1;
		xQueueSend(msgQh_dataManagementHandle, &sptr_msgQ_dmHandle, 1 / portTICK_PERIOD_MS);
	}
}

void usrAppHomepageBgroundPicOrg_Set(const uint8_t picIst, bool nvsRecord_IF, bool refresh_IF){

	bGroundImg_themeParam.bGround_picOrg_ist = picIst;

	if(nvsRecord_IF){

		devSystemInfoLocalRecord_save(saveObj_devGuiBussinessHome_themeParam, &bGroundImg_themeParam);
	}

	if(refresh_IF){

		stt_msgDats_dataManagementHandle sptr_msgQ_dmHandle = {0};

		sptr_msgQ_dmHandle.msgType = dataManagement_msgType_homePagePicGroundChg;
		sptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpPicGroundChg.picGroundChg_notice = 1;
		xQueueSend(msgQh_dataManagementHandle, &sptr_msgQ_dmHandle, 1 / portTICK_PERIOD_MS);
	}
}

uint8_t usrAppHomepageThemeType_Get(void){

	return bGroundImg_themeParam.bGround_keyTheme_ist;
}

uint8_t usrAppHomepageBgroundPicOrg_Get(void){

	return bGroundImg_themeParam.bGround_picOrg_ist;
}

lv_style_t *usrAppHomepageBkPicStyle_prevDataGet(uint8_t ist){

	lv_style_copy(styleBroundPicOrg_prevStyle, &lv_style_plain);

//	switch(bGroundImg_themeParam.bGround_picOrg_ist){

//		case bGroudImg_objInsert_usrPic:{}break;
//			
//		case bGroudImg_objInsert_pureColor1:{}break;
//		case bGroudImg_objInsert_pureColor2:{}break;
//		case bGroudImg_objInsert_pureColor3:{}break;
//		case bGroudImg_objInsert_pureColor4:{}break;
//		case bGroudImg_objInsert_pureColor5:{}break;
//		case bGroudImg_objInsert_pureColor6:{}break;
//		case bGroudImg_objInsert_pureColor7:{}break;

//		case bGroudImg_objInsert_figureA:{}break;
//		case bGroudImg_objInsert_figureB:{}break;
//		case bGroudImg_objInsert_figureC:{}break;

//		default:break;
//	}

	return styleBroundPicOrg_prevStyle;
}

lv_img_dsc_t *usrAppHomepageBkPic_prevDataGet(uint8_t ist){

	const lv_img_dsc_t *res = NULL;

	switch(ist){

		case bGroudImg_objInsert_usrPic:{res = &bGroundPrev_picFigure_pic;}break;
	
		case bGroudImg_objInsert_figureA:{res = &bGroundPrev_picFigure_1;}break;
		case bGroudImg_objInsert_figureB:{res = &bGroundPrev_picFigure_2;}break;
		case bGroudImg_objInsert_figureC:{res = &bGroundPrev_picFigure_3;}break;
		case bGroudImg_objInsert_figureD:{res = &bGroundPrev_picFigure_4;}break;
		case bGroudImg_objInsert_figureE:{res = &bGroundPrev_picFigure_5;}break;
		case bGroudImg_objInsert_figureF:{res = &bGroundPrev_picFigure_6;}break;
		case bGroudImg_objInsert_figureG:{res = &bGroundPrev_picFigure_7;}break;
		case bGroudImg_objInsert_figureH:{res = &bGroundPrev_picFigure_8;}break;
		case bGroudImg_objInsert_figureI:{res = &bGroundPrev_picFigure_9;}break;
		case bGroudImg_objInsert_figureJ:{res = &bGroundPrev_picFigure_10;}break;
		case bGroudImg_objInsert_figureK:{res = &bGroundPrev_picFigure_11;}break;
		case bGroudImg_objInsert_figureL:{res = &bGroundPrev_picFigure_12;}break;
		case bGroudImg_objInsert_figureM:{res = &bGroundPrev_picFigure_13;}break;
		case bGroudImg_objInsert_figureN:{res = &bGroundPrev_picFigure_14;}break;
		case bGroudImg_objInsert_figureO:{res = &bGroundPrev_picFigure_15;}break;
		case bGroudImg_objInsert_figureP:{res = &bGroundPrev_picFigure_16;}break;
		case bGroudImg_objInsert_figureQ:{res = &bGroundPrev_picFigure_17;}break;
		case bGroudImg_objInsert_figureR:{res = &bGroundPrev_picFigure_18;}break;
		case bGroudImg_objInsert_figureS:{res = &bGroundPrev_picFigure_19;}break;
		case bGroudImg_objInsert_figureT:{res = &bGroundPrev_picFigure_20;}break;

		case bGroudImg_objInsert_pureColor1:{res = &bGroundPrev_picColor_1;}break;
		case bGroudImg_objInsert_pureColor2:{res = &bGroundPrev_picColor_2;}break;
		case bGroudImg_objInsert_pureColor3:{res = &bGroundPrev_picColor_3;}break;
		case bGroudImg_objInsert_pureColor4:{res = &bGroundPrev_picColor_4;}break;
		case bGroudImg_objInsert_pureColor5:{res = &bGroundPrev_picColor_5;}break;
		case bGroudImg_objInsert_pureColor6:{res = &bGroundPrev_picColor_6;}break;
		case bGroudImg_objInsert_pureColor7:{res = &bGroundPrev_picColor_7;}break;

		default:{res = &bGroundPrev_picFigure_null;}break;
	}

	return res;
}

static lv_img_dsc_t *usrAppHomepageBkPic_dataGet(void){

	const lv_img_dsc_t *res = NULL;

	if(bGroundPic_reserveFlg){

		imgData_bGroudPic.data = dataPtr_bGroundPic;
		res = (lv_img_dsc_t *)&imgData_bGroudPic;

		printf("bkPicGet usr.\n");
	}
	else{

		printf("bkPicGet def.\n");

		if(devStatusDispMethod_landscapeIf_get()){

			switch(bGroundImg_themeParam.bGround_keyTheme_ist){
			
//				case homepageThemeType_jianJie:{res = &homepage_bkPic_jijianSl;}break;
//				case homepageThemeType_keAi:{res = &homepage_bkPic_keAiSl;}break;
//				case homepageThemeType_ouZhou:{res = &homepage_bkPic_jianJieSl;}break;
				default:{res = &homepage_bkPic_palaceSl;}break;
			}
		}
		else
		{
			switch(bGroundImg_themeParam.bGround_keyTheme_ist){
			
//				case homepageThemeType_jianJie:{res = &homepage_bkPic_jianJie;}break;
//				case homepageThemeType_keAi:{res = &homepage_bkPic_keAi;}break;
//				case homepageThemeType_ouZhou:{res = &homepage_bkPic_jiJian;}break;
				default:{res = &homepage_bkPic_palace;}break;
			}
		}
	}

	return res;
}

static void usrAppBgroudObj_styleRefresh(void){

	switch(guiPage_current){

		case bussinessType_Home:
		case bussinessType_Menu:{

			switch(bGroundImg_themeParam.bGround_picOrg_ist){
			
				case bGroudImg_objInsert_usrPic:{

					scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0xe0e0e0);
					
//					lv_img_set_src(objFigure_ptr[0], usrAppHomepageBkPic_dataGet());
					scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x5d462b);
					
				}break;

				case bGroudImg_objInsert_figureA:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x5d462b);}break;
				case bGroudImg_objInsert_figureB:{scrHeaderSysInfo_baseColor = LV_COLOR_MAKE(204, 151, 150);}break;
				case bGroudImg_objInsert_figureC:{scrHeaderSysInfo_baseColor = LV_COLOR_MAKE(210, 227, 0);}break;
				case bGroudImg_objInsert_figureD:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x5d462b);}break;
				case bGroudImg_objInsert_figureE:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x5d462b);}break;
				case bGroudImg_objInsert_figureF:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x1d1d1d);}break;
				case bGroudImg_objInsert_figureG:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x1d1d1d);}break;
				case bGroudImg_objInsert_figureH:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x1d1d1d);}break;
				case bGroudImg_objInsert_figureI:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x1d1d1d);}break;
				case bGroudImg_objInsert_figureJ:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x1d1d1d);}break;
				case bGroudImg_objInsert_figureK:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x1d1d1d);}break;
				case bGroudImg_objInsert_figureL:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x1d1d1d);}break;
				case bGroudImg_objInsert_figureM:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x1d1d1d);}break;
				case bGroudImg_objInsert_figureN:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0xffffff);}break;
				case bGroudImg_objInsert_figureO:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x1d1d1d);}break;
				case bGroudImg_objInsert_figureP:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x1d1d1d);}break;
				case bGroudImg_objInsert_figureQ:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0xffffff);}break;
				case bGroudImg_objInsert_figureR:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x1d1d1d);}break;
				case bGroudImg_objInsert_figureS:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x1d1d1d);}break;
				case bGroudImg_objInsert_figureT:{scrHeaderSysInfo_baseColor = LV_COLOR_MAKE(212, 212, 212);}break;

				case bGroudImg_objInsert_pureColor1:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0xffffff);}break;
				case bGroudImg_objInsert_pureColor2:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0xffffff);}break;
				case bGroudImg_objInsert_pureColor3:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0xffffff);}break;
				case bGroudImg_objInsert_pureColor4:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0xffffff);}break;
				case bGroudImg_objInsert_pureColor5:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0x2c2c2c);}break;
				case bGroudImg_objInsert_pureColor6:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0xffffff);}break;
				case bGroudImg_objInsert_pureColor7:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0xffffff);}break;
			
				default:{scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0xffffff);}break;
			}
			
		}break;

		default:{

			scrHeaderSysInfo_baseColor = LV_COLOR_HEX(0xe0e0e0);

		}break;
	}

	styleText_time->text.color = scrHeaderSysInfo_baseColor;
	styleImg_iconHeater->image.color = scrHeaderSysInfo_baseColor;
	styleText_temperature->text.color = scrHeaderSysInfo_baseColor;
	styleText_elecAnodenum->text.color = scrHeaderSysInfo_baseColor;

	if(textHeaderObj_time)
		lv_obj_set_style(textHeaderObj_time, styleText_time);
	if(iconHeaderObj_tempraA)
		lv_obj_set_style(iconHeaderObj_tempraA, styleImg_iconHeater);
	if(iconHeaderObj_tempraB)
		lv_obj_set_style(iconHeaderObj_tempraB, styleImg_iconHeater);
	if(iconHeaderObj_elec)
		lv_obj_set_style(iconHeaderObj_elec, styleImg_iconHeater);
	if(iconHeaderObj_alarm)
		lv_obj_set_style(iconHeaderObj_alarm, styleImg_iconHeater);
	if(iconHeaderObj_node){ //仅子设备时，图标根据界面变色

		lv_img_set_style(iconHeaderObj_node, styleImg_iconHeater);

		// if(mwifi_is_connected()){
		// 	if(esp_mesh_get_layer() != MESH_ROOT){

		// 		styleIcon_meshNode->image.color = scrHeaderSysInfo_baseColor;
		// 		lv_img_set_style(iconHeaderObj_node, styleIcon_meshNode);	
		// 	}	
		// }		
	}
}

static void usrAppBgroudObj_figurePicPullze(const lv_img_dsc_t *figPic){

	uint8_t loop_x = 0,
			loop_y = 0;
	uint8_t loopCreat = 0;

	if(devStatusDispMethod_landscapeIf_get()){

		loop_x = 320 / figPic->header.w;
		loop_y = 240 / figPic->header.h;

		if(320 % figPic->header.w)loop_x += 1;
		if(240 % figPic->header.h)loop_y += 1;
	}
	else
	{
		loop_x = 240 / figPic->header.w;
		loop_y = 320 / figPic->header.h;

		if(240 % figPic->header.w)loop_x += 1;
		if(320 % figPic->header.h)loop_y += 1;
	}

	objFigure_ptr[0] = lv_img_create(objPicBg_ptr, NULL);
	lv_img_set_src(objFigure_ptr[0], figPic);
	lv_obj_set_protect(objFigure_ptr[0], LV_PROTECT_POS);
	lv_obj_set_pos(objFigure_ptr[0], 0, 0);

	for(loopCreat = 1; loopCreat < (loop_x * loop_y); loopCreat ++){
	
		objFigure_ptr[loopCreat] = lv_img_create(objPicBg_ptr, objFigure_ptr[0]);
		lv_obj_set_protect(objFigure_ptr[0], LV_PROTECT_POS);
		lv_obj_set_pos(objFigure_ptr[loopCreat], loopCreat % loop_x * figPic->header.w, loopCreat / loop_x * figPic->header.h);
	}
}

static void usrAppBgroudObj_pureColorGenerate(lv_color_t objColor, uint8_t opa){

	styleBgPic_colorStyle->body.main_color = objColor;
	styleBgPic_colorStyle->body.grad_color = objColor;
	styleBgPic_colorStyle->body.opa = opa;
	lv_obj_set_style(objPicBg_ptr, styleBgPic_colorStyle);
}

static lv_obj_t *usrAppHomepageBground_objPicFigure_get(void){

	uint8_t loopCreat = 0;

	objPicBg_ptr = lv_obj_create(lv_scr_act(), NULL);

	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(objPicBg_ptr, 320, 240)):
		(lv_obj_set_size(objPicBg_ptr, 240, 320));

//	switch(bGroundImg_themeParam.bGround_picOrg_ist){

//		case bGroudImg_objInsert_usrPic:{}break;
//			
//		case bGroudImg_objInsert_pureColor1:{}break;
//		case bGroudImg_objInsert_pureColor2:{}break;
//		case bGroudImg_objInsert_pureColor3:{}break;
//		case bGroudImg_objInsert_pureColor4:{}break;
//		case bGroudImg_objInsert_pureColor5:{}break;
//		case bGroudImg_objInsert_pureColor6:{}break;
//		case bGroudImg_objInsert_pureColor7:{}break;

//		case bGroudImg_objInsert_figureA:{}break;
//		case bGroudImg_objInsert_figureB:{}break;
//		case bGroudImg_objInsert_figureC:{}break;

//		default:break;
//	}

	switch(bGroundImg_themeParam.bGround_picOrg_ist){ 

		case bGroudImg_objInsert_usrPic:{
			
			objFigure_ptr[0] = lv_img_create(objPicBg_ptr, NULL);
			
//			lv_img_set_src(objFigure_ptr[0], usrAppHomepageBkPic_dataGet());
			usrAppBgroudObj_figurePicPullze(&bGround_picFigure_0);

		}break;
		
		case bGroudImg_objInsert_figureA:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_1);}break;
		case bGroudImg_objInsert_figureB:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_2);}break;
		case bGroudImg_objInsert_figureC:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_3);}break;
		case bGroudImg_objInsert_figureD:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_4);}break;
		case bGroudImg_objInsert_figureE:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_5);}break;
		case bGroudImg_objInsert_figureF:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_6);}break;
		case bGroudImg_objInsert_figureG:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_7);}break;
		case bGroudImg_objInsert_figureH:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_8);}break;
		case bGroudImg_objInsert_figureI:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_9);}break;
		case bGroudImg_objInsert_figureJ:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_10);}break;
		case bGroudImg_objInsert_figureK:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_11);}break;
		case bGroudImg_objInsert_figureL:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_12);}break;
		case bGroudImg_objInsert_figureM:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_13);}break;
		case bGroudImg_objInsert_figureN:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_14);}break;
		case bGroudImg_objInsert_figureO:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_15);}break;
		case bGroudImg_objInsert_figureP:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_16);}break;
		case bGroudImg_objInsert_figureQ:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_17);}break;
		case bGroudImg_objInsert_figureR:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_18);}break;
		case bGroudImg_objInsert_figureS:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_19);}break;
		case bGroudImg_objInsert_figureT:{usrAppBgroudObj_figurePicPullze(&bGround_picFigure_20);}break;

		case bGroudImg_objInsert_pureColor1:{usrAppBgroudObj_pureColorGenerate(LV_COLOR_HEX(0xe0d5ce), LV_OPA_100);}break;
		case bGroudImg_objInsert_pureColor2:{usrAppBgroudObj_pureColorGenerate(LV_COLOR_HEX(0xe2cea7), LV_OPA_100);}break;
		case bGroudImg_objInsert_pureColor3:{usrAppBgroudObj_pureColorGenerate(LV_COLOR_HEX(0xf0d7da), LV_OPA_100);}break;
		case bGroudImg_objInsert_pureColor4:{usrAppBgroudObj_pureColorGenerate(LV_COLOR_HEX(0xe7dddc), LV_OPA_100);}break;
		case bGroudImg_objInsert_pureColor5:{usrAppBgroudObj_pureColorGenerate(LV_COLOR_HEX(0xf8eedb), LV_OPA_100);}break;
		case bGroudImg_objInsert_pureColor6:{usrAppBgroudObj_pureColorGenerate(LV_COLOR_HEX(0xa6afb2), LV_OPA_100);}break;
		case bGroudImg_objInsert_pureColor7:{usrAppBgroudObj_pureColorGenerate(LV_COLOR_HEX(0xaaaaaa), LV_OPA_100);}break;

		default:{}break;
	}

	return objPicBg_ptr;
}

static lv_img_dsc_t *usrAppHomepageBindingIcon_dataGet(void){

	const lv_img_dsc_t *res = NULL;

	switch(currentDev_typeGet()){

		case devTypeDef_dimmer:
		case devTypeDef_curtain:{

			res = &iconPage_binding;
			
		}break;

		default:{

			res = &iconPage_binding_x;
			
		}break;
	}

	return res;
}

static lv_img_dsc_t *usrAppHomepageBtnBkPic_dataGet(bool pic_preIf){

	const lv_img_dsc_t *res = NULL;

	if(devStatusDispMethod_landscapeIf_get()){

		switch(bGroundImg_themeParam.bGround_keyTheme_ist){
			
			case homepageThemeType_jianJie:{
		
				(pic_preIf)?(res = &homepage_buttonPicRel_jiJianSl):(res = &homepage_buttonPicPre_jiJianSl); //色调调反，黑关白开
		
			}break;
			
			case homepageThemeType_keAi:{
		
				(pic_preIf)?(res = &homepage_buttonPicRel_keAiSl):(res = &homepage_buttonPicPre_keAiSl);
		
			}break;
		
			default:{
		
				(pic_preIf)?(res = &homepage_buttonPicRel_jianJieSl):(res = &homepage_buttonPicPre_jianJieSl);
		
			}break;
		}
	}
	else
	{
		switch(bGroundImg_themeParam.bGround_keyTheme_ist){
			
			case homepageThemeType_jianJie:{
		
				(pic_preIf)?(res = &homepage_buttonPicRel_jianJie):(res = &homepage_buttonPicPre_jianJie); //色调调反，黑关白开
		
			}break;
			
			case homepageThemeType_keAi:{
		
				(pic_preIf)?(res = &homepage_buttonPicRel_fenSe):(res = &homepage_buttonPicPre_fenSe);
		
			}break;
		
			default:{
		
				(pic_preIf)?(res = &homepage_buttonPicRel_jiJian):(res = &homepage_buttonPicPre_jiJian);
		
			}break;
		}
	}

	return res;
}

static lv_font_t *usrAppHomepageBtnBkText_fontGet(uint8_t countryFlg){

	lv_font_t *res = NULL;

	switch(countryFlg){
		
		case countryT_Arabic:
			
				res = &lv_font_arabic_17;
				break;
		
		case countryT_Hebrew:

				res = &lv_font_hebrew_17;
				break;

		case countryT_EnglishSerail:
		default:

				res = &lv_font_consola_17;
				break;
	}

	return res;
}

static lv_res_t funCb_btnActionClick_devMulitSw_mainBtnA(lv_obj_t *btn){

	stt_devDataPonitTypedef devDataPoint = {0};

	currentDev_dataPointGet(&devDataPoint);

	switch(currentDev_typeGet()){

		case devTypeDef_mulitSwOneBit:
		case devTypeDef_relayBox_1bit:{

			devDataPoint.devType_mulitSwitch_oneBit.opCtrl_bit1 = \
			devDataPoint.devType_mulitSwitch_oneBit.rsv = 0;
		
			devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1 = !devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1;
			currentDev_dataPointSet(&devDataPoint, true, mutualCtrlTrigIf_A, true, true, true);
//			(devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
//			(devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
//			(devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
//			(devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1)?
//				(lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOn)):
//				(lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOff));
		}break;

		case devTypeDef_mulitSwTwoBit:{

//			usrApplication_systemRestartTrig(5); //debug

			devDataPoint.devType_mulitSwitch_twoBit.opCtrl_bit1 = \
			devDataPoint.devType_mulitSwitch_twoBit.opCtrl_bit2 = \
			devDataPoint.devType_mulitSwitch_twoBit.rsv = 0;
			
			devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1 = !devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1;
			currentDev_dataPointSet(&devDataPoint, true, mutualCtrlTrigIf_A, true, true, true);
//			(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
//			(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
//			(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
//			(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?
//				(lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOn)):
//				(lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOff));
		}break;

		case devTypeDef_mulitSwThreeBit:{

			devDataPoint.devType_mulitSwitch_threeBit.opCtrl_bit1 = \
			devDataPoint.devType_mulitSwitch_threeBit.opCtrl_bit2 = \
			devDataPoint.devType_mulitSwitch_threeBit.opCtrl_bit3 = \
			devDataPoint.devType_mulitSwitch_threeBit.rsv = 0;

			devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1 = !devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1;
			currentDev_dataPointSet(&devDataPoint, true, mutualCtrlTrigIf_A, true, true, true);
//			(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
//			(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
//			(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
//			(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?
//				(lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOn)):
//				(lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOff));
		}break;

		case devTypeDef_scenario:{

			devDataPoint.devType_scenario.devScenario_opNum = 1;
			currentDev_dataPointSet(&devDataPoint, true, false, true, true, true);

		}break;

		case devTypeDef_thermostatExtension:{

			local_guiHomeBussiness_thermostat(imageBK);
			
			iconBtn_thermostatEx_pageReturn = lv_imgbtn_create(imageBK, NULL);
			lv_imgbtn_set_src(iconBtn_thermostatEx_pageReturn, LV_BTN_STATE_REL, &specifyIcon_thermPageReturn);
			lv_imgbtn_set_src(iconBtn_thermostatEx_pageReturn, LV_BTN_STATE_PR, &specifyIcon_thermPageReturn);
			lv_obj_set_protect(iconBtn_thermostatEx_pageReturn, LV_PROTECT_POS);
			(devStatusDispMethod_landscapeIf_get())?
				(lv_obj_align(iconBtn_thermostatEx_pageReturn, imageBK, LV_ALIGN_IN_TOP_LEFT, 15, 25)):
				(lv_obj_align(iconBtn_thermostatEx_pageReturn, imageBK, LV_ALIGN_IN_TOP_LEFT, 10, 25));
			lv_imgbtn_set_action(iconBtn_thermostatEx_pageReturn, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devThermoEx_fold);
			lv_imgbtn_set_style(iconBtn_thermostatEx_pageReturn, LV_IMGBTN_STYLE_PR, styleBtn_devMulitSw_statusOn);
			lv_imgbtn_set_style(iconBtn_thermostatEx_pageReturn, LV_IMGBTN_STYLE_REL, styleBtn_devMulitSw_statusOff);

			lv_obj_del(btn_bk_thermostatEx_fold);
			lv_obj_del(btn_bk_devMulitSw_A);
			lv_obj_del(btn_bk_devMulitSw_B);

			dispFlg_devThermostat_firstPage = false;

		}break;

		default:break;
	}

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devMulitSw_mainBtnB(lv_obj_t *btn){

	stt_devDataPonitTypedef devDataPoint = {0};

	currentDev_dataPointGet(&devDataPoint);

	switch(currentDev_typeGet()){

		case devTypeDef_mulitSwTwoBit:{

			devDataPoint.devType_mulitSwitch_twoBit.opCtrl_bit1 = \
			devDataPoint.devType_mulitSwitch_twoBit.opCtrl_bit2 = \
			devDataPoint.devType_mulitSwitch_twoBit.rsv = 0;

			devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2 = !devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2;
			currentDev_dataPointSet(&devDataPoint, true, mutualCtrlTrigIf_B, true, true, true);
//			(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
//			(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
//			(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));
//			(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?
//				(lv_imgbtn_set_style(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOn)):
//				(lv_imgbtn_set_style(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOff));
		}break;
		
		case devTypeDef_mulitSwThreeBit:{

			devDataPoint.devType_mulitSwitch_threeBit.opCtrl_bit1 = \
			devDataPoint.devType_mulitSwitch_threeBit.opCtrl_bit2 = \
			devDataPoint.devType_mulitSwitch_threeBit.opCtrl_bit3 = \
			devDataPoint.devType_mulitSwitch_threeBit.rsv = 0;

			devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2 = !devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2;
			currentDev_dataPointSet(&devDataPoint, true, mutualCtrlTrigIf_B, true, true, true);
			(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
//			(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
//			(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));
//			(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?
//				(lv_imgbtn_set_style(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOn)):
//				(lv_imgbtn_set_style(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOff));
		}break;

		case devTypeDef_scenario:{

			devDataPoint.devType_scenario.devScenario_opNum = 2;
			currentDev_dataPointSet(&devDataPoint, true, false, true, true, true);
		
		}break;

		case devTypeDef_thermostatExtension:{

			uint8_t devThermostatSwStatus = devDriverBussiness_thermostatSwitch_exSwitchParamGet();

			devThermostatSwStatus ^= (1 << 0);

			devDriverBussiness_thermostatSwitch_exSwitchParamSet(devThermostatSwStatus);
			devDriverParamChg_dataRealesTrig(true, mutualCtrlTrigIf_A, true, true, true); //上报触发

		}break;

		default:break;
	}

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devMulitSw_mainBtnC(lv_obj_t *btn){

	stt_devDataPonitTypedef devDataPoint = {0};

	currentDev_dataPointGet(&devDataPoint);

	switch(currentDev_typeGet()){
		
		case devTypeDef_mulitSwThreeBit:{

			devDataPoint.devType_mulitSwitch_threeBit.opCtrl_bit1 = \
			devDataPoint.devType_mulitSwitch_threeBit.opCtrl_bit2 = \
			devDataPoint.devType_mulitSwitch_threeBit.opCtrl_bit3 = \
			devDataPoint.devType_mulitSwitch_threeBit.rsv = 0;
		
			devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3 = !devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3;
			currentDev_dataPointSet(&devDataPoint, true, mutualCtrlTrigIf_C, true, true, true);
//			(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?(lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
//			(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?(lv_img_set_style(iconBtn_toilet, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_toilet, styleIconvText_devMulitSw_statusOff));
//			(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?(lv_label_set_style(textBtn_toilet, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_toilet, styleIconvText_devMulitSw_statusOff));
//			(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?
//				(lv_imgbtn_set_style(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOn)):
//				(lv_imgbtn_set_style(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOff));
		}break;

		case devTypeDef_scenario:{

			devDataPoint.devType_scenario.devScenario_opNum = 4;
			currentDev_dataPointSet(&devDataPoint, true, false, true, true, true);

		}break;

		case devTypeDef_thermostatExtension:{

			uint8_t devThermostatSwStatus = devDriverBussiness_thermostatSwitch_exSwitchParamGet();
			
			devThermostatSwStatus ^= (1 << 1);
			
			devDriverBussiness_thermostatSwitch_exSwitchParamSet(devThermostatSwStatus);
			devDriverParamChg_dataRealesTrig(true, mutualCtrlTrigIf_B, true, true, true); //上报触发

		}break;

		default:break;
	}

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devThermostat_tempAdd(lv_obj_t *btn){

	stt_devDataPonitTypedef devDataPoint = {0};
	char tempDisp[5] = {0};

	currentDev_dataPointGet(&devDataPoint);
	
//	printf("add+.\n");

	if(devDataPoint.devType_thermostat.devThermostat_tempratureTarget == 0)
		devDataPoint.devType_thermostat.devThermostat_tempratureTarget = 16;

	if(devDataPoint.devType_thermostat.devThermostat_tempratureTarget < 40)
		devDataPoint.devType_thermostat.devThermostat_tempratureTarget += 1;

	currentDev_dataPointSet(&devDataPoint, true, false, true, true, true);
	
	lv_slider_set_value(slider_tempAdj_devThermostat, devDataPoint.devType_thermostat.devThermostat_tempratureTarget - 16);
	sprintf(tempDisp, "%02d", (uint8_t)devDataPoint.devType_thermostat.devThermostat_tempratureTarget);
	lv_label_set_text(labelTempInstTarget_devThermostat, tempDisp);
	lv_lmeter_set_value(lmeterTempInstTarget_devThermostat, (uint8_t)devDataPoint.devType_thermostat.devThermostat_tempratureTarget);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devThermostat_tempCut(lv_obj_t *btn){

	stt_devDataPonitTypedef devDataPoint = {0};
	char tempDisp[5] = {0};

	currentDev_dataPointGet(&devDataPoint);
	
//	printf("cut-.\n");

	if(devDataPoint.devType_thermostat.devThermostat_tempratureTarget == 0)
		devDataPoint.devType_thermostat.devThermostat_tempratureTarget = 16;

	if(devDataPoint.devType_thermostat.devThermostat_tempratureTarget > 16)
		devDataPoint.devType_thermostat.devThermostat_tempratureTarget -= 1;

	currentDev_dataPointSet(&devDataPoint, true, true, true, true, true);

	lv_slider_set_value(slider_tempAdj_devThermostat, devDataPoint.devType_thermostat.devThermostat_tempratureTarget - 16);
	sprintf(tempDisp, "%02d", (uint8_t)devDataPoint.devType_thermostat.devThermostat_tempratureTarget);
	lv_label_set_text(labelTempInstTarget_devThermostat, tempDisp);
	lv_lmeter_set_value(lmeterTempInstTarget_devThermostat, (uint8_t)devDataPoint.devType_thermostat.devThermostat_tempratureTarget);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devThermoEx_fold(lv_obj_t *btn){

	local_guiHomeBussiness_thermostatExtension(imageBK);

	lv_obj_del(lmeterTempInstTarget_devThermostat);
	lv_obj_del(lmeterTempInstCurrent_devThermostat);
	lv_obj_del(labelTempInstTarget_devThermostat);
	lv_obj_del(labelTempInstCurrent_devThermostat);
	lv_obj_del(slider_tempAdj_devThermostat);
	lv_obj_del(btn_tempAdjAdd_devThermostat);
	lv_obj_del(btn_tempAdjCut_devThermostat);
	lv_obj_del(sw_devRunningEnable_devThermostat);
	lv_obj_del(cb_devEcoEnable_devThermostat);
	lv_obj_del(iconBtn_thermostatEx_pageReturn);
	
	return LV_RES_OK;
}


static lv_res_t funCb_btnActionClick_devCurtain_open(lv_obj_t *btn){

	stt_devDataPonitTypedef devDataPoint = {0};

	devDataPoint.devType_curtain.devCurtain_actEnumVal = 1;
	devDataPoint.devType_curtain.devCurtain_actMethod = 0; //按钮方式

	currentDev_dataPointSet(&devDataPoint, false, mutualCtrlTrigIf_A, true, true, true);

	lv_imgbtn_set_style(btn_bk_devCurtain_open, LV_BTN_STATE_REL, styleBtn_devCurtain_statusPre);
	lv_imgbtn_set_style(btn_bk_devCurtain_stop, LV_BTN_STATE_REL, styleBtn_devCurtain_statusRel);
	lv_imgbtn_set_style(btn_bk_devCurtain_close, LV_BTN_STATE_REL, styleBtn_devCurtain_statusRel);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devCurtain_stop(lv_obj_t *btn){

	stt_devDataPonitTypedef devDataPoint = {0};

	devDataPoint.devType_curtain.devCurtain_actEnumVal = 2;
	devDataPoint.devType_curtain.devCurtain_actMethod = 0; //按钮方式

	currentDev_dataPointSet(&devDataPoint, false, mutualCtrlTrigIf_A, true, true, true);

	lv_imgbtn_set_style(btn_bk_devCurtain_open, LV_BTN_STATE_REL, styleBtn_devCurtain_statusRel);
	lv_imgbtn_set_style(btn_bk_devCurtain_stop, LV_BTN_STATE_REL, styleBtn_devCurtain_statusPre);
	lv_imgbtn_set_style(btn_bk_devCurtain_close, LV_BTN_STATE_REL, styleBtn_devCurtain_statusRel);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devCurtain_close(lv_obj_t *btn){

	stt_devDataPonitTypedef devDataPoint = {0};

	devDataPoint.devType_curtain.devCurtain_actEnumVal = 4;
	devDataPoint.devType_curtain.devCurtain_actMethod = 0; //按钮方式

	currentDev_dataPointSet(&devDataPoint, false, mutualCtrlTrigIf_A, true, true, true);

	lv_imgbtn_set_style(btn_bk_devCurtain_open, LV_BTN_STATE_REL, styleBtn_devCurtain_statusRel);
	lv_imgbtn_set_style(btn_bk_devCurtain_stop, LV_BTN_STATE_REL, styleBtn_devCurtain_statusRel);
	lv_imgbtn_set_style(btn_bk_devCurtain_close, LV_BTN_STATE_REL, styleBtn_devCurtain_statusPre);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devDimmer_mainBtn(lv_obj_t *btn){

	stt_devDataPonitTypedef devDataPoint = {0};
	uint8_t brightnessTemp = 0;

	currentDev_dataPointGet(&devDataPoint);
	brightnessTemp = devDataPoint.devType_dimmer.devDimmer_brightnessVal;

	(brightnessTemp)?
		(brightnessTemp = 0):
		(brightnessTemp = devDriverBussiness_dimmerSwitch_brightnessLastGet());

	usrApp_ctrlObjSlidlingTrig(); //滑动冷却触发

	if(brightnessTemp){

		stylePhotoBk_devDimmer->image.opa = brightnessTemp * 2 + 50;
	}
	else
	{
		stylePhotoBk_devDimmer->image.opa = 0;
	}
	lv_img_set_style(photoB_bk_devDimmer, stylePhotoBk_devDimmer);
	stylePhotoAk_devDimmer->image.intense = 100 - brightnessTemp;
	lv_img_set_style(photoA_bk_devDimmer, stylePhotoAk_devDimmer);

	sprintf(str_sliderBkVal_devDimmer, "%d%%", brightnessTemp);
	lv_label_set_text(label_bk_devDimmer, str_sliderBkVal_devDimmer);

	devDataPoint.devType_dimmer.devDimmer_brightnessVal = brightnessTemp;
	currentDev_dataPointSet(&devDataPoint, true, mutualCtrlTrigIf_A, true, true, true);

	lv_slider_set_value_anim(slider_bk_devDimmer, brightnessTemp, 300);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_bindingBtnA(lv_obj_t *btn){

	stt_devDataPonitTypedef devDataPoint = {0};

	currentDev_dataPointGet(&devDataPoint);

	switch(currentDev_typeGet()){
		
		case devTypeDef_mulitSwOneBit:
		case devTypeDef_relayBox_1bit:
		case devTypeDef_mulitSwTwoBit:
		case devTypeDef_mulitSwThreeBit:
		case devTypeDef_dimmer:
		case devTypeDef_curtain:{

			mutualCtrlTrigIf_A = !mutualCtrlTrigIf_A;
			(mutualCtrlTrigIf_A)?
				(lv_imgbtn_set_src(iconBtn_binding_A, LV_BTN_STATE_REL, usrAppHomepageBindingIcon_dataGet())):
				(lv_imgbtn_set_src(iconBtn_binding_A, LV_BTN_STATE_REL, &iconPage_unbinding));

		}break;

		default:break;
	}

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_bindingBtnB(lv_obj_t *btn){

	stt_devDataPonitTypedef devDataPoint = {0};

	currentDev_dataPointGet(&devDataPoint);

	switch(currentDev_typeGet()){

		case devTypeDef_mulitSwTwoBit:
		case devTypeDef_mulitSwThreeBit:{

			mutualCtrlTrigIf_B = !mutualCtrlTrigIf_B;
			(mutualCtrlTrigIf_B)?
				(lv_imgbtn_set_src(iconBtn_binding_B, LV_BTN_STATE_REL, usrAppHomepageBindingIcon_dataGet())):
				(lv_imgbtn_set_src(iconBtn_binding_B, LV_BTN_STATE_REL, &iconPage_unbinding));

		}break;

		default:break;
	}

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_bindingBtnC(lv_obj_t *btn){

	switch(currentDev_typeGet()){

		case devTypeDef_mulitSwThreeBit:{
		
			mutualCtrlTrigIf_C = !mutualCtrlTrigIf_C;
			(mutualCtrlTrigIf_C)?
				(lv_imgbtn_set_src(iconBtn_binding_C, LV_BTN_STATE_REL, usrAppHomepageBindingIcon_dataGet())):
				(lv_imgbtn_set_src(iconBtn_binding_C, LV_BTN_STATE_REL, &iconPage_unbinding));
		
		}break;
		
		default:break;
	}

	return LV_RES_OK;
}

static lv_res_t funCb_rollAction_devHeater_mainRoller(lv_obj_t *roller){

	uint16_t optionRollerSelect = lv_roller_get_selected(roller);

	for(uint8_t loop = 0; loop < DEVICE_HEATER_OPREAT_ACTION_NUM; loop ++){

		if(optionRollerSelect == objDevHeater_btnmDispConferenceTab[loop].optionSelect){

			stt_devDataPonitTypedef devDataPoint = {0};

			devDataPoint.devType_heater.devHeater_swEnumVal = objDevHeater_btnmDispConferenceTab[loop].opreatActCurrent;

			currentDev_dataPointSet(&devDataPoint, true, true, true, true, true);	
		}
	}

	return LV_RES_OK;
}

static lv_res_t funCb_btnmActionClick_devFans_gearBtnm(lv_obj_t *btnm, const char *txt){

	uint8_t loop = 0;

	for(loop = 0; loop < DEVICE_FANS_OPREAT_ACTION_NUM + 1; loop ++){

		if(!strcmp(txt, btnm_str_devFans[loop])){

			printf("btn %s has be selected.\n", txt);
			
			break;
		}
	}

	if(loop <= DEVICE_FANS_OPREAT_ACTION_NUM){
		
		stt_devDataPonitTypedef devDataPoint = {0};

		devDataPoint.devType_fans.devFans_swEnumVal = objDevFans_btnmDispConferenceTab[loop].opreatActCurrent;
		currentDev_dataPointSet(&devDataPoint, true, true, true, true, true);
		switch(loop){

			case fansOpreatAct_firstGear:{
			
				lv_label_set_text(textInstract_devFans, "1");
				styleImage_devFans_icon->image.color = LV_COLOR_MAKE(0, 128, 255);
				styleImage_devFans_icon->image.intense = LV_OPA_COVER;
				lv_obj_refresh_style(icomImage_devFans);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_pos(icomImage_devFans, 55, 40)):
					(lv_obj_set_pos(icomImage_devFans, 15, 70));
				lv_obj_align(textInstract_devFans, icomImage_devFans, LV_ALIGN_OUT_RIGHT_BOTTOM, 40, 5);
			
			}break;
			
			case fansOpreatAct_secondGear:{
			
				lv_label_set_text(textInstract_devFans, "2");
				styleImage_devFans_icon->image.color = LV_COLOR_MAKE(255, 255, 0);
				styleImage_devFans_icon->image.intense = LV_OPA_COVER;
				lv_obj_refresh_style(icomImage_devFans);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_pos(icomImage_devFans, 55, 40)):
					(lv_obj_set_pos(icomImage_devFans, 15, 70));
				lv_obj_align(textInstract_devFans, icomImage_devFans, LV_ALIGN_OUT_RIGHT_BOTTOM, 40, 5);
			
			}break;
			
			case fansOpreatAct_thirdGear:{
			
				lv_label_set_text(textInstract_devFans, "3");
				styleImage_devFans_icon->image.color = LV_COLOR_MAKE(255, 128, 0);
				styleImage_devFans_icon->image.intense = LV_OPA_COVER;
				lv_obj_refresh_style(icomImage_devFans);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_pos(icomImage_devFans, 55, 40)):
					(lv_obj_set_pos(icomImage_devFans, 15, 70));
				lv_obj_align(textInstract_devFans, icomImage_devFans, LV_ALIGN_OUT_RIGHT_BOTTOM, 40, 5);
			
			}break;
			
			case fansOpreatAct_stop:
			default:{
			
				lv_label_set_text(textInstract_devFans, "\0");
				styleImage_devFans_icon->image.color = LV_COLOR_MAKE(224, 224, 220);
				styleImage_devFans_icon->image.intense = LV_OPA_COVER;
				lv_obj_refresh_style(icomImage_devFans);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_pos(icomImage_devFans, 108, 40)):
					(lv_obj_set_pos(icomImage_devFans, 68, 70));
				lv_obj_align(textInstract_devFans, icomImage_devFans, LV_ALIGN_OUT_RIGHT_BOTTOM, 40, 5);
				
			}break;
		}
	}

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devHeater_pageTimeSet_btnConfirm(lv_obj_t *btn){

	char dataDispTemp[16] = {0};
	uint32_t timeCountSet_temp = (lv_roller_get_selected(rollerH_timeSetPage_devHeater) * 3600UL) + 
								 (lv_roller_get_selected(rollerM_timeSetPage_devHeater) * 60UL) + 
								 (lv_roller_get_selected(rollerS_timeSetPage_devHeater) * 1UL);
	const stt_devDataPonitTypedef datapointTemp = {

		.devType_heater.devHeater_swEnumVal = heaterOpreatAct_closeAfterTimeCustom,
	};

	devDriverBussiness_heaterSwitch_closePeriodCustom_Set(timeCountSet_temp, true);
	devDriverBussiness_heaterSwitch_periphStatusReales(&datapointTemp);
	sprintf(dataDispTemp, "%02d:%02d:%02d", (int)lv_roller_get_selected(rollerH_timeSetPage_devHeater),
											(int)lv_roller_get_selected(rollerM_timeSetPage_devHeater),
											(int)lv_roller_get_selected(rollerS_timeSetPage_devHeater));
	lv_label_set_text(textTimeInstract_target_devHeater, dataDispTemp);

	if(page_timeSet_devHeater){

		lv_obj_del(page_timeSet_devHeater);
		page_timeSet_devHeater = NULL;
	}

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devHeater_pageTimeSet_btnCancel(lv_obj_t *btn){

	if(page_timeSet_devHeater){

		lv_obj_del(page_timeSet_devHeater);
		page_timeSet_devHeater = NULL;
	}

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devHeater_timeSet(lv_obj_t *btn){

	uint32_t timeCountCustom = devDriverBussiness_heaterSwitch_closePeriodCustom_Get();

	if(page_timeSet_devHeater == NULL)
		page_timeSet_devHeater = lv_page_create(lv_scr_act(), NULL);

	lv_obj_set_size(page_timeSet_devHeater, 200, 180);
	lv_obj_align(page_timeSet_devHeater, icomImageA_devHeater, LV_ALIGN_CENTER, 0, 50);
//	lv_obj_set_pos(page_timeSet_devHeater, 20, 60);					
	lv_page_set_style(page_timeSet_devHeater, LV_PAGE_STYLE_SB, stylePage_devHeater_timeSet);
	lv_page_set_style(page_timeSet_devHeater, LV_PAGE_STYLE_BG, stylePage_devHeater_timeSet);
	lv_page_set_sb_mode(page_timeSet_devHeater, LV_SB_MODE_HIDE); 	
	lv_page_set_scrl_fit(page_timeSet_devHeater, false, true); //key opration
	lv_page_set_scrl_layout(page_timeSet_devHeater, LV_LAYOUT_PRETTY);

	labelTitle_timeSetPage_devHeater = lv_label_create(page_timeSet_devHeater, NULL);
	lv_label_set_recolor(labelTitle_timeSetPage_devHeater, true);
	lv_label_set_text(labelTitle_timeSetPage_devHeater, "#393939 time set#");
	lv_obj_set_protect(labelTitle_timeSetPage_devHeater, LV_PROTECT_POS);
	lv_obj_align(labelTitle_timeSetPage_devHeater, page_timeSet_devHeater, LV_ALIGN_CENTER, 0, 0);
	
	rollerH_timeSetPage_devHeater = lv_roller_create(page_timeSet_devHeater, NULL);
	lv_roller_set_action(rollerH_timeSetPage_devHeater, NULL);
	lv_roller_set_options(rollerH_timeSetPage_devHeater, "00\n""01\n""02\n""03\n""04\n""05\n"
													     "06\n""07\n""08\n""09\n""10\n""11\n"
													     "12\n""13\n""14\n""15\n""16\n""17\n"
													     "18\n""19\n""20\n""21\n""22\n""23");
	lv_obj_set_protect(rollerH_timeSetPage_devHeater, LV_PROTECT_POS);
	lv_roller_set_hor_fit(rollerH_timeSetPage_devHeater, false);
	lv_obj_set_width(rollerH_timeSetPage_devHeater, 40);
	lv_obj_align(rollerH_timeSetPage_devHeater, labelTitle_timeSetPage_devHeater, LV_ALIGN_OUT_BOTTOM_MID, -70, 20);
	lv_page_glue_obj(rollerH_timeSetPage_devHeater, true);
	lv_roller_set_selected(rollerH_timeSetPage_devHeater, timeCountCustom % (3600UL * 24UL) / 3600UL, false);

	lv_roller_set_style(rollerH_timeSetPage_devHeater, LV_ROLLER_STYLE_BG, styleTextRoller_devHeater_timeSetPage_bg);
	lv_roller_set_style(rollerH_timeSetPage_devHeater, LV_ROLLER_STYLE_SEL, styleTextRoller_devHeater_timeSetPage_sel);
	lv_roller_set_visible_row_count(rollerH_timeSetPage_devHeater, 4);

	rollerM_timeSetPage_devHeater = lv_roller_create(page_timeSet_devHeater, NULL);
	lv_roller_set_action(rollerM_timeSetPage_devHeater, NULL);
	lv_roller_set_options(rollerM_timeSetPage_devHeater, "00\n""01\n""02\n""03\n""04\n""05\n"
														 "06\n""07\n""08\n""09\n""10\n""11\n"
														 "12\n""13\n""14\n""15\n""16\n""17\n"
														 "18\n""19\n""20\n""21\n""22\n""23\n"
														 "24\n""25\n""26\n""27\n""28\n""29\n"
														 "30\n""31\n""32\n""33\n""34\n""35\n"
														 "36\n""37\n""38\n""39\n""40\n""41\n"
														 "42\n""43\n""44\n""45\n""46\n""47\n"
														 "48\n""49\n""50\n""51\n""52\n""53\n"
														 "54\n""55\n""56\n""57\n""58\n""59");
	lv_obj_set_protect(rollerM_timeSetPage_devHeater, LV_PROTECT_POS);
	lv_roller_set_hor_fit(rollerM_timeSetPage_devHeater, false);
	lv_obj_set_width(rollerM_timeSetPage_devHeater, 40);
	lv_obj_align(rollerM_timeSetPage_devHeater, rollerH_timeSetPage_devHeater, LV_ALIGN_OUT_RIGHT_MID, 25, 0);
	lv_page_glue_obj(rollerM_timeSetPage_devHeater, true);
	lv_roller_set_selected(rollerM_timeSetPage_devHeater, timeCountCustom % 3600UL / 60, false);

	lv_roller_set_style(rollerM_timeSetPage_devHeater, LV_ROLLER_STYLE_BG, styleTextRoller_devHeater_timeSetPage_bg);
	lv_roller_set_style(rollerM_timeSetPage_devHeater, LV_ROLLER_STYLE_SEL, styleTextRoller_devHeater_timeSetPage_sel);
	lv_roller_set_visible_row_count(rollerM_timeSetPage_devHeater, 4);

	rollerS_timeSetPage_devHeater = lv_roller_create(page_timeSet_devHeater, NULL);
	lv_roller_set_action(rollerS_timeSetPage_devHeater, NULL);
	lv_roller_set_options(rollerS_timeSetPage_devHeater, "00\n""01\n""02\n""03\n""04\n""05\n"
														 "06\n""07\n""08\n""09\n""10\n""11\n"
														 "12\n""13\n""14\n""15\n""16\n""17\n"
														 "18\n""19\n""20\n""21\n""22\n""23\n"
														 "24\n""25\n""26\n""27\n""28\n""29\n"
														 "30\n""31\n""32\n""33\n""34\n""35\n"
														 "36\n""37\n""38\n""39\n""40\n""41\n"
														 "42\n""43\n""44\n""45\n""46\n""47\n"
														 "48\n""49\n""50\n""51\n""52\n""53\n"
														 "54\n""55\n""56\n""57\n""58\n""59");
	lv_obj_set_protect(rollerS_timeSetPage_devHeater, LV_PROTECT_POS);
	lv_roller_set_hor_fit(rollerS_timeSetPage_devHeater, false);
	lv_obj_set_width(rollerS_timeSetPage_devHeater, 40);
	lv_obj_align(rollerS_timeSetPage_devHeater, rollerM_timeSetPage_devHeater, LV_ALIGN_OUT_RIGHT_MID, 25, 0);
	lv_page_glue_obj(rollerS_timeSetPage_devHeater, true);
	lv_roller_set_selected(rollerS_timeSetPage_devHeater, timeCountCustom % 60UL, false);

	lv_roller_set_style(rollerS_timeSetPage_devHeater, LV_ROLLER_STYLE_BG, styleTextRoller_devHeater_timeSetPage_bg);
	lv_roller_set_style(rollerS_timeSetPage_devHeater, LV_ROLLER_STYLE_SEL, styleTextRoller_devHeater_timeSetPage_sel);
	lv_roller_set_visible_row_count(rollerS_timeSetPage_devHeater, 4);
	
	labelH_timeSetPage_devHeater = lv_label_create(page_timeSet_devHeater, NULL);
	lv_label_set_text(labelH_timeSetPage_devHeater, "hour");
	lv_obj_set_protect(labelH_timeSetPage_devHeater, LV_PROTECT_POS);
	lv_obj_align(labelH_timeSetPage_devHeater, rollerH_timeSetPage_devHeater, LV_ALIGN_IN_RIGHT_MID, 35, 3);
	lv_obj_set_style(labelH_timeSetPage_devHeater, styleTextRollerTips_devHeater_timeSetPage);

	labelM_timeSetPage_devHeater = lv_label_create(page_timeSet_devHeater, labelH_timeSetPage_devHeater);
	lv_label_set_text(labelM_timeSetPage_devHeater, "min");
	lv_obj_set_protect(labelM_timeSetPage_devHeater, LV_PROTECT_POS);
	lv_obj_align(labelM_timeSetPage_devHeater, rollerM_timeSetPage_devHeater, LV_ALIGN_IN_RIGHT_MID, 15, 0);
	lv_obj_set_style(labelM_timeSetPage_devHeater, styleTextRollerTips_devHeater_timeSetPage);

	labelS_timeSetPage_devHeater = lv_label_create(page_timeSet_devHeater, labelH_timeSetPage_devHeater);
	lv_label_set_text(labelS_timeSetPage_devHeater, "sec");
	lv_obj_set_protect(labelS_timeSetPage_devHeater, LV_PROTECT_POS);
	lv_obj_align(labelS_timeSetPage_devHeater, rollerS_timeSetPage_devHeater, LV_ALIGN_IN_RIGHT_MID, 15, 0);
	lv_obj_set_style(labelS_timeSetPage_devHeater, styleTextRollerTips_devHeater_timeSetPage);

	btnConfirm_timeSetPage_devHeater = lv_btn_create(page_timeSet_devHeater, NULL);
    lv_btn_set_style(btnConfirm_timeSetPage_devHeater, LV_BTN_STYLE_REL, stylebtnBk_transFull);
    lv_btn_set_style(btnConfirm_timeSetPage_devHeater, LV_BTN_STYLE_PR, stylebtnBk_transFull);
    lv_btn_set_style(btnConfirm_timeSetPage_devHeater, LV_BTN_STYLE_TGL_REL, stylebtnBk_transFull);
    lv_btn_set_style(btnConfirm_timeSetPage_devHeater, LV_BTN_STYLE_TGL_PR, stylebtnBk_transFull);
	lv_btn_set_fit(btnConfirm_timeSetPage_devHeater, false, false);
	lv_obj_set_size(btnConfirm_timeSetPage_devHeater, 70, 25);
	lv_page_glue_obj(btnConfirm_timeSetPage_devHeater, true);
	lv_obj_set_protect(btnConfirm_timeSetPage_devHeater, LV_PROTECT_FOLLOW);
	lv_obj_set_protect(btnConfirm_timeSetPage_devHeater, LV_PROTECT_POS);
	lv_obj_align(btnConfirm_timeSetPage_devHeater, page_timeSet_devHeater, LV_ALIGN_IN_BOTTOM_LEFT, 15, -1);
	lv_btn_set_action(btnConfirm_timeSetPage_devHeater, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devHeater_pageTimeSet_btnConfirm);
	btnCancel_timeSetPage_devHeater = lv_btn_create(page_timeSet_devHeater, btnConfirm_timeSetPage_devHeater);
	lv_obj_align(btnCancel_timeSetPage_devHeater, btnConfirm_timeSetPage_devHeater, LV_ALIGN_CENTER, 95, 0);
	lv_btn_set_action(btnCancel_timeSetPage_devHeater, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devHeater_pageTimeSet_btnCancel);

	labelConfirm_timeSetPage_devHeater = lv_label_create(page_timeSet_devHeater, NULL);
	lv_obj_set_style(labelConfirm_timeSetPage_devHeater, styleTextBtn_devHeater_timeSetPage);
	lv_obj_set_protect(labelConfirm_timeSetPage_devHeater, LV_PROTECT_POS);
	lv_obj_align(labelConfirm_timeSetPage_devHeater, btnConfirm_timeSetPage_devHeater, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(labelConfirm_timeSetPage_devHeater, "confirm");
	labelCancel_timeSetPage_devHeater = lv_label_create(page_timeSet_devHeater, labelConfirm_timeSetPage_devHeater);
	lv_obj_set_style(labelCancel_timeSetPage_devHeater, styleTextBtn_devHeater_timeSetPage);
	lv_obj_align(labelCancel_timeSetPage_devHeater, btnCancel_timeSetPage_devHeater, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(labelCancel_timeSetPage_devHeater, "cancel");

	usrApp_fullScreenRefresh_self(50, 0); 

	return LV_RES_OK;
}

void devHeater_bussinessDispRefresh_customGearSetTrig(void){

	char dataDispTemp[16] = {0};
	uint32_t closeCounterTemp = devDriverBussiness_heaterSwitch_closePeriodCustom_Get();
	
	sprintf(dataDispTemp, "%02d:%02d:%02d", (int)((closeCounterTemp % 86400UL) / 3600UL),
											(int)((closeCounterTemp % 3600UL) / 60UL),
											(int)(closeCounterTemp % 60UL));
	lv_label_set_text(textTimeInstract_target_devHeater, dataDispTemp);
	lv_label_set_text(textTimeInstract_current_devHeater, dataDispTemp);
}

static lv_res_t funCb_btnmActionClick_devHeater_gearBtnm(lv_obj_t *btnm, const char *txt){

	uint8_t loop = 0;
//	uint8_t toggledNum = lv_btnm_get_toggled(btnm);

//	printf("btnm toggled %d is trig.\n", toggledNum);

	if(devStatusDispMethod_landscapeIf_get()){

		for(loop = 0; loop < DEVICE_HEATER_OPREAT_ACTION_NUM + 1; loop ++){

			if(!strcmp(txt, btnm_str_devHeater_Y[loop])){

//				printf("btn %s has be selected.\n", txt);
				break;
			}
		}

	}else{

		for(loop = 0; loop < DEVICE_HEATER_OPREAT_ACTION_NUM + 2; loop ++){

			if(!strcmp(txt, btnm_str_devHeater_X[loop])){

//				printf("btn %s has be selected.\n", txt);
				break;
			}
		}		
		
		if((loop == 4) || (loop == 5))loop --;
	}

	if(loop <= DEVICE_HEATER_OPREAT_ACTION_NUM){

		stt_devDataPonitTypedef devDataPoint = {0};

		devDataPoint.devType_heater.devHeater_swEnumVal = objDevHeater_btnmDispConferenceTab[loop].opreatActCurrent;
		currentDev_dataPointSet(&devDataPoint, true, true, true, true, true);

		if(devDataPoint.devType_heater.devHeater_swEnumVal == heaterOpreatAct_closeAfterTimeCustom){

			if(imgBtn_devHeater_timeSet == NULL)
				imgBtn_devHeater_timeSet = lv_imgbtn_create(lv_obj_get_parent(btnm), NULL);
			lv_obj_set_protect(imgBtn_devHeater_timeSet, LV_PROTECT_POS);
//			lv_obj_align(imgBtn_devHeater_timeSet, btnm_bk_devHeater, LV_ALIGN_OUT_TOP_RIGHT, 60, 32);
			(devStatusDispMethod_landscapeIf_get())?
				(lv_obj_set_pos(imgBtn_devHeater_timeSet, 259, 130)):
				(lv_obj_set_pos(imgBtn_devHeater_timeSet, 182, 155));	
			lv_imgbtn_set_src(imgBtn_devHeater_timeSet, LV_BTN_STATE_REL, &iconSet_HomePageDeviceHeater);
			lv_imgbtn_set_src(imgBtn_devHeater_timeSet, LV_BTN_STATE_PR, &iconSet_HomePageDeviceHeater);
			lv_imgbtn_set_action(imgBtn_devHeater_timeSet, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devHeater_timeSet);
		}
		else
		{
			if(imgBtn_devHeater_timeSet){

				lv_obj_del(imgBtn_devHeater_timeSet);
				imgBtn_devHeater_timeSet = NULL;
			}
		}
		
		switch(devDataPoint.devType_heater.devHeater_swEnumVal){
			
			case heaterOpreatAct_open:{

				lv_label_set_text(textTimeInstract_target_devHeater, "\0");
				lv_label_set_text(textTimeInstract_current_devHeater, "OPEN");

			}break;
			
			case heaterOpreatAct_closeAfter30Min:{

				lv_label_set_text(textTimeInstract_target_devHeater, "00:30:00");
				lv_label_set_text(textTimeInstract_current_devHeater, "00:30:00");

			}break;
			
			case heaterOpreatAct_closeAfter60Min:{

				lv_label_set_text(textTimeInstract_target_devHeater, "00:60:00");
				lv_label_set_text(textTimeInstract_current_devHeater, "00:60:00");

			}break;
			
			case heaterOpreatAct_closeAfterTimeCustom:{

				devHeater_bussinessDispRefresh_customGearSetTrig();

			}break;
			
			case heaterOpreatAct_close:
			default:{

				lv_label_set_text(textTimeInstract_target_devHeater, "\0");
				lv_label_set_text(textTimeInstract_current_devHeater, "CLOSE");

			}break;
		}
	}

	return LV_RES_OK;
}

static lv_res_t funCb_slidAction_devDimmer_mainSlider(lv_obj_t *slider){

	stt_devDataPonitTypedef devDataPoint = {0};
	stt_msgDats_uiHpDevCtrlObjValChg msgQh_sptrDataDimmerCtrlObjChg = {0};

	usrApp_ctrlObjSlidlingTrig(); //滑动冷却触发

	devDataPoint.devType_dimmer.devDimmer_brightnessVal = (uint8_t)lv_slider_get_value(slider);
	currentDev_dataPointSet(&devDataPoint, true, mutualCtrlTrigIf_A, true, true, true);

	msgQh_sptrDataDimmerCtrlObjChg.devType = devTypeDef_dimmer;
	msgQh_sptrDataDimmerCtrlObjChg.msgData_ctrlObjParam.ctrlObjDataDev_Dimmer.objChg_brightness =\
		devDataPoint.devType_dimmer.devDimmer_brightnessVal;

//	if(lv_slider_get_value(slider)){

//		stylePhotoBk_devDimmer->image.opa = ((uint8_t)lv_slider_get_value(slider)) * 2 + 50;
//	}
//	else
//	{
//		stylePhotoBk_devDimmer->image.opa = 0;
//	}
//	lv_img_set_style(photoB_bk_devDimmer, stylePhotoBk_devDimmer);
//	stylePhotoAk_devDimmer->image.intense = 100 - ((uint8_t)lv_slider_get_value(slider));
//	lv_img_set_style(photoA_bk_devDimmer, stylePhotoAk_devDimmer);

//	sprintf(str_sliderBkVal_devDimmer, "%d%%", lv_slider_get_value(slider));
//	lv_label_set_text(label_bk_devDimmer, str_sliderBkVal_devDimmer);

	xQueueSendToBack(msgQh_uiHp_devCtrlObjValChg, &msgQh_sptrDataDimmerCtrlObjChg, 1 / portTICK_PERIOD_MS);

	return LV_RES_OK;
}

static lv_res_t funCb_slidAction_devCurtain_mainSlider(lv_obj_t *slider){

	uint8_t orbitalTimePercent = (uint8_t)lv_slider_get_value(slider);
	stt_devDataPonitTypedef devDataPoint = {0};

	usrApp_ctrlObjSlidlingTrig(); //滑动冷却触发

	devDataPoint.devType_curtain.devCurtain_actEnumVal = orbitalTimePercent;
	devDataPoint.devType_curtain.devCurtain_actMethod = 1; //滑条方式

	currentDev_dataPointSet(&devDataPoint, false, mutualCtrlTrigIf_A, true, true, true);
	sprintf(str_devParamPositionAdj_devCurtain, "%d%%", orbitalTimePercent);
	lv_label_set_text(label_bk_devCurtain_positionAdj, str_devParamPositionAdj_devCurtain);

	return LV_RES_OK;
}

static lv_res_t funCb_slidAction_devThermostat_mainSlider(lv_obj_t *slider){

	uint8_t tempTarget = (uint8_t)lv_slider_get_value(slider);
	char tempDisp[5] = {0};
	stt_devDataPonitTypedef devDataPoint = {0};

	currentDev_dataPointGet(&devDataPoint);

	usrApp_ctrlObjSlidlingTrig(); //滑动冷却触发

	devDataPoint.devType_thermostat.devThermostat_tempratureTarget = 16 + tempTarget;

	currentDev_dataPointSet(&devDataPoint, true, false, true, true, true);	

	lv_lmeter_set_value(lmeterTempInstTarget_devThermostat, tempTarget + 16);
	sprintf(tempDisp, "%02d", tempTarget + 16);
	lv_label_set_text(labelTempInstTarget_devThermostat, tempDisp);

	return LV_RES_OK;
}

static void usrApp_devThermostat_ctrlObj_reserveSet(bool reserve_IF){

	if(reserve_IF){

		lv_obj_set_click(slider_tempAdj_devThermostat, true);
		lv_obj_set_click(btn_tempAdjAdd_devThermostat, true);
		lv_obj_set_click(btn_tempAdjCut_devThermostat, true);
		lv_obj_set_click(cb_devEcoEnable_devThermostat, true);
		
		styleLmeter_devThermostat_tempTarget->line.color = LV_COLOR_WHITE;
		styleLmeter_devThermostat_tempTarget->body.main_color = LV_COLOR_MAKE(0, 0, 255);
		styleLmeter_devThermostat_tempTarget->body.grad_color = LV_COLOR_MAKE(0, 0, 255);
		styleLmeter_devThermostat_tempCurrent->line.color = LV_COLOR_WHITE;
		styleLmeter_devThermostat_tempCurrent->body.main_color = LV_COLOR_MAKE(0, 255, 0);
		styleLmeter_devThermostat_tempCurrent->body.grad_color = LV_COLOR_MAKE(0, 255, 0);
		styleLabel_devThermostat_tempTarget->text.color = LV_COLOR_MAKE(255, 255, 0);
		styleLabel_devThermostat_tempCurrent->text.color = LV_COLOR_MAKE(0, 255, 0);
		styleSliderBk_devThermostat_indic->body.grad_color = LV_COLOR_GREEN;
		styleSliderBk_devThermostat_indic->body.main_color = LV_COLOR_LIME;
		styleSliderBk_devThermostat_indic->body.shadow.color = LV_COLOR_LIME;
		styleCb_devThermostat_EcoEnText->text.opa = LV_OPA_100;
		styleCb_devThermostat_EcoEnBoxRel->body.opa = LV_OPA_100;
		styleCb_devThermostat_EcoEnBoxTgRel->body.opa = LV_OPA_100;
		styleTextBtnBk_devThermostat_tempAdj->text.color = LV_COLOR_MAKE(255, 255, 255);
	}
	else
	{
		lv_obj_set_click(slider_tempAdj_devThermostat, false);
		lv_obj_set_click(btn_tempAdjAdd_devThermostat, false);
		lv_obj_set_click(btn_tempAdjCut_devThermostat, false);
		lv_obj_set_click(cb_devEcoEnable_devThermostat, false);

		styleLmeter_devThermostat_tempTarget->line.color = LV_COLOR_GRAY;
		styleLmeter_devThermostat_tempTarget->body.main_color = LV_COLOR_GRAY;
		styleLmeter_devThermostat_tempTarget->body.grad_color = LV_COLOR_GRAY;
		styleLmeter_devThermostat_tempCurrent->line.color = LV_COLOR_GRAY;
		styleLmeter_devThermostat_tempCurrent->body.main_color = LV_COLOR_GRAY;
		styleLmeter_devThermostat_tempCurrent->body.grad_color = LV_COLOR_GRAY;
		styleLabel_devThermostat_tempTarget->text.color = LV_COLOR_GRAY;
		styleLabel_devThermostat_tempCurrent->text.color = LV_COLOR_GRAY;
		styleSliderBk_devThermostat_indic->body.grad_color = LV_COLOR_GRAY;
		styleSliderBk_devThermostat_indic->body.main_color = LV_COLOR_GRAY;
		styleSliderBk_devThermostat_indic->body.shadow.color = LV_COLOR_GRAY;
		styleCb_devThermostat_EcoEnText->text.opa = LV_OPA_0;
		styleCb_devThermostat_EcoEnBoxRel->body.opa = LV_OPA_0;
		styleCb_devThermostat_EcoEnBoxTgRel->body.opa = LV_OPA_0;
		styleTextBtnBk_devThermostat_tempAdj->text.color = LV_COLOR_GRAY;
	}

	lv_obj_refresh_style(lmeterTempInstTarget_devThermostat);
	lv_obj_refresh_style(lmeterTempInstCurrent_devThermostat);
	lv_obj_refresh_style(labelTempInstTarget_devThermostat);
	lv_obj_refresh_style(labelTempInstCurrent_devThermostat);
	lv_obj_refresh_style(slider_tempAdj_devThermostat);
	lv_obj_refresh_style(cb_devEcoEnable_devThermostat);
	lv_obj_refresh_style(textBtn_tempAdjAdd_devThermostat);
	lv_obj_refresh_style(textBtn_tempAdjCut_devThermostat);
}

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)
static lv_res_t funCb_btnActionClick_devThermostat_rollerChg(lv_obj_t *btn){

	uint8_t loop = 0;
	stt_thermostat_actAttr devParam_thermostat = {0};
	stt_devDataPonitTypedef devDataPoint = {0};

	currentDev_dataPointGet(&devDataPoint);
	devDriverBussiness_thermostatSwitch_devParam_get(&devParam_thermostat);	
	devParam_thermostat.deviceRunning_MODE ++;
	devDriverBussiness_thermostatSwitch_devParam_runMode_set(devParam_thermostat.deviceRunning_MODE);
	for(loop = 0; loop < DEVICE_THERMOSTAT_OPREAT_ACTION_NUM; loop ++){

		if(devParam_thermostat.deviceRunning_MODE == objDevThermostat_rollerDispModeSel[loop].opreatActCurrent){

			lv_roller_set_selected(roller_devRunningMode_devThermostat, objDevThermostat_rollerDispModeSel[loop].optionSelect, false);
		
			break;
		}
	}

	if(devParam_thermostat.deviceRunning_MODE == thermostatRunningStatus_mode_stop){

		usrApp_devThermostat_ctrlObj_reserveSet(false);
		devDataPoint.devType_thermostat.devThermostat_running_en = 0;
	}
	else{

		usrApp_devThermostat_ctrlObj_reserveSet(true);
		devDataPoint.devType_thermostat.devThermostat_running_en = 1;
	}

	currentDev_dataPointSet(&devDataPoint, true, true, true, true, true);
	
	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devThermostat_ventFanOpraet(lv_obj_t *btn){

	(LV_BTN_STATE_TGL_REL == lv_imgbtn_get_state(btn))?
		(devDriverBussiness_thermostatSwitch_devParam_exFansState_set(true)):
		(devDriverBussiness_thermostatSwitch_devParam_exFansState_set(false));

	return LV_RES_OK;
}

static lv_res_t funCb_rollerAction_devThermostat_gearSel(lv_obj_t *rol){

	uint8_t loop = 0;
	stt_thermostat_actAttr devParam_thermostat = {0};
	uint16_t valSet = lv_roller_get_selected(rol);
	stt_devDataPonitTypedef devDataPoint = {0};

	currentDev_dataPointGet(&devDataPoint);
	devDriverBussiness_thermostatSwitch_devParam_get(&devParam_thermostat);	

	for(loop = 0; loop < DEVICE_THERMOSTAT_OPREAT_ACTION_NUM; loop ++){

		if(valSet == objDevThermostat_rollerDispModeSel[loop].optionSelect){

			devParam_thermostat.deviceRunning_MODE = objDevThermostat_rollerDispModeSel[loop].opreatActCurrent;
			devDriverBussiness_thermostatSwitch_devParam_runMode_set(devParam_thermostat.deviceRunning_MODE);
		
			break;
		}
	}

	if(devParam_thermostat.deviceRunning_MODE == thermostatRunningStatus_mode_stop){

		usrApp_devThermostat_ctrlObj_reserveSet(false);
		devDataPoint.devType_thermostat.devThermostat_running_en = 0;
	}
	else{

		usrApp_devThermostat_ctrlObj_reserveSet(true);
		devDataPoint.devType_thermostat.devThermostat_running_en = 1;
	}

	currentDev_dataPointSet(&devDataPoint, true, true, true, true, true);

	return LV_RES_OK;
}
#else
static lv_res_t funCb_swAction_devThermostat_runningEnable(lv_obj_t *sw){

	stt_devDataPonitTypedef devDataPoint = {0};

	currentDev_dataPointGet(&devDataPoint);

	if(lv_sw_get_state(sw)){

		usrApp_devThermostat_ctrlObj_reserveSet(true);
		devDataPoint.devType_thermostat.devThermostat_running_en = 1;
	}
	else
	{
		usrApp_devThermostat_ctrlObj_reserveSet(false);
		devDataPoint.devType_thermostat.devThermostat_running_en = 0;
	}

	currentDev_dataPointSet(&devDataPoint, true, false, true, true, true);

	return LV_RES_OK;
}
#endif

static lv_res_t funCb_cbChecked_devThermostat_EcoEnable(lv_obj_t *cb){

	stt_devDataPonitTypedef devDataPoint = {0};

	currentDev_dataPointGet(&devDataPoint);

	(lv_cb_is_checked(cb))?
		(devDataPoint.devType_thermostat.devThermostat_nightMode_en = 1):
		(devDataPoint.devType_thermostat.devThermostat_nightMode_en = 0);

	currentDev_dataPointSet(&devDataPoint, true, false, true, true, true);

	return LV_RES_OK;
}

static lv_res_t funCb_slidAction_devCurtainPosTips_mainSlider(lv_obj_t *slider){

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_homeMenu_click(lv_obj_t *btn){

	uint16_t devRunningFlg_temp = currentDevRunningFlg_paramGet();
	bool menuGenerate_en = false;

#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SLAVE_MCU)
 #if(DRVMETHOD_BY_SLAVE_MCU_RELAY_TEST == 1)
	return LV_RES_OK;
 #endif
#endif

	if(devRunningFlg_temp & DEV_RUNNING_FLG_BIT_DEVLOCK){

		
	}
	else
	{
		switch(currentDev_typeGet()){

			case devTypeDef_scenario:{

				if(0 == devDriverBussiness_scnarioSwitch_driverClamDown_get()){

					menuGenerate_en = true;
				}			

			}break;

			default:{

				menuGenerate_en = true;

			}break;
		}		
	}

	if(menuGenerate_en)lvGui_usrSwitch(bussinessType_Menu, true);
	lvGuiLinkageConfig_devGraphCtrlBlock_listLoadFlg_set(false);

	if(menuGenerate_en){
		if(NULL != btn_homeMenu)lv_obj_del(btn_homeMenu);btn_homeMenu = NULL;
		if(NULL != btn_unlockMenu)lv_obj_del(btn_unlockMenu);btn_unlockMenu = NULL;
	}

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_homeUnlock_click(lv_obj_t *btn){

	uint16_t devRunningFlg_temp = currentDevRunningFlg_paramGet();

	if(devRunningFlg_temp & DEV_RUNNING_FLG_BIT_DEVLOCK){ //解锁

		devRunningFlg_temp &= ~DEV_RUNNING_FLG_BIT_DEVLOCK;
		currentDevRunningFlg_paramSet(devRunningFlg_temp, true);
	}

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_homeUnlock_longPre(lv_obj_t *btn){

	kPreReaptParam_unlock.counterDN = 2;

	if(kPreReaptParam_unlock.counterUP > 6){

		kPreReaptParam_unlock.counterUP = 0;
		
		devSystemInfoLocalRecord_allErase();		
		usrApplication_systemRestartTrig(6);

		printf("factory reset trig!\n");
	}

	return LV_RES_OK;
}

static void funCb_tabLoad_homeTabviewChg(lv_obj_t *tv, uint16_t id){

//	printf("homepage tv load %d.\n", id);

	if(0 == id){


	}
	else
	if(1 == id){

		
	}
}

static void pageHeader_infoRefreshLoop(void){

	const uint8_t localCounter_period = 5;
	static uint8_t localCounter = localCounter_period;

	if(localCounter){

		localCounter --;
		return;
	}
	else{

		localCounter = localCounter_period;

		if(lvGuiRefreshCounter_bussinessBk){

			lvGuiRefreshCounter_bussinessBk --;

			lv_obj_refresh_style(lv_layer_top());
			lv_obj_refresh_style(imageBK);			
		}
	}

	if(devRunningTimeFromPowerUp_couter < 2)return;

#if(SCREENSAVER_RUNNING_ENABLE == 1)

	if(screensaverDispAttrParam.flg_screensaverPageCreat == 1){

		return; //锁屏界面不刷新页眉信息		
	}
#endif

	{//页眉时间信息更新 --变化时更新

		stt_localTime devSystime_temp = {0};
		static uint8_t localRecord_min = 0;

		usrAppDevCurrentSystemTime_paramGet(&devSystime_temp);

		if(localRecord_min != devSystime_temp.time_Minute){

			localRecord_min = devSystime_temp.time_Minute;
			
			sprintf(textStr_time, "%02d:%02d", devSystime_temp.time_Hour, 
											   devSystime_temp.time_Minute);
			lv_label_set_static_text(textHeaderObj_time, (const char*)textStr_time);
//			lv_obj_refresh_style(textHeaderObj_time);
//			vTaskDelay(20 / portTICK_PERIOD_MS);
		}
	}

	{//页眉定时类图标提示更新 --变化时更新

			   uint16_t devRunningFlg_temp 		  	  = currentDevRunningFlg_paramGet();
			   
			   bool 	tipsAlarm_remindIf 		  	  = false;
			   bool 	tipsGreenMode_remindIf	  	  = false;
			   bool 	tipsNightMode_remindIf	  	  = false;
			   bool 	tipsDeviceLock_remindIf	  	  = false;
			   
		static bool 	tipsAlarm_remindIf_record 	  = false;
		static bool 	tipsGreenMode_remindIf_record = false;
		static bool 	tipsNightMode_remindIf_record = false;
		static bool 	tipsDeviceLock_remindIf_record= false;

		if((devRunningFlg_temp & DEV_RUNNING_FLG_BIT_TIMER)||
		   (devRunningFlg_temp & DEV_RUNNING_FLG_BIT_DELAY)){

			tipsAlarm_remindIf = true;
		}

		if(tipsAlarm_remindIf_record != tipsAlarm_remindIf){

			tipsAlarm_remindIf_record = tipsAlarm_remindIf;

			if(tipsAlarm_remindIf_record){

				if(iconHeaderObj_alarm == NULL)iconHeaderObj_alarm = lv_img_create(lv_layer_top(), NULL);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_pos(iconHeaderObj_alarm, 62, 1)):
					(lv_obj_set_pos(iconHeaderObj_alarm, 60, 1));
				lv_img_set_src(iconHeaderObj_alarm, &iconHeader_alarm);
				lv_obj_set_style(iconHeaderObj_alarm, styleImg_iconHeater);
			}
			else
			{
				lv_obj_del(iconHeaderObj_alarm);
				iconHeaderObj_alarm = NULL;
			}
		}

		if(devRunningFlg_temp & DEV_RUNNING_FLG_BIT_GREENMODE){

			tipsGreenMode_remindIf = true;
		}

		if(tipsGreenMode_remindIf_record != tipsGreenMode_remindIf){

			tipsGreenMode_remindIf_record = tipsGreenMode_remindIf;

			if(tipsGreenMode_remindIf_record){

				if(iconHeaderObj_greenMode == NULL)iconHeaderObj_greenMode = lv_img_create(lv_layer_top(), NULL);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_pos(iconHeaderObj_greenMode, 83, 1)):
					(lv_obj_set_pos(iconHeaderObj_greenMode, 133, 1));
				lv_img_set_src(iconHeaderObj_greenMode, &iconHeader_greenMode);
			}
			else
			{
				lv_obj_del(iconHeaderObj_greenMode);
				iconHeaderObj_greenMode = NULL;
			}
		}

		if(devRunningFlg_temp & DEV_RUNNING_FLG_BIT_NIGHTMODE){

			tipsNightMode_remindIf = true;
		}

		if(tipsNightMode_remindIf_record != tipsNightMode_remindIf){

			tipsNightMode_remindIf_record = tipsNightMode_remindIf;

			if(tipsNightMode_remindIf_record){

				if(iconHeaderObj_nightMode == NULL)iconHeaderObj_nightMode = lv_img_create(lv_layer_top(), NULL);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_pos(iconHeaderObj_nightMode, 104, 4)):
					(lv_obj_set_pos(iconHeaderObj_nightMode, 25, 23));
				lv_img_set_src(iconHeaderObj_nightMode, &iconHeader_nightMoon);
			}
			else
			{
				lv_obj_del(iconHeaderObj_nightMode);
				iconHeaderObj_nightMode = NULL;
			}
		}

		if(devRunningFlg_temp & DEV_RUNNING_FLG_BIT_DEVLOCK){

			tipsDeviceLock_remindIf = true;
		}

		if(tipsDeviceLock_remindIf_record != tipsDeviceLock_remindIf){

			tipsDeviceLock_remindIf_record = tipsDeviceLock_remindIf;

			if(tipsDeviceLock_remindIf_record){

				if(iconHeaderObj_devLock == NULL)iconHeaderObj_devLock = lv_img_create(lv_layer_top(), NULL);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_pos(iconHeaderObj_devLock,  3, 23)):
					(lv_obj_set_pos(iconHeaderObj_devLock, 49, 23));
				lv_img_set_src(iconHeaderObj_devLock, &iconHeader_lock);
			}
			else
			{
				lv_obj_del(iconHeaderObj_devLock);
				iconHeaderObj_devLock = NULL;
			}
		}
	}

	{//页眉信号强度提示更新 --持续更新

		int8_t signalStrength_temp = devMeshSignalFromParentVal_Get();
		static int8_t signalStrength_record = 0;
		const int8_t signalStrength_levelTab[3] = {-70, -90, -120};
		wifi_ap_record_t apInfo = {0};
		esp_err_t ret = ESP_OK;
		wifi_mode_t wMode_temp = WIFI_MODE_NULL;
		bool devMqttIsConnected = devSystemMqttIsConnected();
		
		ret = esp_wifi_sta_get_ap_info(&apInfo);
		esp_wifi_get_mode(&wMode_temp);

		if(ESP_ERR_WIFI_CONN != ret && WIFI_MODE_STA == wMode_temp){

			if(ESP_OK == ret){

				signalStrength_temp = apInfo.rssi;

				if(signalStrength_record != signalStrength_temp){
				
					signalStrength_record = signalStrength_temp;
				
					if(signalStrength_record > signalStrength_levelTab[0]){
				
						lv_img_set_src(iconHeaderObj_wifi, &iconHeader_wifi_A);
					}
					else
					if((signalStrength_record > signalStrength_levelTab[1]) &&
					   (signalStrength_record < signalStrength_levelTab[0])){
				
						lv_img_set_src(iconHeaderObj_wifi, &iconHeader_wifi_B);
					}
					else
					if((signalStrength_record > signalStrength_levelTab[2]) &&
					   (signalStrength_record < signalStrength_levelTab[1])){
					
						lv_img_set_src(iconHeaderObj_wifi, &iconHeader_wifi_C);
					}
					else
					if(signalStrength_record < signalStrength_levelTab[2]){
				
						lv_img_set_src(iconHeaderObj_wifi, &iconHeader_wifi_D);
					}	
				}
			}
			else
			{
				const lv_img_dsc_t *iconPtr_tab[4] = {

					&iconHeader_wifi_D,
					&iconHeader_wifi_C,
					&iconHeader_wifi_B,
					&iconHeader_wifi_A,
				};

				const uint8_t iconReferPeriod = 2;
				static uint8_t iconReferCounter = 0;
				static uint8_t iconIst = 0;

				if(iconReferCounter < iconReferPeriod)iconReferCounter ++;
				else{

					iconReferCounter = 0;

					if(iconIst == 4)iconIst  = 0;
					else{

						lv_img_set_src(iconHeaderObj_wifi, iconPtr_tab[iconIst]);

						iconIst ++;
					}
				}
			}

			(devMqttIsConnected)?
				(styleImg_iconWifi->image.color = scrHeaderSysInfo_baseColor):
				(styleImg_iconWifi->image.color = LV_COLOR_YELLOW);
			lv_obj_set_style(iconHeaderObj_wifi, styleImg_iconWifi);
		}
		else
		{
			lv_img_set_src(iconHeaderObj_wifi, &iconHeader_wifi_offline);

			styleImg_iconWifi->image.color = LV_COLOR_MAKE(237, 28, 36);
			lv_obj_set_style(iconHeaderObj_wifi, styleImg_iconWifi);
		}
	}

	// {//页眉节点数量显示更新 --变化时更新

	// 	uint16_t nodeNum_temp = devMeshNodeNum_Get();
	// 	static uint16_t nodeNum_record = 0;
	// 	uint8_t devMesh_statusRole_temp = roleMeshStatus_noConnect;
	// 	static uint8_t devMesh_statusRole_record = roleMeshStatus_noConnect;

	// 	if(nodeNum_record != nodeNum_temp){

	// 		nodeNum_record = nodeNum_temp;

	// 		sprintf(textStr_nodeNum, "%02d", nodeNum_record);
	// 		lv_label_set_static_text(textHeaderObj_meshNodeNum, (const char*)textStr_nodeNum);			
	// 	}

	// 	if(mwifi_is_connected()){

	// 		if(esp_mesh_get_layer() == MESH_ROOT){

	// 			devMesh_statusRole_temp = roleMeshStatus_root;
	// 		}
	// 		else
	// 		{
	// 			devMesh_statusRole_temp = roleMeshStatus_node;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		devMesh_statusRole_temp = roleMeshStatus_noConnect;
	// 	}
		
	// 	if(devMesh_statusRole_record != devMesh_statusRole_temp){

	// 		uint16_t devRunningFlg_temp = currentDevRunningFlg_paramGet();

	// 		devMesh_statusRole_record = devMesh_statusRole_temp;
			
	// 		switch(devMesh_statusRole_record){

	// 			case roleMeshStatus_root:{

	// 				lv_img_set_src(iconHeaderObj_node, &iconHeader_wifi_master);
	// 				styleIcon_meshNode->image.color = LV_COLOR_MAKE(226, 245, 250);
	// //					styleIcon_meshNode->image.color = LV_COLOR_WHITE;
	// 				styleIcon_meshNode->image.intense = LV_OPA_COVER;
	// 				lv_img_set_style(iconHeaderObj_node, styleIcon_meshNode);

	// 				devRunningFlg_temp |= DEV_RUNNING_FLG_BIT_DEVHOSTIF;			
					
	// 			}break;
				
	// 			case roleMeshStatus_node:{

	// 				lv_img_set_src(iconHeaderObj_node, &iconHeader_node);
	// 				styleIcon_meshNode->image.color = scrHeaderSysInfo_baseColor;
	// 				styleIcon_meshNode->image.intense = LV_OPA_COVER;
	// 				lv_img_set_style(iconHeaderObj_node, styleIcon_meshNode);

	// 				devRunningFlg_temp &= ~DEV_RUNNING_FLG_BIT_DEVHOSTIF;

	// 			}break;
				
	// 			case roleMeshStatus_noConnect:
	// 			default:{

	// 				lv_img_set_src(iconHeaderObj_node, &iconHeader_node);
	// 				styleIcon_meshNode->image.color = LV_COLOR_MAKE(237, 28, 36);
	// 				styleIcon_meshNode->image.intense = LV_OPA_COVER;
	// 				lv_img_set_style(iconHeaderObj_node, styleIcon_meshNode);

	// 				devRunningFlg_temp &= ~DEV_RUNNING_FLG_BIT_DEVHOSTIF;

	// 			}break;

	// //				case roleStatus_root:lv_label_set_text(textHeaderObj_meshRole, "m");break;
	// //				case roleStatus_node:lv_label_set_text(textHeaderObj_meshRole, "s");break;
	// //				case roleStatus_noConnect:
	// //				default:lv_label_set_text(textHeaderObj_meshRole, "x");break;
	// 		}

	// 		currentDevRunningFlg_paramSet(devRunningFlg_temp, true);

	// 		lv_obj_refresh_style(iconHeaderObj_node);
	// 	}
	// }

#if(LVAPP_DISP_ELECPARAM_HIDDEN_EN != 1) //电量参数显示条件编译
	{//页眉电参数显示更新

		stt_devStatusRecord devStatusRecordFlg_temp = {0};

		devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);	

		if(devStatusRecordFlg_temp.devElecsumDisp_IF){ //电量

			float elecSum_temp = devDriverBussiness_elecMeasure_valElecConsumGet();
	
			if(elecParamDisp_record.elecParam_elecsum != elecSum_temp){
	
				elecParamDisp_record.elecParam_elecsum = elecSum_temp;
	
				if(elecParamDisp_record.elecParam_elecsum > 0.0F && elecParamDisp_record.elecParam_elecsum < 1.0F){
	
					sprintf(textStr_elecSum, "%.04fkWh", elecParamDisp_record.elecParam_elecsum);
				}
				else
				if(elecParamDisp_record.elecParam_elecsum > 1.0F && elecParamDisp_record.elecParam_elecsum < 10.0F){
	
					sprintf(textStr_elecSum, "%.03fkWh", elecParamDisp_record.elecParam_elecsum);
				}
				else
				if(elecParamDisp_record.elecParam_elecsum > 10.0F && elecParamDisp_record.elecParam_elecsum < 100.0F){
	
					sprintf(textStr_elecSum, "%.02fkWh", elecParamDisp_record.elecParam_elecsum);
				}
				else
				if(elecParamDisp_record.elecParam_elecsum > 100.0F && elecParamDisp_record.elecParam_elecsum < 1000.0F){
	
					sprintf(textStr_elecSum, "%.02fkWh", elecParamDisp_record.elecParam_elecsum);
				}
				else{
	
					sprintf(textStr_elecSum, "%.01fkWh", elecParamDisp_record.elecParam_elecsum);
				}
	
				lv_label_set_static_text(textHeaderObj_elec, (const char*)textStr_elecSum); 
//				lv_obj_refresh_style(textHeaderObj_elec);
//				vTaskDelay(20 / portTICK_PERIOD_MS);
			}
		}
		else //功率
		{
			float elecPow_temp = devDriverBussiness_elecMeasure_valElecPowerGet();

			if(elecParamDisp_record.elecParam_power != elecPow_temp){

				elecParamDisp_record.elecParam_power = elecPow_temp;

				sprintf(textStr_elecSum, "%.01fW", elecParamDisp_record.elecParam_power);
				lv_label_set_static_text(textHeaderObj_elec, (const char*)textStr_elecSum); 
			}
		}
	}
#endif

	{
		static devTypeDef_enum devType_rcd = devTypeDef_null;

		if(devType_rcd != currentDev_typeGet()){

			devType_rcd = currentDev_typeGet();

			usrApp_temperatureCtrlObj_generate();
		}
	}
	if(usrApp_temperatureCtrlObj_reserveGet()){//页眉温度显示更新

		float temperature_temp = devDriverBussiness_temperatureMeasure_get();
		static float temperature_record = 0.0F;

		if(temperature_record != temperature_temp){

			temperature_record = temperature_temp;

			sprintf(textStr_temperature, "%02.01f", temperature_record);
			lv_label_set_static_text(textHeaderObj_temperature, textStr_temperature);	
//			lv_obj_refresh_style(textHeaderObj_temperature);
//			vTaskDelay(20 / portTICK_PERIOD_MS);

			lv_obj_align(iconHeaderObj_tempraB, textHeaderObj_temperature, LV_ALIGN_IN_RIGHT_MID, 14, 0);
		}
	}

#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SLAVE_MCU)
 #if(DRVMETHOD_BY_SLAVE_MCU_RELAY_TEST == 1)
 	{ //磁保持继电器测试数据更新

		char strParam_disp[32] = {0};

		sprintf(strParam_disp, "#F5EF9C time:%02d:%02d:%02d#", paramMagRelayTest.dataRcd.timeRecord / 3600,
															   paramMagRelayTest.dataRcd.timeRecord % 3600 / 60,
															   paramMagRelayTest.dataRcd.timeRecord % 60);
		lv_label_set_text(text_timeRecord, (const char *)strParam_disp);
		sprintf(strParam_disp, "#9AF8F8 loop:%d#.", paramMagRelayTest.dataRcd.relayActLoop);
		lv_label_set_text(text_loopCounter, (const char *)strParam_disp);
 	}
 #endif
#endif
}

static void pageActivity_infoRefreshLoop(void){
	
	const uint8_t localCounter_period = 0;
	static uint8_t localCounter = localCounter_period;

	if(localCounter){

		localCounter --;
		return;
	}
	else{

		localCounter = localCounter_period;
	}

	if(!pageRefreshTrig_counter){

		pageRefreshTrig_counter = COUNTER_DISENABLE_MASK_SPECIALVAL_U16;
		
		if(screenNull_refresher){

			lv_obj_del(screenNull_refresher);
			screenNull_refresher = NULL;
		}
	}

	switch(guiPage_record){

		case bussinessType_Home:{

			stt_msgDats_dataManagementHandle rptr_msgQ_dmHandle = {0};
			stt_msgDats_uiHpDevCtrlObjValChg rptr_msgQ_hpObjChgHandle = {0};
			uint8_t btnBindingStatus_temp[DEVICE_MUTUAL_CTRL_GROUP_NUM] = {0}; 

			//home界面控件值更变消息通知
			if(xQueueReceive(msgQh_uiHp_devCtrlObjValChg, &rptr_msgQ_hpObjChgHandle, 5 / portTICK_RATE_MS) == pdTRUE){

				switch(rptr_msgQ_hpObjChgHandle.devType){

					case devTypeDef_dimmer:{

						uint8_t brightnessTemp = rptr_msgQ_hpObjChgHandle.msgData_ctrlObjParam.ctrlObjDataDev_Dimmer.objChg_brightness;

						if(brightnessTemp){

							stylePhotoBk_devDimmer->image.opa = brightnessTemp * 2 + 50;
						}
						else
						{
							stylePhotoBk_devDimmer->image.opa = 0;
						}
						lv_img_set_style(photoB_bk_devDimmer, stylePhotoBk_devDimmer);
						stylePhotoAk_devDimmer->image.intense = 100 - brightnessTemp;
						lv_img_set_style(photoA_bk_devDimmer, stylePhotoAk_devDimmer);

						sprintf(str_sliderBkVal_devDimmer, "%d%%", brightnessTemp);
						lv_label_set_text(label_bk_devDimmer, str_sliderBkVal_devDimmer);						

					}break;

					default:break;
				}
			}

			//控件显示信息刷新业务
			if(xQueueReceive(msgQh_dataManagementHandle, &rptr_msgQ_dmHandle, 5 / portTICK_RATE_MS) == pdTRUE){

				switch(rptr_msgQ_dmHandle.msgType){

					case dataManagement_msgType_homePageCtrlObjTextChg:{ //文字

						stt_dataDisp_guiBussinessHome_btnText dataTextObjDisp_temp = {0};
						usrAppHomepageBtnTextDisp_paramGet(&dataTextObjDisp_temp);
					
						styleBtn_Text->text.font = usrAppHomepageBtnBkText_fontGet(dataTextObjDisp_temp.countryFlg);
						styleIconvText_devMulitSw_statusOn->text.font = styleBtn_Text->text.font;
						styleIconvText_devMulitSw_statusOff->text.font = styleBtn_Text->text.font;

						switch(currentDev_typeGet()){

							case devTypeDef_mulitSwOneBit:
							case devTypeDef_relayBox_1bit:{

								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjTextChg.objChg_bitHold & (1 << 0))
									lv_label_set_text(textBtn_meeting, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[0]);

								lv_obj_refresh_style(textBtn_meeting);

							}break;
							
							case devTypeDef_mulitSwTwoBit:{

								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjTextChg.objChg_bitHold & (1 << 0))
									lv_label_set_text(textBtn_meeting, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[0]);
								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjTextChg.objChg_bitHold & (1 << 1))
									lv_label_set_text(textBtn_sleeping, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[1]);

								lv_obj_refresh_style(textBtn_meeting);
								lv_obj_refresh_style(textBtn_sleeping);

							}break;

							case devTypeDef_thermostatExtension:{

								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjTextChg.objChg_bitHold & (1 << 0))
									lv_label_set_text(textBtn_meeting, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[1]);
								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjTextChg.objChg_bitHold & (1 << 1))
									lv_label_set_text(textBtn_sleeping, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[2]);
								
								lv_obj_refresh_style(textBtn_meeting);
								lv_obj_refresh_style(textBtn_sleeping);

							}break;
							
							case devTypeDef_mulitSwThreeBit:
							case devTypeDef_scenario:{
								
								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjTextChg.objChg_bitHold & (1 << 0))
									lv_label_set_text(textBtn_meeting, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[0]);
								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjTextChg.objChg_bitHold & (1 << 1))
									lv_label_set_text(textBtn_sleeping, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[1]);
								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjTextChg.objChg_bitHold & (1 << 2))
									lv_label_set_text(textBtn_toilet, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[2]);

								lv_obj_refresh_style(textBtn_meeting);
								lv_obj_refresh_style(textBtn_sleeping);
								lv_obj_refresh_style(textBtn_toilet);

							}break;

							default:break;
						}

						usrApp_fullScreenRefresh_self(400, 0); //刷新

					}break;
					
					case dataManagement_msgType_homePageCtrlObjIconChg:{ //图标

//						printf("icon change ,bithold:%02X.\n", rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjIconChg.objChg_bitHold);

						uint8_t dataIconObjDisp_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {0};
						usrAppHomepageBtnIconNumDisp_paramGet(dataIconObjDisp_temp);
					
						switch(currentDev_typeGet()){
						
							case devTypeDef_mulitSwOneBit:
							case devTypeDef_relayBox_1bit:{

								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjIconChg.objChg_bitHold & (1 << 0))
									lv_img_set_src(iconBtn_meeting, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[0]));
								
								lv_obj_refresh_style(iconBtn_meeting);
							}break;
							
							case devTypeDef_mulitSwTwoBit:{
								
								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjIconChg.objChg_bitHold & (1 << 0))
									lv_img_set_src(iconBtn_meeting, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[0]));
								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjIconChg.objChg_bitHold & (1 << 1))
									lv_img_set_src(iconBtn_sleeping, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[1]));

								lv_obj_refresh_style(iconBtn_meeting);
								lv_obj_refresh_style(iconBtn_sleeping);
							}break;
							
							case devTypeDef_thermostatExtension:{

								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjIconChg.objChg_bitHold & (1 << 0))
									lv_img_set_src(iconBtn_meeting, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[1]));
								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjIconChg.objChg_bitHold & (1 << 1))
									lv_img_set_src(iconBtn_sleeping, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[2]));
								
								lv_obj_refresh_style(iconBtn_meeting);
								lv_obj_refresh_style(iconBtn_sleeping);
							}break;
							
							case devTypeDef_mulitSwThreeBit:
							case devTypeDef_scenario:{
								
								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjIconChg.objChg_bitHold & (1 << 0))
									lv_img_set_src(iconBtn_meeting, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[0]));
								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjIconChg.objChg_bitHold & (1 << 1))
									lv_img_set_src(iconBtn_sleeping, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[1]));
								if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpCtrlObjIconChg.objChg_bitHold & (1 << 2))
									lv_img_set_src(iconBtn_toilet, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[2]));

								lv_obj_refresh_style(iconBtn_meeting);
								lv_obj_refresh_style(iconBtn_sleeping);
								lv_obj_refresh_style(iconBtn_toilet);
							}break;
						
							default:break;
						}

						usrApp_fullScreenRefresh_self(400, 0); //刷新
						
					}break;

					case dataManagement_msgType_homePagePicGroundChg:{

						if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpPicGroundChg.picGroundChg_notice){

							lvGui_homepageRefresh();
							usrAppBgroudObj_styleRefresh();
						}

					}break;

					case dataManagement_msgType_homePageThemeTypeChg:{ //home界面风格 

						if(rptr_msgQ_dmHandle.msgData_dmHandle.dataAb_hpThemeTypeChg.themeTypeChg_notice){

							stt_devDataPonitTypedef devDataPoint = {0};
							
							currentDev_dataPointGet(&devDataPoint);

							switch(currentDev_typeGet()){
							
								case devTypeDef_mulitSwOneBit:
								case devTypeDef_relayBox_1bit:{
							
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));

									lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));

									lv_obj_refresh_style(btn_bk_devMulitSw_A);
									lv_obj_refresh_style(iconBtn_meeting);
									lv_obj_refresh_style(textBtn_meeting);
									
								}break;
								
								case devTypeDef_mulitSwTwoBit:{
							
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));

									lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));
									lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));

									lv_obj_refresh_style(btn_bk_devMulitSw_A);
									lv_obj_refresh_style(iconBtn_meeting);
									lv_obj_refresh_style(textBtn_meeting);
									lv_obj_refresh_style(btn_bk_devMulitSw_B);
									lv_obj_refresh_style(iconBtn_sleeping);
									lv_obj_refresh_style(textBtn_sleeping);
	
								}break;

								case devTypeDef_thermostatExtension:{

									uint8_t devThermostatSwStatus = devDriverBussiness_thermostatSwitch_exSwitchParamGet();

									(devThermostatSwStatus & (1 << 0))?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
									(devThermostatSwStatus & (1 << 0))?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
									(devThermostatSwStatus & (1 << 0))?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
									(devThermostatSwStatus & (1 << 1))?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
									(devThermostatSwStatus & (1 << 1))?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
									(devThermostatSwStatus & (1 << 1))?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));

									lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));
									lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));
									
									lv_obj_refresh_style(btn_bk_devMulitSw_A);
									lv_obj_refresh_style(iconBtn_meeting);
									lv_obj_refresh_style(textBtn_meeting);
									lv_obj_refresh_style(btn_bk_devMulitSw_B);
									lv_obj_refresh_style(iconBtn_sleeping);
									lv_obj_refresh_style(textBtn_sleeping);

								}break;
								
								case devTypeDef_mulitSwThreeBit:
								case devTypeDef_scenario:{
									
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?(lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?(lv_img_set_style(iconBtn_toilet, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_toilet, styleIconvText_devMulitSw_statusOff));
									(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?(lv_label_set_style(textBtn_toilet, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_toilet, styleIconvText_devMulitSw_statusOff));

									lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));
									lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));
									lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));
									
									lv_obj_refresh_style(btn_bk_devMulitSw_A);
									lv_obj_refresh_style(iconBtn_meeting);
									lv_obj_refresh_style(textBtn_meeting);
									lv_obj_refresh_style(btn_bk_devMulitSw_B);
									lv_obj_refresh_style(iconBtn_sleeping);
									lv_obj_refresh_style(textBtn_sleeping);
									lv_obj_refresh_style(btn_bk_devMulitSw_C);
									lv_obj_refresh_style(iconBtn_toilet);
									lv_obj_refresh_style(textBtn_toilet);

								}break;
							
								default:break;
							}
						}

						usrAppBgroudObj_styleRefresh();
						
						lv_obj_refresh_style(imageBK);
						
					}break;

					default:break;
				}

				devScreenBkLight_weakUp(); //屏幕唤醒
			}

			//互控图标业务
			devMutualCtrlGroupInfo_groupInsertGet(btnBindingStatus_temp);
			if(memcmp(btnBindingStatus_record, btnBindingStatus_temp, sizeof(uint8_t) * DEVICE_MUTUAL_CTRL_GROUP_NUM)){

				const lv_coord_t mulitPosX_bindingIcon_oft = -45,
								 mulitPosY_bindingIcon_oft = -14;

				switch(currentDev_typeGet()){

					case devTypeDef_mulitSwOneBit:
					case devTypeDef_relayBox_1bit:{

						if(btnBindingStatus_record[0] != btnBindingStatus_temp[0]){

							if(btnBindingStatus_temp[0] == DEVICE_MUTUALGROUP_INVALID_INSERT_A ||
							   btnBindingStatus_temp[0] == DEVICE_MUTUALGROUP_INVALID_INSERT_B){

								if(iconBtn_binding_A != NULL){

								   lv_obj_del(iconBtn_binding_A);
								   iconBtn_binding_A = NULL;
								}
							}
							else
							{
								if(iconBtn_binding_A == NULL){

									iconBtn_binding_A = lv_imgbtn_create(btn_bk_devMulitSw_A, NULL);
									mutualCtrlTrigIf_A = true;
									lv_imgbtn_set_src(iconBtn_binding_A, LV_BTN_STATE_REL, usrAppHomepageBindingIcon_dataGet());
									lv_imgbtn_set_src(iconBtn_binding_A, LV_BTN_STATE_PR, &iconPage_unbinding);
									lv_btn_set_action(iconBtn_binding_A, LV_BTN_ACTION_CLICK, funCb_btnActionClick_bindingBtnA);
									lv_obj_set_protect(iconBtn_binding_A, LV_PROTECT_POS);
									lv_obj_align(iconBtn_binding_A, NULL, LV_ALIGN_OUT_RIGHT_MID, mulitPosX_bindingIcon_oft, mulitPosY_bindingIcon_oft);
								}

								lv_obj_set_click(iconBtn_binding_A, false);
								lv_imgbtn_set_style(iconBtn_binding_A, LV_BTN_STATE_REL, styleIconBinding_reserveIf);
							}
						}

					}break;

					case devTypeDef_mulitSwTwoBit:
					case devTypeDef_thermostatExtension:{

						if(btnBindingStatus_record[0] != btnBindingStatus_temp[0]){
						
							if(btnBindingStatus_temp[0] == DEVICE_MUTUALGROUP_INVALID_INSERT_A ||
							   btnBindingStatus_temp[0] == DEVICE_MUTUALGROUP_INVALID_INSERT_B){
						
								if(iconBtn_binding_A != NULL){
						
								   lv_obj_del(iconBtn_binding_A);
								   iconBtn_binding_A = NULL;
								}
							}
							else
							{
								if(iconBtn_binding_A == NULL){
						
									iconBtn_binding_A = lv_imgbtn_create(btn_bk_devMulitSw_A, NULL);
									mutualCtrlTrigIf_A = true;
									lv_imgbtn_set_src(iconBtn_binding_A, LV_BTN_STATE_REL, usrAppHomepageBindingIcon_dataGet());
									lv_imgbtn_set_src(iconBtn_binding_A, LV_BTN_STATE_PR, &iconPage_unbinding);
									lv_btn_set_action(iconBtn_binding_A, LV_BTN_ACTION_CLICK, funCb_btnActionClick_bindingBtnA);
									lv_obj_set_protect(iconBtn_binding_A, LV_PROTECT_POS);
									lv_obj_align(iconBtn_binding_A, NULL, LV_ALIGN_OUT_RIGHT_MID, mulitPosX_bindingIcon_oft, mulitPosY_bindingIcon_oft);
								}

								lv_obj_set_click(iconBtn_binding_A, false);
								lv_imgbtn_set_style(iconBtn_binding_A, LV_BTN_STATE_REL, styleIconBinding_reserveIf);
							}
						}
						
						if(btnBindingStatus_record[1] != btnBindingStatus_temp[1]){

							if(btnBindingStatus_temp[1] == DEVICE_MUTUALGROUP_INVALID_INSERT_A ||
							   btnBindingStatus_temp[1] == DEVICE_MUTUALGROUP_INVALID_INSERT_B){

								if(iconBtn_binding_B != NULL){

								   lv_obj_del(iconBtn_binding_B);
								   iconBtn_binding_B = NULL;
								}
							}
							else
							{
								if(iconBtn_binding_B == NULL){

									iconBtn_binding_B = lv_imgbtn_create(btn_bk_devMulitSw_B, NULL);
									mutualCtrlTrigIf_B = true;
									lv_imgbtn_set_src(iconBtn_binding_B, LV_BTN_STATE_REL, usrAppHomepageBindingIcon_dataGet());
									lv_imgbtn_set_src(iconBtn_binding_B, LV_BTN_STATE_PR, &iconPage_unbinding);
									lv_btn_set_action(iconBtn_binding_B, LV_BTN_ACTION_CLICK, funCb_btnActionClick_bindingBtnB);
									lv_obj_set_protect(iconBtn_binding_B, LV_PROTECT_POS);
									lv_obj_align(iconBtn_binding_B, NULL, LV_ALIGN_OUT_RIGHT_MID, mulitPosX_bindingIcon_oft, mulitPosY_bindingIcon_oft);
								}
								
								lv_obj_set_click(iconBtn_binding_B, false);
								lv_imgbtn_set_style(iconBtn_binding_B, LV_BTN_STATE_REL, styleIconBinding_reserveIf);
							}
						}
						
					}break;

					case devTypeDef_mulitSwThreeBit:{

						if(btnBindingStatus_record[0] != btnBindingStatus_temp[0]){
						
							if(btnBindingStatus_temp[0] == DEVICE_MUTUALGROUP_INVALID_INSERT_A ||
							   btnBindingStatus_temp[0] == DEVICE_MUTUALGROUP_INVALID_INSERT_B){
						
								if(iconBtn_binding_A != NULL){
						
								   lv_obj_del(iconBtn_binding_A);
								   iconBtn_binding_A = NULL;
								}
							}
							else
							{
								if(iconBtn_binding_A == NULL){
						
									iconBtn_binding_A = lv_imgbtn_create(btn_bk_devMulitSw_A, NULL);
									mutualCtrlTrigIf_A = true;
									lv_imgbtn_set_src(iconBtn_binding_A, LV_BTN_STATE_REL, usrAppHomepageBindingIcon_dataGet());
									lv_imgbtn_set_src(iconBtn_binding_A, LV_BTN_STATE_PR, &iconPage_unbinding);
									lv_btn_set_action(iconBtn_binding_A, LV_BTN_ACTION_CLICK, funCb_btnActionClick_bindingBtnA);
									lv_obj_set_protect(iconBtn_binding_A, LV_PROTECT_POS);
									lv_obj_align(iconBtn_binding_A, NULL, LV_ALIGN_OUT_RIGHT_MID, mulitPosX_bindingIcon_oft, mulitPosY_bindingIcon_oft);
								}

								lv_obj_set_click(iconBtn_binding_A, false);
								lv_imgbtn_set_style(iconBtn_binding_A, LV_BTN_STATE_REL, styleIconBinding_reserveIf);
							}
						}
						
						if(btnBindingStatus_record[1] != btnBindingStatus_temp[1]){
						
							if(btnBindingStatus_temp[1] == DEVICE_MUTUALGROUP_INVALID_INSERT_A ||
							   btnBindingStatus_temp[1] == DEVICE_MUTUALGROUP_INVALID_INSERT_B){
						
								if(iconBtn_binding_B != NULL){
						
								   lv_obj_del(iconBtn_binding_B);
								   iconBtn_binding_B = NULL;
								}
							}
							else
							{
								if(iconBtn_binding_B == NULL){
						
									iconBtn_binding_B = lv_imgbtn_create(btn_bk_devMulitSw_B, NULL);
									mutualCtrlTrigIf_B = true;
									lv_imgbtn_set_src(iconBtn_binding_B, LV_BTN_STATE_REL, usrAppHomepageBindingIcon_dataGet());
									lv_imgbtn_set_src(iconBtn_binding_B, LV_BTN_STATE_PR, &iconPage_unbinding);
									lv_btn_set_action(iconBtn_binding_B, LV_BTN_ACTION_CLICK, funCb_btnActionClick_bindingBtnB);
									lv_obj_set_protect(iconBtn_binding_B, LV_PROTECT_POS);
									lv_obj_align(iconBtn_binding_B, NULL, LV_ALIGN_OUT_RIGHT_MID, mulitPosX_bindingIcon_oft, mulitPosY_bindingIcon_oft);
								}

								lv_obj_set_click(iconBtn_binding_B, false);
								lv_imgbtn_set_style(iconBtn_binding_B, LV_BTN_STATE_REL, styleIconBinding_reserveIf);
							}
						}

						if(btnBindingStatus_record[2] != btnBindingStatus_temp[2]){
						
							if(btnBindingStatus_temp[2] == DEVICE_MUTUALGROUP_INVALID_INSERT_A ||
							   btnBindingStatus_temp[2] == DEVICE_MUTUALGROUP_INVALID_INSERT_B){
						
								if(iconBtn_binding_C != NULL){
						
								   lv_obj_del(iconBtn_binding_C);
								   iconBtn_binding_C = NULL;
								}
							}
							else
							{
								if(iconBtn_binding_C == NULL){
						
									iconBtn_binding_C = lv_imgbtn_create(btn_bk_devMulitSw_C, NULL);
									mutualCtrlTrigIf_C = true;
									lv_imgbtn_set_src(iconBtn_binding_C, LV_BTN_STATE_REL, usrAppHomepageBindingIcon_dataGet());
									lv_imgbtn_set_src(iconBtn_binding_C, LV_BTN_STATE_PR, &iconPage_unbinding);
									lv_btn_set_action(iconBtn_binding_C, LV_BTN_ACTION_CLICK, funCb_btnActionClick_bindingBtnC);
									lv_obj_set_protect(iconBtn_binding_C, LV_PROTECT_POS);
									lv_obj_align(iconBtn_binding_C, NULL, LV_ALIGN_OUT_RIGHT_MID, mulitPosX_bindingIcon_oft, mulitPosY_bindingIcon_oft);
								}

								lv_obj_set_click(iconBtn_binding_C, false);
								lv_imgbtn_set_style(iconBtn_binding_C, LV_BTN_STATE_REL, styleIconBinding_reserveIf);
							}
						}
						
					}break;

					case devTypeDef_curtain:{

						if(btnBindingStatus_record[0] != btnBindingStatus_temp[0]){

							if(btnBindingStatus_temp[0] == DEVICE_MUTUALGROUP_INVALID_INSERT_A ||
							   btnBindingStatus_temp[0] == DEVICE_MUTUALGROUP_INVALID_INSERT_B){

								if(iconBtn_binding_A != NULL){

								   lv_obj_del(iconBtn_binding_A);
								   iconBtn_binding_A = NULL;
								}
							}
							else
							{
								if(iconBtn_binding_A == NULL){

									iconBtn_binding_A = lv_imgbtn_create(imageBK, NULL);
									mutualCtrlTrigIf_A = true;
									lv_imgbtn_set_src(iconBtn_binding_A, LV_BTN_STATE_REL, usrAppHomepageBindingIcon_dataGet());
									lv_imgbtn_set_src(iconBtn_binding_A, LV_BTN_STATE_PR, &iconPage_unbinding);
									lv_btn_set_action(iconBtn_binding_A, LV_BTN_ACTION_CLICK, funCb_btnActionClick_bindingBtnA);
									lv_obj_set_protect(iconBtn_binding_A, LV_PROTECT_POS);
									lv_obj_align(iconBtn_binding_A, slider_bk_devCurtain, LV_ALIGN_OUT_TOP_LEFT, 0, -5);
								}
							}
						}

					}break;

					case devTypeDef_dimmer:{

						if(btnBindingStatus_record[0] != btnBindingStatus_temp[0]){

							if(btnBindingStatus_temp[0] == DEVICE_MUTUALGROUP_INVALID_INSERT_A ||
							   btnBindingStatus_temp[0] == DEVICE_MUTUALGROUP_INVALID_INSERT_B){

								if(iconBtn_binding_A != NULL){

								   lv_obj_del(iconBtn_binding_A);
								   iconBtn_binding_A = NULL;
								}
							}
							else
							{
								if(iconBtn_binding_A == NULL){

									iconBtn_binding_A = lv_imgbtn_create(imageBK, NULL);
									mutualCtrlTrigIf_A = true;
									lv_imgbtn_set_src(iconBtn_binding_A, LV_BTN_STATE_REL, usrAppHomepageBindingIcon_dataGet());
									lv_imgbtn_set_src(iconBtn_binding_A, LV_BTN_STATE_PR, &iconPage_unbinding);
									lv_btn_set_action(iconBtn_binding_A, LV_BTN_ACTION_CLICK, funCb_btnActionClick_bindingBtnA);
									lv_obj_set_protect(iconBtn_binding_A, LV_PROTECT_POS);
									lv_obj_align(iconBtn_binding_A, slider_bk_devDimmer, LV_ALIGN_OUT_TOP_RIGHT, 0, -5);
								}

								lv_obj_set_click(iconBtn_binding_A, false);
								lv_imgbtn_set_style(iconBtn_binding_A, LV_BTN_STATE_REL, styleIconBinding_reserveIf);
							}
						}

					}break;

					default:break;
				}

				memcpy(btnBindingStatus_record, btnBindingStatus_temp, sizeof(uint8_t) * DEVICE_MUTUAL_CTRL_GROUP_NUM); //比较值更新
			}

			//针对不同开关主界面动态内容逻辑业务
			switch(currentDev_typeGet()){

				case devTypeDef_scenario:{

					lv_obj_t *preloadParent = NULL;
					static bool scenarioDriver_preload_trigFlg = false;
					uint16_t scenarioDriverClamDown_counter = devDriverBussiness_scnarioSwitch_driverClamDown_get();
					stt_devDataPonitTypedef devDataPoint = {0};

					currentDev_dataPointGet(&devDataPoint);

					switch(devDataPoint.devType_scenario.devScenario_opNum){
					
						case 1:preloadParent = btn_bk_devMulitSw_A;break;
					
						case 2:preloadParent = btn_bk_devMulitSw_B;break;
					
						case 4:
						default:preloadParent = btn_bk_devMulitSw_C;break;
					}

					if(!scenarioDriver_preload_trigFlg){

						if(scenarioDriverClamDown_counter){

							scenarioDriver_preload_trigFlg = true;

							lv_obj_set_click(btn_bk_devMulitSw_A, false);
							lv_obj_set_click(btn_bk_devMulitSw_B, false);
							lv_obj_set_click(btn_bk_devMulitSw_C, false);

//							printf("watch point_B.\n");
						
							if(devDataPoint.devType_scenario.devScenario_opNum){

//								printf("watch point_C.\n");

								preload_driverCalmDown_devScenario = lv_preload_create(preloadParent, NULL);
								(devStatusDispMethod_landscapeIf_get())?
									(lv_obj_set_size(preload_driverCalmDown_devScenario, 50, 50)):
									(lv_obj_set_size(preload_driverCalmDown_devScenario, 65, 65));
								lv_obj_set_protect(preload_driverCalmDown_devScenario, LV_PROTECT_POS);
								lv_obj_align(preload_driverCalmDown_devScenario, preloadParent, LV_ALIGN_CENTER, 0, 0);
								lv_preload_set_spin_time(preload_driverCalmDown_devScenario, 750);
								lv_preload_set_style(preload_driverCalmDown_devScenario, LV_PRELOAD_STYLE_MAIN, stylePreload_devScenario_driverCalmDown);
							}
						}
					}
					else
					{
						static uint8_t calmDownWarnning_counter = 0; //冷却黄色警告
						static bool calmDownWarnning_flg = false;
						const uint8_t calmDownWarnning_period = 3;

						if(calmDownWarnning_counter < calmDownWarnning_period)calmDownWarnning_counter ++;
						else{

							calmDownWarnning_counter = 0;
							calmDownWarnning_flg = !calmDownWarnning_flg;

//							(calmDownWarnning_flg)?
//								(lv_imgbtn_set_style(preloadParent, LV_BTN_STATE_REL, styleBtn_devScenario_driverCalmDown)):
//								(lv_imgbtn_set_style(preloadParent, LV_BTN_STATE_REL, &lv_style_plain));
						}
					
						if(!scenarioDriverClamDown_counter){

							scenarioDriver_preload_trigFlg = false;

							lv_obj_set_click(btn_bk_devMulitSw_A, true);
							lv_obj_set_click(btn_bk_devMulitSw_B, true);
							lv_obj_set_click(btn_bk_devMulitSw_C, true);
						
							if(preload_driverCalmDown_devScenario){

								lv_obj_del(preload_driverCalmDown_devScenario);
								preload_driverCalmDown_devScenario = NULL;
							}

							lv_imgbtn_set_style(preloadParent, LV_BTN_STATE_REL, &lv_style_plain);
							
							lv_obj_refresh_style(btn_bk_devMulitSw_A);
							lv_obj_refresh_style(btn_bk_devMulitSw_B);
							lv_obj_refresh_style(btn_bk_devMulitSw_C);
						}
					}

				}break;

				case devTypeDef_curtain:{

					stt_msgDats_devCurtainDriver rptr_msgQ_devCurtainDriver = {0};

					if(xQueueReceive(msgQh_devCurtainDriver, &rptr_msgQ_devCurtainDriver, 0) == pdTRUE){

						switch(rptr_msgQ_devCurtainDriver.msgType){

							case msgType_devCurtainDriver_opreatStop:{ //窗帘驱动，回弹至停止消息接收后进行UI更新

								if(rptr_msgQ_devCurtainDriver.msgDats.data_opreatStop.opreatStop_sig){

									uint8_t devCurtain_orbitalPosPercent = devCurtain_currentPositionPercentGet();
//									lv_coord_t imageCurtainPosAdj_temp = devCurtain_orbitalPosPercent * 12;

//									imageCurtainPosAdj_temp /= 10;
//									lv_obj_set_pos(image_bk_devCurtain_bodyLeft, 2 - imageCurtainPosAdj_temp, 75);
//									lv_obj_set_pos(image_bk_devCurtain_bodyRight, 120 + imageCurtainPosAdj_temp, 75);

									//滑块及文字控件再刷新，防止信息同步不完整
									lv_slider_set_value(slider_bk_devCurtain, devCurtain_orbitalPosPercent);
									sprintf(str_devParamPositionCur_devCurtain, "%d", devCurtain_orbitalPosPercent);
									lv_label_set_text(label_bk_devCurtain_positionCur, str_devParamPositionCur_devCurtain);
									sprintf(str_devParamPositionAdj_devCurtain, "%d%%", devCurtain_orbitalPosPercent);
									lv_label_set_text(label_bk_devCurtain_positionAdj, str_devParamPositionAdj_devCurtain);

									lv_imgbtn_set_style(btn_bk_devCurtain_open, LV_BTN_STATE_REL, styleBtn_devCurtain_statusRel);
									lv_imgbtn_set_style(btn_bk_devCurtain_stop, LV_BTN_STATE_REL, styleBtn_devCurtain_statusPre);
									lv_imgbtn_set_style(btn_bk_devCurtain_close, LV_BTN_STATE_REL, styleBtn_devCurtain_statusRel);
								}

							}break;
							
							case msgType_devCurtainDriver_orbitalChgingByBtn:{

//								lv_coord_t imageCurtainPosAdj_temp = 12 * rptr_msgQ_devCurtainDriver.msgDats.data_orbitalChgingByBtn.orbitalPosPercent;
//								static lv_coord_t imageCurtainPosAdj_record = 0;

//								imageCurtainPosAdj_temp /= 10;
//								if(!(imageCurtainPosAdj_temp % 9)){

//									lv_obj_set_pos(image_bk_devCurtain_bodyLeft, 2 - imageCurtainPosAdj_temp, 75);
//									lv_obj_set_pos(image_bk_devCurtain_bodyRight, 120 + imageCurtainPosAdj_temp, 75);
//								}

								lv_slider_set_value(slider_bk_devCurtain, rptr_msgQ_devCurtainDriver.msgDats.data_orbitalChgingByBtn.orbitalPosPercent);
								sprintf(str_devParamPositionAdj_devCurtain, "%d%%", rptr_msgQ_devCurtainDriver.msgDats.data_orbitalChgingByBtn.orbitalPosPercent);
								lv_label_set_text(label_bk_devCurtain_positionAdj, str_devParamPositionAdj_devCurtain);
								sprintf(str_devParamPositionCur_devCurtain, "%d", rptr_msgQ_devCurtainDriver.msgDats.data_orbitalChgingByBtn.orbitalPosPercent);
								lv_label_set_text(label_bk_devCurtain_positionCur, str_devParamPositionCur_devCurtain);

							}break;

							case msgType_devCurtainDriver_orbitalChgingBySlider:{
								
								sprintf(str_devParamPositionCur_devCurtain, "%d", rptr_msgQ_devCurtainDriver.msgDats.data_orbitalChgingBySlider.orbitalPosPercent);
								lv_label_set_text(label_bk_devCurtain_positionCur, str_devParamPositionCur_devCurtain);

							}break;

							lv_obj_refresh_style(label_bk_devCurtain_positionCur);

							default:break;
						}
					}
				
				}break;

				case devTypeDef_heater:{

					stt_msgDats_devHeaterDriver rptr_msgQ_devHeaterDriver = {0};
					stt_devDataPonitTypedef devDataPoint = {0};
	 
					if(xQueueReceive(msgQh_devHeaterDriver, &rptr_msgQ_devHeaterDriver, 10 / portTICK_RATE_MS) == pdTRUE){

						switch(rptr_msgQ_devHeaterDriver.msgType){

							case msgType_devHeaterDriver_devActChg:{

								switch(rptr_msgQ_devHeaterDriver.msgDats.data_opreatChg.act){

									case heaterOpreatAct_close:{

										sprintf(str_devRunningRemind_devHeater, "CLOSE");
										devDataPoint.devType_heater.devHeater_swEnumVal = heaterOpreatAct_close;
										currentDev_dataPointSet(&devDataPoint, true, true, true, true, true);
										lv_label_set_text(textTimeInstract_target_devHeater, "\0");
										lv_btnm_set_toggle(btnm_bk_devHeater, true, 0);

										if(imgBtn_devHeater_timeSet && (guiPage_record == bussinessType_Home)){ //补充操作
										
											lv_obj_del(imgBtn_devHeater_timeSet);
											imgBtn_devHeater_timeSet = NULL;
										}

									}break;
									
									case heaterOpreatAct_open:{

										sprintf(str_devRunningRemind_devHeater, "OPEN");
										devDataPoint.devType_heater.devHeater_swEnumVal = heaterOpreatAct_open;
										currentDev_dataPointSet(&devDataPoint, true, true, true, true, true);
										lv_label_set_text(textTimeInstract_target_devHeater, "\0");

									}break;

									default:break;
								}
								
							}break;
							
							case msgType_devHeaterDriver_closeCounterChg:{

								sprintf(str_devRunningRemind_devHeater, "%02d:%02d:%02d",
																		(int)((rptr_msgQ_devHeaterDriver.msgDats.data_counterChg.counter % 86400UL) / 3600UL),
																		(int)((rptr_msgQ_devHeaterDriver.msgDats.data_counterChg.counter % 3600UL) / 60UL),
																		(int)(rptr_msgQ_devHeaterDriver.msgDats.data_counterChg.counter % 60UL));

							}break;

							default:break;
						}

						lv_label_set_text(textTimeInstract_current_devHeater, str_devRunningRemind_devHeater);
					}
				
				}break;

				case devTypeDef_thermostat:{

					stt_thermostat_actAttr devParam_thermostat = {0};
					static uint8_t tempCurrent_record = 0;

					devDriverBussiness_thermostatSwitch_devParam_get(&devParam_thermostat);
				
					if(tempCurrent_record != (uint8_t)(devParam_thermostat.temperatureVal_current)){

						char tempCurrentDisp[5] = {0};

						tempCurrent_record = (uint8_t)(devParam_thermostat.temperatureVal_current);
						lv_lmeter_set_value(lmeterTempInstCurrent_devThermostat, tempCurrent_record);
						sprintf(tempCurrentDisp, "%02d", tempCurrent_record);
						lv_label_set_text(labelTempInstCurrent_devThermostat, tempCurrentDisp);
					}

				}break;

				case devTypeDef_thermostatExtension:{

					static stt_thermostat_actAttr devParamTemp_thermostat = {0};
						   stt_thermostat_actAttr devParamRecord_thermostat = {0};

					static uint8_t tempCurrent_record = 0;

					devDriverBussiness_thermostatSwitch_devParam_get(&devParamTemp_thermostat);

					if(false == dispFlg_devThermostat_firstPage){

						if(tempCurrent_record != (uint8_t)(devParamTemp_thermostat.temperatureVal_current)){

							char tempCurrentDisp[5] = {0};

							tempCurrent_record = (uint8_t)(devParamTemp_thermostat.temperatureVal_current);
							lv_lmeter_set_value(lmeterTempInstCurrent_devThermostat, tempCurrent_record);
							sprintf(tempCurrentDisp, "%02d", tempCurrent_record);
							lv_label_set_text(labelTempInstCurrent_devThermostat, tempCurrentDisp);
						}
					}
					else
					{
						if(memcmp(&devParamRecord_thermostat, &devParamTemp_thermostat, sizeof(stt_thermostat_actAttr))){
						
							if((NULL != textBtn_thermostatExFold_temp) &&
							   (NULL != textBtn_thermostatExFold_eco)){
						
								memcpy(&devParamRecord_thermostat, &devParamTemp_thermostat, sizeof(stt_thermostat_actAttr));
						
								sprintf(textDispTemp_thermostatExBtnFold_temp, "#FFFF00 %dC# #00FF00 %dC#", devParamRecord_thermostat.temperatureVal_target,
																											devParamRecord_thermostat.temperatureVal_current);
								lv_label_set_text(textBtn_thermostatExFold_temp, (const char *)textDispTemp_thermostatExBtnFold_temp);
								
								(devParamRecord_thermostat.workModeInNight_IF)?
								   (lv_label_set_text(textBtn_thermostatExFold_eco, "#80FFFF ECO #")):
								   (lv_label_set_text(textBtn_thermostatExFold_eco, "#D2D2D2 ECO #"));
							}
						}
					}

				}break;

				default:break;	
			}

		}break;

		case bussinessType_menuPageWifiConfig:{

//			bool networkGetConnectFlg_record = meshNetwork_connectReserve_IF_get();

			lvGui_wifiConfig_bussiness_configComplete_tipsDetect(); //常规探测

//			if(!networkGetConnectFlg_record) //针对首次配网情况下
//				if(flgGet_gotRouterOrMeshConnect()){

//					lvGui_wifiConfig_bussiness_configComplete_tipsTrig();
//				}
					
		}break;

		case bussinessType_menuPageOther:{

			const uint8_t loopPeriod = 30;
			static uint8_t loopCounter = 0;	

			if(loopCounter < loopPeriod)loopCounter ++;
			else{

				loopCounter = 0;

				lvGuiOther_devInfoRefresh();
			}

		}break;

		case bussinessType_menuPageLinkageConfig:{

			

		}break;

		default:break;
	}
}

void pageHome_buttonMain_imageRefresh(bool freshNoRecord){ //界面切换时调用一次，数据点设置时调用一次

	if(devRunningTimeFromPowerUp_couter < 6)return; //避免控件未创建就被使用

	if(guiPage_current == bussinessType_Home){

		stt_devDataPonitTypedef devDataPoint = {0};
		static stt_devDataPonitTypedef devDataPoint_record = {0};
		bool imageRefresh_IF = false;
		uint8_t devExParamDiffrent_flg = 0;

		if(freshNoRecord){ //强制触发差异

			imageRefresh_IF = true;
		}

		currentDev_dataPointGet(&devDataPoint); //数据点对比触发差异
		if(memcmp(&devDataPoint_record, &devDataPoint, sizeof(stt_devDataPonitTypedef))){ //开关值变化时才刷新，避免重复刷新，影响界面显示效能

			imageRefresh_IF = true;
		}

		switch(currentDev_typeGet()){ //特殊对比触发差异 -1

			case devTypeDef_thermostatExtension:{

				uint8_t	devThermostatExSwStatus_temp = devDriverBussiness_thermostatSwitch_exSwitchParamGet();
				static uint8_t devThermostatExSwStatus_record = 0;

				if(devThermostatExSwStatus_record != devThermostatExSwStatus_temp){

					devExParamDiffrent_flg = devThermostatExSwStatus_record ^ devThermostatExSwStatus_temp;

					devThermostatExSwStatus_record = devThermostatExSwStatus_temp;

					imageRefresh_IF = true;
				}

			}break;

			default:{}break;
		}

		switch(currentDev_typeGet()){ //特殊对比触发差异 -2
		
			case devTypeDef_thermostat:
			case devTypeDef_thermostatExtension:{

				stt_thermostat_actAttr devParam_temp = {0};
				static stt_thermostat_actAttr devParam_record = {0};

				devDriverBussiness_thermostatSwitch_devParam_get(&devParam_temp);
				if(memcmp(&devParam_record, &devParam_temp, sizeof(stt_thermostat_actAttr))){

					memcpy(&devParam_record, &devParam_temp, sizeof(stt_thermostat_actAttr));

					imageRefresh_IF = true;
				}

			}break;

			default:break;
		}

		if(imageRefresh_IF){

			switch(currentDev_typeGet()){
	
				case devTypeDef_mulitSwOneBit:
				case devTypeDef_relayBox_1bit:{

					if(devDataPoint_record.devType_mulitSwitch_oneBit.swVal_bit1 != devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1){
						
						(devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
						(devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
						(devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
//						(devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1)?
//							(lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOn)):
//							(lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOff));

						lv_obj_refresh_style(btn_bk_devMulitSw_A); //必须刷新一下，否则非手动操作时图片刷不动
					}
					
				}break;
	
				case devTypeDef_mulitSwTwoBit:{

#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SLAVE_MCU)
 #if(DRVMETHOD_BY_SLAVE_MCU_RELAY_TEST == 1)
					return;					
 #endif
#endif
					if(devDataPoint_record.devType_mulitSwitch_twoBit.swVal_bit1 != devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1){

						(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
						(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
						(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
//						(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?
//							(lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOn)):
//							(lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOff));			

						lv_obj_refresh_style(btn_bk_devMulitSw_A); //必须刷新一下，否则非手动操作时图片刷不动
					}

					if(devDataPoint_record.devType_mulitSwitch_twoBit.swVal_bit2 != devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2){

						(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
						(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
						(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));
//						(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?
//							(lv_imgbtn_set_style(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOn)):
//							(lv_imgbtn_set_style(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOff));

						lv_obj_refresh_style(btn_bk_devMulitSw_B); //必须刷新一下，否则非手动操作时图片刷不动
					}
		
				}break;
	
				case devTypeDef_mulitSwThreeBit:{

//					printf("sw statusData refresh:bit1:%d.\n", devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1);

					if(devDataPoint_record.devType_mulitSwitch_threeBit.swVal_bit1 != devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1){

						(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
						(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
						(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
//						(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?
//							(lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOn)):
//							(lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOff));

						lv_obj_refresh_style(btn_bk_devMulitSw_A); //必须刷新一下，否则非手动操作时图片刷不动
					}

					if(devDataPoint_record.devType_mulitSwitch_threeBit.swVal_bit2 != devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2){

						(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
						(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
						(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));
//						(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?
//							(lv_imgbtn_set_style(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOn)):
//							(lv_imgbtn_set_style(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOff));

						lv_obj_refresh_style(btn_bk_devMulitSw_B); //必须刷新一下，否则非手动操作时图片刷不动
					}

					if(devDataPoint_record.devType_mulitSwitch_threeBit.swVal_bit3 != devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3){

						(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?(lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
						(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?(lv_img_set_style(iconBtn_toilet, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_toilet, styleIconvText_devMulitSw_statusOff));
						(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?(lv_label_set_style(textBtn_toilet, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_toilet, styleIconvText_devMulitSw_statusOff));
//						(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?
//							(lv_imgbtn_set_style(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOn)):
//							(lv_imgbtn_set_style(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, styleBtn_devMulitSw_statusOff));						

						lv_obj_refresh_style(btn_bk_devMulitSw_C); //必须刷新一下，否则非手动操作时图片刷不动
					}
					
				}break;

				case devTypeDef_scenario:{

					if(devDataPoint_record.devType_scenario.devScenario_opNum != devDataPoint.devType_scenario.devScenario_opNum){

						lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false));
						lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff);
						lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff);
						lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false));
						lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff);
						lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff);
						lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false));
						lv_img_set_style(iconBtn_toilet, styleIconvText_devMulitSw_statusOff);
						lv_label_set_style(textBtn_toilet, styleIconvText_devMulitSw_statusOff);

						switch(devDataPoint.devType_scenario.devScenario_opNum){

							case 1:{

								lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true));
								lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn);
								lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn);
								
							}break;

							case 2:{

								lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true));
								lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn);
								lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn);

							}break;

							case 4:{

								lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true));
								lv_img_set_style(iconBtn_toilet, styleIconvText_devMulitSw_statusOn);
								lv_label_set_style(textBtn_toilet, styleIconvText_devMulitSw_statusOn);

							}break;

							default:break;
						}

						//必须刷新一下，否则非手动操作时图片刷不动
						lv_obj_refresh_style(btn_bk_devMulitSw_A);
						lv_obj_refresh_style(btn_bk_devMulitSw_B);
						lv_obj_refresh_style(btn_bk_devMulitSw_C);
					}

				}break;
	
				case devTypeDef_dimmer:{

					lv_slider_set_value(slider_bk_devDimmer, devDataPoint.devType_dimmer.devDimmer_brightnessVal);
					sprintf(str_sliderBkVal_devDimmer, "%d%%", lv_slider_get_value(slider_bk_devDimmer));
					lv_label_set_text(label_bk_devDimmer, str_sliderBkVal_devDimmer);

					if(lv_slider_get_value(slider_bk_devDimmer)){
					
						stylePhotoBk_devDimmer->image.opa = ((uint8_t)lv_slider_get_value(slider_bk_devDimmer)) * 2 + 50;
					}
					else
					{
						stylePhotoBk_devDimmer->image.opa = 0;
					}
					lv_img_set_style(photoB_bk_devDimmer, stylePhotoBk_devDimmer);
					stylePhotoAk_devDimmer->image.intense = 100 - ((uint8_t)lv_slider_get_value(slider_bk_devDimmer));
					lv_img_set_style(photoA_bk_devDimmer, stylePhotoAk_devDimmer);
					
				}break;

				case devTypeDef_curtain:{

					uint8_t devCurtain_orbitalPosPercent = devCurtain_currentPositionPercentGet();

					lv_slider_set_value(slider_bk_devCurtain, devCurtain_orbitalPosPercent);
					sprintf(str_devParamPositionCur_devCurtain, "%d", devCurtain_orbitalPosPercent);
					lv_label_set_text(label_bk_devCurtain_positionCur, str_devParamPositionCur_devCurtain);
					sprintf(str_devParamPositionAdj_devCurtain, "%d%%", devCurtain_orbitalPosPercent);
					lv_label_set_text(label_bk_devCurtain_positionAdj, str_devParamPositionAdj_devCurtain);
					
				}break;

				case devTypeDef_fans:{

					uint8_t loop = 0;

					for(loop = 0; loop < DEVICE_FANS_OPREAT_ACTION_NUM; loop ++){

						if(devDataPoint.devType_fans.devFans_swEnumVal == objDevFans_btnmDispConferenceTab[loop].opreatActCurrent){
					
							break;
						}
					}		

					if(loop <= DEVICE_FANS_OPREAT_ACTION_NUM){

						switch(loop){

							case fansOpreatAct_firstGear:{

								lv_btnm_set_toggle(btnm_bk_devFans, true, 1);
								lv_label_set_text(textInstract_devFans, "1");
								styleImage_devFans_icon->image.color = LV_COLOR_MAKE(0, 128, 255);
								styleImage_devFans_icon->image.intense = LV_OPA_COVER;
								lv_obj_refresh_style(icomImage_devFans);
								(devStatusDispMethod_landscapeIf_get())?
									(lv_obj_set_pos(icomImage_devFans, 55, 40)):
									(lv_obj_set_pos(icomImage_devFans, 15, 70));
								lv_obj_align(textInstract_devFans, icomImage_devFans, LV_ALIGN_OUT_RIGHT_BOTTOM, 40, 5);
							
							}break;
							
							case fansOpreatAct_secondGear:{

								lv_btnm_set_toggle(btnm_bk_devFans, true, 2);
								lv_label_set_text(textInstract_devFans, "2");
								styleImage_devFans_icon->image.color = LV_COLOR_MAKE(255, 255, 0);
								styleImage_devFans_icon->image.intense = LV_OPA_COVER;
								lv_obj_refresh_style(icomImage_devFans);
								(devStatusDispMethod_landscapeIf_get())?
									(lv_obj_set_pos(icomImage_devFans, 55, 40)):
									(lv_obj_set_pos(icomImage_devFans, 15, 70));
								lv_obj_align(textInstract_devFans, icomImage_devFans, LV_ALIGN_OUT_RIGHT_BOTTOM, 40, 5);
								
							}break;
							
							case fansOpreatAct_thirdGear:{

								lv_btnm_set_toggle(btnm_bk_devFans, true, 3);
								lv_label_set_text(textInstract_devFans, "3");
								styleImage_devFans_icon->image.color = LV_COLOR_MAKE(255, 128, 0);
								styleImage_devFans_icon->image.intense = LV_OPA_COVER;
								lv_obj_refresh_style(icomImage_devFans);
								(devStatusDispMethod_landscapeIf_get())?
									(lv_obj_set_pos(icomImage_devFans, 55, 40)):
									(lv_obj_set_pos(icomImage_devFans, 15, 70));
								lv_obj_align(textInstract_devFans, icomImage_devFans, LV_ALIGN_OUT_RIGHT_BOTTOM, 40, 5);
								
							}break;
							
							case fansOpreatAct_stop:
							default:{

								lv_btnm_set_toggle(btnm_bk_devFans, true, 0);
								lv_label_set_text(textInstract_devFans, "\0");
								styleImage_devFans_icon->image.color = LV_COLOR_MAKE(224, 224, 220);
								styleImage_devFans_icon->image.intense = LV_OPA_COVER;
								lv_obj_refresh_style(icomImage_devFans);
								(devStatusDispMethod_landscapeIf_get())?
									(lv_obj_set_pos(icomImage_devFans, 108, 40)):
									(lv_obj_set_pos(icomImage_devFans, 68, 70));
								lv_obj_align(textInstract_devFans, icomImage_devFans, LV_ALIGN_OUT_RIGHT_BOTTOM, 40, 5);
								
							}break;
						}
					}

				}break;

				case devTypeDef_heater:{

					uint8_t loop = 0;

					for(loop = 0; loop < DEVICE_HEATER_OPREAT_ACTION_NUM; loop ++){
					
						if(devDataPoint.devType_heater.devHeater_swEnumVal == objDevHeater_btnmDispConferenceTab[loop].opreatActCurrent){

							lv_btnm_set_toggle(btnm_bk_devHeater, true, loop);
							
							break;
						}
					}	

					if(loop <= DEVICE_HEATER_OPREAT_ACTION_NUM){

						switch(loop){
							
							case heaterOpreatAct_open:{
						
								lv_label_set_text(textTimeInstract_target_devHeater, "\0");
						
							}break;
							
							case heaterOpreatAct_closeAfter30Min:{
						
								lv_label_set_text(textTimeInstract_target_devHeater, "00:30:00");
						
							}break;
							
							case heaterOpreatAct_closeAfter60Min:{
						
								lv_label_set_text(textTimeInstract_target_devHeater, "00:60:00");
						
							}break;
							
							case heaterOpreatAct_closeAfterTimeCustom:{
						
								char dataDispTemp[15] = {0};
								uint32_t closeCounterTemp = devDriverBussiness_heaterSwitch_closePeriodCustom_Get();
						
								sprintf(dataDispTemp, "%02d:%02d:%02d", (int)((closeCounterTemp % 86400UL) / 3600UL),
																		(int)((closeCounterTemp % 3600UL) / 60UL),
																		(int)(closeCounterTemp % 60UL));
								lv_label_set_text(textTimeInstract_target_devHeater, dataDispTemp);
						
							}break;
							
							case heaterOpreatAct_close:
							default:{
						
								lv_label_set_text(textTimeInstract_target_devHeater, "\0");
						
							}break;
						}
					}

				}break;

				case devTypeDef_thermostat:{

					uint8_t loop = 0;
					stt_thermostat_actAttr devParam_thermostat = {0};
					char tempDisp[5] = {0};
					
					devDriverBussiness_thermostatSwitch_devParam_get(&devParam_thermostat);

					if(!devDataPoint.devType_thermostat.devThermostat_tempratureTarget)
						devDataPoint.devType_thermostat.devThermostat_tempratureTarget = 16;

					lv_slider_set_value(slider_tempAdj_devThermostat, devDataPoint.devType_thermostat.devThermostat_tempratureTarget - 16);
					sprintf(tempDisp, "%02d", (uint8_t)devDataPoint.devType_thermostat.devThermostat_tempratureTarget);
					lv_label_set_text(labelTempInstTarget_devThermostat, tempDisp);
					lv_lmeter_set_value(lmeterTempInstTarget_devThermostat, (uint8_t)devDataPoint.devType_thermostat.devThermostat_tempratureTarget);
					lv_cb_set_checked(cb_devEcoEnable_devThermostat, 
									  devDataPoint.devType_thermostat.devThermostat_nightMode_en);
					usrApp_devThermostat_ctrlObj_reserveSet(devDataPoint.devType_thermostat.devThermostat_running_en);

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)
					for(loop = 0; loop < DEVICE_THERMOSTAT_OPREAT_ACTION_NUM; loop ++){
					
						if(devParam_thermostat.deviceRunning_MODE == objDevThermostat_rollerDispModeSel[loop].opreatActCurrent){
					
							lv_roller_set_selected(roller_devRunningMode_devThermostat, objDevThermostat_rollerDispModeSel[loop].optionSelect, false);
							break;
						}
					}
					(devParam_thermostat.deviceExSwstatus_fans)?
						(lv_imgbtn_set_state(btn_ventilationFan_devThermostat, LV_BTN_STATE_TGL_REL)):
						(lv_imgbtn_set_state(btn_ventilationFan_devThermostat, LV_BTN_STATE_REL));
#else
					(devDataPoint.devType_thermostat.devThermostat_running_en)?
						lv_sw_on(sw_devRunningEnable_devThermostat):
						lv_sw_off(sw_devRunningEnable_devThermostat);
#endif
				}break;

				case devTypeDef_thermostatExtension:{

					if(!devDataPoint.devType_thermostat.devThermostat_tempratureTarget)
						devDataPoint.devType_thermostat.devThermostat_tempratureTarget = 16;

					if(false == dispFlg_devThermostat_firstPage){ //home二级界面

						uint8_t loop = 0;
						stt_thermostat_actAttr devParam_thermostat = {0};
						char tempDisp[5] = {0};
						
						devDriverBussiness_thermostatSwitch_devParam_get(&devParam_thermostat);

						lv_slider_set_value(slider_tempAdj_devThermostat, devDataPoint.devType_thermostat.devThermostat_tempratureTarget - 16);
						sprintf(tempDisp, "%02d", (uint8_t)devDataPoint.devType_thermostat.devThermostat_tempratureTarget);
						lv_label_set_text(labelTempInstTarget_devThermostat, tempDisp);
						lv_lmeter_set_value(lmeterTempInstTarget_devThermostat, (uint8_t)devDataPoint.devType_thermostat.devThermostat_tempratureTarget);
						lv_cb_set_checked(cb_devEcoEnable_devThermostat, 
										  devDataPoint.devType_thermostat.devThermostat_nightMode_en);
						usrApp_devThermostat_ctrlObj_reserveSet(devDataPoint.devType_thermostat.devThermostat_running_en);

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)
						for(loop = 0; loop < DEVICE_THERMOSTAT_OPREAT_ACTION_NUM; loop ++){
						
							if(devParam_thermostat.deviceRunning_MODE == objDevThermostat_rollerDispModeSel[loop].opreatActCurrent){
						
								lv_roller_set_selected(roller_devRunningMode_devThermostat, objDevThermostat_rollerDispModeSel[loop].optionSelect, false);
								break;
							}
						}
						(devParam_thermostat.deviceExSwstatus_fans)?
							(lv_imgbtn_set_state(btn_ventilationFan_devThermostat, LV_BTN_STATE_TGL_REL)):
							(lv_imgbtn_set_state(btn_ventilationFan_devThermostat, LV_BTN_STATE_REL));
#else
						(devDataPoint.devType_thermostat.devThermostat_running_en)?
							lv_sw_on(sw_devRunningEnable_devThermostat):
							lv_sw_off(sw_devRunningEnable_devThermostat);
#endif

					}
					else //home一级界面
					{
						stt_thermostat_actAttr devParamTemp_thermostat = {0};
						uint8_t devThermostatExSwStatus = devDriverBussiness_thermostatSwitch_exSwitchParamGet();
					
						devDriverBussiness_thermostatSwitch_devParam_get(&devParamTemp_thermostat);
						
						sprintf(textDispTemp_thermostatExBtnFold_temp, "#FFFF00 %dC# #00FF00 %dC#", devParamTemp_thermostat.temperatureVal_target,
																									devParamTemp_thermostat.temperatureVal_current);
						lv_label_set_text(textBtn_thermostatExFold_temp, (const char *)textDispTemp_thermostatExBtnFold_temp);
						
						(devParamTemp_thermostat.workModeInNight_IF)?
						   (lv_label_set_text(textBtn_thermostatExFold_eco, "#80FFFF ECO #")):
						   (lv_label_set_text(textBtn_thermostatExFold_eco, "#D2D2D2 ECO #"));

						if(devExParamDiffrent_flg & (1 << 0)){

							(devThermostatExSwStatus & (1 << 0))?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
							(devThermostatExSwStatus & (1 << 0))?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
							(devThermostatExSwStatus & (1 << 0))?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
							lv_obj_refresh_style(btn_bk_devMulitSw_A); //必须刷新一下，否则非手动操作时图片刷不动
						}

						if(devExParamDiffrent_flg & (1 << 1)){

							(devThermostatExSwStatus & (1 << 1))?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
							(devThermostatExSwStatus & (1 << 1))?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
							(devThermostatExSwStatus & (1 << 1))?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));
							lv_obj_refresh_style(btn_bk_devMulitSw_B); //必须刷新一下，否则非手动操作时图片刷不动
						}
					}

				}break;
	
				default:break;
			}

			memcpy(&devDataPoint_record, &devDataPoint, sizeof(stt_devDataPonitTypedef));
		}
	}
}

static lv_res_t funCb_btnActionClick_homeMenu_prLoop(lv_obj_t *btn){

//	const lv_coord_t btnSizeShift = 22; //图标边界保护坐标偏移限定值

//	lv_indev_t * indev = lv_indev_get_act();
//	lv_point_t positionCurrent;

//	lv_indev_get_point(indev, &positionCurrent); //获取当前输入设备输入坐标值

//	//图标边界保护业务逻辑
//	if(positionCurrent.x > LV_HOR_RES - btnSizeShift)positionCurrent.x = LV_HOR_RES - btnSizeShift;
//	if(positionCurrent.x < btnSizeShift)positionCurrent.x = btnSizeShift;
//	if(positionCurrent.y > LV_VER_RES - btnSizeShift)positionCurrent.y = LV_VER_RES - btnSizeShift;
//	if(positionCurrent.y < btnSizeShift)positionCurrent.y = btnSizeShift;
//	lv_obj_set_pos(btn_homeMenu, positionCurrent.x - btnSizeShift, positionCurrent.y - btnSizeShift);
	
	return LV_RES_OK;
}

#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SLAVE_MCU)
 #if(DRVMETHOD_BY_SLAVE_MCU_RELAY_TEST == 1)

static lv_res_t guiBtnFunc_relayMagTest_reset(lv_obj_t *btn){

	paramMagRelayTest.dataRcd.timeRecord = 0;
	paramMagRelayTest.dataRcd.relayActLoop = 0;

	lv_label_set_text(textBtn_sleeping, "RESET");

	debugTestMagRelay_paramSave();

	return LV_RES_OK;
}

static lv_res_t guiBtnFunc_relayMagTest_startEn(lv_obj_t *btn){

	paramMagRelayTest.relayTest_EN = !paramMagRelayTest.relayTest_EN;

	(paramMagRelayTest.relayTest_EN)?
		(lv_label_set_text(textBtn_meeting, "STOP")):
		(lv_label_set_text(textBtn_meeting, "START"));


	if(!paramMagRelayTest.relayTest_EN){

		debugTestMagRelay_paramSave();
	}

	return LV_RES_OK;
}

static void local_guiHomeBussiness_relayMagTest(lv_obj_t * obj_Parent){

	stt_dataDisp_guiBussinessHome_btnText dataTextObjDisp_temp = {0};
	uint8_t dataIconObjDisp_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {0};
	stt_devDataPonitTypedef devDataPoint = {0};
	
	currentDev_dataPointGet(&devDataPoint);
	usrAppHomepageBtnTextDisp_paramGet(&dataTextObjDisp_temp);
	usrAppHomepageBtnIconNumDisp_paramGet(dataIconObjDisp_temp);

	styleBtn_Text->text.font = usrAppHomepageBtnBkText_fontGet(dataTextObjDisp_temp.countryFlg);
	styleIconvText_devMulitSw_statusOn->text.font = styleBtn_Text->text.font;
	styleIconvText_devMulitSw_statusOff->text.font = styleBtn_Text->text.font;

	text_timeRecord = lv_label_create(obj_Parent, NULL);
	text_loopCounter = lv_label_create(obj_Parent, NULL);

	lv_obj_set_pos(text_timeRecord, 15, 30);
	lv_obj_set_pos(text_loopCounter, 15, 50);
	lv_label_set_recolor(text_timeRecord, true);
	lv_label_set_recolor(text_loopCounter, true);

	//home界面开关按键创建，并设置底图、位置、及加载动画
	btn_bk_devMulitSw_A = lv_imgbtn_create(obj_Parent, NULL);
//	lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false));
	lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_A, 20,  55)):
		(lv_obj_set_pos(btn_bk_devMulitSw_A,  5,  81));
	btn_bk_devMulitSw_B = lv_imgbtn_create(obj_Parent, btn_bk_devMulitSw_A);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_B, 20, 137)):
		(lv_obj_set_pos(btn_bk_devMulitSw_B,  5, 187));
	lv_obj_animate(btn_bk_devMulitSw_A, LV_ANIM_FLOAT_RIGHT, 200,	 0, NULL);
	lv_obj_animate(btn_bk_devMulitSw_B, LV_ANIM_FLOAT_RIGHT, 200,	50, NULL);
//	lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, styleBtn_devMulitSw_statusOn);
//	lv_imgbtn_set_style(btn_bk_devMulitSw_B, LV_BTN_STATE_PR, styleBtn_devMulitSw_statusOn);
	//home界面开关按键回调创建
	lv_btn_set_action(btn_bk_devMulitSw_A, LV_BTN_ACTION_CLICK, guiBtnFunc_relayMagTest_startEn);
	lv_btn_set_action(btn_bk_devMulitSw_B, LV_BTN_ACTION_CLICK, guiBtnFunc_relayMagTest_reset);
	//home界面开关按键说明文字创建，并设置位置
	iconBtn_meeting = lv_img_create(btn_bk_devMulitSw_A, NULL);
	iconBtn_sleeping = lv_img_create(btn_bk_devMulitSw_B, NULL);
	lv_img_set_src(iconBtn_meeting, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[0]));
	lv_img_set_src(iconBtn_sleeping, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[1]));
//	lv_img_set_style(iconBtn_meeting, styleBtnImg_icon);
//	lv_img_set_style(iconBtn_sleeping, styleBtnImg_icon);
	lv_obj_set_protect(iconBtn_meeting, LV_PROTECT_POS);
	lv_obj_set_protect(iconBtn_sleeping, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_meeting, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_meeting, NULL, LV_ALIGN_OUT_LEFT_MID, 65, 15));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_sleeping, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_sleeping, NULL, LV_ALIGN_OUT_LEFT_MID, 65, 15));
	//home界面开关按键说明文字风格加载
	textBtn_meeting = lv_label_create(btn_bk_devMulitSw_A, NULL);
	textBtn_sleeping = lv_label_create(btn_bk_devMulitSw_B, NULL);
//	lv_obj_set_style(textBtn_meeting, styleBtn_Text);
//	lv_obj_set_style(textBtn_sleeping, styleBtn_Text);
	lv_label_set_align(textBtn_meeting, LV_LABEL_ALIGN_CENTER);
	lv_label_set_align(textBtn_sleeping, LV_LABEL_ALIGN_CENTER);
	lv_label_set_long_mode(textBtn_meeting, LV_LABEL_LONG_DOT);
	lv_label_set_long_mode(textBtn_sleeping, LV_LABEL_LONG_DOT);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_meeting, 168, 25)):
		(lv_obj_set_size(textBtn_meeting, 128, 25));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_sleeping, 168, 25)):
		(lv_obj_set_size(textBtn_sleeping, 128, 25));
	lv_label_set_text(textBtn_meeting, "START");
	lv_label_set_text(textBtn_sleeping, "RESET");
	lv_obj_set_protect(textBtn_meeting, LV_PROTECT_POS);
	lv_obj_set_protect(textBtn_sleeping, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_meeting, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_meeting, NULL, LV_ALIGN_IN_LEFT_MID, 71, 20));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_sleeping, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_sleeping, NULL, LV_ALIGN_IN_LEFT_MID, 71, 20));

//	(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
//	(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
//	(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
//	(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
//	(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
//	(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));
}
 #endif
#endif

static void local_guiHomeBussiness_mulitSwOneBit(lv_obj_t * obj_Parent){

	stt_dataDisp_guiBussinessHome_btnText dataTextObjDisp_temp = {0};
	uint8_t dataIconObjDisp_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {0};
	stt_devDataPonitTypedef devDataPoint = {0};
	
	currentDev_dataPointGet(&devDataPoint);
	usrAppHomepageBtnTextDisp_paramGet(&dataTextObjDisp_temp);
	usrAppHomepageBtnIconNumDisp_paramGet(dataIconObjDisp_temp);
	
	styleBtn_Text->text.font = usrAppHomepageBtnBkText_fontGet(dataTextObjDisp_temp.countryFlg);
	styleIconvText_devMulitSw_statusOn->text.font = styleBtn_Text->text.font;
	styleIconvText_devMulitSw_statusOff->text.font = styleBtn_Text->text.font;

	//home界面开关按键创建，并设置底图、位置、及加载动画
	btn_bk_devMulitSw_A = lv_imgbtn_create(obj_Parent, NULL);
//	lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false));
	lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_A, 20,  95)):
		(lv_obj_set_pos(btn_bk_devMulitSw_A,  5, 134));
	lv_obj_animate(btn_bk_devMulitSw_A, LV_ANIM_FLOAT_RIGHT, 200, 0, NULL);
//	lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, styleBtn_devMulitSw_statusOn);
	//home界面开关按键回调创建
	lv_btn_set_action(btn_bk_devMulitSw_A, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devMulitSw_mainBtnA);
	//home界面开关按键说明文字创建，并设置位置
	iconBtn_meeting = lv_img_create(btn_bk_devMulitSw_A, NULL);
	lv_img_set_src(iconBtn_meeting, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[0]));
//	lv_img_set_style(iconBtn_meeting, styleBtnImg_icon);
	lv_obj_set_protect(iconBtn_meeting, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_meeting, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_meeting, NULL, LV_ALIGN_OUT_LEFT_MID, 65, 15));
	//home界面开关按键说明文字风格加载
	textBtn_meeting = lv_label_create(btn_bk_devMulitSw_A, NULL);
//	lv_obj_set_style(textBtn_meeting, styleBtn_Text);
	lv_label_set_align(textBtn_meeting, LV_LABEL_ALIGN_CENTER);
	lv_label_set_long_mode(textBtn_meeting, LV_LABEL_LONG_DOT);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_meeting, 168, 25)):
		(lv_obj_set_size(textBtn_meeting, 128, 25));
	lv_label_set_text(textBtn_meeting, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[0]);
	lv_obj_set_protect(textBtn_meeting, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_meeting, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_meeting, NULL, LV_ALIGN_IN_LEFT_MID, 71, 20));

	(devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
	(devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
	(devDataPoint.devType_mulitSwitch_oneBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
}

static void local_guiHomeBussiness_mulitSwTwoBit(lv_obj_t * obj_Parent){

#if(DEVICE_DRIVER_DEFINITION == DEVICE_DRIVER_METHOD_BY_SLAVE_MCU)
 #if(DRVMETHOD_BY_SLAVE_MCU_RELAY_TEST == 1)
		
	local_guiHomeBussiness_relayMagTest(obj_Parent);
	return;
 #endif
#endif

	stt_dataDisp_guiBussinessHome_btnText dataTextObjDisp_temp = {0};
	uint8_t dataIconObjDisp_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {0};
	stt_devDataPonitTypedef devDataPoint = {0};
	
	currentDev_dataPointGet(&devDataPoint);
	usrAppHomepageBtnTextDisp_paramGet(&dataTextObjDisp_temp);
	usrAppHomepageBtnIconNumDisp_paramGet(dataIconObjDisp_temp);

	styleBtn_Text->text.font = usrAppHomepageBtnBkText_fontGet(dataTextObjDisp_temp.countryFlg);
	styleIconvText_devMulitSw_statusOn->text.font = styleBtn_Text->text.font;
	styleIconvText_devMulitSw_statusOff->text.font = styleBtn_Text->text.font;

	//home界面开关按键创建，并设置底图、位置、及加载动画
	btn_bk_devMulitSw_A = lv_imgbtn_create(obj_Parent, NULL);
//	lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false));
	lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_A, 20,  55)):
		(lv_obj_set_pos(btn_bk_devMulitSw_A,  5,  81));
	btn_bk_devMulitSw_B = lv_imgbtn_create(obj_Parent, btn_bk_devMulitSw_A);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_B, 20, 137)):
		(lv_obj_set_pos(btn_bk_devMulitSw_B,  5, 187));
	lv_obj_animate(btn_bk_devMulitSw_A, LV_ANIM_FLOAT_RIGHT, 200,	 0, NULL);
	lv_obj_animate(btn_bk_devMulitSw_B, LV_ANIM_FLOAT_RIGHT, 200,	50, NULL);
//	lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, styleBtn_devMulitSw_statusOn);
//	lv_imgbtn_set_style(btn_bk_devMulitSw_B, LV_BTN_STATE_PR, styleBtn_devMulitSw_statusOn);
	//home界面开关按键回调创建
	lv_btn_set_action(btn_bk_devMulitSw_A, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devMulitSw_mainBtnA);
	lv_btn_set_action(btn_bk_devMulitSw_B, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devMulitSw_mainBtnB);
	//home界面开关按键说明文字创建，并设置位置
	iconBtn_meeting = lv_img_create(btn_bk_devMulitSw_A, NULL);
	iconBtn_sleeping = lv_img_create(btn_bk_devMulitSw_B, NULL);
	lv_img_set_src(iconBtn_meeting, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[0]));
	lv_img_set_src(iconBtn_sleeping, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[1]));
//	lv_img_set_style(iconBtn_meeting, styleBtnImg_icon);
//	lv_img_set_style(iconBtn_sleeping, styleBtnImg_icon);
	lv_obj_set_protect(iconBtn_meeting, LV_PROTECT_POS);
	lv_obj_set_protect(iconBtn_sleeping, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_meeting, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_meeting, NULL, LV_ALIGN_OUT_LEFT_MID, 65, 15));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_sleeping, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_sleeping, NULL, LV_ALIGN_OUT_LEFT_MID, 65, 15));
	//home界面开关按键说明文字风格加载
	textBtn_meeting = lv_label_create(btn_bk_devMulitSw_A, NULL);
	textBtn_sleeping = lv_label_create(btn_bk_devMulitSw_B, NULL);
//	lv_obj_set_style(textBtn_meeting, styleBtn_Text);
//	lv_obj_set_style(textBtn_sleeping, styleBtn_Text);
	lv_label_set_align(textBtn_meeting, LV_LABEL_ALIGN_CENTER);
	lv_label_set_align(textBtn_sleeping, LV_LABEL_ALIGN_CENTER);
	lv_label_set_long_mode(textBtn_meeting, LV_LABEL_LONG_DOT);
	lv_label_set_long_mode(textBtn_sleeping, LV_LABEL_LONG_DOT);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_meeting, 168, 25)):
		(lv_obj_set_size(textBtn_meeting, 128, 25));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_sleeping, 168, 25)):
		(lv_obj_set_size(textBtn_sleeping, 128, 25));
	lv_label_set_text(textBtn_meeting, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[0]);
	lv_label_set_text(textBtn_sleeping, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[1]);
	lv_obj_set_protect(textBtn_meeting, LV_PROTECT_POS);
	lv_obj_set_protect(textBtn_sleeping, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_meeting, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_meeting, NULL, LV_ALIGN_IN_LEFT_MID, 71, 20));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_sleeping, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_sleeping, NULL, LV_ALIGN_IN_LEFT_MID, 71, 20));

	(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
	(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
	(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
	(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
	(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
	(devDataPoint.devType_mulitSwitch_twoBit.swVal_bit2)?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));	
}

static void local_guiHomeBussiness_mulitSwThreeBit(lv_obj_t * obj_Parent){

	stt_dataDisp_guiBussinessHome_btnText dataTextObjDisp_temp = {0};
	uint8_t dataIconObjDisp_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {0};
	stt_devDataPonitTypedef devDataPoint = {0};
	
	currentDev_dataPointGet(&devDataPoint);	
	usrAppHomepageBtnTextDisp_paramGet(&dataTextObjDisp_temp);
	usrAppHomepageBtnIconNumDisp_paramGet(dataIconObjDisp_temp);

	styleBtn_Text->text.font = usrAppHomepageBtnBkText_fontGet(dataTextObjDisp_temp.countryFlg);
	styleIconvText_devMulitSw_statusOn->text.font = styleBtn_Text->text.font;
	styleIconvText_devMulitSw_statusOff->text.font = styleBtn_Text->text.font;

	//home界面开关按键创建，并设置底图、位置、及加载动画
	btn_bk_devMulitSw_A = lv_imgbtn_create(obj_Parent, NULL);
//	lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false));
	lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));
	lv_obj_set_protect(btn_bk_devMulitSw_A, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_A, 20,  30)):
		(lv_obj_set_pos(btn_bk_devMulitSw_A,  5,  41));
	btn_bk_devMulitSw_B = lv_imgbtn_create(obj_Parent, btn_bk_devMulitSw_A);
	lv_obj_set_protect(btn_bk_devMulitSw_B, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_B, 20, 100)):
		(lv_obj_set_pos(btn_bk_devMulitSw_B,  5, 134));
	btn_bk_devMulitSw_C = lv_imgbtn_create(obj_Parent, btn_bk_devMulitSw_A);
	lv_obj_set_protect(btn_bk_devMulitSw_C, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_C, 20, 170)):
		(lv_obj_set_pos(btn_bk_devMulitSw_C,  5, 227));
	lv_obj_animate(btn_bk_devMulitSw_A, LV_ANIM_FLOAT_RIGHT, 200,	0, NULL);
	lv_obj_animate(btn_bk_devMulitSw_B, LV_ANIM_FLOAT_RIGHT, 200,  50, NULL);
	lv_obj_animate(btn_bk_devMulitSw_C, LV_ANIM_FLOAT_RIGHT, 200, 100, NULL);
//	lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, styleBtn_devMulitSw_statusOn);
//	lv_imgbtn_set_style(btn_bk_devMulitSw_B, LV_BTN_STATE_PR, styleBtn_devMulitSw_statusOn);
//	lv_imgbtn_set_style(btn_bk_devMulitSw_C, LV_BTN_STATE_PR, styleBtn_devMulitSw_statusOn);
	//home界面开关按键回调创建
	lv_btn_set_action(btn_bk_devMulitSw_A, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devMulitSw_mainBtnA);
	lv_btn_set_action(btn_bk_devMulitSw_B, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devMulitSw_mainBtnB);
	lv_btn_set_action(btn_bk_devMulitSw_C, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devMulitSw_mainBtnC);
	//home界面开关按键说明文字创建，并设置位置
	iconBtn_meeting = lv_img_create(btn_bk_devMulitSw_A, NULL);
	iconBtn_sleeping = lv_img_create(btn_bk_devMulitSw_B, NULL);
	iconBtn_toilet = lv_img_create(btn_bk_devMulitSw_C, NULL);
	lv_img_set_src(iconBtn_meeting, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[0]));
	lv_img_set_src(iconBtn_sleeping, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[1]));
	lv_img_set_src(iconBtn_toilet, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[2]));
//	lv_img_set_style(iconBtn_meeting, styleBtnImg_icon);
//	lv_img_set_style(iconBtn_sleeping, styleBtnImg_icon);
//	lv_img_set_style(iconBtn_toilet, styleBtnImg_icon);
	lv_obj_set_protect(iconBtn_meeting, LV_PROTECT_POS);
	lv_obj_set_protect(iconBtn_sleeping, LV_PROTECT_POS);
	lv_obj_set_protect(iconBtn_toilet, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_meeting, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_meeting, NULL, LV_ALIGN_OUT_LEFT_MID, 65, 15));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_sleeping, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_sleeping, NULL, LV_ALIGN_OUT_LEFT_MID, 65, 15));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_toilet, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_toilet, NULL, LV_ALIGN_OUT_LEFT_MID, 65, 15));

//	lv_btn_set_ink_in_time(btn_bk_devMulitSw_A, 200);
//	lv_btn_set_ink_wait_time(btn_bk_devMulitSw_A, 50);
//	lv_btn_set_ink_out_time(btn_bk_devMulitSw_A, 200);
//	lv_btn_set_ink_in_time(btn_bk_devMulitSw_B, 200);
//	lv_btn_set_ink_wait_time(btn_bk_devMulitSw_B, 50);
//	lv_btn_set_ink_out_time(btn_bk_devMulitSw_B, 200);
//	lv_btn_set_ink_in_time(btn_bk_devMulitSw_C, 200);
//	lv_btn_set_ink_wait_time(btn_bk_devMulitSw_C, 50);
//	lv_btn_set_ink_out_time(btn_bk_devMulitSw_C, 200);

//	testImg_data.data = dataPtr_btnTextImg_sw_A;
//	lv_obj_t *iconTest = lv_img_create(btn_bk_devMulitSw_A, NULL);
//	lv_img_set_src(iconTest, &testImg_data);
//	lv_obj_set_protect(iconTest, LV_PROTECT_POS);
//	lv_obj_align(iconTest, btn_bk_devMulitSw_A, LV_ALIGN_CENTER, 0, -30);
//	lv_obj_set_top(iconTest, true);
	
	//home界面开关按键说明文字风格加载
	textBtn_meeting = lv_label_create(btn_bk_devMulitSw_A, NULL);
	textBtn_sleeping = lv_label_create(btn_bk_devMulitSw_B, NULL);
	textBtn_toilet = lv_label_create(btn_bk_devMulitSw_C, NULL);
//	lv_obj_set_style(textBtn_meeting, styleBtn_Text);
//	lv_obj_set_style(textBtn_sleeping, styleBtn_Text);
//	lv_obj_set_style(textBtn_toilet, styleBtn_Text);
	lv_label_set_align(textBtn_meeting, LV_LABEL_ALIGN_CENTER);
	lv_label_set_align(textBtn_sleeping, LV_LABEL_ALIGN_CENTER);
	lv_label_set_align(textBtn_toilet, LV_LABEL_ALIGN_CENTER);
	lv_label_set_long_mode(textBtn_meeting, LV_LABEL_LONG_ROLL);
	lv_label_set_long_mode(textBtn_sleeping, LV_LABEL_LONG_DOT);
	lv_label_set_long_mode(textBtn_toilet, LV_LABEL_LONG_DOT);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_meeting, 168, 25)):
		(lv_obj_set_size(textBtn_meeting, 128, 25));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_sleeping, 168, 25)):
		(lv_obj_set_size(textBtn_sleeping, 128, 25));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_toilet, 168, 25)):
		(lv_obj_set_size(textBtn_toilet, 128, 25));
	lv_label_set_text(textBtn_meeting, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[0]);
	lv_label_set_text(textBtn_sleeping, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[1]);
	lv_label_set_text(textBtn_toilet, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[2]);
	lv_obj_set_protect(textBtn_meeting, LV_PROTECT_POS);
	lv_obj_set_protect(textBtn_sleeping, LV_PROTECT_POS);
	lv_obj_set_protect(textBtn_toilet, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_meeting, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_meeting, NULL, LV_ALIGN_IN_LEFT_MID, 71, 20));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_sleeping, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_sleeping, NULL, LV_ALIGN_IN_LEFT_MID, 71, 20));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_toilet, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_toilet, NULL, LV_ALIGN_IN_LEFT_MID, 71, 20));

	(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
	(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
	(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit1)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
	(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
	(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
	(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit2)?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));
	(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?(lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
	(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?(lv_img_set_style(iconBtn_toilet, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_toilet, styleIconvText_devMulitSw_statusOff));
	(devDataPoint.devType_mulitSwitch_threeBit.swVal_bit3)?(lv_label_set_style(textBtn_toilet, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_toilet, styleIconvText_devMulitSw_statusOff));
}

static void local_guiHomeBussiness_dimmer(lv_obj_t * obj_Parent){

	stt_devDataPonitTypedef devDataPoint = {0};
	
	currentDev_dataPointGet(&devDataPoint);

	photoA_bk_devDimmer = lv_imgbtn_create(obj_Parent, NULL);
	photoB_bk_devDimmer = lv_img_create(obj_Parent, NULL);
	lv_imgbtn_set_src(photoA_bk_devDimmer, LV_BTN_STATE_REL, &iconLightA_HomePageDeviceDimmer);
	lv_imgbtn_set_src(photoA_bk_devDimmer, LV_BTN_STATE_PR, &iconLightA_HomePageDeviceDimmer);
	lv_img_set_src(photoB_bk_devDimmer, &iconLightB_HomePageDeviceDimmer);
	lv_obj_set_protect(photoA_bk_devDimmer, LV_PROTECT_POS);
	lv_obj_set_protect(photoB_bk_devDimmer, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(photoA_bk_devDimmer, 80, 30)):
		(lv_obj_set_pos(photoA_bk_devDimmer, 40, 60));
	lv_obj_align(photoB_bk_devDimmer, photoA_bk_devDimmer, LV_ALIGN_CENTER, 5, -25);
	lv_img_set_style(photoB_bk_devDimmer, stylePhotoBk_devDimmer);
	lv_imgbtn_set_action(photoA_bk_devDimmer, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devDimmer_mainBtn);

	slider_bk_devDimmer = lv_slider_create(obj_Parent, NULL);
	lv_obj_set_size(slider_bk_devDimmer, 200, 30);
	lv_obj_set_protect(slider_bk_devDimmer, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(slider_bk_devDimmer, 60, 200)):
		(lv_obj_set_pos(slider_bk_devDimmer, 20, 260));
	lv_slider_set_action(slider_bk_devDimmer, funCb_slidAction_devDimmer_mainSlider);
	lv_slider_set_range(slider_bk_devDimmer, 0, DEVICE_DIMMER_BRIGHTNESS_MAX_VAL);

	lv_slider_set_style(slider_bk_devDimmer, LV_SLIDER_STYLE_BG, styleSliderBk_devDimmer_bg);
	lv_slider_set_style(slider_bk_devDimmer, LV_SLIDER_STYLE_INDIC, styleSliderBk_devDimmer_indic);
	lv_slider_set_style(slider_bk_devDimmer, LV_SLIDER_STYLE_KNOB, styleSliderBk_devDimmer_knob);

	label_bk_devDimmer = lv_label_create(obj_Parent, NULL);
	lv_label_set_style(label_bk_devDimmer, styleText_devDimmer_SliderBk);
	lv_slider_set_value(slider_bk_devDimmer, devDataPoint.devType_dimmer.devDimmer_brightnessVal);
	sprintf(str_sliderBkVal_devDimmer, "%d%%", lv_slider_get_value(slider_bk_devDimmer));
	lv_label_set_text(label_bk_devDimmer, str_sliderBkVal_devDimmer);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(label_bk_devDimmer, slider_bk_devDimmer, LV_ALIGN_OUT_TOP_RIGHT, -20, -20)):
		(lv_obj_align(label_bk_devDimmer, slider_bk_devDimmer, LV_ALIGN_OUT_TOP_MID, 0, 0));
	
	if(lv_slider_get_value(slider_bk_devDimmer)){
	
		stylePhotoBk_devDimmer->image.opa = ((uint8_t)lv_slider_get_value(slider_bk_devDimmer)) * 2 + 50;
	}
	else
	{
		stylePhotoBk_devDimmer->image.opa = 0;
	}
	lv_img_set_style(photoB_bk_devDimmer, stylePhotoBk_devDimmer);
	stylePhotoAk_devDimmer->image.color = LV_COLOR_GRAY;
	stylePhotoAk_devDimmer->image.intense = 100 - ((uint8_t)lv_slider_get_value(slider_bk_devDimmer));
	lv_img_set_style(photoA_bk_devDimmer, stylePhotoAk_devDimmer);
}

static void local_guiHomeBussiness_fans(lv_obj_t * obj_Parent){

	icomImage_devFans = lv_img_create(obj_Parent, NULL);
	lv_img_set_src(icomImage_devFans, &iconFans_HomePageDeviceFans);
	lv_img_set_style(icomImage_devFans, styleImage_devFans_icon);
	lv_obj_set_protect(icomImage_devFans, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(icomImage_devFans, 108, 40)):
		(lv_obj_set_pos(icomImage_devFans, 68, 60));

	textInstract_devFans = lv_label_create(obj_Parent, NULL);
	lv_label_set_text(textInstract_devFans, "\0");
	lv_label_set_style(textInstract_devFans, styleText_devFans_instract);
	lv_obj_set_protect(textInstract_devFans, LV_PROTECT_POS);
	lv_obj_align(textInstract_devFans, icomImage_devFans, LV_ALIGN_OUT_RIGHT_BOTTOM, 40, 5);

	btnm_bk_devFans = lv_btnm_create(obj_Parent, NULL);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(btnm_bk_devFans, 195, 50)):
		(lv_obj_set_size(btnm_bk_devFans, 195, 65));

	lv_btnm_set_style(btnm_bk_devFans, LV_BTNM_STYLE_BG, styleBtnm_devFans_btnBg);
	lv_btnm_set_style(btnm_bk_devFans, LV_BTNM_STYLE_BTN_REL, styleBtnm_devFans_btnRel);
	lv_btnm_set_style(btnm_bk_devFans, LV_BTNM_STYLE_BTN_PR, styleBtnm_devFans_btnPre);
	lv_btnm_set_style(btnm_bk_devFans, LV_BTNM_STYLE_BTN_TGL_REL, styleBtnm_devFans_btnTglRel);
	lv_btnm_set_style(btnm_bk_devFans, LV_BTNM_STYLE_BTN_TGL_PR, styleBtnm_devFans_btnTglPre);
	lv_btnm_set_style(btnm_bk_devFans, LV_BTNM_STYLE_BTN_INA, styleBtnm_devFans_btnIna);

	lv_btnm_set_action(btnm_bk_devFans, funCb_btnmActionClick_devFans_gearBtnm);
	lv_btnm_set_map(btnm_bk_devFans, btnm_str_devFans);
	lv_btnm_set_toggle(btnm_bk_devFans, true, 0);
	lv_obj_set_protect(btnm_bk_devFans, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btnm_bk_devFans, 60, 185)):
		(lv_obj_set_pos(btnm_bk_devFans, 20, 230));
}

static void local_guiHomeBussiness_thermostat(lv_obj_t * obj_Parent){

	uint8_t loop = 0;
	stt_thermostat_actAttr devParam_thermostat = {0};
	char tempDisp[5] = {0};

	devDriverBussiness_thermostatSwitch_devParam_get(&devParam_thermostat);
	if(devParam_thermostat.temperatureVal_target < 16)
		devParam_thermostat.temperatureVal_target = 16;

	lmeterTempInstTarget_devThermostat = lv_lmeter_create(obj_Parent, NULL);
	lv_lmeter_set_range(lmeterTempInstTarget_devThermostat, 0, 45);				 
	lv_lmeter_set_value(lmeterTempInstTarget_devThermostat, (uint8_t)devParam_thermostat.temperatureVal_target);					
	lv_lmeter_set_style(lmeterTempInstTarget_devThermostat, styleLmeter_devThermostat_tempTarget);	
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(lmeterTempInstTarget_devThermostat, 200, 170)):
		(lv_obj_set_size(lmeterTempInstTarget_devThermostat, 200, 200));
	lv_lmeter_set_scale(lmeterTempInstTarget_devThermostat, 260, 61);
	lv_obj_set_protect(lmeterTempInstTarget_devThermostat, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(lmeterTempInstTarget_devThermostat, 60, 30)):
		(lv_obj_set_pos(lmeterTempInstTarget_devThermostat, 20, 50));
	lmeterTempInstCurrent_devThermostat = lv_lmeter_create(obj_Parent, NULL);
	lv_lmeter_set_range(lmeterTempInstCurrent_devThermostat, 0, 45);				 
	lv_lmeter_set_value(lmeterTempInstCurrent_devThermostat, (uint8_t)devParam_thermostat.temperatureVal_current);					
	lv_lmeter_set_style(lmeterTempInstCurrent_devThermostat, styleLmeter_devThermostat_tempCurrent);	
	lv_obj_set_size(lmeterTempInstCurrent_devThermostat, 180, 180);
	lv_lmeter_set_scale(lmeterTempInstCurrent_devThermostat, 260, 61);
	lv_obj_set_protect(lmeterTempInstCurrent_devThermostat, LV_PROTECT_POS);
	lv_obj_align(lmeterTempInstCurrent_devThermostat, lmeterTempInstTarget_devThermostat, LV_ALIGN_CENTER, 0, 0);

	labelTempInstTarget_devThermostat = lv_label_create(obj_Parent, NULL);
	sprintf(tempDisp, "%02d", (uint8_t)devParam_thermostat.temperatureVal_target);
	lv_label_set_text(labelTempInstTarget_devThermostat, tempDisp);
	lv_label_set_style(labelTempInstTarget_devThermostat, styleLabel_devThermostat_tempTarget);
	lv_obj_set_protect(labelTempInstTarget_devThermostat, LV_PROTECT_POS);
	lv_obj_align(labelTempInstTarget_devThermostat, lmeterTempInstCurrent_devThermostat, LV_ALIGN_CENTER, -5, -15);
	labelTempInstCurrent_devThermostat = lv_label_create(obj_Parent, NULL);
	sprintf(tempDisp, "%02d", (uint8_t)devParam_thermostat.temperatureVal_current);
	lv_label_set_text(labelTempInstCurrent_devThermostat, tempDisp);
	lv_label_set_style(labelTempInstCurrent_devThermostat, styleLabel_devThermostat_tempCurrent);
	lv_obj_set_protect(labelTempInstCurrent_devThermostat, LV_PROTECT_POS);
	lv_obj_align(labelTempInstCurrent_devThermostat, labelTempInstTarget_devThermostat, LV_ALIGN_OUT_BOTTOM_RIGHT, 30, -5);

	slider_tempAdj_devThermostat = lv_slider_create(obj_Parent, NULL);
	lv_obj_set_size(slider_tempAdj_devThermostat, 180, 30);
	lv_slider_set_style(slider_tempAdj_devThermostat, LV_SLIDER_STYLE_BG, styleSliderBk_devThermostat_bg);
	lv_slider_set_style(slider_tempAdj_devThermostat, LV_SLIDER_STYLE_INDIC, styleSliderBk_devThermostat_indic);
	lv_slider_set_style(slider_tempAdj_devThermostat, LV_SLIDER_STYLE_KNOB, styleSliderBk_devThermostat_knob);
	lv_obj_set_protect(slider_tempAdj_devThermostat, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(slider_tempAdj_devThermostat, lmeterTempInstTarget_devThermostat, LV_ALIGN_OUT_BOTTOM_MID, 0, 8)):
		(lv_obj_align(slider_tempAdj_devThermostat, lmeterTempInstTarget_devThermostat, LV_ALIGN_OUT_BOTTOM_MID, 0, 20));
	lv_slider_set_action(slider_tempAdj_devThermostat, funCb_slidAction_devThermostat_mainSlider);
	lv_slider_set_range(slider_tempAdj_devThermostat, 0, DEVICE_THERMOSTAT_ADJUST_EQUAL_RANGE);	
	lv_bar_set_value(slider_tempAdj_devThermostat, 0);
	lv_slider_set_value(slider_tempAdj_devThermostat, devParam_thermostat.temperatureVal_target - 16);

	btn_tempAdjAdd_devThermostat = lv_btn_create(obj_Parent, NULL);
	lv_btn_set_action(btn_tempAdjAdd_devThermostat, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devThermostat_tempAdd);
	lv_obj_set_size(btn_tempAdjAdd_devThermostat, 50, 50);
    lv_btn_set_style(btn_tempAdjAdd_devThermostat, LV_BTN_STYLE_REL, 	stylebtnBk_transFull);
    lv_btn_set_style(btn_tempAdjAdd_devThermostat, LV_BTN_STYLE_PR, 	stylebtnBk_transFull);
    lv_btn_set_style(btn_tempAdjAdd_devThermostat, LV_BTN_STYLE_TGL_REL,stylebtnBk_transFull);
    lv_btn_set_style(btn_tempAdjAdd_devThermostat, LV_BTN_STYLE_TGL_PR, stylebtnBk_transFull);
	lv_obj_set_protect(btn_tempAdjAdd_devThermostat, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(btn_tempAdjAdd_devThermostat, slider_tempAdj_devThermostat, LV_ALIGN_OUT_RIGHT_MID, 5, 0)):
		(lv_obj_align(btn_tempAdjAdd_devThermostat, slider_tempAdj_devThermostat, LV_ALIGN_OUT_TOP_RIGHT, 10, 0));
	btn_tempAdjCut_devThermostat = lv_btn_create(obj_Parent, NULL);
	lv_btn_set_action(btn_tempAdjCut_devThermostat, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devThermostat_tempCut);
	lv_obj_set_size(btn_tempAdjCut_devThermostat, 50, 50);
    lv_btn_set_style(btn_tempAdjCut_devThermostat, LV_BTN_STYLE_REL, 	stylebtnBk_transFull);
    lv_btn_set_style(btn_tempAdjCut_devThermostat, LV_BTN_STYLE_PR, 	stylebtnBk_transFull);
    lv_btn_set_style(btn_tempAdjCut_devThermostat, LV_BTN_STYLE_TGL_REL,stylebtnBk_transFull);
    lv_btn_set_style(btn_tempAdjCut_devThermostat, LV_BTN_STYLE_TGL_PR, stylebtnBk_transFull);
	lv_obj_set_protect(btn_tempAdjCut_devThermostat, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(btn_tempAdjCut_devThermostat, slider_tempAdj_devThermostat, LV_ALIGN_OUT_LEFT_MID, -5, 0)):
		(lv_obj_align(btn_tempAdjCut_devThermostat, slider_tempAdj_devThermostat, LV_ALIGN_OUT_TOP_LEFT, -10, 0));

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)
	roller_devRunningMode_devThermostat = lv_roller_create(obj_Parent, NULL);
	lv_roller_set_action(roller_devRunningMode_devThermostat, funCb_rollerAction_devThermostat_gearSel);
	lv_roller_set_options(roller_devRunningMode_devThermostat, rollerGearStr_devThermostat);
	lv_obj_set_protect(roller_devRunningMode_devThermostat, LV_PROTECT_POS);
	lv_roller_set_hor_fit(roller_devRunningMode_devThermostat, false);
	lv_obj_set_width(roller_devRunningMode_devThermostat, 80);
	lv_obj_align(roller_devRunningMode_devThermostat, lmeterTempInstTarget_devThermostat, LV_ALIGN_OUT_BOTTOM_MID, 0, -50);
	for(loop = 0; loop < DEVICE_THERMOSTAT_OPREAT_ACTION_NUM; loop ++){

		if(devParam_thermostat.deviceRunning_MODE == objDevThermostat_rollerDispModeSel[loop].opreatActCurrent){

			lv_roller_set_selected(roller_devRunningMode_devThermostat, objDevThermostat_rollerDispModeSel[loop].optionSelect, false);
			break;
		}
	}
	//	lv_obj_align(roller_devRunningMode_devThermostat, NULL, LV_ALIGN_CENTER, 0, 0);
	//	lv_roller_set_selected(roller_devRunningMode_devThermostat, cyIst, false);
	lv_roller_set_style(roller_devRunningMode_devThermostat, LV_ROLLER_STYLE_BG, styleRoller_thermostatGearSel_bg);
	lv_roller_set_style(roller_devRunningMode_devThermostat, LV_ROLLER_STYLE_SEL, styleRoller_thermostatGearSel_sel);
	lv_roller_set_visible_row_count(roller_devRunningMode_devThermostat, 3);

	btn_devRunningModeSw_devThermostat = lv_btn_create(obj_Parent, NULL);
//	lv_btn_set_style(btn_devRunningModeSw_devThermostat, LV_BTN_STYLE_REL, &lv_style_transp);
//	lv_btn_set_style(btn_devRunningModeSw_devThermostat, LV_BTN_STYLE_PR, &lv_style_transp);
//	lv_btn_set_style(btn_devRunningModeSw_devThermostat, LV_BTN_STYLE_TGL_REL, &lv_style_transp);
//	lv_btn_set_style(btn_devRunningModeSw_devThermostat, LV_BTN_STYLE_TGL_PR, &lv_style_transp);
	lv_obj_set_size(btn_devRunningModeSw_devThermostat, 50, 28);
	lv_obj_set_protect(btn_devRunningModeSw_devThermostat, LV_PROTECT_FOLLOW);
	lv_obj_set_protect(btn_devRunningModeSw_devThermostat, LV_PROTECT_POS);
	lv_obj_align(btn_devRunningModeSw_devThermostat, lmeterTempInstTarget_devThermostat, LV_ALIGN_OUT_BOTTOM_MID, 0, -33);
	lv_btn_set_action(btn_devRunningModeSw_devThermostat, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devThermostat_rollerChg);

	btn_ventilationFan_devThermostat = lv_imgbtn_create(obj_Parent, NULL);
	lv_imgbtn_set_toggle(btn_ventilationFan_devThermostat, true);
	lv_imgbtn_set_src(btn_ventilationFan_devThermostat, LV_BTN_STATE_REL, &iconDevThermostat_ventFan);
	lv_imgbtn_set_src(btn_ventilationFan_devThermostat, LV_BTN_STATE_PR, &iconDevThermostat_ventFan);
	lv_imgbtn_set_src(btn_ventilationFan_devThermostat, LV_BTN_STATE_TGL_REL, &iconDevThermostat_ventFan);
	lv_imgbtn_set_src(btn_ventilationFan_devThermostat, LV_BTN_STATE_TGL_PR, &iconDevThermostat_ventFan);
	lv_imgbtn_set_style(btn_ventilationFan_devThermostat, LV_BTN_STATE_REL, styleBtn_thermostatVentFan_rel);
	lv_imgbtn_set_style(btn_ventilationFan_devThermostat, LV_BTN_STATE_PR, styleBtn_thermostatVentFan_pre);
	lv_imgbtn_set_style(btn_ventilationFan_devThermostat, LV_BTN_STATE_TGL_REL, styleBtn_thermostatVentFan_pre);
	lv_imgbtn_set_style(btn_ventilationFan_devThermostat, LV_BTN_STATE_TGL_PR, styleBtn_thermostatVentFan_pre);
	lv_obj_set_protect(btn_ventilationFan_devThermostat, LV_PROTECT_POS);
	lv_obj_align(btn_ventilationFan_devThermostat, lmeterTempInstTarget_devThermostat, LV_ALIGN_OUT_TOP_LEFT, -10, 34);
	lv_imgbtn_set_action(btn_ventilationFan_devThermostat, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devThermostat_ventFanOpraet);
	(devParam_thermostat.deviceExSwstatus_fans)?
		(lv_imgbtn_set_state(btn_ventilationFan_devThermostat, LV_BTN_STATE_TGL_REL)):
		(lv_imgbtn_set_state(btn_ventilationFan_devThermostat, LV_BTN_STATE_REL));
#else
	sw_devRunningEnable_devThermostat = lv_sw_create(obj_Parent, NULL);
	lv_obj_set_size(sw_devRunningEnable_devThermostat, 55, 25);
	lv_obj_set_protect(sw_devRunningEnable_devThermostat, LV_PROTECT_POS);
	lv_obj_align(sw_devRunningEnable_devThermostat, lmeterTempInstTarget_devThermostat, LV_ALIGN_OUT_BOTTOM_MID, 0, -25);
	if(devParam_thermostat.deviceRunning_EN){

		lv_sw_on(sw_devRunningEnable_devThermostat);
	}
	else
	{
		lv_sw_off(sw_devRunningEnable_devThermostat);
	}
	lv_sw_set_action(sw_devRunningEnable_devThermostat, funCb_swAction_devThermostat_runningEnable);
#endif

	cb_devEcoEnable_devThermostat = lv_cb_create(obj_Parent, NULL);
	lv_cb_set_action(cb_devEcoEnable_devThermostat, funCb_cbChecked_devThermostat_EcoEnable);
	lv_obj_set_protect(cb_devEcoEnable_devThermostat, LV_PROTECT_POS);
	lv_obj_align(cb_devEcoEnable_devThermostat, labelTempInstCurrent_devThermostat, LV_ALIGN_OUT_LEFT_BOTTOM, 30, -5);
	lv_cb_set_style(cb_devEcoEnable_devThermostat, LV_CB_STYLE_BG, styleCb_devThermostat_EcoEnText);
	lv_style_copy(styleCb_devThermostat_EcoEnBoxRel, lv_cb_get_style(cb_devEcoEnable_devThermostat, LV_CB_STYLE_BOX_REL));
	lv_style_copy(styleCb_devThermostat_EcoEnBoxTgRel, lv_cb_get_style(cb_devEcoEnable_devThermostat, LV_CB_STYLE_BOX_TGL_REL));
	lv_cb_set_style(cb_devEcoEnable_devThermostat, LV_CB_STYLE_BOX_REL, styleCb_devThermostat_EcoEnBoxRel);
	lv_cb_set_style(cb_devEcoEnable_devThermostat, LV_CB_STYLE_BOX_TGL_REL, styleCb_devThermostat_EcoEnBoxTgRel);
	lv_cb_set_text(cb_devEcoEnable_devThermostat, "ECO");
	lv_cb_set_checked(cb_devEcoEnable_devThermostat, devParam_thermostat.workModeInNight_IF);

	textBtn_tempAdjAdd_devThermostat = lv_label_create(obj_Parent, NULL);
	lv_label_set_text(textBtn_tempAdjAdd_devThermostat, "+");
	lv_label_set_style(textBtn_tempAdjAdd_devThermostat, styleTextBtnBk_devThermostat_tempAdj);
	lv_obj_set_protect(textBtn_tempAdjAdd_devThermostat, LV_PROTECT_POS);
	lv_obj_align(textBtn_tempAdjAdd_devThermostat, btn_tempAdjAdd_devThermostat, LV_ALIGN_CENTER, 0, 0);
	textBtn_tempAdjCut_devThermostat = lv_label_create(obj_Parent, NULL);
	lv_label_set_text(textBtn_tempAdjCut_devThermostat, "_");	
	lv_label_set_style(textBtn_tempAdjCut_devThermostat, styleTextBtnBk_devThermostat_tempAdj);
	lv_obj_set_protect(textBtn_tempAdjCut_devThermostat, LV_PROTECT_POS);
	lv_obj_align(textBtn_tempAdjCut_devThermostat, btn_tempAdjCut_devThermostat, LV_ALIGN_CENTER, 0, -15);

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)
	(thermostatRunningStatus_mode_stop == devParam_thermostat.deviceRunning_MODE)?
#else
	(0 == devParam_thermostat.deviceRunning_EN)?
#endif
		(usrApp_devThermostat_ctrlObj_reserveSet(false)):
		(usrApp_devThermostat_ctrlObj_reserveSet(true));
}

static void local_guiHomeBussiness_thermostatExtension(lv_obj_t * obj_Parent){

	stt_dataDisp_guiBussinessHome_btnText dataTextObjDisp_temp = {0};
	uint8_t dataIconObjDisp_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {0};
	stt_devDataPonitTypedef devDataPoint = {0};
	uint8_t devThermostatExSwStatus = devDriverBussiness_thermostatSwitch_exSwitchParamGet();
	
	currentDev_dataPointGet(&devDataPoint);	
	usrAppHomepageBtnTextDisp_paramGet(&dataTextObjDisp_temp);
	usrAppHomepageBtnIconNumDisp_paramGet(dataIconObjDisp_temp);

	styleBtn_Text->text.font = usrAppHomepageBtnBkText_fontGet(dataTextObjDisp_temp.countryFlg);
	styleIconvText_devMulitSw_statusOn->text.font = styleBtn_Text->text.font;
	styleIconvText_devMulitSw_statusOff->text.font = styleBtn_Text->text.font;

	//home界面开关按键创建，并设置底图、位置、及加载动画
	btn_bk_thermostatEx_fold = lv_imgbtn_create(obj_Parent, NULL);
//	lv_imgbtn_set_src(btn_bk_thermostatEx_fold, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false));
	lv_imgbtn_set_src(btn_bk_thermostatEx_fold, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_thermostatEx_fold, 20,  30)):
		(lv_obj_set_pos(btn_bk_thermostatEx_fold,  5,  41));
	btn_bk_devMulitSw_A = lv_imgbtn_create(obj_Parent, btn_bk_thermostatEx_fold);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_A, 20, 100)):
		(lv_obj_set_pos(btn_bk_devMulitSw_A,  5, 134));
	btn_bk_devMulitSw_B = lv_imgbtn_create(obj_Parent, btn_bk_thermostatEx_fold);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_B, 20, 170)):
		(lv_obj_set_pos(btn_bk_devMulitSw_B,  5, 227));
	lv_obj_animate(btn_bk_thermostatEx_fold, LV_ANIM_FLOAT_RIGHT, 200,	0, NULL);
	lv_obj_animate(btn_bk_devMulitSw_A, LV_ANIM_FLOAT_RIGHT, 200,  50, NULL);
	lv_obj_animate(btn_bk_devMulitSw_B, LV_ANIM_FLOAT_RIGHT, 200, 100, NULL);
	//home界面开关按键回调创建
	lv_btn_set_action(btn_bk_thermostatEx_fold, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devMulitSw_mainBtnA);
	lv_btn_set_action(btn_bk_devMulitSw_A, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devMulitSw_mainBtnB);
	lv_btn_set_action(btn_bk_devMulitSw_B, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devMulitSw_mainBtnC);
	
	//home界面开关按键说明文字创建，并设置位置
	iconBtn_thermostatEx_fold = lv_img_create(btn_bk_thermostatEx_fold, NULL);
	iconBtn_meeting = lv_img_create(btn_bk_devMulitSw_A, NULL);
	iconBtn_sleeping = lv_img_create(btn_bk_devMulitSw_B, NULL);
	lv_img_set_src(iconBtn_thermostatEx_fold, &specifyIcon_thermostat);
	lv_img_set_src(iconBtn_meeting, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[1]));
	lv_img_set_src(iconBtn_sleeping, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[2]));
	lv_obj_set_protect(iconBtn_thermostatEx_fold, LV_PROTECT_POS);
	lv_obj_set_protect(iconBtn_meeting, LV_PROTECT_POS);
	lv_obj_set_protect(iconBtn_sleeping, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_thermostatEx_fold, NULL, LV_ALIGN_OUT_LEFT_MID, 78, -3)):
		(lv_obj_align(iconBtn_thermostatEx_fold, NULL, LV_ALIGN_OUT_LEFT_MID, 65, 15));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_meeting, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_meeting, NULL, LV_ALIGN_OUT_LEFT_MID, 65, 15));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_sleeping, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_sleeping, NULL, LV_ALIGN_OUT_LEFT_MID, 65, 15));
	
	//home界面开关按键说明文字风格加载
	textBtn_thermostatExFold_temp = lv_label_create(btn_bk_thermostatEx_fold, NULL);
	textBtn_meeting = lv_label_create(btn_bk_devMulitSw_A, NULL);
	textBtn_sleeping = lv_label_create(btn_bk_devMulitSw_B, NULL);
	lv_label_set_align(textBtn_thermostatExFold_temp, LV_LABEL_ALIGN_CENTER);
	lv_label_set_align(textBtn_meeting, LV_LABEL_ALIGN_CENTER);
	lv_label_set_align(textBtn_sleeping, LV_LABEL_ALIGN_CENTER);
	lv_label_set_long_mode(textBtn_thermostatExFold_temp, LV_LABEL_LONG_ROLL);
	lv_label_set_long_mode(textBtn_meeting, LV_LABEL_LONG_DOT);
	lv_label_set_long_mode(textBtn_sleeping, LV_LABEL_LONG_DOT);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_thermostatExFold_temp, 168, 25)):
		(lv_obj_set_size(textBtn_thermostatExFold_temp, 128, 25));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_meeting, 168, 25)):
		(lv_obj_set_size(textBtn_meeting, 128, 25));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_sleeping, 168, 25)):
		(lv_obj_set_size(textBtn_sleeping, 128, 25));
	lv_label_set_text(textBtn_thermostatExFold_temp, (const char*)textDispTemp_thermostatExBtnFold_temp);
	lv_label_set_text(textBtn_meeting, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[1]);
	lv_label_set_text(textBtn_sleeping, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[2]);
	lv_obj_set_protect(textBtn_thermostatExFold_temp, LV_PROTECT_POS);
	lv_obj_set_protect(textBtn_meeting, LV_PROTECT_POS);
	lv_obj_set_protect(textBtn_sleeping, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_thermostatExFold_temp, NULL, LV_ALIGN_IN_LEFT_MID, 45, 7)):
		(lv_obj_align(textBtn_thermostatExFold_temp, NULL, LV_ALIGN_IN_LEFT_MID, 50, 27));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_meeting, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_meeting, NULL, LV_ALIGN_IN_LEFT_MID, 71, 20));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_sleeping, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_sleeping, NULL, LV_ALIGN_IN_LEFT_MID, 71, 20));

	lv_label_set_recolor(textBtn_thermostatExFold_temp, true);
	textBtn_thermostatExFold_eco = lv_label_create(btn_bk_thermostatEx_fold, textBtn_thermostatExFold_temp);
	lv_obj_align(textBtn_thermostatExFold_eco, textBtn_thermostatExFold_temp, LV_ALIGN_IN_RIGHT_MID, 75, 0);

	lv_imgbtn_set_src(btn_bk_thermostatEx_fold, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true));
//	lv_img_set_style(iconBtn_thermostatEx_fold, styleIconvText_devMulitSw_statusOn);
	lv_label_set_style(textBtn_thermostatExFold_temp, styleText_thermostatEx_btnFold);
	lv_label_set_style(textBtn_thermostatExFold_eco, styleText_thermostatEx_btnFold);

	(devThermostatExSwStatus & (1 << 0))?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
	(devThermostatExSwStatus & (1 << 0))?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
	(devThermostatExSwStatus & (1 << 0))?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
	(devThermostatExSwStatus & (1 << 1))?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
	(devThermostatExSwStatus & (1 << 1))?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
	(devThermostatExSwStatus & (1 << 1))?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));

	dispFlg_devThermostat_firstPage = true;
}

static void local_guiHomeBussiness_scenario(lv_obj_t * obj_Parent){

	stt_dataDisp_guiBussinessHome_btnText dataTextObjDisp_temp = {0};
	uint8_t dataIconObjDisp_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {0};
	stt_devDataPonitTypedef devDataPoint = {0};
	
	currentDev_dataPointGet(&devDataPoint);	
	usrAppHomepageBtnTextDisp_paramGet(&dataTextObjDisp_temp);
	usrAppHomepageBtnIconNumDisp_paramGet(dataIconObjDisp_temp);

	styleBtn_Text->text.font = usrAppHomepageBtnBkText_fontGet(dataTextObjDisp_temp.countryFlg);
	styleIconvText_devMulitSw_statusOn->text.font = styleBtn_Text->text.font;
	styleIconvText_devMulitSw_statusOff->text.font = styleBtn_Text->text.font;

	//home界面开关按键创建，并设置底图、位置、及加载动画
	btn_bk_devMulitSw_A = lv_imgbtn_create(obj_Parent, NULL);
	lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, usrAppHomepageBtnBkPic_dataGet(false));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_A, 20,  30)):
		(lv_obj_set_pos(btn_bk_devMulitSw_A,  5,  41));
	btn_bk_devMulitSw_B = lv_imgbtn_create(obj_Parent, btn_bk_devMulitSw_A);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_B, 20, 100)):
		(lv_obj_set_pos(btn_bk_devMulitSw_B,  5, 134));
	btn_bk_devMulitSw_C = lv_imgbtn_create(obj_Parent, btn_bk_devMulitSw_A);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devMulitSw_C, 20, 170)):
		(lv_obj_set_pos(btn_bk_devMulitSw_C,  5, 227));
	lv_obj_animate(btn_bk_devMulitSw_A, LV_ANIM_FLOAT_RIGHT, 200,	 0, NULL);
	lv_obj_animate(btn_bk_devMulitSw_B, LV_ANIM_FLOAT_RIGHT, 200,	50, NULL);
	lv_obj_animate(btn_bk_devMulitSw_C, LV_ANIM_FLOAT_RIGHT, 200,  100, NULL);
//	lv_imgbtn_set_style(btn_bk_devMulitSw_A, LV_BTN_STATE_PR, styleBtn_devMulitSw_statusOn);
//	lv_imgbtn_set_style(btn_bk_devMulitSw_B, LV_BTN_STATE_PR, styleBtn_devMulitSw_statusOn);
//	lv_imgbtn_set_style(btn_bk_devMulitSw_C, LV_BTN_STATE_PR, styleBtn_devMulitSw_statusOn);
	//home界面开关按键回调创建
	lv_btn_set_action(btn_bk_devMulitSw_A, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devMulitSw_mainBtnA);
	lv_btn_set_action(btn_bk_devMulitSw_B, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devMulitSw_mainBtnB);
	lv_btn_set_action(btn_bk_devMulitSw_C, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devMulitSw_mainBtnC);
	//home界面开关按键说明文字创建，并设置位置
	iconBtn_meeting = lv_img_create(btn_bk_devMulitSw_A, NULL);
	iconBtn_sleeping = lv_img_create(btn_bk_devMulitSw_B, NULL);
	iconBtn_toilet = lv_img_create(btn_bk_devMulitSw_C, NULL);
	lv_img_set_src(iconBtn_meeting, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[0]));
	lv_img_set_src(iconBtn_sleeping, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[1]));
	lv_img_set_src(iconBtn_toilet, usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[2]));
	lv_obj_set_protect(iconBtn_meeting, LV_PROTECT_POS);
	lv_obj_set_protect(iconBtn_sleeping, LV_PROTECT_POS);
	lv_obj_set_protect(iconBtn_toilet, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_meeting, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_meeting, NULL, LV_ALIGN_IN_TOP_RIGHT, 35, 20));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_sleeping, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_sleeping, NULL, LV_ALIGN_IN_TOP_RIGHT, 35, 20));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(iconBtn_toilet, NULL, LV_ALIGN_OUT_LEFT_MID, 65, -2)):
		(lv_obj_align(iconBtn_toilet, NULL, LV_ALIGN_IN_TOP_RIGHT, 35, 20));

	//home界面开关按键说明文字风格加载
	textBtn_meeting = lv_label_create(btn_bk_devMulitSw_A, NULL);
	textBtn_sleeping = lv_label_create(btn_bk_devMulitSw_B, NULL);
	textBtn_toilet = lv_label_create(btn_bk_devMulitSw_C, NULL);
	lv_label_set_align(textBtn_meeting, LV_LABEL_ALIGN_CENTER);
	lv_label_set_align(textBtn_sleeping, LV_LABEL_ALIGN_CENTER);
	lv_label_set_align(textBtn_toilet, LV_LABEL_ALIGN_CENTER);
	lv_label_set_long_mode(textBtn_meeting, LV_LABEL_LONG_ROLL);
	lv_label_set_long_mode(textBtn_sleeping, LV_LABEL_LONG_DOT);
	lv_label_set_long_mode(textBtn_toilet, LV_LABEL_LONG_DOT);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_meeting, 168, 25)):
		(lv_obj_set_size(textBtn_meeting, 140, 25));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_sleeping, 168, 25)):
		(lv_obj_set_size(textBtn_sleeping, 140, 25));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(textBtn_toilet, 168, 25)):
		(lv_obj_set_size(textBtn_toilet, 140, 25));

	lv_label_set_text(textBtn_meeting, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[0]);
	lv_label_set_text(textBtn_sleeping, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[1]);
	lv_label_set_text(textBtn_toilet, (const char*)dataTextObjDisp_temp.dataBtnTextDisp[2]);
	lv_obj_set_protect(textBtn_meeting, LV_PROTECT_POS);
	lv_obj_set_protect(textBtn_sleeping, LV_PROTECT_POS);
	lv_obj_set_protect(textBtn_toilet, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_meeting, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_meeting, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 85, 15));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_sleeping, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_sleeping, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 85, 15));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(textBtn_toilet, NULL, LV_ALIGN_IN_LEFT_MID, 75, 5)):
		(lv_obj_align(textBtn_toilet, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 85, 15));

	(1 == devDataPoint.devType_scenario.devScenario_opNum)?(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_A, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
	(1 == devDataPoint.devType_scenario.devScenario_opNum)?(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_meeting, styleIconvText_devMulitSw_statusOff));
	(1 == devDataPoint.devType_scenario.devScenario_opNum)?(lv_label_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_meeting, styleIconvText_devMulitSw_statusOff));
	(2 == devDataPoint.devType_scenario.devScenario_opNum)?(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_B, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
	(2 == devDataPoint.devType_scenario.devScenario_opNum)?(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_sleeping, styleIconvText_devMulitSw_statusOff));
	(2 == devDataPoint.devType_scenario.devScenario_opNum)?(lv_label_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_sleeping, styleIconvText_devMulitSw_statusOff));
	(4 == devDataPoint.devType_scenario.devScenario_opNum)?(lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(true))):(lv_imgbtn_set_src(btn_bk_devMulitSw_C, LV_BTN_STATE_REL, usrAppHomepageBtnBkPic_dataGet(false)));
	(4 == devDataPoint.devType_scenario.devScenario_opNum)?(lv_img_set_style(iconBtn_toilet, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(iconBtn_toilet, styleIconvText_devMulitSw_statusOff));
	(4 == devDataPoint.devType_scenario.devScenario_opNum)?(lv_label_set_style(textBtn_toilet, styleIconvText_devMulitSw_statusOn)):(lv_img_set_style(textBtn_toilet, styleIconvText_devMulitSw_statusOff));
}

static void local_guiHomeBussiness_curtain(lv_obj_t * obj_Parent){

	uint8_t devCurtain_orbitalPosPercent = devCurtain_currentPositionPercentGet();

	image_bk_devCurtain_body = lv_img_create(obj_Parent, NULL);
	lv_img_set_src(image_bk_devCurtain_body, &imageCurtain_body);
	lv_img_set_style(image_bk_devCurtain_body, styleImageBk_devCurtain_bkImgBody);
	lv_obj_set_protect(image_bk_devCurtain_body, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(image_bk_devCurtain_body, 35, 40)):
		(lv_obj_set_pos(image_bk_devCurtain_body, 34, 47));

	slider_bk_devCurtain = lv_slider_create(obj_Parent, NULL);
	lv_obj_set_size(slider_bk_devCurtain, 190, 30);
	lv_obj_set_protect(slider_bk_devCurtain, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(slider_bk_devCurtain, 26, 205)):
		(lv_obj_set_pos(slider_bk_devCurtain, 25, 235));
	lv_obj_set_top(slider_bk_devCurtain, true);
	lv_slider_set_action(slider_bk_devCurtain, funCb_slidAction_devCurtain_mainSlider);
	lv_bar_set_value(slider_bk_devCurtain, DEVICE_CURTAIN_ORBITAL_POSITION_MAX_VAL);
	lv_slider_set_value(slider_bk_devCurtain, devCurtain_orbitalPosPercent);

	lv_slider_set_style(slider_bk_devCurtain, LV_SLIDER_STYLE_BG, styleSliderBk_devCurtain_bg);
	lv_slider_set_style(slider_bk_devCurtain, LV_SLIDER_STYLE_INDIC, styleSliderBk_devCurtain_indic);
	lv_slider_set_style(slider_bk_devCurtain, LV_SLIDER_STYLE_KNOB, styleSliderBk_devCurtain_knob);

	btn_bk_devCurtain_open = lv_imgbtn_create(obj_Parent, NULL);
	lv_imgbtn_set_src(btn_bk_devCurtain_open, LV_BTN_STATE_REL, &iconPage_curtainOpen_rel);
	lv_imgbtn_set_src(btn_bk_devCurtain_open, LV_BTN_STATE_PR, &iconPage_curtainOpen_rel);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devCurtain_open, 240, 155)):
		(lv_obj_set_pos(btn_bk_devCurtain_open, 25, 280));

	lv_obj_set_top(btn_bk_devCurtain_open, true);
	lv_obj_animate(btn_bk_devCurtain_open, LV_ANIM_FLOAT_RIGHT, 200, 0, NULL);
	lv_imgbtn_set_style(btn_bk_devCurtain_open, LV_BTN_STATE_REL, styleBtn_devCurtain_statusRel);
	lv_btn_set_action(btn_bk_devCurtain_open, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devCurtain_open);

	btn_bk_devCurtain_stop = lv_imgbtn_create(obj_Parent, NULL);
	lv_imgbtn_set_src(btn_bk_devCurtain_stop, LV_BTN_STATE_REL, &iconPage_curtainPluse_rel);
	lv_imgbtn_set_src(btn_bk_devCurtain_stop, LV_BTN_STATE_PR, &iconPage_curtainPluse_rel);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devCurtain_stop, 251, 100)):
		(lv_obj_set_pos(btn_bk_devCurtain_stop, 105, 280));
	lv_obj_set_top(btn_bk_devCurtain_stop, true);
	lv_obj_animate(btn_bk_devCurtain_stop, LV_ANIM_FLOAT_RIGHT, 200, 0, NULL);
	lv_imgbtn_set_style(btn_bk_devCurtain_stop, LV_BTN_STATE_REL, styleBtn_devCurtain_statusRel);
	lv_btn_set_action(btn_bk_devCurtain_stop, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devCurtain_stop);

	btn_bk_devCurtain_close = lv_imgbtn_create(obj_Parent, NULL);
	lv_imgbtn_set_src(btn_bk_devCurtain_close, LV_BTN_STATE_REL, &iconPage_curtainClose_rel);
	lv_imgbtn_set_src(btn_bk_devCurtain_close, LV_BTN_STATE_PR, &iconPage_curtainClose_rel);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_bk_devCurtain_close, 240, 45)):
		(lv_obj_set_pos(btn_bk_devCurtain_close, 165, 280));

	lv_obj_set_top(btn_bk_devCurtain_close, true);
	lv_obj_animate(btn_bk_devCurtain_close, LV_ANIM_FLOAT_RIGHT, 200, 0, NULL);
	lv_imgbtn_set_style(btn_bk_devCurtain_close, LV_BTN_STATE_REL, styleBtn_devCurtain_statusRel);
	lv_btn_set_action(btn_bk_devCurtain_close, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devCurtain_close);

	label_bk_devCurtain_positionCur = lv_label_create(obj_Parent, NULL);
	lv_label_set_style(label_bk_devCurtain_positionCur, styleText_devCurtain_Bk_positionCurr);
	lv_label_set_align(label_bk_devCurtain_positionCur, LV_LABEL_ALIGN_CENTER);
	lv_label_set_long_mode(label_bk_devCurtain_positionCur, LV_LABEL_LONG_SCROLL);
	sprintf(str_devParamPositionCur_devCurtain, "%d", devCurtain_orbitalPosPercent);
	lv_label_set_text(label_bk_devCurtain_positionCur, str_devParamPositionCur_devCurtain);
	lv_obj_set_protect(label_bk_devCurtain_positionCur, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(label_bk_devCurtain_positionCur, 101, 154)):
		(lv_obj_set_pos(label_bk_devCurtain_positionCur, 100, 164));
//	lv_obj_align(label_bk_devCurtain_positionCur, image_bk_devCurtain_body, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
	lv_obj_set_top(label_bk_devCurtain_positionCur, true);

	label_bk_devCurtain_positionAdj = lv_label_create(obj_Parent, NULL);
	lv_label_set_style(label_bk_devCurtain_positionAdj, styleText_devCurtain_Bk_positionTips);
	lv_label_set_long_mode(label_bk_devCurtain_positionAdj, LV_LABEL_LONG_SCROLL);
	sprintf(str_devParamPositionAdj_devCurtain, "%d%%", devCurtain_orbitalPosPercent);
	lv_label_set_text(label_bk_devCurtain_positionAdj, str_devParamPositionAdj_devCurtain);
	lv_obj_set_protect(label_bk_devCurtain_positionAdj, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(label_bk_devCurtain_positionAdj, slider_bk_devCurtain, LV_ALIGN_OUT_RIGHT_MID, 30, -3)):
		(lv_obj_align(label_bk_devCurtain_positionAdj, slider_bk_devCurtain, LV_ALIGN_OUT_TOP_RIGHT, -10, 0));
	lv_obj_set_top(label_bk_devCurtain_positionAdj, true);
}

static void local_guiHomeBussiness_heater(lv_obj_t * obj_Parent){

	stt_devDataPonitTypedef devDataPoint = {0};
	
	currentDev_dataPointGet(&devDataPoint);

	icomImageA_devHeater = lv_img_create(obj_Parent, NULL);
	lv_img_set_src(icomImageA_devHeater, &iconHeater_HomePageDeviceHeater);
	lv_img_set_style(icomImageA_devHeater, styleImage_devHeater_icon);
	lv_obj_set_protect(icomImageA_devHeater, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(icomImageA_devHeater, 65, 35)):
		(lv_obj_set_pos(icomImageA_devHeater, 19, 60));

	textTimeInstract_target_devHeater = lv_label_create(obj_Parent, NULL);
	lv_label_set_text(textTimeInstract_target_devHeater, "\0");
	lv_label_set_style(textTimeInstract_target_devHeater, styleText_devHeater_timeInstTar);
	lv_obj_set_protect(textTimeInstract_target_devHeater, LV_PROTECT_POS);
	lv_obj_align(textTimeInstract_target_devHeater, icomImageA_devHeater, LV_ALIGN_CENTER, -60, -20);
	textTimeInstract_current_devHeater = lv_label_create(obj_Parent, NULL);
	lv_label_set_text(textTimeInstract_current_devHeater, "CLOSE");
	lv_label_set_style(textTimeInstract_current_devHeater, styleText_devHeater_timeInstCur);
	lv_obj_set_protect(textTimeInstract_current_devHeater, LV_PROTECT_POS);
	lv_obj_align(textTimeInstract_current_devHeater, icomImageA_devHeater, LV_ALIGN_CENTER, -35, 0);

	btnm_bk_devHeater = lv_btnm_create(obj_Parent, NULL);
	lv_btnm_set_style(btnm_bk_devHeater, LV_BTNM_STYLE_BG, styleBtnm_devHeater_btnBg);
	lv_btnm_set_style(btnm_bk_devHeater, LV_BTNM_STYLE_BTN_REL, styleBtnm_devHeater_btnRel);
	lv_btnm_set_style(btnm_bk_devHeater, LV_BTNM_STYLE_BTN_PR, styleBtnm_devHeater_btnPre);
	lv_btnm_set_style(btnm_bk_devHeater, LV_BTNM_STYLE_BTN_TGL_REL, styleBtnm_devHeater_btnTglRel);
	lv_btnm_set_style(btnm_bk_devHeater, LV_BTNM_STYLE_BTN_TGL_PR, styleBtnm_devHeater_btnTglPre);
	lv_btnm_set_style(btnm_bk_devHeater, LV_BTNM_STYLE_BTN_INA, styleBtnm_devHeater_btnIna);
	lv_btnm_set_action(btnm_bk_devHeater, funCb_btnmActionClick_devHeater_gearBtnm);
	lv_btnm_set_toggle(btnm_bk_devHeater, true, 0);
	lv_obj_set_protect(btnm_bk_devHeater, LV_PROTECT_POS);
	if(devStatusDispMethod_landscapeIf_get()){

		lv_obj_set_size(btnm_bk_devHeater, 300, 70);
		lv_btnm_set_map(btnm_bk_devHeater, btnm_str_devHeater_Y);
		lv_obj_set_pos(btnm_bk_devHeater, 10, 165);

	}else{

		lv_obj_set_size(btnm_bk_devHeater, 210, 120);
		lv_btnm_set_map(btnm_bk_devHeater, btnm_str_devHeater_X);
		lv_obj_set_pos(btnm_bk_devHeater, 15, 195);
	}

	if(devDataPoint.devType_heater.devHeater_swEnumVal == heaterOpreatAct_closeAfterTimeCustom){

		imgBtn_devHeater_timeSet = lv_imgbtn_create(obj_Parent, NULL);
		lv_obj_set_protect(imgBtn_devHeater_timeSet, LV_PROTECT_POS);
//		lv_obj_align(imgBtn_devHeater_timeSet, btnm_bk_devHeater, LV_ALIGN_OUT_TOP_RIGHT, 60, 32);
		(devStatusDispMethod_landscapeIf_get())?
			(lv_obj_set_pos(imgBtn_devHeater_timeSet, 259, 130)):
			(lv_obj_set_pos(imgBtn_devHeater_timeSet, 182, 155));		
		lv_imgbtn_set_src(imgBtn_devHeater_timeSet, LV_BTN_STATE_REL, &iconSet_HomePageDeviceHeater);
		lv_imgbtn_set_src(imgBtn_devHeater_timeSet, LV_BTN_STATE_PR, &iconSet_HomePageDeviceHeater);
		lv_imgbtn_set_action(imgBtn_devHeater_timeSet, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devHeater_timeSet);
	}
}

static void guiBussiness_tipsLoopTimerCreat(const char *strTips){

#if(SCREENSAVER_RUNNING_ENABLE == 1)
	
	if(screensaverDispAttrParam.flg_screensaverPageCreat == 1){

		return; //锁屏界面不做提示
	}
#endif

	if(!trigFlg_loopTimerTips){

		trigFlg_loopTimerTips = true;

		usr_loopTimer_tipsKeeper_trig();

		icon_loopTimerTips = lv_img_create(lv_scr_act(), NULL);
		lv_img_set_src(icon_loopTimerTips, &iconAlarm_tips);
		(devStatusDispMethod_landscapeIf_get())?
			(lv_obj_set_pos(icon_loopTimerTips, 85, 45)):
			(lv_obj_set_pos(icon_loopTimerTips, 45, 85));

		text_loopTimerTips = lv_label_create(icon_loopTimerTips, NULL);
		lv_obj_set_style(text_loopTimerTips, styleText_loopTimerTips);
		lv_label_set_text(text_loopTimerTips, strTips);
		lv_obj_set_protect(text_loopTimerTips, LV_PROTECT_POS);
		lv_obj_align(text_loopTimerTips, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

		lv_obj_animate(icon_loopTimerTips, LV_ANIM_FLOAT_BOTTOM, 200, 0, NULL);
	}
}

static void guiBussiness_tipsSystemRestartCreat(const char *tipsDelayCounter){

	lv_indev_enable(LV_INDEV_TYPE_POINTER, false); //界面阻塞，禁止控件触发

	if(page_sysRestartTips == NULL){

		page_sysRestartTips = lv_page_create(lv_scr_act(), NULL);
		lv_obj_set_top(page_sysRestartTips, true);
		lv_obj_set_size(page_sysRestartTips, 120, 120);
		lv_obj_set_protect(page_sysRestartTips, LV_PROTECT_POS);
		lv_obj_align(page_sysRestartTips, NULL, LV_ALIGN_CENTER, 0, 0);
		lv_page_set_style(page_sysRestartTips, LV_PAGE_STYLE_SB, stylePage_sysRestartTips);
		lv_page_set_style(page_sysRestartTips, LV_PAGE_STYLE_BG, stylePage_sysRestartTips);
		lv_page_set_sb_mode(page_sysRestartTips, LV_SB_MODE_HIDE);	
		lv_page_set_scrl_fit(page_sysRestartTips, false, true); //key opration
		lv_page_set_scrl_layout(page_sysRestartTips, LV_LAYOUT_PRETTY);

		label_sysRestartTips_Counter = lv_label_create(page_sysRestartTips, NULL);
		lv_label_set_style(label_sysRestartTips_Counter, styleLabelCounter_sysRestartTips);
		lv_obj_set_protect(label_sysRestartTips_Counter, LV_PROTECT_POS);
		lv_obj_align(label_sysRestartTips_Counter, page_sysRestartTips, LV_ALIGN_CENTER, -10, 0);
		label_sysRestartTips_ref = lv_label_create(page_sysRestartTips, NULL);
		lv_label_set_style(label_sysRestartTips_ref, styleLabelRef_sysRestartTips);
		lv_obj_set_protect(label_sysRestartTips_ref, LV_PROTECT_POS);
		lv_obj_align(label_sysRestartTips_ref, page_sysRestartTips, LV_ALIGN_IN_BOTTOM_MID, -28, -5);
	}

	lv_label_set_text(label_sysRestartTips_Counter, tipsDelayCounter);
	lv_label_set_text(label_sysRestartTips_ref, "sys restart");

	lv_obj_refresh_style(page_sysRestartTips);

	usrApp_fullScreenRefresh_self(50, 0); 
}

static void guiBussiness_guiBlockTipsCreat(void){

#if(SCREENSAVER_RUNNING_ENABLE == 1)
		
	if(screensaverDispAttrParam.flg_screensaverPageCreat == 1){

		return; //锁屏界面不做提示
	}
#endif

	lv_indev_enable(LV_INDEV_TYPE_POINTER, false); //界面阻塞，禁止控件触发

	if(NULL == page_guiBlockTips){

		page_guiBlockTips = lv_page_create(lv_layer_top(), NULL);
		lv_obj_set_size(page_guiBlockTips, 200, 60);
		lv_obj_set_protect(page_guiBlockTips, LV_PROTECT_POS);
		lv_obj_align(page_guiBlockTips, NULL, LV_ALIGN_CENTER, 0, -20);
		lv_obj_set_top(page_guiBlockTips, true);
		lv_page_set_style(page_guiBlockTips, LV_PAGE_STYLE_SB, stylePage_guiBlockTips);
		lv_page_set_style(page_guiBlockTips, LV_PAGE_STYLE_BG, stylePage_guiBlockTips);
		lv_page_set_sb_mode(page_guiBlockTips, LV_SB_MODE_HIDE);	
	//	lv_page_set_scrl_width(page_guiBlockTips, 180);
		lv_page_set_scrl_fit(page_guiBlockTips, false, false); //key opration
		lv_page_set_scrl_layout(page_guiBlockTips, LV_LAYOUT_PRETTY);		

		iconPage_guiBlockTips = lv_img_create(page_guiBlockTips, NULL);
		lv_img_set_src(iconPage_guiBlockTips, &iconUiBlock_preload);
		lv_obj_set_protect(iconPage_guiBlockTips, LV_PROTECT_POS);
		lv_obj_align(iconPage_guiBlockTips, NULL, LV_ALIGN_IN_LEFT_MID, 5, -3);

		labelPage_guiBlockTips = lv_label_create(page_guiBlockTips, NULL);
		lv_obj_set_protect(labelPage_guiBlockTips, LV_PROTECT_POS);
		lv_obj_set_protect(labelPage_guiBlockTips, LV_PROTECT_FOLLOW);
		lv_obj_set_size(labelPage_guiBlockTips, 125, 40);
		lv_label_set_recolor(labelPage_guiBlockTips, true);
		lv_label_set_align(labelPage_guiBlockTips, LV_LABEL_ALIGN_CENTER);
		lv_label_set_long_mode(labelPage_guiBlockTips, LV_LABEL_LONG_SCROLL);
		lv_label_set_style(labelPage_guiBlockTips, styleLabelRef_guiBlockTips);
		lv_obj_align(labelPage_guiBlockTips, iconPage_guiBlockTips, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, -20);
	}

	switch(icon_pageGuiBlockType){
	
		case 1:{
	
			lv_img_set_src(iconPage_guiBlockTips, &iconUiBlock_warning);
	
		}break;
	
		case 2:{
	
			lv_img_set_src(iconPage_guiBlockTips, &iconUiBlock_success);
	
		}break;
	
		default:{
	
			lv_img_set_src(iconPage_guiBlockTips, &iconUiBlock_preload);
	
		}break;
	}

	lv_label_set_text(labelPage_guiBlockTips, (const char *)str_pageGuiBlockTips);

	usrApp_fullScreenRefresh_self(50, 80); 
}

static void guiBussiness_fullScreenTipsCreat(void){

	if(!screenFullReminder_flg){

		screenFullReminder_flg = true;

		pageReminder_screenFull = lv_obj_create(lv_layer_top(), NULL);

		(devStatusDispMethod_landscapeIf_get())?
			(lv_obj_set_size(pageReminder_screenFull, 320, 240)):
			(lv_obj_set_size(pageReminder_screenFull, 240, 320));

		labelReminder_screenFull = lv_label_create(pageReminder_screenFull, NULL);
		lv_label_set_recolor(labelReminder_screenFull, true);
		lv_label_set_align(labelReminder_screenFull, LV_LABEL_ALIGN_CENTER);
		lv_obj_align(labelReminder_screenFull, NULL, LV_ALIGN_CENTER, -75, 0);
		lv_obj_set_top(pageReminder_screenFull, true);
	}

	lv_label_set_text(labelReminder_screenFull, str_tipsFullScreen);
}

static void guiBussiness_guiBlockTipsDelete(void){

	if(page_guiBlockTips){

		lv_obj_del(page_guiBlockTips);
		page_guiBlockTips = NULL;		
	}

	lv_indev_enable(LV_INDEV_TYPE_POINTER, true); //加载等待结束，恢复触摸
}

static void guiBussiness_fullScreenTipsDelete(void){

	if(screenFullReminder_flg){

		lv_obj_del(pageReminder_screenFull);

		screenFullReminder_flg = false;
	}
}

static void guiBussiness_tipsLoopTimerDelete(void){

	if(!usr_loopTimer_tipsKeeper_read()){
		
		if(icon_loopTimerTips){
		
			lv_obj_del(icon_loopTimerTips);
			trigFlg_loopTimerTips = false;

			usrApp_fullScreenRefresh_self(50, 0); //刷新
		}
	}
}

static const struct stt_guiHomeBussiness{

	guiHomeFun_local_bussiness guiHome_disp;
	devTypeDef_enum currentDevType;

}guiHomeBussiness_tab[DEVICE_TYPE_LIST_NUM] = {

	{local_guiHomeBussiness_mulitSwOneBit, 			devTypeDef_mulitSwOneBit},
	{local_guiHomeBussiness_mulitSwTwoBit, 			devTypeDef_mulitSwTwoBit},
	{local_guiHomeBussiness_mulitSwThreeBit,		devTypeDef_mulitSwThreeBit},
	{local_guiHomeBussiness_dimmer,					devTypeDef_dimmer},
	{local_guiHomeBussiness_fans, 					devTypeDef_fans},	
	{local_guiHomeBussiness_scenario, 				devTypeDef_scenario},
	{local_guiHomeBussiness_curtain, 				devTypeDef_curtain},
	{local_guiHomeBussiness_heater, 				devTypeDef_heater},
	{local_guiHomeBussiness_thermostat, 			devTypeDef_thermostat},
	{local_guiHomeBussiness_thermostatExtension,	devTypeDef_thermostatExtension},
};

static void lvGui_businessHome(lv_obj_t * obj_Parent){

	uint8_t loop = 0;
	devTypeDef_enum devType_temp = currentDev_typeGet();
	stt_devStatusRecord devStatusRecordFlg_temp = {0};

	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

//	//home界面进入菜单按键创建，并设置底图、位置、及加载动画
//	btn_homeMenu = lv_imgbtn_create(lv_scr_act(), NULL);
//	lv_imgbtn_set_src(btn_homeMenu, LV_BTN_STATE_REL, &iconHome_menu);
//	lv_imgbtn_set_src(btn_homeMenu, LV_BTN_STATE_PR, &iconHome_menu);
//	lv_btn_set_action(btn_homeMenu, LV_BTN_ACTION_CLICK, funCb_btnActionClick_homeMenu_click);
//	lv_btn_set_action(btn_homeMenu, LV_BTN_ACTION_LONG_PR_REPEAT, funCb_btnActionClick_homeMenu_prLoop);
//	lv_obj_set_pos(btn_homeMenu, 190, 41);
//	lv_obj_set_top(btn_homeMenu, true);
//	btn_homeMenu->drag = 1; //设置为可拖动
//	btn_homeMenu->drag_throw = 1;
//	lv_obj_animate(btn_homeMenu, LV_ANIM_FLOAT_LEFT, 300, 100, NULL);

	btn_homeMenu = lv_btn_create(lv_scr_act(), NULL);
	lv_obj_set_size(btn_homeMenu, 120, 45);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_homeMenu, -15, -20)):
		(lv_obj_set_pos(btn_homeMenu, -15, -15));
	lv_btn_set_style(btn_homeMenu, LV_BTN_STYLE_REL, &lv_style_transp);
	lv_btn_set_style(btn_homeMenu, LV_BTN_STYLE_PR, &lv_style_transp);
	lv_btn_set_style(btn_homeMenu, LV_BTN_STYLE_TGL_REL, &lv_style_transp);
	lv_btn_set_style(btn_homeMenu, LV_BTN_STYLE_TGL_PR, &lv_style_transp);
	lv_btn_set_action(btn_homeMenu, LV_BTN_ACTION_LONG_PR, funCb_btnActionClick_homeMenu_click);

	btn_unlockMenu = lv_btn_create(lv_scr_act(), btn_homeMenu);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(btn_unlockMenu, 230, -20)):
		(lv_obj_set_pos(btn_unlockMenu, 150, -15));
	lv_btn_set_action(btn_unlockMenu, LV_BTN_ACTION_LONG_PR, 		funCb_btnActionClick_homeUnlock_click);
	lv_btn_set_action(btn_unlockMenu, LV_BTN_ACTION_LONG_PR_REPEAT, funCb_btnActionClick_homeUnlock_longPre);

	switch(bGroundImg_themeParam.bGround_picOrg_ist){

		case bGroudImg_objInsert_usrPic:

		case bGroudImg_objInsert_figureA:
		case bGroudImg_objInsert_figureB:
		case bGroudImg_objInsert_figureC:
		case bGroudImg_objInsert_figureD:
		case bGroudImg_objInsert_figureE:
		case bGroudImg_objInsert_figureF:
		case bGroudImg_objInsert_figureG:
		case bGroudImg_objInsert_figureH:
		case bGroudImg_objInsert_figureI:
		case bGroudImg_objInsert_figureJ:
		case bGroudImg_objInsert_figureK:
		case bGroudImg_objInsert_figureL:
		case bGroudImg_objInsert_figureM:
		case bGroudImg_objInsert_figureN:
		case bGroudImg_objInsert_figureO:
		case bGroudImg_objInsert_figureP:
		case bGroudImg_objInsert_figureQ:
		case bGroudImg_objInsert_figureR:
		case bGroudImg_objInsert_figureS:
		case bGroudImg_objInsert_figureT:{

			switch(devType_temp){

				case devTypeDef_mulitSwOneBit:
				case devTypeDef_mulitSwTwoBit:
				case devTypeDef_mulitSwThreeBit:
				case devTypeDef_scenario:{

					styleImgBk_underlying->image.intense = 10;
					styleImgBk_underlying->image.color = LV_COLOR_BLACK;
					
					lv_img_set_style(imageBK, styleImgBk_underlying);

				}break;
				
				case devTypeDef_dimmer:
				case devTypeDef_fans:
				case devTypeDef_curtain:
				case devTypeDef_thermostat:
				case devTypeDef_thermostatExtension:
				case devTypeDef_heater:{

					styleImgBk_underlying->image.intense = LV_OPA_50;
					styleImgBk_underlying->image.color = LV_COLOR_GRAY;

					lv_img_set_style(imageBK, styleImgBk_underlying);

				}break;
				
				default:{}break;
			}

		}break;

		default:break;
	}

	if(1 == devStatusRecordFlg_temp.devListManager_En){

	}else{

		for(loop = 0; loop < DEVICE_TYPE_LIST_NUM; loop ++){

			if(devType_temp == guiHomeBussiness_tab[loop].currentDevType){

				guiHomeBussiness_tab[loop].guiHome_disp(obj_Parent);
				break;
			}
		}
	}
	
//	vTaskDelay(50 / portTICK_PERIOD_MS);
	lv_obj_refresh_style(imageBK);
}

void lvglUsrApp_touchDisTrig(uint16_t timeDelay, uint16_t timeKeep){

	lv_indev_enable(LV_INDEV_TYPE_POINTER, false);

	cntrDat_scrDis.timeDelay = timeDelay;
	cntrDat_scrDis.timeKeep  = timeKeep;
	
	cntrDat_scrDis.actStatus = scrDisAct_statusDelay;
}

void usrApp_fullScreenRefresh_self(uint16_t freshTime, lv_coord_t y){

	const bool refreshEnable = false; //自刷新使能

	if(false == refreshEnable)return; //自刷新使能判断

	if(pageRefreshTrig_counter != COUNTER_DISENABLE_MASK_SPECIALVAL_U16){

//		printf("debugVal:%d.\n", pageRefreshTrig_counter);
		return;
	}

	if(NULL == screenNull_refresher)
		screenNull_refresher = lv_obj_create(lv_scr_act(), NULL);
		
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(screenNull_refresher, 320, y?y:240)):
		(lv_obj_set_size(screenNull_refresher, 240, y?y:320));
	lv_obj_set_top(screenNull_refresher, false);
	lv_obj_set_style(screenNull_refresher, &lv_style_transp);
	lv_obj_animate(screenNull_refresher, LV_ANIM_GROW_H, freshTime, 0, NULL);
	
	pageRefreshTrig_counter = freshTime;
}

static bool usrApp_temperatureCtrlObj_reserveGet(void){

	bool res = false;

	if((devTypeDef_thermostat == currentDev_typeGet()) ||
	   (devTypeDef_thermostatExtension == currentDev_typeGet())){

		res = true;
	}

	return res;
}

static void usrApp_temperatureCtrlObj_generate(void){

	if(usrApp_temperatureCtrlObj_reserveGet()){

		if(textHeaderObj_temperature == NULL){

			iconHeaderObj_tempraA = lv_img_create(lv_layer_top(), NULL);
			iconHeaderObj_tempraB = lv_img_create(lv_layer_top(), NULL);

			lv_img_set_src(iconHeaderObj_tempraA, &iconHeader_tempra_A);
			lv_img_set_src(iconHeaderObj_tempraB, &iconHeader_tempra_B);

			textHeaderObj_temperature = lv_label_create(lv_layer_top(), NULL);
			lv_label_set_text(textHeaderObj_temperature, "24.1");
			
			lv_obj_set_style(iconHeaderObj_tempraA, styleImg_iconHeater);
			lv_obj_set_style(iconHeaderObj_tempraB, styleImg_iconHeater);			
			lv_obj_set_style(textHeaderObj_temperature, styleText_temperature);		

#if(LVAPP_DISP_ELECPARAM_HIDDEN_EN != 1) //电量参数显示条件编译
			
			if(devStatusDispMethod_landscapeIf_get()){ //横屏位置处理
		
				lv_obj_set_pos(iconHeaderObj_tempraA,	  174, 6);
				lv_obj_set_pos(iconHeaderObj_tempraB,	  217, 7);
				lv_obj_set_pos(textHeaderObj_temperature, 189, 7);
			}
			else
			{
				lv_obj_set_pos(iconHeaderObj_tempraA,	  152, 23);
				lv_obj_set_pos(iconHeaderObj_tempraB,	  195, 24);
				lv_obj_set_pos(textHeaderObj_temperature, 167, 24);
			}
#else

			if(devStatusDispMethod_landscapeIf_get()){ //横屏位置处理
		
				lv_obj_set_pos(iconHeaderObj_tempraA,	  214, 6);
				lv_obj_set_pos(iconHeaderObj_tempraB,	  257, 7);
				lv_obj_set_pos(textHeaderObj_temperature, 229, 7);
			}
			else
			{
				lv_obj_set_pos(iconHeaderObj_tempraA,	  152, 6);
				lv_obj_set_pos(iconHeaderObj_tempraB,	  195, 7);
				lv_obj_set_pos(textHeaderObj_temperature, 167, 7);
			}
#endif
		}
	}
	else
	{
		if(textHeaderObj_temperature != NULL){

			lv_obj_del(iconHeaderObj_tempraA);
			lv_obj_del(iconHeaderObj_tempraB);
			lv_obj_del(textHeaderObj_temperature);

			iconHeaderObj_tempraA = NULL;
			iconHeaderObj_tempraB = NULL;
			textHeaderObj_temperature = NULL;
		}
	}
}

static lv_res_t funCb_btnNewVersionRunningConfirm_funBack(lv_obj_t *btn){

	uiBlockFlg_newVersionRunningConfirm = false;

	return LV_RES_OK;
}

static void lvglUsrApp_newVersionRunningConfirm(void){

#ifdef STR_VERSION_REALES_REFERENCE

	lv_style_t styleObj_mainCont,
			   stylePage_verNotice,
			   styleLabel_verTitle,
			   styleLabel_verDiscrib,
			   styleBtn_usrConfirmOk,
			   styleLabelBtn_usrConfirmOk;

	if(false == uiBlockFlg_newVersionRunningConfirm)return;

	xSemaphoreTake(xSph_lvglOpreat, portMAX_DELAY); // --lvgl[take up]

	lv_obj_t *mainCont = lv_obj_create(lv_scr_act(), NULL),
		 	 *pageVerNotice = lv_page_create(mainCont, NULL),
		 	 *textVerTitile = lv_label_create(pageVerNotice, NULL),
		 	 *textVerDiscrib = lv_label_create(pageVerNotice, NULL),
		 	 *btnUsrConfirm_ok = lv_btn_create(mainCont, NULL),
		 	 *LabelBtnUsrConfirm_ok = lv_label_create(btnUsrConfirm_ok, NULL);
	char verDiscrib[16] = {0};

	lv_style_copy(&styleObj_mainCont, &lv_style_plain);
	styleObj_mainCont.body.border.part = LV_BORDER_NONE;
	styleObj_mainCont.body.radius = 5;
	styleObj_mainCont.body.main_color = LV_COLOR_WHITE;
	styleObj_mainCont.body.grad_color = LV_COLOR_WHITE;
	lv_obj_set_style(mainCont, &styleObj_mainCont);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(mainCont, 320, 240)):
		(lv_obj_set_size(mainCont, 240, 320));

	lv_style_copy(&stylePage_verNotice, &lv_style_plain);
	stylePage_verNotice.body.empty = 1;
	stylePage_verNotice.body.main_color = LV_COLOR_WHITE;
	stylePage_verNotice.body.grad_color = LV_COLOR_WHITE;
	stylePage_verNotice.body.border.part = LV_BORDER_NONE;
	lv_page_set_style(pageVerNotice, LV_PAGE_STYLE_SB, &stylePage_verNotice);
	lv_page_set_style(pageVerNotice, LV_PAGE_STYLE_BG, &stylePage_verNotice);
	lv_page_set_style(pageVerNotice, LV_PAGE_STYLE_SCRL, &lv_style_transp_fit);
	lv_obj_set_protect(pageVerNotice, LV_PROTECT_POS);
	lv_obj_align(pageVerNotice, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_page_set_sb_mode(pageVerNotice, LV_SB_MODE_DRAG);
	lv_page_set_scrl_fit(pageVerNotice, true, false);
	if(devStatusDispMethod_landscapeIf_get()){

		lv_obj_set_size(pageVerNotice, 320, 205);
		lv_page_set_scrl_width(pageVerNotice, 300);
		lv_page_set_scrl_height(pageVerNotice, 400);

	}else{

		lv_obj_set_size(pageVerNotice, 240, 285);
		lv_page_set_scrl_width(pageVerNotice, 220);
		lv_page_set_scrl_height(pageVerNotice, 400);
	}

	sscanf(L8_DEVICE_VERSION_REF_DISCRIPTION, "%*[^-]-%[^\[]", verDiscrib);
	lv_style_copy(&styleLabel_verTitle, &lv_style_plain);
	styleLabel_verTitle.body.empty = 1;
	styleLabel_verTitle.text.font = &lv_font_consola_16;
	styleLabel_verTitle.text.color = LV_COLOR_MAKE(0x00, 0x80, 0xFF);
	lv_label_set_style(textVerTitile, &styleLabel_verTitle);
	lv_label_set_align(textVerTitile, LV_LABEL_ALIGN_LEFT);
	lv_label_set_text(textVerTitile, (const char *)verDiscrib);
	
	lv_style_copy(&styleLabel_verDiscrib, &lv_style_plain);
	styleLabel_verDiscrib.body.empty = 1;
	styleLabel_verDiscrib.text.font = &lv_font_consola_13;
	styleLabel_verDiscrib.text.color = LV_COLOR_MAKE(0x60, 0x60, 0x60);
	lv_label_set_style(textVerDiscrib, &styleLabel_verDiscrib);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_width(textVerDiscrib, 300)):
		(lv_obj_set_width(textVerDiscrib, 220));
	lv_obj_set_protect(textVerDiscrib, LV_PROTECT_POS);
	lv_obj_align(textVerDiscrib, textVerTitile, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 15);
	lv_label_set_recolor(textVerDiscrib, true);
	lv_label_set_align(textVerDiscrib, LV_LABEL_ALIGN_LEFT);
	lv_label_set_long_mode(textVerDiscrib, LV_LABEL_LONG_EXPAND);
	lv_label_set_text(textVerDiscrib, STR_VERSION_REALES_REFERENCE);
	lv_style_copy(&styleBtn_usrConfirmOk, &lv_style_plain);
	styleBtn_usrConfirmOk.body.empty = 1;
	lv_btn_set_style(btnUsrConfirm_ok, LV_BTN_STYLE_REL, &styleBtn_usrConfirmOk);
	lv_btn_set_style(btnUsrConfirm_ok, LV_BTN_STYLE_PR, &styleBtn_usrConfirmOk);
	lv_obj_set_size(btnUsrConfirm_ok, 200, 40);
	lv_obj_set_protect(btnUsrConfirm_ok, LV_PROTECT_POS);
	lv_obj_align(btnUsrConfirm_ok, NULL, LV_ALIGN_IN_BOTTOM_MID, -29, -9);
	lv_btn_set_action(btnUsrConfirm_ok, LV_BTN_ACTION_CLICK, funCb_btnNewVersionRunningConfirm_funBack);

	lv_style_copy(&styleLabelBtn_usrConfirmOk, &lv_style_plain);
	styleLabelBtn_usrConfirmOk.text.font = &lv_font_dejavu_20;
	styleLabelBtn_usrConfirmOk.text.color = LV_COLOR_GREEN;
	lv_label_set_style(LabelBtnUsrConfirm_ok, &styleLabelBtn_usrConfirmOk);
	lv_obj_set_size(LabelBtnUsrConfirm_ok, 180, 35);
	lv_obj_set_protect(LabelBtnUsrConfirm_ok, LV_PROTECT_POS);
	lv_obj_align(LabelBtnUsrConfirm_ok, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(LabelBtnUsrConfirm_ok, "ok, i got it");

	vTaskDelay(100 / portTICK_PERIOD_MS);
	lv_page_scroll_hor(pageVerNotice, devStatusDispMethod_landscapeIf_get()?(-320):(-240));
	vTaskDelay(300 / portTICK_PERIOD_MS);
	lv_page_scroll_hor(pageVerNotice, devStatusDispMethod_landscapeIf_get()?(320):(240));
	vTaskDelay(300 / portTICK_PERIOD_MS);

	xSemaphoreGive(xSph_lvglOpreat); //   --lvgl[realse]

	while(true == uiBlockFlg_newVersionRunningConfirm)vTaskDelay(20 / portTICK_PERIOD_MS);
	lv_obj_del(mainCont);
#endif
}

static void lvglUsrApp_uiInitialize(void){

	lv_obj_t *scr = NULL;

	uint8_t loop = 0;

	stt_devStatusRecord devStatusRecordFlg_temp = {0};
	
	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

	xSemaphoreTake(xSph_lvglOpreat, portMAX_DELAY); // --lvgl[take up]

	scr = lv_obj_create(NULL, NULL);

//	lv_obj_set_size(scr, 320, 320);

	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(scr, 320, 240)):
		(lv_obj_set_size(scr, 240, 320));
	
	lv_scr_load(scr);

//	lv_theme_t *th = lv_theme_mono_init(100, NULL);
//	lv_theme_t *th = lv_theme_zen_init(100, NULL);
//	lv_theme_t *th = lv_theme_nemo_init(100, NULL);
//	lv_theme_t *th = lv_theme_night_init(100, NULL);
//	lv_theme_t *th = lv_theme_material_init(100, NULL);
	lv_theme_t *th = lv_theme_alien_init(100, NULL);
//	lv_theme_t *th = lv_theme_default_init(100, NULL);
	lv_theme_set_current(th);

	lvGuiHome_styleApplicationInit(); //home界面总体风格初始化

	xSemaphoreGive(xSph_lvglOpreat); //   --lvgl[realse]

//	lvGui_upgradeTestDisp(); //hellow world test

	lvGui_usrAppPowerUp_Animation();

	xSemaphoreTake(xSph_lvglOpreat, portMAX_DELAY); // --lvgl[take up]

	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(lv_layer_top(), 320, 240)):
		(lv_obj_set_size(lv_layer_top(), 240, 320));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(lv_layer_sys(), 320, 240)):
		(lv_obj_set_size(lv_layer_sys(), 240, 320));

	imageBK = usrAppHomepageBground_objPicFigure_get();

//	guiBussiness_guiBlockTipsCreat();
//	lvGui_usrAppBussinessRunning_block(0, "wifi\nreconnecting...", 30);
//	while(1)vTaskDelay(50 / portTICK_PERIOD_MS);
	
	iconHeaderObj_wifi = lv_img_create(lv_layer_top(), NULL);
	iconHeaderObj_node = lv_img_create(lv_layer_top(), NULL);

	lv_obj_set_pos(iconHeaderObj_wifi,	   	  1,   1);
	lv_obj_set_pos(iconHeaderObj_node,	  	 25,   1);
	
	lv_img_set_src(iconHeaderObj_wifi, &iconHeader_wifi_D);
	lv_img_set_src(iconHeaderObj_node, &iconHeader_apple);

	usrApp_fullScreenRefresh_self(50, 0);

//	xSemaphoreGive(xSph_lvglOpreat);
//	while(1)vTaskDelay(50 / portTICK_PERIOD_MS);

#if(LVAPP_DISP_ELECPARAM_HIDDEN_EN != 1) //电量参数显示条件编译

	iconHeaderObj_elec = lv_img_create(lv_layer_top(), NULL);
	lv_obj_set_pos(iconHeaderObj_elec,		150,   1);
	lv_img_set_src(iconHeaderObj_elec, &iconHeader_elec);
	lv_obj_set_style(iconHeaderObj_elec, styleImg_iconHeater);
#endif

//	while(1)vTaskDelay(50 / portTICK_PERIOD_MS);

	lv_img_set_style(iconHeaderObj_node, styleImg_iconHeater);

	textHeaderObj_time = lv_label_create(lv_layer_top(), NULL);
	lv_label_set_text(textHeaderObj_time, "00:00");
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(textHeaderObj_time, 125, 3)):
		(lv_obj_set_pos(textHeaderObj_time,  84, 2));
	lv_obj_set_style(textHeaderObj_time, styleText_time);

#if(LVAPP_DISP_ELECPARAM_HIDDEN_EN != 1) //电量参数显示条件编译

	textHeaderObj_elec = lv_label_create(lv_layer_top(), NULL);
	lv_obj_set_style(textHeaderObj_elec, styleText_elecAnodenum);
	(devStatusRecordFlg_temp.devElecsumDisp_IF)?
		(lv_label_set_text(textHeaderObj_elec, "0.0kWh")):
		(lv_label_set_text(textHeaderObj_elec, "0.0W"));
	lv_obj_set_protect(textHeaderObj_elec, LV_PROTECT_POS);
	lv_label_set_long_mode(textHeaderObj_elec, LV_LABEL_LONG_SCROLL);
	lv_obj_set_size(textHeaderObj_elec, 20, 11);
	lv_obj_set_pos(textHeaderObj_elec, 167, 7);
#endif

	textHeaderObj_meshNodeNum = lv_label_create(lv_layer_top(), NULL);
	lv_label_set_text(textHeaderObj_meshNodeNum, "01");
	lv_obj_set_pos(textHeaderObj_meshNodeNum, 46, 7);
	lv_obj_set_style(textHeaderObj_meshNodeNum, styleText_elecAnodenum);

	textHeaderObj_meshRole = lv_label_create(lv_layer_top(), NULL);
	lv_label_set_text(textHeaderObj_meshRole, " ");
	lv_obj_set_protect(textHeaderObj_meshRole, LV_PROTECT_POS);
	lv_obj_align(textHeaderObj_meshRole, textHeaderObj_meshNodeNum, LV_ALIGN_OUT_TOP_MID, 0, 5);
	lv_obj_set_style(textHeaderObj_meshNodeNum, styleText_elecAnodenum);

#if(LVAPP_DISP_ELECPARAM_HIDDEN_EN != 1) //电量参数显示条件编译

	if(devStatusDispMethod_landscapeIf_get()){ //横屏位置处理

		lv_obj_set_pos(iconHeaderObj_elec,		  230, 1);
		lv_obj_set_pos(textHeaderObj_elec, 		  247, 7);
	}
#else

#endif

	lv_obj_set_style(iconHeaderObj_wifi, styleImg_iconWifi);

	usrAppBgroudObj_styleRefresh();
	
	lvGuiRefreshCounter_bussinessBk = 1;

	xSemaphoreGive(xSph_lvglOpreat); //   --lvgl[realse]

//	while(1)vTaskDelay(50 / portTICK_PERIOD_MS);

	xEventGp_screenTouch = xEventGroupCreate();
	msgQh_wifiConfigCompleteTips = xQueueCreate(2, sizeof(uint8_t));	
	msgQh_uiHp_devCtrlObjValChg = xQueueCreate(10, sizeof(stt_msgDats_uiHpDevCtrlObjValChg));
}

static void task_guiSwitch_Detecting(void *pvParameter){

	uint8_t loop = 0;

	EventBits_t screenTouch_etBits = 0;
	EventBits_t loopTimerTips_etBits = 0;

	uint8_t msgQrptr_devRestartdelayCounter = 0;

	static bool guiFirst_flg = false;
	
//	lv_point_t indevPoint = {0};

//	lv_indev_get_point(lv_indev_get_act(), &indevPoint);
//	printf("point_x:%d, point_y:%d.\n", indevPoint.x, indevPoint.y);
//	lv_indev_get_vect(lv_indev_get_act(), &indevPoint);
//	printf("vect_x:%d, vect_y:%d.\n", indevPoint.x, indevPoint.y);

	for(;;){

		xSemaphoreTake(xSph_lvglOpreat, portMAX_DELAY); // --lvgl[take up]

		//特殊触摸信号检测
		if(xEventGp_screenTouch){

			screenTouch_etBits = xEventGroupWaitBits(xEventGp_screenTouch, 
													 TOUCHEVENT_FLG_BITHOLD_RESERVE,
													 pdTRUE,
													 pdFALSE,
													 5);
		}

		if((screenTouch_etBits & TOUCHEVENT_FLG_BITHOLD_TOUCHTRIG) == TOUCHEVENT_FLG_BITHOLD_TOUCHTRIG){

//			printf("touch get, point_x:%d, point_y:%d.\n", devTouchGetInfo.touchPosition[0].x,
//														   devTouchGetInfo.touchPosition[0].y);

//			printf("touch get, point:%d.\n", devTouchGetInfo.touchNum);
//			devScreenBkLight_weakUp();

			usrAppHomepageRecovery_dispTimeoutCount_refresh();

#if(SCREENSAVER_RUNNING_ENABLE == 1)

			screensaverDispAttrParam.idleCounter = 0;
			if(screensaverDispAttrParam.flg_screensaverPageCreat == 1){

				lvglUsrApp_epidemicSituation_disp(false);
			}
#endif
		}

		/*业务保留，暂时失能*/
//		if((screenTouch_etBits & TOUCHEVENT_FLG_BITHOLD_GESTRUEHAP) == TOUCHEVENT_FLG_BITHOLD_GESTRUEHAP){

//			switch(devTouchGetInfo.valGestrue){

//				case FTIC_EXTRA_REGISTERVAL_GES_mUP:{

//					printf("gestrue move up happen.\n");

//				}break;

//				case FTIC_EXTRA_REGISTERVAL_GES_mDN:{

//					printf("gestrue move down happen.\n");
//					
//				}break;

//				case FTIC_EXTRA_REGISTERVAL_GES_mLT:{

//					switch(guiPage_current){
//					
//						case bussinessType_Home:{

//							bool gestureReserve_flg = true;

//							if(gestureReserve_flg)
//								if(ctrlObj_slidingCalmDownCounter)gestureReserve_flg = false;

//							if(gestureReserve_flg){

//								switch(currentDev_typeGet()){

//									case devTypeDef_scenario:{

//										if(devDriverBussiness_scnarioSwitch_driverClamDown_get())
//											gestureReserve_flg = false;
//									
//									}break;

//									default:break;
//								}
//							}

//							if(gestureReserve_flg)
//								lvGui_usrSwitch(bussinessType_Menu, teue);
//					
//						}break;
//					
//						default:break;
//					}
//					
//					printf("gestrue move left happen.\n");

//				}break;

//				case FTIC_EXTRA_REGISTERVAL_GES_mRT:{

//					printf("gestrue move right happen.\n");

//				}break;

//				case FTIC_EXTRA_REGISTERVAL_GES_zIN:{

//					printf("gestrue zoom in happen.\n");

//				}break;

//				case FTIC_EXTRA_REGISTERVAL_GES_zOUT:{

//					printf("gestrue zoom out happen.\n");

//				}break;

//				default:{

//					printf("unknow gestrue:%02X.\n", devTouchGetInfo.valGestrue);
//					
//				}break;
//			}			
//		}

		if(true == usrApp_guiSwitchParam.usrAppGuiSwitch_flg){ //界面切换业务

			bool corposeIf = false;

			if(NULL != usrApp_guiSwitchParam.usrAppGuiSwitch_preFunc){

				usrApp_guiSwitchParam.usrAppGuiSwitch_preFunc();
				usrApp_guiSwitchParam.usrAppGuiSwitch_preFunc = NULL;
			}
			else{

				corposeIf = true;
			}
			
			usrApp_guiSwitchParam.usrAppGuiSwitch_flg = false;
			lvGui_usrSwitch(usrApp_guiSwitchParam.guiTarget, corposeIf);
		}

		if(!homepageRecovery_timeoutCounter){ //界面显示超时

			if((guiPage_current != bussinessType_Home) &&
			   (guiPage_current != bussinessType_menuPageWifiConfig)){

				usrGuiBussiness_type guiPage_temp = guiPage_current;

				lvGui_usrSwitch(bussinessType_Home, true);

				switch(guiPage_temp){

					case bussinessType_menuPageOther:{guiDispTimeOut_pageOther();}break;
					
					case bussinessType_menuPageDelayer:{guiDispTimeOut_pageDelayerSet();}break;
					
					case bussinessType_menuPageTimer:{guiDispTimeOut_pageTimerSet();}break;
					
					case bussinessType_menuPageLinkageConfig:{guiDispTimeOut_pageLinkageCfg();}break;
					
					case bussinessType_menuPageSetting:{guiDispTimeOut_pageSettingSet();}break;
					
					case bussinessType_menuPageWifiConfig:{guiDispTimeOut_pageWifiCfg();}break;

					default:{}break;
				}

				printf("page disp time out.\n");
			}
		}

		if(guiInfoRefresh_calmDn_counter)guiInfoRefresh_calmDn_counter --;
		else{

			//页眉信息更新
			pageHeader_infoRefreshLoop();
	
			//当前活动界面信息动态刷新
			pageActivity_infoRefreshLoop();
			
			//tips事件检测 ---定时事件
			if(xEventGp_tipsLoopTimer){ //先进行一下非空检测，防止初始化延后导致读取空内存
	
				loopTimerTips_etBits = xEventGroupWaitBits(xEventGp_tipsLoopTimer, 
														   LOOPTIMEREVENT_FLG_BITHOLD_RESERVE,
														   pdTRUE,
														   pdFALSE,
														   5);
			}
	
			if(loopTimerTips_etBits){
	
				for(loop = 0; loop < USRAPP_VALDEFINE_TRIGTIMER_NUM; loop ++){
				
					if(loopTimerTips_etBits & (1 << loop)){
	
						char strTips[15] = {0};
	
						sprintf(strTips, "timer_%d up!", loop + 1);
						guiBussiness_tipsLoopTimerCreat(strTips);

//						printf("timer:%d guiTips trig rx!\n", loop);
					}
				}

				if(loop >= USRAPP_VALDEFINE_TRIGTIMER_NUM){

					char strTips[15] = {0};

					if(loopTimerTips_etBits & LOOPTIMEREVENT_FLG_BITHOLD_DELAYUP){
						
						sprintf(strTips, "delayer up!");
						guiBussiness_tipsLoopTimerCreat(strTips);
					}
					else
					if(loopTimerTips_etBits & LOOPTIMEREVENT_FLG_BITHOLD_GUI_BLOCK){

						guiBussiness_guiBlockTipsCreat();
					}
					else
					if(loopTimerTips_etBits & LOOPTIMEREVENT_FLG_BITHOLD_GUI_BLOCKCEL){

						guiBussiness_guiBlockTipsDelete();
					}
					else
					if(loopTimerTips_etBits & LOOPTIMEREVENT_FLG_BITHOLD_FS_TIPS){
						
						guiBussiness_fullScreenTipsCreat();
						usrApp_fullScreenRefresh_self(200, 0);
					}
					else
					if(loopTimerTips_etBits & LOOPTIMEREVENT_FLG_BITHOLD_FS_TIPS_CEL){

						guiBussiness_fullScreenTipsDelete();
					}
#if(SCREENSAVER_RUNNING_ENABLE == 1)

					else
					if(loopTimerTips_etBits & LOOPTIMEREVENT_FLG_BITHOLD_FS_EPIDEMIC){

						lvglUsrApp_epidemicSituation_disp(true);
					}
#endif
				}
			}
	
			if(trigFlg_loopTimerTips){
	
				guiBussiness_tipsLoopTimerDelete();
			}
	
			//消息通知接收 ---设备重启延时执行倒计时 时间提示
			if(xQueueReceive(msgQh_systemRestartDelayCounterTips, &msgQrptr_devRestartdelayCounter, 1 / portTICK_RATE_MS) == pdTRUE){
	
				char counterDisp_text[5] = {0};
	
				sprintf(counterDisp_text, "%d", msgQrptr_devRestartdelayCounter);
				guiBussiness_tipsSystemRestartCreat(counterDisp_text);
			}
		}

		if(cntrDat_scrDis.actComplete == 1){

			cntrDat_scrDis.actComplete = 0;
			lv_indev_enable(LV_INDEV_TYPE_POINTER, true);
		}

		if(!uiBlockFlg_systemInit){

			if(devRunningTimeFromPowerUp_couter > 4){

				uiBlockFlg_systemInit = true;
				lvGui_usrAppBussinessRunning_block(0, "system\ninitializing...", 7);
			}
		}

		if(false == guiFirst_flg){

			guiFirst_flg = true;

			lvGui_usrSwitch(bussinessType_Home, true);
		}

#if(SCREENSAVER_RUNNING_ENABLE == 1)

		if(screensaverDispAttrParam.flg_screensaverDataRefresh == 1){
		
			screensaverDispAttrParam.flg_screensaverDataRefresh = 0;
		
			if(screensaverDispAttrParam.flg_screensaverPageCreat == 1){
		
				lvglUsrApp_epidemicSituationDisp_allObjRefresh();
			}
		} 
#endif

		xSemaphoreGive(xSph_lvglOpreat); //  --lvgl[realse]
	}
}

#if(SCREENSAVER_RUNNING_ENABLE == 1)
 static void lvglUsrApp_epidemicSituationDisp_allObjRefresh(void){

	if(mwifi_is_connected()){

		char epidNumStrTemp[32] = {0};
		char epidCyNameStrTemp[64] = {0};
		stt_localTime devParamTemp_infoData = {0};
		
		usrAppDevCurrentSystemTime_paramGet(&devParamTemp_infoData);
		
		sprintf(epidNumStrTemp, "%d/%d/%d", devParamTemp_infoData.time_Year,
											devParamTemp_infoData.time_Month,
											devParamTemp_infoData.time_Day);
		lv_label_set_text(objLabel_epidCurDate, (const char *)epidNumStrTemp);
		sprintf(epidNumStrTemp, "#00FF00 %d#", epidemicDataRunningParam.epidData_cure);
		lv_label_set_text(objLabel_epidCure, (const char *)epidNumStrTemp);
		sprintf(epidNumStrTemp, "#FFFF00 %d#", epidemicDataRunningParam.epidData_confirmed);
		lv_label_set_text(objLabel_epidConfirmed, (const char *)epidNumStrTemp);
		sprintf(epidNumStrTemp, "#808080 %d#", epidemicDataRunningParam.epidData_deaths);
		lv_label_set_text(objLabel_epidDeaths, (const char *)epidNumStrTemp);

		paramSettingGet_epidCyName(epidCyNameStrTemp);
		lv_label_set_text(objLabel_epidLocation, (const char *)epidCyNameStrTemp);
	}
	else
	{
		lv_label_set_text(objLabel_epidCurDate,   "xxxx");
		lv_label_set_text(objLabel_epidCure, 	  "XXXX");
		lv_label_set_text(objLabel_epidConfirmed, "XXXX");
		lv_label_set_text(objLabel_epidDeaths, 	  "XXXX");
		lv_label_set_text(objLabel_epidLocation,  "xxxx");
	}

	usrApp_fullScreenRefresh_self(50, 0);
 }

 static void lvglUsrApp_epidemicSituationDisp_styleInit(void){

	lv_style_copy(styleLabel_epidTitle, &lv_style_plain);
	styleLabel_epidTitle->text.font = &lv_font_dejavu_20;
	styleLabel_epidTitle->text.color = LV_COLOR_WHITE;

	lv_style_copy(styleLabel_epidCurDate, &lv_style_plain);
	styleLabel_epidCurDate->text.font = &lv_font_consola_13;
	styleLabel_epidCurDate->text.color = LV_COLOR_MAKE(0, 192, 192);

	lv_style_copy(styleLabel_epidNum, &lv_style_plain);
	styleLabel_epidNum->text.font = &lv_font_dejavu_30;
	styleLabel_epidNum->text.color = LV_COLOR_GRAY;

	lv_style_copy(styleLabel_epidRef, &lv_style_plain);
	styleLabel_epidRef->text.font = &lv_font_arial_15;
	styleLabel_epidRef->text.color = LV_COLOR_SILVER;

	lv_style_copy(styleLabel_epidLocation, &lv_style_plain);
	styleLabel_epidLocation->text.font = &lv_font_consola_13;
	styleLabel_epidLocation->text.color = LV_COLOR_MAKE(200, 191, 231);
 }

 static void lvglUsrApp_epidemicSituation_disp(bool creatIf){

	static lv_obj_t *objPageBase[64] = {NULL};
	const lv_img_dsc_t *figPic = &bGround_picFigure_14;

	uint8_t loop_x = 0,
			loop_y = 0;
	uint8_t loopCreat = 0;

	(true == creatIf)?
		(screensaverDispAttrParam.flg_screensaverPageCreat = 1):
		(screensaverDispAttrParam.flg_screensaverPageCreat = 0);

	if(creatIf == false){

		if(objPage_epidemicDisp != NULL){

			lv_obj_del(objPage_epidemicDisp);
			objPage_epidemicDisp = NULL;
			return;
		}
	}
	else
	{
		if(objPage_epidemicDisp != NULL)return;
	}

	objPage_epidemicDisp = lv_obj_create(lv_layer_top(), NULL);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(objPage_epidemicDisp, 320, 240)):
		(lv_obj_set_size(objPage_epidemicDisp, 240, 320));
	
	if(devStatusDispMethod_landscapeIf_get()){

		loop_x = 320 / figPic->header.w;
		loop_y = 240 / figPic->header.h;

		if(320 % figPic->header.w)loop_x += 1;
		if(240 % figPic->header.h)loop_y += 1;
	}
	else
	{
		loop_x = 240 / figPic->header.w;
		loop_y = 320 / figPic->header.h;

		if(240 % figPic->header.w)loop_x += 1;
		if(320 % figPic->header.h)loop_y += 1;
	}

	objPageBase[0] = lv_img_create(objPage_epidemicDisp, NULL);
	lv_img_set_src(objPageBase[0], figPic);
	lv_obj_set_protect(objPageBase[0], LV_PROTECT_POS);
	lv_obj_set_pos(objPageBase[0], 0, 0);

	for(loopCreat = 1; loopCreat < (loop_x * loop_y); loopCreat ++){

		objPageBase[loopCreat] = lv_img_create(objPage_epidemicDisp, objPageBase[0]);
		lv_obj_set_protect(objPageBase[loopCreat], LV_PROTECT_POS);
		lv_obj_set_pos(objPageBase[loopCreat], loopCreat % loop_x * figPic->header.w, loopCreat / loop_x * figPic->header.h);
	}

	objLabel_epidTitle = lv_label_create(objPage_epidemicDisp, NULL);
	lv_label_set_text(objLabel_epidTitle, "NCP epidemic situation");
	lv_obj_set_style(objLabel_epidTitle, styleLabel_epidTitle);
	lv_obj_set_protect(objLabel_epidTitle, LV_PROTECT_POS);
	devStatusDispMethod_landscapeIf_get()?
		(lv_obj_set_pos(objLabel_epidTitle, 62, 10)):
		(lv_obj_set_pos(objLabel_epidTitle, 17, 20));

	objLabel_epidCurDate = lv_label_create(objPage_epidemicDisp, NULL);
	lv_label_set_recolor(objLabel_epidCurDate, true);
	lv_obj_set_style(objLabel_epidCurDate, styleLabel_epidCurDate);
	lv_obj_set_protect(objLabel_epidCurDate, LV_PROTECT_POS);
	lv_obj_align(objLabel_epidCurDate, objLabel_epidTitle, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	objLabel_epidLocalRef = lv_label_create(objPage_epidemicDisp, NULL);
	lv_label_set_text(objLabel_epidLocalRef, "Location:");
	lv_obj_set_style(objLabel_epidLocalRef, styleLabel_epidLocation);
	lv_obj_set_protect(objLabel_epidLocalRef, LV_PROTECT_POS);
	lv_obj_align(objLabel_epidLocalRef, objLabel_epidCurDate, LV_ALIGN_OUT_RIGHT_MID, 42, 0);
//	lv_obj_align(objLabel_epidLocalRef, objLabel_epidTitle, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

	objLabel_epidLocation = lv_label_create(objPage_epidemicDisp, NULL);
	lv_label_set_recolor(objLabel_epidLocation, true);
	lv_label_set_long_mode(objLabel_epidLocation, LV_LABEL_LONG_ROLL);
	devStatusDispMethod_landscapeIf_get()?
		(lv_obj_set_size(objLabel_epidLocation, 75, 25)):
		(lv_obj_set_size(objLabel_epidLocation, 65, 25));
	lv_obj_set_style(objLabel_epidLocation, styleLabel_epidLocation);
	lv_obj_set_protect(objLabel_epidLocation, LV_PROTECT_POS);
	lv_obj_align(objLabel_epidLocation, objLabel_epidLocalRef, LV_ALIGN_OUT_RIGHT_MID, 3, 6);

	objLabel_epidCure = lv_label_create(objPage_epidemicDisp, NULL);
	lv_label_set_align(objLabel_epidCure, LV_LABEL_ALIGN_CENTER);
	lv_label_set_long_mode(objLabel_epidCure, LV_LABEL_LONG_ROLL);
	lv_obj_set_size(objLabel_epidCure, 140, 40);
	lv_label_set_recolor(objLabel_epidCure, true);
	lv_obj_set_style(objLabel_epidCure, styleLabel_epidNum);
	lv_obj_set_protect(objLabel_epidCure, LV_PROTECT_POS);
	devStatusDispMethod_landscapeIf_get()?
		(lv_obj_align(objLabel_epidCure, objLabel_epidTitle, LV_ALIGN_OUT_BOTTOM_LEFT, 60, 26)):
		(lv_obj_align(objLabel_epidCure, objLabel_epidTitle, LV_ALIGN_OUT_BOTTOM_LEFT, 65, 35));

	objLabel_epidConfirmed = lv_label_create(objPage_epidemicDisp, NULL);
	lv_label_set_align(objLabel_epidConfirmed, LV_LABEL_ALIGN_CENTER);
	lv_label_set_long_mode(objLabel_epidConfirmed, LV_LABEL_LONG_ROLL);
	lv_obj_set_size(objLabel_epidConfirmed, 140, 40);
	lv_label_set_recolor(objLabel_epidConfirmed, true);
	lv_obj_set_style(objLabel_epidConfirmed, styleLabel_epidNum);
	lv_obj_set_protect(objLabel_epidConfirmed, LV_PROTECT_POS);
	devStatusDispMethod_landscapeIf_get()?
		(lv_obj_align(objLabel_epidConfirmed, objLabel_epidCure, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 22)):
		(lv_obj_align(objLabel_epidConfirmed, objLabel_epidCure, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40));

	objLabel_epidDeaths = lv_label_create(objPage_epidemicDisp, NULL);
	lv_label_set_align(objLabel_epidDeaths, LV_LABEL_ALIGN_CENTER);
	lv_label_set_long_mode(objLabel_epidDeaths, LV_LABEL_LONG_ROLL);
	lv_obj_set_size(objLabel_epidDeaths, 140, 40);
	lv_label_set_recolor(objLabel_epidDeaths, true);
	lv_obj_set_style(objLabel_epidDeaths, styleLabel_epidNum);
	lv_obj_set_protect(objLabel_epidDeaths, LV_PROTECT_POS);
	devStatusDispMethod_landscapeIf_get()?
		(lv_obj_align(objLabel_epidDeaths, objLabel_epidConfirmed, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 22)):
		(lv_obj_align(objLabel_epidDeaths, objLabel_epidConfirmed, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40));

	objIcon_epidCure = lv_img_create(objPage_epidemicDisp, NULL);
	lv_img_set_src(objIcon_epidCure, &epidemicIcon_cure);
	lv_obj_set_protect(objIcon_epidCure, LV_PROTECT_POS);
	lv_obj_align(objIcon_epidCure, objLabel_epidCure, LV_ALIGN_OUT_LEFT_MID, 5, 0);

	objIcon_epidConfirmed = lv_img_create(objPage_epidemicDisp, NULL);
	lv_img_set_src(objIcon_epidConfirmed, &epidemicIcon_confirmed);
	lv_obj_set_protect(objIcon_epidConfirmed, LV_PROTECT_POS);
	lv_obj_align(objIcon_epidConfirmed, objLabel_epidConfirmed, LV_ALIGN_OUT_LEFT_MID, 5, 0);
	
	objIcon_epidDeaths = lv_img_create(objPage_epidemicDisp, NULL);
	lv_img_set_src(objIcon_epidDeaths, &epidemicIcon_deaths);
	lv_obj_set_protect(objIcon_epidDeaths, LV_PROTECT_POS);
	lv_obj_align(objIcon_epidDeaths, objLabel_epidDeaths, LV_ALIGN_OUT_LEFT_MID, 5, 0);

	objLabelRef_epidCure = lv_label_create(objPage_epidemicDisp, NULL);
	lv_label_set_text(objLabelRef_epidCure, "Cumulative cure");
	lv_obj_set_style(objLabelRef_epidCure, styleLabel_epidRef);
	lv_obj_set_protect(objLabelRef_epidCure, LV_PROTECT_POS);
	lv_obj_align(objLabelRef_epidCure, objLabel_epidCure, LV_ALIGN_OUT_BOTTOM_MID, 0, -4);

	objLabelRef_epidConfirmed = lv_label_create(objPage_epidemicDisp, NULL);
	lv_label_set_text(objLabelRef_epidConfirmed, "Existing confirmed");
	lv_obj_set_style(objLabelRef_epidConfirmed, styleLabel_epidRef);
	lv_obj_set_protect(objLabelRef_epidConfirmed, LV_PROTECT_POS);
	lv_obj_align(objLabelRef_epidConfirmed, objLabel_epidConfirmed, LV_ALIGN_OUT_BOTTOM_MID, 0, -4);

	objLabelRef_epidDeaths = lv_label_create(objPage_epidemicDisp, NULL);
	lv_label_set_text(objLabelRef_epidDeaths, "Cumulative death");
	lv_obj_set_style(objLabelRef_epidDeaths, styleLabel_epidRef);
	lv_obj_set_protect(objLabelRef_epidDeaths, LV_PROTECT_POS);
	lv_obj_align(objLabelRef_epidDeaths, objLabel_epidDeaths, LV_ALIGN_OUT_BOTTOM_MID, 0, -4);

//	lv_obj_animate(objLabel_epidTitle,		 LV_ANIM_FLOAT_TOP,   100, 50, NULL);
//	lv_obj_animate(objIcon_epidCure,		 LV_ANIM_FLOAT_LEFT,  100, 50, NULL);
//	lv_obj_animate(objIcon_epidConfirmed, 	 LV_ANIM_FLOAT_LEFT,  100, 50, NULL);
//	lv_obj_animate(objIcon_epidDeaths,	 	 LV_ANIM_FLOAT_LEFT,  100, 50, NULL);
//	
//	lv_obj_animate(objLabel_epidCure,		 LV_ANIM_FLOAT_LEFT,  200, 50, NULL);
//	lv_obj_animate(objLabel_epidConfirmed, 	 LV_ANIM_FLOAT_LEFT,  200, 50, NULL);
//	lv_obj_animate(objLabel_epidDeaths,	 	 LV_ANIM_FLOAT_LEFT,  200, 50, NULL);
//	lv_obj_animate(objLabelRef_epidCure,	 LV_ANIM_FLOAT_LEFT,  200, 50, NULL);
//	lv_obj_animate(objLabelRef_epidConfirmed,LV_ANIM_FLOAT_LEFT,  200, 50, NULL);
//	lv_obj_animate(objLabelRef_epidDeaths,	 LV_ANIM_FLOAT_LEFT,  200, 50, NULL);

	lvglUsrApp_epidemicSituationDisp_allObjRefresh();

	epidemicDataRunningParam.reqTimeCounter =\
		epidemicDataRunningParam.reqTimePeriod - 3; //刚进入锁屏，触发提前获取疫情数据
 }

 bool screensaverDispApp_period1sRealesRunning(void){

	bool resEpidDataReq_creat = false;

	static uint16_t nodeNum_record = 0;

	stt_devStatusRecord devStatusRecordFlg_temp = {0};

	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

	if(systemDevice_startUpTime_get() < 30)return resEpidDataReq_creat;

	if(epidemicDataRunningParam.reqTimeCounter < epidemicDataRunningParam.reqTimePeriod)epidemicDataRunningParam.reqTimeCounter ++;
	else{

		epidemicDataRunningParam.reqTimeCounter = 0;
		resEpidDataReq_creat = true;
	} //即时主机疫情显示设置关闭，也不能阻挡子设备进行疫情数据请求
	if(devStatusRecordFlg_temp.screensaverRunning_IF == 0)return resEpidDataReq_creat;

	if(guiPage_current == bussinessType_Home){

		screensaverDispAttrParam.en_idleCounter = 1;
	}
	else
	{
		if(screensaverDispAttrParam.en_idleCounter){

			screensaverDispAttrParam.en_idleCounter = 0;
			screensaverDispAttrParam.idleCounter = 0;
		}
	}

	if(screensaverDispAttrParam.en_idleCounter == 1){

		if(screensaverDispAttrParam.idleCounter < screensaverDispAttrParam.idlePeriod)screensaverDispAttrParam.idleCounter ++;
		else{

			xEventGroupSetBits(xEventGp_tipsLoopTimer, LOOPTIMEREVENT_FLG_BITHOLD_FS_EPIDEMIC);
		}
	}

	if(nodeNum_record != devMeshNodeNum_Get()){ //节点数量发生变化，触发提前获取疫情数据

		nodeNum_record = devMeshNodeNum_Get();

		epidemicDataRunningParam.reqTimeCounter =\
			epidemicDataRunningParam.reqTimePeriod - 3; //MQTT连接成功，触发提前获取疫情数据
	}

	return resEpidDataReq_creat;
 }

 static void lvglUsrApp_screensaverNormal_A_allObjRefresh(void){

	stt_devDataPonitTypedef devDataPoint = {0};
	
	currentDev_dataPointGet(&devDataPoint);		

	switch(currentDev_typeGet()){

		case devTypeDef_mulitSwOneBit:
		case devTypeDef_mulitSwTwoBit:
		case devTypeDef_mulitSwThreeBit:{

			uint8_t statusTemp = 0;
			uint8_t loopCreat = 0,
					countCreat = 0;
			
			memcpy(&statusTemp, &devDataPoint, sizeof(uint8_t));
			switch(currentDev_typeGet()){
				
				case devTypeDef_mulitSwTwoBit:loopCreat = 2;break;
				case devTypeDef_mulitSwThreeBit:loopCreat = 3;break;
				default:loopCreat = 1;break;
			}
			
			for(countCreat = 0; countCreat < loopCreat; countCreat ++){
			
				if(statusTemp & (1 << countCreat)){
				
					styleSsrBaseObj[countCreat]->image.color = LV_COLOR_BLUE;
					styleSsrBaseIcon[countCreat]->image.color = LV_COLOR_WHITE;
				}
				else{
				
					styleSsrBaseObj[countCreat]->image.color = LV_COLOR_WHITE;
					styleSsrBaseIcon[countCreat]->image.color = LV_COLOR_GRAY;
				}
				lv_obj_refresh_style(objBase_ssr_swCtrlStatus[countCreat]);
				lv_obj_refresh_style(objIcon_ssr_swCtrlStatus[countCreat]);
			}

		}break;
		
		case devTypeDef_dimmer:{}break;
		case devTypeDef_fans:{}break;
		case devTypeDef_scenario:{}break;
		case devTypeDef_curtain:{}break;
		case devTypeDef_thermostat:{}break;
		case devTypeDef_heater:{}break;
		case devTypeDef_thermostatExtension:{}break;
		
		default:break;
	}
 }

 static void lvglUsrApp_screensaverNormal_A_styleInit(void){

	lv_style_copy(styleLabel_scrsaverA_time, &lv_style_plain);
	styleLabel_scrsaverA_time->text.font = &lv_font_arialNum_100;
	styleLabel_scrsaverA_time->text.color = LV_COLOR_WHITE;

	lv_style_copy(styleLabel_scrsaverA_timeDot, &lv_style_plain);
	styleLabel_scrsaverA_timeDot->text.font = &lv_font_dejavu_40;
	styleLabel_scrsaverA_timeDot->text.color = LV_COLOR_WHITE;

	lv_style_copy(styleLabel_scrsaverA_date, &lv_style_plain);
	styleLabel_scrsaverA_date->text.font = &lv_font_dejavu_20;
	styleLabel_scrsaverA_date->text.color = LV_COLOR_WHITE;

	lv_style_copy(styleSsrBaseObj[0], &lv_style_plain);
	styleSsrBaseObj[0]->image.color = LV_COLOR_WHITE;
	styleSsrBaseObj[0]->image.intense = LV_OPA_COVER;
	styleSsrBaseObj[0]->image.opa = LV_OPA_60;
	lv_style_copy(styleSsrBaseObj[1], styleSsrBaseObj[0]);
	lv_style_copy(styleSsrBaseObj[2], styleSsrBaseObj[0]);

	lv_style_copy(styleSsrBaseIcon[0], &lv_style_plain);
	styleSsrBaseIcon[0]->image.color = LV_COLOR_GRAY;
	styleSsrBaseIcon[0]->image.intense = LV_OPA_COVER;
	styleSsrBaseIcon[0]->image.opa = LV_OPA_80;
	lv_style_copy(styleSsrBaseIcon[1], styleSsrBaseIcon[0]);
	lv_style_copy(styleSsrBaseIcon[2], styleSsrBaseIcon[0]);

	lv_style_copy(styleSsrBaseLabel[0], &lv_style_plain);
	styleSsrBaseLabel[0]->text.font = &lv_font_consola_13;
	styleSsrBaseLabel[0]->text.color = LV_COLOR_SILVER;
	lv_style_copy(styleSsrBaseLabel[1], styleSsrBaseLabel[0]);
	lv_style_copy(styleSsrBaseLabel[2], styleSsrBaseLabel[0]);
 }

 static void lvglUsrApp_screensaverNormal_A_disp(bool creatIf){

	 static lv_obj_t *objPageBase[64] = {NULL};
	 const lv_img_dsc_t *figPic = &bGround_picFigure_14;
	 
	 uint8_t loop_x = 0,
			 loop_y = 0;
	 uint8_t loopCreat = 0;

	 stt_dataDisp_guiBussinessHome_btnText dataTextObjDisp_temp = {0};
	 uint8_t dataIconObjDisp_temp[GUIBUSSINESS_CTRLOBJ_MAX_NUM] = {0};

	 usrAppHomepageBtnTextDisp_paramGet(&dataTextObjDisp_temp);
	 usrAppHomepageBtnIconNumDisp_paramGet(dataIconObjDisp_temp);
	 
	 (true == creatIf)?
		 (screensaverDispAttrParam.flg_screensaverPageCreat = 1):
		 (screensaverDispAttrParam.flg_screensaverPageCreat = 0);
	 
	 if(creatIf == false){
	 
		 if(objPage_screensaver_A != NULL){
	 
			 lv_obj_del(objPage_screensaver_A);
			 objPage_screensaver_A = NULL;
			 return;
		 }
	 }
	 else
	 {
		 if(objPage_screensaver_A != NULL)return;
	 }
	 
	 objPage_screensaver_A = lv_obj_create(lv_layer_top(), NULL);
	 (devStatusDispMethod_landscapeIf_get())?
		 (lv_obj_set_size(objPage_screensaver_A, 320, 240)):
		 (lv_obj_set_size(objPage_screensaver_A, 240, 320));
	 
	 if(devStatusDispMethod_landscapeIf_get()){
	 
		 loop_x = 320 / figPic->header.w;
		 loop_y = 240 / figPic->header.h;
	 
		 if(320 % figPic->header.w)loop_x += 1;
		 if(240 % figPic->header.h)loop_y += 1;
	 }
	 else
	 {
		 loop_x = 240 / figPic->header.w;
		 loop_y = 320 / figPic->header.h;
	 
		 if(240 % figPic->header.w)loop_x += 1;
		 if(320 % figPic->header.h)loop_y += 1;
	 }
	 
	 objPageBase[0] = lv_img_create(objPage_screensaver_A, NULL);
	 lv_img_set_src(objPageBase[0], figPic);
	 lv_obj_set_protect(objPageBase[0], LV_PROTECT_POS);
	 lv_obj_set_pos(objPageBase[0], 0, 0);
	 
	 for(loopCreat = 1; loopCreat < (loop_x * loop_y); loopCreat ++){
	 
		 objPageBase[loopCreat] = lv_img_create(objPage_screensaver_A, objPageBase[0]);
		 lv_obj_set_protect(objPageBase[loopCreat], LV_PROTECT_POS);
		 lv_obj_set_pos(objPageBase[loopCreat], loopCreat % loop_x * figPic->header.w, loopCreat / loop_x * figPic->header.h);
	 }

	 objLabel_scrsaverA_timeDot = lv_label_create(objPage_screensaver_A, NULL);
	 lv_label_set_text(objLabel_scrsaverA_timeDot, ":");
	 lv_obj_set_style(objLabel_scrsaverA_timeDot, styleLabel_scrsaverA_timeDot);
	 lv_obj_set_protect(objLabel_scrsaverA_timeDot, LV_PROTECT_POS);
	 lv_obj_align(objLabel_scrsaverA_timeDot, objPage_screensaver_A, LV_ALIGN_IN_TOP_MID, -5, devStatusDispMethod_landscapeIf_get()?(20):(35));
	 objLabel_scrsaverA_time_H = lv_label_create(objPage_screensaver_A, NULL);
	 lv_label_set_text(objLabel_scrsaverA_time_H, "01");
	 lv_obj_set_style(objLabel_scrsaverA_time_H, styleLabel_scrsaverA_time);
	 lv_obj_set_protect(objLabel_scrsaverA_time_H, LV_PROTECT_POS);
	 lv_obj_align(objLabel_scrsaverA_time_H, objLabel_scrsaverA_timeDot, LV_ALIGN_OUT_LEFT_MID, -7, 0);
	 objLabel_scrsaverA_time_M = lv_label_create(objPage_screensaver_A, NULL);
	 lv_label_set_text(objLabel_scrsaverA_time_M, "01");
	 lv_obj_set_style(objLabel_scrsaverA_time_M, styleLabel_scrsaverA_time);
	 lv_obj_set_protect(objLabel_scrsaverA_time_M, LV_PROTECT_POS);
	 lv_obj_align(objLabel_scrsaverA_time_M, objLabel_scrsaverA_timeDot, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

	 objLabel_scrsaverA_date = lv_label_create(objPage_screensaver_A, NULL);
	 lv_label_set_text(objLabel_scrsaverA_date, "Sep 29, Wednesday");
	 lv_obj_set_style(objLabel_scrsaverA_date, styleLabel_scrsaverA_date);
	 lv_obj_set_protect(objLabel_scrsaverA_date, LV_PROTECT_POS);
	 lv_obj_align(objLabel_scrsaverA_date, objLabel_scrsaverA_timeDot, LV_ALIGN_OUT_BOTTOM_MID, 5, 25);

	 switch(currentDev_typeGet()){

		case devTypeDef_mulitSwOneBit:
		case devTypeDef_mulitSwTwoBit:
		case devTypeDef_mulitSwThreeBit:
		case devTypeDef_fans:
		case devTypeDef_scenario:
		case devTypeDef_thermostat:
		case devTypeDef_heater:
		case devTypeDef_thermostatExtension:{

			uint8_t loopCreat = 0,
					countCreat = 0;
			switch(currentDev_typeGet()){
			
				case devTypeDef_mulitSwTwoBit:loopCreat = 2;break;
				case devTypeDef_mulitSwThreeBit:loopCreat = 3;break;
				default:loopCreat = 1;break;
			}

			for(countCreat = 0; countCreat < loopCreat; countCreat ++){

				objBase_ssr_swCtrlStatus[countCreat] = lv_img_create(objPage_screensaver_A, NULL);
				lv_obj_set_protect(objBase_ssr_swCtrlStatus[countCreat], LV_PROTECT_POS);
				lv_img_set_src(objBase_ssr_swCtrlStatus[countCreat], &scrsaverStatusIconBk);
				lv_obj_set_style(objBase_ssr_swCtrlStatus[countCreat], styleSsrBaseObj[countCreat]);
			}
			switch(currentDev_typeGet()){
			
				case devTypeDef_mulitSwTwoBit:{

					lv_obj_align(objBase_ssr_swCtrlStatus[0], objPage_screensaver_A, LV_ALIGN_IN_TOP_MID, -50, devStatusDispMethod_landscapeIf_get()?(175):(245));
					lv_obj_align(objBase_ssr_swCtrlStatus[1], objPage_screensaver_A, LV_ALIGN_IN_TOP_MID, 50, devStatusDispMethod_landscapeIf_get()?(175):(245));

				}break;
				
				case devTypeDef_mulitSwThreeBit:{

					lv_obj_align(objBase_ssr_swCtrlStatus[1], objPage_screensaver_A, LV_ALIGN_IN_TOP_MID, 0, devStatusDispMethod_landscapeIf_get()?(175):(245));
					lv_obj_align(objBase_ssr_swCtrlStatus[0], objBase_ssr_swCtrlStatus[1], LV_ALIGN_OUT_LEFT_BOTTOM, -20, 0);
					lv_obj_align(objBase_ssr_swCtrlStatus[2], objBase_ssr_swCtrlStatus[1], LV_ALIGN_OUT_RIGHT_BOTTOM, 20, 0);

				}break;
				
				default:{

					lv_obj_align(objBase_ssr_swCtrlStatus[0], objPage_screensaver_A, LV_ALIGN_IN_TOP_MID, 0, devStatusDispMethod_landscapeIf_get()?(175):(245));

				}break;
			}
			for(countCreat = 0; countCreat < loopCreat; countCreat ++){

				objIcon_ssr_swCtrlStatus[countCreat] = lv_img_create(objPage_screensaver_A, NULL);
				lv_obj_set_protect(objIcon_ssr_swCtrlStatus[countCreat], LV_PROTECT_POS);
				lv_img_set_src(objIcon_ssr_swCtrlStatus[countCreat], usrAppHomepageBtnIconDisp_dataGet(dataIconObjDisp_temp[countCreat]));
				lv_obj_align(objIcon_ssr_swCtrlStatus[countCreat], objBase_ssr_swCtrlStatus[countCreat], LV_ALIGN_CENTER, 0, 0);
				lv_obj_set_style(objIcon_ssr_swCtrlStatus[countCreat], styleSsrBaseIcon[countCreat]);
				objLabel_ssr_swNanme[countCreat] = lv_label_create(objPage_screensaver_A, NULL);
				lv_label_set_align(objLabel_ssr_swNanme[countCreat], LV_LABEL_ALIGN_CENTER);
				lv_label_set_long_mode(objLabel_ssr_swNanme[countCreat], LV_LABEL_LONG_DOT);
				lv_obj_set_size(objLabel_ssr_swNanme[countCreat], 60, 20);
				lv_label_set_text(objLabel_ssr_swNanme[countCreat], (const char*)dataTextObjDisp_temp.dataBtnTextDisp[countCreat]);
				lv_obj_set_protect(objLabel_ssr_swNanme[countCreat], LV_PROTECT_POS);
				lv_obj_align(objLabel_ssr_swNanme[countCreat], objBase_ssr_swCtrlStatus[countCreat], LV_ALIGN_OUT_TOP_MID, 0, 0);
				lv_label_set_style(objLabel_ssr_swNanme[countCreat], styleSsrBaseLabel[countCreat]);
			}

		}break;


		case devTypeDef_dimmer:
		case devTypeDef_curtain:{

			

		}break;

		default:break;
	 }

	 lvglUsrApp_screensaverNormal_A_allObjRefresh();
 }
#endif

void lvGui_usrAppBussinessRunning_block(uint8_t iconType, const char *strTips, uint8_t timeOut){

	icon_pageGuiBlockType = iconType;
	strcpy(str_pageGuiBlockTips, strTips);
	xEventGroupSetBits(xEventGp_tipsLoopTimer, LOOPTIMEREVENT_FLG_BITHOLD_GUI_BLOCK);

	usr_guiBlockCounter_trig(timeOut);
}

void lvGui_tipsFullScreen_generate(const char *strTips, uint16_t timeOut){

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED)||\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_SOCKET)||\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_MOUDLE)||\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_RELAY_BOX)	

	devTipsStatusRunning_abnormalTrig(tipsRunningStatus_upgrading, timeOut);	
#else

	strcpy(str_tipsFullScreen, strTips);
	xEventGroupSetBits(xEventGp_tipsLoopTimer, LOOPTIMEREVENT_FLG_BITHOLD_FS_TIPS);
	
	usr_tipsFullScreen_trig(timeOut);	
#endif
}

void lvGui_tipsFullScreen_generateAutoTime(const char *strTips){

	uint16_t timeKeep = 0;
	uint8_t meshNodeNum = (uint8_t)esp_mesh_get_total_node_num();

	strcpy(str_tipsFullScreen, strTips);
	xEventGroupSetBits(xEventGp_tipsLoopTimer, LOOPTIMEREVENT_FLG_BITHOLD_FS_TIPS);

	if(meshNodeNum < 30)timeKeep = 180;
	else{

		timeKeep = meshNodeNum / 10 * 120; //每10个两分钟
	}

	usr_tipsFullScreen_trig(timeKeep);
}

void lvGui_usrAppBussinessRunning_blockCancel(void){

	xEventGroupSetBits(xEventGp_tipsLoopTimer, LOOPTIMEREVENT_FLG_BITHOLD_GUI_BLOCKCEL);
}

void lvGui_tipsFullScreen_distruction(void){

	xEventGroupSetBits(xEventGp_tipsLoopTimer, LOOPTIMEREVENT_FLG_BITHOLD_FS_TIPS_CEL);
}

static void lvGui_homepageRefresh(void){

	if(guiPage_current == bussinessType_Home){

		lv_obj_del(imageBK);

		imageBK = usrAppHomepageBground_objPicFigure_get();
		lvGui_businessHome(imageBK);
		pageHome_buttonMain_imageRefresh(true);
		
		//home界面互控图标比较值重置，以及相关互控图标对象重置（lv系统删除对象指针后 不会 进行NULL赋值）
		memset(btnBindingStatus_record, 0, sizeof(uint8_t) * DEVICE_MUTUAL_CTRL_GROUP_NUM);
//		if(iconBtn_binding_A){lv_obj_del(iconBtn_binding_A);iconBtn_binding_A = NULL;}
//		if(iconBtn_binding_B){lv_obj_del(iconBtn_binding_B);iconBtn_binding_B = NULL;}
//		if(iconBtn_binding_C){lv_obj_del(iconBtn_binding_C);iconBtn_binding_C = NULL;}
		iconBtn_binding_A = NULL;
		iconBtn_binding_B = NULL;
		iconBtn_binding_C = NULL;

		usrApp_fullScreenRefresh_self(20, 0);
	}
}

static void lvGui_usrSwitch(usrGuiBussiness_type guiPage, bool corpse_if){

	guiPage_current = guiPage;

	lvGuiRefreshCounter_bussinessBk = 0;

	//GUI界面刷新检测
	if(guiPage_record != guiPage_current){
	
		guiPage_record = guiPage_current;

		lv_obj_del(imageBK);
	
		lv_style_copy(styleBk_secMenu, &lv_style_plain);
		
		switch(guiPage_record){
	
			case bussinessType_Home:{
	
				imageBK = usrAppHomepageBground_objPicFigure_get();
				lvGui_businessHome(imageBK);
				pageHome_buttonMain_imageRefresh(true);
	
				//home界面互控图标比较值重置，以及相关互控图标对象重置（lv系统删除对象指针后 不会 进行NULL赋值）
				memset(btnBindingStatus_record, 0, sizeof(uint8_t) * DEVICE_MUTUAL_CTRL_GROUP_NUM);
//				if(iconBtn_binding_A){lv_obj_del(iconBtn_binding_A);iconBtn_binding_A = NULL;}
//				if(iconBtn_binding_B){lv_obj_del(iconBtn_binding_B);iconBtn_binding_B = NULL;}
//				if(iconBtn_binding_C){lv_obj_del(iconBtn_binding_C);iconBtn_binding_C = NULL;}
				iconBtn_binding_A = NULL;
				iconBtn_binding_B = NULL;
				iconBtn_binding_C = NULL;

			}break;
			
			case bussinessType_Menu:{
				
				imageBK = usrAppHomepageBground_objPicFigure_get();
				lvGui_businessMenu(imageBK);

				switch(bGroundImg_themeParam.bGround_picOrg_ist){
				
					case bGroudImg_objInsert_usrPic:
				
					case bGroudImg_objInsert_figureA:
					case bGroudImg_objInsert_figureB:
					case bGroudImg_objInsert_figureC:
					case bGroudImg_objInsert_figureD:
					case bGroudImg_objInsert_figureE:
					case bGroudImg_objInsert_figureF:
					case bGroudImg_objInsert_figureG:
					case bGroudImg_objInsert_figureH:
					case bGroudImg_objInsert_figureI:
					case bGroudImg_objInsert_figureJ:
					case bGroudImg_objInsert_figureK:
					case bGroudImg_objInsert_figureL:
					case bGroudImg_objInsert_figureM:
					case bGroudImg_objInsert_figureN:
					case bGroudImg_objInsert_figureO:
					case bGroudImg_objInsert_figureP:
					case bGroudImg_objInsert_figureQ:
					case bGroudImg_objInsert_figureR:
					case bGroudImg_objInsert_figureS:
					case bGroudImg_objInsert_figureT:{
				
						switch(currentDev_typeGet()){
				
							case devTypeDef_mulitSwOneBit:
							case devTypeDef_mulitSwTwoBit:
							case devTypeDef_mulitSwThreeBit:
							case devTypeDef_scenario:{
				
								styleImgBk_underlying->image.intense = 10;
								styleImgBk_underlying->image.color = LV_COLOR_BLACK;
								
								lv_img_set_style(imageBK, styleImgBk_underlying);
				
							}break;
							
							case devTypeDef_dimmer:
							case devTypeDef_fans:
							case devTypeDef_curtain:
							case devTypeDef_thermostat:
							case devTypeDef_heater:{
				
								styleImgBk_underlying->image.intense = LV_OPA_50;
								styleImgBk_underlying->image.color = LV_COLOR_GRAY;
				
								lv_img_set_style(imageBK, styleImgBk_underlying);
				
							}break;
							
							default:{}break;
						}
				
					}break;
				
					default:break;
				}
				
			}break;
			
			case bussinessType_menuPageOther:{
	
				imageBK = lv_obj_create(lv_scr_act(), NULL);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_size(imageBK, 320, 75)):
					(lv_obj_set_size(imageBK, 240, 75));
				styleBk_secMenu->body.main_color = LV_COLOR_BLACK;
				styleBk_secMenu->body.grad_color = LV_COLOR_WHITE;
				lv_obj_set_style(imageBK, styleBk_secMenu);
				lvGui_businessMenu_other(imageBK);
				
			}break;
			
			case bussinessType_menuPageDelayer:{
	
				imageBK = lv_obj_create(lv_scr_act(), NULL);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_size(imageBK, 320, 75)):
					(lv_obj_set_size(imageBK, 240, 75));
				styleBk_secMenu->body.main_color = LV_COLOR_BLACK;
				styleBk_secMenu->body.grad_color = LV_COLOR_WHITE;
				lv_obj_set_style(imageBK, styleBk_secMenu);
				lvGui_businessMenu_delayer(imageBK);
	
			}break;
			
			case bussinessType_menuPageTimer:{
	
				imageBK = lv_obj_create(lv_scr_act(), NULL);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_size(imageBK, 320, 75)):
					(lv_obj_set_size(imageBK, 240, 75));
				styleBk_secMenu->body.main_color = LV_COLOR_BLACK;
				styleBk_secMenu->body.grad_color = LV_COLOR_WHITE;
				lv_obj_set_style(imageBK, styleBk_secMenu);
				lvGui_businessMenu_timer(imageBK);
	
			}break;
			
			case bussinessType_menuPageLinkageConfig:{
	
				imageBK = lv_obj_create(lv_scr_act(), NULL);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_size(imageBK, 320, 75)):
					(lv_obj_set_size(imageBK, 240, 75));
				styleBk_secMenu->body.main_color = LV_COLOR_BLACK;
				styleBk_secMenu->body.grad_color = LV_COLOR_GRAY;
				lv_obj_set_style(imageBK, styleBk_secMenu);
				lvGui_businessMenu_linkageConfig(imageBK);
	
			}break;
			
			case bussinessType_menuPageSetting:{
	
				imageBK = lv_obj_create(lv_scr_act(), NULL);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_size(imageBK, 320, 75)):
					(lv_obj_set_size(imageBK, 240, 75));
				styleBk_secMenu->body.main_color = LV_COLOR_BLACK;
				styleBk_secMenu->body.grad_color = LV_COLOR_GRAY;
				lv_obj_set_style(imageBK, styleBk_secMenu);
				lvGui_businessMenu_settingSet(imageBK);
	
			}break;
			
			case bussinessType_menuPageWifiConfig:{
	
				imageBK = lv_obj_create(lv_scr_act(), NULL);
				(devStatusDispMethod_landscapeIf_get())?
					(lv_obj_set_size(imageBK, 320, 75)):
					(lv_obj_set_size(imageBK, 240, 75));
				styleBk_secMenu->body.main_color = LV_COLOR_BLACK;
				styleBk_secMenu->body.grad_color = LV_COLOR_WHITE;
				lv_obj_set_style(imageBK, styleBk_secMenu);
				lvGui_businessMenu_wifiConfig(imageBK);
	
			}break;
	
			default:break;
		}

		usrAppBgroudObj_styleRefresh();

		switch(currentDev_typeGet()){ //删除对象后补充进行NULL赋值,防止一些业务对象动态生成紊乱

			case devTypeDef_thermostat:
			case devTypeDef_thermostatExtension:{

			}break;

			default:break;
		}
		
		lv_obj_refresh_style(imageBK);
		lv_task_handler();

		usrApp_fullScreenRefresh_self(20, 0);
	}

	if(guiPage_current != bussinessType_Home){

		usrAppHomepageRecovery_dispTimeoutCount_refresh();
	}
}

void lvGui_usrSwitch_withPrefunc(usrGuiBussiness_type guiPage, funcPtr_lvUsrGuiSwitchPreAction func){

	usrApp_guiSwitchParam.usrAppGuiSwitch_preFunc = func;
	usrApp_guiSwitchParam.usrAppGuiSwitch_flg = true;
	usrApp_guiSwitchParam.guiTarget = guiPage;
}

static void lvGuiHome_styleMemoryInitialization(void){

	static bool memAlloced_flg = false;

	if(true == memAlloced_flg)return;
	else memAlloced_flg = true;

	localMemory_lvObjStyle_mallocedFlg = true;

	styleLabel_epidTitle 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabel_epidCurDate 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabel_epidNum 				= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabel_epidRef 				= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabel_epidLocation 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));

	styleLabel_scrsaverA_time 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabel_scrsaverA_timeDot 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabel_scrsaverA_date 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSsrBaseObj[0] 				= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSsrBaseObj[1] 				= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSsrBaseObj[2] 				= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSsrBaseIcon[0] 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSsrBaseIcon[1] 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSsrBaseIcon[2] 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSsrBaseLabel[0] 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSsrBaseLabel[1] 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSsrBaseLabel[2] 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));

	styleImg_iconHeater 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleImg_iconWifi 				= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_versionDiscrib 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_elecAnodenum 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_temperature 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_loopTimerTips 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_time 					= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleIcon_meshNode 				= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtn_Text 					= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBgPic_colorStyle 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBk_secMenu 				= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtn_devMulitSw_statusOn 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtn_devMulitSw_statusOff 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleIconvText_devMulitSw_statusOn = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleIconvText_devMulitSw_statusOff = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnImg_icon 				= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSliderBk_devDimmer_bg 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSliderBk_devDimmer_indic 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSliderBk_devDimmer_knob 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePhotoAk_devDimmer 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePhotoBk_devDimmer 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_devDimmer_SliderBk 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_devHeater_Bk_timeRemind = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_devCurtain_Bk_positionCurr = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_devCurtain_Bk_positionTips = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtn_devCurtain_statusPre 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtn_devCurtain_statusRel 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleImageBk_devCurtain_bkImgBody = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSliderBk_devCurtain_bg 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSliderBk_devCurtain_indic 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSliderBk_devCurtain_knob 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleImage_devFans_icon 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_devFans_instract 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnm_devFans_btnBg 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnm_devFans_btnRel 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnm_devFans_btnPre 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnm_devFans_btnTglRel 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnm_devFans_btnTglPre 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnm_devFans_btnIna 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleImage_devHeater_icon 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_devHeater_timeInstTar = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_devHeater_timeInstCur = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnm_devHeater_btnBg 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnm_devHeater_btnRel 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnm_devHeater_btnPre 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnm_devHeater_btnTglRel 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnm_devHeater_btnTglPre 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtnm_devHeater_btnIna 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePage_devHeater_timeSet 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleTextBtn_devHeater_timeSetPage = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleTextRoller_devHeater_timeSetPage_bg = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleTextRoller_devHeater_timeSetPage_sel = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleTextRollerTips_devHeater_timeSetPage = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLmeter_devThermostat_tempTarget = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLmeter_devThermostat_tempCurrent = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabel_devThermostat_tempTarget = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabel_devThermostat_tempCurrent = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSliderBk_devThermostat_bg = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSliderBk_devThermostat_indic = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleSliderBk_devThermostat_knob = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleTextBtnBk_devThermostat_tempAdj = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleCb_devThermostat_EcoEnText = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleCb_devThermostat_EcoEnBoxRel = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleCb_devThermostat_EcoEnBoxTgRel = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleRoller_thermostatGearSel_bg =  (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleRoller_thermostatGearSel_sel =  (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtn_thermostatVentFan_rel	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtn_thermostatVentFan_pre	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_thermostatEx_btnFold 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePreload_devScenario_driverCalmDown = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtn_devScenario_driverCalmDown = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleImgBk_underlying 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylebtnBk_transFull 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePage_sysRestartTips 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabelCounter_sysRestartTips = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabelRef_sysRestartTips 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleIconBinding_reserveIf 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePage_guiBlockTips		 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabelRef_guiBlockTips 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePreload_guiBlockTips 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));

	styleBroundPicOrg_prevStyle 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
}

static void lvGuiHome_styleApplicationInit(void){

	lvGuiHome_styleMemoryInitialization();

	//背景底图颜色
	lv_style_copy(styleBgPic_colorStyle, &lv_style_plain);
	styleBgPic_colorStyle->body.main_color = LV_COLOR_MAKE(0xe0, 0xd5, 0xce);
	styleBgPic_colorStyle->body.grad_color = LV_COLOR_MAKE(0xe0, 0xd5, 0xce);

	//图片风格设定：页眉常规图标
	lv_style_copy(styleImg_iconHeater, &lv_style_plain);
	styleImg_iconHeater->image.color = scrHeaderSysInfo_baseColor;
	styleImg_iconHeater->image.intense = LV_OPA_COVER;

	lv_style_copy(styleImg_iconWifi, &lv_style_plain);
	styleImg_iconWifi->image.color = scrHeaderSysInfo_baseColor;
	styleImg_iconWifi->image.intense = LV_OPA_COVER;

	//文字风格设定：页眉指示文字 电量及节点设备数量 
	lv_style_copy(styleText_elecAnodenum, &lv_style_plain);
	styleText_elecAnodenum->text.font = &lv_font_consola_13;
	styleText_elecAnodenum->text.color = scrHeaderSysInfo_baseColor;

	lv_style_copy(styleText_temperature, &lv_style_plain);
	styleText_temperature->text.font = &lv_font_consola_13;
	styleText_temperature->text.color = scrHeaderSysInfo_baseColor;

	//文字风格设定：页眉指示文字 时间
	lv_style_copy(styleText_time, &lv_style_plain);
	styleText_time->text.font = &lv_font_dejavu_20;
	styleText_time->text.color = scrHeaderSysInfo_baseColor;

	//文字风格设定：多位开关名称文字描述
	lv_style_copy(styleBtn_Text, &lv_style_plain);
	styleBtn_Text->text.font = &lv_font_arabic_17;
	styleBtn_Text->text.color = LV_COLOR_WHITE;

	//文字风格设定：调光开关亮度文字描述
	lv_style_copy(styleText_devDimmer_SliderBk, &lv_style_plain);
	styleText_devDimmer_SliderBk->text.font = &lv_font_dejavu_30;
	styleText_devDimmer_SliderBk->text.color = LV_COLOR_WHITE;

	//文字风格设定：定时响应对应定时类别文字描述
	lv_style_copy(styleText_loopTimerTips, &lv_style_plain);
	styleText_loopTimerTips->text.font = &lv_font_dejavu_20;
	styleText_loopTimerTips->text.color = LV_COLOR_WHITE;

	//文字风格设定：风扇开关对应档位文字描述
	lv_style_copy(styleText_devFans_instract, &lv_style_plain);
	styleText_devFans_instract->text.font = &lv_font_arialNum_100;
	styleText_devFans_instract->text.color = LV_COLOR_MAKE(153, 217, 234);

	//文字风格设定：热水器开关对应档位文字描述
	lv_style_copy(styleText_devHeater_timeInstTar, &lv_style_plain);
	styleText_devHeater_timeInstTar->text.font = &lv_font_consola_19;
	styleText_devHeater_timeInstTar->text.color = LV_COLOR_MAKE(255, 240, 155);
	lv_style_copy(styleText_devHeater_timeInstCur, &lv_style_plain);
	styleText_devHeater_timeInstCur->text.font = &lv_font_consola_19;
	styleText_devHeater_timeInstCur->text.color = LV_COLOR_MAKE(126, 220, 231);

	//文字风格设定：热水器开关对应延时关闭文字描述
	lv_style_copy(styleText_devHeater_Bk_timeRemind, &lv_style_plain);
	styleText_devHeater_Bk_timeRemind->text.font = &lv_font_consola_16;
	styleText_devHeater_Bk_timeRemind->text.color = LV_COLOR_ORANGE;

	//文字风格设定：窗帘位置信息描述文字
	lv_style_copy(styleText_devCurtain_Bk_positionCurr, &lv_style_plain);
	styleText_devCurtain_Bk_positionCurr->text.font = &lv_font_dejavu_40;
	styleText_devCurtain_Bk_positionCurr->text.color = LV_COLOR_MAKE(224, 224, 220);
	lv_style_copy(styleText_devCurtain_Bk_positionTips, &lv_style_plain);
	styleText_devCurtain_Bk_positionTips->text.font = &lv_font_consola_19;
	styleText_devCurtain_Bk_positionTips->text.color = LV_COLOR_WHITE;

	//文字风格设定：恒温器温度信息描述
	lv_style_copy(styleLabel_devThermostat_tempTarget, &lv_style_plain);
	styleLabel_devThermostat_tempTarget->text.font = &lv_font_arialNum_100;
	styleLabel_devThermostat_tempTarget->text.color = LV_COLOR_MAKE(255, 255, 0);
	lv_style_copy(styleLabel_devThermostat_tempCurrent, &lv_style_plain);
	styleLabel_devThermostat_tempCurrent->text.font = &lv_font_dejavu_30;
	styleLabel_devThermostat_tempCurrent->text.color = LV_COLOR_MAKE(0, 255, 0);
	lv_style_copy(styleTextBtnBk_devThermostat_tempAdj, &lv_style_plain);
	styleTextBtnBk_devThermostat_tempAdj->text.font = &lv_font_dejavu_30;
	styleTextBtnBk_devThermostat_tempAdj->text.color = LV_COLOR_MAKE(255, 255, 255);

	lv_style_copy(styleTextRollerTips_devHeater_timeSetPage, &lv_style_plain);
	styleTextRollerTips_devHeater_timeSetPage->text.font = &lv_font_arial_15;
	styleTextRollerTips_devHeater_timeSetPage->text.color = LV_COLOR_MAKE(80, 240, 80);

	//文字风格设定：恒温器扩展按键信息描述
	lv_style_copy(styleText_thermostatEx_btnFold, &lv_style_plain);
	styleText_thermostatEx_btnFold->text.font = &lv_font_dejavu_20;
	styleText_thermostatEx_btnFold->text.color = LV_COLOR_MAKE(255, 255, 255);

	//图片风格设定：页眉mesh节点图标颜色
	lv_style_copy(styleIcon_meshNode, &lv_style_plain);
	styleIcon_meshNode->image.color = LV_COLOR_RED;
	styleIcon_meshNode->image.intense = LV_OPA_COVER;

	//图片风格设定：多位开关按键 房间描述图案
	lv_style_copy(styleBtnImg_icon, &lv_style_plain);
	styleBtnImg_icon->image.color = LV_COLOR_WHITE;
	styleBtnImg_icon->image.intense = LV_OPA_COVER;

	//图片风格设定：多位开关按键 开关状态下按键底图
	lv_style_copy(styleBtn_devMulitSw_statusOn, &lv_style_plain);
	lv_style_copy(styleBtn_devMulitSw_statusOff, &lv_style_plain);
	styleBtn_devMulitSw_statusOn->image.color = LV_COLOR_SILVER;
	styleBtn_devMulitSw_statusOn->image.intense = LV_OPA_50;
	styleBtn_devMulitSw_statusOff->image.color = LV_COLOR_GRAY;
	styleBtn_devMulitSw_statusOff->image.intense = LV_OPA_50;

	//图片及文字风格设定：多位开关按键 开关状态下图标及文字风格
	lv_style_copy(styleIconvText_devMulitSw_statusOn, &lv_style_plain);
	lv_style_copy(styleIconvText_devMulitSw_statusOff, &lv_style_plain);
	styleIconvText_devMulitSw_statusOn->image.color = LV_COLOR_WHITE;
	styleIconvText_devMulitSw_statusOn->image.intense = LV_OPA_COVER;
	styleIconvText_devMulitSw_statusOn->text.color = LV_COLOR_WHITE;
	styleIconvText_devMulitSw_statusOff->image.color = LV_COLOR_BLACK;
	styleIconvText_devMulitSw_statusOff->image.intense = LV_OPA_COVER;
	styleIconvText_devMulitSw_statusOff->text.color = LV_COLOR_BLACK;

	//图片风格设定：窗帘开关按键 开关状态下按键底图
	lv_style_copy(styleBtn_devCurtain_statusPre, &lv_style_plain);
	lv_style_copy(styleBtn_devCurtain_statusRel, &lv_style_plain);
	styleBtn_devCurtain_statusPre->image.color = LV_COLOR_MAKE(128, 128, 255);
	styleBtn_devCurtain_statusPre->image.intense = LV_OPA_40;
	styleBtn_devCurtain_statusRel->image.color = LV_COLOR_GRAY;
	styleBtn_devCurtain_statusRel->image.intense = LV_OPA_30;

	//图片风格设定：窗帘图标风格
	lv_style_copy(styleImageBk_devCurtain_bkImgBody, &lv_style_plain);
	styleImageBk_devCurtain_bkImgBody->image.color = LV_COLOR_MAKE(224, 224, 220);
	styleImageBk_devCurtain_bkImgBody->image.intense = LV_OPA_COVER;

	//图片风格设定：风扇图标图片
	lv_style_copy(styleImage_devFans_icon, &lv_style_plain);
	styleImage_devFans_icon->image.color = LV_COLOR_MAKE(224, 224, 220);
	styleImage_devFans_icon->image.intense = LV_OPA_COVER;

	//图片风格设定：热水器图标图片
	lv_style_copy(styleImage_devHeater_icon, &lv_style_plain);
	styleImage_devHeater_icon->image.color = LV_COLOR_MAKE(224, 224, 220);
	styleImage_devHeater_icon->image.intense = LV_OPA_COVER;

	//图片风格设定：调光亮度提示图片底图
	lv_style_copy(stylePhotoAk_devDimmer, &lv_style_plain);
	stylePhotoBk_devDimmer->image.intense = LV_OPA_0;
	lv_style_copy(stylePhotoBk_devDimmer, &lv_style_plain);
	stylePhotoBk_devDimmer->image.opa = LV_OPA_0;

	//图片风格设定：背景图
	lv_style_copy(styleImgBk_underlying, &lv_style_plain);
	styleImgBk_underlying->image.intense = LV_OPA_50;
	styleImgBk_underlying->image.color = LV_COLOR_GRAY;
//	styleImgBk_underlying->image.intense = LV_OPA_0;
//	styleImgBk_underlying->image.opa = LV_OPA_0;

	//控间风格设定：按键透明
	stylebtnBk_transFull->body.main_color = LV_COLOR_TRANSP;
	stylebtnBk_transFull->body.grad_color = LV_COLOR_TRANSP;
	stylebtnBk_transFull->body.border.part = LV_BORDER_NONE;
    stylebtnBk_transFull->body.opa = LV_OPA_TRANSP;
	stylebtnBk_transFull->body.radius = 0;
    stylebtnBk_transFull->body.shadow.width =  0;

	//控件风格设定；恒温器线路表
	lv_style_copy(styleLmeter_devThermostat_tempTarget, &lv_style_pretty_color);
	styleLmeter_devThermostat_tempTarget->line.width = 2;
	styleLmeter_devThermostat_tempTarget->body.padding.hor = 10;
	styleLmeter_devThermostat_tempTarget->line.color = LV_COLOR_WHITE;
	styleLmeter_devThermostat_tempTarget->body.main_color = LV_COLOR_MAKE(0, 0, 255); 		/*Light blue*/
	styleLmeter_devThermostat_tempTarget->body.grad_color = LV_COLOR_MAKE(0, 0, 255);		/*Dark blue*/
	lv_style_copy(styleLmeter_devThermostat_tempCurrent, &lv_style_pretty_color);
	styleLmeter_devThermostat_tempCurrent->line.width = 2;
	styleLmeter_devThermostat_tempCurrent->body.padding.hor = 6;
	styleLmeter_devThermostat_tempCurrent->line.color = LV_COLOR_WHITE;
//	styleLmeter_devThermostat_tempCurrent->line.opa = LV_OPA_TRANSP;
	styleLmeter_devThermostat_tempCurrent->body.main_color = LV_COLOR_MAKE(0, 255, 0); 	/*Light blue*/
	styleLmeter_devThermostat_tempCurrent->body.grad_color = LV_COLOR_MAKE(0, 255, 0); 

	//控件风格设定：调光开关及窗帘开关滑动条
	lv_style_copy(styleSliderBk_devDimmer_bg, &lv_style_pretty);
	styleSliderBk_devDimmer_bg->body.main_color = LV_COLOR_BLACK;
	styleSliderBk_devDimmer_bg->body.grad_color = LV_COLOR_GRAY;
	styleSliderBk_devDimmer_bg->body.radius = LV_RADIUS_CIRCLE;
	styleSliderBk_devDimmer_bg->body.border.color = LV_COLOR_WHITE;
	lv_style_copy(styleSliderBk_devDimmer_indic, &lv_style_pretty);
	styleSliderBk_devDimmer_indic->body.grad_color = LV_COLOR_MAKE(64, 128, 128);
	styleSliderBk_devDimmer_indic->body.main_color = LV_COLOR_SILVER;
	styleSliderBk_devDimmer_indic->body.radius = LV_RADIUS_CIRCLE;
	styleSliderBk_devDimmer_indic->body.shadow.width = 10;
	styleSliderBk_devDimmer_indic->body.shadow.color = LV_COLOR_GRAY;
	styleSliderBk_devDimmer_indic->body.padding.hor = 3;
	styleSliderBk_devDimmer_indic->body.padding.ver = 3;
	lv_style_copy(styleSliderBk_devDimmer_knob, &lv_style_pretty);
	styleSliderBk_devDimmer_knob->body.radius = LV_RADIUS_CIRCLE;
	styleSliderBk_devDimmer_knob->body.opa = LV_OPA_70;
	styleSliderBk_devDimmer_knob->body.padding.ver = 10;

	lv_style_copy(styleSliderBk_devCurtain_bg, &lv_style_pretty);
	styleSliderBk_devCurtain_bg->body.main_color = LV_COLOR_BLACK;
	styleSliderBk_devCurtain_bg->body.grad_color = LV_COLOR_GRAY;
	styleSliderBk_devCurtain_bg->body.radius = LV_RADIUS_CIRCLE;
	styleSliderBk_devCurtain_bg->body.border.color = LV_COLOR_WHITE;
	lv_style_copy(styleSliderBk_devCurtain_indic, &lv_style_pretty);
	styleSliderBk_devCurtain_indic->body.grad_color = LV_COLOR_MAKE(64, 128, 128);
	styleSliderBk_devCurtain_indic->body.main_color = LV_COLOR_SILVER;
	styleSliderBk_devCurtain_indic->body.radius = LV_RADIUS_CIRCLE;
	styleSliderBk_devCurtain_indic->body.shadow.width = 10;
	styleSliderBk_devCurtain_indic->body.shadow.color = LV_COLOR_GRAY;
	styleSliderBk_devCurtain_indic->body.padding.hor = 3;
	styleSliderBk_devCurtain_indic->body.padding.ver = 3;
	lv_style_copy(styleSliderBk_devCurtain_knob, &lv_style_pretty);
	styleSliderBk_devCurtain_knob->body.radius = LV_RADIUS_CIRCLE;
	styleSliderBk_devCurtain_knob->body.opa = LV_OPA_70;
	styleSliderBk_devCurtain_knob->body.padding.ver = 10;

	lv_style_copy(styleSliderBk_devThermostat_bg, &lv_style_pretty);
	styleSliderBk_devThermostat_bg->body.main_color = LV_COLOR_BLACK;
	styleSliderBk_devThermostat_bg->body.grad_color = LV_COLOR_GRAY;
	styleSliderBk_devThermostat_bg->body.radius = LV_RADIUS_CIRCLE;
	styleSliderBk_devThermostat_bg->body.border.color = LV_COLOR_WHITE;
	lv_style_copy(styleSliderBk_devThermostat_indic, &lv_style_pretty);
	styleSliderBk_devThermostat_indic->body.grad_color = LV_COLOR_GREEN;
	styleSliderBk_devThermostat_indic->body.main_color = LV_COLOR_LIME;
	styleSliderBk_devThermostat_indic->body.radius = LV_RADIUS_CIRCLE;
	styleSliderBk_devThermostat_indic->body.shadow.width = 10;
	styleSliderBk_devThermostat_indic->body.shadow.color = LV_COLOR_LIME;
	styleSliderBk_devThermostat_indic->body.padding.hor = 3;
	styleSliderBk_devThermostat_indic->body.padding.ver = 3;
	lv_style_copy(styleSliderBk_devThermostat_knob, &lv_style_pretty);
	styleSliderBk_devThermostat_knob->body.radius = LV_RADIUS_CIRCLE;
	styleSliderBk_devThermostat_knob->body.opa = LV_OPA_70;
	styleSliderBk_devThermostat_knob->body.padding.ver = 10;

	lv_style_copy(styleCb_devThermostat_EcoEnText, &lv_style_plain);
	styleCb_devThermostat_EcoEnText->body.border.part = LV_BORDER_NONE;
	styleCb_devThermostat_EcoEnText->body.empty = 1;
	styleCb_devThermostat_EcoEnText->text.color = LV_COLOR_MAKE(128, 255, 255);
	styleCb_devThermostat_EcoEnText->text.font = &lv_font_consola_19;
	styleCb_devThermostat_EcoEnText->text.opa = LV_OPA_0;

	//控件风格设定：风扇开关档位按钮矩阵选定框背景、选中风格设置
    lv_style_copy(styleBtnm_devFans_btnBg, &lv_style_plain);
	styleBtnm_devFans_btnBg->body.opa = LV_OPA_70;
	styleBtnm_devFans_btnBg->body.radius = 4;
    styleBtnm_devFans_btnBg->body.main_color = lv_color_hsv_to_rgb(15, 11, 30);
    styleBtnm_devFans_btnBg->body.grad_color = lv_color_hsv_to_rgb(15, 11, 30);
    styleBtnm_devFans_btnBg->text.color = lv_color_hsv_to_rgb(15, 5, 95);

    lv_style_copy(styleBtnm_devFans_btnRel, &lv_style_pretty);
    styleBtnm_devFans_btnRel->body.main_color = lv_color_hsv_to_rgb(15, 10, 40);
    styleBtnm_devFans_btnRel->body.grad_color = lv_color_hsv_to_rgb(15, 10, 20);
    styleBtnm_devFans_btnRel->body.border.color = LV_COLOR_HEX3(0x111);
    styleBtnm_devFans_btnRel->body.border.width = 1;
    styleBtnm_devFans_btnRel->body.border.opa = LV_OPA_70;
    styleBtnm_devFans_btnRel->body.padding.hor = LV_DPI / 4;
    styleBtnm_devFans_btnRel->body.padding.ver = LV_DPI / 8;
    styleBtnm_devFans_btnRel->body.shadow.type = LV_SHADOW_BOTTOM;
    styleBtnm_devFans_btnRel->body.shadow.color = LV_COLOR_HEX3(0x111);
    styleBtnm_devFans_btnRel->body.shadow.width = LV_DPI / 30;
    styleBtnm_devFans_btnRel->text.color = LV_COLOR_HEX3(0xeee);

    lv_style_copy(styleBtnm_devFans_btnPre, styleBtnm_devFans_btnRel);
    styleBtnm_devFans_btnPre->body.main_color = lv_color_hsv_to_rgb(15, 10, 30);
    styleBtnm_devFans_btnPre->body.grad_color = lv_color_hsv_to_rgb(15, 10, 10);

    lv_style_copy(styleBtnm_devFans_btnTglRel, styleBtnm_devFans_btnRel);
    styleBtnm_devFans_btnTglRel->body.main_color = lv_color_hsv_to_rgb(15, 10, 20);
    styleBtnm_devFans_btnTglRel->body.grad_color = lv_color_hsv_to_rgb(15, 10, 40);
    styleBtnm_devFans_btnTglRel->body.shadow.width = LV_DPI / 40;
    styleBtnm_devFans_btnTglRel->text.color = LV_COLOR_HEX3(0xddd);

    lv_style_copy(styleBtnm_devFans_btnTglPre, styleBtnm_devFans_btnRel);
    styleBtnm_devFans_btnTglPre->body.main_color = lv_color_hsv_to_rgb(15, 10, 10);
    styleBtnm_devFans_btnTglPre->body.grad_color = lv_color_hsv_to_rgb(15, 10, 30);
    styleBtnm_devFans_btnTglPre->body.shadow.width = LV_DPI / 30;
    styleBtnm_devFans_btnTglPre->text.color = LV_COLOR_HEX3(0xddd);

    lv_style_copy(styleBtnm_devFans_btnIna, styleBtnm_devFans_btnRel);
    styleBtnm_devFans_btnIna->body.main_color = lv_color_hsv_to_rgb(15, 10, 20);
    styleBtnm_devFans_btnIna->body.grad_color = lv_color_hsv_to_rgb(15, 10, 20);
    styleBtnm_devFans_btnIna->text.color = LV_COLOR_HEX3(0xaaa);
    styleBtnm_devFans_btnIna->body.shadow.width = 0;

	lv_style_copy(stylePage_guiBlockTips, &lv_style_plain_color);
	stylePage_guiBlockTips->body.main_color = LV_COLOR_WHITE;
	stylePage_guiBlockTips->body.grad_color = LV_COLOR_WHITE;
	stylePage_guiBlockTips->body.border.part = LV_BORDER_NONE;
	stylePage_guiBlockTips->body.radius = 6;
	stylePage_guiBlockTips->body.opa = LV_OPA_90;
	stylePage_guiBlockTips->body.padding.hor = 0;
	stylePage_guiBlockTips->body.padding.inner = 0;	
	
	lv_style_copy(styleLabelRef_guiBlockTips, &lv_style_plain);
	styleLabelRef_guiBlockTips->text.font = &lv_font_consola_16;
	styleLabelRef_guiBlockTips->text.color = LV_COLOR_BLACK;

//    lv_style_copy(styleBtnm_devFans_btnBg, &lv_style_plain);
//    styleBtnm_devFans_btnBg->body.main_color = LV_COLOR_HEX3(0x333);
//    styleBtnm_devFans_btnBg->body.grad_color =  LV_COLOR_HEX3(0x333);
//    styleBtnm_devFans_btnBg->body.border.width = 2;
//    styleBtnm_devFans_btnBg->body.border.color =  LV_COLOR_HEX3(0x666);
//    styleBtnm_devFans_btnBg->body.shadow.color = LV_COLOR_SILVER;

//    lv_style_copy(styleBtnm_devFans_btnRel, &lv_style_plain);
//    styleBtnm_devFans_btnRel->glass = 0;
//    styleBtnm_devFans_btnRel->body.empty = 1;
//    styleBtnm_devFans_btnRel->body.radius = LV_RADIUS_CIRCLE;
//    styleBtnm_devFans_btnRel->body.border.width = 2;
//    styleBtnm_devFans_btnRel->body.border.color = lv_color_hsv_to_rgb(15, 70, 90);
//    styleBtnm_devFans_btnRel->body.border.opa = LV_OPA_80;
//    styleBtnm_devFans_btnRel->body.padding.hor = LV_DPI / 4;
//    styleBtnm_devFans_btnRel->body.padding.ver = LV_DPI / 6;
//    styleBtnm_devFans_btnRel->body.padding.inner = LV_DPI / 10;
//    styleBtnm_devFans_btnRel->text.color = lv_color_hsv_to_rgb(15, 8, 96);

//    lv_style_copy(styleBtnm_devFans_btnPre, styleBtnm_devFans_btnRel);
//    styleBtnm_devFans_btnPre->body.opa = LV_OPA_COVER;
//    styleBtnm_devFans_btnPre->body.empty = 0;
//    styleBtnm_devFans_btnPre->body.main_color = lv_color_hsv_to_rgb(15, 50, 50);
//    styleBtnm_devFans_btnPre->body.grad_color = lv_color_hsv_to_rgb(15, 50, 50);
//    styleBtnm_devFans_btnPre->body.border.opa = LV_OPA_60;
//    styleBtnm_devFans_btnPre->text.color = lv_color_hsv_to_rgb(15, 10, 100);

//    lv_style_copy(styleBtnm_devFans_btnTglRel, styleBtnm_devFans_btnPre);
//    styleBtnm_devFans_btnTglRel->body.opa = LV_OPA_COVER;
//    styleBtnm_devFans_btnTglRel->body.empty = 0;
//    styleBtnm_devFans_btnTglRel->body.main_color = lv_color_hsv_to_rgb(15, 50, 60);
//    styleBtnm_devFans_btnTglRel->body.grad_color = lv_color_hsv_to_rgb(15, 50, 60);
//    styleBtnm_devFans_btnTglRel->body.border.opa = LV_OPA_60;
//    styleBtnm_devFans_btnTglRel->body.border.color = lv_color_hsv_to_rgb(15, 80, 90);
//    styleBtnm_devFans_btnTglRel->text.color = lv_color_hsv_to_rgb(15, 0, 100);

//    lv_style_copy(styleBtnm_devFans_btnTglPre, styleBtnm_devFans_btnTglRel);
//    styleBtnm_devFans_btnTglPre->body.opa = LV_OPA_COVER;
//    styleBtnm_devFans_btnTglPre->body.empty = 0;
//    styleBtnm_devFans_btnTglPre->body.main_color = lv_color_hsv_to_rgb(15, 50, 50);
//    styleBtnm_devFans_btnTglPre->body.grad_color = lv_color_hsv_to_rgb(15, 50, 50);
//    styleBtnm_devFans_btnTglPre->body.border.opa = LV_OPA_60;
//    styleBtnm_devFans_btnTglPre->body.border.color = lv_color_hsv_to_rgb(15, 80, 70);
//    styleBtnm_devFans_btnTglPre->text.color = lv_color_hsv_to_rgb(15, 10, 90);

//    lv_style_copy(styleBtnm_devFans_btnIna, styleBtnm_devFans_btnRel);
//    styleBtnm_devFans_btnIna->body.border.opa = LV_OPA_60;
//    styleBtnm_devFans_btnIna->body.border.color = lv_color_hsv_to_rgb(15, 10, 50);
//    styleBtnm_devFans_btnIna->text.color = lv_color_hsv_to_rgb(15, 10, 90);

	//控件风格设定：热水器开关档位按钮矩阵选定框背景、选中风格设置
    lv_style_copy(styleBtnm_devHeater_btnBg, &lv_style_plain);
	styleBtnm_devHeater_btnBg->body.opa = LV_OPA_70;
	styleBtnm_devHeater_btnBg->body.radius = 4;
    styleBtnm_devHeater_btnBg->body.main_color = lv_color_hsv_to_rgb(15, 11, 30);
    styleBtnm_devHeater_btnBg->body.grad_color = lv_color_hsv_to_rgb(15, 11, 30);
    styleBtnm_devHeater_btnBg->text.color = lv_color_hsv_to_rgb(15, 5, 95);
	// styleBtnm_devHeater_btnBg->text.font = &lv_font_arial_15;

    lv_style_copy(styleBtnm_devHeater_btnRel, &lv_style_pretty);
    styleBtnm_devHeater_btnRel->body.main_color = lv_color_hsv_to_rgb(15, 10, 40);
    styleBtnm_devHeater_btnRel->body.grad_color = lv_color_hsv_to_rgb(15, 10, 20);
    styleBtnm_devHeater_btnRel->body.border.color = LV_COLOR_HEX3(0x111);
    styleBtnm_devHeater_btnRel->body.border.width = 1;
    styleBtnm_devHeater_btnRel->body.border.opa = LV_OPA_70;
    styleBtnm_devHeater_btnRel->body.padding.hor = LV_DPI / 4;
    styleBtnm_devHeater_btnRel->body.padding.ver = LV_DPI / 8;
    styleBtnm_devHeater_btnRel->body.shadow.type = LV_SHADOW_BOTTOM;
    styleBtnm_devHeater_btnRel->body.shadow.color = LV_COLOR_HEX3(0x111);
    styleBtnm_devHeater_btnRel->body.shadow.width = LV_DPI / 30;
    styleBtnm_devHeater_btnRel->text.color = LV_COLOR_HEX3(0xeee);

    lv_style_copy(styleBtnm_devHeater_btnPre, styleBtnm_devHeater_btnRel);
    styleBtnm_devHeater_btnPre->body.main_color = lv_color_hsv_to_rgb(15, 10, 30);
    styleBtnm_devHeater_btnPre->body.grad_color = lv_color_hsv_to_rgb(15, 10, 10);

    lv_style_copy(styleBtnm_devHeater_btnTglRel, styleBtnm_devHeater_btnRel);
    styleBtnm_devHeater_btnTglRel->body.main_color = lv_color_hsv_to_rgb(15, 10, 20);
    styleBtnm_devHeater_btnTglRel->body.grad_color = lv_color_hsv_to_rgb(15, 10, 40);
    styleBtnm_devHeater_btnTglRel->body.shadow.width = LV_DPI / 40;
    styleBtnm_devHeater_btnTglRel->text.color = LV_COLOR_MAKE(130, 251, 133);

    lv_style_copy(styleBtnm_devHeater_btnTglPre, styleBtnm_devHeater_btnRel);
    styleBtnm_devHeater_btnTglPre->body.main_color = lv_color_hsv_to_rgb(15, 10, 10);
    styleBtnm_devHeater_btnTglPre->body.grad_color = lv_color_hsv_to_rgb(15, 10, 30);
    styleBtnm_devHeater_btnTglPre->body.shadow.width = LV_DPI / 30;
    styleBtnm_devHeater_btnTglPre->text.color = LV_COLOR_MAKE(130, 251, 133);

    lv_style_copy(styleBtnm_devHeater_btnIna, styleBtnm_devHeater_btnRel);
    styleBtnm_devHeater_btnIna->body.main_color = lv_color_hsv_to_rgb(15, 10, 20);
    styleBtnm_devHeater_btnIna->body.grad_color = lv_color_hsv_to_rgb(15, 10, 20);
    styleBtnm_devHeater_btnIna->text.color = LV_COLOR_MAKE(130, 251, 133);
    styleBtnm_devHeater_btnIna->body.shadow.width = 0;

	//控件风格设定：热水器时间设置界面，滚轮设置
	lv_style_copy(styleTextRoller_devHeater_timeSetPage_bg, &lv_style_plain);
	styleTextRoller_devHeater_timeSetPage_bg->body.main_color = LV_COLOR_SILVER;
	styleTextRoller_devHeater_timeSetPage_bg->body.grad_color = LV_COLOR_SILVER;
	styleTextRoller_devHeater_timeSetPage_bg->body.opa = LV_OPA_90;
	styleTextRoller_devHeater_timeSetPage_bg->text.font = &lv_font_consola_19;
	styleTextRoller_devHeater_timeSetPage_bg->text.line_space = 5;
	styleTextRoller_devHeater_timeSetPage_bg->text.opa = LV_OPA_40;
	lv_style_copy(styleTextRoller_devHeater_timeSetPage_sel, &lv_style_plain);
	styleTextRoller_devHeater_timeSetPage_sel->body.empty = 1;
	styleTextRoller_devHeater_timeSetPage_sel->body.radius = 30;
	styleTextRoller_devHeater_timeSetPage_sel->body.opa = LV_OPA_90;
	styleTextRoller_devHeater_timeSetPage_sel->text.color = LV_COLOR_BLACK;
	styleTextRoller_devHeater_timeSetPage_sel->text.font = &lv_font_consola_19;

	//控件风格设定：热水器自定义时间设置界面
	lv_style_copy(stylePage_devHeater_timeSet, &lv_style_plain_color);
	stylePage_devHeater_timeSet->body.main_color = LV_COLOR_SILVER;
	stylePage_devHeater_timeSet->body.grad_color = LV_COLOR_SILVER;
	stylePage_devHeater_timeSet->body.border.part = LV_BORDER_NONE;
	stylePage_devHeater_timeSet->body.radius = 6;
	stylePage_devHeater_timeSet->body.opa = LV_OPA_90;
	stylePage_devHeater_timeSet->body.padding.hor = 0;
	stylePage_devHeater_timeSet->body.padding.inner = 0;	

	//控件风格设定：场景开关 触发冷却 预加载器 风格
	lv_style_copy(stylePreload_devScenario_driverCalmDown, &lv_style_plain);
	stylePreload_devScenario_driverCalmDown->line.width = 10;						  /*10 px thick arc*/
	stylePreload_devScenario_driverCalmDown->line.color = LV_COLOR_HEX3(0x258);	   	  /*Blueish arc color*/
	stylePreload_devScenario_driverCalmDown->body.border.color = LV_COLOR_HEX3(0xBBB); /*Gray background color*/
	stylePreload_devScenario_driverCalmDown->body.border.width = 10;
	stylePreload_devScenario_driverCalmDown->body.padding.hor = 0;

	//控件风格设定：界面阻塞加载等待
	lv_style_copy(stylePreload_guiBlockTips, &lv_style_plain);
	stylePreload_guiBlockTips->line.width = 8;						  /*10 px thick arc*/
	stylePreload_guiBlockTips->line.color = LV_COLOR_HEX3(0x258);	   	  /*Blueish arc color*/
	stylePreload_guiBlockTips->body.border.color = LV_COLOR_HEX3(0xBBB); /*Gray background color*/
	stylePreload_guiBlockTips->body.border.width = 8;
	stylePreload_guiBlockTips->body.padding.hor = 0;

	//控件风格设定：场景开关 触发冷却 按键图片 风格
	lv_style_copy(styleBtn_devScenario_driverCalmDown, &lv_style_plain);
	styleBtn_devScenario_driverCalmDown->image.color = LV_COLOR_YELLOW;
	styleBtn_devScenario_driverCalmDown->image.intense = LV_OPA_50;
	
	//文字风格设定：按钮字体
	lv_style_copy(styleTextBtn_devHeater_timeSetPage, &lv_style_plain);
	styleTextBtn_devHeater_timeSetPage->text.font = &lv_font_consola_17;
	styleTextBtn_devHeater_timeSetPage->text.color = LV_COLOR_MAKE(0, 128, 192);

	//控件风格设定：重启提示弹窗
	lv_style_copy(stylePage_sysRestartTips, &lv_style_plain_color);
	stylePage_sysRestartTips->body.main_color = LV_COLOR_WHITE;
	stylePage_sysRestartTips->body.grad_color = LV_COLOR_WHITE;
	stylePage_sysRestartTips->body.border.part = LV_BORDER_NONE;
	stylePage_sysRestartTips->body.radius = 6;
	stylePage_sysRestartTips->body.opa = LV_OPA_90;
	stylePage_sysRestartTips->body.padding.hor = 0;
	stylePage_sysRestartTips->body.padding.inner = 0;	

	lv_style_copy(styleLabelCounter_sysRestartTips, &lv_style_plain);
	styleLabelCounter_sysRestartTips->text.font = &lv_font_arialNum_100;
	styleLabelCounter_sysRestartTips->text.color = LV_COLOR_RED;

	lv_style_copy(styleLabelRef_sysRestartTips, &lv_style_plain);
	styleLabelRef_sysRestartTips->text.font = &lv_font_consola_17;
	styleLabelRef_sysRestartTips->text.color = LV_COLOR_BLACK;

	lv_style_copy(styleIconBinding_reserveIf, &lv_style_plain);
	styleIconBinding_reserveIf->image.color = LV_COLOR_YELLOW;
	styleIconBinding_reserveIf->image.intense = LV_OPA_50;

	//控件风格设定：恒温器工作模式档位选择
	lv_style_copy(styleRoller_thermostatGearSel_bg, &lv_style_plain);
	styleRoller_thermostatGearSel_bg->body.empty = 1;
	styleRoller_thermostatGearSel_bg->text.color = LV_COLOR_GRAY;
	styleRoller_thermostatGearSel_bg->text.font = &lv_font_consola_17;
	styleRoller_thermostatGearSel_bg->text.line_space = 5;
	styleRoller_thermostatGearSel_bg->text.opa = LV_OPA_COVER;
	lv_style_copy(styleRoller_thermostatGearSel_sel, &lv_style_plain);
	styleRoller_thermostatGearSel_sel->body.empty = 1;
	styleRoller_thermostatGearSel_sel->body.radius = 30;
	styleRoller_thermostatGearSel_sel->text.color = LV_COLOR_MAKE(192, 255, 0);
	styleRoller_thermostatGearSel_sel->text.font = &lv_font_dejavu_20;

	lv_style_copy(styleBtn_thermostatVentFan_rel, &lv_style_plain);
	styleBtn_thermostatVentFan_rel->image.color = LV_COLOR_WHITE;
	styleBtn_thermostatVentFan_rel->image.intense = LV_OPA_COVER;
	lv_style_copy(styleBtn_thermostatVentFan_pre, &styleBtn_thermostatVentFan_rel);
	styleBtn_thermostatVentFan_pre->image.color = LV_COLOR_MAKE(0x54, 0xc2, 0xba);

#if(SCREENSAVER_RUNNING_ENABLE == 1)

	lvglUsrApp_epidemicSituationDisp_styleInit();
	lvglUsrApp_screensaverNormal_A_styleInit();
#endif
}

void lvGui_usrAppElecParam_dispRefresh(void){

	elecParamDisp_record.elecParam_elecsum = 99999.123F;
	elecParamDisp_record.elecParam_power = 99999.123F;
}

void lvGui_upgradeTestDisp(void){

	xSemaphoreTake(xSph_lvglOpreat, portMAX_DELAY); // --lvgl[take up]

	lv_obj_t *label_upgrade = lv_label_create(lv_scr_act(), NULL);

	lv_label_set_recolor(label_upgrade, true);
	lv_label_set_text(label_upgrade, "#B5E61D hellow world#");

	lv_obj_align(label_upgrade, NULL, LV_ALIGN_CENTER, 0, 0);

	xSemaphoreGive(xSph_lvglOpreat); //   --lvgl[realse]

	while(1)vTaskDelay(100 / portTICK_PERIOD_MS);
}

static void lvGui_usrAppPowerUp_Animation(void){

	uint8_t loop = 0;

	xSemaphoreTake(xSph_lvglOpreat, portMAX_DELAY); // --lvgl[take up]

	lv_obj_t *animPic_A = lv_img_create(lv_scr_act(), NULL),
			 *animPic_B = NULL,
			 *versionDisp = NULL;

	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(animPic_A, 69, 65)):
		(lv_obj_set_pos(animPic_A, 25, 100));

#if(L8_STARTUP_PIC_LOGO_DEF == L8_STARTUP_PIC_LOGO_SPECIFY_MERIDSMAER)

	lv_img_set_src(animPic_A, &serPic_meridsmart);	
#else

	lv_img_set_src(animPic_A, &lanbonSealPic_B);	
#endif

	if(deviceFistRunningJudge_get()){ //首次运行，显示版本号

		versionDisp = lv_label_create(lv_scr_act(), NULL);
		stt_devStatusRecord devStatusRecordFlg_temp = {0};
		
		devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

		uiBlockFlg_newVersionRunningConfirm = true;

		devStatusRecordFlg_temp.devUpgradeFirstRunning_FLG = 1;

		lv_style_copy(styleText_versionDiscrib, &lv_style_plain);
		styleText_versionDiscrib->text.font = &lv_font_consola_16;
		styleText_versionDiscrib->text.color = LV_COLOR_MAKE(34, 177, 76);
		lv_label_set_style(versionDisp, styleText_versionDiscrib);

		lv_label_set_recolor(versionDisp, true);
		lv_label_set_text(versionDisp, "new version:\n  "L8_DEVICE_VERSION_REF_DISCRIPTION);
		(devStatusDispMethod_landscapeIf_get())?
			(lv_obj_align(versionDisp, animPic_A, LV_ALIGN_OUT_BOTTOM_MID, 0, 50)):
			(lv_obj_align(versionDisp, animPic_A, LV_ALIGN_OUT_BOTTOM_MID, 0, 80));
		
		devStatusRecordIF_paramSet(&devStatusRecordFlg_temp, true);
		vTaskDelay(200 / portTICK_PERIOD_MS);
	}

	xSemaphoreGive(xSph_lvglOpreat); //   --lvgl[realse]
	
	for(loop = 0; loop < 2; loop ++){

		devScreenBkLight_weakUp();
		vTaskDelay(750 / portTICK_PERIOD_MS);
	}

	xSemaphoreTake(xSph_lvglOpreat, portMAX_DELAY); // --lvgl[take up]

#if(L8_STARTUP_PIC_LOGO_DEF == L8_STARTUP_PIC_LOGO_ROUTINE)

	animPic_B = lv_img_create(lv_scr_act(), NULL),
	lv_img_set_src(animPic_B, &lanbonSealPic_A);
	lv_obj_set_protect(animPic_B, LV_PROTECT_POS);
	lv_obj_align(animPic_B, animPic_A, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	xSemaphoreGive(xSph_lvglOpreat); //   --lvgl[realse]

	for(loop = 0; loop < 170; loop ++){

		xSemaphoreTake(xSph_lvglOpreat, portMAX_DELAY); // --lvgl[take up]
		lv_obj_set_width(animPic_B, loop);
		xSemaphoreGive(xSph_lvglOpreat); //   --lvgl[realse]
		vTaskDelay(2 / portTICK_PERIOD_MS);
	}
	
	for(loop = 0; loop < 3; loop ++){

		devScreenBkLight_weakUp();
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

	xSemaphoreTake(xSph_lvglOpreat, portMAX_DELAY); // --lvgl[take up]
#else

	for(loop = 0; loop < 6; loop ++){

		devScreenBkLight_weakUp();
		vTaskDelay(600 / portTICK_PERIOD_MS);
	}
#endif

	if(versionDisp){lv_obj_del(versionDisp); versionDisp = NULL;}
	if(animPic_A){lv_obj_del(animPic_A); animPic_A = NULL;}
	if(animPic_B){lv_obj_del(animPic_B); animPic_B = NULL;}

	xSemaphoreGive(xSph_lvglOpreat); //   --lvgl[realse]

	lvglUsrApp_newVersionRunningConfirm();
}

void lvGui_businessInit(void){

	lvglUsrApp_uiInitialize();

//	lvglUsrApp_screensaverNormal_A_styleInit();
//	lvglUsrApp_screensaverNormal_A_disp(true);
//	while(1);

	xTaskCreate(task_guiSwitch_Detecting, //Task Function
				"guiDetect", //Task Name
				1024 * 6,	 //Stack Depth
				NULL,		 //Parameters
				CONFIG_MDF_TASK_DEFAULT_PRIOTY,	//Priority  
				NULL);		 //Task Handler

	usrAppHomepageThemeType_Set(bGroundImg_themeParam.bGround_keyTheme_ist, false, false); //数据自更新，因为guiInit在dataInit之后，对象创建较晚

//	vTaskDelay(3000 / portTICK_PERIOD_MS);
//	lv_obj_clean(imageBK);

//	printf("Qr code creat res:%d.\n", EncodeData("hellow, Lanbon!"));

//	for(uint8_t loopa = 0; loopa < MAX_MODULESIZE; loopa ++){

//		for(uint8_t loopb = 0; loopb < MAX_MODULESIZE; loopb ++){

//			(m_byModuleData[loopa][loopb])?
//				(externSocket_ex_disp_fill(QR_BASIC_POSITION_X + loopa * QR_PIXEL_SIZE, QR_BASIC_POSITION_Y + loopb * QR_PIXEL_SIZE, QR_BASIC_POSITION_X + QR_PIXEL_SIZE + loopa * QR_PIXEL_SIZE, QR_BASIC_POSITION_Y + QR_PIXEL_SIZE + loopb * QR_PIXEL_SIZE, LV_COLOR_BLACK)):
//				(externSocket_ex_disp_fill(QR_BASIC_POSITION_X + loopa * QR_PIXEL_SIZE, QR_BASIC_POSITION_Y + loopb * QR_PIXEL_SIZE, QR_BASIC_POSITION_X + QR_PIXEL_SIZE + loopa * QR_PIXEL_SIZE, QR_BASIC_POSITION_Y + QR_PIXEL_SIZE + loopb * QR_PIXEL_SIZE, LV_COLOR_WHITE));
//		}
//	}

//	lv_obj_t * btn = lv_imgbtn_create(lv_scr_act(), NULL);
//	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, &testPic_P);
//	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, &testPic);

//	for(uint8_t loop = 0; loop < 20; loop ++){

//		vTaskDelay(1000 / portTICK_PERIOD_MS);
//		(!(loop % 2))?(lv_img_set_src(la, &testPic)):(lv_img_set_src(la, &testPic_P));
//	}
//	
//	lv_obj_set_width(wp, LV_HOR_RES * 4);
//	lv_obj_set_protect(wp, LV_PROTECT_POS);

//	lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
//	lv_obj_set_pos(label, 20, 100);
//	lv_label_set_text(label, "1245AE6BCC");
}
