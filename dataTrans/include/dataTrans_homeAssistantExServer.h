/**
 * @file dataTrans_homeAssistantExServer.h
 *
 */

#ifndef DATATRANS_HOMEASSISTANTEXSERVER_H
#define DATATRANS_HOMEASSISTANTEXSERVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "esp_types.h"

#include "devDriver_manage.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void mqtt_exHomeassistant_start(void);
void mqtt_exHomeassistant_serverSwitch(stt_mqttExServerCfgParam *serverCgf_param);
void mqtt_exHomeassistant_stop(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DATATRANS_HOMEASSISTANTEXSERVER_H*/



