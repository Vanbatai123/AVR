//==============================================================================
// File:           bootldr.h
// Compiler:       IAR Atmel AVR C/EC++ Compiler
// Output Size:    -
// Created:        Mon Apr 03 23:49:58 2023

//
// Description:    This file is generated by 'create' tool and is to be included
//                 in the 'bootldr' project. It contains the settings to
//                 configure the boot loader according to the configurations
//                 used in the encrypted file.
//==============================================================================


#ifndef bootldr_h
#define bootldr_h


#define PAGE_SIZE 128
#define MEM_SIZE 14336
#define CRC_CHECK
#define SIGNATURE 0x7162BF4A
#define BUFFER_SIZE 148
#define INITIALVECTOR_3 0xEFCCDCE2
#define INITIALVECTOR_2 0xA0A8B83F
#define INITIALVECTOR_1 0xF7964E9A
#define INITIALVECTOR_0 0xCCA1CE73
#define KEY_COUNT 3


#endif // bootldr_h
