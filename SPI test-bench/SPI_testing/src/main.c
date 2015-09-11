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
#include "InitDevice.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_usart.h"
#include "wifi_bglib.h"

//Define BGLIB
BGLIB_DEFINE()

//temporary buffer to receive data when sending commands
uint8_t tmp_buf[256];
uint8_t tmp_idx_w=0;
//receive buffer
uint8_t rx_buf[BGLIB_MSG_HEADER_LEN];

// Function prototype
void spi_output(uint8 len1,uint8* data1,uint16 len2,uint8* data2);

// Function definition
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

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  ipv4 ip_address, netmask, gateway;
  uint8 SSID[] = {'G','e','c','k','o','+','W','F','1','2','1'};

  /* Chip errata */
  CHIP_Init();

  // All the peripheral initialization is done here: CMU, USART and GPIOs
  enter_DefaultMode_from_RESET();

  // Enable AUTOCS control for the USART master
  USART1->CTRL |= USART_CTRL_AUTOCS;

  // Initialize BGLib by passing the callback function pointer
  BGLIB_INITIALIZE(spi_output);

  wifi_cmd_hardware_io_port_config_direction(4,0x1,0);
  wifi_cmd_hardware_io_port_write(4,1,1);

  wifi_cmd_sme_set_operating_mode(2);

  wifi_cmd_sme_wifi_on();

  wifi_cmd_system_set_max_power_saving_state(0);


  // Set IP, Netmask and Gateway
  ip_address.a[0]=192; ip_address.a[1]=168; ip_address.a[2]=1; ip_address.a[3]=1;
  netmask.a[0]=0;	netmask.a[1]=255	;	netmask.a[2]=255;	netmask.a[3]=255;
  gateway.a[0]=192; gateway.a[1]=168; gateway.a[2]=1; gateway.a[3]=1;

  wifi_cmd_tcpip_configure(ip_address.u, netmask.u, gateway.u, 0);

  wifi_cmd_sme_start_ap_mode(11,0,11,SSID);



  /* Infinite loop */
  while (1) {

  }
}
