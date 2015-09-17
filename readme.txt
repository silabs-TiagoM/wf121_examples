This example project uses the Wonder Gecko STK + DKWF121 to create an Access Point with a TCP server to which clientes can connect and send data to. The data send by clients will be shown in the debug console output. The WF121 is controlled by the Wonder Gecko MCU through SPI interface.


HOW TO:

1 - Import the project to your Simplicity IDE workspace as instructed in this link:
http://community.silabs.com/t5/8-bit-MCU-Knowledge-Base/How-to-copy-and-import-a-project-in-Studio-2-0/ta-p/120798

2 - Flash the WF121 with the wf121_spi_slave hex or dfu files.

3 - Connect the Wonder Gecko STK and DKWF121 as follows:

|  WGSTK pins   |   DKWF121 pins  |
 ---------------------------------
           5V  <->  5V      
          GND  <->  GND
          PD0  <->  RD2   (MOSI)
          PD1  <->  RD3   (MISO)
          PD2  <->  RD1   (CLK)
          PD3  <->  RD9   (CS)
          PC0  <->  RD4   (NOTIFY)

The WGSTK pins listed here are available through the Expansion header and on the DFWF121 they are at the edges of the prototyping area.

4 - Make sure that the WGSTK Power Switch is on the DBG position and connect it to the PC through the J-link micro USB connector.

5 - Hit the debug button in the Simplicity IDE and run the firmware. You should start seeing messages in the debug console.

6 - Connect to the Wifi network Gecko+WF121, you should see the message "Client joined with MAC xx xx xx xx xx xx" in the debug console.

7 - Open Putty and connect to the IP 192.168.1.1 Port 8080 through Telnet. If you start typing in the Telnet terminal you should see the same data being printed in the debug console.

