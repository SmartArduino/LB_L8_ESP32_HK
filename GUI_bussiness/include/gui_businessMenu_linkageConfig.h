/**
 * @file gui_businessMenu_linkageConfig.h
 *
 */

#ifndef GUI_BUSINESSMENU_LINKAGECONFIG_H
#define GUI_BUSINESSMENU_LINKAGECONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "devDataManage.h"

#include "mlink.h"
#include "mwifi.h"
#include "mdf_common.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a test screen with a lot objects and apply the given theme on them
 * @param th pointer to a theme
 */
void lvGui_businessMenu_linkageConfig(lv_obj_t * obj_Parent);

void guiDispTimeOut_pageLinkageCfg(void);

void dataTransBussiness_pageLinkageCfg_superCtrlActivityFunction(void);
void dataTransBussiness_pageLinkageCfg_superSycnActivityFunction(void);

void lvGuiLinkageConfig_devGraphCtrlBlock_listNodeUnitRefresh(uint8_t devMac[MWIFI_ADDR_LEN], uint8_t devState);
void lvGuiLinkageConfig_devGraphCtrlBlock_listLoadInitialization(stt_nodeObj_listManageDevCtrlBase *devNodeListHead, lv_obj_t *obj_Parent);
void lvGuiLinkageConfig_devGraphCtrlBlock_listLoadPrePage(lv_obj_t * obj_Parent);
void lvGuiLinkageConfig_devGraphCtrlBlock_listLoadFlg_set(bool flg);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*GUI_BUSINESSMENU_LINKAGECONFIG_H*/


