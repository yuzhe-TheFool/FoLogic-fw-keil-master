/*
 * This file is part of the FoLogic-firmware project.
 *
 * Copyright (C) 2026 the fool
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "include/fx2.h"                            
#include "include/fx2regs.h"                        
#include "include/syncdly.h"     // SYNCDELAY macro 

#define GPIFTRIGWR 0
#define GPIFTRIGRD 4

#define GPIF_EP2 0
#define GPIF_EP4 1
#define GPIF_EP6 2
#define GPIF_EP8 3

BOOL capturing = FALSE;
BOOL pktsent = FALSE;

const char xdata WaveData[128] =     
{                                      
// Wave 0 
/* LenBr */ 0x08,     0x02,     0x93,     0x38,     0x01,     0x01,     0x01,     0x07,
/* Opcode*/ 0x01,     0x01,     0x03,     0x01,     0x00,     0x00,     0x00,     0x00,
/* Output*/ 0x03,     0x03,     0x01,     0x03,     0x01,     0x03,     0x03,     0x03,
/* LFun  */ 0x87,     0x36,     0xF0,     0x3F,     0x00,     0x00,     0x00,     0x3F,
// Wave 1 
///* LenBr */ 0xF0,     0x01,     0x3F,     0x01,     0x01,     0x01,     0x00,     0x07,
/* LenBr */ 0xE8,     0x01,     0x3F,     0x01,     0x01,     0x00,     0x00,     0x07,
/* Opcode*/ 0x07,     0x06,     0x01,     0x00,     0x00,     0x00,     0x00,     0x00,
/* Output*/ 0x00,     0x00,     0x02,     0x02,     0x02,     0x02,     0x02,     0x02,
/* LFun  */ 0x6E,     0x00,     0x2D,     0x00,     0x00,     0x00,     0x00,     0x3F,
// Wave 2 
/* LenBr */ 0x01,     0x01,     0x01,     0x01,     0x01,     0x01,     0x01,     0x07,
/* Opcode*/ 0x00,     0x00,     0x00,     0x00,     0x00,     0x00,     0x00,     0x00,
/* Output*/ 0x06,     0x06,     0x06,     0x06,     0x06,     0x06,     0x06,     0x06,
/* LFun  */ 0x00,     0x00,     0x00,     0x00,     0x00,     0x00,     0x00,     0x3F,
// Wave 3 
/* LenBr */ 0x01,     0x01,     0x01,     0x01,     0x01,     0x01,     0x01,     0x07,
/* Opcode*/ 0x00,     0x00,     0x00,     0x00,     0x00,     0x00,     0x00,     0x00,
/* Output*/ 0x06,     0x06,     0x06,     0x06,     0x06,     0x06,     0x06,     0x06,
/* LFun  */ 0x00,     0x00,     0x00,     0x00,     0x00,     0x00,     0x00,     0x3F,
};                     

void setup_gpif_waveforms(void)
{
  BYTE i;

  // use dual autopointer feature... 
  AUTOPTRSETUP = 0x07;          // inc both pointers, 
                                // ...warning: this introduces pdata hole(s)
                                // ...at E67B (XAUTODAT1) and E67C (XAUTODAT2)

  // source
  AUTOPTRH1 = MSB( &WaveData );
  AUTOPTRL1 = LSB( &WaveData );
  
  // destination
  AUTOPTRH2 = 0xE4;
  AUTOPTRL2 = 0x00;
 
  // transfer
  for ( i = 0x00; i < 128; i++ )
  {
    EXTAUTODAT2 = EXTAUTODAT1;
  }
}

void init_capture_intf(void)
{
  IFCONFIG = 0xb6;
  // 7	IFCLKSRC=1   , FIFOs executes on internal clk source
  // 6	xMHz=0       , 30MHz internal clk rate
  // 5	IFCLKOE=1    , Drive IFCLK pin signal at 30MHz
  // 4	IFCLKPOL=1   , Invert IFCLK pin signal from internal clk
  // 3	ASYNC=0      , master samples synchronously
  // 2	GSTATE=1     , Drive GPIF states out on PORTE[2:0], debug WF
  // 1:0	IFCFG=10     , FX2 in GPIF master mode

  // abort any waveforms pending
  GPIFABORT = 0xFF;

  GPIFREADYCFG = 0x00;
  GPIFCTLCFG = 0x00;
  GPIFIDLECS = 0x00;
  GPIFIDLECTL = 0x03;	// CTL2 = 1; CTL1 = 1; CTL0 = 1
  GPIFWFSELECT = 0xE4;
  GPIFREADYSTAT = 0x00;

  capturing = FALSE;
}

void init_config_intf(void)
{
  IFCONFIG = 0xa6;
  // 7	IFCLKSRC=1   , FIFOs executes on internal clk source
  // 6	xMHz=0       , 30MHz internal clk rate
  // 5	IFCLKOE=1    , Drive IFCLK pin signal at 30MHz
  // 4	IFCLKPOL=0   , Don't invert IFCLK pin signal from internal clk
  // 3	ASYNC=0      , master samples synchronously
  // 2	GSTATE=1     , Drive GPIF states out on PORTE[2:0], debug WF
  // 1:0	IFCFG=10     , FX2 in GPIF master mode

  // abort any waveforms pending
  GPIFABORT = 0xFF;
  
  GPIFREADYCFG = 0xE0;	// Internal RDY = 1; asynchronous RDY signals
  GPIFCTLCFG = 0x00;
  GPIFIDLECS = 0x00;
  GPIFIDLECTL = 0x00;	// CTL2 = 0; CTL1 = 0; CTL0 = 0 (Release CTL pins)
  GPIFWFSELECT = 0xE4;
  GPIFREADYSTAT = 0x00;

  EP2FIFOPFH = 0x40;
  EP2FIFOPFL = 0x01;
  EP2GPIFFLGSEL = 0x00;	

  // Change Status LED to Config Status
  // IOA &= 0xfd;
  // IOA |= 0x02;
}

void start_capture(void)
{
  // Sample clear before each acquistion
  IOA &= 0xfb;

  // Abort currently executing GPIF waveform (if any)
  GPIFABORT = 0xff;

  // Proper FIFO reset: switch to manual mode, reset, restore
  FIFORESET = 0x80;  // set NAKALL bit to NAK all transfers from host
  SYNCDELAY;
  EP6FIFOCFG = 0;    // switch to manual mode
  SYNCDELAY;
  FIFORESET = 0x06;  // reset EP6 FIFO
  SYNCDELAY;
  EP6FIFOCFG = bmAUTOIN | bmWORDWIDE | bmZEROLENIN;  // manual mode (no AUTOIN)
  SYNCDELAY;
  FIFORESET = 0x00;  // clear NAKALL bit to resume normal operation
  SYNCDELAY;	 

  //Ensure GPIF is idle before reconfiguration
  while (!(GPIFTRIG & 0x80));

  // Clear the stop flag
  GPIFREADYCFG &= 0x7f;

  // sample clear end
 // IOA |= 0x08;

  // Execute the whole GPIF waveform once.
  GPIFTCB1= 0;
  SYNCDELAY;
  GPIFTCB0= 1;

  // Perform the initial GPIF read
  while ( !(GPIFTRIG & 0x80 ) ); // wait until things are finished
  GPIFTRIG = GPIFTRIGRD | GPIF_EP6;

  // Update the status
  capturing = TRUE;
  pktsent = FALSE;
  IOA |= 0x04;
}

void stop_capture(void)
{
  GPIFREADYCFG |= bmBIT7;
  IOA &= 0xfb;
  SYNCDELAY;
  INPKTEND = 0x86;
}

void poll_intf(void)
{
  /* When byte count �� 512, commit via INPKTEND */
  if (capturing) {
    if (EP6FIFOFLGS & 0x01) {
    //  if (!pktsent) {
        INPKTEND = 0x06;  // EP6, commit (SKIP=0)
        SYNCDELAY;
   //     pktsent = TRUE;
    //  }
   // } else {
   //   pktsent = FALSE;
   }
  }

  /* Polling capture status. */
  if (capturing && (GPIFTRIG & 0x80))
  {
      SYNCDELAY;
      INPKTEND = 0x86;
      SYNCDELAY;
      /* Stop requested via stop_capture() (INTRDY=1). */
      FIFORESET = 0x80;
      SYNCDELAY;
      EP6FIFOCFG = 0;
      SYNCDELAY;
      FIFORESET = 0x06;
      SYNCDELAY;
      EP6FIFOCFG = 0;
      SYNCDELAY;
      FIFORESET = 0x00;
      SYNCDELAY;
      capturing = FALSE;
  }
}
