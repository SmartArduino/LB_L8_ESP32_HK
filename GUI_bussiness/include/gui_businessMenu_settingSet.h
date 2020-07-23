/**
 * @file gui_businessMenu_settingSet.h
 *
 */

#ifndef GUI_BUSINESSMENU_SETTINGSET_H
#define GUI_BUSINESSMENU_SETTINGSET_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "iot_lvgl.h"

/*********************
 *      DEFINES
 *********************/
#define LV_FNUM_SLIDER_ALCS_R			0
#define LV_FNUM_SLIDER_ALCS_G			1
#define LV_FNUM_SLIDER_ALCS_B		    2

#define LV_FNUM_CB_ALCS_AUTOMATIC		0
#define LV_FNUM_CB_ALCS_BRE_IF			1
#define LV_FNUM_CB_ALCS_SETALL_IF		2

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void appUiElementSet_upgradeAvailable(bool val);

void lvGui_businessMenu_settingSet(lv_obj_t * obj_Parent);
void guiDispTimeOut_pageSettingSet(void);

void paramSettingGet_epidCyName(char *nameStr);

/**
 * Create a test screen with a lot objects and apply the given theme on them
 * @param th pointer to a theme
 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*GUI_BUSINESSMENU_SETTINGSET_H*/



