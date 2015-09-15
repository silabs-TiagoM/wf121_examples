/**************************************************************************//**
 * @file
 * @brief Empty Project
 * @author Energy Micro AS
 * @version 3.20.2
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See 
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"  
 * for details. Before using this software for any purpose, you must agree to the 
 * terms of that agreement.
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "InitDevice.h"
#include "segmentlcd.h"
#include "gpiointerrupt.h"
#include "printSWO.h"
#include "em_int.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_usart.h"
#include "em_lcd.h"
#include "em_gpio.h"
#include "wifi_bglib.h"
//#include "bsp_trace.h"

#define LED0_SET 	GPIO_PinOutSet(gpioPortE, 2)
#define LED0_CLEAR 	GPIO_PinOutClear(gpioPortE, 2)
#define LED1_SET 	GPIO_PinOutSet(gpioPortE, 3)
#define LED1_CLEAR 	GPIO_PinOutClear(gpioPortE, 3)
#define NOTIFY		GPIO_PinInGet(gpioPortC, 0)

//Define BGLIB
BGLIB_DEFINE()

//Notification counters
//interrupt will increment notifications
//main loop reads messages and increments notifications_handled
volatile uint32_t notifications=0;
uint32_t notifications_handled=0;

//receive buffer
uint8_t rx_buf[BGLIB_MSG_MAXLEN];
//temporary buffer to receive data when sending commands
uint8_t tmp_buf[256];
uint8_t tmp_idx_w=0;
uint8_t tmp_idx_r=0;

// Set IP, Netmask and Gateway
ipv4 ip_address;
ipv4 netmask;
ipv4 gateway;
uint8_t SSID[] = {'G','e','c','k','o','+','W','F','1','2','1'};

// Booleans to keep track of WF121 configuration steps
bool ap_mode_started = false;

/**************************************************************************//**
 * @brief  SPI callback to be used by BGLib
 * @param[in]  len1  Data length for buffer data1
 * @param[in]  data1 First data buffer to be sent
 * @param[in]  len2  Data length for buffer data 2
 * @param[in]  data2 Second data buffer to be sent
 *****************************************************************************/
void spi_output(uint8 len1,uint8* data1,uint16 len2,uint8* data2) {

	//when outputting spi message we have to buffer incoming bytes and check their validity later
	while(len1--)
	{
		tmp_buf[tmp_idx_w++] = USART_SpiTransfer(USART1, *data1++);
	}

	while(len2--)
	{
		tmp_buf[tmp_idx_w++] = USART_SpiTransfer(USART1, *data2++);
	}
}

// SPI read
char spi_read(void)
{
	//read buffered messages first
    if(tmp_idx_w!=tmp_idx_r)
        return tmp_buf[tmp_idx_r++];

    //send 0's to get data from slave
    return USART_SpiTransfer(USART1, 0x00);
}

// Notify callback
void notify_callback(uint8_t pin)
{
	notifications++;
}

//Read header
int read_header(uint8_t *rx)
{
    int i;

    // read first byte
    *rx=spi_read();

    // if first byte is 0, not valid message. SPI interface uses 0 as padding byte
    if(*rx==0)
        return -1;

    // otherwise read the entire header
    rx++;

    for( i=0 ; i<BGLIB_MSG_HEADER_LEN-1 ; i++)
    {
        *rx++=spi_read();
    }

    return 0;
}

//read message from module
void run_state_machine()
{
    int n;
    struct wifi_cmd_packet *pck;

    uint8_t *rx=&rx_buf[BGLIB_MSG_HEADER_LEN];

    if(read_header(rx_buf))
        return ;//no message, return

    for(n=0 ; n<BGLIB_MSG_LEN(rx_buf); n++)
    {
    	//read payload
        *rx++=spi_read();
    }

    pck=BGLIB_MSG(rx_buf);
    //find message
    switch(BGLIB_MSG_ID(rx_buf))
    {
    // cases are shown in the sequence as the command responses and events should be received from the module
        case wifi_evt_system_boot_id:
			//module has booted, synchronize handled notifications to received interrupts
            notifications_handled=notifications;
            printf("System has booted\n");
            ap_mode_started = false;
            printf("Sending command: wifi_cmd_sme_set_operating_mode(2)\n");
            wifi_cmd_sme_set_operating_mode(2);
            return ;//return as no need to increment notifications handled

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
        	wifi_cmd_tcpip_start_tcp_server(8080,-1);
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

        default:
        	printf("Unknown Event\n");
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

	  /* Initialize GPIO interrupt dispatcher */
	  GPIOINT_Init();

	  /* Register callbacks before setting up and enabling pin interrupt. */
	  GPIOINT_CallbackRegister(0, notify_callback);

	  /* Set falling edge interrupt for PC0 which is connected to the WF121 notify pin */
	  GPIO_IntConfig(gpioPortC, 0, true, false, true);

	  // Initialize BGLib by passing the callback function pointer
	  BGLIB_INITIALIZE(spi_output);
}



/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{

  ip_address.a[0]=192; ip_address.a[1]=168; ip_address.a[2]=1; ip_address.a[3]=1;
  netmask.a[0]=255;	netmask.a[1]=255;	netmask.a[2]=255;	netmask.a[3]=0;
  gateway.a[0]=192; gateway.a[1]=168; gateway.a[2]=1; gateway.a[3]=1;

  init_device();

  /* Configures the SWO to output both printf-information, PC-samples and interrupt trace. */
  setupSWOForPrint();

  printf("Hello Word - This is Wonder Gecko + WF121 SPI Demo\n");

  // If the WF121 is already holding the notify line high let's look at what's up
  if(NOTIFY) run_state_machine();

  /* Infinite loop */
  while (1) {

	  if((notifications>notifications_handled)||(tmp_idx_w!=tmp_idx_r))
	  {
		  //if WF121 notifies of new messages, or we sent data and need to check for possible incoming messages
		  run_state_machine();

	  }
  }
}
