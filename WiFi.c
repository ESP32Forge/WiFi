/**
 * @file      WiFi.c
 * @authors   Álvaro Velasco García
 * @date      March 16, 2025
 *
 * @brief     This source file defines the functions to initialize and de-initialize
 *            the WiFi peripheral.
 */

/***************************************************************************************
 * Includes
 ***************************************************************************************/
#include <WiFi.h>
#include <Debug.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>

/***************************************************************************************
 * Defines
 ***************************************************************************************/

#if DEBUG_MODE_ENABLE == 1
  /* Tag to show traces in button BSP module. */
  #define TAG "CORE_WIFI"
#endif

/***************************************************************************************
 * Functions
 ***************************************************************************************/

WiFi_return WiFi_init(const wifi_mode_t mode, wifi_config_t config, 
  const event_handlers handlers)
{

  /* Check the given mode prameter and store the properly interface to use. */
  wifi_interface_t interface;
  switch(mode)
  {
    case WIFI_MODE_STA:
      interface = WIFI_IF_STA;
      break;
    case WIFI_MODE_AP:
      interface = WIFI_IF_AP;
      break;
    default:
      return CORE_WIFI_INVALID_WIFI_MODE_ERR; 
  }

  /* WiFi peripheral needs the non-volatile memory to operate. */
  esp_err_t ret = ESP_error_check(nvs_flash_init());
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
  {
    ESP_error_check(nvs_flash_erase());
    ret = ESP_error_check(nvs_flash_init());
  }

  if(ret != ESP_OK)
  {
    return CORE_WIFI_INIT_ERR;
  }

  /**  Initialize the WiFi peripheral and their mechanics. **/

  /* Initialize the TCP/IP stack. */
  if(ESP_error_check(esp_netif_init()) != ESP_OK)
  {
    return CORE_WIFI_INIT_ERR;
  }

  /* Initialize the internal loop of WiFi events handler. */
  if(ESP_error_check(esp_event_loop_create_default()) != ESP_OK)
  {
    return CORE_WIFI_INIT_ERR;
  }

  /* Initialize the WiFi as the mode selected with the default parameters. */
  switch(mode)
  {
    case WIFI_MODE_AP:
      esp_netif_create_default_wifi_ap();
      break;
    case WIFI_MODE_STA:
      esp_netif_create_default_wifi_sta();
      break;
    default:
      return CORE_WIFI_INVALID_WIFI_MODE_ERR; 
  }

  /* Copy the default WiFi configuration in a variable and initialize the
   * WiFi with it. 
   */
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  if(ESP_error_check(esp_wifi_init(&cfg)) != ESP_OK)
  {
    return CORE_WIFI_INIT_ERR;
  }

  /* If the given WiFi event handler is not NULL, register the function that
   * will handle the WiFi events. 
   */
  if(handlers.WiFi_event_handler != NULL)
  {
    if(ESP_error_check(esp_event_handler_register(WIFI_EVENT, 
                         handlers.WiFi_events_to_handle,
                         handlers.WiFi_event_handler , NULL)) != ESP_OK)
    {
      return CORE_WIFI_INIT_ERR;
    }
  }

  /* If the given IP event handler is not NULL, register the function that
   * will handle the IP events. 
   */
  if(handlers.IP_event_handler != NULL)
  {
    if(ESP_error_check(esp_event_handler_register(IP_EVENT, 
                         handlers.IP_events_to_handle,
                         handlers.IP_event_handler , NULL)) != ESP_OK)
    {
      return CORE_WIFI_INIT_ERR;
    }
  }

  /* Set station mode to the WiFi peripheral. */
  if(ESP_error_check(esp_wifi_set_mode(mode)) != ESP_OK)
  {
    return CORE_WIFI_INIT_ERR;
  }

  /* Set the configuration to the station. */
  if(ESP_error_check(esp_wifi_set_config(interface, &config)) != ESP_OK)
  {
    return CORE_WIFI_INIT_ERR;
  }
  
  /* Start WiFi in station mode. */
  if(ESP_error_check(esp_wifi_start()) != ESP_OK)
  {
    return CORE_WIFI_INIT_ERR;
  }

  return CORE_WIFI_OK;
}

WiFi_return de_init_WiFi(void)
{
  if(ESP_error_check(esp_wifi_stop()) != ESP_OK)
  {
    return CORE_WIFI_DE_INIT_ERR;
  }

  if(ESP_error_check(esp_wifi_deinit()) != ESP_OK)
  {
    return CORE_WIFI_DE_INIT_ERR;
  }
  
  return CORE_WIFI_OK;
}

inline WiFi_return core_WiFi_LOG(const WiFi_return ret)
{
  #if DEBUG_MODE_ENABLE == 1
    switch(ret)
    {
      #define WIFI_RETURN(enumerate)   \
        case enumerate:                \
          if(ret > 0)                  \
          {                            \
            ESP_LOGE(TAG, #enumerate); \
          }                            \
          else                         \
          {                            \
            ESP_LOGI(TAG, #enumerate); \
          }                            \
          break;       
        WIFI_RETURNS
      #undef WIFI_RETURN
      default:
        ESP_LOGE(TAG, "Undefined return.");
        break;
    }
  #endif
  return ret;
}
