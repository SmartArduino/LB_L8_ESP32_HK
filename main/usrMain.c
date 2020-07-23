/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "mdf_common.h"
#include "mwifi.h"
#include "mlink.h"
#include "mupgrade.h"

#include "mespnow.h"
#include "mconfig_blufi.h"
#include "mconfig_chain.h"

#include "os.h"

#include "tcpip_adapter.h"

//#include "light_driver.h"

#include "lcd_adapter.h"

#include "mqtt_client.h"

#include "QR_Encode.h"

/* lvgl includes */
#include "iot_lvgl.h"

/* lvgl test includes */
#include "lv_test_theme.h"

#include "devDriver_manage.h"

#include "bussiness_timerHard.h"
#include "bussiness_timerSoft.h"

#include "gui_businessHome.h"
#include "gui_businessMenu_linkageConfig.h"
#include "mechanical_bussinessOpreat.h"

#include "appLaunch_HT.h"

#include "dataTrans_remoteServer.h"
#include "dataTrans_localHandler.h"
#include "dataTrans_meshUpgrade.h"
#include "dataTrans_homeAssistantExServer.h"

#define LIGHT_TID                     (1)
#define LIGHT_RESTART_COUNT_RESET     (3)
#define LIGHT_RESTART_TIMEOUT_MS      (3000)
#define LIGHT_STORE_RESTART_COUNT_KEY "light_count"

#define EVENT_GROUP_TRIGGER_HANDLE     BIT0
#define EVENT_GROUP_TRIGGER_RECV       BIT1

LV_IMG_DECLARE(lanbonSealPic_A);
LV_IMG_DECLARE(lanbonSealPic_B);

LV_IMG_DECLARE(testPic);
LV_IMG_DECLARE(testPic_P);
LV_IMG_DECLARE(ttA);
LV_IMG_DECLARE(night_Sight);

extern EventGroupHandle_t xEventGp_devApplication;

extern stt_nodeDev_hbDataManage *listHead_nodeDevDataManage;

TaskHandle_t taskHandle_communicateTrigBussiness  = NULL;

uint16_t timeCounetr_mwifiReorganize = 0;
bool funcTrigFlg_mwifiReorganize = false;

static const unsigned int dataSaveLen_test = 1024;

static struct
{
    lv_img_header_t header;
    uint32_t data_size;
    uint8_t * data;
	
}testImg_data = {

	.header.always_zero = 0,
	.header.w = 120,
	.header.h = 30,
	.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
	.data_size = GUI_BUSSINESS_HOME_BTNTEXT_PIC_PIXEL_SIZE * LV_IMG_PX_SIZE_ALPHA_BYTE,
	.data = NULL,
};

const uint8_t sssssT[960 * LV_IMG_PX_SIZE_ALPHA_BYTE] = {

	0
};
	
/**
 * @brief The value of the cid corresponding to each attribute of the light
 */
enum light_cid {
    LIGHT_CID_STATUS            = 0,
    LIGHT_CID_HUE               = 1,
    LIGHT_CID_SATURATION        = 2,
    LIGHT_CID_VALUE             = 3,
    LIGHT_CID_COLOR_TEMPERATURE = 4,
    LIGHT_CID_BRIGHTNESS        = 5,
    LIGHT_CID_MODE              = 6,
};

enum light_status {
    LIGHT_STATUS_ONOFF000              = 0,
    LIGHT_STATUS_ONOFF001              = 1,
	LIGHT_STATUS_ONOFF010			   = 2,
	LIGHT_STATUS_ONOFF011 			   = 3,
	LIGHT_STATUS_ONOFF100			   = 4,
	LIGHT_STATUS_ONOFF101			   = 5,
	LIGHT_STATUS_ONOFF110			   = 6,
	LIGHT_STATUS_ONOFF111			   = 7,

    LIGHT_STATUS_SWITCH            = 8,
    LIGHT_STATUS_HUE               = 9,
    LIGHT_STATUS_BRIGHTNESS        = 10,
    LIGHT_STATUS_COLOR_TEMPERATURE = 11,
};

static const char *TAG                          = "lanbon_L8 - usrMain";
static TaskHandle_t g_root_write_task_handle    = NULL;
static EventGroupHandle_t g_event_group_trigger = NULL;

/* LVGL Object */
static lv_obj_t *chart = NULL;
static lv_obj_t *gauge = NULL;
static lv_chart_series_t *series = NULL;

static EventGroupHandle_t wifi_event_group;
const static int CONNECTED_BIT = BIT0;

static void littlevgl_demo(void);

static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            esp_wifi_connect();
            xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
            break;
        default:
            break;
    }
    return ESP_OK;
}

static void wifi_init_custom(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "LANBON_DEVELOP002",
            .password = "Lanbon22*#",
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
//    ESP_LOGI(TAG, "start the WIFI SSID:[%s]", CONFIG_WIFI_SSID);
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "Waiting for wifi");
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
}

static void littlevgl_usrApp(void){

	lvGui_businessInit();
}

static lv_res_t on_led_switch_toggled(lv_obj_t *sw)
{
    ESP_LOGI(TAG, "Hello");
    return LV_RES_OK;
}

static void littlevgl_demo(void)
{
    lv_obj_t *scr = lv_obj_create(NULL, NULL);
    lv_scr_load(scr);

    lv_theme_t *th = lv_theme_alien_init(100, NULL);
    lv_theme_set_current(th);

    lv_obj_t *tabview = lv_tabview_create(lv_scr_act(), NULL);

    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, SYMBOL_LOOP);
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, SYMBOL_HOME);
    lv_obj_t *tab3 = lv_tabview_add_tab(tabview, SYMBOL_SETTINGS);
    lv_tabview_set_tab_act(tabview, 1, false);

    chart = lv_chart_create(tab2, NULL);
    lv_obj_set_size(chart, 300, 150);
    lv_chart_set_point_count(chart, 20);
    lv_obj_align(chart, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(chart, (lv_chart_type_t)(LV_CHART_TYPE_POINT | LV_CHART_TYPE_LINE));
    lv_chart_set_series_opa(chart, LV_OPA_70);
    lv_chart_set_series_width(chart, 4);
    lv_chart_set_range(chart, 0, 100);
    series = lv_chart_add_series(chart, LV_COLOR_RED);

    static lv_color_t needle_colors[] = {LV_COLOR_RED};
    gauge = lv_gauge_create(tab1, NULL);
    lv_gauge_set_needle_count(gauge,
                              sizeof(needle_colors) / sizeof(needle_colors[0]), needle_colors);
    lv_obj_align(gauge, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_gauge_set_value(gauge, 0, 50);

    char name[10];
    int i;
    lv_obj_t *labels[3];
    lv_obj_t *switches[3];
    for (i = 0; i < 3; i++)
    {
        labels[i] = lv_label_create(tab3, NULL);
        sprintf(name, "LED%d", i + 1);
        lv_label_set_text(labels[i], name);
    }
    lv_obj_align(labels[0], NULL, LV_ALIGN_IN_TOP_MID, -40, 20);
    for (i = 1; i < 3; i++)
    {
        lv_obj_align(labels[i], labels[i - 1], LV_ALIGN_OUT_BOTTOM_MID, 0, 35);
    }
    for (i = 0; i < 3; i++)
    {
        switches[i] = lv_sw_create(tab3, NULL);
        lv_obj_align(switches[i], labels[i], LV_ALIGN_OUT_RIGHT_MID, 10, 0);
        lv_sw_set_action(switches[i], on_led_switch_toggled);
    }
}

static mdf_err_t wifi_init()
{
    mdf_err_t ret          = nvs_flash_init();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        MDF_ERROR_ASSERT(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    MDF_ERROR_ASSERT(ret);

    tcpip_adapter_init();
    MDF_ERROR_ASSERT(esp_event_loop_init(NULL, NULL));
    MDF_ERROR_ASSERT(esp_wifi_init(&cfg));
    MDF_ERROR_ASSERT(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
    MDF_ERROR_ASSERT(esp_wifi_set_mode(WIFI_MODE_STA));
    MDF_ERROR_ASSERT(esp_wifi_set_ps(WIFI_PS_NONE));
    MDF_ERROR_ASSERT(esp_mesh_set_6m_rate(false));
    MDF_ERROR_ASSERT(esp_wifi_start());

    return MDF_OK;
}

/**
 * @brief Timed printing system information
 */
static void show_system_info_timercb(void *timer)
{
    uint8_t primary                 = 0;
    wifi_second_chan_t second       = 0;
    mesh_addr_t parent_bssid        = {0};
    uint8_t sta_mac[MWIFI_ADDR_LEN] = {0};
    mesh_assoc_t mesh_assoc         = {0x0};
    wifi_sta_list_t wifi_sta_list   = {0x0};
	uint8_t mutualGroupInsert_temp[DEVICE_MUTUAL_CTRL_GROUP_NUM] = {0};
//	lv_mem_monitor_t lv_mon			= {0};

    esp_wifi_get_mac(ESP_IF_WIFI_STA, sta_mac);
    esp_wifi_ap_get_sta_list(&wifi_sta_list);
    esp_wifi_get_channel(&primary, &second);
    esp_wifi_vnd_mesh_get(&mesh_assoc);
    esp_mesh_get_parent_bssid(&parent_bssid);
	devMutualCtrlGroupInfo_groupInsertGet(mutualGroupInsert_temp);
//	lv_mem_monitor(&lv_mon);

    ESP_LOGI(TAG, 
    		 "System information, channel: %d, layer: %d, self mac: " MACSTR ", parent bssid: " MACSTR
             ", parent rssi: %d, mwifi connected(?):%d, node num: %d, nodeApp num: %d, free heap: %u, mutualGroupInsert:[%d]-[%d]-[%d].\n", 
             primary,
             esp_mesh_get_layer(), MAC2STR(sta_mac), MAC2STR(parent_bssid.addr),
             mesh_assoc.rssi, mwifi_is_connected(), esp_mesh_get_total_node_num(), 
             L8devHbDataManageList_nodeNumDetect(listHead_nodeDevDataManage), 
             esp_get_free_heap_size(),
             mutualGroupInsert_temp[0], mutualGroupInsert_temp[1], mutualGroupInsert_temp[2]);

//	MDF_LOGI("lv mem used:%6d(%3d%%), lv mem biggest free:%6d.\n",
//			 (int)lv_mon.total_size - (int)lv_mon.free_size,
//			 lv_mon.used_pct,
//			 (int)lv_mon.free_biggest_size);

//    for (int i = 0; i < wifi_sta_list.num; i++) {
//        MDF_LOGI("Child mac: " MACSTR, MAC2STR(wifi_sta_list.sta[i].mac));
//    }

#ifdef CONFIG_LIGHT_MEMORY_DEBUG

    if (!heap_caps_check_integrity_all(true)) {
        MDF_LOGE("At least one heap is corrupt");
    }

    mdf_mem_print_heap();
    mdf_mem_print_record();
#endif /**< CONFIG_LIGHT_MEMORY_DEBUG */
}

static void restart_count_erase_timercb(void *timer)
{
    if (!xTimerStop(timer, portMAX_DELAY)) {
        MDF_LOGE("xTimerStop timer: %p", timer);
    }

    if (!xTimerDelete(timer, portMAX_DELAY)) {
        MDF_LOGE("xTimerDelete timer: %p", timer);
    }

    mdf_info_erase(LIGHT_STORE_RESTART_COUNT_KEY);
    MDF_LOGD("Erase restart count");
}

static int restart_count_get()
{
    mdf_err_t ret             = MDF_OK;
    TimerHandle_t timer       = NULL;
    uint32_t restart_count    = 0;
    RESET_REASON reset_reason = rtc_get_reset_reason(0);

    mdf_info_load(LIGHT_STORE_RESTART_COUNT_KEY, &restart_count, sizeof(uint32_t));

    if (reset_reason != POWERON_RESET && reset_reason != RTCWDT_RTC_RESET) {
        restart_count = 0;
        MDF_LOGW("restart reason: %d", reset_reason);
    }

    /**< If the device restarts within the instruction time,
         the event_mdoe value will be incremented by one */
    restart_count++;
    ret = mdf_info_save(LIGHT_STORE_RESTART_COUNT_KEY, &restart_count, sizeof(uint32_t));
    MDF_ERROR_CHECK(ret != ESP_OK, ret, "Save the number of restarts within the set time");

    timer = xTimerCreate("restart_count_erase", LIGHT_RESTART_TIMEOUT_MS / portTICK_RATE_MS,
                         false, NULL, restart_count_erase_timercb);
    MDF_ERROR_CHECK(!timer, ret, "xTaskCreate, timer: %p", timer);

    xTimerStart(timer, 0);

    return restart_count;
}

static bool light_restart_is_exception()
{
    mdf_err_t ret                      = ESP_OK;
    ssize_t coredump_len               = 0;
    esp_partition_iterator_t part_itra = NULL;

    part_itra = esp_partition_find(ESP_PARTITION_TYPE_DATA,
                                   ESP_PARTITION_SUBTYPE_DATA_COREDUMP, NULL);
    MDF_ERROR_CHECK(!part_itra, false, "<%s> esp_partition_find fail", mdf_err_to_name(ret));

    const esp_partition_t *coredump_part = esp_partition_get(part_itra);
    MDF_ERROR_CHECK(!coredump_part, false, "<%s> esp_partition_get fail", mdf_err_to_name(ret));

    ret = esp_partition_read(coredump_part, sizeof(ssize_t), &coredump_len, sizeof(ssize_t));
    MDF_ERROR_CHECK(ret, false, "<%s> esp_partition_read fail", mdf_err_to_name(ret));

    if (coredump_len <= 0) {
        return false;
    }

    /**< erase all coredump partition */
    ret = esp_partition_erase_range(coredump_part, 0, coredump_part->size);
    MDF_ERROR_CHECK(ret, false, "<%s> esp_partition_erase_range fail", mdf_err_to_name(ret));

    return true;
}

static mdf_err_t get_network_config(mwifi_init_config_t *init_config, mwifi_config_t *ap_config)
{
    MDF_PARAM_CHECK(init_config);
    MDF_PARAM_CHECK(ap_config);

    mconfig_data_t *mconfig_data        = NULL;
    mconfig_blufi_config_t blufi_config = {
        .company_id = 0x02E5, /**< Espressif Incorporated */
        .tid        = LIGHT_TID,
    };

    MDF_ERROR_ASSERT(mconfig_chain_slave_init());

    /**
     * @brief Switch to master mode to configure the network for other devices
     */
    uint8_t sta_mac[6] = {0};
    MDF_ERROR_ASSERT(esp_wifi_get_mac(ESP_IF_WIFI_STA, sta_mac));
    sprintf(blufi_config.name, "LB8_%02X%02X%02X%02X%02X%02X", sta_mac[0], 
															   sta_mac[1],
															   sta_mac[2],
															   sta_mac[3],
															   sta_mac[4],
															   sta_mac[5]);
    MDF_LOGI("BLE name: %s", blufi_config.name);

    MDF_ERROR_ASSERT(mconfig_blufi_init(&blufi_config));

	lvGui_usrAppBussinessRunning_blockCancel();
	
    MDF_ERROR_ASSERT(mconfig_queue_read(&mconfig_data, portMAX_DELAY));
    MDF_ERROR_ASSERT(mconfig_chain_slave_deinit());
    MDF_ERROR_ASSERT(mconfig_blufi_deinit());

    memcpy(ap_config, &mconfig_data->config, sizeof(mwifi_config_t));
    memcpy(init_config, &mconfig_data->init_config, sizeof(mwifi_init_config_t));

    mdf_info_save("init_config", init_config, sizeof(mwifi_init_config_t));
    mdf_info_save("ap_config", ap_config, sizeof(mwifi_config_t));

    /**
     * @brief Switch to master mode to configure the network for other devices
     */
    if (mconfig_data->whitelist_size > 0) {
        for (int i = 0; i < mconfig_data->whitelist_size / sizeof(mconfig_whitelist_t); ++i) {
            MDF_LOGD("count: %d, data: " MACSTR,
                     i, MAC2STR((uint8_t *)mconfig_data->whitelist_data + i * sizeof(mconfig_whitelist_t)));
        }

        MDF_ERROR_ASSERT(mconfig_chain_master(mconfig_data, 60000 / portTICK_RATE_MS));
    }

    MDF_FREE(mconfig_data);

    return MDF_OK;
}

static mdf_err_t mlink_set_value(uint16_t cid, void *arg)
{
    int value = *((int *)arg);

    switch (cid) {
        case LIGHT_CID_STATUS:
			
            switch (value) {
				case  LIGHT_STATUS_ONOFF000:
				case  LIGHT_STATUS_ONOFF001:
				case  LIGHT_STATUS_ONOFF010:
				case  LIGHT_STATUS_ONOFF011:		
				case  LIGHT_STATUS_ONOFF100:		
				case  LIGHT_STATUS_ONOFF101:			 
				case  LIGHT_STATUS_ONOFF110:			
				case  LIGHT_STATUS_ONOFF111:{			

					stt_devDataPonitTypedef dataVal_set = {0};

					dataVal_set.devType_mulitSwitch_threeBit.swVal_bit1 = (uint8_t)value >> 0;
					dataVal_set.devType_mulitSwitch_threeBit.swVal_bit2 = (uint8_t)value >> 1;
					dataVal_set.devType_mulitSwitch_threeBit.swVal_bit3 = (uint8_t)value >> 2;
					currentDev_dataPointSet(&dataVal_set, true, true, true, false, true);

//					light_driver_set_switch(value);

				}break;

//                case LIGHT_STATUS_SWITCH:
//                    light_driver_set_switch(!light_driver_get_switch());
//                    break;

//                case LIGHT_STATUS_HUE: {
//                    uint16_t hue = light_driver_get_hue();
//                    hue = (hue + 60) % 360;

//                    light_driver_set_saturation(100);
//                    light_driver_set_hue(hue);
//                    break;
//                }

//                case LIGHT_STATUS_BRIGHTNESS: {
//                    if (light_driver_get_mode() == MODE_HSV) {
//                        uint8_t value = (light_driver_get_value() + 20) % 100;
//                        light_driver_set_value(value);
//                    } else {
//                        uint8_t brightness = (light_driver_get_brightness() + 20) % 100;
//                        light_driver_set_brightness(brightness);
//                    }

//                    break;
//                }

//                case LIGHT_STATUS_COLOR_TEMPERATURE: {
//                    uint8_t color_temperature = (light_driver_get_color_temperature() + 20) % 100;

//                    if (!light_driver_get_brightness()) {
//                        light_driver_set_brightness(30);
//                    }

//                    light_driver_set_color_temperature(color_temperature);

//                    break;
//                }

                default:
                    break;
            }

            break;

//        case LIGHT_CID_MODE:
//            switch (value) {
//                case MODE_BRIGHTNESS_INCREASE:
//                    light_driver_fade_brightness(100);
//                    break;

//                case MODE_BRIGHTNESS_DECREASE:
//                    light_driver_fade_brightness(0);
//                    break;

//                case MODE_HUE_INCREASE:
//                    light_driver_set_saturation(100);
//                    light_driver_fade_hue(360);
//                    break;

//                case MODE_HUE_DECREASE:
//                    light_driver_set_saturation(100);
//                    light_driver_fade_hue(0);
//                    break;

//                case MODE_WARM_INCREASE:
//                    if (!light_driver_get_brightness()) {
//                        light_driver_set_brightness(30);
//                    }

//                    light_driver_fade_warm(100);
//                    break;

//                case MODE_WARM_DECREASE:
//                    if (!light_driver_get_brightness()) {
//                        light_driver_set_brightness(30);
//                    }

//                    light_driver_fade_warm(0);
//                    break;

//                case MODE_NONE:
//                    light_driver_fade_stop();
//                    break;

//                default:
//                    break;
//            }

//            break;

//        case LIGHT_CID_HUE:
//            light_driver_set_hue(value);
//            break;

//        case LIGHT_CID_SATURATION:
//            light_driver_set_saturation(value);
//            break;

//        case LIGHT_CID_VALUE:
//            light_driver_set_value(value);
//            break;

//        case LIGHT_CID_COLOR_TEMPERATURE:
//            light_driver_set_color_temperature(value);
//            break;

//        case LIGHT_CID_BRIGHTNESS:
//            light_driver_set_brightness(value);
//            break;

        default:
            MDF_LOGE("No support cid: %d", cid);
            return MDF_FAIL;
    }

    MDF_LOGD("cid: %d, value: %d", cid, value);

    return MDF_OK;
}

static mdf_err_t mlink_get_value(uint16_t cid, void *arg)
{
//    int *value = (int *)arg;

//    switch (cid) {
//        case LIGHT_CID_STATUS:
//            *value = light_driver_get_switch();
//            break;

//        case LIGHT_CID_HUE:
//            *value = light_driver_get_hue();
//            break;

//        case LIGHT_CID_SATURATION:
//            *value = light_driver_get_saturation();
//            break;

//        case LIGHT_CID_VALUE:
//            *value = light_driver_get_value();
//            break;

//        case LIGHT_CID_COLOR_TEMPERATURE:
//            *value = light_driver_get_color_temperature();
//            break;

//        case LIGHT_CID_BRIGHTNESS:
//            *value = light_driver_get_brightness();
//            break;

//        case LIGHT_CID_MODE:
//            *value = light_driver_get_mode();
//            break;

//        default:
//            MDF_LOGE("No support cid: %d", cid);
//            return MDF_FAIL;
//    }

//    MDF_LOGV("cid: %d, value: %d", cid, *value);

    return MDF_OK;
}

static void root_write_task(void *arg)
{
    mdf_err_t ret = MDF_OK;
    char *data    = NULL;
    size_t size   = 0;
    uint8_t src_addr[MWIFI_ADDR_LEN] = {0x0};
    mwifi_data_type_t data_type      = {0};
	mlink_httpd_type_t *httpd_type 	 = NULL;

    MDF_LOGW("root_write_task is running");

    while (mwifi_is_connected() && esp_mesh_get_layer() == MESH_ROOT) {
        size = MWIFI_PAYLOAD_LEN * 4;
        MDF_FREE(data);
		ret = mwifi_root_read(src_addr, &data_type, &data, &size, portMAX_DELAY);
        MDF_ERROR_CONTINUE(ret != MDF_OK, "<%s> mwifi_root_read", mdf_err_to_name(ret));

        if (data_type.upgrade) {
            ret = mupgrade_root_handle(src_addr, data, size);
            MDF_ERROR_CONTINUE(ret != MDF_OK, "<%s> mupgrade_handle", mdf_err_to_name(ret));
            continue;
        }

		httpd_type = (mlink_httpd_type_t *)&(data_type.custom);
//		MDF_LOGD("root receive dataType:%d.\n", httpd_type->format);
		if(httpd_type->format == MLINK_HTTPD_FORMAT_HEX){

			uint8_t *dataRcv_temp = (uint8_t *)data;

//			printf("root read node cmd:%02X<<<<<.\n", dataRcv_temp[0]);

			if((dataRcv_temp[0] == L8DEV_MESH_HEARTBEAT_REQ) || //心跳截留
			   (dataRcv_temp[0] == L8DEV_MESH_CMD_DETAIL_INFO_REPORT) || //设备详情上报截留
			   (dataRcv_temp[0] == L8DEV_MESH_FWARE_UPGRADE) || //远程升级通知截留
			   (dataRcv_temp[0] == L8DEV_MESH_CMD_EPID_DATA_REQ) || //疫情反向请求截留
			   (dataRcv_temp[0] == L8DEV_MESH_CMD_DEVINFO_LIST_REQ)){ //mesh所有设备列表请求截留

				dataHandler_devNodeMeshData(src_addr, httpd_type, data, size);
			}
			else //非特定命令转发
			{
				mqtt_remoteDataTrans(dataRcv_temp[0], &dataRcv_temp[1], size - 1); //下标0是cmd，后边都是数据
			}

			continue;
		}

        MDF_LOGD("Root receive, addr: " MACSTR ", size: %d, data: %.*s",
                 MAC2STR(src_addr), size, size, data);

        switch (data_type.protocol) {
            case MLINK_PROTO_HTTPD: {
                mlink_httpd_t httpd_data  = {
                    .size       = size,
                    .data       = data,
                    .addrs_num  = 1,
                    .addrs_list = src_addr,
                };
                memcpy(&httpd_data.type, &data_type.custom, sizeof(httpd_data.type));

                ret = mlink_httpd_write(&httpd_data, portMAX_DELAY);
                MDF_ERROR_BREAK(ret != MDF_OK, "<%s> mlink_httpd_write", mdf_err_to_name(ret));

                break;
            }

            case MLINK_PROTO_NOTICE: {
                ret = mlink_notice_write(data, size, src_addr);
                MDF_ERROR_BREAK(ret != MDF_OK, "<%s> mlink_httpd_write", mdf_err_to_name(ret));
                break;
            }

            default:
                MDF_LOGW("Does not support the protocol: %d", data_type.protocol);
                break;
        }
    }

    MDF_LOGW("root_write_task is exit");

    MDF_FREE(data);
    g_root_write_task_handle = NULL;
    vTaskDelete(NULL);
}

void request_handle_task(void *arg)
{
    mdf_err_t ret = MDF_OK;
    uint8_t *data = NULL;
    size_t size   = MWIFI_PAYLOAD_LEN;
    mwifi_data_type_t data_type      = {0x0};
    uint8_t src_addr[MWIFI_ADDR_LEN] = {0x0};

    for (;;) {
        if (!mwifi_is_connected()) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
            continue;
        }

        size = MWIFI_PAYLOAD_LEN;
        MDF_FREE(data);
        ret = mwifi_read(src_addr, &data_type, &data, &size, portMAX_DELAY);
        MDF_ERROR_CONTINUE(ret != MDF_OK, "<%s> Receive a packet targeted to self over the mesh network",
                           mdf_err_to_name(ret));

        if (data_type.upgrade) {
            ret = mupgrade_handle(src_addr, data, size);
            MDF_ERROR_CONTINUE(ret != MDF_OK, "<%s> mupgrade_handle", mdf_err_to_name(ret));

			datatransOpreation_heartbeatHold_auto(); //心跳挂起
			tipsOpreatAutoSet_sysUpgrading();

            continue;
        }

		if(!strcmp(L8_MESH_CONMUNICATE_UPGSTBY_NOTICE_STR, (const char*)data)){

			if(esp_mesh_get_layer() != MESH_ROOT){

				lvGui_tipsFullScreen_generateAutoTime("upgrade stand by. . .");
			}

			datatransOpreation_heartbeatHold_auto(); //心跳挂起
			tipsOpreatAutoSet_sysUpgrading();
		}
		else
		if(!strcmp(L8_MESH_CONMUNICATE_RESTART_NOTICE_STR, (const char*)data)){

			stt_devStatusRecord devStatusRecordFlg_temp = {0};
		
			devStatusRecordIF_paramGet(&devStatusRecordFlg_temp);
			devStatusRecordFlg_temp.devUpgradeFirstRunning_FLG = 0; //复位首次运行标志
			devStatusRecordIF_paramSet(&devStatusRecordFlg_temp, true);

			lvGui_tipsFullScreen_generate("upgrade finish", 2);
			lvGui_systemRestartCountingDown_trig(3);
		}

        MDF_LOGI("Node receive, addr: " MACSTR ", size: %d, data: %.*s", MAC2STR(src_addr), size, size, data);

        mlink_httpd_type_t *httpd_type = (mlink_httpd_type_t *)&data_type.custom;

		if(httpd_type->format == MLINK_HTTPD_FORMAT_HEX){

			dataHandler_devNodeMeshData(src_addr, httpd_type, data, size);
		}
		else
		if(httpd_type->format ==  MLINK_HTTPD_FORMAT_JSON){

	        // ret = mlink_handle(src_addr, httpd_type, data, size);
	        // MDF_ERROR_CONTINUE(ret != MDF_OK, "<%s> mlink_handle", mdf_err_to_name(ret));			
		}

        if (httpd_type->from == MLINK_HTTPD_FROM_DEVICE) {
            data_type.protocol = MLINK_PROTO_NOTICE;
            ret = mwifi_write(NULL, &data_type, "status", strlen("status"), true);
            MDF_ERROR_CONTINUE(ret != MDF_OK, "<%s> mlink_handle", mdf_err_to_name(ret));
        }

		if(esp_mesh_get_layer() == MESH_ROOT){

			if((esp_mesh_get_total_node_num() > 1) &&
			   (L8devHbDataManageList_nodeNumDetect(listHead_nodeDevDataManage) == 0)){

				if(false == funcTrigFlg_mwifiReorganize){

//					uint32_t heartbeatPeriodVal_dynamic = esp_mesh_get_total_node_num() / 2 * 1000;
					uint32_t heartbeatPeriodVal_dynamic = 120 * 1000;		//重组核准时间直接改为120s

					funcTrigFlg_mwifiReorganize = true;
					timeCounetr_mwifiReorganize = heartbeatPeriodVal_dynamic;

					ESP_LOGW(TAG, "mwifi app abnormal, mwifi detect trig.\n");
				}
				else
				{
					if(0 == timeCounetr_mwifiReorganize){

						funcTrigFlg_mwifiReorganize = false;

						usrApplication_systemRestartTrig(3);

						ESP_LOGW(TAG, "user application trig, mwifi reorganize, restart!\n");
					}
				}
			}
			else
			{
				funcTrigFlg_mwifiReorganize = false;
				timeCounetr_mwifiReorganize = 0;
			}
		}
    }

    MDF_FREE(data);
    vTaskDelete(NULL);
}

/**
 * @brief All module events will be sent to this task in esp-mdf
 *
 * @Note:
 *     1. Do not block or lengthy operations in the callback function.
 *     2. Do not consume a lot of memory in the callback function.
 *        The task memory of the callback function is only 4KB.
 */
static mdf_err_t event_loop_cb(mdf_event_loop_t event, void *ctx){

	extern void lvGui_wifiConfig_bussiness_configComplete_tipsTrig(void);

    MDF_LOGI("event_loop_cb, event: 0x%x", event);
    mdf_err_t ret = MDF_OK;

    switch (event) {
        case MDF_EVENT_MWIFI_STARTED:
            MDF_LOGI("MESH is started");
            break;

        case MDF_EVENT_MWIFI_PARENT_CONNECTED:

			meshNetwork_connectReserve_IF_set(true);
			flgSet_gotRouterOrMeshConnect(true);
			deviceDatapointSynchronousReport_actionTrig(); //远程上线通知
			devDetailInfoUploadTrig(); //触发节点详情更新上传

			usrMeshApplication_rootFirstConNoticeTrig(); //若是root节点，触发一次mesh广播通知

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED)|\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_SOCKET)|\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_MOUDLE)

			devBeepTips_trig(4, 8, 100, 40, 2); //beeps提示
#else

			lvGui_usrAppBussinessRunning_block(2, "\nwifi connected", 5);  //UI阻塞提示，wifi连接成功
			lvGui_wifiConfig_bussiness_configComplete_tipsTrig();
#endif 
			
            MDF_LOGI("Parent is connected on station interface");
//            light_driver_breath_stop();
            break;

        case MDF_EVENT_MWIFI_PARENT_DISCONNECTED:

			flgSet_gotRouterOrMeshConnect(false);
		
            MDF_LOGI("Parent is disconnected on station interface");
            break;

        case MDF_EVENT_MWIFI_FIND_NETWORK: {
            MDF_LOGI("the root connects to another router with the same SSID");
            mwifi_config_t ap_config  = {0x0};
            wifi_second_chan_t second = 0;

            mdf_info_load("ap_config", &ap_config, sizeof(mwifi_config_t));
            esp_wifi_get_channel(&ap_config.channel, &second);
            mwifi_set_config(&ap_config);
            mdf_info_save("ap_config", &ap_config, sizeof(mwifi_config_t));
            break;
        }

        case MDF_EVENT_MWIFI_ROUTING_TABLE_ADD:
        case MDF_EVENT_MWIFI_ROUTING_TABLE_REMOVE: {
            MDF_LOGI("total_num: %d", esp_mesh_get_total_node_num());

            uint8_t sta_mac[MWIFI_ADDR_LEN] = {0x0};
            MDF_ERROR_ASSERT(esp_wifi_get_mac(ESP_IF_WIFI_STA, sta_mac));

            ret = mlink_notice_write("http", strlen("http"), sta_mac);
            MDF_ERROR_BREAK(ret != MDF_OK, "<%s> mlink_httpd_write", mdf_err_to_name(ret));
            break;
        }

        case MDF_EVENT_MWIFI_ROOT_GOT_IP:{
            MDF_LOGI("Root obtains the IP address");

	        tcpip_adapter_ip_info_t ip_info;
	        tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
			MDF_LOGI("<MESH_EVENT_ROOT_GOT_IP>sta ip: " IPSTR ", mask: " IPSTR ", gw: " IPSTR,
			IP2STR(&ip_info.ip),
			IP2STR(&ip_info.netmask),
			IP2STR(&ip_info.gw));

			flgSet_gotRouterOrMeshConnect(true);

            ret = mlink_notice_init();
            MDF_ERROR_BREAK(ret != MDF_OK, "<%s> mlink_notice_init", mdf_err_to_name(ret));

            uint8_t sta_mac[MWIFI_ADDR_LEN] = {0x0};
            MDF_ERROR_ASSERT(esp_wifi_get_mac(ESP_IF_WIFI_STA, sta_mac));

            ret = mlink_notice_write("http", strlen("http"), sta_mac);
            MDF_ERROR_BREAK(ret != MDF_OK, "<%s> mlink_httpd_write", mdf_err_to_name(ret));

            ret = mlink_httpd_start();
            MDF_ERROR_BREAK(ret != MDF_OK, "<%s> mlink_httpd_start", mdf_err_to_name(ret));

            if (!g_root_write_task_handle) {
                xTaskCreate(root_write_task, "root_write", 8 * 1024,
                            NULL, CONFIG_MDF_TASK_DEFAULT_PRIOTY, &g_root_write_task_handle);
            }

            mqtt_app_start();
            // mqtt_exHomeassistant_start();

//			//upgrade test
//			const uint8_t targetDev_address[MWIFI_ADDR_LEN] = MWIFI_ADDR_ANY;
//			esp_wifi_get_mac(ESP_IF_WIFI_STA, targetDev_address);
//			usrAppUpgrade_targetDevaddrSet(targetDev_address);
//			usrApp_firewareUpgrade_trig(true, (uint8_t)currentDev_typeGet());

            break;
        }

        case MDF_EVENT_MWIFI_ROOT_LOST_IP:
            MDF_LOGI("Root loses the IP address");

			flgSet_gotRouterOrMeshConnect(false);

			mqtt_app_stop();
            // mqtt_exHomeassistant_stop();

            ret = mlink_notice_deinit();
            MDF_ERROR_BREAK(ret != MDF_OK, "<%s> mlink_notice_deinit", mdf_err_to_name(ret));

            ret = mlink_httpd_stop();
            MDF_ERROR_BREAK(ret != MDF_OK, "<%s> mlink_httpd_stop", mdf_err_to_name(ret));
            break;

        case MDF_EVENT_MCONFIG_BLUFI_STA_DISCONNECTED:
//            light_driver_breath_start(128, 128, 0); /**< yellow blink */
            break;

        case MDF_EVENT_MCONFIG_BLUFI_STA_CONNECTED:
//            light_driver_breath_start(255, 128, 0); /**< orange blink */
            break;

        case MDF_EVENT_MCONFIG_BLUFI_FINISH:
        case MDF_EVENT_MCONFIG_CHAIN_FINISH:
//            light_driver_breath_start(0, 255, 0); /**< green blink */
            break;

        case MDF_EVENT_MUPGRADE_STARTED:
            MDF_LOGI("Enter upgrade mode");
//            light_driver_breath_start(0, 0, 128); /**< blue blink */
            vTaskDelay(3000 / portTICK_RATE_MS);
//            light_driver_breath_stop();
            break;

        case MDF_EVENT_MUPGRADE_STATUS: {

			char strTips[32] = {0};
			sprintf(strTips, "upgrade progress %d%%", (int)ctx);
			lvGui_tipsFullScreen_generate(strTips, 180);
			
            MDF_LOGI("The upgrade progress is: %d%%", (int)ctx);
            mwifi_data_type_t data_type = {
                .protocol = MLINK_PROTO_NOTICE,
            };
            ret = mwifi_write(NULL, &data_type, "ota_status", strlen("ota_status"), true);
            MDF_ERROR_BREAK(ret != MDF_OK, "<%s> mwifi_write", esp_err_to_name(ret));
            break;
        }

        case MDF_EVENT_MUPGRADE_FINISH:

			if(esp_mesh_get_layer() != MESH_ROOT){

				lvGui_tipsFullScreen_generateAutoTime("Upgrade completed \nwaiting for restart");
	            MDF_LOGI("Upgrade completed waiting for restart");
			}
//          light_driver_breath_start(0, 0, 255); /**< blue blink */
            break;

        case MDF_EVENT_MLINK_SYSTEM_RESET:
            MDF_LOGW("Erase information saved in flash and system restart");

            ret = mdf_info_erase(MDF_SPACE_NAME);
            MDF_ERROR_BREAK(ret != 0, "Erase the information");
			devSystemInfoLocalRecord_allErase(); //信息清空

            esp_restart();
            break;

        case MDF_EVENT_MLINK_SYSTEM_REBOOT:
            MDF_LOGW("Restart PRO and APP CPUs");
            esp_restart();
            break;

        case MDF_EVENT_MLINK_SET_STATUS:
            if (!g_event_group_trigger) {
                g_event_group_trigger = xEventGroupCreate();
            }

            xEventGroupSetBits(g_event_group_trigger, EVENT_GROUP_TRIGGER_HANDLE);
            break;

        case MDF_EVENT_MESPNOW_RECV:
            if ((int)ctx == MESPNOW_TRANS_PIPE_CONTROL) {
                xEventGroupSetBits(g_event_group_trigger, EVENT_GROUP_TRIGGER_RECV);
            }

            break;

        default:
            break;
    }

    return MDF_OK;
}

static void trigger_handle_task(void *arg)
{
    mdf_err_t ret = MDF_OK;

    if (!g_event_group_trigger) {
        g_event_group_trigger = xEventGroupCreate();
    }

    for (;;) {

		uint16_t nodeNum_temp = devMeshNodeNum_Get();
		static uint16_t nodeNum_record = 0;
	
		if(nodeNum_record != nodeNum_temp){

			nodeNum_record = nodeNum_temp;

			devDetailInfoUploadTrig(); //根节点变化，触发节点详情更新上传
		}
		
        EventBits_t uxBits = 0;

        if (uxBits & EVENT_GROUP_TRIGGER_HANDLE) {
            ret = mlink_trigger_handle(MLINK_COMMUNICATE_MESH);
            MDF_ERROR_CONTINUE(ret != MDF_OK, "<%s> mlink_trigger_handle", mdf_err_to_name(ret));
        }

		uxBits = xEventGroupWaitBits(xEventGp_devApplication, 
									 DEVAPPLICATION_FLG_BITHOLD_RESERVE,
									 pdTRUE,
									 pdFALSE,
									 2 / portTICK_RATE_MS);

		if(uxBits & DEVAPPLICATION_FLG_MQTT_LOGIN_NOTICE){ //mqtt通信触发：MQTT登录通知

//			ESP_LOGI(TAG, "mqtt login notice.\n");

			mqtt_rootDevLoginConnectNotice_trig();
			vTaskDelay(20 / portTICK_RATE_MS);
		}

		if(uxBits & DEVAPPLICATION_FLG_BITHOLD_HEARTBEAT){ //mqtt通信触发：心跳

//			ESP_LOGI(TAG, "heartbeat trig.\n");

			devHeartbeat_dataTrans_bussinessTrig();
			vTaskDelay(20 / portTICK_RATE_MS);
		}

		if(uxBits & DEVAPPLICATION_FLG_BITHOLD_MUTUALTRIG){ //mesh内部通信触发：互控

//			ESP_LOGI(TAG, "mutualCtrl trig.\n");

			funcation_usrAppMutualCtrlActionTrig(); 
			vTaskDelay(20 / portTICK_RATE_MS);
		}

		if(uxBits & DEVAPPLICATION_FLG_BITHOLD_DEVEKECSUM_REPORT){ //mqtt通信触发：电量上报

//			ESP_LOGI(TAG, "decElecsum info report trig.\n");

			mqtt_rootDevRemoteDatatransLoop_elecSumReport();
			vTaskDelay(20 / portTICK_RATE_MS);
		}

		if(uxBits & DEVAPPLICATION_FLG_BITHOLD_ALLDEVDINFO_REPORT){ //mqtt通信触发：所有设备详细信息上报

//			ESP_LOGI(TAG, "decElecsum info report trig.\n");

			dataTransBussiness_allNodeDetailInfoReport();
			vTaskDelay(20 / portTICK_RATE_MS);
		}

		if(uxBits & DEVAPPLICATION_FLG_BITHOLD_DEVDRV_SCENARIO){ //mqtt通信触发：场景

			ESP_LOGI(TAG, "scenario driver action trig.\n");

			devDriverBussiness_scnarioSwitch_actionTrig();
			vTaskDelay(20 / portTICK_RATE_MS);
		}

		if(uxBits & DEVAPPLICATION_FLG_BITHOLD_DEVSTATUS_SYNCHRO){ //mqtt通信触发：状态主动同步

//			ESP_LOGI(TAG, "devStatus synchro trig.");
			
			usrApp_deviceStatusSynchronousInitiative();
			vTaskDelay(20 / portTICK_RATE_MS);
		}	

		if(uxBits & DEVAPPLICATION_FLG_DEVNODE_UPGRADE_CHECK){ //mqtt通信触发：固件升级是否可用检查

			devFireware_upgradeReserveCheck();
			vTaskDelay(20 / portTICK_RATE_MS);
		}

		if(uxBits & DEVAPPLICATION_FLG_DEVNODE_UPGRADE_REQUEST){ //mdf内部通信触发：子设备固件升级请求通知

			mwifiApp_firewareUpgrade_nodeNoticeToRoot();
			vTaskDelay(20 / portTICK_RATE_MS);
		}

		if(uxBits & DEVAPPLICATION_FLG_HOST_UPGRATE_TASK_CREAT){ //mdf内部通信触发：升级业务启动

			upgradeMeshOTA_taskCreatAction();
			vTaskDelay(20 / portTICK_RATE_MS);
		}

		if(uxBits & DEVAPPLICATION_FLG_SERVER_CFGPARAM_CHG){ //mdf内部通信触发：mqtt服务器切换

			dtRmoteServer_serverSwitchByDefault();
			vTaskDelay(20 / portTICK_RATE_MS);
		}

		if(uxBits & DEVAPPLICATION_FLG_DEVLOCAL_SUPER_CTRL){ //mdf内部通信触发：超级控制

			dataTransBussiness_pageLinkageCfg_superCtrlActivityFunction();
			vTaskDelay(20 / portTICK_RATE_MS);
		}

		if(uxBits & DEVAPPLICATION_FLG_DEVLOCAL_SUPER_SYCN){ //mdf内部通信触发：超级同步

			dataTransBussiness_pageLinkageCfg_superSycnActivityFunction();
			vTaskDelay(20 / portTICK_RATE_MS);
		}

		if(uxBits & DEVAPPLICATION_FLG_DEVINFO_LIST_REQ){ //mdf内部通信触发：设备列表请求

			deviceDetailInfoListRequest_bussinessTrig();
			vTaskDelay(20 / portTICK_RATE_MS);
		}

#if(SCREENSAVER_RUNNING_ENABLE == 1)

		if(uxBits & DEVAPPLICATION_FLG_EPIDEMIC_DATA_REQ){

			mqtt_rootDevEpidemicDataReq_trig();
			vTaskDelay(20 / portTICK_RATE_MS);
		}
#endif

		vTaskDelay(20 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

static void developer_debugTask(void *arg){

//    gpio_config_t io_conf;
//	
//    //disable interrupt
//    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
//    //set as output mode
//    io_conf.mode = GPIO_MODE_OUTPUT;
//    //disable pull-down mode
//    io_conf.pull_down_en = 0;
//    //disable pull-up mode
//    io_conf.pull_up_en = 0;
//    //bit mask of the pins that you want to set
//    io_conf.pin_bit_mask = GPIO_SEL_16 | GPIO_SEL_17 | GPIO_SEL_32;
//    //configure GPIO with the given settings
//    gpio_config(&io_conf);

//    //set as output mode
//    io_conf.mode = GPIO_MODE_INPUT;
//    //bit mask of the pins that you want to set
//    io_conf.pin_bit_mask = GPIO_SEL_26;
//    //configure GPIO with the given settings
//    gpio_config(&io_conf);

//	for(;;){

//		if(gpio_get_level(GPIO_NUM_26)){

//			gpio_set_level(GPIO_NUM_16, 0);
//			gpio_set_level(GPIO_NUM_17, 0);
//			gpio_set_level(GPIO_NUM_32, 0);

//		}else{

//			gpio_set_level(GPIO_NUM_16, 1);
//			gpio_set_level(GPIO_NUM_17, 1);
//			gpio_set_level(GPIO_NUM_32, 1);
//		}

//		vTaskDelay(100 / portTICK_PERIOD_MS);

//		gpio_set_level(GPIO_NUM_16, 0);
//		gpio_set_level(GPIO_NUM_17, 0);
//		gpio_set_level(GPIO_NUM_32, 0);
//		vTaskDelay(500 / portTICK_PERIOD_MS);

//		gpio_set_level(GPIO_NUM_16, 1);
//		gpio_set_level(GPIO_NUM_17, 1);
//		gpio_set_level(GPIO_NUM_32, 1);
//		vTaskDelay(500 / portTICK_PERIOD_MS);
//	}

//	deviceHardwareAcoustoOptic_Init();
//	bussiness_devLight_testApp();

	vTaskDelete(NULL);
}

static void usrApplication_logConfigInit(void){

/**
 * @brief Set the log level for serial port printing.
 */
// 	esp_log_level_set("*", ESP_LOG_INFO);
	esp_log_level_set("*", ESP_LOG_WARN);
//	esp_log_level_set("lanbon_L8 - timerSoft", ESP_LOG_DEBUG);
//	esp_log_level_set(TAG, ESP_LOG_DEBUG);

	esp_log_level_set("gpio", 			ESP_LOG_INFO);
	esp_log_level_set("mupgrade_root", 	ESP_LOG_INFO);
	esp_log_level_set("wifi", 			ESP_LOG_WARN);
	esp_log_level_set("mesh", 			ESP_LOG_WARN);
}

void app_main()
{
    char name[32]                       = {0};
    uint8_t sta_mac[6]                  = {0};
    mwifi_config_t ap_config            = {0x0};
    mwifi_init_config_t init_config     = {0x0};

	/**
	* @brief Log配置
	*/
	usrApplication_logConfigInit();

//	ESP_LOGW("USR test", "lv_style_t size:%d", sizeof(lv_style_t));

#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_MOUDLE) //为了读取拨码数据，提前初始化驱动
	devMechanicalOpreatPeriphInit();
#endif

//	devSystemInfoLocalRecord_preSaveTest();
	devSystemInfoLocalRecord_initialize();

	deviceHardwareAcoustoOptic_Init();
	devDriverManageBussiness_initialition();
	usrApp_bussinessHardTimer_Init();
	usrApp_bussinessSoftTimer_Init();

    /**
     * @brief Initialize LittlevGL GUI or led tips
     */
#if(L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_INFRARED) ||\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_SOCKET) ||\
   (L8_DEVICE_TYPE_PANEL_DEF == DEV_TYPES_PANEL_DEF_INDEP_MOUDLE)

	devOpreation_bussinessInit();
	vTaskDelay(100 / portTICK_PERIOD_MS);
#else

	lvgl_init();
	vTaskDelay(100 / portTICK_PERIOD_MS);
	littlevgl_usrApp();
	vTaskDelay(5000 / portTICK_PERIOD_MS);
#endif

    appLaunchHT_generate();

    xTaskCreate(trigger_handle_task, "trigger_handle", 1024 * 8,  NULL, 4, &taskHandle_communicateTrigBussiness);

    TimerHandle_t timer = xTimerCreate("show_system_info", 10000 / portTICK_RATE_MS,
                                       true, NULL, show_system_info_timercb);

    xTimerStart(timer, 0);
}
