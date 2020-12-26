/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>

#include "mdf_common.h"

#include "sdkconfig.h"

/* lvgl includes */
#include "iot_lvgl.h"

#include "devDriver_manage.h"

#include "dataTrans_remoteServer.h"
#include "dataTrans_localHandler.h"
#include "dataTrans_meshUpgrade.h"
#include "appLaunch_HT.h"

#include "gui_businessMenu_settingSet.h"os_zalloc

#include "gui_businessHome.h"
#include "gui_businessReuse_reactionObjPage.h"
	
#define OBJ_DDLIST_DEVTYPE_FREENUM			1
#define OBJ_DDLIST_HPTHEME_FREENUM			2

#define FUNCTION_NUM_DEF_SCREENLIGHT_TIME	13

LV_FONT_DECLARE(lv_font_dejavu_15);
LV_FONT_DECLARE(lv_font_consola_13);
LV_FONT_DECLARE(lv_font_consola_16);
LV_FONT_DECLARE(lv_font_consola_17);
LV_FONT_DECLARE(lv_font_consola_19);
LV_FONT_DECLARE(lv_font_ariblk_18);
LV_FONT_DECLARE(lv_font_arial_15);

LV_IMG_DECLARE(iconMenu_funBack_arrowLeft);
LV_IMG_DECLARE(iconMenu_funBack_homePage);
LV_IMG_DECLARE(imageBtn_feedBackNormal);
LV_IMG_DECLARE(bGroundPrev_picFigure_sel);

static const char *deviceType_listTab = {

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_ALL_RESERVE) 

	"Switch-1bit\n"
	"Switch-2bit\n"
	"Switch-3bit\n"
	"Dimmer\n"
	"Fans\n"
	"Scenario\n"
	"Curtain\n" 
	"Heater\n"
	"Thermostat\n"
	"ThermostatEx"
		
#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_SHARE_MIX)

	"Switch-1bit\n"
	"Switch-2bit\n"
	"Switch-3bit\n"
	"Curtain\n" 
	"Scenario"

#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_FANS)
	
	"Fans"
	
#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_DIMMER)
	
	"Dimmer"
	
#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_HEATER)

	"Heater"

#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED)

	"infrared"

#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_SOCKET)

	"socket"

#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_MOUDLE)

	"moudle 3 bit"

#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_RELAY_BOX)

	"relay 3 bit"

#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_MULIT_THERMO)

 #if(DEVICE_THERMOSTAT_PEDESTAL_HEATER_RESERVE == 1)

	"Thermostat"
 #else
 
	"Thermostat\n"
	"ThermostatEx"
 #endif

#elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)

	"Thermostat"

#endif
};

static const char *homepageThemeStyle_listTab = {

	"theme-A\n"
	"theme-B\n"
	"theme-C"
};

static const char *mbox_btnm_textTab[] ={"Yes", "No", ""}; /*Button description. '\221' lv_btnm like control char*/

static const struct stt_deviceTypeTab_disp{

	devTypeDef_enum devType;
	uint8_t devInst_disp;
	
}deviceTypeTab_disp[DEVICE_TYPE_LIST_NUM] = {

 #if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_ALL_RESERVE) 
	
	{devTypeDef_mulitSwOneBit,			0},
	{devTypeDef_mulitSwTwoBit,			1},
	{devTypeDef_mulitSwThreeBit,		2},
	{devTypeDef_dimmer, 				3},
	{devTypeDef_fans,					4},
	{devTypeDef_scenario,				5},
	{devTypeDef_curtain,				6},
	{devTypeDef_heater, 				7}, 
	{devTypeDef_thermostat, 			8}, 
	{devTypeDef_thermostatExtension, 	9}, 

 #elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_SHARE_MIX)

	{devTypeDef_mulitSwOneBit, 	 		0},
	{devTypeDef_mulitSwTwoBit, 	 		1},
	{devTypeDef_mulitSwThreeBit,		2},
	{devTypeDef_curtain,				3}, 
	{devTypeDef_scenario,				4},
	
 #elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_FANS)
		
	{devTypeDef_fans,					0},
		
 #elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_DIMMER)
		
	{devTypeDef_dimmer, 				0},
		
 #elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_HEATER)
	
	{devTypeDef_heater, 				0}, 

 #elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_MULIT_THERMO)

	{devTypeDef_thermostat,		 		0}, 
	{devTypeDef_thermostatExtension,	1}, 

 #elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)

	{devTypeDef_thermostat,		 		0}, 
 
 #endif
};

static const struct stt_deviceTypeStr_disp{

	devTypeDef_enum devType;
	char *devName;
	
}deviceTypeStr_disp[DEVICE_TYPE_LIST_NUM + 1] = {

 #if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_ALL_RESERVE) 
	
	{devTypeDef_mulitSwOneBit,			"1 gang switch"},
	{devTypeDef_mulitSwTwoBit,			"2 gang switch"},
	{devTypeDef_mulitSwThreeBit,		"3 gang switch"},
	{devTypeDef_dimmer, 				"Dimmer"},
	{devTypeDef_fans,					"Fans"},
	{devTypeDef_scenario,				"Scene"},
	{devTypeDef_curtain,				"Curtain"},
	{devTypeDef_heater, 				"Heater"}, 
	{devTypeDef_thermostat, 			"Thermostat"}, 
	{devTypeDef_thermostatExtension, 	"ThermostatEx"}, 
	{devTypeDef_null, 					NULL}, 

 #elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_SHARE_MIX)

	{devTypeDef_mulitSwOneBit,			"1 gang switch"},
	{devTypeDef_mulitSwTwoBit,			"2 gang switch"},
	{devTypeDef_mulitSwThreeBit,		"3 gang switch"},
	{devTypeDef_curtain,				"Curtain"},
	// {devTypeDef_scenario,				"Scene"},
	{devTypeDef_null, 					NULL}, 
	
 #elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_FANS)
		
	{devTypeDef_fans,					"Fans"},
	{devTypeDef_null, 					NULL}, 
		
 #elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_DIMMER)
		
	{devTypeDef_dimmer, 				"Dimmer"},
	{devTypeDef_null, 					NULL}, 
		
 #elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_HEATER)
	
	{devTypeDef_heater, 				"Heater"}, 
	{devTypeDef_null, 					NULL}, 

 #elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_MULIT_THERMO)

   #if(DEVICE_THERMOSTAT_PEDESTAL_HEATER_RESERVE == 1)

	{devTypeDef_thermostat, 			"Thermostat"}, 
	{devTypeDef_null, 					NULL}, 
   #else
   
	{devTypeDef_thermostat, 			"Thermostat"}, 
	{devTypeDef_thermostatExtension, 	"ThermostatEx"}, 
	{devTypeDef_null, 					NULL}, 
   #endif

 #elif(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)

	{devTypeDef_thermostat, 			"Thermostat"}, 
	{devTypeDef_null, 					NULL}, 

 #endif
};

static const struct stt_gearScreenLightTime{

	uint16_t screenLightTime2Sec;
	char strDisp_ref[60];
	
}screenLightTimeGear_refTab[FUNCTION_NUM_DEF_SCREENLIGHT_TIME] = {

	{60 * 00 + 1 * 10, "#FFFF00 00##C0C0FF min##FFFF00 10##C0C0FF sec#"},
	{60 * 00 + 1 * 20, "#FFFF00 00##C0C0FF min##FFFF00 20##C0C0FF sec#"},
	{60 * 00 + 1 * 30, "#FFFF00 00##C0C0FF min##FFFF00 30##C0C0FF sec#"},
	{60 * 01 + 1 * 00, "#FFFF00 01##C0C0FF min##FFFF00 00##C0C0FF sec#"},
	{60 * 01 + 1 * 30, "#FFFF00 01##C0C0FF min##FFFF00 30##C0C0FF sec#"},
	{60 * 02 + 1 * 00, "#FFFF00 02##C0C0FF min##FFFF00 00##C0C0FF sec#"},
	{60 * 02 + 1 * 30, "#FFFF00 02##C0C0FF min##FFFF00 30##C0C0FF sec#"},
	{60 * 03 + 1 * 00, "#FFFF00 03##C0C0FF min##FFFF00 00##C0C0FF sec#"},
	{60 * 05 + 1 * 00, "#FFFF00 05##C0C0FF min##FFFF00 00##C0C0FF sec#"},
	{60 * 10 + 1 * 00, "#FFFF00 10##C0C0FF min##FFFF00 00##C0C0FF sec#"},
	{60 * 20 + 1 * 00, "#FFFF00 20##C0C0FF min##FFFF00 00##C0C0FF sec#"},
	{60 * 30 + 1 * 00, "#FFFF00 30##C0C0FF min##FFFF00 00##C0C0FF sec#"},
	{COUNTER_DISENABLE_MASK_SPECIALVAL_U16, 	    "#FFFF00   ALWAYS#"}
};

 #define EPID_STATISTICAL_COUNTRY_SUM		226

 static const char epidCyAbbre[EPID_STATISTICAL_COUNTRY_SUM][3] = {

	 "AF","AX","AL","DZ","AS","AD","AO","AI","AG","AR","AM","AW","AU",
	 "AT","AZ","BD","BH","BS","BB","BY","BE","BZ","BJ","BM","BT","BO",
	 "BA","BW","BV","BR","BN","BG","BF","BI","KH","CM","CA","CV","CF",
	 "TD","CL","CX","CC","CO","KM","CD","CG","CK","CR","CI","CN","HR",
	 "CU","CZ","CY","DK","DJ","DM","DW","EC","EG","GQ","ER","EE","ET",
	 "FO","FJ","FI","FR","FX","GF","PF","GA","GM","GE","DE","GH","GI",
	 "GR","GD","GP","GU","GT","GG","GW","GN","GY","HT","HN","HU","IS",
	 "IN","ID","IR","IQ","IE","IM","IL","IT","JM","JP","JE","JO","KZ",
	 "KE","KI","KR","KD","KW","KG","LO","LV","LB","LS","LR","LY","LI",
	 "LT","LU","MK","MW","MY","MG","MV","ML","MT","MH","MQ","MR","MU",
	 "YT","MX","MF","MD","MC","MN","ME","MS","MA","MZ","MM","NA","NR",
	 "NP","NL","NC","NZ","NI","NE","NG","NU","NF","NO","OM","PK","PW",
	 "PS","PA","PG","PE","PH","PN","PL","PT","PR","QA","RE","RO","RW",
	 "RU","SH","KN","LC","VG","SV","WS","SM","ST","SA","SN","SC","SL",
	 "SG","RS","SK","SI","SB","SO","ZA","ES","LK","SD","SR","SZ","SE",
	 "CH","SY","TJ","TZ","TH","TT","TL","TG","TK","TO","TN","TR","TM",
	 "TV","UG","UA","AE","UK","US","UY","UZ","VN","VA","VE","VN","WF",
	 "EH","YE","YU","ZM","ZW"
 };

 static const char epidCyName[] = {

	 "Afghanistan\n""Aland Islands\n""Albania\n""Algeria\n""American Samoa\n""Andorra\n"
	 "Angola\n""Anguilla\n""Antigua and Barbuda\n""Argentina\n""Armenia\n""Aruba\n"
	 "Australia\n""Austria\n""Azerbaijan\n""Bangladesh\n""Bahrain\n""Bahamas\n""Barbados\n"
	 "Belarus\n""Belgium\n""Belize\n""Benin\n""Bermuda\n""Bhutan\n""Bolivia\n"
	 "Bosnia and Herzegovina\n""Botswana\n""Bouvet Island\n""Brazil\n""Brunei\n"
	 "Bulgaria\n""Burkina Faso\n""Burundi\n""Cambodia\n""Cameroon\n""Canada\n"
	 "Cape Verde\n""Central African Republic\n""Chad\n""Chile\n""Christmas Islands\n"
	 "Cocos (keeling) Islands\n""Colombia\n""Comoros\n""Congo (Congo-Kinshasa)\n"
	 "Congo\n""Cook Islands\n""Costa Rica\n""Cote D'Ivoire\n""China\n""Croatia\n"
	 "Cuba\n""Czech\n""Cyprus\n""Denmark\n""Djibouti\n""Dominica\n""East Timor\n"
	 "Ecuador\n""Egypt\n""Equatorial Guinea\n""Eritrea\n""Estonia\n""Ethiopia\n"
	 "Faroe Islands\n""Fiji\n""Finland\n""France\n""MetropolitanFrance\n""French Guiana\n"
	 "French Polynesia\n""Gabon\n""Gambia\n""Georgia\n""Germany\n""Ghana\n"
	 "Gibraltar\n""Greece\n""Grenada\n""Guadeloupe\n""Guam\n""Guatemala\n""Guernsey\n"
	 "Guinea-Bissau\n""Guinea\n""Guyana\n""Haiti\n""Honduras\n""Hungary\n""Iceland\n"
	 "India\n""Indonesia\n""Iran\n""Iraq\n""Ireland\n""Isle of Man\n""Israel\n""Italy\n"
	 "Jamaica\n""Japan\n""Jersey\n""Jordan\n""Kazakhstan\n""Kenya\n""Kiribati\n"
	 "Korea (South)\n""Korea (North)\n""Kuwait\n""Kyrgyzstan\n""Laos\n""Latvia\n"
	 "Lebanon\n""Lesotho\n""Liberia\n""Libya\n""Liechtenstein\n""Lithuania\n""Luxembourg\n"
	 "Macedonia\n""Malawi\n""Malaysia\n""Madagascar\n""Maldives\n""Mali\n""Malta\n"
	 "Marshall Islands\n""Martinique\n""Mauritania\n""Mauritius\n""Mayotte\n""Mexico\n"
	 "Micronesia\n""Moldova\n""Monaco\n""Mongolia\n""Montenegro\n""Montserrat\n"
	 "Morocco\n""Mozambique\n""Myanmar\n""Namibia\n""Nauru\n""Nepal\n""Netherlands\n"
	 "New Caledonia\n""New Zealand\n""Nicaragua\n""Niger\n""Nigeria\n""Niue\n"
	 "Norfolk Island\n""Norway\n""Oman\n""Pakistan\n""Palau\n""Palestine\n""Panama\n"
	 "Papua New Guinea\n""Peru\n""Philippines\n""Pitcairn Islands\n""Poland\n""Portugal\n"
	 "Puerto Rico\n""Qatar\n""Reunion\n""Romania\n""Rwanda\n""Russian Federation\n"
	 "Saint Helena\n""Saint Kitts-Nevis\n""Saint Lucia\n""Saint Vincent and the Grenadines\n"
	 "El Salvador\n""Samoa\n""San Marino\n""Sao Tome and Principe\n""Saudi Arabia\n"
	 "Senegal\n""Seychelles\n""Sierra Leone\n""Singapore\n""Serbia\n""Slovakia\n"
	 "Slovenia\n""Solomon Islands\n""Somalia\n""South Africa\n""Spain\n""Sri Lanka\n"
	 "Sudan\n""Suriname\n""Swaziland\n""Sweden\n""Switzerland\n""Syria\n""Tajikistan\n"
	 "Tanzania\n""Thailand\n""Trinidad and Tobago\n""Timor-Leste\n""Togo\n""Tokelau\n"
	 "Tonga\n""Tunisia\n""Turkey\n""Turkmenistan\n""Tuvalu\n""Uganda\n""Ukraine\n"
	 "United Arab Emirates\n""United Kingdom\n""United States\n""Uruguay\n""Uzbekistan\n"
	 "Vanuatu\n""Vatican City\n""Venezuela\n""Vietnam\n""Wallis and Futuna\n"
	 "Western Sahara\n""Yemen\n""Yugoslavia\n""Zambia\n""Zimbabwe"
 };
 
// static const struct stt_epidCyName{

//	char cyName[36];
//	char cyAbbre[2];
//	
// }epidCountryList[EPID_STATISTICAL_COUNTRY_SUM] = {

//	 {"Afghanistan","AF"},	{"Aland Islands","AX"},		{"Albania","AL"},
//	 {"Algeria","DZ"},		{"American Samoa","AS"},	{"Andorra","AD"},
//	 {"Angola","AO"},		{"Anguilla","AI"},			{"Antigua and Barbuda","AG"},
//	 {"Argentina","AR"},	{"Armenia","AM"},			{"Aruba","AW"},
//	 {"Australia","AU"},	{"Austria","AT"},			{"Azerbaijan","AZ"},	
//	 {"Bangladesh","BD"},	{"Bahrain","BH"},			{"Bahamas","BS"},		
//	 {"Barbados","BB"},		{"Belarus","BY"},			{"Belgium","BE"},
//	 {"Belize","BZ"},		{"Benin","BJ"},				{"Bermuda","BM"},
//	 {"Bhutan","BT"},		{"Bolivia","BO"},			{"Bosnia and Herzegovina","BA"},
//	 {"Botswana","BW"},		{"Bouvet Island","BV"},		{"Brazil","BR"},
//	 {"Brunei","BN"},		{"Bulgaria","BG"},			{"Burkina Faso","BF"},
//	 {"Burundi","BI"},		{"Cambodia","KH"},			{"Cameroon","CM"},
//	 {"Canada","CA"},		{"Cape Verde","CV"},		{"Central African Republic","CF"},
//	 {"Chad","TD"},			{"Chile","CL"},				{"Christmas Islands","CX"},
//	 {"Cocos (keeling) Islands","CC"}, {"Colombia","CO"},{"Comoros","KM"},
//	 {"Congo (Congo-Kinshasa)","CD"},  {"Congo","CG"},	{"Cook Islands","CK"},
//	 {"Costa Rica","CR"},	{"Cote D'Ivoire","CI"},		{"China","CN"},
//	 {"Croatia","HR"},		{"Cuba","CU"},				{"Czech","CZ"},
//	 {"Cyprus","CY"},		{"Denmark","DK"},			{"Djibouti","DJ"},
//	 {"Dominica","DM"},		{"East Timor",""},			{"Ecuador","EC"},
//	 {"Egypt","EG"},		{"Equatorial Guinea","GQ"},	{"Eritrea","ER"},
//	 {"Estonia","EE"},		{"Ethiopia","ET"},			{"Faroe Islands","FO"},
//	 {"Fiji","FJ"},			{"Finland","FI"},			{"France","FR"},
//	 {"MetropolitanFrance","FX"},{"French Guiana","GF"},{"French Polynesia","PF"},
//	 {"Gabon","GA"},		{"Gambia","GM"},			{"Georgia","GE"},
//	 {"Germany","DE"},		{"Ghana","GH"},				{"Gibraltar","GI"},
//	 {"Greece","GR"},		{"Grenada","GD"},			{"Guadeloupe","GP"},
//	 {"Guam","GU"},			{"Guatemala","GT"},			{"Guernsey","GG"},
//	 {"Guinea-Bissau","GW"},{"Guinea","GN"},			{"Guyana","GY"},
//	 {"Haiti","HT"},		{"Honduras","HN"},			{"Hungary","HU"},
//	 {"Iceland","IS"},		{"India","IN"},				{"Indonesia","ID"},
//	 {"Iran","IR"},			{"Iraq","IQ"},				{"Ireland","IE"},
//	 {"Isle of Man","IM"},	{"Israel","IL"},			{"Italy","IT"},
//	 {"Jamaica","JM"},		{"Japan","JP"},				{"Jersey","JE"},
//	 {"Jordan","JO"},		{"Kazakhstan","KZ"},		{"Kenya","KE"},		
//	 {"Kiribati","KI"},		{"Korea (South)","KR"},		{"Korea (North)","KD"},
//	 {"Kuwait","KW"},		{"Kyrgyzstan","KG"},		{"Laos","LO"},
//	 {"Latvia","LV"},		{"Lebanon","LB"},			{"Lesotho","LS"},
//	 {"Liberia","LR"},		{"Libya","LY"},				{"Liechtenstein","LI"},
//	 {"Lithuania","LT"},	{"Luxembourg","LU"},		{"Macedonia","MK"},
//	 {"Malawi","MW"},		{"Malaysia","MY"},			{"Madagascar","MG"},
//	 {"Maldives","MV"},		{"Mali","ML"},				{"Malta","MT"},
//	 {"Marshall Islands","MH"},{"Martinique","MQ"},		{"Mauritania","MR"},
//	 {"Mauritius","MU"},	{"Mayotte","YT"},			{"Mexico","MX"},
//	 {"Micronesia","MF"},	{"Moldova","MD"},			{"Monaco","MC"},
//	 {"Mongolia","MN"},		{"Montenegro","ME"},		{"Montserrat","MS"},
//	 {"Morocco","MA"},		{"Mozambique","MZ"},		{"Myanmar","MM"},
//	 {"Namibia","NA"},		{"Nauru","NR"},				{"Nepal","NP"},
//	 {"Netherlands","NL"},	{"New Caledonia","NC"},		{"New Zealand","NZ"},
//	 {"Nicaragua","NI"},	{"Niger","NE"},				{"Nigeria","NG"},
//	 {"Niue","NU"},			{"Norfolk Island","NF"},	{"Norway","NO"},
//	 {"Oman","OM"},			{"Pakistan","PK"},			{"Palau","PW"},
//	 {"Palestine","PS"},	{"Panama","PA"},			{"Papua New Guinea","PG"},
//	 {"Peru","PE"},			{"Philippines","PH"},		{"Pitcairn Islands","PN"},
//	 {"Poland","PL"},		{"Portugal","PT"},			{"Puerto Rico","PR"},
//	 {"Qatar","QA"},		{"Reunion","RE"},			{"Romania","RO"},
//	 {"Rwanda","RW"},		{"Russian Federation","RU"},{"Saint Helena","SH"},
//	 {"Saint Kitts-Nevis","KN"},{"Saint Lucia","LC"},	{"Saint Vincent and the Grenadines","VG"},
//	 {"El Salvador","SV"},	{"Samoa","WS"},				{"San Marino","SM"},
//	 {"Sao Tome and Principe","ST"},{"Saudi Arabia","SA"},{"Senegal","SN"},
//	 {"Seychelles","SC"},	{"Sierra Leone","SL"},		{"Singapore","SG"},
//	 {"Serbia","RS"},		{"Slovakia","SK"},			{"Slovenia","SI"},
//	 {"Solomon Islands","SB"},{"Somalia","SO"},			{"South Africa","ZA"},
//	 {"Spain","ES"},		{"Sri Lanka","LK"},			{"Sudan","SD"},
//	 {"Suriname","SR"},		{"Swaziland","SZ"},			{"Sweden","SE"},
//	 {"Switzerland","CH"},	{"Syria","SY"},				{"Tajikistan","TJ"},
//	 {"Tanzania","TZ"},		{"Thailand","TH"},			{"Trinidad and Tobago","TT"},
//	 {"Timor-Leste","TL"},	{"Togo","TG"},				{"Tokelau","TK"},
//	 {"Tonga","TO"},		{"Tunisia","TN"},			{"Turkey","TR"},
//	 {"Turkmenistan","TM"},	{"Tuvalu","TV"},			{"Uganda","UG"},
//	 {"Ukraine","UA"},		{"United Arab Emirates","AE"},{"United Kingdom","UK"},
//	 {"United States","US"},{"Uruguay","UY"},			{"Uzbekistan","UZ"},
//	 {"Vanuatu","VN"},		{"Vatican City","VA"},		{"Venezuela","VE"},
//	 {"Vietnam","VN"},		{"Wallis and Futuna","WF"},	{"Western Sahara","EH"},
//	 {"Yemen","YE"},		{"Yugoslavia","YU"},		{"Zambia","ZM"},
//	 {"Zimbabwe","ZW"}
// };

static const uint8_t screenBrightness_sliderAdj_div = 20;

static uint8_t devTypeDdist_istRecord = 0;

static lv_style_t *stylePage_funSetOption = NULL;
static lv_style_t *styleText_menuLevel_A = NULL;
static lv_style_t *styleText_menuLevel_B = NULL;
static lv_style_t *styleText_menuLevel_C = NULL;
static lv_style_t *styleBk_objBground = NULL;
static lv_style_t *styleDdlistSettingA_devType = NULL;
static lv_style_t *styleLabel_devTypeSelected = NULL;
static lv_style_t *bg_styleDevStatusRecordIF = NULL;
static lv_style_t *indic_styleDevStatusRecordIF = NULL;
static lv_style_t *knob_on_styleDevStatusRecordIF = NULL;
static lv_style_t *knob_off_styleDevStatusRecordIF = NULL;
static lv_style_t *styleMbox_bg = NULL;
static lv_style_t *styleMbox_btn_pr = NULL;
static lv_style_t *styleMbox_btn_rel = NULL;
static lv_style_t *styleBtn_specialTransparent = NULL;
static lv_style_t *styleImg_menuFun_btnFun = NULL;
static lv_style_t *stylePageCb_elecParamDispSet = NULL;
static lv_style_t *stylePageCb_screenLandscape = NULL;
static lv_style_t *styleRoller_epidemicCyLocation_bg = NULL;
static lv_style_t *styleRoller_epidemicCyLocation_sel = NULL;
static lv_style_t *styleText_menuTzSet = NULL;
static lv_style_t *stylePageCb_tzSetMinusIf = NULL;
static lv_style_t *styleBtn_devAtmosLightColorSet = NULL;

static lv_style_t *stylePage_devTypeSet = NULL;
static lv_style_t *styleTextPageTitle_devTypeSet = NULL;
static lv_style_t *stylePageCb_devTypeSet = NULL;
static lv_style_t *styleLabelPageBtn_devTypeSet = NULL;

static lv_style_t *stylePage_devAtmosLightColorSet = NULL;
static lv_style_t *styleTextPageTitle_devAtmosLightColorSet = NULL;
static lv_style_t *styleLabelPageBtn_devAtmosLightColorSet = NULL;
static lv_style_t *styleLabelPageCb_devALSC_breathIf = NULL;
static lv_style_t *styleLabelPageCb_devALSC_setAllSw = NULL;
static lv_style_t *styleLabelPageCb_devALSC_autoSet = NULL;

static lv_obj_t *menuBtnChoIcon_fun_back = NULL;
static lv_obj_t *menuBtnChoIcon_fun_home = NULL;
static lv_obj_t *imgMenuBtnChoIcon_fun_home = NULL;
static lv_obj_t *imgMenuBtnChoIcon_fun_back = NULL;

static lv_obj_t *page_funSetOption = NULL;
static lv_obj_t *text_Title = NULL;
static lv_obj_t *textSettingA_deviceType = NULL;
static lv_obj_t	*textSettingA_devStatusRecordIF = NULL;
static lv_obj_t	*textSettingA_devTimezoneSet = NULL;
static lv_obj_t	*textSettingA_devListManagerEN = NULL;
static lv_obj_t	*textSettingA_devHomeAssistantEN = NULL;
static lv_obj_t	*textSettingA_bGroundPicSelect = NULL;
static lv_obj_t	*textSettingA_devAtmosLightColorSet = NULL;
static lv_obj_t *textSettingA_homepageThemestyle = NULL;
static lv_obj_t *textSettingA_factoryRecoveryIf = NULL;
static lv_obj_t *textSettingA_touchRecalibrationIf = NULL;
static lv_obj_t *textSettingA_screenBrightness = NULL;
static lv_obj_t *textSettingA_tempCalibration = NULL;
static lv_obj_t *textSettingA_screenLightTime = NULL;
static lv_obj_t *textSettingA_elecParamDispSelect = NULL;
static lv_obj_t *textSettingA_elecsumResetClear = NULL;
static lv_obj_t *textSettingA_screenLandscape = NULL;
static lv_obj_t *btnSettingA_factoryRecoveryIf = NULL;
static lv_obj_t *textBtnRef_factoryRecoveryIf = NULL;
static lv_obj_t *btnSettingA_touchRecalibrationIf = NULL;
static lv_obj_t *textBtnRef_touchRecalibrationIf = NULL;
static lv_obj_t *btnSettingA_elecsumReset = NULL;
static lv_obj_t *textBtnRef_elecsumReset = NULL;
static lv_obj_t	*ddlistSettingA_deviceType = NULL;
static lv_obj_t	*btnSettingA_deviceType = NULL;
static lv_obj_t	*btnLabelSettingA_deviceType = NULL;
static lv_obj_t *objRoller_tzSet_hour = NULL;
static lv_obj_t *objRoller_tzSet_minute = NULL;
static lv_obj_t *objLabel_tzSet_hour = NULL;
static lv_obj_t *objLabel_tzSet_minute = NULL;
static lv_obj_t *cbTzSet_minusIf = NULL;
static lv_obj_t	*pageSettingA_deviceTypeSelect = NULL;
static lv_obj_t *objpageDevTypeSelBtn_confirm = NULL;
static lv_obj_t *objpageDevTypeSelBtn_cancel = NULL;
static lv_obj_t *objpageDevTypeSelLabel_title = NULL;
static lv_obj_t *objCb_devTypeSel[DEVICE_TYPE_LIST_NUM] = {NULL};
static lv_obj_t *objpageDevTypeSelBtnLabel_confirm = NULL;
static lv_obj_t *objpageDevTypeSelBtnLabel_cancel = NULL;
static lv_obj_t	*pageSettingA_devAtmosLightColorSet = NULL;
static lv_obj_t *objpageDevAtmosLightColorSet_title = NULL;
static lv_obj_t	*btnSettingA_devAtmosLightColorSet = NULL;
static lv_obj_t *objpageDevAtmosLightColorSetBtn_confirm = NULL;
static lv_obj_t *objpageDevAtmosLightColorSetBtn_cancel = NULL;
static lv_obj_t *objpageDevAtmosLightColorSet_confirm = NULL;
static lv_obj_t *objpageDevAtmosLightColorSet_cancel = NULL;
static lv_obj_t *objpageDevALCSetLabel_colorPreview = NULL;
static lv_obj_t *objpageDevALCSetBlock_colorPreview = NULL;
static lv_obj_t *objpageDevALCSetLabel_ref = NULL;
static lv_obj_t *objpageDevALCSetSlider_color_r = NULL;
static lv_obj_t *objpageDevALCSetSlider_color_g = NULL;
static lv_obj_t *objpageDevALCSetSlider_color_b = NULL;
static lv_obj_t *objpageDevALCSetCb_automaticSet = NULL;
static lv_obj_t *objpageDevALCSetCb_breathIf = NULL;
// static lv_obj_t *objpageDevALCSetCb_setAllSwitch = NULL;
static lv_obj_t	*ddlistSettingA_homepageThemestyle = NULL;
static lv_obj_t	*swSettingA_devStatusRecordIF = NULL;
static lv_obj_t	*swSettingA_devListManagerEN = NULL;
static lv_obj_t	*swSettingA_devHomeAssistantEN = NULL;
static lv_obj_t	*pageBgroundPic_select = NULL;
static lv_obj_t	*imgBgroundPicSel_reserve = NULL;
static lv_obj_t	*btnBgroundPicSel_previewObjBase[BROUND_PIC_SELECT_MAX_NUM] = {NULL};
static lv_obj_t	*btnBgroundPicSel_preview[BROUND_PIC_SELECT_MAX_NUM] = {NULL};
static lv_obj_t *mboxFactoryRecovery_comfirm = NULL;
static lv_obj_t *mboxTouchRecalibration_comfirm = NULL;
static lv_obj_t *mboxTouchElecsumReset_comfirm = NULL;
static lv_obj_t *mboxTouchFwareUpgrade_comfirm = NULL;
static lv_obj_t *mboxScreenLandscape_comfirm = NULL;
static lv_obj_t *sliderSettingA_screenBrightness = NULL;
static lv_obj_t *sliderSettingA_screenBrightnessSleep = NULL;
static lv_obj_t *sliderSettingA_tempratureCal = NULL;
static lv_obj_t *textSliderRef_tempratureCal = NULL;
static lv_obj_t *btnValAdd_tempratureCal = NULL;
static lv_obj_t *textBtnValAdd_tempratureCal = NULL;
static lv_obj_t *btnValCut_tempratureCal = NULL;
static lv_obj_t *textBtnValCut_tempratureCal = NULL;
static lv_obj_t	*swSettingA_epidemicReportIF = NULL;
static lv_obj_t	*rollerEpidemic_cyLocation = NULL;
static lv_obj_t *textBtnTimeRef_screenLightTime = NULL;
static lv_obj_t *btnTimeAdd_screenLightTime = NULL;
static lv_obj_t *textBtnTimeAdd_screenLightTime = NULL;
static lv_obj_t *btnTimeCut_screenLightTime = NULL;
static lv_obj_t *textBtnTimeCut_screenLightTime = NULL;
static lv_obj_t *cbDispSelect_elecParam_power = NULL;
static lv_obj_t *cbDispSelect_elecParam_elecsum = NULL;
static lv_obj_t *cbDispSelect_screenVer = NULL;
static lv_obj_t *cbDispSelect_screenHor = NULL;
static lv_obj_t *textSettingA_fwareUpgrade = NULL;
static lv_obj_t *btnSettingA_fwareUpgrade = NULL;
static lv_obj_t *textBtnRef_fwareUpgrade = NULL;
static lv_obj_t *textSettingA_epidemicReport = NULL;
static lv_obj_t *textSettingA_epidemicCountry = NULL;

static bool 	upgradeAvailable_flg = false;
static bool 	settingItem_ALCS_affiObjs_availableFlg = false;

static uint8_t  functionGearScreenTime_ref = 0;
static bool 	opreatTemp_screenLandscapeIf = false;

static uint8_t 	screenLandscapeCoordinate_objOffset = 0;

static char 	textlabel_dispTemp[32] = {0};

static uint8_t  devTypeSelectOpreation_dataTemp = 0;

stt_devAtmosLightRunningParam atmosLightRunParam_dataTemp = {0};
static bool		atmosLightCfgOptAvailableAllSw_if = false;

static void lvGui_settingSet_epidemicCtrlObj_opt(bool creatIf);
static void lvApp_objCb_devTypeSel_batchDel(void);
static void lvGui_pageAtmosLightColorSetAffiAttend(bool creatIf);

static void currentGui_elementClear(void){

	lvApp_objCb_devTypeSel_batchDel();
	lv_obj_del(page_funSetOption);
	
	textSettingA_epidemicCountry = NULL;
	rollerEpidemic_cyLocation = NULL;

	if(NULL != pageSettingA_deviceTypeSelect){

		lv_obj_del(pageSettingA_deviceTypeSelect);
		pageSettingA_deviceTypeSelect = NULL;
	}

	if(NULL != pageSettingA_devAtmosLightColorSet){

		lv_obj_del(pageSettingA_devAtmosLightColorSet);
		pageSettingA_devAtmosLightColorSet = NULL;

		settingItem_ALCS_affiObjs_availableFlg = false;
	}
}

void guiDispTimeOut_pageSettingSet(void){

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

static lv_res_t funCb_btnActionClick_bGroundPicOrgSelcet(lv_obj_t *btn){

	LV_OBJ_FREE_NUM_TYPE btnFreeNum = lv_obj_get_free_num(btn);
	uint8_t loop = 0;

	printf("bGroundOrg sel:%d.\n", btnFreeNum);

	lv_obj_del(imgBgroundPicSel_reserve);

	for(loop = 0; loop < BROUND_PIC_SELECT_MAX_NUM; loop ++){

		(btnFreeNum == loop)?
			(lv_imgbtn_set_state(btnBgroundPicSel_preview[loop], LV_BTN_STATE_TGL_REL)):
			(lv_imgbtn_set_state(btnBgroundPicSel_preview[loop], LV_BTN_STATE_TGL_PR));
	}

	imgBgroundPicSel_reserve = lv_img_create(btnBgroundPicSel_preview[btnFreeNum], NULL);
	lv_img_set_src(imgBgroundPicSel_reserve, &bGroundPrev_picFigure_sel);

	usrAppHomepageBgroundPicOrg_Set(btnFreeNum, true, false);

	return LV_RES_OK;
}

void appUiElementSet_upgradeAvailable(bool val){

	upgradeAvailable_flg = val;
}

static lv_res_t funCb_mboxBtnActionClick_factoryRecovery(lv_obj_t * mbox, const char * txt){

	uint8_t loop = 0;

	printf("factoryRecovery mbox btn: %s released\n", txt);

	do{

		if(!strcmp(mbox_btnm_textTab[loop], txt)){

			switch(loop){

				case 0:{

					mdf_info_erase("ESP-MDF");
					devSystemInfoLocalRecord_allErase();
					hap_reset_to_factory();

					usrApplication_systemRestartTrig(3);

				}break;

				case 1:{


				}break;

				default:break;
			}

			break;
		}
			
		loop ++;
	
	}while(mbox_btnm_textTab[loop]);

	lv_mbox_start_auto_close(mboxFactoryRecovery_comfirm, 20);

	return LV_RES_OK;
}

static lv_res_t funCb_mboxBtnActionClick_touchRecalibration(lv_obj_t * mbox, const char * txt){

	uint8_t loop = 0;

	printf("touchRecalibration mbox btn: %s released\n", txt);

	do{

		if(!strcmp(mbox_btnm_textTab[loop], txt)){

			switch(loop){

				case 0:{

					extern void usrApp_devIptdrv_paramRecalibration_set(bool reCalibra_if);

					usrApp_devIptdrv_paramRecalibration_set(true);

					usrApplication_systemRestartTrig(5);

				}break;

				case 1:{


				}break;

				default:break;
			}

			break;
		}
			
		loop ++;
	
	}while(mbox_btnm_textTab[loop]);

	lv_mbox_start_auto_close(mboxTouchRecalibration_comfirm, 20);

	return LV_RES_OK;
}

static lv_res_t funCb_mboxBtnActionClick_elecsumReset(lv_obj_t * mbox, const char * txt){

	uint8_t loop = 0;

	printf("elecsumReset mbox btn: %s released\n", txt);

	do{

		if(!strcmp(mbox_btnm_textTab[loop], txt)){

			switch(loop){

				case 0:{

					devDriverBussiness_elecMeasure_elecSumResetClear();

				}break;

				case 1:{


				}break;

				default:break;
			}

			break;
		}
			
		loop ++;
	
	}while(mbox_btnm_textTab[loop]);

	lv_mbox_start_auto_close(mboxTouchElecsumReset_comfirm, 20);

	return LV_RES_OK;
}

static lv_res_t funCb_mboxBtnActionClick_firewareUpgrade(lv_obj_t * mbox, const char * txt){

	uint8_t loop = 0;

	printf("firewareUpgrade mbox btn: %s released\n", txt);

	do{

		if(!strcmp(mbox_btnm_textTab[loop], txt)){

			switch(loop){

				case 0:{				

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_SHARE_MIX)

					//同开关类型强制批量升级
					usrApp_firewareUpgrade_trig(true, (uint8_t)currentDev_typeGet());

//					//根节点设备被授权后，有权发起所有设备一起升级 --六合一有效
//					(esp_mesh_get_layer() == MESH_ROOT)?
//						(usrApp_firewareUpgrade_trig(true)):
//						(usrApp_firewareUpgrade_trig(false));
#else

//					usrApp_firewareUpgrade_trig(false);

					usrApp_firewareUpgrade_trig(true, (uint8_t)currentDev_typeGet());
#endif

					lvGui_tipsFullScreen_generate("system upgrading...", 245);

				}break;

				case 1:{


				}break;

				default:break;
			}

			break;
		}
			
		loop ++;
	
	}while(mbox_btnm_textTab[loop]);

	lv_mbox_start_auto_close(mboxTouchFwareUpgrade_comfirm, 20);

	return LV_RES_OK;
}

static lv_res_t funCb_mboxBtnActionClick_screenLandscapeConfirm(lv_obj_t * mbox, const char * txt){

	uint8_t loop = 0;

	printf("screen landscape opreation mbox btn: %s released\n", txt);

	do{

		if(!strcmp(mbox_btnm_textTab[loop], txt)){

			switch(loop){

				case 0:{

					stt_devStatusRecord devStatusRecordFlg_temp = {0};

					devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

					if(opreatTemp_screenLandscapeIf){

						devStatusRecordFlg_temp.devScreenLandscape_IF = 1;
						lv_cb_set_checked(cbDispSelect_screenVer, false);
						lv_cb_set_checked(cbDispSelect_screenHor, true);
					}
					else
					{
						devStatusRecordFlg_temp.devScreenLandscape_IF = 0;
						lv_cb_set_checked(cbDispSelect_screenVer, true);
						lv_cb_set_checked(cbDispSelect_screenHor, false);
					}

//					usrApp_devIptdrv_paramRecalibration_set(true);
					devStatusRecordIF_paramSet(&devStatusRecordFlg_temp, true);
					usrApplication_systemRestartTrig(3);

				}break;

				case 1:{

					if(devStatusDispMethod_landscapeIf_get()){

						lv_cb_set_checked(cbDispSelect_screenVer, false);
						lv_cb_set_checked(cbDispSelect_screenHor, true);
					}
					else
					{
						lv_cb_set_checked(cbDispSelect_screenVer, true);
						lv_cb_set_checked(cbDispSelect_screenHor, false);
					}

				}break;

				default:break;
			}

			break;
		}
			
		loop ++;
	
	}while(mbox_btnm_textTab[loop]);

	lv_mbox_start_auto_close(mboxScreenLandscape_comfirm, 20);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_factoryRecovery(lv_obj_t *btn){

	mboxFactoryRecovery_comfirm = lv_mbox_create(lv_scr_act(), NULL);
	lv_obj_set_protect(mboxFactoryRecovery_comfirm, LV_PROTECT_POS);
	lv_obj_align(mboxFactoryRecovery_comfirm, NULL, LV_ALIGN_CENTER, -35, -55);
	lv_obj_set_width(mboxFactoryRecovery_comfirm, 230);
	lv_mbox_set_text(mboxFactoryRecovery_comfirm, "factory recovery opreat need to restart the devive, are you sure to continue?");
	lv_mbox_add_btns(mboxFactoryRecovery_comfirm, mbox_btnm_textTab, NULL);
	lv_mbox_set_style(mboxFactoryRecovery_comfirm, LV_MBOX_STYLE_BG, styleMbox_bg);
	lv_mbox_set_style(mboxFactoryRecovery_comfirm, LV_MBOX_STYLE_BTN_REL, styleMbox_btn_rel);
	lv_mbox_set_style(mboxFactoryRecovery_comfirm, LV_MBOX_STYLE_BTN_PR, styleMbox_btn_pr);
	lv_mbox_set_action(mboxFactoryRecovery_comfirm, funCb_mboxBtnActionClick_factoryRecovery);
	lv_mbox_set_anim_time(mboxFactoryRecovery_comfirm, 200);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_touchRecalibration(lv_obj_t *btn){

	mboxTouchRecalibration_comfirm = lv_mbox_create(lv_scr_act(), NULL);
	lv_obj_set_protect(mboxTouchRecalibration_comfirm, LV_PROTECT_POS);
	lv_obj_align(mboxTouchRecalibration_comfirm, NULL, LV_ALIGN_CENTER, -35, -55);
	lv_obj_set_width(mboxTouchRecalibration_comfirm, 230);
	lv_mbox_set_text(mboxTouchRecalibration_comfirm, "touch recalibration opreat need to restart the devive, are you sure to continue?");
	lv_mbox_add_btns(mboxTouchRecalibration_comfirm, mbox_btnm_textTab, NULL);
	lv_mbox_set_style(mboxTouchRecalibration_comfirm, LV_MBOX_STYLE_BG, styleMbox_bg);
	lv_mbox_set_style(mboxTouchRecalibration_comfirm, LV_MBOX_STYLE_BTN_REL, styleMbox_btn_rel);
	lv_mbox_set_style(mboxTouchRecalibration_comfirm, LV_MBOX_STYLE_BTN_PR, styleMbox_btn_pr);
	lv_mbox_set_action(mboxTouchRecalibration_comfirm, funCb_mboxBtnActionClick_touchRecalibration);
	lv_mbox_set_anim_time(mboxTouchRecalibration_comfirm, 200);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_elecsumResetClear(lv_obj_t *btn){

	mboxTouchElecsumReset_comfirm = lv_mbox_create(lv_scr_act(), NULL);
	lv_obj_set_protect(mboxTouchElecsumReset_comfirm, LV_PROTECT_POS);
	lv_obj_align(mboxTouchElecsumReset_comfirm, NULL, LV_ALIGN_CENTER, -35, -55);
	lv_obj_set_width(mboxTouchElecsumReset_comfirm, 230);
	lv_mbox_set_text(mboxTouchElecsumReset_comfirm, "electric sum will be cleared, are you sure to continue?");
	lv_mbox_add_btns(mboxTouchElecsumReset_comfirm, mbox_btnm_textTab, NULL);
	lv_mbox_set_style(mboxTouchElecsumReset_comfirm, LV_MBOX_STYLE_BG, styleMbox_bg);
	lv_mbox_set_style(mboxTouchElecsumReset_comfirm, LV_MBOX_STYLE_BTN_REL, styleMbox_btn_rel);
	lv_mbox_set_style(mboxTouchElecsumReset_comfirm, LV_MBOX_STYLE_BTN_PR, styleMbox_btn_pr);
	lv_mbox_set_action(mboxTouchElecsumReset_comfirm, funCb_mboxBtnActionClick_elecsumReset);
	lv_mbox_set_anim_time(mboxTouchElecsumReset_comfirm, 200);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_fwareUpgrade(lv_obj_t *btn){

	mboxTouchFwareUpgrade_comfirm = lv_mbox_create(lv_scr_act(), NULL);
	lv_obj_set_protect(mboxTouchFwareUpgrade_comfirm, LV_PROTECT_POS);
	lv_obj_align(mboxTouchFwareUpgrade_comfirm, NULL, LV_ALIGN_CENTER, -35, -55);
	lv_obj_set_width(mboxTouchFwareUpgrade_comfirm, 230);
	lv_mbox_set_text(mboxTouchFwareUpgrade_comfirm, "the system will be updated, are you sure to continue?");
	lv_mbox_add_btns(mboxTouchFwareUpgrade_comfirm, mbox_btnm_textTab, NULL);
	lv_mbox_set_style(mboxTouchFwareUpgrade_comfirm, LV_MBOX_STYLE_BG, styleMbox_bg);
	lv_mbox_set_style(mboxTouchFwareUpgrade_comfirm, LV_MBOX_STYLE_BTN_REL, styleMbox_btn_rel);
	lv_mbox_set_style(mboxTouchFwareUpgrade_comfirm, LV_MBOX_STYLE_BTN_PR, styleMbox_btn_pr);
	lv_mbox_set_action(mboxTouchFwareUpgrade_comfirm, funCb_mboxBtnActionClick_firewareUpgrade);
	lv_mbox_set_anim_time(mboxTouchFwareUpgrade_comfirm, 200);

	return LV_RES_OK;
}

void screenLandscape_opreatConfirm(void){

	mboxScreenLandscape_comfirm = lv_mbox_create(lv_scr_act(), NULL);
	lv_obj_set_protect(mboxScreenLandscape_comfirm, LV_PROTECT_POS);
	lv_obj_align(mboxScreenLandscape_comfirm, NULL, LV_ALIGN_CENTER, -35, -55);
	lv_obj_set_width(mboxScreenLandscape_comfirm, 230);
	lv_mbox_set_text(mboxScreenLandscape_comfirm, "This operation needs to be restarted to take effect, confirm to continue.");
	lv_mbox_add_btns(mboxScreenLandscape_comfirm, mbox_btnm_textTab, NULL);
	lv_mbox_set_style(mboxScreenLandscape_comfirm, LV_MBOX_STYLE_BG, styleMbox_bg);
	lv_mbox_set_style(mboxScreenLandscape_comfirm, LV_MBOX_STYLE_BTN_REL, styleMbox_btn_rel);
	lv_mbox_set_style(mboxScreenLandscape_comfirm, LV_MBOX_STYLE_BTN_PR, styleMbox_btn_pr);
	lv_mbox_set_action(mboxScreenLandscape_comfirm, funCb_mboxBtnActionClick_screenLandscapeConfirm);
	lv_mbox_set_anim_time(mboxScreenLandscape_comfirm, 200);

	return LV_RES_OK;
}

static lv_res_t funCb_cbOpreat_elecParamDispSelect_power(lv_obj_t *cb){

	extern void lvGui_usrAppElecParam_dispRefresh(void);

	stt_devStatusRecord devStatusRecordFlg_temp = {0};

	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

	if(lv_cb_is_checked(cb)){

		lv_cb_set_checked(cbDispSelect_elecParam_elecsum, false);
		devStatusRecordFlg_temp.devElecsumDisp_IF = 0;
		devStatusRecordIF_paramSet(&devStatusRecordFlg_temp, true);

		lvGui_usrAppElecParam_dispRefresh();
	}	
	else
	{
		lv_cb_set_checked(cbDispSelect_elecParam_power, true);
	}

	return LV_RES_OK;
}

static lv_res_t funCb_cbOpreat_elecParamDispSelect_elecsum(lv_obj_t *cb){

	extern void lvGui_usrAppElecParam_dispRefresh(void);

	stt_devStatusRecord devStatusRecordFlg_temp = {0};

	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

	if(lv_cb_is_checked(cb)){

		lv_cb_set_checked(cbDispSelect_elecParam_power, false);
		devStatusRecordFlg_temp.devElecsumDisp_IF = 1;
		devStatusRecordIF_paramSet(&devStatusRecordFlg_temp, true);

		lvGui_usrAppElecParam_dispRefresh();
	}	
	else
	{
		lv_cb_set_checked(cbDispSelect_elecParam_elecsum, true);
	}

	return LV_RES_OK;
}

static lv_res_t funCb_cbOpreat_scrrenDispMethod_ver(lv_obj_t *cb){

	stt_devStatusRecord devStatusRecordFlg_temp = {0};

	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

	if(lv_cb_is_checked(cb)){

		if(devStatusRecordFlg_temp.devScreenLandscape_IF){

			opreatTemp_screenLandscapeIf = false;
			
			lv_cb_set_checked(cbDispSelect_screenHor, false);
		
			screenLandscape_opreatConfirm();
		}
	}	
	else
	{
		lv_cb_set_checked(cb, true);
		lv_cb_set_checked(cbDispSelect_screenHor, false);
	}

	return LV_RES_OK;
}

static lv_res_t funCb_cbOpreat_scrrenDispMethod_hor(lv_obj_t *cb){

	stt_devStatusRecord devStatusRecordFlg_temp = {0};

	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

	if(lv_cb_is_checked(cb)){

		if(!devStatusRecordFlg_temp.devScreenLandscape_IF){

			opreatTemp_screenLandscapeIf = true;

			lv_cb_set_checked(cbDispSelect_screenVer, false);

			screenLandscape_opreatConfirm();
		}
	}	
	else
	{
		lv_cb_set_checked(cb, true);
		lv_cb_set_checked(cbDispSelect_screenVer, false);
	}

	return LV_RES_OK;
}

static lv_res_t funCb_slidAction_functionSet_screenBrightnessAdj(lv_obj_t *slider){

	LV_OBJ_FREE_NUM_TYPE sliderFreeNum = lv_obj_get_free_num(slider);

	uint8_t brightnessTemp = lv_slider_get_value(slider) * (DEVLEDC_SCREEN_BRIGHTNESS_LEVEL_DIV / screenBrightness_sliderAdj_div);
	uint8_t brightnessA = devScreenDriver_configParam_brightness_get();
	uint8_t brightnessB = devScreenDriver_configParam_brightnessSleep_get();

	switch(sliderFreeNum){

		case 0:{

			if(brightnessTemp == 0){
			
				lv_slider_set_value(sliderSettingA_screenBrightness, 1);
				brightnessTemp = lv_slider_get_value(slider) * (DEVLEDC_SCREEN_BRIGHTNESS_LEVEL_DIV / screenBrightness_sliderAdj_div);
			}

			devScreenDriver_configParam_brightness_set(brightnessTemp, true);
			if(brightnessTemp < brightnessB){

				devScreenDriver_configParam_brightnessSleep_set(brightnessTemp, true);
				lv_slider_set_value(sliderSettingA_screenBrightnessSleep, brightnessTemp / (DEVLEDC_SCREEN_BRIGHTNESS_LEVEL_DIV / screenBrightness_sliderAdj_div));
			}

		}break;

		case 1:{

			devScreenDriver_configParam_brightnessSleep_set(brightnessTemp, true);
			if(brightnessTemp > brightnessA){

				devScreenDriver_configParam_brightness_set(brightnessTemp, true);
				lv_slider_set_value(sliderSettingA_screenBrightness, brightnessTemp / (DEVLEDC_SCREEN_BRIGHTNESS_LEVEL_DIV / screenBrightness_sliderAdj_div));
			}

		}break;
	}

	return LV_RES_OK;
}

static lv_res_t funCb_slidAction_functionSet_tempratureCal(lv_obj_t *slider){

	uint8_t tempratureCal = lv_slider_get_value(slider);

	sprintf(textlabel_dispTemp, "#FFFF00 %.1f#", devTempratureSensor_dataCal_hanldeGet(tempratureCal));
	lv_label_set_text(textSliderRef_tempratureCal, textlabel_dispTemp);
	
	devTempratureSensor_dataCal_set(tempratureCal, true);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_functionSet_tempratureCalAdd(lv_obj_t *btn){

	uint8_t dataCal = devTempratureSensor_dataCal_get();

	if(dataCal < DEV_TEMPRATURE_DATA_CAL_MAX_RANGE)dataCal += 1;
	devTempratureSensor_dataCal_set(dataCal, true);

	sprintf(textlabel_dispTemp, "#FFFF00 %.1f#", devTempratureSensor_dataCal_hanldeGet(dataCal));
	lv_label_set_text(textSliderRef_tempratureCal, textlabel_dispTemp);
	lv_slider_set_value(sliderSettingA_tempratureCal, dataCal);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_functionSet_tempratureCalCut(lv_obj_t *btn){

	uint8_t dataCal = devTempratureSensor_dataCal_get();

	if(dataCal > 0)dataCal -= 1;
	devTempratureSensor_dataCal_set(dataCal, true);

	sprintf(textlabel_dispTemp, "#FFFF00 %.1f#", devTempratureSensor_dataCal_hanldeGet(dataCal));
	lv_label_set_text(textSliderRef_tempratureCal, textlabel_dispTemp);
	lv_slider_set_value(sliderSettingA_tempratureCal, dataCal);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_functionSet_screenLightTimeAdd(lv_obj_t *btn){

	if(functionGearScreenTime_ref < (FUNCTION_NUM_DEF_SCREENLIGHT_TIME - 1))
		functionGearScreenTime_ref ++;

	lv_label_set_text(textBtnTimeRef_screenLightTime, screenLightTimeGear_refTab[functionGearScreenTime_ref].strDisp_ref);
	lv_obj_refresh_style(textBtnTimeRef_screenLightTime);

	devScreenDriver_configParam_screenLightTime_set(screenLightTimeGear_refTab[functionGearScreenTime_ref].screenLightTime2Sec, true);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_functionSet_screenLightTimeCut(lv_obj_t *btn){

	if(functionGearScreenTime_ref)
		functionGearScreenTime_ref --;

	lv_label_set_text(textBtnTimeRef_screenLightTime, screenLightTimeGear_refTab[functionGearScreenTime_ref].strDisp_ref);
	lv_obj_refresh_style(textBtnTimeRef_screenLightTime);

	devScreenDriver_configParam_screenLightTime_set(screenLightTimeGear_refTab[functionGearScreenTime_ref].screenLightTime2Sec, true);

	return LV_RES_OK;
}

static void lvApp_objCb_devTypeSel_batchDel(void){

	uint8_t loop = 0;

	for(loop = 0; loop < DEVICE_TYPE_LIST_NUM; loop ++){

		if(NULL != objCb_devTypeSel[loop]){

			lv_obj_del(objCb_devTypeSel[loop]);
			objCb_devTypeSel[loop] = NULL;
		}
	}
}

static lv_res_t funCb_btnActionPress_btnPage_devType_confirm(lv_obj_t *btn){

	stt_devDataPonitTypedef devDataPoint_temp = {0};

	lv_label_set_text(btnLabelSettingA_deviceType, deviceTypeStr_disp[devTypeSelectOpreation_dataTemp].devName);
	
	if(deviceTypeStr_disp[devTypeSelectOpreation_dataTemp].devType != currentDev_typeGet()){

		deviceDestoryPreventChange(currentDev_typeGet());

		currentDev_typeSet(deviceTypeStr_disp[devTypeSelectOpreation_dataTemp].devType, true);
		devDriverManageBussiness_deviceChangeRefresh(); //驱动更新
		switch(currentDev_typeGet()){
		
			case devTypeDef_thermostat:
			case devTypeDef_thermostatExtension:{
		
				devDataPoint_temp.devType_thermostat.devThermostat_tempratureTarget = DEVICE_THERMOSTAT_TEMPTARGET_DEFAULT;
				currentDev_dataPointSet(&devDataPoint_temp, true, false, false, false, false); //数据点加载默认值
		
			}break;
		
			case devTypeDef_scenario:{
		
				/*什么都不做*/
		
			}break;
		
			case devTypeDef_curtain:{
		
				stt_devStatusRecord devStatusRecordFlg_temp = {0};
				
				devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);
		
				devDataPoint_temp.devType_curtain.devCurtain_actMethod = 1;
				devDataPoint_temp.devType_curtain.devCurtain_actEnumVal = 0;
				currentDev_dataPointSet(&devDataPoint_temp, true, true, true, false, false);
		
				devStatusRecordFlg_temp.devStatusOnOffRecord_IF = 0;
				devStatusRecordIF_paramSet(&devStatusRecordFlg_temp, true);
			}break;
		
			default:{
		
				currentDev_dataPointSet(&devDataPoint_temp, true, true, true, false, false); //数据点清零
			}break;
		}
		
		devSystemInfoLocalRecord_normalClear();
		
		usrAppHomepageBtnTextDisp_defaultLoad(deviceTypeStr_disp[devTypeSelectOpreation_dataTemp].devType, true); //图文显示恢复默认
		usrAppHomepageBtnIconNumDisp_defaultLoad(deviceTypeStr_disp[devTypeSelectOpreation_dataTemp].devType, true);

		htAccessoryDeviceRegisterByEvt_trig();
	}

	lvApp_objCb_devTypeSel_batchDel();
	lv_obj_del(pageSettingA_deviceTypeSelect);
	pageSettingA_deviceTypeSelect = NULL;

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionPress_btnPage_devType_cancel(lv_obj_t *btn){

	lvApp_objCb_devTypeSel_batchDel();
	lv_obj_del(pageSettingA_deviceTypeSelect);
	pageSettingA_deviceTypeSelect = NULL;

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionPress_btnPage_devAtmosLightColor_confirm(lv_obj_t *btn){

	settingItem_ALCS_affiObjs_availableFlg = false;

	if(atmosphereLightType_idleCfgPreview == devAtmosphere_statusTips_trigGet())
		devAtmosphere_statusTips_trigSet(atmosphereLightType_none);

	devAtmosLight_runningParam_set(&atmosLightRunParam_dataTemp, true);
	if(true == atmosLightCfgOptAvailableAllSw_if){

		atmosLightCfgOptAvailableAllSw_if = false;
	}

	lv_obj_del(pageSettingA_devAtmosLightColorSet);
	pageSettingA_devAtmosLightColorSet = NULL;

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionPress_btnPage_devAtmosLightColor_cancel(lv_obj_t *btn){

	settingItem_ALCS_affiObjs_availableFlg = false;

	if(atmosphereLightType_idleCfgPreview == devAtmosphere_statusTips_trigGet())
		devAtmosphere_statusTips_trigSet(atmosphereLightType_none);

	lv_obj_del(pageSettingA_devAtmosLightColorSet);
	pageSettingA_devAtmosLightColorSet = NULL;

	return LV_RES_OK;
}

static lv_res_t funCb_cbOpreat_devTypeSelOpreation(lv_obj_t *cb){

	LV_OBJ_FREE_NUM_TYPE id = lv_obj_get_free_num(cb);
	uint8_t loop = 0;

	lv_cb_set_checked(cb, true);

	devTypeSelectOpreation_dataTemp = id;
	for(loop = 0; loop < DEVICE_TYPE_LIST_NUM; loop ++){

		if(NULL != objCb_devTypeSel[loop]){

			if(loop != id){

				lv_cb_set_checked(objCb_devTypeSel[loop], false);
			}
		}
	}

	return LV_RES_OK;
}

static void lvGui_pageDevTypeSeclect_creat(void){

	uint8_t loop = 0;

	if(pageSettingA_deviceTypeSelect == NULL)
		pageSettingA_deviceTypeSelect = lv_page_create(lv_scr_act(), NULL);

	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(pageSettingA_deviceTypeSelect, 280, 190)):
		(lv_obj_set_size(pageSettingA_deviceTypeSelect, 200, 260));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(pageSettingA_deviceTypeSelect, 20, 25)):
		(lv_obj_set_pos(pageSettingA_deviceTypeSelect, 20, 45));		
	lv_page_set_style(pageSettingA_deviceTypeSelect, LV_PAGE_STYLE_SB, stylePage_devTypeSet);
	lv_page_set_style(pageSettingA_deviceTypeSelect, LV_PAGE_STYLE_BG, stylePage_devTypeSet);
	lv_page_set_sb_mode(pageSettingA_deviceTypeSelect, LV_SB_MODE_HIDE); 	
	lv_page_set_scrl_fit(pageSettingA_deviceTypeSelect, false, true); //key opration
	lv_page_set_scrl_layout(pageSettingA_deviceTypeSelect, LV_LAYOUT_PRETTY);

	objpageDevTypeSelLabel_title = lv_label_create(pageSettingA_deviceTypeSelect, NULL);
	lv_obj_set_style(objpageDevTypeSelLabel_title, styleTextPageTitle_devTypeSet);
	lv_label_set_text(objpageDevTypeSelLabel_title, "Switch type");
	lv_obj_set_protect(objpageDevTypeSelLabel_title, LV_PROTECT_POS);
	lv_obj_align(objpageDevTypeSelLabel_title, pageSettingA_deviceTypeSelect, LV_ALIGN_IN_TOP_MID, 0, -5);

	for(loop = 0; loop < DEVICE_TYPE_LIST_NUM; loop ++){

		if(NULL != deviceTypeStr_disp[loop].devName){

			objCb_devTypeSel[loop] = lv_cb_create(pageSettingA_deviceTypeSelect, NULL);
			lv_obj_set_size(objCb_devTypeSel[loop], 160, 20);
			lv_cb_set_text(objCb_devTypeSel[loop], deviceTypeStr_disp[loop].devName);
			lv_obj_set_protect(objCb_devTypeSel[loop], LV_PROTECT_POS);
			lv_obj_set_protect(objCb_devTypeSel[loop], LV_PROTECT_FOLLOW);
			lv_cb_set_style(objCb_devTypeSel[loop], LV_CB_STYLE_BG, stylePageCb_devTypeSet);

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_ALL_RESERVE)

			(devStatusDispMethod_landscapeIf_get())?
				(lv_obj_align(objCb_devTypeSel[loop], objpageDevTypeSelLabel_title, LV_ALIGN_OUT_BOTTOM_LEFT, (loop % 2) * 120 - 75, loop / 2 * 25 + 5)):
				(lv_obj_align(objCb_devTypeSel[loop], objpageDevTypeSelLabel_title, LV_ALIGN_OUT_BOTTOM_LEFT, -30, loop * 20));
#else

			(devStatusDispMethod_landscapeIf_get())?
				(lv_obj_align(objCb_devTypeSel[loop], objpageDevTypeSelLabel_title, LV_ALIGN_OUT_BOTTOM_LEFT, -25, loop * 25)):
				(lv_obj_align(objCb_devTypeSel[loop], objpageDevTypeSelLabel_title, LV_ALIGN_OUT_BOTTOM_LEFT, -20, loop * 30 + 10));
#endif
			lv_obj_set_free_num(objCb_devTypeSel[loop], loop);
			lv_cb_set_action(objCb_devTypeSel[loop], funCb_cbOpreat_devTypeSelOpreation);
			if(currentDev_typeGet() == deviceTypeStr_disp[loop].devType)lv_cb_set_checked(objCb_devTypeSel[loop], true);
		}
		else{

			break;
		}
	}

	objpageDevTypeSelBtn_confirm = lv_btn_create(pageSettingA_deviceTypeSelect, NULL);
    lv_btn_set_style(objpageDevTypeSelBtn_confirm, LV_BTN_STYLE_REL, &lv_style_transp);
    lv_btn_set_style(objpageDevTypeSelBtn_confirm, LV_BTN_STYLE_PR, &lv_style_transp);
    lv_btn_set_style(objpageDevTypeSelBtn_confirm, LV_BTN_STYLE_TGL_REL, &lv_style_transp);
    lv_btn_set_style(objpageDevTypeSelBtn_confirm, LV_BTN_STYLE_TGL_PR, &lv_style_transp);
	lv_btn_set_fit(objpageDevTypeSelBtn_confirm, false, false);
	lv_obj_set_size(objpageDevTypeSelBtn_confirm, 70, 25);
	lv_page_glue_obj(objpageDevTypeSelBtn_confirm, true);
	lv_obj_set_protect(objpageDevTypeSelBtn_confirm, LV_PROTECT_FOLLOW);
	lv_obj_set_protect(objpageDevTypeSelBtn_confirm, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(objpageDevTypeSelBtn_confirm, pageSettingA_deviceTypeSelect, LV_ALIGN_IN_BOTTOM_LEFT, 45, -1)):
		(lv_obj_align(objpageDevTypeSelBtn_confirm, pageSettingA_deviceTypeSelect, LV_ALIGN_IN_BOTTOM_LEFT, 25, -1));
	lv_btn_set_action(objpageDevTypeSelBtn_confirm, LV_BTN_ACTION_CLICK, funCb_btnActionPress_btnPage_devType_confirm);
	objpageDevTypeSelBtn_cancel = lv_btn_create(pageSettingA_deviceTypeSelect, objpageDevTypeSelBtn_confirm);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(objpageDevTypeSelBtn_cancel, pageSettingA_deviceTypeSelect, LV_ALIGN_IN_BOTTOM_RIGHT, -45, -1)):
		(lv_obj_align(objpageDevTypeSelBtn_cancel, objpageDevTypeSelBtn_confirm, LV_ALIGN_CENTER, 85, 0));
	lv_btn_set_action(objpageDevTypeSelBtn_cancel, LV_BTN_ACTION_CLICK, funCb_btnActionPress_btnPage_devType_cancel);

	objpageDevTypeSelBtnLabel_confirm = lv_label_create(objpageDevTypeSelBtn_confirm, NULL);
	lv_obj_set_style(objpageDevTypeSelBtnLabel_confirm, styleLabelPageBtn_devTypeSet);
	lv_label_set_text(objpageDevTypeSelBtnLabel_confirm, "confirm");
	objpageDevTypeSelBtnLabel_cancel = lv_label_create(objpageDevTypeSelBtn_cancel, objpageDevTypeSelBtnLabel_confirm);
	lv_obj_set_style(objpageDevTypeSelBtnLabel_cancel, styleLabelPageBtn_devTypeSet);
	lv_label_set_text(objpageDevTypeSelBtnLabel_cancel, "cancel");

	usrApp_fullScreenRefresh_self(20, 80);

}

static lv_res_t funCb_slidAction_ALCS_set_color(lv_obj_t *slider){

	LV_OBJ_FREE_NUM_TYPE objFnum = lv_obj_get_free_num(slider);

	switch(objFnum){

		case LV_FNUM_SLIDER_ALCS_R:{

			atmosLightRunParam_dataTemp.lightColorCustomParamcfg.red = lv_slider_get_value(slider);
	
		}break;
		
		case LV_FNUM_SLIDER_ALCS_G:{

			atmosLightRunParam_dataTemp.lightColorCustomParamcfg.green = lv_slider_get_value(slider);

		}break;
		
		case LV_FNUM_SLIDER_ALCS_B:{

			atmosLightRunParam_dataTemp.lightColorCustomParamcfg.blue = lv_slider_get_value(slider);

		}break;

		default:break;
	}

	styleBtn_devAtmosLightColorSet->body.main_color = atmosLightRunParam_dataTemp.lightColorCustomParamcfg;
	styleBtn_devAtmosLightColorSet->body.grad_color = atmosLightRunParam_dataTemp.lightColorCustomParamcfg;	
	lv_obj_set_style(objpageDevALCSetBlock_colorPreview, styleBtn_devAtmosLightColorSet);
	lv_obj_refresh_style(objpageDevALCSetBlock_colorPreview);
	stylePage_devAtmosLightColorSet->body.border.color = atmosLightRunParam_dataTemp.lightColorCustomParamcfg;
	lv_obj_refresh_style(pageSettingA_devAtmosLightColorSet);

	return LV_RES_OK;
}

static lv_res_t funCb_cbAction_ALCS_exParamSet(lv_obj_t *cb){

	LV_OBJ_FREE_NUM_TYPE objFnum = lv_obj_get_free_num(cb);

	switch(objFnum){

		case LV_FNUM_CB_ALCS_AUTOMATIC:{

			atmosLightRunParam_dataTemp.runingAsCustomCfg_flg = !lv_cb_is_checked(cb);
			lvGui_pageAtmosLightColorSetAffiAttend(!lv_cb_is_checked(cb));

		}break;

		case LV_FNUM_CB_ALCS_BRE_IF:{

			(true == lv_cb_is_checked(cb))?
				(atmosLightRunParam_dataTemp.customCfg_breathIf = 1):
				(atmosLightRunParam_dataTemp.customCfg_breathIf = 0);

		}break;

		case LV_FNUM_CB_ALCS_SETALL_IF:{

			atmosLightCfgOptAvailableAllSw_if = lv_cb_is_checked(cb);

		}break;

		default:break;
	}

	return LV_RES_OK;
}

static void lvGui_pageAtmosLightColorSetAffiAttend(bool creatIf){

	if(creatIf){

		devAtmosphere_statusTips_trigSet(atmosphereLightType_idleCfgPreview);

		if(false == settingItem_ALCS_affiObjs_availableFlg){

			objpageDevALCSetLabel_colorPreview = lv_label_create(pageSettingA_devAtmosLightColorSet, NULL);
			lv_obj_set_style(objpageDevALCSetLabel_colorPreview, styleTextPageTitle_devAtmosLightColorSet);
			lv_label_set_text(objpageDevALCSetLabel_colorPreview, "Preview:");
			lv_obj_set_protect(objpageDevALCSetLabel_colorPreview, LV_PROTECT_POS);
			if(true == devStatusDispMethod_landscapeIf_get()){
				lv_obj_align(objpageDevALCSetLabel_colorPreview, objpageDevAtmosLightColorSet_title, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 20);
			}else{
				lv_obj_align(objpageDevALCSetLabel_colorPreview, objpageDevAtmosLightColorSet_title, LV_ALIGN_OUT_BOTTOM_RIGHT, 15, 20);
			}
			objpageDevALCSetBlock_colorPreview = lv_obj_create(pageSettingA_devAtmosLightColorSet, NULL);
			lv_obj_set_size(objpageDevALCSetBlock_colorPreview, 35, 35);
			lv_obj_set_protect(objpageDevALCSetBlock_colorPreview, LV_PROTECT_POS);
			styleBtn_devAtmosLightColorSet->body.main_color = atmosLightRunParam_dataTemp.lightColorCustomParamcfg;
			styleBtn_devAtmosLightColorSet->body.grad_color = atmosLightRunParam_dataTemp.lightColorCustomParamcfg;
			lv_obj_set_style(objpageDevALCSetBlock_colorPreview, styleBtn_devAtmosLightColorSet);
			if(true == devStatusDispMethod_landscapeIf_get()){
				lv_obj_align(objpageDevALCSetBlock_colorPreview, objpageDevALCSetLabel_colorPreview, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
			}else{
				lv_obj_align(objpageDevALCSetBlock_colorPreview, objpageDevALCSetLabel_colorPreview, LV_ALIGN_OUT_BOTTOM_MID, 3, 14);
			}
			
			objpageDevALCSetLabel_ref = lv_label_create(pageSettingA_devAtmosLightColorSet, NULL);
			lv_label_set_style(objpageDevALCSetLabel_ref, styleTextPageTitle_devAtmosLightColorSet);
			lv_label_set_recolor(objpageDevALCSetLabel_ref, true);
			lv_label_set_text(objpageDevALCSetLabel_ref, "#FF2020 R  # #20FF20 G  # #2020FF B#");
			lv_obj_set_protect(objpageDevALCSetLabel_ref, LV_PROTECT_POS);
			if(true == devStatusDispMethod_landscapeIf_get()){
				lv_obj_align(objpageDevALCSetLabel_ref, objpageDevALCSetLabel_colorPreview, LV_ALIGN_OUT_LEFT_BOTTOM, -35, 100);
			}else{
				lv_obj_align(objpageDevALCSetLabel_ref, objpageDevALCSetLabel_colorPreview, LV_ALIGN_OUT_LEFT_BOTTOM, -12, 100);
			}
			
			objpageDevALCSetSlider_color_r = lv_slider_create(pageSettingA_devAtmosLightColorSet, NULL);
			lv_slider_set_range(objpageDevALCSetSlider_color_r, 0, 32 - 1);
			lv_obj_set_size(objpageDevALCSetSlider_color_r, 13, 110);
			lv_obj_set_protect(objpageDevALCSetSlider_color_r, LV_PROTECT_POS);
			lv_obj_align(objpageDevALCSetSlider_color_r, objpageDevALCSetLabel_ref, LV_ALIGN_OUT_TOP_LEFT, -2, -5);
			lv_slider_set_action(objpageDevALCSetSlider_color_r, funCb_slidAction_ALCS_set_color);
			lv_obj_set_free_num(objpageDevALCSetSlider_color_r, LV_FNUM_SLIDER_ALCS_R);
			lv_slider_set_value(objpageDevALCSetSlider_color_r, atmosLightRunParam_dataTemp.lightColorCustomParamcfg.red);
			objpageDevALCSetSlider_color_g = lv_slider_create(pageSettingA_devAtmosLightColorSet, objpageDevALCSetSlider_color_r);
			lv_slider_set_range(objpageDevALCSetSlider_color_g, 0, 64 - 1);
			lv_obj_align(objpageDevALCSetSlider_color_g, objpageDevALCSetSlider_color_r, LV_ALIGN_OUT_RIGHT_MID, 18, 0);
			lv_slider_set_action(objpageDevALCSetSlider_color_g, funCb_slidAction_ALCS_set_color);
			lv_obj_set_free_num(objpageDevALCSetSlider_color_g, LV_FNUM_SLIDER_ALCS_G);
			lv_slider_set_value(objpageDevALCSetSlider_color_g, atmosLightRunParam_dataTemp.lightColorCustomParamcfg.green);
			objpageDevALCSetSlider_color_b = lv_slider_create(pageSettingA_devAtmosLightColorSet, objpageDevALCSetSlider_color_r);
			lv_slider_set_range(objpageDevALCSetSlider_color_b, 0, 32 - 1);
			lv_obj_align(objpageDevALCSetSlider_color_b, objpageDevALCSetSlider_color_g, LV_ALIGN_OUT_RIGHT_MID, 19, 0);
			lv_slider_set_action(objpageDevALCSetSlider_color_b, funCb_slidAction_ALCS_set_color);
			lv_obj_set_free_num(objpageDevALCSetSlider_color_b, LV_FNUM_SLIDER_ALCS_B);
			lv_slider_set_value(objpageDevALCSetSlider_color_b, atmosLightRunParam_dataTemp.lightColorCustomParamcfg.blue);
			
			objpageDevALCSetCb_breathIf = lv_cb_create(pageSettingA_devAtmosLightColorSet, objpageDevALCSetCb_automaticSet);
			lv_label_set_recolor(objpageDevALCSetCb_breathIf, true);
			lv_obj_set_size(objpageDevALCSetCb_breathIf, 15, 80);
			if(true == devStatusDispMethod_landscapeIf_get()){
				lv_obj_align(objpageDevALCSetCb_breathIf, objpageDevALCSetLabel_colorPreview, LV_ALIGN_OUT_BOTTOM_LEFT, -27, 15);
			}else{
				lv_obj_align(objpageDevALCSetCb_breathIf, objpageDevALCSetLabel_ref, LV_ALIGN_OUT_BOTTOM_LEFT, -10, 0);
			}
			lv_cb_set_style(objpageDevALCSetCb_breathIf, LV_CB_STYLE_BG, styleLabelPageCb_devALSC_breathIf);
			lv_cb_set_text(objpageDevALCSetCb_breathIf, "illumination breath");
			lv_obj_set_free_num(objpageDevALCSetCb_breathIf, LV_FNUM_CB_ALCS_BRE_IF);
			(atmosLightRunParam_dataTemp.customCfg_breathIf)?
				(lv_cb_set_checked(objpageDevALCSetCb_breathIf, true)):
				(lv_cb_set_checked(objpageDevALCSetCb_breathIf, false));

			settingItem_ALCS_affiObjs_availableFlg = true;
		}

		// lv_obj_align(objpageDevALCSetCb_setAllSwitch, objpageDevALCSetCb_breathIf, LV_ALIGN_OUT_BOTTOM_MID, -10, -10);

		if(true == devStatusDispMethod_landscapeIf_get()){
			lv_obj_align(objpageDevALCSetCb_automaticSet, objpageDevALCSetCb_breathIf, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
			// lv_obj_align(objpageDevALCSetCb_automaticSet, objpageDevALCSetCb_setAllSwitch, LV_ALIGN_OUT_BOTTOM_MID, 0, -5);
		}else{
			lv_obj_align(objpageDevALCSetCb_automaticSet, pageSettingA_devAtmosLightColorSet, LV_ALIGN_IN_BOTTOM_MID, 0, -35);
			// lv_obj_align(objpageDevALCSetCb_automaticSet, pageSettingA_devAtmosLightColorSet, LV_ALIGN_IN_BOTTOM_MID, 0, -22);
		}
	}
	else{

		if(true == settingItem_ALCS_affiObjs_availableFlg){

			lv_obj_del(objpageDevALCSetLabel_colorPreview);
			lv_obj_del(objpageDevALCSetBlock_colorPreview);
			lv_obj_del(objpageDevALCSetLabel_ref);
			lv_obj_del(objpageDevALCSetSlider_color_r);
			lv_obj_del(objpageDevALCSetSlider_color_g);
			lv_obj_del(objpageDevALCSetSlider_color_b);
			lv_obj_del(objpageDevALCSetCb_breathIf);

			settingItem_ALCS_affiObjs_availableFlg = false;
		}
		
		lv_obj_align(objpageDevALCSetCb_automaticSet, pageSettingA_devAtmosLightColorSet, LV_ALIGN_CENTER, 0, 0);
		if(true == devStatusDispMethod_landscapeIf_get()){
			// lv_obj_align(objpageDevALCSetCb_setAllSwitch, objpageDevALCSetCb_automaticSet, LV_ALIGN_OUT_BOTTOM_MID, 14, 5);		
		}else{
			// lv_obj_align(objpageDevALCSetCb_setAllSwitch, objpageDevALCSetCb_automaticSet, LV_ALIGN_OUT_BOTTOM_MID, 14, 5);	
		}
	}
}

static void lvGui_pageAtmosLightColorSet_creat(void){

	devAtmosLight_runningParam_get(&atmosLightRunParam_dataTemp);

	atmosLightCfgOptAvailableAllSw_if = false;

	if(pageSettingA_devAtmosLightColorSet == NULL)
		pageSettingA_devAtmosLightColorSet = lv_page_create(lv_scr_act(), NULL);

	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(pageSettingA_devAtmosLightColorSet, 280, 190)):
		(lv_obj_set_size(pageSettingA_devAtmosLightColorSet, 200, 260));
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_pos(pageSettingA_devAtmosLightColorSet, 20, 25)):
		(lv_obj_set_pos(pageSettingA_devAtmosLightColorSet, 20, 45));	
	stylePage_devAtmosLightColorSet->body.border.color = atmosLightRunParam_dataTemp.lightColorCustomParamcfg;
	lv_page_set_style(pageSettingA_devAtmosLightColorSet, LV_PAGE_STYLE_SB, stylePage_devAtmosLightColorSet);
	lv_page_set_style(pageSettingA_devAtmosLightColorSet, LV_PAGE_STYLE_BG, stylePage_devAtmosLightColorSet);
	lv_page_set_sb_mode(pageSettingA_devAtmosLightColorSet, LV_SB_MODE_HIDE); 	
	lv_page_set_scrl_fit(pageSettingA_devAtmosLightColorSet, false, true); //key opration
	lv_page_set_scrl_layout(pageSettingA_devAtmosLightColorSet, LV_LAYOUT_PRETTY);

	objpageDevAtmosLightColorSet_title = lv_label_create(pageSettingA_devAtmosLightColorSet, NULL);
	lv_obj_set_style(objpageDevAtmosLightColorSet_title, styleTextPageTitle_devAtmosLightColorSet);
	lv_label_set_text(objpageDevAtmosLightColorSet_title, "Illumination set");
	lv_obj_set_protect(objpageDevAtmosLightColorSet_title, LV_PROTECT_POS);
	lv_obj_align(objpageDevAtmosLightColorSet_title, pageSettingA_deviceTypeSelect, LV_ALIGN_IN_TOP_MID, 0, -5);

	objpageDevALCSetCb_automaticSet = lv_cb_create(pageSettingA_devAtmosLightColorSet, NULL);
	lv_label_set_recolor(objpageDevALCSetCb_automaticSet, true);
	lv_obj_set_size(objpageDevALCSetCb_automaticSet, 15, 80);
	lv_obj_set_protect(objpageDevALCSetCb_automaticSet, LV_PROTECT_POS);
	lv_obj_align(objpageDevALCSetCb_automaticSet, pageSettingA_devAtmosLightColorSet, LV_ALIGN_CENTER, 0, 0);
	lv_cb_set_style(objpageDevALCSetCb_automaticSet, LV_CB_STYLE_BG, styleLabelPageCb_devALSC_autoSet);
	lv_cb_set_text(objpageDevALCSetCb_automaticSet, "Automatic");
	lv_obj_set_free_num(objpageDevALCSetCb_automaticSet, LV_FNUM_CB_ALCS_AUTOMATIC);
	lv_cb_set_action(objpageDevALCSetCb_automaticSet, funCb_cbAction_ALCS_exParamSet);
	(atmosLightRunParam_dataTemp.runingAsCustomCfg_flg)?
		(lv_cb_set_checked(objpageDevALCSetCb_automaticSet, false)):
		(lv_cb_set_checked(objpageDevALCSetCb_automaticSet, true));
	// objpageDevALCSetCb_setAllSwitch = lv_cb_create(pageSettingA_devAtmosLightColorSet, objpageDevALCSetCb_automaticSet);
	// lv_cb_set_style(objpageDevALCSetCb_setAllSwitch, LV_CB_STYLE_BG, styleLabelPageCb_devALSC_setAllSw);
	// lv_cb_set_text(objpageDevALCSetCb_setAllSwitch, "set all switches");
	// lv_obj_set_free_num(objpageDevALCSetCb_setAllSwitch, LV_FNUM_CB_ALCS_SETALL_IF);
	// lv_cb_set_checked(objpageDevALCSetCb_setAllSwitch, false);
	lvGui_pageAtmosLightColorSetAffiAttend(!lv_cb_is_checked(objpageDevALCSetCb_automaticSet));

	objpageDevAtmosLightColorSetBtn_confirm = lv_btn_create(pageSettingA_devAtmosLightColorSet, NULL);
    lv_btn_set_style(objpageDevAtmosLightColorSetBtn_confirm, LV_BTN_STYLE_REL, &lv_style_transp);
    lv_btn_set_style(objpageDevAtmosLightColorSetBtn_confirm, LV_BTN_STYLE_PR, &lv_style_transp);
    lv_btn_set_style(objpageDevAtmosLightColorSetBtn_confirm, LV_BTN_STYLE_TGL_REL, &lv_style_transp);
    lv_btn_set_style(objpageDevAtmosLightColorSetBtn_confirm, LV_BTN_STYLE_TGL_PR, &lv_style_transp);
	lv_btn_set_fit(objpageDevAtmosLightColorSetBtn_confirm, false, false);
	lv_obj_set_size(objpageDevAtmosLightColorSetBtn_confirm, 80, 25);
	lv_page_glue_obj(objpageDevAtmosLightColorSetBtn_confirm, true);
	lv_obj_set_protect(objpageDevAtmosLightColorSetBtn_confirm, LV_PROTECT_FOLLOW);
	lv_obj_set_protect(objpageDevAtmosLightColorSetBtn_confirm, LV_PROTECT_POS);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(objpageDevAtmosLightColorSetBtn_confirm, pageSettingA_devAtmosLightColorSet, LV_ALIGN_IN_BOTTOM_LEFT, 45, -1)):
		(lv_obj_align(objpageDevAtmosLightColorSetBtn_confirm, pageSettingA_devAtmosLightColorSet, LV_ALIGN_IN_BOTTOM_LEFT, 25, -1));
	lv_btn_set_action(objpageDevAtmosLightColorSetBtn_confirm, LV_BTN_ACTION_CLICK, funCb_btnActionPress_btnPage_devAtmosLightColor_confirm);
	objpageDevAtmosLightColorSetBtn_cancel = lv_btn_create(pageSettingA_devAtmosLightColorSet, objpageDevAtmosLightColorSetBtn_confirm);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_align(objpageDevAtmosLightColorSetBtn_cancel, pageSettingA_devAtmosLightColorSet, LV_ALIGN_IN_BOTTOM_RIGHT, -45, -1)):
		(lv_obj_align(objpageDevAtmosLightColorSetBtn_cancel, objpageDevAtmosLightColorSetBtn_confirm, LV_ALIGN_CENTER, 85, 0));
	lv_btn_set_action(objpageDevAtmosLightColorSetBtn_cancel, LV_BTN_ACTION_CLICK, funCb_btnActionPress_btnPage_devAtmosLightColor_cancel);
	
	objpageDevAtmosLightColorSet_confirm = lv_label_create(objpageDevAtmosLightColorSetBtn_confirm, NULL);
	lv_obj_set_style(objpageDevAtmosLightColorSet_confirm, styleLabelPageBtn_devAtmosLightColorSet);
	lv_label_set_text(objpageDevAtmosLightColorSet_confirm, "confirm");
	objpageDevAtmosLightColorSet_cancel = lv_label_create(objpageDevAtmosLightColorSetBtn_cancel, objpageDevAtmosLightColorSet_confirm);
	lv_obj_set_style(objpageDevAtmosLightColorSet_cancel, styleLabelPageBtn_devAtmosLightColorSet);
	lv_label_set_text(objpageDevAtmosLightColorSet_cancel, "cancel");

	usrApp_fullScreenRefresh_self(20, 80);
}

static lv_res_t funCb_btnActionClick_btnDevType_funBack(lv_obj_t *btn){

	lvGui_pageDevTypeSeclect_creat();

	return LV_RES_OK;
}

static lv_res_t funCb_ddlistDevTypeDef(lv_obj_t *ddlist){

	uint8_t loop = 0;
	uint8_t ddlist_id = lv_obj_get_free_num(ddlist);	

	if(ddlist_id == OBJ_DDLIST_DEVTYPE_FREENUM){

		uint16_t ddlist_opSelect = lv_ddlist_get_selected(ddlist);

		if(devTypeDdist_istRecord != ddlist_opSelect){

			devTypeDdist_istRecord = ddlist_opSelect;
			
			for(loop = 0; loop < DEVICE_TYPE_LIST_NUM; loop ++){
			
				if(ddlist_opSelect == deviceTypeTab_disp[loop].devInst_disp){
			
					stt_devDataPonitTypedef devDataPoint_temp = {0};
			
					currentDev_typeSet(deviceTypeTab_disp[loop].devType, true);
					devDriverManageBussiness_deviceChangeRefresh(); //驱动更新
					switch(currentDev_typeGet()){

						case devTypeDef_thermostat:
						case devTypeDef_thermostatExtension:{

							devDataPoint_temp.devType_thermostat.devThermostat_tempratureTarget = DEVICE_THERMOSTAT_TEMPTARGET_DEFAULT;
							currentDev_dataPointSet(&devDataPoint_temp, true, false, false, false, false); //数据点加载默认值

						}break;

						case devTypeDef_scenario:{

							/*什么都不做*/
				
						}break;

						case devTypeDef_curtain:{

							stt_devStatusRecord devStatusRecordFlg_temp = {0};
							
							devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

							devDataPoint_temp.devType_curtain.devCurtain_actMethod = 1;
							devDataPoint_temp.devType_curtain.devCurtain_actEnumVal = 0;
							currentDev_dataPointSet(&devDataPoint_temp, true, true, true, false, false);

							devStatusRecordFlg_temp.devStatusOnOffRecord_IF = 0;
							devStatusRecordIF_paramSet(&devStatusRecordFlg_temp, true);
						}break;

						default:{

							currentDev_dataPointSet(&devDataPoint_temp, true, true, true, false, false); //数据点清零
						}break;
					}
			
					devSystemInfoLocalRecord_normalClear();
			
					usrAppHomepageBtnTextDisp_defaultLoad(deviceTypeTab_disp[loop].devType, true); //图文显示恢复默认
					usrAppHomepageBtnIconNumDisp_defaultLoad(deviceTypeTab_disp[loop].devType, true);
					
					break;
				}
			}
		}
	}

	return LV_RES_OK;
}

static lv_res_t funCb_ddlistHomepageThemeDef(lv_obj_t *ddlist){

	uint8_t loop = 0;
	uint8_t ddlist_id = lv_obj_get_free_num(ddlist);	

	if(ddlist_id == OBJ_DDLIST_HPTHEME_FREENUM){

		uint16_t ddlist_opSelect = lv_ddlist_get_selected(ddlist);

		usrAppHomepageThemeType_Set(ddlist_opSelect, true, true);
	}

	return LV_RES_OK;
}

static lv_res_t funCb_swDevStatusRecordIF(lv_obj_t *sw){

	uint8_t sw_action = (uint8_t)!lv_sw_get_state(sw);
	stt_devStatusRecord devStatusRecordFlg_temp = {0};

	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

	printf("swDevStatusRecordIF set value get:%d.\n", sw_action);

	if(sw_action){

		devStatusRecordFlg_temp.devStatusOnOffRecord_IF = 1;
	}
	else
	{
		devStatusRecordFlg_temp.devStatusOnOffRecord_IF = 0;
	}
	devStatusRecordIF_paramSet(&devStatusRecordFlg_temp, true);

	return LV_RES_OK;
}

static lv_res_t funCb_rollerOpreat_timeZoneUnitSetHour(lv_obj_t *rol){

	uint16_t valSet = lv_roller_get_selected(rol);
	stt_timeZone tzData = {0};
	
	deviceParamGet_timeZone(&tzData);
	tzData.timeZone_H = valSet;
	if(lv_cb_is_checked(cbTzSet_minusIf) && tzData.timeZone_H < 24)tzData.timeZone_H += 24;
	deviceParamSet_timeZone(&tzData, true);

	// printf("tz H:%d,M:%d.\n", tzData.timeZone_H, tzData.timeZone_M);

	return LV_RES_OK;
}

static lv_res_t funCb_rollerOpreat_timeZoneUnitSetMinute(lv_obj_t *rol){

	uint16_t valSet = lv_roller_get_selected(rol);
	stt_timeZone tzData = {0};
	
	deviceParamGet_timeZone(&tzData);
	tzData.timeZone_M = valSet;
	deviceParamSet_timeZone(&tzData, true);

	// printf("tz H:%d,M:%d.\n", tzData.timeZone_H, tzData.timeZone_M);

	return LV_RES_OK;
}

static lv_res_t funCb_cbOpreat_tzSet_minusIf(lv_obj_t *cb){

	stt_timeZone tzData = {0};
	
	deviceParamGet_timeZone(&tzData);
	if(lv_cb_is_checked(cb)){

		if(tzData.timeZone_H > 23);
		else{

			tzData.timeZone_H += 24;
		}

	}else{

		if(tzData.timeZone_H > 23){

			tzData.timeZone_H -= 24;
		}
	}

	deviceParamSet_timeZone(&tzData, true);

	// printf("tz H:%d,M:%d.\n", tzData.timeZone_H, tzData.timeZone_M);

	return LV_RES_OK;
}

static lv_res_t funCb_btnActionClick_devAtmosLightColorSet(lv_obj_t *sw){

	lvGui_pageAtmosLightColorSet_creat();

	return LV_RES_OK;
}

// static lv_res_t funCb_swDevListManagerEN(lv_obj_t *sw){

// 	uint8_t sw_action = (uint8_t)!lv_sw_get_state(sw);
// 	stt_devStatusRecord devStatusRecordFlg_temp = {0};

// 	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

// 	printf("swDevListManagerEN set value get:%d.\n", sw_action);

// 	if(sw_action){

// 		devStatusRecordFlg_temp.devListManager_En = 1;
// 	}
// 	else
// 	{
// 		devStatusRecordFlg_temp.devListManager_En = 0;
// 	}
// 	devStatusRecordIF_paramSet(&devStatusRecordFlg_temp, true);

// 	return LV_RES_OK;
// }

// static lv_res_t funCb_homeassistantEnable(lv_obj_t *sw){

// 	uint8_t sw_action = (uint8_t)!lv_sw_get_state(sw);
// 	stt_devStatusRecord devStatusRecordFlg_temp = {0};

// 	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

// 	printf("homeassistantEnable set value get:%d.\n", sw_action);

// 	if(sw_action){

// 		devStatusRecordFlg_temp.homeassitant_En = 1;
// 	}
// 	else
// 	{
// 		devStatusRecordFlg_temp.homeassitant_En = 0;
// 	}
	
// 	devStatusRecordIF_paramSet(&devStatusRecordFlg_temp, true);

// 	dtRmoteServer_serverSwitchByDefault_trig(sw_action);
// 	lvGui_usrAppBussinessRunning_block(0, "\nparam changing", 5);	

// 	return LV_RES_OK;
// }

static lv_res_t funCb_epidemicReportIF(lv_obj_t *sw){

	uint8_t sw_action = (uint8_t)!lv_sw_get_state(sw);
	stt_devStatusRecord devStatusRecordFlg_temp = {0};

	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);

	printf("swEpidemicReportIF set value get:%d.\n", sw_action);

	if(sw_action){

		devStatusRecordFlg_temp.screensaverRunning_IF = 1;
	}
	else
	{
		devStatusRecordFlg_temp.screensaverRunning_IF = 0;
	}
	
	lvGui_settingSet_epidemicCtrlObj_opt(devStatusRecordFlg_temp.screensaverRunning_IF);
	
	devStatusRecordIF_paramSet(&devStatusRecordFlg_temp, true);

	return LV_RES_OK;
}

static lv_res_t funCb_rollerEpidemicCyLocation(lv_obj_t *rol){

	uint16_t valSet = lv_roller_get_selected(rol);

	sprintf(textlabel_dispTemp, "location[#FFFF00 %s#]:", epidCyAbbre[valSet]);
	lv_label_set_text(textSettingA_epidemicCountry, (const char *)textlabel_dispTemp);

	dispApplication_epidCyLocation_set(valSet, true);
	
	return LV_RES_OK;
}

static void lvGuiMenuSettingSet_styleMemoryInitialization(void){

	static bool memAlloced_flg = false;

	if(true == memAlloced_flg)return;
	else memAlloced_flg = true;

	stylePage_funSetOption 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_menuLevel_A 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_menuLevel_B 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_menuLevel_C 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBk_objBground 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleDdlistSettingA_devType = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabel_devTypeSelected	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	bg_styleDevStatusRecordIF 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	indic_styleDevStatusRecordIF = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	knob_on_styleDevStatusRecordIF = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	knob_off_styleDevStatusRecordIF = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleMbox_bg 				= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleMbox_btn_pr 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleMbox_btn_rel 			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtn_specialTransparent = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleImg_menuFun_btnFun 	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePageCb_elecParamDispSet = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePageCb_screenLandscape = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleRoller_epidemicCyLocation_bg = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleRoller_epidemicCyLocation_sel = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePage_devTypeSet 		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleTextPageTitle_devTypeSet = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePageCb_devTypeSet		= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabelPageBtn_devTypeSet = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleText_menuTzSet			= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePageCb_tzSetMinusIf	= (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleBtn_devAtmosLightColorSet = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	stylePage_devAtmosLightColorSet = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleTextPageTitle_devAtmosLightColorSet = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabelPageBtn_devAtmosLightColorSet = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabelPageCb_devALSC_breathIf = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabelPageCb_devALSC_setAllSw = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
	styleLabelPageCb_devALSC_autoSet = (lv_style_t *)LV_MEM_CUSTOM_ALLOC(sizeof(lv_style_t));
}

static void lvGuiSettingSet_objStyle_Init(void){

	lvGuiMenuSettingSet_styleMemoryInitialization();

	lv_style_copy(styleText_menuLevel_A, &lv_style_plain);
	styleText_menuLevel_A->text.font = &lv_font_dejavu_20;
	styleText_menuLevel_A->text.color = LV_COLOR_WHITE;
	
	lv_style_copy(styleText_menuLevel_B, &lv_style_plain);
	styleText_menuLevel_B->text.font = &lv_font_dejavu_15;
	styleText_menuLevel_B->text.color = LV_COLOR_WHITE;

	lv_style_copy(styleText_menuLevel_C, &lv_style_plain);
	styleText_menuLevel_C->text.font = &lv_font_consola_16;
	styleText_menuLevel_C->text.color = LV_COLOR_WHITE;

	lv_style_copy(stylePage_funSetOption, &lv_style_plain);
	stylePage_funSetOption->body.main_color = LV_COLOR_GRAY;
	stylePage_funSetOption->body.grad_color = LV_COLOR_GRAY;

	lv_style_copy(styleDdlistSettingA_devType, &lv_style_pretty);
	styleDdlistSettingA_devType->body.shadow.width = 1;
	styleDdlistSettingA_devType->text.color = LV_COLOR_MAKE(0x10, 0x20, 0x50);
	styleDdlistSettingA_devType->text.font = &lv_font_dejavu_15;
	
	lv_style_copy(styleLabel_devTypeSelected, &lv_style_plain);
	styleLabel_devTypeSelected->text.font = &lv_font_ariblk_18;
	styleLabel_devTypeSelected->text.color = LV_COLOR_MAKE(192, 255, 0);

	lv_style_copy(bg_styleDevStatusRecordIF, &lv_style_pretty);
	bg_styleDevStatusRecordIF->body.radius = LV_RADIUS_CIRCLE;
	lv_style_copy(indic_styleDevStatusRecordIF, &lv_style_pretty_color);
	indic_styleDevStatusRecordIF->body.radius = LV_RADIUS_CIRCLE;
	indic_styleDevStatusRecordIF->body.main_color = LV_COLOR_HEX(0x9fc8ef);
	indic_styleDevStatusRecordIF->body.grad_color = LV_COLOR_HEX(0x9fc8ef);
	indic_styleDevStatusRecordIF->body.padding.hor = 0;
	indic_styleDevStatusRecordIF->body.padding.ver = 0;	
	lv_style_copy(knob_off_styleDevStatusRecordIF, &lv_style_pretty);
	knob_off_styleDevStatusRecordIF->body.radius = LV_RADIUS_CIRCLE;
	knob_off_styleDevStatusRecordIF->body.shadow.width = 4;
	knob_off_styleDevStatusRecordIF->body.shadow.type = LV_SHADOW_BOTTOM;
	lv_style_copy(knob_on_styleDevStatusRecordIF, &lv_style_pretty_color);
	knob_on_styleDevStatusRecordIF->body.radius = LV_RADIUS_CIRCLE;
	knob_on_styleDevStatusRecordIF->body.shadow.width = 4;
	knob_on_styleDevStatusRecordIF->body.shadow.type = LV_SHADOW_BOTTOM;

	lv_style_copy(styleMbox_bg, &lv_style_pretty);
	styleMbox_bg->body.main_color = LV_COLOR_MAKE(0xf5, 0x45, 0x2e);
	styleMbox_bg->body.grad_color = LV_COLOR_MAKE(0xb9, 0x1d, 0x09);
	styleMbox_bg->body.border.color = LV_COLOR_MAKE(0x3f, 0x0a, 0x03);
	styleMbox_bg->text.font = &lv_font_consola_17;
	styleMbox_bg->text.color = LV_COLOR_WHITE;
	styleMbox_bg->body.padding.hor = 12;
	styleMbox_bg->body.padding.ver = 8;
	styleMbox_bg->body.shadow.width = 8;
	lv_style_copy(styleMbox_btn_rel, &lv_style_btn_rel);
	styleMbox_btn_rel->text.font = &lv_font_consola_19;
	styleMbox_btn_rel->body.empty = 1;					 /*Draw only the border*/
	styleMbox_btn_rel->body.border.color = LV_COLOR_WHITE;
	styleMbox_btn_rel->body.border.width = 2;
	styleMbox_btn_rel->body.border.opa = LV_OPA_70;
	styleMbox_btn_rel->body.padding.hor = 12;
	styleMbox_btn_rel->body.padding.ver = 8;
	lv_style_copy(styleMbox_btn_pr, styleMbox_btn_rel);
	styleMbox_btn_pr->body.empty = 0;
	styleMbox_btn_pr->body.main_color = LV_COLOR_MAKE(0x5d, 0x0f, 0x04);
	styleMbox_btn_pr->body.grad_color = LV_COLOR_MAKE(0x5d, 0x0f, 0x04);

    lv_style_copy(styleBtn_specialTransparent, &lv_style_btn_rel);
	styleBtn_specialTransparent->body.main_color = LV_COLOR_TRANSP;
	styleBtn_specialTransparent->body.grad_color = LV_COLOR_TRANSP;
	styleBtn_specialTransparent->body.border.part = LV_BORDER_NONE;
    styleBtn_specialTransparent->body.opa = LV_OPA_TRANSP;
	styleBtn_specialTransparent->body.radius = 0;
    styleBtn_specialTransparent->body.shadow.width =  0;

	lv_style_copy(styleImg_menuFun_btnFun, &lv_style_plain);
	styleImg_menuFun_btnFun->image.intense = LV_OPA_COVER;
	styleImg_menuFun_btnFun->image.color = LV_COLOR_MAKE(200, 191, 231);

	lv_style_copy(stylePageCb_elecParamDispSet, &lv_style_plain);
	stylePageCb_elecParamDispSet->body.border.part = LV_BORDER_NONE;
	stylePageCb_elecParamDispSet->body.empty = 1;
	stylePageCb_elecParamDispSet->text.color = LV_COLOR_AQUA;
	stylePageCb_elecParamDispSet->text.font = &lv_font_consola_16;

	lv_style_copy(stylePageCb_screenLandscape, &lv_style_plain);
	stylePageCb_screenLandscape->body.border.part = LV_BORDER_NONE;
	stylePageCb_screenLandscape->body.empty = 1;
	stylePageCb_screenLandscape->text.color = LV_COLOR_AQUA;
	stylePageCb_screenLandscape->text.font = &lv_font_consola_16;

	lv_style_copy(styleRoller_epidemicCyLocation_bg, &lv_style_plain);
	styleRoller_epidemicCyLocation_bg->body.main_color = LV_COLOR_GRAY;
	styleRoller_epidemicCyLocation_bg->body.grad_color = LV_COLOR_GRAY;
	styleRoller_epidemicCyLocation_bg->text.color = LV_COLOR_SILVER;
	styleRoller_epidemicCyLocation_bg->text.font = &lv_font_consola_13;
	styleRoller_epidemicCyLocation_bg->text.line_space = 5;
	styleRoller_epidemicCyLocation_bg->text.opa = LV_OPA_COVER;
	lv_style_copy(styleRoller_epidemicCyLocation_sel, &lv_style_plain);
	styleRoller_epidemicCyLocation_sel->body.empty = 1;
	styleRoller_epidemicCyLocation_sel->body.radius = 30;
	styleRoller_epidemicCyLocation_sel->text.color = LV_COLOR_YELLOW;
	styleRoller_epidemicCyLocation_sel->text.font = &lv_font_consola_13;

	lv_style_copy(stylePage_devTypeSet, &lv_style_plain_color);
	stylePage_devTypeSet->body.main_color = LV_COLOR_SILVER;
	stylePage_devTypeSet->body.grad_color = LV_COLOR_SILVER;
	stylePage_devTypeSet->body.border.part = LV_BORDER_NONE;
	stylePage_devTypeSet->body.radius = 6;
	stylePage_devTypeSet->body.opa = LV_OPA_90;
	stylePage_devTypeSet->body.padding.hor = 0;
	stylePage_devTypeSet->body.padding.inner = 0;	

	lv_style_copy(stylePage_devAtmosLightColorSet, &lv_style_plain_color);
	stylePage_devAtmosLightColorSet->body.main_color = LV_COLOR_MAKE(0xB0, 0xB0, 0xB0);
	stylePage_devAtmosLightColorSet->body.grad_color = LV_COLOR_MAKE(0xB0, 0xB0, 0xB0);
	stylePage_devAtmosLightColorSet->body.border.part = LV_BORDER_FULL;
	stylePage_devAtmosLightColorSet->body.border.width = 2;
	stylePage_devAtmosLightColorSet->body.border.color = LV_COLOR_YELLOW;
	stylePage_devAtmosLightColorSet->body.radius = 3;
	stylePage_devAtmosLightColorSet->body.opa = LV_OPA_COVER;
	stylePage_devAtmosLightColorSet->body.padding.hor = 0;
	stylePage_devAtmosLightColorSet->body.padding.inner = 0;

	lv_style_copy(styleTextPageTitle_devTypeSet, &lv_style_plain);
	styleTextPageTitle_devTypeSet->text.font = &lv_font_consola_19;
	styleTextPageTitle_devTypeSet->text.color = LV_COLOR_BLACK;

	lv_style_copy(stylePageCb_devTypeSet, &lv_style_plain);
	stylePageCb_devTypeSet->body.border.part = LV_BORDER_NONE;
	stylePageCb_devTypeSet->body.empty = 1;
//	stylePageCb_devTypeSet->text.color = LV_COLOR_MAKE(192, 255, 0);
	stylePageCb_devTypeSet->text.color = LV_COLOR_BLACK;
	stylePageCb_devTypeSet->text.font = &lv_font_consola_17;

	lv_style_copy(styleLabelPageBtn_devTypeSet, &lv_style_plain);
	styleLabelPageBtn_devTypeSet->text.font = &lv_font_ariblk_18;
	styleLabelPageBtn_devTypeSet->text.color = LV_COLOR_MAKE(0, 128, 255);

	lv_style_copy(styleText_menuTzSet, &lv_style_plain);
	styleText_menuTzSet->text.font = &lv_font_arial_15;
	styleText_menuTzSet->text.color = LV_COLOR_MAKE(80, 240, 80);

	lv_style_copy(stylePageCb_tzSetMinusIf, &lv_style_plain);
	stylePageCb_tzSetMinusIf->body.border.part = LV_BORDER_NONE;
	stylePageCb_tzSetMinusIf->body.empty = 1;
	stylePageCb_tzSetMinusIf->text.color = LV_COLOR_AQUA;
	stylePageCb_tzSetMinusIf->text.font = &lv_font_arial_15;

	lv_style_copy(styleBtn_devAtmosLightColorSet, &lv_style_plain);
	styleBtn_devAtmosLightColorSet->body.main_color = LV_COLOR_WHITE;
	styleBtn_devAtmosLightColorSet->body.grad_color = LV_COLOR_WHITE;
	styleBtn_devAtmosLightColorSet->body.opa = LV_OPA_COVER;
	styleBtn_devAtmosLightColorSet->body.border.part = LV_BORDER_NONE;
	styleBtn_devAtmosLightColorSet->body.radius = 0;

	lv_style_copy(styleTextPageTitle_devAtmosLightColorSet, &lv_style_plain);
	styleTextPageTitle_devAtmosLightColorSet->body.border.part = LV_BORDER_NONE;
	styleTextPageTitle_devAtmosLightColorSet->body.empty = 1;
//	styleTextPageTitle_devAtmosLightColorSet->text.color = LV_COLOR_MAKE(192, 255, 0);
	styleTextPageTitle_devAtmosLightColorSet->text.color = LV_COLOR_BLACK;
	styleTextPageTitle_devAtmosLightColorSet->text.font = &lv_font_consola_17;

	lv_style_copy(styleLabelPageBtn_devAtmosLightColorSet, &lv_style_plain);
	styleLabelPageBtn_devAtmosLightColorSet->text.font = &lv_font_consola_19;
	styleLabelPageBtn_devAtmosLightColorSet->text.color = LV_COLOR_MAKE(0, 128, 255);

	lv_style_copy(styleLabelPageCb_devALSC_breathIf, &lv_style_plain);
	styleLabelPageCb_devALSC_breathIf->body.border.part = LV_BORDER_NONE;
	styleLabelPageCb_devALSC_breathIf->body.empty = 1;
	styleLabelPageCb_devALSC_breathIf->text.color = LV_COLOR_MAKE(128, 128, 255);
	styleLabelPageCb_devALSC_breathIf->text.font = &lv_font_dejavu_15;

	lv_style_copy(styleLabelPageCb_devALSC_setAllSw, styleLabelPageCb_devALSC_breathIf);
	styleLabelPageCb_devALSC_setAllSw->text.color = LV_COLOR_MAKE(255, 128, 64);

	lv_style_copy(styleLabelPageCb_devALSC_autoSet, styleLabelPageCb_devALSC_breathIf);
	styleLabelPageCb_devALSC_autoSet->text.color = LV_COLOR_CYAN;
	styleLabelPageCb_devALSC_autoSet->text.font = &lv_font_consola_16;
}

static void lvGui_settingSet_epidemicCtrlObj_opt(bool creatIf){

	if(esp_mesh_get_layer() != MESH_ROOT)return;

	if(creatIf){

		if(textSettingA_epidemicCountry == NULL){

			uint8_t cyIst = dispApplication_epidCyLocation_get();
			
			textSettingA_epidemicCountry = lv_label_create(page_funSetOption, NULL);
			lv_label_set_recolor(textSettingA_epidemicCountry, true);
			sprintf(textlabel_dispTemp, "location[#FFFF00 %s#]:", epidCyAbbre[cyIst]);
			lv_label_set_text(textSettingA_epidemicCountry, (const char *)textlabel_dispTemp);
			lv_obj_set_style(textSettingA_epidemicCountry, styleText_menuLevel_B);
			lv_obj_set_protect(textSettingA_epidemicCountry, LV_PROTECT_POS);
			lv_obj_align(textSettingA_epidemicCountry, textSettingA_epidemicReport, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
			
			rollerEpidemic_cyLocation = lv_roller_create(page_funSetOption, NULL);
			lv_roller_set_action(rollerEpidemic_cyLocation, funCb_rollerEpidemicCyLocation);
			lv_roller_set_options(rollerEpidemic_cyLocation, epidCyName);
			lv_obj_set_protect(rollerEpidemic_cyLocation, LV_PROTECT_POS);
			lv_roller_set_hor_fit(rollerEpidemic_cyLocation, false);
			lv_obj_set_width(rollerEpidemic_cyLocation, 215);
			lv_obj_align(rollerEpidemic_cyLocation, textSettingA_epidemicCountry, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset, 5);
			lv_roller_set_selected(rollerEpidemic_cyLocation, cyIst, false);
			lv_roller_set_style(rollerEpidemic_cyLocation, LV_ROLLER_STYLE_BG, styleRoller_epidemicCyLocation_bg);
			lv_roller_set_style(rollerEpidemic_cyLocation, LV_ROLLER_STYLE_SEL, styleRoller_epidemicCyLocation_sel);
			lv_roller_set_visible_row_count(rollerEpidemic_cyLocation, 4);
		}
	}
	else
	{
		if(textSettingA_epidemicCountry != NULL){

			lv_obj_del(rollerEpidemic_cyLocation);
			lv_obj_del(textSettingA_epidemicCountry);

			rollerEpidemic_cyLocation = NULL;
			textSettingA_epidemicCountry = NULL;
		}
	}
}

void paramCfgTemp_ALCScolor_get(lv_color_t *c){

	memcpy(c, &atmosLightRunParam_dataTemp.lightColorCustomParamcfg, sizeof(lv_color_t));
}

void paramSettingGet_epidCyAbbre(char param[2]){

	memcpy(param, epidCyAbbre[dispApplication_epidCyLocation_get()], sizeof(char) * 2);
}

void paramSettingGet_epidCyName(char *nameStr){

	const uint8_t ctIstMax = 226;

	uint8_t targetIst = dispApplication_epidCyLocation_get() + 1;
	uint8_t loop = 0;
	char cyNameStr_temp[32] = {0};
	uint16_t cyTempCounter = 0;
	uint16_t tabCyNameCursor = 0;

	if(targetIst > ctIstMax)return;
	else
	if(targetIst == ctIstMax){
		
		strcpy(nameStr, "Zimbabwe");
	}
	else{
		
		for(loop = 0; loop < targetIst; loop ++){
	
			memset(cyNameStr_temp, 0, sizeof(char) * 32);
	
			do{
				
				cyTempCounter ++;
				tabCyNameCursor ++;
			
			}while(epidCyName[tabCyNameCursor] != '\n');
	
			memcpy(cyNameStr_temp, &epidCyName[tabCyNameCursor - cyTempCounter], cyTempCounter);
	
			cyTempCounter = 0;
			tabCyNameCursor ++;
		}
		
		strcpy(nameStr, cyNameStr_temp);	
	}
	
	printf("epidCyName get:<%s>\n", nameStr);
}

void lvGui_businessMenu_settingSet(lv_obj_t * obj_Parent){

	const uint16_t obj_animate_time = 150;
	const uint16_t obj_animate_delay = 100;
	uint16_t obj_animate_delayBasic = 0;

	uint8_t loop = 0;
	devTypeDef_enum devType_Temp;
	uint8_t homepageThemeStyle_temp = 0;
	stt_devStatusRecord devStatusRecordFlg_temp = {0};
	stt_timeZone tzData = {0};

	lv_obj_t *lvObjAlign_temp = NULL;
	
	lvGuiSettingSet_objStyle_Init();

	devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);
	devAtmosLight_runningParam_get(&atmosLightRunParam_dataTemp);

	deviceParamGet_timeZone(&tzData);

	(devStatusDispMethod_landscapeIf_get())?
		(screenLandscapeCoordinate_objOffset = 60):
		(screenLandscapeCoordinate_objOffset = 0);

	text_Title = lv_label_create(obj_Parent, NULL);
	lv_label_set_text(text_Title, "device define");
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
	lv_obj_set_pos(menuBtnChoIcon_fun_back, 0, 22);
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

	lv_obj_set_click(menuBtnChoIcon_fun_home, false);
	lv_obj_set_click(menuBtnChoIcon_fun_back, false);

	page_funSetOption = lv_page_create(lv_scr_act(), NULL);
	(devStatusDispMethod_landscapeIf_get())?
		(lv_obj_set_size(page_funSetOption, 320, 165)):
		(lv_obj_set_size(page_funSetOption, 240, 245));
	lv_obj_set_pos(page_funSetOption, 0, 75);
	lv_page_set_style(page_funSetOption, LV_PAGE_STYLE_SB, stylePage_funSetOption);
	lv_page_set_style(page_funSetOption, LV_PAGE_STYLE_BG, stylePage_funSetOption);
	lv_page_set_sb_mode(page_funSetOption, LV_SB_MODE_HIDE);
	lv_page_set_scrl_fit(page_funSetOption, false, false); //key opration
	if(devStatusDispMethod_landscapeIf_get()){
		
		lv_page_set_scrl_width(page_funSetOption, 300); 
		lv_page_set_scrl_height(page_funSetOption, 1150); 
	}
	else
	{
		lv_page_set_scrl_width(page_funSetOption, 220); //scrl尺寸必须大于set size尺寸 才可以进行拖拽
		lv_page_set_scrl_height(page_funSetOption, 1200); //scrl尺寸必须大于set size尺寸 才可以进行拖拽
	}

	lv_page_set_scrl_layout(page_funSetOption, LV_LAYOUT_CENTER);
//	lv_page_scroll_ver(page_funSetOption, 480);

	textSettingA_deviceType = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_deviceType, "Device Type:");
	lv_obj_set_style(textSettingA_deviceType, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_deviceType, LV_PROTECT_POS);
	lv_obj_align(textSettingA_deviceType, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 20);
	btnSettingA_deviceType = lv_btn_create(page_funSetOption, NULL);
	lv_obj_set_size(btnSettingA_deviceType, 200, 35);
    lv_btn_set_style(btnSettingA_deviceType, LV_BTN_STATE_REL, &lv_style_transp);
	lv_btn_set_style(btnSettingA_deviceType, LV_BTN_STATE_PR, &lv_style_transp);
	lv_obj_set_protect(btnSettingA_deviceType, LV_PROTECT_POS);
	lv_obj_align(btnSettingA_deviceType, textSettingA_deviceType, LV_ALIGN_OUT_BOTTOM_MID, screenLandscapeCoordinate_objOffset + 35, 15);
	lv_btn_set_action(btnSettingA_deviceType, LV_BTN_ACTION_CLICK, funCb_btnActionClick_btnDevType_funBack);
	btnLabelSettingA_deviceType = lv_label_create(btnSettingA_deviceType, NULL);
	lv_label_set_align(btnLabelSettingA_deviceType, LV_LABEL_ALIGN_CENTER);
	lv_label_set_long_mode(btnLabelSettingA_deviceType, LV_LABEL_LONG_ROLL);
	lv_obj_set_size(btnLabelSettingA_deviceType, 180, 30);
	lv_label_set_recolor(btnLabelSettingA_deviceType, true);
	lv_obj_set_style(btnLabelSettingA_deviceType, styleLabel_devTypeSelected);
	lv_obj_set_protect(btnLabelSettingA_deviceType, LV_PROTECT_POS);
	devType_Temp = currentDev_typeGet();
	for(loop = 0; loop < DEVICE_TYPE_LIST_NUM; loop ++){

		if(devType_Temp == deviceTypeTab_disp[loop].devType){

			lv_label_set_text(btnLabelSettingA_deviceType, deviceTypeStr_disp[loop].devName);
			devTypeDdist_istRecord = loop;
			break;
		}
	}
//	ddlistSettingA_deviceType = lv_ddlist_create(page_funSetOption, NULL);
//	lv_ddlist_set_style(ddlistSettingA_deviceType, LV_DDLIST_STYLE_SB, styleDdlistSettingA_devType);
//	lv_ddlist_set_style(ddlistSettingA_deviceType, LV_DDLIST_STYLE_BG, styleDdlistSettingA_devType);
//	lv_obj_set_protect(ddlistSettingA_deviceType, LV_PROTECT_POS);
//	lv_obj_align(ddlistSettingA_deviceType, textSettingA_deviceType, LV_ALIGN_OUT_BOTTOM_MID, screenLandscapeCoordinate_objOffset + 50, 25);
//	lv_ddlist_set_options(ddlistSettingA_deviceType, deviceType_listTab);
//	lv_ddlist_set_action(ddlistSettingA_deviceType, funCb_ddlistDevTypeDef);
//	lv_obj_set_top(ddlistSettingA_deviceType, true);
//	lv_obj_set_free_num(ddlistSettingA_deviceType, OBJ_DDLIST_DEVTYPE_FREENUM);
//	devType_Temp = currentDev_typeGet();
//	for(loop = 0; loop < DEVICE_TYPE_LIST_NUM; loop ++){

//		if(devType_Temp == deviceTypeTab_disp[loop].devType){

//			lv_ddlist_set_selected(ddlistSettingA_deviceType, deviceTypeTab_disp[loop].devInst_disp);
//			devTypeDdist_istRecord = deviceTypeTab_disp[loop].devInst_disp;
//			break;
//		}
//	}

	textSettingA_devStatusRecordIF = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_devStatusRecordIF, "Stay The Status:");
	lv_obj_set_style(textSettingA_devStatusRecordIF, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_devStatusRecordIF, LV_PROTECT_POS);
	lv_obj_align(textSettingA_devStatusRecordIF, textSettingA_deviceType, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	swSettingA_devStatusRecordIF = lv_sw_create(page_funSetOption, NULL);
	lv_sw_set_style(swSettingA_devStatusRecordIF, LV_SW_STYLE_BG, bg_styleDevStatusRecordIF);
	lv_sw_set_style(swSettingA_devStatusRecordIF, LV_SW_STYLE_INDIC, indic_styleDevStatusRecordIF);
	lv_sw_set_style(swSettingA_devStatusRecordIF, LV_SW_STYLE_KNOB_ON, knob_on_styleDevStatusRecordIF);
	lv_sw_set_style(swSettingA_devStatusRecordIF, LV_SW_STYLE_KNOB_OFF, knob_off_styleDevStatusRecordIF);
	lv_obj_set_protect(swSettingA_devStatusRecordIF, LV_PROTECT_POS);
	lv_obj_align(swSettingA_devStatusRecordIF, textSettingA_devStatusRecordIF, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 50, 20);
	lv_sw_set_anim_time(swSettingA_devStatusRecordIF, 100);
	if(devStatusRecordFlg_temp.devStatusOnOffRecord_IF)lv_sw_on(swSettingA_devStatusRecordIF);
	else lv_sw_off(swSettingA_devStatusRecordIF);
	lv_sw_set_action(swSettingA_devStatusRecordIF, funCb_swDevStatusRecordIF);
	lvObjAlign_temp = textSettingA_devStatusRecordIF;

	textSettingA_bGroundPicSelect = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_bGroundPicSelect, "Background Set:");
	lv_obj_set_style(textSettingA_bGroundPicSelect, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_bGroundPicSelect, LV_PROTECT_POS);
	lv_obj_align(textSettingA_bGroundPicSelect, lvObjAlign_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	pageBgroundPic_select = lv_page_create(page_funSetOption, NULL);
	lv_obj_set_size(pageBgroundPic_select, 200, 65);
	lv_obj_set_protect(pageBgroundPic_select, LV_PROTECT_POS);
	lv_obj_align(pageBgroundPic_select, textSettingA_bGroundPicSelect, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 10, 0);
	lv_page_set_scrl_fit(pageBgroundPic_select, true, false);
//	lv_page_set_scrl_width(pageBgroundPic_select, 720); 
	lv_page_set_scrl_height(pageBgroundPic_select, 45); 
	lv_page_set_sb_mode(pageBgroundPic_select, LV_SB_MODE_HIDE);
	lv_page_set_style(pageBgroundPic_select, LV_PAGE_STYLE_SB, stylePage_funSetOption);
	lv_page_set_style(pageBgroundPic_select, LV_PAGE_STYLE_BG, stylePage_funSetOption);
	for(loop = 0; loop < BROUND_PIC_SELECT_MAX_NUM; loop ++){

		btnBgroundPicSel_preview[loop] = lv_imgbtn_create(pageBgroundPic_select, NULL);
		lv_obj_set_size(btnBgroundPicSel_preview[loop], 40, 40);
		lv_imgbtn_set_toggle(btnBgroundPicSel_preview[loop], true);
//		lv_imgbtn_set_src(btnBgroundPicSel_preview[loop], LV_BTN_STATE_PR, usrAppHomepageBkPic_prevDataGet(loop));
//		lv_imgbtn_set_src(btnBgroundPicSel_preview[loop], LV_BTN_STATE_REL, usrAppHomepageBkPic_prevDataGet(loop));
		lv_imgbtn_set_src(btnBgroundPicSel_preview[loop], LV_BTN_STATE_TGL_REL, usrAppHomepageBkPic_prevDataGet(loop));
		lv_imgbtn_set_src(btnBgroundPicSel_preview[loop], LV_BTN_STATE_TGL_PR, usrAppHomepageBkPic_prevDataGet(loop));
		lv_imgbtn_set_action(btnBgroundPicSel_preview[loop], LV_BTN_ACTION_CLICK, funCb_btnActionClick_bGroundPicOrgSelcet);
		lv_obj_set_free_num(btnBgroundPicSel_preview[loop], loop);
		lv_imgbtn_set_style(btnBgroundPicSel_preview[loop], LV_IMGBTN_STYLE_TGL_PR, usrAppHomepageBkPicStyle_prevDataGet(loop));
		lv_imgbtn_set_style(btnBgroundPicSel_preview[loop], LV_IMGBTN_STYLE_TGL_REL, usrAppHomepageBkPicStyle_prevDataGet(loop));
		lv_obj_set_protect(btnBgroundPicSel_preview[loop], LV_PROTECT_POS);
		lv_obj_set_pos(btnBgroundPicSel_preview[loop], loop * 45 + 5, 3);
		lv_page_glue_obj(btnBgroundPicSel_preview[loop], true);
		(loop == usrAppHomepageBgroundPicOrg_Get())?
			(lv_imgbtn_set_state(btnBgroundPicSel_preview[loop], LV_BTN_STATE_TGL_PR)):
			(lv_imgbtn_set_state(btnBgroundPicSel_preview[loop], LV_BTN_STATE_TGL_REL));
	}
	imgBgroundPicSel_reserve = lv_img_create(btnBgroundPicSel_preview[usrAppHomepageBgroundPicOrg_Get()], NULL);
	lv_img_set_src(imgBgroundPicSel_reserve, &bGroundPrev_picFigure_sel);
	lv_page_focus(pageBgroundPic_select, btnBgroundPicSel_preview[usrAppHomepageBgroundPicOrg_Get()], 0);
	lvObjAlign_temp = textSettingA_bGroundPicSelect;

	textSettingA_devAtmosLightColorSet = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_devAtmosLightColorSet, "Illumination Set:");
	lv_obj_set_style(textSettingA_devAtmosLightColorSet, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_devAtmosLightColorSet, LV_PROTECT_POS);
	lv_obj_align(textSettingA_devAtmosLightColorSet, lvObjAlign_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	btnSettingA_devAtmosLightColorSet = lv_btn_create(page_funSetOption, NULL);
	styleBtn_devAtmosLightColorSet->body.main_color = atmosLightRunParam_dataTemp.lightColorCustomParamcfg;
	styleBtn_devAtmosLightColorSet->body.grad_color = atmosLightRunParam_dataTemp.lightColorCustomParamcfg;
	lv_btn_set_style(btnSettingA_devAtmosLightColorSet, LV_BTN_STYLE_REL, styleBtn_devAtmosLightColorSet);
	lv_btn_set_style(btnSettingA_devAtmosLightColorSet, LV_BTN_STYLE_PR, styleBtn_devAtmosLightColorSet);
	lv_btn_set_action(btnSettingA_devAtmosLightColorSet, LV_BTN_ACTION_CLICK, funCb_btnActionClick_devAtmosLightColorSet);
	lv_obj_set_size(btnSettingA_devAtmosLightColorSet, 80, 30);
	lv_obj_set_protect(btnSettingA_devAtmosLightColorSet, LV_PROTECT_POS);
	lv_obj_align(btnSettingA_devAtmosLightColorSet, textSettingA_devAtmosLightColorSet, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 50, 15);
	lvObjAlign_temp = textSettingA_devAtmosLightColorSet;

	textSettingA_devTimezoneSet = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_devTimezoneSet, "Time zone Set:");
	lv_obj_set_style(textSettingA_devTimezoneSet, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_devTimezoneSet, LV_PROTECT_POS);
	lv_obj_align(textSettingA_devTimezoneSet, lvObjAlign_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	objRoller_tzSet_hour = lv_roller_create(page_funSetOption, NULL);
	lv_roller_set_style(objRoller_tzSet_hour, LV_ROLLER_STYLE_BG, styleRoller_epidemicCyLocation_bg);
	lv_roller_set_style(objRoller_tzSet_hour, LV_ROLLER_STYLE_SEL, styleRoller_epidemicCyLocation_sel);
	lv_roller_set_action(objRoller_tzSet_hour, funCb_rollerOpreat_timeZoneUnitSetHour);
	lv_roller_set_options(objRoller_tzSet_hour, "00\n""01\n""02\n""03\n""04\n""05\n"
												"06\n""07\n""08\n""09\n""10\n""11\n"
												"12\n""13\n""14\n""15\n""16\n""17\n"
												"18\n""19\n""20\n""21\n""22\n""23");
	lv_roller_set_selected(objRoller_tzSet_hour, (tzData.timeZone_H > 23)?(tzData.timeZone_H - 24):(tzData.timeZone_H), false);
	lv_obj_set_protect(objRoller_tzSet_hour, LV_PROTECT_POS);
	lv_roller_set_hor_fit(objRoller_tzSet_hour, false);
	lv_obj_set_width(objRoller_tzSet_hour, 40);
	lv_obj_align(objRoller_tzSet_hour, textSettingA_devTimezoneSet, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 5, 2);
	lv_roller_set_visible_row_count(objRoller_tzSet_hour, 3);
	objRoller_tzSet_minute = lv_roller_create(page_funSetOption, NULL);
	lv_roller_set_style(objRoller_tzSet_minute, LV_ROLLER_STYLE_BG, styleRoller_epidemicCyLocation_bg);
	lv_roller_set_style(objRoller_tzSet_minute, LV_ROLLER_STYLE_SEL, styleRoller_epidemicCyLocation_sel);
	lv_roller_set_action(objRoller_tzSet_minute, funCb_rollerOpreat_timeZoneUnitSetMinute);
	lv_roller_set_options(objRoller_tzSet_minute, "00\n""01\n""02\n""03\n""04\n""05\n"
												  "06\n""07\n""08\n""09\n""10\n""11\n"
												  "12\n""13\n""14\n""15\n""16\n""17\n"
												  "18\n""19\n""20\n""21\n""22\n""23\n"
												  "24\n""25\n""26\n""27\n""28\n""29\n"
												  "30\n""31\n""32\n""33\n""34\n""35\n"
												  "36\n""37\n""38\n""39\n""40\n""41\n"
												  "42\n""43\n""44\n""45\n""46\n""47\n"
												  "48\n""49\n""50\n""51\n""52\n""53\n"
												  "54\n""55\n""56\n""57\n""58\n""59");
	lv_roller_set_selected(objRoller_tzSet_minute, tzData.timeZone_M, false);
	lv_obj_set_protect(objRoller_tzSet_minute, LV_PROTECT_POS);
	lv_roller_set_hor_fit(objRoller_tzSet_minute, false);
	lv_obj_set_width(objRoller_tzSet_minute, 40);
	lv_obj_align(objRoller_tzSet_minute, objRoller_tzSet_hour, LV_ALIGN_OUT_RIGHT_TOP, 25, 0);
	lv_roller_set_visible_row_count(objRoller_tzSet_minute, 3);
	objLabel_tzSet_hour = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(objLabel_tzSet_hour, "hour");
	lv_obj_set_protect(objLabel_tzSet_hour, LV_PROTECT_POS);
	lv_obj_align(objLabel_tzSet_hour, objRoller_tzSet_hour, LV_ALIGN_CENTER, 35, 2);
	lv_obj_set_style(objLabel_tzSet_hour, styleText_menuTzSet);
	objLabel_tzSet_minute = lv_label_create(page_funSetOption, objLabel_tzSet_hour);
	lv_label_set_text(objLabel_tzSet_minute, "minute");
	lv_obj_set_protect(objLabel_tzSet_minute, LV_PROTECT_POS);
	lv_obj_align(objLabel_tzSet_minute, objRoller_tzSet_minute, LV_ALIGN_CENTER, 35, -1);
	lv_obj_set_style(objLabel_tzSet_minute, styleText_menuTzSet);
	cbTzSet_minusIf = lv_cb_create(page_funSetOption, NULL);
	lv_obj_set_size(cbTzSet_minusIf, 160, 15);
	lv_cb_set_text(cbTzSet_minusIf, "minus");
	lv_obj_set_protect(cbTzSet_minusIf, LV_PROTECT_POS);
	lv_obj_set_protect(cbTzSet_minusIf, LV_PROTECT_FOLLOW);
	lv_obj_align(cbTzSet_minusIf, objLabel_tzSet_minute, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
	lv_cb_set_style(cbTzSet_minusIf, LV_CB_STYLE_BG, stylePageCb_tzSetMinusIf);
	lv_cb_set_action(cbTzSet_minusIf, funCb_cbOpreat_tzSet_minusIf);
	lv_cb_set_checked(cbTzSet_minusIf, (tzData.timeZone_H > 23)?(true):(false));
	lvObjAlign_temp = textSettingA_devTimezoneSet;

	// textSettingA_devListManagerEN = lv_label_create(page_funSetOption, NULL);
	// lv_label_set_text(textSettingA_devListManagerEN, "device manager:");
	// lv_obj_set_style(textSettingA_devListManagerEN, styleText_menuLevel_B);
	// lv_obj_set_protect(textSettingA_devListManagerEN, LV_PROTECT_POS);
	// lv_obj_align(textSettingA_devListManagerEN, lvObjAlign_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	// swSettingA_devListManagerEN = lv_sw_create(page_funSetOption, NULL);
	// lv_sw_set_style(swSettingA_devListManagerEN, LV_SW_STYLE_BG, bg_styleDevStatusRecordIF);
	// lv_sw_set_style(swSettingA_devListManagerEN, LV_SW_STYLE_INDIC, indic_styleDevStatusRecordIF);
	// lv_sw_set_style(swSettingA_devListManagerEN, LV_SW_STYLE_KNOB_ON, knob_on_styleDevStatusRecordIF);
	// lv_sw_set_style(swSettingA_devListManagerEN, LV_SW_STYLE_KNOB_OFF, knob_off_styleDevStatusRecordIF);
	// lv_obj_set_protect(swSettingA_devListManagerEN, LV_PROTECT_POS);
	// lv_obj_align(swSettingA_devListManagerEN, textSettingA_devListManagerEN, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 50, 20);
	// lv_sw_set_anim_time(swSettingA_devListManagerEN, 100);
	// if(devStatusRecordFlg_temp.devListManager_En)lv_sw_on(swSettingA_devListManagerEN);
	// else lv_sw_off(swSettingA_devListManagerEN);
	// lv_sw_set_action(swSettingA_devListManagerEN, funCb_swDevListManagerEN);
	// lvObjAlign_temp = textSettingA_devListManagerEN;

	// textSettingA_devHomeAssistantEN = lv_label_create(page_funSetOption, NULL);
	// lv_label_set_text(textSettingA_devHomeAssistantEN, "homeassistant support:");
	// lv_obj_set_style(textSettingA_devHomeAssistantEN, styleText_menuLevel_B);
	// lv_obj_set_protect(textSettingA_devHomeAssistantEN, LV_PROTECT_POS);
	// lv_obj_align(textSettingA_devHomeAssistantEN, lvObjAlign_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	// swSettingA_devHomeAssistantEN = lv_sw_create(page_funSetOption, NULL);
	// lv_sw_set_style(swSettingA_devHomeAssistantEN, LV_SW_STYLE_BG, bg_styleDevStatusRecordIF);
	// lv_sw_set_style(swSettingA_devHomeAssistantEN, LV_SW_STYLE_INDIC, indic_styleDevStatusRecordIF);
	// lv_sw_set_style(swSettingA_devHomeAssistantEN, LV_SW_STYLE_KNOB_ON, knob_on_styleDevStatusRecordIF);
	// lv_sw_set_style(swSettingA_devHomeAssistantEN, LV_SW_STYLE_KNOB_OFF, knob_off_styleDevStatusRecordIF);
	// lv_obj_set_protect(swSettingA_devHomeAssistantEN, LV_PROTECT_POS);
	// lv_obj_align(swSettingA_devHomeAssistantEN, textSettingA_devHomeAssistantEN, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 50, 20);
	// lv_sw_set_anim_time(swSettingA_devHomeAssistantEN, 100);
	// if(devStatusRecordFlg_temp.homeassitant_En)lv_sw_on(swSettingA_devHomeAssistantEN);
	// else lv_sw_off(swSettingA_devHomeAssistantEN);
	// lv_sw_set_action(swSettingA_devHomeAssistantEN, funCb_homeassistantEnable);
	// lvObjAlign_temp = textSettingA_devHomeAssistantEN;

	switch(currentDev_typeGet()){

		case devTypeDef_mulitSwOneBit:
		case devTypeDef_mulitSwTwoBit:
		case devTypeDef_mulitSwThreeBit:
		case devTypeDef_thermostatExtension:{

			textSettingA_homepageThemestyle = lv_label_create(page_funSetOption, NULL);
			lv_label_set_text(textSettingA_homepageThemestyle, "Theme:");
			lv_obj_set_style(textSettingA_homepageThemestyle, styleText_menuLevel_B);
			lv_obj_set_protect(textSettingA_homepageThemestyle, LV_PROTECT_POS);
			lv_obj_align(textSettingA_homepageThemestyle, lvObjAlign_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
			ddlistSettingA_homepageThemestyle = lv_ddlist_create(page_funSetOption, NULL);
			lv_ddlist_set_style(ddlistSettingA_homepageThemestyle, LV_DDLIST_STYLE_SB, styleDdlistSettingA_devType);
			lv_ddlist_set_style(ddlistSettingA_homepageThemestyle, LV_DDLIST_STYLE_BG, styleDdlistSettingA_devType);
			lv_obj_set_protect(ddlistSettingA_homepageThemestyle, LV_PROTECT_POS);
			lv_obj_align(ddlistSettingA_homepageThemestyle, textSettingA_homepageThemestyle, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 50, 20);
			lv_ddlist_set_options(ddlistSettingA_homepageThemestyle, homepageThemeStyle_listTab);
			lv_ddlist_set_action(ddlistSettingA_homepageThemestyle, funCb_ddlistHomepageThemeDef);
			lv_obj_set_top(ddlistSettingA_homepageThemestyle, true);
			lv_obj_set_free_num(ddlistSettingA_homepageThemestyle, OBJ_DDLIST_HPTHEME_FREENUM);
			homepageThemeStyle_temp = usrAppHomepageThemeType_Get();
			lv_ddlist_set_selected(ddlistSettingA_homepageThemestyle, homepageThemeStyle_temp);
			lvObjAlign_temp = textSettingA_homepageThemestyle;

		}break;

		default:{}break;
	}

	textSettingA_factoryRecoveryIf = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_factoryRecoveryIf, "Factory Reset:");
	lv_obj_set_style(textSettingA_factoryRecoveryIf, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_factoryRecoveryIf, LV_PROTECT_POS);
	switch(currentDev_typeGet()){

		case devTypeDef_mulitSwOneBit:
		case devTypeDef_mulitSwTwoBit:
		case devTypeDef_mulitSwThreeBit:
		case devTypeDef_thermostatExtension:{
			
			lv_obj_align(textSettingA_factoryRecoveryIf, lvObjAlign_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
			
		}break;

		default:{
		
			lv_obj_align(textSettingA_factoryRecoveryIf, lvObjAlign_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);

		}break;
	}
	btnSettingA_factoryRecoveryIf = lv_btn_create(page_funSetOption, NULL);
	lv_obj_set_size(btnSettingA_factoryRecoveryIf, 60, 20);
	lv_obj_set_protect(btnSettingA_factoryRecoveryIf, LV_PROTECT_POS);
	lv_btn_set_action(btnSettingA_factoryRecoveryIf, LV_BTN_ACTION_CLICK, funCb_btnActionClick_factoryRecovery);
	lv_obj_align(btnSettingA_factoryRecoveryIf, textSettingA_factoryRecoveryIf, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 60, 20);
    lv_btn_set_style(btnSettingA_factoryRecoveryIf, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
    lv_btn_set_style(btnSettingA_factoryRecoveryIf, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
    lv_btn_set_style(btnSettingA_factoryRecoveryIf, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
    lv_btn_set_style(btnSettingA_factoryRecoveryIf, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
	textBtnRef_factoryRecoveryIf = lv_label_create(btnSettingA_factoryRecoveryIf, NULL);
	lv_label_set_recolor(textBtnRef_factoryRecoveryIf, true);
	lv_label_set_text(textBtnRef_factoryRecoveryIf, "#00FF40 >>>o<<<#");
	lv_obj_set_style(textBtnRef_factoryRecoveryIf, styleText_menuLevel_B);
	lv_obj_set_protect(textBtnRef_factoryRecoveryIf, LV_PROTECT_POS);
	lv_obj_align(textBtnRef_factoryRecoveryIf, NULL, LV_ALIGN_CENTER, 0, 0);
	lvObjAlign_temp = textSettingA_factoryRecoveryIf;

//	textSettingA_touchRecalibrationIf = lv_label_create(page_funSetOption, NULL);
//	lv_label_set_text(textSettingA_touchRecalibrationIf, "touch recalibration:");
//	lv_obj_set_style(textSettingA_touchRecalibrationIf, styleText_menuLevel_B);
//	lv_obj_set_protect(textSettingA_touchRecalibrationIf, LV_PROTECT_POS);
//	lv_obj_align(textSettingA_touchRecalibrationIf, textSettingA_factoryRecoveryIf, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
//	btnSettingA_touchRecalibrationIf = lv_btn_create(page_funSetOption, NULL);
//	lv_obj_set_size(btnSettingA_touchRecalibrationIf, 60, 20);
//	lv_obj_set_protect(btnSettingA_touchRecalibrationIf, LV_PROTECT_POS);
//	lv_btn_set_action(btnSettingA_touchRecalibrationIf, LV_BTN_ACTION_CLICK, funCb_btnActionClick_touchRecalibration);
//	lv_obj_align(btnSettingA_touchRecalibrationIf, textSettingA_touchRecalibrationIf, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 60, 20);
//    lv_btn_set_style(btnSettingA_touchRecalibrationIf, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
//    lv_btn_set_style(btnSettingA_touchRecalibrationIf, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
//    lv_btn_set_style(btnSettingA_touchRecalibrationIf, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
//    lv_btn_set_style(btnSettingA_touchRecalibrationIf, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
//	textBtnRef_touchRecalibrationIf = lv_label_create(btnSettingA_touchRecalibrationIf, NULL);
//	lv_label_set_recolor(textBtnRef_touchRecalibrationIf, true);
//	lv_label_set_text(textBtnRef_touchRecalibrationIf, "#00FF40 >>>o<<<#");
//	lv_obj_set_style(textBtnRef_touchRecalibrationIf, styleText_menuLevel_B);
//	lv_obj_set_protect(textBtnRef_touchRecalibrationIf, LV_PROTECT_POS);
//	lv_obj_align(textBtnRef_touchRecalibrationIf, NULL, LV_ALIGN_CENTER, 0, 0);

	textSettingA_screenBrightness = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_screenBrightness, "Screen Brightness:");
	lv_obj_set_style(textSettingA_screenBrightness, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_screenBrightness, LV_PROTECT_POS);
	lv_obj_align(textSettingA_screenBrightness, lvObjAlign_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	sliderSettingA_screenBrightness = lv_slider_create(page_funSetOption, NULL);
	lv_obj_set_size(sliderSettingA_screenBrightness, 180, 15);
	lv_obj_set_protect(sliderSettingA_screenBrightness, LV_PROTECT_POS);
	lv_obj_align(sliderSettingA_screenBrightness, textSettingA_screenBrightness, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 10, 10);
	lv_obj_set_free_num(sliderSettingA_screenBrightness, 0);
	lv_slider_set_action(sliderSettingA_screenBrightness, funCb_slidAction_functionSet_screenBrightnessAdj);
	lv_slider_set_range(sliderSettingA_screenBrightness, 0, screenBrightness_sliderAdj_div);
	uint8_t brightnessSlider_valDisp = devScreenDriver_configParam_brightness_get();
	lv_slider_set_value(sliderSettingA_screenBrightness, brightnessSlider_valDisp / (DEVLEDC_SCREEN_BRIGHTNESS_LEVEL_DIV / screenBrightness_sliderAdj_div));
	sliderSettingA_screenBrightnessSleep = lv_slider_create(page_funSetOption, sliderSettingA_screenBrightness);
	lv_slider_set_action(sliderSettingA_screenBrightnessSleep, funCb_slidAction_functionSet_screenBrightnessAdj);
	lv_obj_set_free_num(sliderSettingA_screenBrightnessSleep, 1);
	lv_obj_align(sliderSettingA_screenBrightnessSleep, textSettingA_screenBrightness, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 10, 40);
	uint8_t brightnessSleepSlider_valDisp = devScreenDriver_configParam_brightnessSleep_get();
	lv_slider_set_value(sliderSettingA_screenBrightnessSleep, brightnessSleepSlider_valDisp / (DEVLEDC_SCREEN_BRIGHTNESS_LEVEL_DIV / screenBrightness_sliderAdj_div));
	lvObjAlign_temp = textSettingA_screenBrightness;

	textSettingA_tempCalibration = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_tempCalibration, "Temperature Calibration:");
	lv_obj_set_style(textSettingA_tempCalibration, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_tempCalibration, LV_PROTECT_POS);
	lv_obj_align(textSettingA_tempCalibration, lvObjAlign_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	textSliderRef_tempratureCal = lv_label_create(page_funSetOption, NULL);
	sprintf(textlabel_dispTemp, "#FFFF00 %.1f#", devTempratureSensor_dataCal_hanldeGet(devTempratureSensor_dataCal_get()));
	lv_label_set_recolor(textSliderRef_tempratureCal, true);
	lv_label_set_text(textSliderRef_tempratureCal, textlabel_dispTemp);
	lv_obj_set_style(textSliderRef_tempratureCal, styleText_menuLevel_B);
	lv_obj_set_protect(textSliderRef_tempratureCal, LV_PROTECT_POS);
	lv_obj_align(textSliderRef_tempratureCal, textSettingA_tempCalibration, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 85, 7);
	sliderSettingA_tempratureCal = lv_slider_create(page_funSetOption, NULL);
	lv_obj_set_size(sliderSettingA_tempratureCal, 180, 15);
	lv_obj_set_protect(sliderSettingA_tempratureCal, LV_PROTECT_POS);
	lv_obj_align(sliderSettingA_tempratureCal, textSettingA_tempCalibration, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 12, 30);
	lv_slider_set_action(sliderSettingA_tempratureCal, funCb_slidAction_functionSet_tempratureCal);
	lv_slider_set_range(sliderSettingA_tempratureCal, 0, DEV_TEMPRATURE_DATA_CAL_MAX_RANGE);
	lv_slider_set_value(sliderSettingA_tempratureCal, devTempratureSensor_dataCal_get());
	btnValAdd_tempratureCal = lv_btn_create(page_funSetOption, NULL);
	lv_obj_set_size(btnValAdd_tempratureCal, 40, 40);
	lv_obj_set_protect(btnValAdd_tempratureCal, LV_PROTECT_POS);
	lv_btn_set_action(btnValAdd_tempratureCal, LV_BTN_ACTION_CLICK, funCb_btnActionClick_functionSet_tempratureCalAdd);
	lv_obj_align(btnValAdd_tempratureCal, textSettingA_tempCalibration, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 155, -18);
    lv_btn_set_style(btnValAdd_tempratureCal, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
    lv_btn_set_style(btnValAdd_tempratureCal, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
    lv_btn_set_style(btnValAdd_tempratureCal, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
    lv_btn_set_style(btnValAdd_tempratureCal, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
	textBtnValAdd_tempratureCal = lv_label_create(btnValAdd_tempratureCal, NULL);
	lv_label_set_recolor(textBtnValAdd_tempratureCal, true);
	lv_label_set_text(textBtnValAdd_tempratureCal, "#00FF40 + #");
	lv_obj_set_style(textBtnValAdd_tempratureCal, styleText_menuLevel_A);
	lv_obj_set_protect(textBtnValAdd_tempratureCal, LV_PROTECT_POS);
	lv_obj_align(textBtnValAdd_tempratureCal, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 2);
	btnValCut_tempratureCal = lv_btn_create(page_funSetOption, NULL);
	lv_obj_set_size(btnValCut_tempratureCal, 40, 40);
	lv_obj_set_protect(btnValCut_tempratureCal, LV_PROTECT_POS);
	lv_btn_set_action(btnValCut_tempratureCal, LV_BTN_ACTION_CLICK, funCb_btnActionClick_functionSet_tempratureCalCut);
	lv_obj_align(btnValCut_tempratureCal, textSettingA_tempCalibration, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 10, -18);
    lv_btn_set_style(btnValCut_tempratureCal, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
    lv_btn_set_style(btnValCut_tempratureCal, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
    lv_btn_set_style(btnValCut_tempratureCal, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
    lv_btn_set_style(btnValCut_tempratureCal, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
	textBtnValCut_tempratureCal = lv_label_create(btnValCut_tempratureCal, NULL);
	lv_label_set_recolor(textBtnValCut_tempratureCal, true);
	lv_label_set_text(textBtnValCut_tempratureCal, "#00FF40 _ #");
	lv_obj_set_style(textBtnValCut_tempratureCal, styleText_menuLevel_A);
	lv_obj_set_protect(textBtnValCut_tempratureCal, LV_PROTECT_POS);
	lv_obj_align(textBtnValCut_tempratureCal, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -6);

	textSettingA_screenLightTime = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_screenLightTime, "Screen Light Time:");
	lv_obj_set_style(textSettingA_screenLightTime, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_screenLightTime, LV_PROTECT_POS);
	lv_obj_align(textSettingA_screenLightTime, textSettingA_tempCalibration, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	textBtnTimeRef_screenLightTime = lv_label_create(page_funSetOption, NULL);
	lv_label_set_recolor(textBtnTimeRef_screenLightTime, true);
	uint32_t screenLightTime_temp = devScreenDriver_configParam_screenLightTime_get();
	for(loop = 0; loop < FUNCTION_NUM_DEF_SCREENLIGHT_TIME; loop ++){

		if(screenLightTime_temp == screenLightTimeGear_refTab[loop].screenLightTime2Sec){

			functionGearScreenTime_ref = loop;
			break;
		}
	}
	lv_label_set_text(textBtnTimeRef_screenLightTime, screenLightTimeGear_refTab[functionGearScreenTime_ref].strDisp_ref);
	lv_obj_set_style(textBtnTimeRef_screenLightTime, styleText_menuLevel_B);
	lv_obj_set_protect(textBtnTimeRef_screenLightTime, LV_PROTECT_POS);
	lv_obj_align(textBtnTimeRef_screenLightTime, textSettingA_screenLightTime, LV_ALIGN_OUT_BOTTOM_MID, screenLandscapeCoordinate_objOffset + 35, 25);
	btnTimeAdd_screenLightTime = lv_btn_create(page_funSetOption, NULL);
	lv_obj_set_size(btnTimeAdd_screenLightTime, 40, 40);
	lv_obj_set_protect(btnTimeAdd_screenLightTime, LV_PROTECT_POS);
	lv_btn_set_action(btnTimeAdd_screenLightTime, LV_BTN_ACTION_CLICK, funCb_btnActionClick_functionSet_screenLightTimeAdd);
	lv_obj_align(btnTimeAdd_screenLightTime, textBtnTimeRef_screenLightTime, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, 0);
    lv_btn_set_style(btnTimeAdd_screenLightTime, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
    lv_btn_set_style(btnTimeAdd_screenLightTime, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
    lv_btn_set_style(btnTimeAdd_screenLightTime, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
    lv_btn_set_style(btnTimeAdd_screenLightTime, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
	textBtnTimeAdd_screenLightTime = lv_label_create(btnTimeAdd_screenLightTime, NULL);
	lv_label_set_recolor(textBtnTimeAdd_screenLightTime, true);
	lv_label_set_text(textBtnTimeAdd_screenLightTime, "#00FF40 >>>>#");
	lv_obj_set_style(textBtnTimeAdd_screenLightTime, styleText_menuLevel_B);
	lv_obj_set_protect(textBtnTimeAdd_screenLightTime, LV_PROTECT_POS);
	lv_obj_align(textBtnTimeAdd_screenLightTime, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 2);
	btnTimeCut_screenLightTime = lv_btn_create(page_funSetOption, NULL);
	lv_obj_set_size(btnTimeCut_screenLightTime, 40, 40);
	lv_obj_set_protect(btnTimeCut_screenLightTime, LV_PROTECT_POS);
	lv_btn_set_action(btnTimeCut_screenLightTime, LV_BTN_ACTION_CLICK, funCb_btnActionClick_functionSet_screenLightTimeCut);
	lv_obj_align(btnTimeCut_screenLightTime, textBtnTimeRef_screenLightTime, LV_ALIGN_OUT_LEFT_BOTTOM, -10, 0);
    lv_btn_set_style(btnTimeCut_screenLightTime, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
    lv_btn_set_style(btnTimeCut_screenLightTime, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
    lv_btn_set_style(btnTimeCut_screenLightTime, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
    lv_btn_set_style(btnTimeCut_screenLightTime, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
	textBtnTimeCut_screenLightTime = lv_label_create(btnTimeCut_screenLightTime, NULL);
	lv_label_set_recolor(textBtnTimeCut_screenLightTime, true);
	lv_label_set_text(textBtnTimeCut_screenLightTime, "#00FF40 <<<<#");
	lv_obj_set_style(textBtnTimeCut_screenLightTime, styleText_menuLevel_B);
	lv_obj_set_protect(textBtnTimeCut_screenLightTime, LV_PROTECT_POS);
	lv_obj_align(textBtnTimeCut_screenLightTime, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 2);

#if(0 == LVAPP_DISP_ELECPARAM_HIDDEN_EN)//热水器及恒温器电量功能暂隐
	textSettingA_elecParamDispSelect = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_elecParamDispSelect, "Electric Parameter Display:");
	lv_obj_set_style(textSettingA_elecParamDispSelect, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_elecParamDispSelect, LV_PROTECT_POS);
	lv_obj_align(textSettingA_elecParamDispSelect, textSettingA_screenLightTime, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	cbDispSelect_elecParam_power = lv_cb_create(page_funSetOption, NULL);
	lv_obj_set_size(cbDispSelect_elecParam_power, 160, 15);
	lv_cb_set_text(cbDispSelect_elecParam_power, " power");
	lv_obj_set_protect(cbDispSelect_elecParam_power, LV_PROTECT_POS);
	lv_obj_set_protect(cbDispSelect_elecParam_power, LV_PROTECT_FOLLOW);
	lv_obj_align(cbDispSelect_elecParam_power, textSettingA_elecParamDispSelect, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 10, 6);
	(devStatusRecordFlg_temp.devElecsumDisp_IF)?
		(lv_cb_set_checked(cbDispSelect_elecParam_power, false)):
		(lv_cb_set_checked(cbDispSelect_elecParam_power, true));
	cbDispSelect_elecParam_elecsum = lv_cb_create(page_funSetOption, NULL);
	lv_obj_set_size(cbDispSelect_elecParam_elecsum, 160, 15);
	lv_cb_set_text(cbDispSelect_elecParam_elecsum, " electric sum");
	lv_obj_set_protect(cbDispSelect_elecParam_elecsum, LV_PROTECT_POS);
	lv_obj_set_protect(cbDispSelect_elecParam_elecsum, LV_PROTECT_FOLLOW);
	lv_obj_align(cbDispSelect_elecParam_elecsum, textSettingA_elecParamDispSelect, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 10, 31);
	lv_cb_set_style(cbDispSelect_elecParam_power, LV_CB_STYLE_BG, stylePageCb_elecParamDispSet);
	lv_cb_set_style(cbDispSelect_elecParam_elecsum, LV_CB_STYLE_BG, stylePageCb_elecParamDispSet);
	lv_cb_set_action(cbDispSelect_elecParam_power, funCb_cbOpreat_elecParamDispSelect_power);
	lv_cb_set_action(cbDispSelect_elecParam_elecsum, funCb_cbOpreat_elecParamDispSelect_elecsum);
	(devStatusRecordFlg_temp.devElecsumDisp_IF)?
		(lv_cb_set_checked(cbDispSelect_elecParam_elecsum, true)):
		(lv_cb_set_checked(cbDispSelect_elecParam_elecsum, false));

	textSettingA_elecsumResetClear = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_elecsumResetClear, "Electric Sum Reset:");
	lv_obj_set_style(textSettingA_elecsumResetClear, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_elecsumResetClear, LV_PROTECT_POS);
	lv_obj_align(textSettingA_elecsumResetClear, textSettingA_elecParamDispSelect, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	btnSettingA_elecsumReset = lv_btn_create(page_funSetOption, NULL);
	lv_obj_set_size(btnSettingA_elecsumReset, 60, 20);
	lv_obj_set_protect(btnSettingA_elecsumReset, LV_PROTECT_POS);
	lv_btn_set_action(btnSettingA_elecsumReset, LV_BTN_ACTION_CLICK, funCb_btnActionClick_elecsumResetClear);
	lv_obj_align(btnSettingA_elecsumReset, textSettingA_elecsumResetClear, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 60, 20);
    lv_btn_set_style(btnSettingA_elecsumReset, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
    lv_btn_set_style(btnSettingA_elecsumReset, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
    lv_btn_set_style(btnSettingA_elecsumReset, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
    lv_btn_set_style(btnSettingA_elecsumReset, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
	textBtnRef_elecsumReset = lv_label_create(btnSettingA_elecsumReset, NULL);
	lv_label_set_recolor(textBtnRef_elecsumReset, true);
	lv_label_set_text(textBtnRef_elecsumReset, "#00FF40 >>>o<<<#");
	lv_obj_set_style(textBtnRef_elecsumReset, styleText_menuLevel_B);
	lv_obj_set_protect(textBtnRef_elecsumReset, LV_PROTECT_POS);
	lv_obj_align(textBtnRef_elecsumReset, NULL, LV_ALIGN_CENTER, 0, 0);

	textSettingA_screenLandscape = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_screenLandscape, "Screen Display Angle:");
	lv_obj_set_style(textSettingA_screenLandscape, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_screenLandscape, LV_PROTECT_POS);
	lv_obj_align(textSettingA_screenLandscape, textSettingA_elecsumResetClear, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	cbDispSelect_screenVer = lv_cb_create(page_funSetOption, NULL);
	lv_obj_set_size(cbDispSelect_screenVer, 160, 15);
	lv_cb_set_text(cbDispSelect_screenVer, " vertical");
	lv_obj_set_protect(cbDispSelect_screenVer, LV_PROTECT_POS);
	lv_obj_set_protect(cbDispSelect_screenVer, LV_PROTECT_FOLLOW);
	lv_obj_align(cbDispSelect_screenVer, textSettingA_screenLandscape, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 10, 6);
	(devStatusRecordFlg_temp.devScreenLandscape_IF)?
		(lv_cb_set_checked(cbDispSelect_screenVer, false)):
		(lv_cb_set_checked(cbDispSelect_screenVer, true));
	cbDispSelect_screenHor = lv_cb_create(page_funSetOption, NULL);
	lv_obj_set_size(cbDispSelect_screenHor, 160, 15);
	lv_cb_set_text(cbDispSelect_screenHor, " horizontal");
	lv_obj_set_protect(cbDispSelect_screenHor, LV_PROTECT_POS);
	lv_obj_set_protect(cbDispSelect_screenHor, LV_PROTECT_FOLLOW);
	lv_obj_align(cbDispSelect_screenHor, textSettingA_screenLandscape, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 10, 31);
	(devStatusRecordFlg_temp.devScreenLandscape_IF)?
		(lv_cb_set_checked(cbDispSelect_screenHor, true)):
		(lv_cb_set_checked(cbDispSelect_screenHor, false));
	lv_cb_set_style(cbDispSelect_screenVer, LV_CB_STYLE_BG, stylePageCb_screenLandscape);
	lv_cb_set_style(cbDispSelect_screenHor, LV_CB_STYLE_BG, stylePageCb_screenLandscape);
	lv_cb_set_action(cbDispSelect_screenVer, funCb_cbOpreat_scrrenDispMethod_ver);
	lv_cb_set_action(cbDispSelect_screenHor, funCb_cbOpreat_scrrenDispMethod_hor);
	opreatTemp_screenLandscapeIf = devStatusRecordFlg_temp.devScreenLandscape_IF;
#else

	textSettingA_screenLandscape = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_screenLandscape, "screen display angle:");
	lv_obj_set_style(textSettingA_screenLandscape, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_screenLandscape, LV_PROTECT_POS);
	lv_obj_align(textSettingA_screenLandscape, textSettingA_screenLightTime, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	cbDispSelect_screenVer = lv_cb_create(page_funSetOption, NULL);
	lv_obj_set_size(cbDispSelect_screenVer, 160, 15);
	lv_cb_set_text(cbDispSelect_screenVer, " vertical");
	lv_obj_set_protect(cbDispSelect_screenVer, LV_PROTECT_POS);
	lv_obj_set_protect(cbDispSelect_screenVer, LV_PROTECT_FOLLOW);
	lv_obj_align(cbDispSelect_screenVer, textSettingA_screenLandscape, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 10, 6);
	(devStatusRecordFlg_temp.devScreenLandscape_IF)?
		(lv_cb_set_checked(cbDispSelect_screenVer, false)):
		(lv_cb_set_checked(cbDispSelect_screenVer, true));
	cbDispSelect_screenHor = lv_cb_create(page_funSetOption, NULL);
	lv_obj_set_size(cbDispSelect_screenHor, 160, 15);
	lv_cb_set_text(cbDispSelect_screenHor, " horizontal");
	lv_obj_set_protect(cbDispSelect_screenHor, LV_PROTECT_POS);
	lv_obj_set_protect(cbDispSelect_screenHor, LV_PROTECT_FOLLOW);
	lv_obj_align(cbDispSelect_screenHor, textSettingA_screenLandscape, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 10, 31);
	(devStatusRecordFlg_temp.devScreenLandscape_IF)?
		(lv_cb_set_checked(cbDispSelect_screenHor, true)):
		(lv_cb_set_checked(cbDispSelect_screenHor, false));
	lv_cb_set_style(cbDispSelect_screenVer, LV_CB_STYLE_BG, stylePageCb_screenLandscape);
	lv_cb_set_style(cbDispSelect_screenHor, LV_CB_STYLE_BG, stylePageCb_screenLandscape);
	lv_cb_set_action(cbDispSelect_screenVer, funCb_cbOpreat_scrrenDispMethod_ver);
	lv_cb_set_action(cbDispSelect_screenHor, funCb_cbOpreat_scrrenDispMethod_hor);
	opreatTemp_screenLandscapeIf = devStatusRecordFlg_temp.devScreenLandscape_IF;
#endif

	if(upgradeAvailable_flg){

		textSettingA_fwareUpgrade = lv_label_create(page_funSetOption, NULL);
		lv_label_set_text(textSettingA_fwareUpgrade, "Firmware Upgrade:");
		lv_obj_set_style(textSettingA_fwareUpgrade, styleText_menuLevel_B);
		lv_obj_set_protect(textSettingA_fwareUpgrade, LV_PROTECT_POS);
		lv_obj_align(textSettingA_fwareUpgrade, textSettingA_screenLandscape, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
		btnSettingA_fwareUpgrade = lv_btn_create(page_funSetOption, NULL);
		lv_obj_set_size(btnSettingA_fwareUpgrade, 60, 20);
		lv_obj_set_protect(btnSettingA_fwareUpgrade, LV_PROTECT_POS);
		lv_btn_set_action(btnSettingA_fwareUpgrade, LV_BTN_ACTION_CLICK, funCb_btnActionClick_fwareUpgrade);
		lv_obj_align(btnSettingA_fwareUpgrade, textSettingA_fwareUpgrade, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 60, 20);
	    lv_btn_set_style(btnSettingA_fwareUpgrade, LV_BTN_STYLE_REL, styleBtn_specialTransparent);
	    lv_btn_set_style(btnSettingA_fwareUpgrade, LV_BTN_STYLE_PR, styleBtn_specialTransparent);
	    lv_btn_set_style(btnSettingA_fwareUpgrade, LV_BTN_STYLE_TGL_REL, styleBtn_specialTransparent);
	    lv_btn_set_style(btnSettingA_fwareUpgrade, LV_BTN_STYLE_TGL_PR, styleBtn_specialTransparent);
		textBtnRef_fwareUpgrade = lv_label_create(btnSettingA_fwareUpgrade, NULL);
		lv_label_set_recolor(textBtnRef_fwareUpgrade, true);
		lv_label_set_text(textBtnRef_fwareUpgrade, "#00FF40 >>>o<<<#");
		lv_obj_set_style(textBtnRef_fwareUpgrade, styleText_menuLevel_B);
		lv_obj_set_protect(textBtnRef_fwareUpgrade, LV_PROTECT_POS);
		lv_obj_align(textBtnRef_fwareUpgrade, NULL, LV_ALIGN_CENTER, 0, 0);
	}

#if(SCREENSAVER_RUNNING_ENABLE == 1)

	textSettingA_epidemicReport = lv_label_create(page_funSetOption, NULL);
	lv_label_set_text(textSettingA_epidemicReport, "epidemic report:");
	lv_obj_set_style(textSettingA_epidemicReport, styleText_menuLevel_B);
	lv_obj_set_protect(textSettingA_epidemicReport, LV_PROTECT_POS);
	lv_obj_align(textSettingA_epidemicReport, (upgradeAvailable_flg)?textSettingA_fwareUpgrade:textSettingA_screenLandscape, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 60);
	swSettingA_epidemicReportIF = lv_sw_create(page_funSetOption, NULL);
	lv_sw_set_style(swSettingA_epidemicReportIF, LV_SW_STYLE_BG, bg_styleDevStatusRecordIF);
	lv_sw_set_style(swSettingA_epidemicReportIF, LV_SW_STYLE_INDIC, indic_styleDevStatusRecordIF);
	lv_sw_set_style(swSettingA_epidemicReportIF, LV_SW_STYLE_KNOB_ON, knob_on_styleDevStatusRecordIF);
	lv_sw_set_style(swSettingA_epidemicReportIF, LV_SW_STYLE_KNOB_OFF, knob_off_styleDevStatusRecordIF);
	lv_obj_set_protect(swSettingA_epidemicReportIF, LV_PROTECT_POS);
	lv_obj_align(swSettingA_epidemicReportIF, textSettingA_epidemicReport, LV_ALIGN_OUT_BOTTOM_LEFT, screenLandscapeCoordinate_objOffset + 50, 20);
	lv_sw_set_anim_time(swSettingA_epidemicReportIF, 100);
	if(devStatusRecordFlg_temp.screensaverRunning_IF)lv_sw_on(swSettingA_epidemicReportIF);
	else lv_sw_off(swSettingA_epidemicReportIF);
	lv_sw_set_action(swSettingA_epidemicReportIF, funCb_epidemicReportIF);
	
	lvGui_settingSet_epidemicCtrlObj_opt(devStatusRecordFlg_temp.screensaverRunning_IF);
#endif

	lv_page_glue_obj(textSettingA_deviceType, true);
	lv_page_glue_obj(btnSettingA_deviceType, true);
//	lv_page_glue_obj(ddlistSettingA_deviceType, true);
	lv_page_glue_obj(textSettingA_devAtmosLightColorSet, true);
	lv_page_glue_obj(btnSettingA_devAtmosLightColorSet, true);
	switch(currentDev_typeGet()){
		case devTypeDef_mulitSwOneBit:
		case devTypeDef_mulitSwTwoBit:
		case devTypeDef_mulitSwThreeBit:
		case devTypeDef_thermostatExtension:{
			lv_page_glue_obj(textSettingA_homepageThemestyle, true);
			lv_page_glue_obj(ddlistSettingA_homepageThemestyle, true);
		}break;
		default:{}break;
	}
	lv_page_glue_obj(textSettingA_devTimezoneSet, true);
	lv_page_glue_obj(objRoller_tzSet_hour, true);
	lv_page_glue_obj(objRoller_tzSet_minute, true);
	lv_page_glue_obj(objLabel_tzSet_hour, true);
	lv_page_glue_obj(objLabel_tzSet_minute, true);
	lv_page_glue_obj(cbTzSet_minusIf, true);
	lv_page_glue_obj(textSettingA_devStatusRecordIF, true);
	lv_page_glue_obj(textSettingA_bGroundPicSelect, true);
	lv_page_glue_obj(swSettingA_devStatusRecordIF, true);
	lv_page_glue_obj(textSettingA_factoryRecoveryIf, true);
	lv_page_glue_obj(btnSettingA_factoryRecoveryIf, true);
// lv_page_glue_obj(textSettingA_devListManagerEN, true);
// lv_page_glue_obj(swSettingA_devListManagerEN, true);
// if(swSettingA_devHomeAssistantEN)lv_page_glue_obj(swSettingA_devHomeAssistantEN, true);
// if(textSettingA_devHomeAssistantEN)lv_page_glue_obj(textSettingA_devHomeAssistantEN, true);
//	lv_page_glue_obj(textSettingA_touchRecalibrationIf, true);
//	lv_page_glue_obj(btnSettingA_touchRecalibrationIf, true);
//	lv_page_glue_obj(sliderSettingA_screenBrightness, true);
//	lv_page_glue_obj(sliderSettingA_screenBrightnessSleep, true);
	lv_page_glue_obj(textBtnTimeRef_screenLightTime, true);
	lv_page_glue_obj(textSettingA_tempCalibration, true);
	lv_page_glue_obj(btnTimeAdd_screenLightTime, true);
	lv_page_glue_obj(textBtnTimeAdd_screenLightTime, true);
	lv_page_glue_obj(btnTimeCut_screenLightTime, true);
	lv_page_glue_obj(textBtnTimeCut_screenLightTime, true);
	lv_page_glue_obj(textSliderRef_tempratureCal, true);
#if(0 == LVAPP_DISP_ELECPARAM_HIDDEN_EN)//热水器及恒温器电量功能暂隐
	lv_page_glue_obj(textSettingA_elecParamDispSelect, true);
	lv_page_glue_obj(textSettingA_elecsumResetClear, true);
#endif
	lv_page_glue_obj(textSettingA_screenLandscape, true);

	lv_obj_animate(textSettingA_deviceType, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic, NULL);
	lv_obj_animate(btnSettingA_deviceType, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
//	lv_obj_animate(ddlistSettingA_deviceType, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);

	lv_obj_animate(textSettingA_devStatusRecordIF, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(swSettingA_devStatusRecordIF, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);

	lv_obj_animate(textSettingA_devTimezoneSet, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(objRoller_tzSet_hour, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(objRoller_tzSet_minute, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic, NULL);
	lv_obj_animate(objLabel_tzSet_hour, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic, NULL);
	lv_obj_animate(objLabel_tzSet_minute, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic, NULL);
	lv_obj_animate(cbTzSet_minusIf, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic, NULL);

	// lv_obj_animate(textSettingA_devListManagerEN, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	// lv_obj_animate(swSettingA_devListManagerEN, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);

	// if(textSettingA_devHomeAssistantEN){

	// 	lv_obj_animate(textSettingA_devHomeAssistantEN, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	// 	lv_obj_animate(swSettingA_devHomeAssistantEN, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	// }

	lv_obj_animate(textSettingA_bGroundPicSelect, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(pageBgroundPic_select, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);

	lv_obj_animate(textSettingA_devAtmosLightColorSet, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(btnSettingA_devAtmosLightColorSet, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);

	switch(currentDev_typeGet()){
		case devTypeDef_mulitSwOneBit:
		case devTypeDef_mulitSwTwoBit:
		case devTypeDef_mulitSwThreeBit:
		case devTypeDef_thermostatExtension:{
			lv_obj_animate(textSettingA_homepageThemestyle, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
			lv_obj_animate(ddlistSettingA_homepageThemestyle, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
		}break;
		default:{}break;
	}
	
	lv_obj_animate(textSettingA_factoryRecoveryIf, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(btnSettingA_factoryRecoveryIf, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);

//	lv_obj_animate(textSettingA_touchRecalibrationIf, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
//	lv_obj_animate(btnSettingA_touchRecalibrationIf, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	
	lv_obj_animate(textSettingA_screenBrightness, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(sliderSettingA_screenBrightness, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(sliderSettingA_screenBrightnessSleep, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	
	lv_obj_animate(textSettingA_screenLightTime, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay,  NULL);
	lv_obj_animate(textBtnTimeRef_screenLightTime, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(btnTimeAdd_screenLightTime, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(btnTimeCut_screenLightTime, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);

	lv_obj_animate(textSettingA_tempCalibration, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay,  NULL);
	lv_obj_animate(textSliderRef_tempratureCal, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay,  NULL);
	lv_obj_animate(btnValAdd_tempratureCal, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(btnValCut_tempratureCal, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(sliderSettingA_tempratureCal, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);	

#if(0 == LVAPP_DISP_ELECPARAM_HIDDEN_EN)//热水器及恒温器电量功能暂隐
	lv_obj_animate(textSettingA_elecParamDispSelect, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay,  NULL);
	lv_obj_animate(cbDispSelect_elecParam_power, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(cbDispSelect_elecParam_elecsum, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay,  NULL);
	
	lv_obj_animate(textSettingA_elecsumResetClear, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(btnSettingA_elecsumReset, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
#endif

	lv_obj_animate(textSettingA_screenLandscape, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay,  NULL);
	lv_obj_animate(cbDispSelect_screenVer, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	lv_obj_animate(cbDispSelect_screenHor, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay,  NULL);

	if(upgradeAvailable_flg){

		lv_obj_animate(textSettingA_fwareUpgrade, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
		lv_obj_animate(btnSettingA_fwareUpgrade, LV_ANIM_FLOAT_LEFT, obj_animate_time, obj_animate_delayBasic += obj_animate_delay, NULL);
	}

	vTaskDelay(20 / portTICK_RATE_MS);	
	lv_obj_refresh_style(page_funSetOption);
	lv_obj_refresh_style(obj_Parent);

//	vTaskDelay((obj_animate_delay * 10) / portTICK_RATE_MS);
//	lv_page_focus(page_funSetOption, btnTimeCut_screenLightTime, obj_animate_time * 5);
//	vTaskDelay((obj_animate_time * 15) / portTICK_RATE_MS);
//	lv_page_focus(page_funSetOption, textSettingA_deviceType, obj_animate_time * 3);

	lv_obj_set_click(menuBtnChoIcon_fun_back, true);
	lv_obj_set_click(menuBtnChoIcon_fun_home, true);

	usrApp_fullScreenRefresh_self(50, 0);
}


