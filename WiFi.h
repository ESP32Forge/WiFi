/**
 * @file      WiFi.h
 * @authors   Álvaro Velasco García
 * @date      March 16, 2025
 *
 * @brief     This header file declares the functions to initialize and de-initialize
 *            the WiFi peripheral.
 */

#ifndef WIFI_H_
#define WIFI_H_

/***************************************************************************************
 * Includes
 ***************************************************************************************/
#include <Network_config.h>
#include <esp_netif_types.h>
#include <esp_wifi_types_generic.h>

/***************************************************************************************
 * Defines
 ***************************************************************************************/

/* List of the possible return codes that WiFi module can return. */
#define WIFI_RETURNS                           \
  /* Info codes */                             \
  WIFI_RETURN(CORE_WIFI_OK)                    \
  /* Error codes */                            \
  WIFI_RETURN(CORE_WIFI_INIT_ERR)              \ 
  WIFI_RETURN(CORE_WIFI_INVALID_WIFI_MODE_ERR) \ 
  WIFI_RETURN(CORE_WIFI_DE_INIT_ERR)                        
 
/***************************************************************************************
 * Data Type Definitions
 ***************************************************************************************/

/* Enumerate that lists the posible return codes that the module can return. */
typedef enum
{
  #define WIFI_RETURN(enumerate) enumerate,
    WIFI_RETURNS
  #undef WIFI_RETURN
  /* Last enumerate always, indicates the number of elements. Do not delete */
  NUM_OF_WIFI_RETURNS,
} WiFi_return;

/* Structure that will be used by WiFi_init function to set the WiFi and IP event
 * handlers.
 */
typedef struct
{
  /* Mask that indicates which WiFi events will handle the handler. */
  int WiFi_events_to_handle;
  /* Pointer to the function that will handle the WiFi events. */
  esp_event_handler_t WiFi_event_handler;
  /* Mask that indicates which IP events will handle the handler. */
  ip_event_t IP_events_to_handle;
  /* Pointer to the function that will handle the IP events. */
  esp_event_handler_t IP_event_handler;
} event_handlers;

/***************************************************************************************
 * Functions Prototypes
 ***************************************************************************************/

/**
 * @brief Initializes the WiFi peripheral.
 *
 * @param mode In which mode will operate the peripheral, only available 
 *             WIFI_MODE_STA or WIFI_MODE_AP.
 * 
 * @param config Structure that contains the configuration to set.
 * 
 * @param handlers Structure that contains the pointer to the functions that will handle
 *                 the WiFi and IP events. If you dont want to register a handler, set
 *                 the function pointer to NULL.
 *
 * @return CORE_WIFI_OK if the operation went well,
 *         otherwise:
 * 
 *           - CORE_WIFI_INVALID_WIFI_MODE_ERR:
 *               Invalid mode parameter value.
 * 
 *           - CORE_WIFI_INIT_ERR:
 *               An error ocurred in the intermediate function.
 *         
 */
WiFi_return WiFi_init(const wifi_mode_t mode, wifi_config_t config, 
  const event_handlers handlers);

/**
 * @brief De-initialize the WiFi peripheral.
 *
 * @param void
 *
 * @return CORE_WIFI_OK if the operation went well,
 *         otherwise:
 * 
 *           - CORE_WIFI_DE_INIT_ERR: 
 *               Something failed in the stop WiFi process.
 *      
 */
WiFi_return de_init_WiFi(void);

/**
 * @brief Prints the return of a WiFi module function if the system was configured 
 *        in debug mode.
 *
 * @param ret Received return from a WiFi module function.
 *
 * @return The given return.
 */
WiFi_return core_WiFi_LOG(const WiFi_return ret);

#endif /* WIFI_H_ */
   