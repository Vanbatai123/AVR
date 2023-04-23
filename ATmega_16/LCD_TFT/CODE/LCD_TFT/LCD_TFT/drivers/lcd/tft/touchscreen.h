/**************************************************************************/
/*!
   @file     touchscreen.h
   @author   K. Townsend (microBuilder.eu)
   @date     22 March 2010
   @version  0.10

   @section LICENSE

   Parts copyright (c) 2012, Fabien Royer | Nwazet, LLC. All rights reserved.

   Software License Agreement (BSD License)

   Copyright (c) 2010, microBuilder SARL
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   3. Neither the name of the copyright holders nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef __TOUCHSCREEN_H__
  #define __TOUCHSCREEN_H__

#include "projectconfig.h"
#include "adc.h"

#define CFG_TFTLCD_TS_DEFAULTTHRESHOLD (300)
#define CFG_TFTLCD_TS_KEYPADDELAY      (100)

typedef struct Point
{
  int32_t x;
  int32_t y;
} tsPoint_t;

typedef struct Matrix
{
  int32_t An,
          Bn,
          Cn,
          Dn,
          En,
          Fn,
          Divider;
} tsMatrix_t;

typedef struct
{
  uint32_t xraw;  // Touch screen x
  uint32_t yraw;  // Touch screen Y
  uint16_t xlcd;  // LCD co-ordinate X
  uint16_t ylcd;  // LCD co-ordinate Y
  uint32_t z1;
  uint32_t z2;
  bool valid;     // Whether this is a valid reading or not
} tsTouchData_t;

typedef enum
{
  TS_ERROR_NONE         = 0,
  TS_ERROR_TIMEOUT      = -1,   // Timeout occured before a valid reading
  TS_ERROR_XYMISMATCH   = -2    // Unable to get a stable X/Y value
} tsTouchError_t;

typedef struct
{
  int x;
  int y;
  int pressure;
} TouchData;

// Method Prototypes
void            tsInit ( void );
tsTouchError_t  tsRead(tsTouchData_t* data);
void            tsCalibrate ( void );
tsTouchError_t  tsWaitForEvent(tsTouchData_t* data, uint32_t timeoutMS);
int             tsSetThreshhold(unsigned short value);
unsigned short  tsGetThreshhold(void);
tsMatrix_t*     tsGetCalibrationMatrix();
#endif
