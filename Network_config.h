/**
 * @file      Network_config.h
 * @authors   Álvaro Velasco García
 * @date      March 16, 2025
 *
 * @brief     File that declares macros to configure the network configuration.
 */

#ifndef NETWORK_CONFIG_H_
#define NETWORK_CONFIG_H_

/***************************************************************************************
 * Includes
 ***************************************************************************************/
#include <System_lights.h>
#include <stdint.h>

/***************************************************************************************
 * Defines
 ***************************************************************************************/

/* Defines the identifier of the network between the nodes of the ligh system. */
#define WIFI_SSID "ProtoPixelNetwork"

/* Defines the password of the ligth network. */
#define WIFI_PASS "TheMostInsaneAndSecurePass"

/* Defines the WiFi channel that the AP will use. (wifi_2g_channel_bit_t) */
#define WIFI_CHANNEL WIFI_CHANNEL_1

/* Defines the autentication mode of the WiFi (wifi_auth_mode_t) */
#define WIFI_AUTH_MODE WIFI_AUTH_WPA2_PSK 

/* Maximum number of stations that can be connected to the network. */
#define MAX_STA_CONN 1

/* Socket port that client and server will use stablish the comunciation. */
#define TCP_IP_PORT 7777

/* Data type of the TCP_IP commands. */
#define TCP_COMMAND_TYPE Command_frame

/* Size in bytes of the payload that the TCP_IP frame will have. */
#define TCP_COMMAND_SIZE sizeof(TCP_COMMAND_TYPE)

/***************************************************************************************
 * Data Type Definitions
 ***************************************************************************************/

/* Light effects that can the LEDs perform. */
typedef enum
{
  /* Light effect where it will set the LED in the opossite state that was before. */
  TOOGLE_LED,
  /* Last enumerate always, indicates the number of elements. Do not delete */
  NUM_OF_LIGHT_EFFECTS,
} Light_effect;

/* Structure that defines the frame format that will use the nodes to communicate the
 * commands.
 */
typedef struct
{
  /* Identifier of the LED that where the action/command will take effect. */
  LED_ID ID;
  /* Actions to set to the LED. */
  Light_effect action;
} Command_frame;

#endif /* NETWORK_CONFIG_H_ */