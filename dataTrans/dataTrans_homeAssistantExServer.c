#include "dataTrans_homeAssistantExServer.h"

#include "os.h"

#include "mlink.h"
#include "mwifi.h"

#include "mdf_common.h"
#include "mqtt_client.h"

#include "dataTrans_remoteServer.h"

#include "bussiness_timerSoft.h"
#include "bussiness_timerHard.h"

#include "dataTrans_localHandler.h"
#include "dataTrans_meshUpgrade.h"

#include "devDriver_manage.h"

uint16_t dtCounter_haDataPreventSurge = 0;

static const char *TAG = "lanbon_L8 - haMqttRemote";

static esp_mqtt_client_handle_t exHomeassistantClient = NULL;

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event);

static esp_mqtt_client_config_t mqtt_cfg = {

	.transport = MQTT_TRANSPORT_OVER_TCP,
	.event_handle = mqtt_event_handler,
	.username = "lanbon",
	.password = "lanbon2019.",

    .task_prio 	  = (CONFIG_MDF_TASK_DEFAULT_PRIOTY + 1),                
    .task_stack	  = (1024 * 6),                        
    .buffer_size  = (512 * 2),                     
};	

static bool remoteMqtt_connectFlg = false;

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event){

    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
	uint8_t devRouterBssid[6] = {0};
	uint8_t devSelfMac[MWIFI_ADDR_LEN] = {0};

  // your_context_t *context = event->context;
    switch (event->event_id) {

        case MQTT_EVENT_CONNECTED:

			devRouterConnectBssid_Get(devRouterBssid);
			esp_wifi_get_mac(ESP_IF_WIFI_STA, devSelfMac);

            remoteMqtt_connectFlg = true;

            homeassistantApp_devOnlineSynchronous(client, devSelfMac);

            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            break;

        case MQTT_EVENT_DISCONNECTED:

            remoteMqtt_connectFlg = false;
		
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:

            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_UNSUBSCRIBED:

            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED:

            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_DATA:

            if(dtCounter_haDataPreventSurge)dtCounter_haDataPreventSurge = USRDEF_MQTT_DTSURGE_PREVENT_KPTIME;
            else{

                if(false == bussinessHA_mqttDataHandle(event)){ //ha data process


                }
            }
		
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            break;

        case MQTT_EVENT_ERROR:
		
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;

		case MQTT_EVENT_BEFORE_CONNECT:
            break;
    }

    return ESP_OK;
}

void mqtt_exHomeassistant_start(void){

    esp_err_t ret = ESP_OK;

	stt_mqttExServerCfgParam exHaMqttCfg_temp = {0};

    mqttHaMqttServer_paramGet(&exHaMqttCfg_temp);

	mqtt_cfg.host = (const char *)exHaMqttCfg_temp.hostConnServer.host_domain;
	mqtt_cfg.port = (uint32_t)exHaMqttCfg_temp.hostConnServer.port_remote;
    mqtt_cfg.username = (const char *)exHaMqttCfg_temp.usrName;
    mqtt_cfg.password = (const char *)exHaMqttCfg_temp.usrPsd;

	if(NULL == exHomeassistantClient){ //第一次连接，创建

		exHomeassistantClient = esp_mqtt_client_init((const esp_mqtt_client_config_t *)&mqtt_cfg);
		if(NULL == exHomeassistantClient)ESP_LOGW(TAG, "ha mqtt_client init(first) fail.\n");
	}
	else{ //之前已经连过，重连
	
		ret = esp_mqtt_set_config(exHomeassistantClient, (const esp_mqtt_client_config_t *)&mqtt_cfg);
		if(ret != ESP_OK)ESP_LOGW(TAG, "ha mqtt reconnect fail, code:%08X.\n", ret);
	}

    ret = esp_mqtt_client_start(exHomeassistantClient);
    if(ret != ESP_OK)ESP_LOGW(TAG, "ha mqtt start fail, code:%08X.\n", ret);
}

void mqtt_exHomeassistant_serverSwitch(stt_mqttExServerCfgParam *serverCgf_param){

	static bool flg_serverChanging = false;

	esp_err_t ret = ESP_OK;
	stt_mqttExServerCfgParam serverCgf_temp = {0};

	if(true == flg_serverChanging)return; //正在切换，禁止重入
	else{

		flg_serverChanging = true;
	}

	remoteMqtt_connectFlg = false;

	ret = esp_mqtt_client_stop(exHomeassistantClient);
	if(ret != ESP_OK)ESP_LOGW(TAG, "ha mqtt stop fail, code:%08X.\n", ret);
	vTaskDelay(3000 / portTICK_RATE_MS);

	if(serverCgf_param == NULL){ //形参为空则使用默认服务器

        mqttHaMqttServer_paramGet(&serverCgf_temp);
	}
	else{

		memcpy(&serverCgf_temp, serverCgf_param, sizeof(stt_mqttExServerCfgParam));
	}

	mqtt_cfg.host = (const char *)serverCgf_temp.hostConnServer.host_domain;
	mqtt_cfg.port = (uint32_t)serverCgf_temp.hostConnServer.port_remote;
    mqtt_cfg.username = (const char *)serverCgf_temp.usrName;
    mqtt_cfg.password = (const char *)serverCgf_temp.usrPsd;

	ret = esp_mqtt_set_config(exHomeassistantClient, &mqtt_cfg);
	if(ret != ESP_OK)ESP_LOGW(TAG, "ha mqtt config fail, code:%08X.\n", ret);
	ret = esp_mqtt_client_start(exHomeassistantClient);
	if(ret != ESP_OK)ESP_LOGW(TAG, "ha mqtt reconnect fail, code:%08X.\n", ret);

	flg_serverChanging = false;

	printf("ha mqtt server cfg chg to IP[%s]-port:%d.\n", serverCgf_temp.hostConnServer.host_domain, serverCgf_temp.hostConnServer.port_remote);
}

void mqtt_exHomeassistant_stop(void){

	esp_mqtt_client_stop(exHomeassistantClient);

	remoteMqtt_connectFlg = false;
}