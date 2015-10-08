#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "printSWO.h"
#include "em_int.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_usart.h"
#include "em_lcd.h"
#include "em_gpio.h"
#include "gpiointerrupt.h"
#include "InitDevice.h"
#include "wifi_bglib.h"

// Define BGLIB as indicated in wifi_bglib.h
BGLIB_DEFINE()

/* Notification counters
 * A GPIO interrupt caused by the NOTIFY line will increment notifications.
 * Main loop reads messages and increments notifications_handled. */
volatile uint32_t notifications=0;
uint32_t notifications_handled=0;

// Receive buffer
uint8_t rx_buf[BGLIB_MSG_MAXLEN];

// Temporary buffer to receive data when sending commands
uint8_t tmp_buf[256];
uint8_t tmp_idx_w=0;
uint8_t tmp_idx_r=0;

// IP, Netmask, Gateway and SSID
ipv4 ip_address = {.a[0] = 192, .a[1] = 168, .a[2] = 1, .a[3] = 1};
ipv4 netmask = {.a[0] = 255, .a[1] = 255, .a[2] = 255, .a[3] = 0};
ipv4 gateway = {.a[0] = 192, .a[1] = 168, .a[2] = 1, .a[3] = 1};
uint8_t SSID[] = {'G','e','c','k','o','+','W','F','1','2','1'};
uint8_t tcp_message[] = "Hello this is Gecko\n\r";

// Booleans to prevent ap mode from being issued more than once
bool ap_mode_started = false;

/**************************************************************************//**
 * @brief  Notify callback.
 *
 * This function is called when the NOTIFY line transitions from low to high and
 * it increments the notifications counter. It is a callback registered through
 * the GPIOINT API. Pin parameter is not used for our use as we have only enabled
 * interrupts in one pin.
 ****************************************************************************/
void notify_callback(uint8_t pin)
{
	notifications++;
}

/**************************************************************************//**
 * @brief  SPI callback to be used by BGLib.
 * This function must be given as the parameter for BGLIB_INITIALIZE at the
 * beginning of main.
 *
 * @param[in]  len1  Data length for buffer data1
 * @param[in]  data1 First data buffer to be sent
 * @param[in]  len2  Data length for buffer data 2
 * @param[in]  data2 Second data buffer to be sent
 *****************************************************************************/
void spi_output(uint8 len1,uint8* data1,uint16 len2,uint8* data2) {

	// When outputting spi message we have to buffer incoming bytes to check their validity later
	while(len1--)
	{
		tmp_buf[tmp_idx_w++] = USART_SpiTransfer(USART1, *data1++);
	}

	while(len2--)
	{
		tmp_buf[tmp_idx_w++] = USART_SpiTransfer(USART1, *data2++);
	}
}

/**************************************************************************//**
 * @brief  Function used to read SPI data.
 *
 * This function will first return buffered bytes and if there aren't any it
 * send a 0 to the slave device and returns the received byte.
 *
 * @return Returns one byte from either the buffer or SPI bus.
 *****************************************************************************/
char spi_read(void)
{
	// Read buffered messages first
    if(tmp_idx_w!=tmp_idx_r)
        return tmp_buf[tmp_idx_r++];

    //send 0x00 to clock out data from slave
    return USART_SpiTransfer(USART1, 0x00);
}

/**************************************************************************//**
 * @brief  Function used to read the BGAPI message header.
 * @param[out] rx  Pointer used to store the data
 * @return Returns -1 if there was nothing to read and 0 if it read the header successfully
 *****************************************************************************/
int read_header(uint8_t *rx)
{
    int i;

    // Read first byte
    *rx=spi_read();

    // If first byte is 0, not valid message. SPI interface uses 0 as padding byte
    if(*rx==0)
        return -1;

    // Otherwise read the entire header
    rx++;

    for( i=0 ; i<BGLIB_MSG_HEADER_LEN-1 ; i++)
    {
        *rx++=spi_read();
    }

    return 0;
}

/**************************************************************************//**
 * @brief  Function used to read the BGAPI message payload.
 * @param[out] rx  Pointer used to store the data
 *****************************************************************************/
void read_payload(uint8_t *rx)
{
	for(int n=0 ; n<BGLIB_MSG_LEN(rx_buf); n++)
	{
		// Read payload
		*rx++=spi_read();
	}
}


/**************************************************************************//**
 * @brief  Function used to handle the wf121.
 * This function configures WF121 into an AP and sets up a TCP server.
 *****************************************************************************/
void wf121_run(void)
{
	// Buffer pointer to read payload
    uint8_t *rx = &rx_buf[BGLIB_MSG_HEADER_LEN];

    // Pointer to wifi packet
    struct wifi_cmd_packet* pck;

    if(read_header(rx_buf))
    	// Return if there was no message
        return ;

    // Otherwise read payload
    read_payload(rx);

    pck = BGLIB_MSG(rx_buf);

    // Parse the ID
    switch(BGLIB_MSG_ID(rx_buf))
    {
    	// Cases are shown in the sequence as the command responses and events should be received from the module
        case wifi_evt_system_boot_id:
			// Module has booted, synchronize handled notifications to received interrupts
            notifications_handled=notifications;
            printf("System has booted\n");
            ap_mode_started = false;
            printf("Sending command: wifi_cmd_sme_set_operating_mode(2)\n");
            wifi_cmd_sme_set_operating_mode(2);
            // Return as no need to increment notifications handled
            return ;

        case wifi_rsp_sme_set_operating_mode_id:
        	printf("Command response received: wifi_rsp_sme_set_operating_mode_id\n");
        	printf("Sending command: wifi_cmd_sme_wifi_on\n");
        	wifi_cmd_sme_wifi_on();
        	break;

        case wifi_rsp_sme_wifi_on_id:
        	printf("Command response received: wifi_rsp_sme_wifi_on_id\n");
        	break;

        case wifi_evt_sme_wifi_is_on_id:
        	printf("Event received: wifi_evt_sme_wifi_is_on\n");
        	printf("Sending command: wifi_cmd_tcpip_configure\n");
            wifi_cmd_tcpip_configure(ip_address.u, netmask.u, gateway.u, 0);
            break;

        case wifi_rsp_tcpip_configure_id:
        	printf("Command response received: wifi_rsp_tcpip_configure_id\n");
        	break;

        case wifi_evt_tcpip_configuration_id:
        	/* This event is triggered by other commands such as start_ap_mode command.
        	 * The ap_mode_started bool prevents it from being issued more than once. */
        	printf("Event received: wifi_evt_tcpip_configuration_id\n");
        	if(!ap_mode_started) {
        		printf("Sending command: wifi_cmd_sme_start_ap_mode\n");
        		wifi_cmd_sme_start_ap_mode(11,0,11,SSID);
        	}
        	break;

        case wifi_rsp_sme_start_ap_mode_id:
        	printf("Command response received: wifi_rsp_sme_start_ap_mode_id\n");
        	break;

        case wifi_evt_sme_ap_mode_started_id:
        	printf("Event received: wifi_evt_sme_ap_mode_started_id\n");
        	ap_mode_started = true;
        	printf("Sending command: wifi_cmd_https_enable\n");
        	wifi_cmd_https_enable(0, 1, 0);
        	break;

        case wifi_rsp_https_enable_id:
        	printf("Command response received: wifi_rsp_https_enable_id\n");
        	printf("Sending command: wifi_cmd_tcpip_start_tcp_server\n");
        	wifi_cmd_tcpip_start_tcp_server(8080,-1);
        	break;

        case wifi_rsp_tcpip_start_tcp_server_id:
        	printf("Command response received: wifi_rsp_tcpip_start_tcp_server_id\n");
        	break;

        case wifi_evt_tcpip_endpoint_status_id:
        	if(pck->evt_tcpip_endpoint_status.local_ip.u != 0)
        	{
        		printf("Session opened with IP %03d.%03d.%03d.%03d on endpoint %d\n",
        				pck->evt_tcpip_endpoint_status.remote_ip.a[0],
        				pck->evt_tcpip_endpoint_status.remote_ip.a[1],
        				pck->evt_tcpip_endpoint_status.remote_ip.a[2],
        				pck->evt_tcpip_endpoint_status.remote_ip.a[3],
        				pck->evt_tcpip_endpoint_status.endpoint
        				);

        		wifi_cmd_endpoint_send(pck->evt_tcpip_endpoint_status.endpoint, 21, tcp_message);
        	}
        	break;

        case wifi_evt_endpoint_closing_id:
        	printf("Session on endpoint %d closed\n",pck->evt_endpoint_closing.endpoint);

        	break;

        case wifi_evt_sme_ap_client_joined_id:
        	printf("Client joined with MAC %02X %02X %02X %02X %02X %02X\n",
        			pck->evt_sme_ap_client_joined.mac_address.addr[0],
        			pck->evt_sme_ap_client_joined.mac_address.addr[1],
        			pck->evt_sme_ap_client_joined.mac_address.addr[2],
        			pck->evt_sme_ap_client_joined.mac_address.addr[3],
        			pck->evt_sme_ap_client_joined.mac_address.addr[4],
        			pck->evt_sme_ap_client_joined.mac_address.addr[5]
				   );
        	break;

        case wifi_evt_sme_ap_client_left_id:
			printf("Client left with MAC %02X %02X %02X %02X %02X %02X\n",
					pck->evt_sme_ap_client_joined.mac_address.addr[0],
					pck->evt_sme_ap_client_joined.mac_address.addr[1],
					pck->evt_sme_ap_client_joined.mac_address.addr[2],
					pck->evt_sme_ap_client_joined.mac_address.addr[3],
					pck->evt_sme_ap_client_joined.mac_address.addr[4],
					pck->evt_sme_ap_client_joined.mac_address.addr[5]
				   );
			break;

        case wifi_evt_endpoint_data_id:
        	for(int n=0; n<BGLIB_MSG(rx_buf)->evt_endpoint_data.data.len; n++) {
        		printf("%c",(char)BGLIB_MSG(rx_buf)->evt_endpoint_data.data.data[n]);
        	}
        	break;

        default:
            break;
    }

	notifications_handled++;
}

/**************************************************************************//**
 * @brief  Device initialization function.
 * This function initializes the peripherals and the BGLib
 *****************************************************************************/
void init_device(void)
{
	  // Chip errata
	  CHIP_Init();

	  // All the peripheral initialization is done here: CMU, USART and GPIOs
	  enter_DefaultMode_from_RESET();

	  // Enable AUTOCS control for the USART master (not done in the previous function)
	  USART1->CTRL |= USART_CTRL_AUTOCS;

	  // Initialize GPIO interrupt dispatcher
	  GPIOINT_Init();

	  // Register callbacks before setting up and enabling pin interrupt
	  GPIOINT_CallbackRegister(0, notify_callback);

	  // Set falling edge interrupt for PC0 which is connected to the WF121 notify pin
	  GPIO_IntConfig(gpioPortC, 0, true, false, true);

	  // Initialize BGLib by passing the callback function pointer
	  BGLIB_INITIALIZE(spi_output);
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  init_device();

  // Configures the SWO to output printf-information in the debug console
  setupSWOForPrint();

  // This will be printed in the Simplicity IDE debug console
  printf("Hello Word - This is Wonder Gecko + WF121 SPI Demo\n");

  // Reset the WF121 to start from a known state
  wifi_cmd_system_reset(0);

  // Infinite loop
  while (1) {

	  if((notifications>notifications_handled)||(tmp_idx_w!=tmp_idx_r))
	  {
		  // If WF121 notifies of new messages, or we sent data and need to check for possible incoming messages
		  wf121_run();

	  }
  }
}
