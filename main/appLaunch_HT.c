#include "appLaunch_HT.h"

#include <stdio.h>
#include <string.h>

#include <hap_platform_wifi_extras.h>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include <hap_fw_upgrade.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include <iot_button.h>

#include "gui_businessHome.h"

#include "devDriver_manage.h"

#include "dataTrans_remoteServer.h"

static const char *TAG = "HAP Bridge";

#define BRIDGE_TASK_PRIORITY  2
#define BRIDGE_TASK_STACKSIZE 4 * 1024
#define BRIDGE_TASK_NAME      "hap_bridge"

#define NUM_BRIDGED_ACCESSORIES 3

/* Reset network credentials if button is pressed for more than 3 seconds and then released */
#define RESET_NETWORK_BUTTON_TIMEOUT        3

/* Reset to factory if button is pressed and held for more than 10 seconds */
#define RESET_TO_FACTORY_BUTTON_TIMEOUT     10

/* The button "Boot" will be used as the Reset button for the example */
#define RESET_GPIO  GPIO_NUM_0

extern void lvGui_wifiConfig_bussiness_configComplete_tipsTrig(void);

static void htAccessoryDevice_registerBySwitchType(uint8_t devMAC[MWIFI_ADDR_LEN]);
static esp_err_t lanbonL8HT_nvsDataRecovery(void);

EventGroupHandle_t xEventGp_htApplication = NULL;

static bool nwkL8Hk_wifiConnected_flg = false;
static bool HAP_WAC_STANDBY_FLG = false;

static hap_acc_t *accessory = NULL;
static hap_serv_t *service = NULL;

static char strHK_setup_payload[32] = "X-HM://003KULGZVMJ2T",
            strHK_setup_code[16] = "111-22-333";

/**
 * @brief The network reset button callback handler.
 * Useful for testing the Wi-Fi re-configuration feature of WAC2
 */
static void reset_network_handler(void* arg)
{
    hap_reset_network();
}
/**
 * @brief The factory reset button callback handler.
 */
static void reset_to_factory_handler(void* arg)
{
    hap_reset_to_factory();
}

/**
 * The Reset button  GPIO initialisation function.
 * Same button will be used for resetting Wi-Fi network as well as for reset to factory based on
 * the time for which the button is pressed.
 */
static void reset_key_init(uint32_t key_gpio_pin)
{
    button_handle_t handle = iot_button_create(key_gpio_pin, BUTTON_ACTIVE_LOW);
    iot_button_add_on_release_cb(handle, RESET_NETWORK_BUTTON_TIMEOUT, reset_network_handler, NULL);
    iot_button_add_on_press_cb(handle, RESET_TO_FACTORY_BUTTON_TIMEOUT, reset_to_factory_handler, NULL);
}

/* Setup Information for the Setup Code: 111-22-333 */
static const hap_setup_info_t setup_info = {
    .salt = {
        0x93, 0x15, 0x1A, 0x47, 0x57, 0x55, 0x3C, 0x21, 0x0B, 0x55, 0x89, 0xB8, 0xC3, 0x99, 0xA0, 0xF3
    },
    .verifier = {
        0x9E, 0x9C, 0xC3, 0x73, 0x9B, 0x04, 0x83, 0xC8, 0x13, 0x7C, 0x5B, 0x5F, 0xAC, 0xC5, 0x63, 0xDF,
        0xF4, 0xF1, 0x0F, 0x39, 0x06, 0x4A, 0x20, 0x2D, 0x53, 0x2A, 0x09, 0x20, 0x3A, 0xA6, 0xBA, 0xE3,
        0x1E, 0x42, 0x4E, 0x58, 0x4E, 0xBB, 0x44, 0x5F, 0x7F, 0xDF, 0xCC, 0x11, 0xD0, 0xF7, 0x8B, 0x35,
        0xE1, 0x16, 0xA9, 0x79, 0x30, 0xBC, 0x37, 0x19, 0x77, 0x36, 0xB1, 0xEC, 0xD4, 0x12, 0x4C, 0xE4,
        0x5D, 0xE3, 0x7E, 0x46, 0xA0, 0x2D, 0x10, 0x07, 0xAB, 0x48, 0x40, 0x36, 0xD5, 0x3F, 0x7F, 0xBE,
        0xA5, 0xAE, 0xD0, 0x25, 0x6B, 0xC4, 0x9E, 0xC8, 0x5F, 0xC9, 0x4E, 0x47, 0x0D, 0xBA, 0xD3, 0x63,
        0x44, 0x20, 0x01, 0x69, 0x97, 0xDD, 0x20, 0x54, 0x7C, 0x59, 0x78, 0x3D, 0x5C, 0x6D, 0xC7, 0x1F,
        0xE6, 0xFD, 0xA0, 0x8E, 0x9B, 0x36, 0x45, 0x1F, 0xC1, 0x4B, 0xB5, 0x26, 0xE1, 0x8E, 0xEB, 0x4C,
        0x05, 0x58, 0xD7, 0xC8, 0x80, 0xA1, 0x43, 0x7F, 0x5F, 0xDB, 0x75, 0x1B, 0x19, 0x57, 0x25, 0xAC,
        0x5D, 0xF5, 0x8D, 0xF6, 0x7B, 0xAA, 0xB7, 0x7D, 0xE0, 0x36, 0xEF, 0xEA, 0xF3, 0x57, 0xAC, 0xFE,
        0x12, 0x87, 0xF9, 0x31, 0x4C, 0xF7, 0x44, 0xBD, 0xB6, 0x26, 0x6C, 0xB4, 0x0D, 0x7C, 0x52, 0x4F,
        0x85, 0x56, 0x91, 0x5D, 0x13, 0xD8, 0xDA, 0x8C, 0x45, 0x3E, 0x73, 0xF2, 0xF9, 0x20, 0x39, 0x24,
        0x8B, 0xFB, 0xEE, 0xFD, 0x77, 0x54, 0x8D, 0x37, 0x22, 0xE8, 0x55, 0xC3, 0xD2, 0xF8, 0xB8, 0x23,
        0xB0, 0xE2, 0x9E, 0x43, 0xAE, 0xB4, 0x37, 0xFA, 0xA7, 0x03, 0xF1, 0x82, 0x68, 0x4C, 0xD4, 0x86,
        0xC6, 0x3E, 0xDE, 0x70, 0x11, 0x03, 0x77, 0x46, 0x59, 0x14, 0x97, 0xC6, 0xAE, 0x52, 0x6F, 0x03,
        0x77, 0x36, 0x40, 0xBC, 0xDE, 0xCD, 0x3D, 0xE0, 0x4F, 0x69, 0x18, 0x0D, 0xCA, 0x85, 0x7E, 0x07,
        0x30, 0xF4, 0xA1, 0xCE, 0x05, 0xB5, 0x4B, 0xE1, 0x1D, 0x43, 0xDF, 0xDB, 0x11, 0x43, 0xDE, 0x21,
        0xAC, 0x8F, 0x03, 0x9E, 0x6E, 0x9F, 0xA8, 0xE5, 0x02, 0x06, 0x1C, 0x63, 0x34, 0x22, 0x1D, 0x39,
        0xE3, 0x3D, 0x12, 0x2E, 0xA2, 0xF3, 0xFC, 0xB5, 0xB4, 0x16, 0x9E, 0x0E, 0x7C, 0x52, 0xC8, 0x7D,
        0x50, 0x3D, 0xDB, 0xF5, 0x83, 0x46, 0x18, 0x92, 0x7F, 0x4D, 0x38, 0xAD, 0x0A, 0x2A, 0xBC, 0x2A,
        0x50, 0x4B, 0xDF, 0x5D, 0xFA, 0x93, 0x41, 0x78, 0xD6, 0x45, 0x54, 0xDB, 0x44, 0x81, 0xF7, 0x5A,
        0x0A, 0xDD, 0x18, 0x4F, 0x27, 0xD7, 0xDD, 0x5E, 0xB7, 0x3E, 0x99, 0xE6, 0xE1, 0x69, 0x35, 0x74,
        0xD6, 0x98, 0x58, 0xB2, 0x13, 0x6F, 0xB7, 0x82, 0x72, 0xBC, 0xA6, 0x8B, 0xA3, 0x36, 0x2A, 0xCE,
        0x65, 0x65, 0x51, 0x08, 0x8A, 0x3D, 0x04, 0x93, 0x8F, 0x01, 0x8A, 0xAB, 0x4B, 0xFC, 0x06, 0xF9
    }
};

/* Mandatory identify routine for the accessory (bridge)
 * In a real accessory, something like LED blink should be implemented
 * got visual identification
 */
static int device_identify(hap_acc_t *ha)
{
    ESP_LOGI(TAG, "Bridge identified");
    return HAP_SUCCESS;
}

/* Mandatory identify routine for the bridged accessory
 * In a real bridge, the actual accessory must be sent some request to
 * identify itself visually
 */
static int accessory_identify(hap_acc_t *ha)
{
    hap_serv_t *hs = hap_acc_get_serv_by_uuid(ha, HAP_SERV_UUID_ACCESSORY_INFORMATION);
    hap_char_t *hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_NAME);
    const hap_val_t *val = hap_char_get_val(hc);
    char *name = val->s;

    ESP_LOGI(TAG, "Bridged Accessory %s identified", name);
    return HAP_SUCCESS;
}

/* A dummy callback for handling a write on the "On" characteristic of Fan.
 * In an actual accessory, this should control the hardware
 */
static int light_on(bool value)
{
    ESP_LOGI(TAG, "Received Write. light %s", value ? "On" : "Off");
    /* TODO: Control Actual Hardware */
    return 0;
}

/*
 * An optional HomeKit Event handler which can be used to track HomeKit
 * specific events.
 */
void dev_hap_event_handler(hap_event_t event, void *data)
{
    switch(event) {
        case HAP_EVENT_PAIRING_STARTED :
            ESP_LOGI(TAG, "Pairing Started");
            break;
        case HAP_EVENT_PAIRING_ABORTED :
            ESP_LOGI(TAG, "Pairing Aborted");
            break;
        case HAP_EVENT_CTRL_PAIRED :
            ESP_LOGI(TAG, "Controller %s Paired. Controller count: %d",
                        (char *)data, hap_get_paired_controller_count());
            break;
        case HAP_EVENT_CTRL_UNPAIRED :
            ESP_LOGI(TAG, "Controller %s Removed. Controller count: %d",
                        (char *)data, hap_get_paired_controller_count());
            break;
        case HAP_EVENT_CTRL_CONNECTED :
            ESP_LOGI(TAG, "Controller %s Connected", (char *)data);
            HAP_WAC_STANDBY_FLG = true;
            mqtt_app_start();
            break;
        case HAP_EVENT_CTRL_DISCONNECTED :
            ESP_LOGI(TAG, "Controller %s Disconnected", (char *)data);
            break;
        case HAP_EVENT_WAC_STARTED :
            ESP_LOGI(TAG, "WAC Started with name %s", (char *)data);
            break;
        case HAP_EVENT_WAC_TIMEOUT :
            ESP_LOGI(TAG, "WAC Stopped due to timeout. Please reboot the accessory");
            break;

        case HAP_EVENT_WAC_SUCCESS :
        case HAP_EVENT_EXT_PROV_SUCCESS : {
            hap_provisioned_nw_t *sta = (hap_provisioned_nw_t *)data;
            ESP_LOGI(TAG, "Provisioned to network: %s, with Password: %s",
                    sta->ssid, sta->password);
            break;
        }
        case HAP_EVENT_ACC_REBOOTING : {
            char *reason = (char *)data;
            ESP_LOGI(TAG, "Accessory Rebooting (Reason: %s)",  reason ? reason : "null");
            break;
        }
        default:
            /* Silently ignore unknown events */
            break;
    }
}

static esp_err_t hap_system_handler(void *ctx, system_event_t * event)
{
    switch(event->event_id) {

        case SYSTEM_EVENT_STA_START:
            printf("sta connect start.\n");

            meshNetwork_connectReserve_IF_set(true);

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED)|\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_SOCKET)|\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_MOUDLE)

#else
            lvGui_usrAppBussinessRunning_block(0, "wifi\nconnecting...", 30); //UI阻塞提示，wifi连接中
#endif
            break;
        case SYSTEM_EVENT_STA_STOP:
            printf("sta connect stop.\n");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:{

            wifi_ap_record_t apInfo = {0};

            nwkL8Hk_wifiConnected_flg = true;
            flgSet_gotRouterOrMeshConnect(nwkL8Hk_wifiConnected_flg);

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED)|\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_SOCKET)|\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_MOUDLE)

			devBeepTips_trig(4, 8, 100, 40, 2); //beeps提示
#else

			lvGui_usrAppBussinessRunning_block(2, "\nwifi connected", 5);  //UI阻塞提示，wifi连接成功
			lvGui_wifiConfig_bussiness_configComplete_tipsTrig();
#endif 

                if(ESP_OK == esp_wifi_sta_get_ap_info(&apInfo)){

                    devRouterConnectBssid_Set(apInfo.bssid, true);
                }

            }break;  
        case SYSTEM_EVENT_STA_GOT_IP:
            printf("Got IP: %s\n", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
            if(HAP_WAC_STANDBY_FLG == true){

                mqtt_app_start();
            }
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            printf("Failed to connect to network/disconnected.\n");
            nwkL8Hk_wifiConnected_flg = false;
            break;
        case SYSTEM_EVENT_STA_LOST_IP:
            printf("Disconnected.\n");
            mqtt_app_stop();
            break;
        default:
            break;
    }
    return ESP_OK;
}

/* A dummy callback for handling a read on the "Direction" characteristic of Fan.
 * In an actual accessory, this should read from hardware.
 * Read routines are generally not required as the value is available with th HAP core
 * when it is updated from write routines. For external triggers (like fan switched on/off
 * using physical button), accessories should explicitly call hap_char_update_val()
 * instead of waiting for a read request.
 */
static int htDevice_read(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv)
{
    switch(currentDev_typeGet()){

        case devTypeDef_thermostat:{

            if (!strcmp(hap_char_get_type_uuid(hc), HAP_CHAR_UUID_CURRENT_TEMPERATURE)) {

                hap_val_t new_val;

                new_val.f = devDriverBussiness_temperatureMeasure_get();
                if(new_val.f > 50.0F || new_val.f < -30.0)new_val.f = 0.0F;
                
                hap_char_update_val(hc, &new_val);
                *status_code = HAP_STATUS_SUCCESS;
            }            
        }break;

        default:break;
    }

    return HAP_SUCCESS;
}

/* A dummy callback for handling a write on the "On" characteristic of Fan.
 * In an actual accessory, this should control the hardware
 */
static int htDevice_write(hap_write_data_t write_data[], int count,
        void *serv_priv, void *write_priv)
{
    // ESP_LOGI(TAG, "Write called for Accessory %s", (char *)serv_priv);
    int i, ret = HAP_SUCCESS;
    hap_write_data_t *write;
    stt_devDataPonitTypedef stateSet_temp = {0};

    for (i = 0; i < count; i++) {
        write = &write_data[i];

        switch(currentDev_typeGet()){

            case devTypeDef_mulitSwOneBit:
            case devTypeDef_moudleSwOneBit:
            case devTypeDef_mulitSwTwoBit:
            case devTypeDef_moudleSwTwoBit:
            case devTypeDef_mulitSwThreeBit:
            case devTypeDef_moudleSwThreeBit:{

                int swBitIst = 0;

                sscanf(serv_priv, "L8-Light-%d", &swBitIst);
                swBitIst --;
                
                if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {

                    uint8_t statusHex = 0;

                    currentDev_dataPointGet((stt_devDataPonitTypedef *)&statusHex);

                    hap_char_update_val(write->hc, &(write->val));
                    *(write->status) = HAP_STATUS_SUCCESS;

                    (write->val.b)?(statusHex |= (1 << swBitIst)):(statusHex &= ~(1 << swBitIst));
                    currentDev_dataPointSet((stt_devDataPonitTypedef *)&statusHex, true ,false, false, true ,false);

                } else {

                    *(write->status) = HAP_STATUS_RES_ABSENT;
                }

            }break;

            case devTypeDef_curtain:
            case devTypeDef_moudleSwCurtain:{

                if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_TARGET_POSITION)){

                    hap_char_update_val(write->hc, &(write->val));
                    *(write->status) = HAP_STATUS_SUCCESS;

                    stateSet_temp.devType_curtain.devCurtain_actMethod = 1;
                    stateSet_temp.devType_curtain.devCurtain_actEnumVal = write->val.i;
                    currentDev_dataPointSet(&stateSet_temp, true ,false, false, true ,false);

                } else if(!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_POSITION_STATE)){

                    ESP_LOGI(TAG, "Received Write for curtain positionState %d", write->val.i);

                    hap_char_update_val(write->hc, &(write->val));
                    *(write->status) = HAP_STATUS_SUCCESS;

                }else {

                    *(write->status) = HAP_STATUS_RES_ABSENT;
                }

            }break;

            case devTypeDef_dimmer:{

                if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {

                    ESP_LOGI(TAG, "Received Write for dimmer ct %d", write->val.b);

                    hap_char_update_val(write->hc, &(write->val));
                    *(write->status) = HAP_STATUS_SUCCESS;

                    (true == write->val.b)?
                        (stateSet_temp.devType_dimmer.devDimmer_brightnessVal = devDriverBussiness_dimmerSwitch_brightnessLastGet()):
                        (stateSet_temp.devType_dimmer.devDimmer_brightnessVal = 0);
                    currentDev_dataPointSet(&stateSet_temp, true ,false, false, true ,false);
                        
                } else if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_BRIGHTNESS)) {

                    ESP_LOGI(TAG, "Received Write for dimmer bt %d", write->val.i);

                    hap_char_update_val(write->hc, &(write->val));
                    *(write->status) = HAP_STATUS_SUCCESS;

                    stateSet_temp.devType_dimmer.devDimmer_brightnessVal = write->val.i;
                    currentDev_dataPointSet(&stateSet_temp, true ,false, false, true ,false);

                } else {

                    *(write->status) = HAP_STATUS_RES_ABSENT;
                }

            }break;

            case devTypeDef_thermostat:{

                stt_thermostat_actAttr devParam = {0};

                currentDev_dataPointGet(&stateSet_temp);
                devDriverBussiness_thermostatSwitch_devParam_get(&devParam);

                if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_TARGET_TEMPERATURE)){

                    hap_char_update_val(write->hc, &(write->val));
                    *(write->status) = HAP_STATUS_SUCCESS;

                    stateSet_temp.devType_thermostat.devThermostat_tempratureTarget = (uint8_t)write->val.f;
                    currentDev_dataPointSet(&stateSet_temp, true ,false, false, true ,false);

                } else if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_TARGET_HEATING_COOLING_STATE)){

                    ESP_LOGI(TAG, "Received Write for thermostat workState %d", write->val.i);

                    hap_char_update_val(write->hc, &(write->val));
                    *(write->status) = HAP_STATUS_SUCCESS;
#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)
                    devDriverBussiness_thermostatSwitch_devParam_runMode_set(write->val.i);
#endif
                    switch(write->val.i){
                        case 0:stateSet_temp.devType_thermostat.devThermostat_running_en = 0;break;
                        case 1:
                        case 2:
                        case 3:stateSet_temp.devType_thermostat.devThermostat_running_en = 1;break;
                        default:break;
                    }
                    currentDev_dataPointSet(&stateSet_temp, true ,false, false, true ,false);

                } else {

                    *(write->status) = HAP_STATUS_RES_ABSENT;
                }

            }break;

            case devTypeDef_heater:{

                if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {

                    ESP_LOGI(TAG, "Received Write for heater ct %d", write->val.b);

                    hap_char_update_val(write->hc, &(write->val));
                    *(write->status) = HAP_STATUS_SUCCESS;

                    (true == write->val.b)?
                        (stateSet_temp.devType_heater.devHeater_swEnumVal = heaterOpreatAct_open):
                        (stateSet_temp.devType_heater.devHeater_swEnumVal = heaterOpreatAct_close);
                    currentDev_dataPointSet(&stateSet_temp, true ,false, false, true ,false);
                    
                } else {

                    *(write->status) = HAP_STATUS_RES_ABSENT;
                }

            }break;

            case devTypeDef_socket:{

                 if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {

                    ESP_LOGI(TAG, "Received Write for socket ct %d", write->val.b);

                    hap_char_update_val(write->hc, &(write->val));
                    *(write->status) = HAP_STATUS_SUCCESS;

                    (true == write->val.b)?
                        (stateSet_temp.devType_socket.devSocket_opSw = 1):
                        (stateSet_temp.devType_socket.devSocket_opSw = 0);
                    currentDev_dataPointSet(&stateSet_temp, true ,false, false, true ,false);
                    
                } else {

                    *(write->status) = HAP_STATUS_RES_ABSENT;
                }

            }break;

            default:break;
        }

        if (*(write->status) == HAP_STATUS_SUCCESS) {
            hap_char_update_val(write->hc, &(write->val));
        } else {
            /* Else, set the return value appropriately to report error */
            ret = HAP_FAIL;
        }
    }
    return ret;
}

static void homekit_MFI_chip_gpioDeinit(void){

    gpio_reset_pin(CONFIG_SCL_GPIO);
    gpio_reset_pin(CONFIG_SDA_GPIO);
}

/*The main thread for handling the Bridge Accessory */
static void bridge_thread_entry(void *p)
{
    uint8_t haDevSelfMac[MWIFI_ADDR_LEN] = {0};
    esp_wifi_get_mac(ESP_IF_WIFI_STA, haDevSelfMac);	

    lanbonL8HT_nvsDataRecovery();
    homekit_MFI_chip_gpioDeinit();

    /* Initialize the HAP core */
    hap_init(HAP_TRANSPORT_WIFI);

    htAccessoryDevice_registerBySwitchType(haDevSelfMac);

    /* Register a common button for reset Wi-Fi network and reset to factory.
     */
    // reset_key_init(RESET_GPIO);

    /* Use the setup_payload_gen tool to get the QR code for Accessory Setup.
     * The payload below is for a Bridge with setup code 111-22-333 and setup id ES32
     */
    ESP_LOGI(TAG, "Use setup payload: \"%s\" for Accessory Setup", strHK_setup_payload);

    /* Register an event handler for HomeKit specific events */
    hap_register_event_handler(dev_hap_event_handler);

    /* Register a system event handler for System events */
    hap_platform_wifi_register_system_event_cb(hap_system_handler);

    /* Enable WAC2 as per HAP Spec R12 */
    // hap_enable_wac2();

    /* After all the initializations are done, start the HAP core */
    hap_start();

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED)|\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_SOCKET)|\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_MOUDLE)

#else
   lvGui_usrAppBussinessRunning_blockCancel();
#endif

    /* The task ends here. The read/write callbacks will be invoked by the HAP Framework */
    vTaskDelete(NULL);
}

static esp_err_t lanbonL8HT_nvsDataRecovery(void){

    uint32_t dataLength = 0;
    char *strPtr = NULL;
    nvs_handle my_handle;
    esp_err_t err;

    err = nvs_flash_init_partition("factory_nvs");
    ESP_LOGI(TAG, "nvsTest init res:%d.", err);

    // Open
    err = nvs_open_from_partition("factory_nvs", "hap_setup", NVS_READONLY, &my_handle);
    ESP_LOGI(TAG, "nvsTest open res:%d.", err);
    if (err != ESP_OK) return err;

    // Read
    err = nvs_get_str(my_handle, "setup_payload", NULL, &dataLength);
    ESP_LOGI(TAG, "nvsTest open a res:%04X.", err);
    strPtr = (char *)malloc(dataLength);
    err = nvs_get_str(my_handle, "setup_payload", strPtr, &dataLength);
    ESP_LOGI(TAG, "nvsTest open b res:%04X.", err);    
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;
    ESP_LOGI(TAG, "nvsTest str read:%s.\n", strPtr);
    strcpy(strHK_setup_payload, strPtr);
    free(strPtr);
    strPtr = NULL;

    err = nvs_get_str(my_handle, "setup_code", NULL, &dataLength);
    ESP_LOGI(TAG, "nvsTest open c res:%04X.", err);
    strPtr = (char *)malloc(dataLength);
    err = nvs_get_str(my_handle, "setup_code", strPtr, &dataLength);
    ESP_LOGI(TAG, "nvsTest open d res:%04X.", err);    
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;
    ESP_LOGI(TAG, "nvsTest str read:%s.\n", strPtr);
    strcpy(strHK_setup_code, strPtr);
    free(strPtr);
    strPtr = NULL;

    // Close
    nvs_close(my_handle);
    nvs_flash_deinit_partition("factory_nvs");
    return ESP_OK;
}

static void threadProcess_htEventTrig(void *p){

    hap_acc_t *hap_acc = NULL;
    hap_serv_t *hap_serv = NULL;
    hap_char_t *hap_char = NULL;
    hap_val_t appliance_value = {0};

    EventBits_t uxBits = 0;

    for(;;){

        uxBits = xEventGroupWaitBits(xEventGp_htApplication, 
                                     HTAPPLICATION_FLG_BITHOLD_RESERVE,
                                     pdTRUE,
                                     pdFALSE,
                                     2 / portTICK_RATE_MS);

        if(uxBits & HTAPPLICATION_FLG_BITHOLD_ACCESSORY_REG){

            // hap_delete_all_accessories();
            htAccessoryDevice_registerBySwitchType(NULL);

            vTaskDelay(20 / portTICK_RATE_MS);
        }

        if(uxBits & HTAPPLICATION_FLG_BITHOLD_DATAUPLD_PERIOD){

            if(true == hkL8_nwkWifiConnected_statusGet()){

                switch(currentDev_typeGet()){
                    
                    case devTypeDef_thermostat:{

                        hap_acc = hap_acc_get_by_aid(1);
                        hap_serv = hap_acc_get_serv_by_uuid(hap_acc, HAP_SERV_UUID_THERMOSTAT);
                        hap_char = hap_serv_get_char_by_uuid(hap_serv, HAP_CHAR_UUID_CURRENT_TEMPERATURE);
                        appliance_value.f = devDriverBussiness_temperatureMeasure_get();
                        if(appliance_value.f > 50.0F || appliance_value.f < -30.0)appliance_value.f = 0.0F;
                        hap_char_update_val(hap_char, &appliance_value);

                    }break;

                    default:break;
                }
            }
        }

        if(uxBits & HTAPPLICATION_FLG_BITHOLD_DATAUPLD_TRIG){

            stt_devDataPonitTypedef dpCurrent = {0};

            currentDev_dataPointGet(&dpCurrent);
            deviceStatusUpdateToHT(&dpCurrent);
        }

        vTaskDelay(10 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

static void htAccessoryDevice_registerBySwitchType(uint8_t devMAC[MWIFI_ADDR_LEN]){

    uint8_t haDevSelfMac[MWIFI_ADDR_LEN] = {0};

    char strHandleTemp_cfgSerialNum[32] = {0};

    esp_wifi_get_mac(ESP_IF_WIFI_STA, haDevSelfMac);	
    if(devMAC == NULL)devMAC = haDevSelfMac;

    /* Initialise the mandatory parameters for Accessory which will be added as
     * the mandatory services internally
     */
    hap_acc_cfg_t cfg = {
        .name = "L8_lightBundle",
        .manufacturer = "Lanbon",
        .model = "L8_LightBundle",
        .serial_num = "9020200521",
        .fw_rev = "1.0.1",
        .hw_rev = NULL,
        .pv = "1.0.1",
        .identify_routine = device_identify,
        .cid = HAP_CID_BRIDGE,
    };

    snprintf(strHandleTemp_cfgSerialNum, sizeof(strHandleTemp_cfgSerialNum), "%02X%02X%02X%02X%02X%02X", MAC2STR(devMAC));
    cfg.serial_num = strHandleTemp_cfgSerialNum;

    // if(NULL != devMAC)
    // if(0 == memcmp(haDevSelfMac, devMAC, sizeof(uint8_t) * MWIFI_ADDR_LEN)){
    if(1){

        switch(currentDev_typeGet()){

            case devTypeDef_mulitSwOneBit:
            case devTypeDef_moudleSwOneBit:
            case devTypeDef_mulitSwTwoBit:
            case devTypeDef_moudleSwTwoBit:
            case devTypeDef_mulitSwThreeBit:
            case devTypeDef_moudleSwThreeBit:
            case devTypeDef_curtain:
            case devTypeDef_moudleSwCurtain:{

                uint8_t creatLoop = 0;
                static bool bgCreated_flg = false;

                switch(currentDev_typeGet()){

                    case devTypeDef_mulitSwOneBit:
                    case devTypeDef_moudleSwOneBit:creatLoop = 1;break;

                    case devTypeDef_mulitSwTwoBit:
                    case devTypeDef_moudleSwTwoBit:creatLoop = 2;break;

                    case devTypeDef_mulitSwThreeBit:
                    case devTypeDef_moudleSwThreeBit:creatLoop = 3;break;

                    default:break;
                }

                if(false == bgCreated_flg){

                    bgCreated_flg = true;

                    cfg.name = "L8_bridge";
                    cfg.manufacturer = "Lanbon";
                    cfg.model = "L8_Bridge";
                    cfg.cid = HAP_CID_BRIDGE;

                    accessory = hap_acc_create(&cfg);
                    hap_add_accessory(accessory); 
                }

                switch(currentDev_typeGet()){

                    case devTypeDef_mulitSwOneBit:
                    case devTypeDef_moudleSwOneBit:
                    case devTypeDef_mulitSwTwoBit:
                    case devTypeDef_moudleSwTwoBit:
                    case devTypeDef_mulitSwThreeBit:
                    case devTypeDef_moudleSwThreeBit:{

                        /* Create and add the Accessory to the Bridge object*/
                        for (int i = 0; i < creatLoop; i++) {

                            char accessory_name[16] = {0},
                                 accessory_serialNum[32] = {0};
                                
                            sprintf(accessory_name, "L8-Light-%d", i + 1);
                            sprintf(accessory_serialNum, "%02X%02X%02X%02X%02X%02X-%d", MAC2STR(devMAC), i + 1);

                            hap_acc_cfg_t bridge_cfg = {
                                .name = accessory_name,
                                .manufacturer = "Lanbon",
                                .model = "L8_Light",
                                .serial_num = accessory_serialNum,
                                .fw_rev = "1.0.1",
                                .hw_rev = NULL,
                                .pv = "1.0.1",
                                .identify_routine = accessory_identify,
                                .cid = HAP_CID_BRIDGE,
                            };
                            /* Create accessory object */
                            accessory = hap_acc_create(&bridge_cfg);

                            service = hap_serv_switch_create(false);
                            hap_serv_add_char(service, hap_char_name_create(accessory_name));
                            hap_serv_set_priv(service, strdup(accessory_name));
                            hap_serv_set_write_cb(service, htDevice_write);
                            hap_acc_add_serv(accessory, service);
                            hap_add_bridged_accessory(accessory, hap_get_unique_aid(accessory_name));
                        }  

                    }break;

                    case devTypeDef_curtain:
                    case devTypeDef_moudleSwCurtain:{

                        char accessory_name[16] = {0},
                            accessory_serialNum[32] = {0};
                            
                        sprintf(accessory_name, "L8-Curtain");
                        sprintf(accessory_serialNum, "%02X%02X%02X%02X%02X%02X", MAC2STR(devMAC));

                        hap_acc_cfg_t bridge_cfg = {
                            .name = accessory_name,
                            .manufacturer = "Lanbon",
                            .model = "L8_Curtain",
                            .serial_num = accessory_serialNum,
                            .fw_rev = "1.0.1",
                            .hw_rev = NULL,
                            .pv = "1.0.1",
                            .identify_routine = accessory_identify,
                            .cid = HAP_CID_BRIDGE,
                        };
                        /* Create accessory object */
                        accessory = hap_acc_create(&bridge_cfg);

                        service = hap_serv_window_covering_create(0, 0, 0);
                        hap_serv_add_char(service, hap_char_name_create(accessory_name));
                        hap_serv_set_priv(service, strdup(accessory_name));
                        hap_serv_set_write_cb(service, htDevice_write);
                        hap_acc_add_serv(accessory, service);
                        hap_add_bridged_accessory(accessory, hap_get_unique_aid(accessory_name));

                    }break;

                    default:break;
                }

            }break;

            case devTypeDef_dimmer:{

                cfg.name = "L8_dimmer";
                cfg.manufacturer = "Lanbon";
                cfg.model = "L8_Dimmer";
                cfg.cid = HAP_CID_LIGHTING;

                accessory = hap_acc_create(&cfg);
                service = hap_serv_lightbulb_create(true);
                int ret = hap_serv_add_char(service, hap_char_name_create("My Light"));
                ret |= hap_serv_add_char(service, hap_char_brightness_create(50));
                hap_serv_set_write_cb(service, htDevice_write);

                hap_acc_add_serv(accessory, service);
                hap_add_accessory(accessory);

            }break;

            case devTypeDef_heater:{

                cfg.name = "L8_heater";
                cfg.manufacturer = "Lanbon";
                cfg.model = "L8_Heater";
                cfg.cid = HAP_CID_HEATER;

                accessory = hap_acc_create(&cfg);
                service = hap_serv_switch_create(false);
                hap_serv_set_write_cb(service, htDevice_write);

                hap_acc_add_serv(accessory, service);
                hap_add_accessory(accessory); 

            }break;

            case devTypeDef_thermostat:{

                cfg.name = "L8_thermostat";
                cfg.manufacturer = "Lanbon";
                cfg.model = "L8_Thermostat";
                cfg.cid = HAP_CID_THERMOSTAT; 

                accessory = hap_acc_create(&cfg);
                service = hap_serv_thermostat_create(0, 0, 16.0F, 16.0F, 0);
                hap_serv_set_write_cb(service, htDevice_write);
                hap_serv_set_read_cb(service, htDevice_read);

                hap_acc_add_serv(accessory, service);
                hap_add_accessory(accessory); 

            }break;
            
            case devTypeDef_socket:{

                cfg.name = "L8_socket";
                cfg.manufacturer = "Lanbon";
                cfg.model = "L8_Socket";
                cfg.cid = HAP_CID_SWITCH; 

                accessory = hap_acc_create(&cfg);
                service = hap_serv_switch_create(false);
                hap_serv_set_write_cb(service, htDevice_write);

                hap_acc_add_serv(accessory, service);
                hap_add_accessory(accessory); 

            }break;

            default:break;
        }
    }
}

void homekitDevice_setupPayloadStr_get(char str[]){

    strcpy(str, strHK_setup_payload);
}

void homekitDevice_setupCodeStr_get(char str[]){

    strcpy(str, strHK_setup_code);
}

void deviceDestoryPreventChange(devTypeDef_enum devType){

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_SHARE_MIX)

    hap_acc_t *hap_acc = NULL;

    switch(devType){

        case devTypeDef_mulitSwOneBit:
        case devTypeDef_moudleSwOneBit:{

            hap_acc = hap_acc_get_by_aid(2);
            hap_remove_bridged_accessory(hap_acc);

        }break;

        case devTypeDef_mulitSwTwoBit:
        case devTypeDef_moudleSwTwoBit:{

            hap_acc = hap_acc_get_by_aid(2);
            hap_remove_bridged_accessory(hap_acc);
            hap_acc = hap_acc_get_by_aid(3);
            hap_remove_bridged_accessory(hap_acc);

        }break;

        case devTypeDef_mulitSwThreeBit:
        case devTypeDef_moudleSwThreeBit:{

            hap_acc = hap_acc_get_by_aid(2);
            hap_remove_bridged_accessory(hap_acc);
            hap_acc = hap_acc_get_by_aid(3);
            hap_remove_bridged_accessory(hap_acc);
            hap_acc = hap_acc_get_by_aid(4);
            hap_remove_bridged_accessory(hap_acc);

        }break;

        case devTypeDef_curtain:
        case devTypeDef_moudleSwCurtain:{

            hap_acc = hap_acc_get_by_aid(5);
            hap_remove_bridged_accessory(hap_acc);

        }break;

        default:break;
    }
#endif
}

void deviceStatusUpdateToHT(stt_devDataPonitTypedef *staVal){

    uint8_t statusHex = 0;
    int8_t loop = 0;

    hap_acc_t *hap_acc = NULL;
    hap_serv_t *hap_serv = NULL;
    hap_char_t *hap_char = NULL;
    hap_val_t appliance_value = {0};

    memcpy(&statusHex, staVal, sizeof(uint8_t));

    switch(currentDev_typeGet()){

        case devTypeDef_mulitSwOneBit:
        case devTypeDef_moudleSwOneBit:
        case devTypeDef_mulitSwTwoBit:
        case devTypeDef_moudleSwTwoBit:
        case devTypeDef_mulitSwThreeBit:
        case devTypeDef_moudleSwThreeBit:{

            uint8_t creatLoop = 0;

            switch(currentDev_typeGet()){

                case devTypeDef_mulitSwOneBit:
                case devTypeDef_moudleSwOneBit:creatLoop = 1;break;

                case devTypeDef_mulitSwTwoBit:
                case devTypeDef_moudleSwTwoBit:creatLoop = 2;break;

                case devTypeDef_mulitSwThreeBit:
                case devTypeDef_moudleSwThreeBit:creatLoop = 3;break;

                default:break;
            }

            for(loop = 0; loop < creatLoop; loop ++){

                hap_acc = hap_acc_get_by_aid(loop + 2);
                hap_serv = hap_acc_get_serv_by_uuid(hap_acc, HAP_SERV_UUID_SWITCH);
                hap_char = hap_serv_get_char_by_uuid(hap_serv, HAP_CHAR_UUID_ON);
                (statusHex & (1 << (loop)))?(appliance_value.i = 1):(appliance_value.i = 0);

                hap_char_update_val(hap_char, &appliance_value);
            }
        } 

        case devTypeDef_curtain:
        case devTypeDef_moudleSwCurtain:{

            uint8_t positionTemp = 0;

            if(staVal->devType_curtain.devCurtain_actMethod){

               positionTemp = staVal->devType_curtain.devCurtain_actEnumVal;

            }else{

                switch(staVal->devType_curtain.devCurtain_actEnumVal){

                    case curtainRunningStatus_cTact_stop:positionTemp = devCurtain_currentPositionPercentGet();break;
                    case curtainRunningStatus_cTact_close:positionTemp = 0;break;
                    case curtainRunningStatus_cTact_open:positionTemp = 100;break;

                    default:break;
                }
            }

            hap_acc = hap_acc_get_by_aid(5);
            hap_serv = hap_acc_get_serv_by_uuid(hap_acc, HAP_SERV_UUID_WINDOW_COVERING);
            hap_char = hap_serv_get_char_by_uuid(hap_serv, HAP_CHAR_UUID_TARGET_POSITION);
            appliance_value.i = positionTemp;
            hap_char_update_val(hap_char, &appliance_value);

            hap_char = hap_serv_get_char_by_uuid(hap_serv, HAP_CHAR_UUID_CURRENT_POSITION);
            appliance_value.i = positionTemp;
            hap_char_update_val(hap_char, &appliance_value);

        }break;

        case devTypeDef_dimmer:{

            hap_acc = hap_acc_get_by_aid(1);
            hap_serv = hap_acc_get_serv_by_uuid(hap_acc, HAP_SERV_UUID_LIGHTBULB);
            hap_char = hap_serv_get_char_by_uuid(hap_serv, HAP_CHAR_UUID_BRIGHTNESS);
            appliance_value.i = staVal->devType_dimmer.devDimmer_brightnessVal;
            hap_char_update_val(hap_char, &appliance_value); 

            hap_char = hap_serv_get_char_by_uuid(hap_serv, HAP_CHAR_UUID_ON);
            (staVal->devType_dimmer.devDimmer_brightnessVal)?
                (appliance_value.b = true):
                (appliance_value.b = false);
            hap_char_update_val(hap_char, &appliance_value); 

        }break;

        case devTypeDef_thermostat:{

            stt_thermostat_actAttr devParam = {0};

            hap_acc = hap_acc_get_by_aid(1);
            hap_serv = hap_acc_get_serv_by_uuid(hap_acc, HAP_SERV_UUID_THERMOSTAT);

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_THERMO_INDP_A)
            devDriverBussiness_thermostatSwitch_devParam_get(&devParam);
            hap_char = hap_serv_get_char_by_uuid(hap_serv, HAP_CHAR_UUID_TARGET_HEATING_COOLING_STATE);
            appliance_value.i = devParam.deviceRunning_MODE;
            hap_char_update_val(hap_char, &appliance_value);
#else
            hap_char = hap_serv_get_char_by_uuid(hap_serv, HAP_CHAR_UUID_TARGET_HEATING_COOLING_STATE);
            (staVal->devType_thermostat.devThermostat_running_en)?
                (appliance_value.i = 3):
                (appliance_value.i = 0);
            hap_char_update_val(hap_char, &appliance_value);
#endif

            hap_char = hap_serv_get_char_by_uuid(hap_serv, HAP_CHAR_UUID_TARGET_TEMPERATURE);
            appliance_value.f = (float)staVal->devType_thermostat.devThermostat_tempratureTarget;
            hap_char_update_val(hap_char, &appliance_value);

            hap_char = hap_serv_get_char_by_uuid(hap_serv, HAP_CHAR_UUID_CURRENT_TEMPERATURE);
            appliance_value.f = devDriverBussiness_temperatureMeasure_get();
            if(appliance_value.f > 50.0F || appliance_value.f < -30.0)appliance_value.f = 0.0F;
            hap_char_update_val(hap_char, &appliance_value);

        }break;

        case devTypeDef_heater:{

            hap_acc = hap_acc_get_by_aid(1);
            hap_serv = hap_acc_get_serv_by_uuid(hap_acc, HAP_SERV_UUID_SWITCH);
            hap_char = hap_serv_get_char_by_uuid(hap_serv, HAP_CHAR_UUID_ON);
            (staVal->devType_heater.devHeater_swEnumVal == heaterOpreatAct_close)?
                (appliance_value.b = false):
                (appliance_value.b = true);
            hap_char_update_val(hap_char, &appliance_value); 

        }break;

        case devTypeDef_socket:{

            hap_acc = hap_acc_get_by_aid(1);
            hap_serv = hap_acc_get_serv_by_uuid(hap_acc, HAP_SERV_UUID_SWITCH);
            hap_char = hap_serv_get_char_by_uuid(hap_serv, HAP_CHAR_UUID_ON);
            (staVal->devType_socket.devSocket_opSw == 0)?
                (appliance_value.b = false):
                (appliance_value.b = true);
            hap_char_update_val(hap_char, &appliance_value); 
            
        }break;

        default:break;
    }
}

bool hkL8_nwkWifiConnected_statusGet(void){

    return nwkL8Hk_wifiConnected_flg;
}

void htAccessoryDeviceRegisterByEvt_trig(void){

    xEventGroupSetBits(xEventGp_htApplication, HTAPPLICATION_FLG_BITHOLD_ACCESSORY_REG);
}

void htAccessoryDeviceDataPeriodUploadByEvt_trig(void){

    xEventGroupSetBits(xEventGp_htApplication, HTAPPLICATION_FLG_BITHOLD_DATAUPLD_PERIOD);
}

void htAccessoryDeviceDataUploadTrigByEvt_trig(void){

    xEventGroupSetBits(xEventGp_htApplication, HTAPPLICATION_FLG_BITHOLD_DATAUPLD_TRIG);
}

void htAccessoryDeviceDataPeriodUploadBussiness_loopReales(void){

    static stt_timerLoop timerTrig = {15, 0};

    if(timerTrig.loopCounter < timerTrig.loopPeriod)timerTrig.loopCounter ++;
    else{

        timerTrig.loopCounter = 0;

        if(HAP_WAC_STANDBY_FLG){

            htAccessoryDeviceDataPeriodUploadByEvt_trig();
        }
    } 
}

void appLaunchHT_generate(void){

    esp_log_level_set(TAG, ESP_LOG_INFO);

    xEventGp_htApplication = xEventGroupCreate();

    xTaskCreate(bridge_thread_entry, BRIDGE_TASK_NAME, BRIDGE_TASK_STACKSIZE, NULL, BRIDGE_TASK_PRIORITY, NULL);
    xTaskCreate(threadProcess_htEventTrig, "htEvtHandle", 1024 * 6, NULL, 4, NULL);
}






