/* Name: usbdrvasm.S
 * Project: V-USB, virtual USB port for Atmel's(r) AVR(r) microcontrollers
 * Author: Christian Starkjohann
 * Creation Date: 2007-06-13
 * Tabsize: 4
 * Copyright: (c) 2007 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

/*
General Description:
This module is the assembler part of the USB driver. This file contains
general code (preprocessor acrobatics and CRC computation) and then includes
the file appropriate for the given clock rate.
*/

#define __SFR_OFFSET 0      /* used by avr-libc's register definitions */
#include "usbportability.h"
#include "usbdrv.h"         /* for common defs */

/* register names */
#define x1      r16
#define x2      r17
#define shift   r18
#define cnt     r19
#define x3      r20
#define x4      r21
#define x5		r22
#define bitcnt  x5
#define phase   x4
#define leap    x4

#   ifndef USB_INTR_VECTOR /* default to hardware interrupt INT0 */
#       ifdef INT0_vect
#           define USB_INTR_VECTOR  INT0_vect       // this is the "new" define for the vector
#       else
#           define USB_INTR_VECTOR  SIG_INTERRUPT0  // this is the "old" vector
#       endif
#   endif
    .text
    .global USB_INTR_VECTOR
    .type   USB_INTR_VECTOR, @function
    .global usbCrc16
    .global usbCrc16Append


#if USB_INTR_PENDING < 0x40 /* This is an I/O address, use in and out */
#   define  USB_LOAD_PENDING(reg)   in reg, USB_INTR_PENDING
#   define  USB_STORE_PENDING(reg)  out USB_INTR_PENDING, reg
#else   /* It's a memory address, use lds and sts */
#   define  USB_LOAD_PENDING(reg)   lds reg, USB_INTR_PENDING
#   define  USB_STORE_PENDING(reg)  sts USB_INTR_PENDING, reg
#endif

#define usbTxLen1   usbTxStatus1
#define usbTxBuf1   (usbTxStatus1 + 1)
#define usbTxLen3   usbTxStatus3
#define usbTxBuf3   (usbTxStatus3 + 1)


//----------------------------------------------------------------------------
// Utility functions
//----------------------------------------------------------------------------

#   define argLen   r22 /* argument 2 */
#   define argPtrL  r24 /* argument 1 */
#   define argPtrH  r25 /* argument 1 */

#   define resCrcL  r24 /* result */
#   define resCrcH  r25 /* result */

#   define ptrL     XL
#   define ptrH     XH
#   define ptr      x
#   define byte     r18
#   define bitCnt   r19
#   define polyL    r20
#   define polyH    r21
#   define scratch  r23



// This implementation is slower, but has less code size
//
// extern unsigned usbCrc16(unsigned char *argPtr, unsigned char argLen)//
//   argPtr  r24+25 / r16+r17
//   argLen  r22 / r18
// temp variables:
//   byte    r18 / r22
//   bitCnt  r19
//   poly    r20+r21
//   scratch r23
//   resCrc  r24+r25 / r16+r17
//   ptr     X / Z
usbCrc16:
    mov     ptrL, argPtrL
    mov     ptrH, argPtrH
    ldi     resCrcL, 0
    ldi     resCrcH, 0
    ldi     polyL, lo8(0xa001)
    ldi     polyH, hi8(0xa001)
    com     argLen      // argLen = -argLen - 1: modified loop to ensure that carry is set
    ldi     bitCnt, 0   // loop counter with starnd condition = end condition
    rjmp    usbCrcLoopEntry
usbCrcByteLoop:
    ld      byte, ptr+
    eor     resCrcL, byte
usbCrcBitLoop:
    ror     resCrcH     // carry is always set here (see brcs jumps to here)
    ror     resCrcL
    brcs    usbCrcNoXor
    eor     resCrcL, polyL
    eor     resCrcH, polyH
usbCrcNoXor:
    subi    bitCnt, 224 // (8 * 224) % 256 = 0// this loop iterates 8 times
    brcs    usbCrcBitLoop
usbCrcLoopEntry:
    subi    argLen, -1
    brcs    usbCrcByteLoop
usbCrcReady:
    ret
// Thanks to Reimar Doeffinger for optimizing this CRC routine!


// extern unsigned usbCrc16Append(unsigned char *data, unsigned char len)//
usbCrc16Append:
    rcall   usbCrc16
    st      ptr+, resCrcL
    st      ptr+, resCrcH
    ret

#undef argLen
#undef argPtrL
#undef argPtrH
#undef resCrcL
#undef resCrcH
#undef ptrL
#undef ptrH
#undef ptr
#undef byte
#undef bitCnt
#undef polyL
#undef polyH
#undef scratch


//----------------------------------------------------------------------------
// Now include the clock rate specific code
//----------------------------------------------------------------------------
/*
#ifndef USB_CFG_CLOCK_KHZ
#   ifdef F_CPU
#       define USB_CFG_CLOCK_KHZ (F_CPU/1000)
#   else
#       error "USB_CFG_CLOCK_KHZ not defined in usbconfig.h and no F_CPU set!"
#   endif
#endif
*/
#if USB_CFG_CHECK_CRC   /* separate dispatcher for CRC type modules */
#   if USB_CFG_CLOCK_KHZ == 18000
#       include "usbdrvasm18-crc.inc"
#   else
#       error "USB_CFG_CLOCK_KHZ is not one of the supported crc-rates!"
#   endif
#else   /* USB_CFG_CHECK_CRC */
#   if USB_CFG_CLOCK_KHZ == 16000
#       include "usbdrvasm16.inc"
#   else
#       error "USB_CFG_CLOCK_KHZ is not one of the supported non-crc-rates!"
#   endif
#endif /* USB_CFG_CHECK_CRC */
