/**
 * @file dataTrans_remoteServer.h
 *
 */

#ifndef DATATRANS_REMOTESERVER_H
#define DATATRANS_REMOTESERVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "esp_types.h"

#include "mqtt_client.h"

#include "devDriver_manage.h"

/*********************
 *      DEFINES
 *********************/
#define USRAPP_MQTT_REMOTEREQ_DATAPACK_PER_LENGTH		896	//MQTT数据传输单包长度限制

#define USRDEF_MQTT_DTSURGE_PREVENT_KPTIME				1500 //MQTT连接后残留信息消除等待时间

#define USRDEF_MQTT_DEVCONNECT_NOTICE_LOOP_MAX			10	//mqtt主机上线时，mac更换notice提示

#define USRDEF_MQTT_SERVERSEL_LIST_LEN					2	//服务器可选表长度

#define DATATRAN_MQTT_ROOTOP_RESPCODE_ROUTER_PARAM_CHG  0x0A
#define DATATRAN_MQTT_ROOTOP_RESPCODE_SERVER_PARAM_CHG  0x0B
#define DATATRAN_MQTT_ROOTOP_RESPCODE_MQTT_USRINFO_CHG  0x0C
#define DATATRAN_MQTT_ROOTOP_RESPCODE_MQTT_HASERVER_CHG 0x0D

#define USR_MQTT_RUN_AS_LANBON							0
#define USR_MQTT_RUN_AS_HOMEASSISTANT					1
#define USR_MQTT_APPLICATION_CFG_DEF					USR_MQTT_RUN_AS_HOMEASSISTANT

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void mqtt_app_start(void);
void mqtt_app_stop(void);
void mqtt_app_destory(void);
void mqtt_rootDevRemoteDatatransLoop_elecSumReport(void);
void dataTransBussiness_allNodeDetailInfoReport(void);
void mqtt_rootDevLoginConnectNotice_trig(void);
void mqtt_remoteDataTrans(uint8_t dtCmd, uint8_t *data, uint16_t dataLen);
void mqtt_app_serverSwitch(stt_mqttCfgParam *serverCgf_param);
void usrApp_devRootStatusSynchronousInitiative(void);
void usrApp_deviceStatusSynchronousInitiative(void);
void devFireware_upgradeReserveCheck(void);
void devFireware_upgradeReserveCheck_trigByEvent(void);

void dtRmoteServer_serverSwitchByDefault(void);
void dtRmoteServer_serverSwitchByDefault_trig(bool haServer_IF);

void homeassistantApp_devStateSetSynchronous(esp_mqtt_client_handle_t client, uint8_t devAddr[MWIFI_ADDR_LEN], stt_devDataPonitTypedef status);
void homeassistantApp_devOnlineSynchronous(esp_mqtt_client_handle_t client, uint8_t devAddr[MWIFI_ADDR_LEN]);

bool bussinessHA_mqttDataHandle(esp_mqtt_event_handle_t event);

#if(SCREENSAVER_RUNNING_ENABLE == 1)

 void mqtt_rootDevEpidemicDataReq_trig(void);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DATATRANS_REMOTESERVER_H*/



