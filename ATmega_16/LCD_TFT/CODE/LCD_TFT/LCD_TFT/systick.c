/*
[nwazet Open Source Software & Open Source Hardware
Authors: Fabien Royer
Software License Agreement (BSD License)

Copyright (c) 2010, Nwazet, LLC. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* Neither the name of Nwazet, LLC. nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
* The names '[nwazet', 'nwazet', the ASCII hazelnut in the [nwazet logo and the color of the logo are Trademarks of nwazet, LLC. and cannot be used to endorse or promote products derived from this software or any hardware designs without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Parts of the [nwazet source code is derived from the MicroBuilder codebase.
The MicroBuilder license is provided below and is alos located under .\MicroBuilder\License.txt
*/
#include "systick.h"
#include "projectconfig.c"

static volatile uint32_t _timingDelay;
static volatile uint32_t _ticks;

void systickInit(void){
  if (SysTick_Config(SystemCoreClock / 1000)){
    /* Capture error */
    while (1);
  }
  _ticks = 0;
}
void nopDelay(unsigned short cycles){
  cycles *= 1000;
  for(unsigned short cycle = 0; cycle < cycles; cycle++){
    __asm("nop");
  }
}
void delay(uint32_t nTime){
  _timingDelay = nTime;
  while(_timingDelay != 0);
}

uint32_t systickGetTicks(){
  return _ticks;
}

void OnSystickInterrupt(void)
{
  if(_timingDelay != 0x00){
    _timingDelay--;
  }
  _ticks++;
}
