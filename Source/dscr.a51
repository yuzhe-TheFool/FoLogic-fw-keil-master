;;-----------------------------------------------------------------------------
;; This file is part of the FoLogic-firmware project.
;;
;; Copyright (C) 2026 the fool
;;
;; This program is free software: you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <https://www.gnu.org/licenses/>.
;;-----------------------------------------------------------------------------

DSCR_DEVICE   equ   1  
DSCR_CONFIG   equ   2  
DSCR_STRING   equ   3  
DSCR_INTRFC   equ   4  
DSCR_ENDPNT   equ   5  
DSCR_DEVQUAL  equ   6  

DSCR_DEVICE_LEN   equ   18
DSCR_CONFIG_LEN   equ    9
DSCR_INTRFC_LEN   equ    9
DSCR_ENDPNT_LEN   equ    7
DSCR_DEVQUAL_LEN  equ   10

ET_CONTROL   equ   0   ;; Endpoint type: Control
ET_ISO       equ   1   ;; Endpoint type: Isochronous
ET_BULK      equ   2   ;; Endpoint type: Bulk
ET_INT       equ   3   ;; Endpoint type: Interrupt

public      DeviceDscr, DeviceQualDscr, HighSpeedConfigDscr, FullSpeedConfigDscr, StringDscr, UserDscr

;DSCR   SEGMENT   CODE

;;-----------------------------------------------------------------------------
;; Global Variables
;;-----------------------------------------------------------------------------

;     rseg DSCR                 ;; locate the descriptor table in on-part memory.

CSEG AT 100H               ;; TODO: this needs to be changed before release

DeviceDscr:   
      db   DSCR_DEVICE_LEN      ;; Descriptor length
      db   DSCR_DEVICE          ;; Decriptor type
      dw   0002H                ;; Specification Version (BCD)
      db   0ffH                 ;; Device class
      db   0ffH                 ;; Device sub-class
      db   0ffH                 ;; Device sub-sub-class
      db   64                   ;; Maximum packet size
      dw   0B404H               ;; Vendor ID (Cypress) - byte swapped for WinUSB driver
      dw   1386H                ;; Product ID (Cypress) - byte swapped for WinUSB driver
      dw   0000H                ;; Product version ID
      db   1                    ;; Manufacturer string index
      db   2                    ;; Product string index
      db   0                    ;; Serial number string index
      db   1                    ;; Number of configurations

org (($ / 2) +1) * 2

DeviceQualDscr:
      db   DSCR_DEVQUAL_LEN     ;; Descriptor length
      db   DSCR_DEVQUAL         ;; Decriptor type
      dw   0002H                ;; Specification Version (BCD)
      db   0ffH                 ;; Device class
      db   0ffH                 ;; Device sub-class
      db   0ffH                 ;; Device sub-sub-class
      db   64                   ;; Maximum packet size
      db   1                    ;; Number of configurations
      db   0                    ;; Reserved

org (($ / 2) +1) * 2

HighSpeedConfigDscr:   
      db   DSCR_CONFIG_LEN      ;; Descriptor length
      db   DSCR_CONFIG          ;; Descriptor type
      db   (HighSpeedConfigDscrEnd-HighSpeedConfigDscr) mod 256 ;; Total Length (LSB)
      db   (HighSpeedConfigDscrEnd-HighSpeedConfigDscr)  /  256 ;; Total Length (MSB)
      db   1                    ;; Number of interfaces
      db   1                    ;; Configuration number
      db   0                    ;; Configuration string
      db   80H                  ;; Attributes (bus powered, no wakeup)
      db   32H                  ;; Power requirement (100mA)

;; Interface Descriptor
      db   DSCR_INTRFC_LEN      ;; Descriptor length
      db   DSCR_INTRFC          ;; Descriptor type
      db   0                    ;; Zero-based index of this interface
      db   0                    ;; Alternate setting
      db   2                    ;; Number of end points 
      db   0ffH                 ;; Interface class
      db   0ffH                 ;; Interface sub class
      db   0ffH                 ;; Interface sub sub class
      db   0                    ;; Interface descriptor string index
      
;; Endpoint Descriptor
      db   DSCR_ENDPNT_LEN      ;; Descriptor length
      db   DSCR_ENDPNT          ;; Descriptor type
      db   02H                  ;; Endpoint number, and direction
      db   ET_BULK              ;; Endpoint type
      db   00H                  ;; Maximum packet size (LSB)
      db   02H                  ;; Maximum packet size (MSB)
      db   00H                  ;; Polling interval


;; Endpoint Descriptor
      db   DSCR_ENDPNT_LEN      ;; Descriptor length
      db   DSCR_ENDPNT          ;; Descriptor type
      db   86H                  ;; Endpoint number, and direction
      db   ET_BULK              ;; Endpoint type
      db   00H                  ;; Maximum packet size (LSB)
      db   02H                  ;; Maximum packet size (MSB)
      db   00H                  ;; Polling interval

HighSpeedConfigDscrEnd:   

org (($ / 2) +1) * 2

FullSpeedConfigDscr:   
      db   DSCR_CONFIG_LEN      ;; Descriptor length
      db   DSCR_CONFIG          ;; Descriptor type
      db   (FullSpeedConfigDscrEnd-FullSpeedConfigDscr) mod 256 ;; Total Length (LSB)
      db   (FullSpeedConfigDscrEnd-FullSpeedConfigDscr)  /  256 ;; Total Length (MSB)
      db   1                    ;; Number of interfaces
      db   1                    ;; Configuration number
      db   0                    ;; Configuration string
      db   80H                  ;; Attributes (bus powered, no wakeup)
      db   32H                  ;; Power requirement (100mA)

;; Interface Descriptor
      db   DSCR_INTRFC_LEN      ;; Descriptor length
      db   DSCR_INTRFC          ;; Descriptor type
      db   0                    ;; Zero-based index of this interface
      db   0                    ;; Alternate setting
      db   2                    ;; Number of end points 
      db   0ffH                 ;; Interface class
      db   0ffH                 ;; Interface sub class
      db   0ffH                 ;; Interface sub sub class
      db   0                    ;; Interface descriptor string index
      
;; Endpoint Descriptor
      db   DSCR_ENDPNT_LEN      ;; Descriptor length
      db   DSCR_ENDPNT          ;; Descriptor type
      db   02H                  ;; Endpoint number, and direction
      db   ET_BULK              ;; Endpoint type
      db   40H                  ;; Maximum packet size (LSB)
      db   00H                  ;; Maximum packet size (MSB)
      db   00H                  ;; Polling interval

;; Endpoint Descriptor
      db   DSCR_ENDPNT_LEN      ;; Descriptor length
      db   DSCR_ENDPNT          ;; Descriptor type
      db   86H                  ;; Endpoint number, and direction
      db   ET_BULK              ;; Endpoint type
      db   40H                  ;; Maximum packet size (LSB)
      db   00H                  ;; Maximum packet size (MSB)
      db   00H                  ;; Polling interval

FullSpeedConfigDscrEnd:   

org (($ / 2) +1) * 2

StringDscr:

StringDscr0:   
      db   StringDscr0End-StringDscr0      ;; String descriptor length
      db   DSCR_STRING
      db   09H,04H
StringDscr0End:

StringDscr1:   
      db   StringDscr1End-StringDscr1      ;; String descriptor length
      db   DSCR_STRING
      db   'F',00
      db   'o',00
      db   'L',00
      db   'o',00
      db   'g',00
      db   'i',00
      db   'c',00
StringDscr1End:

StringDscr2:   
      db   StringDscr2End-StringDscr2      ;; Descriptor length
      db   DSCR_STRING
      db   'F',00
      db   'o',00
      db   'L',00
      db   'o',00
      db   'g',00
      db   'i',00
      db   'c',00
StringDscr2End:

StringDscr3:   
      db   StringDscr3End-StringDscr3      ;; Descriptor length
      db   DSCR_STRING
StringDscr3End:

UserDscr:      
      dw   0000H
      end
      
