
;CodeVisionAVR C Compiler V2.05.6 
;(C) Copyright 1998-2012 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type              : ATmega8
;Program type           : Application
;Clock frequency        : 8,000000 MHz
;Memory model           : Small
;Optimize for           : Size
;(s)printf features     : int, width
;(s)scanf features      : int, width
;External RAM size      : 0
;Data Stack size        : 256 byte(s)
;Heap size              : 0 byte(s)
;Promote 'char' to 'int': Yes
;'char' is unsigned     : Yes
;8 bit enums            : Yes
;Global 'const' stored in FLASH: No
;Enhanced function parameter passing: Yes
;Enhanced core instructions: On
;Automatic register allocation for global variables: On
;Smart register allocation: On

	#pragma AVRPART ADMIN PART_NAME ATmega8
	#pragma AVRPART MEMORY PROG_FLASH 8192
	#pragma AVRPART MEMORY EEPROM 512
	#pragma AVRPART MEMORY INT_SRAM SIZE 1024
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x60

	.LISTMAC
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU USR=0xB
	.EQU UDR=0xC
	.EQU SPSR=0xE
	.EQU SPDR=0xF
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU EECR=0x1C
	.EQU EEDR=0x1D
	.EQU EEARL=0x1E
	.EQU EEARH=0x1F
	.EQU WDTCR=0x21
	.EQU MCUCR=0x35
	.EQU GICR=0x3B
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __SRAM_START=0x0060
	.EQU __SRAM_END=0x045F
	.EQU __DSTACK_SIZE=0x0100
	.EQU __HEAP_SIZE=0x0000
	.EQU __CLEAR_SRAM_SIZE=__SRAM_END-__SRAM_START+1

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ANDI R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ORI  R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __CLRD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+(@1))
	LDI  R31,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	LDI  R22,BYTE3(2*@0+(@1))
	LDI  R23,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTW2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	LDI  R24,BYTE3(2*@0+(@1))
	LDI  R25,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+(@2))
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+(@3))
	LDI  R@1,HIGH(@2+(@3))
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+(@3))
	LDI  R@1,HIGH(@2*2+(@3))
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+(@1)
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+(@1)
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	LDS  R22,@0+(@1)+2
	LDS  R23,@0+(@1)+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+(@2)
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+(@3)
	LDS  R@1,@2+(@3)+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+(@1)
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	LDS  R24,@0+(@1)+2
	LDS  R25,@0+(@1)+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+(@1),R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	STS  @0+(@1)+2,R22
	STS  @0+(@1)+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+(@1),R0
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+(@1),R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+(@1),R@2
	STS  @0+(@1)+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTBSR
	STD  Y+@1,R@0
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	RCALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMRDW
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X
	.ENDM

	.MACRO __GETD1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X+
	LD   R22,X
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	RCALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __GETBRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	LD   R@0,X
	.ENDM

	.MACRO __GETWRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	LD   R@0,X+
	LD   R@1,X
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	ST   Z,R@0
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

	.CSEG
	.ORG 0x00

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	RJMP __RESET
	RJMP 0x00
	RJMP 0x00
	RJMP _timer2_comp_isr
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00

_0x0:
	.DB  0x31,0x2E,0x77,0x61,0x76,0x0
_0x4004B:
	.DB  0x80,0x9A,0x90,0x41,0x8E,0x41,0x8F,0x80
	.DB  0x45,0x45,0x45,0x49,0x49,0x49,0x8E,0x8F
	.DB  0x90,0x92,0x92,0x4F,0x99,0x4F,0x55,0x55
	.DB  0x59,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F
	.DB  0x41,0x49,0x4F,0x55,0xA5,0xA5,0xA6,0xA7
	.DB  0xA8,0xA9,0xAA,0xAB,0xAC,0x21,0xAE,0xAF
	.DB  0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7
	.DB  0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF
	.DB  0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7
	.DB  0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF
	.DB  0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7
	.DB  0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF
	.DB  0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7
	.DB  0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF
	.DB  0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7
	.DB  0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF

__GLOBAL_INI_TBL:
	.DW  0x06
	.DW  _0x5
	.DW  _0x0*2

	.DW  0x80
	.DW  _cvt_G002
	.DW  _0x4004B*2

_0xFFFFFFFF:
	.DW  0

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  GICR,R31
	OUT  GICR,R30
	OUT  MCUCR,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	OUT  WDTCR,R31
	OUT  WDTCR,R30

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(__CLEAR_SRAM_SIZE)
	LDI  R25,HIGH(__CLEAR_SRAM_SIZE)
	LDI  R26,__SRAM_START
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30
	LDI  R30,HIGH(__SRAM_END-__HEAP_SIZE)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)
	LDI  R29,HIGH(__SRAM_START+__DSTACK_SIZE)

	RJMP _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x160

	.CSEG
;#include <mega8.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x80
	.EQU __sm_mask=0x70
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0x60
	.EQU __sm_ext_standby=0x70
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
;#include "diskio.h"
;#include "pff.h"
;
;unsigned char buffer[512];   /* буфер дл€ копировани€ данных файла */
;volatile unsigned int count;
;
;interrupt [TIM2_COMP] void timer2_comp_isr(void)
; 0000 0009 {

	.CSEG
_timer2_comp_isr:
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 000A     OCR1A = buffer[count];
	LDS  R30,_count
	LDS  R31,_count+1
	SUBI R30,LOW(-_buffer)
	SBCI R31,HIGH(-_buffer)
	LD   R30,Z
	LDI  R31,0
	OUT  0x2A+1,R31
	OUT  0x2A,R30
; 0000 000B 	if (++count >= 512)
	LDI  R26,LOW(_count)
	LDI  R27,HIGH(_count)
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
	CPI  R30,LOW(0x200)
	LDI  R26,HIGH(0x200)
	CPC  R31,R26
	BRLO _0x3
; 0000 000C 		count = 0;
	LDI  R30,LOW(0)
	STS  _count,R30
	STS  _count+1,R30
; 0000 000D }
_0x3:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	RETI
;
;void main(void)
; 0000 0010 {
_main:
; 0000 0011     unsigned int br;         /* счетчик чтени€/записи файла */
; 0000 0012     unsigned char buf = 0;
; 0000 0013     FATFS fs;         /* –абоча€ область (file system object) дл€ логических дисков */
; 0000 0014 
; 0000 0015 PORTB=0x00;
	SBIW R28,44
;	br -> R16,R17
;	buf -> R19
;	fs -> Y+0
	LDI  R19,0
	LDI  R30,LOW(0)
	OUT  0x18,R30
; 0000 0016 DDRB=0x02;
	LDI  R30,LOW(2)
	OUT  0x17,R30
; 0000 0017 
; 0000 0018 // Timer/Counter 1 initialization
; 0000 0019 // Clock source: System Clock
; 0000 001A // Clock value: 8000,000 kHz
; 0000 001B // Mode: Fast PWM top=0x00FF
; 0000 001C // OC1A output: Non-Inv.
; 0000 001D TCCR1A=0x81;
	LDI  R30,LOW(129)
	OUT  0x2F,R30
; 0000 001E TCCR1B=0x09;
	LDI  R30,LOW(9)
	OUT  0x2E,R30
; 0000 001F TCNT1H=0x00;
	LDI  R30,LOW(0)
	OUT  0x2D,R30
; 0000 0020 TCNT1L=0x00;
	OUT  0x2C,R30
; 0000 0021 OCR1A=0x00;
	RCALL SUBOPT_0x0
	OUT  0x2A+1,R31
	OUT  0x2A,R30
; 0000 0022 
; 0000 0023 
; 0000 0024 // Timer/Counter 2 initialization
; 0000 0025 // Clock source: System Clock
; 0000 0026 // Clock value: 250,000 kHz
; 0000 0027 // Mode: CTC top=OCR2
; 0000 0028 // OC2 output: Disconnected
; 0000 0029 TCCR2=0x0B;
	LDI  R30,LOW(11)
	OUT  0x25,R30
; 0000 002A TCNT2=0x00;
	LDI  R30,LOW(0)
	OUT  0x24,R30
; 0000 002B //OCR2=0x1F; //дл€ 8к√ц
; 0000 002C OCR2=0xB;    //дл€ 22к√ц
	LDI  R30,LOW(11)
	OUT  0x23,R30
; 0000 002D 
; 0000 002E #asm("sei")
	sei
; 0000 002F // Timer(s)/Counter(s) Interrupt(s) initialization
; 0000 0030 
; 0000 0031 
; 0000 0032 if(disk_initialize()==0) //инициализируем флешку
	RCALL _disk_initialize
	CPI  R30,0
	BREQ PC+2
	RJMP _0x4
; 0000 0033 {
; 0000 0034   pf_mount(&fs);  //монтируем файловую систему
	MOVW R26,R28
	RCALL _pf_mount
; 0000 0035   pf_open("1.wav");  //открываем вавку
	__POINTW2MN _0x5,0
	RCALL _pf_open
; 0000 0036   pf_lseek(44);     //перемещаем указатель на 44
	__GETD2N 0x2C
	RCALL _pf_lseek
; 0000 0037   pf_read(buffer, 512,&br);
	RCALL SUBOPT_0x1
	LDI  R30,LOW(512)
	LDI  R31,HIGH(512)
	RCALL SUBOPT_0x2
	IN   R26,SPL
	IN   R27,SPH
	SBIW R26,1
	PUSH R17
	PUSH R16
	RCALL _pf_read
	POP  R16
	POP  R17
; 0000 0038   TIMSK=0x80;
	LDI  R30,LOW(128)
	OUT  0x39,R30
; 0000 0039   while(1)
_0x6:
; 0000 003A   {
; 0000 003B      if(!buf && count>255)
	CPI  R19,0
	BRNE _0xA
	RCALL SUBOPT_0x3
	BRSH _0xB
_0xA:
	RJMP _0x9
_0xB:
; 0000 003C      {
; 0000 003D         pf_read(&buffer[0], 256,&br);
	RCALL SUBOPT_0x1
	LDI  R30,LOW(256)
	LDI  R31,HIGH(256)
	RCALL SUBOPT_0x2
	IN   R26,SPL
	IN   R27,SPH
	SBIW R26,1
	PUSH R17
	PUSH R16
	RCALL _pf_read
	POP  R16
	POP  R17
; 0000 003E         buf=1;
	LDI  R19,LOW(1)
; 0000 003F         if (br < 256)
	__CPWRN 16,17,256
	BRLO _0x8
; 0000 0040 	break;
; 0000 0041      }
; 0000 0042      if(buf && count<256)
_0x9:
	CPI  R19,0
	BREQ _0xE
	RCALL SUBOPT_0x3
	BRLO _0xF
_0xE:
	RJMP _0xD
_0xF:
; 0000 0043      {
; 0000 0044         pf_read(&buffer[256], 256,&br);
	__POINTW1MN _buffer,256
	RCALL SUBOPT_0x2
	LDI  R30,LOW(256)
	LDI  R31,HIGH(256)
	RCALL SUBOPT_0x2
	IN   R26,SPL
	IN   R27,SPH
	SBIW R26,1
	PUSH R17
	PUSH R16
	RCALL _pf_read
	POP  R16
	POP  R17
; 0000 0045         buf = 0;
	LDI  R19,LOW(0)
; 0000 0046         if (br < 256)
	__CPWRN 16,17,256
	BRLO _0x8
; 0000 0047 	break;
; 0000 0048      }
; 0000 0049 
; 0000 004A   }
_0xD:
	RJMP _0x6
_0x8:
; 0000 004B   TIMSK = 0x00;
	LDI  R30,LOW(0)
	OUT  0x39,R30
; 0000 004C   pf_mount(0x00);
	LDI  R26,LOW(0)
	LDI  R27,0
	RCALL _pf_mount
; 0000 004D }
; 0000 004E 
; 0000 004F while (1)
_0x4:
_0x11:
; 0000 0050       {
; 0000 0051 
; 0000 0052 
; 0000 0053       }
	RJMP _0x11
; 0000 0054 }
_0x14:
	RJMP _0x14

	.DSEG
_0x5:
	.BYTE 0x6
;//-----------------------------------------------------------------------
;// PFF - Low level disk control module for ATmega32
;//-----------------------------------------------------------------------
;#define _WRITE_FUNC	1			//allow write operations
;
;#include <mega8.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x80
	.EQU __sm_mask=0x70
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0x60
	.EQU __sm_ext_standby=0x70
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
;#include "diskio.h"
;
;// Definitions for MMC/SDC connection
;#define SD_DI   3
;#define SD_DO   4
;#define SD_CLK  5
;#define SD_CS   2
;//#define SD_INS  6
;//#define SD_WP   7
;
;// Definitions for MMC/SDC command
;#define CMD0	(0x40+0)	// GO_IDLE_STATE
;#define CMD1	(0x40+1)	// SEND_OP_COND (MMC)
;#define	ACMD41	(0xC0+41)	// SEND_OP_COND (SDC)
;#define CMD8	(0x40+8)	// SEND_IF_COND
;#define CMD16	(0x40+16)	// SET_BLOCKLEN
;#define CMD17	(0x40+17)	// READ_SINGLE_BLOCK
;#define CMD24	(0x40+24)	// WRITE_BLOCK
;#define CMD55	(0x40+55)	// APP_CMD
;#define CMD58	(0x40+58)	// READ_OCR
;
;//-----------------------------------------------------------------------
;// SPI functions
;//-----------------------------------------------------------------------
;#define hardware_spi // если определено, то spi аппаратный
;
;#define SPI_PORTX PORTB
;#define SPI_DDRX DDRB
;
;#define SPI_MISO SD_DO
;#define SPI_MOSI SD_DI
;#define SPI_SCK SD_CLK
;#define SPI_SS SD_CS
;
;void INIT_SPI(void) {
; 0001 0029 void INIT_SPI(void) {

	.CSEG
_INIT_SPI:
; 0001 002A #ifdef hardware_spi
; 0001 002B 	/*настройка портов ввода-вывода
; 0001 002C 	 все выводы, кроме MISO выходы*/
; 0001 002D 	SPI_DDRX |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS) | (0 << SPI_MISO);
	IN   R30,0x17
	ORI  R30,LOW(0x2C)
	OUT  0x17,R30
; 0001 002E 	SPI_PORTX |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS) | (1 << SPI_MISO);
	IN   R30,0x18
	ORI  R30,LOW(0x3C)
	OUT  0x18,R30
; 0001 002F 
; 0001 0030 	/*разрешение spi,старший бит вперед,мастер, режим 0*/
; 0001 0031 	SPCR = (1 << SPE) | (0 << DORD) | (1 << MSTR) | (0 << CPOL) | (0 << CPHA) | (1 << SPR1) | (1 << SPR0);
	LDI  R30,LOW(83)
	OUT  0xD,R30
; 0001 0032 	SPSR = (0 << SPI2X);
	LDI  R30,LOW(0)
	OUT  0xE,R30
; 0001 0033 #else
; 0001 0034 	PORTB |= (1<<SD_CS) | (1<<SD_DO) | (1<<SD_DI)/* | (1<<SD_WP) | (1<<SD_INS)*/;
; 0001 0035 	DDRB |=(1<<SD_CS) | (1<<SD_DI) | (1<<SD_CLK);
; 0001 0036 #endif
; 0001 0037 }
	RET
;
;void xmit_spi(BYTE data) {		// Send a byte
; 0001 0039 void xmit_spi(BYTE data) {
_xmit_spi:
; 0001 003A #ifdef hardware_spi
; 0001 003B 	SPDR = data;
	ST   -Y,R26
;	data -> Y+0
	LD   R30,Y
	OUT  0xF,R30
; 0001 003C 	while (!(SPSR & (1 << SPIF)));
_0x20003:
	SBIS 0xE,7
	RJMP _0x20003
; 0001 003D #else
; 0001 003E 	BYTE i;
; 0001 003F 
; 0001 0040 	for (i = 0; i < 8; i++) {
; 0001 0041 		if ((data & 0x80) == 0x00)
; 0001 0042 			PORTB &= ~(1<<SD_DI);
; 0001 0043 		else
; 0001 0044 			PORTB |= (1<<SD_DI);
; 0001 0045 		data = data << 1;
; 0001 0046 		PORTB |= (1<<SD_CLK);
; 0001 0047 		#asm("nop")
; 0001 0048 		PORTB &= ~(1<<SD_CLK);
; 0001 0049 	}
; 0001 004A #endif
; 0001 004B }
	ADIW R28,1
	RET
;
;BYTE rcv_spi(void) {				// Send 0xFF and receive a byte
; 0001 004D BYTE rcv_spi(void) {
_rcv_spi:
; 0001 004E #ifdef hardware_spi
; 0001 004F 	unsigned char data;
; 0001 0050 	SPDR = 0xFF;
	ST   -Y,R17
;	data -> R17
	LDI  R30,LOW(255)
	OUT  0xF,R30
; 0001 0051 	while (!(SPSR & (1 << SPIF)));
_0x20006:
	SBIS 0xE,7
	RJMP _0x20006
; 0001 0052 	data = SPDR;
	IN   R17,15
; 0001 0053 	return data;
	MOV  R30,R17
	LD   R17,Y+
	RET
; 0001 0054 #else
; 0001 0055 	BYTE i, res = 0;
; 0001 0056 
; 0001 0057 	PORTB |= (1<<SD_DI);
; 0001 0058 
; 0001 0059 	for (i = 0; i < 8; i++) {
; 0001 005A 		PORTB |= (1<<SD_CLK);
; 0001 005B 		res = res << 1;
; 0001 005C 		if ((PINB & (1<<SD_DO))!=0x00)
; 0001 005D 		res = res | 0x01;
; 0001 005E 		PORTB &= ~(1<<SD_CLK);
; 0001 005F 		#asm("nop")
; 0001 0060 	}
; 0001 0061 	return res;
; 0001 0062 #endif
; 0001 0063 } /* Send 0xFF and receive a byte */
;//-----------------------------------------------------------------------
;
;// Port Controls (Platform dependent)
;#define SELECT()	PORTB &= ~(1<<SD_CS)		// MMC CS = L
;#define	DESELECT()	PORTB |=  (1<<SD_CS)		// MMC CS = H
;#define	MMC_SEL		!(PORTB & (1<<SD_CS))	// MMC CS status (true:selected)
;
;//-----------------------------------------------------------------------
;//   Module Private Function
;//-----------------------------------------------------------------------
;static BYTE CardType;
;
;//-----------------------------------------------------------------------
;// Deselect the card and release SPI bus
;//-----------------------------------------------------------------------
;static
;void release_spi (void)
; 0001 0075 {
_release_spi_G001:
; 0001 0076 	rcv_spi();
	RCALL _rcv_spi
; 0001 0077 }
	RET
;
;//-----------------------------------------------------------------------
;// Send a command packet to MMC
;//-----------------------------------------------------------------------
;static
;BYTE send_cmd (
; 0001 007E 	BYTE cmd,		// Command byte
; 0001 007F 	DWORD arg		// Argument
; 0001 0080 )
; 0001 0081 {
_send_cmd_G001:
; 0001 0082 	BYTE n, res;
; 0001 0083 
; 0001 0084 
; 0001 0085 	if (cmd & 0x80) {	// ACMD<n> is the command sequense of CMD55-CMD<n>
	RCALL __PUTPARD2
	RCALL __SAVELOCR2
;	cmd -> Y+6
;	arg -> Y+2
;	n -> R17
;	res -> R16
	LDD  R30,Y+6
	ANDI R30,LOW(0x80)
	BREQ _0x20009
; 0001 0086 		cmd &= 0x7F;
	LDD  R30,Y+6
	ANDI R30,0x7F
	STD  Y+6,R30
; 0001 0087 		res = send_cmd(CMD55, 0);
	LDI  R30,LOW(119)
	RCALL SUBOPT_0x4
	MOV  R16,R30
; 0001 0088 		if (res > 1) return res;
	CPI  R16,2
	BRSH _0x200000D
; 0001 0089 	}
; 0001 008A 
; 0001 008B 	// Select the card
; 0001 008C 	DESELECT();
_0x20009:
	SBI  0x18,2
; 0001 008D 	rcv_spi();
	RCALL _rcv_spi
; 0001 008E 	SELECT();
	CBI  0x18,2
; 0001 008F 	rcv_spi();
	RCALL _rcv_spi
; 0001 0090 
; 0001 0091 	// Send a command packet
; 0001 0092 	xmit_spi(cmd);						// Start + Command index
	LDD  R26,Y+6
	RCALL SUBOPT_0x5
; 0001 0093 	xmit_spi((BYTE)(arg >> 24));		// Argument[31..24]
	LDI  R30,LOW(24)
	RCALL __LSRD12
	MOV  R26,R30
	RCALL _xmit_spi
; 0001 0094 	xmit_spi((BYTE)(arg >> 16));		// Argument[23..16]
	RCALL SUBOPT_0x6
	RCALL __LSRD16
	MOV  R26,R30
	RCALL SUBOPT_0x5
; 0001 0095 	xmit_spi((BYTE)(arg >> 8));			// Argument[15..8]
	LDI  R30,LOW(8)
	RCALL __LSRD12
	MOV  R26,R30
	RCALL _xmit_spi
; 0001 0096 	xmit_spi((BYTE)arg);				// Argument[7..0]
	LDD  R26,Y+2
	RCALL _xmit_spi
; 0001 0097 	n = 0x01;							// Dummy CRC + Stop
	LDI  R17,LOW(1)
; 0001 0098 	if (cmd == CMD0) n = 0x95;			// Valid CRC for CMD0(0)
	LDD  R26,Y+6
	CPI  R26,LOW(0x40)
	BRNE _0x2000B
	LDI  R17,LOW(149)
; 0001 0099 	if (cmd == CMD8) n = 0x87;			// Valid CRC for CMD8(0x1AA)
_0x2000B:
	LDD  R26,Y+6
	CPI  R26,LOW(0x48)
	BRNE _0x2000C
	LDI  R17,LOW(135)
; 0001 009A 	xmit_spi(n);
_0x2000C:
	MOV  R26,R17
	RCALL _xmit_spi
; 0001 009B 
; 0001 009C 	// Receive a command response
; 0001 009D 	n = 10;								// Wait for a valid response in timeout of 10 attempts
	LDI  R17,LOW(10)
; 0001 009E 	do {
_0x2000E:
; 0001 009F 		res = rcv_spi();
	RCALL _rcv_spi
	MOV  R16,R30
; 0001 00A0 	} while ((res & 0x80) && --n);
	SBRS R16,7
	RJMP _0x20010
	SUBI R17,LOW(1)
	BRNE _0x20011
_0x20010:
	RJMP _0x2000F
_0x20011:
	RJMP _0x2000E
_0x2000F:
; 0001 00A1 
; 0001 00A2 	return res;			// Return with the response value
_0x200000D:
	MOV  R30,R16
	RCALL __LOADLOCR2
	ADIW R28,7
	RET
; 0001 00A3 }
;
;//--------------------------------------------------------------------------
;//
;//   Public Functions
;//
;//--------------------------------------------------------------------------
;
;//--------------------------------------------------------------------------
;// Initialize Disk Drive
;//--------------------------------------------------------------------------
;DSTATUS disk_initialize (void)
; 0001 00AF {
_disk_initialize:
; 0001 00B0 	BYTE n, cmd, ty, ocr[4];
; 0001 00B1 	WORD tmr;
; 0001 00B2 
; 0001 00B3 
; 0001 00B4 	INIT_SPI();
	SBIW R28,4
	RCALL __SAVELOCR6
;	n -> R17
;	cmd -> R16
;	ty -> R19
;	ocr -> Y+6
;	tmr -> R20,R21
	RCALL _INIT_SPI
; 0001 00B5 
; 0001 00B6 //	if ((PINB&_BV(SD_INS))!=0x00) return STA_NOINIT;
; 0001 00B7 
; 0001 00B8 #if _WRITE_FUNC
; 0001 00B9 	if (MMC_SEL) disk_writep(0, 0);		// Finalize write process if it is in progress
	SBIC 0x18,2
	RJMP _0x20012
	RCALL SUBOPT_0x0
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x7
	RCALL _disk_writep
; 0001 00BA #endif
; 0001 00BB 	for (n = 100; n; n--) rcv_spi();	// Dummy clocks
_0x20012:
	LDI  R17,LOW(100)
_0x20014:
	CPI  R17,0
	BREQ _0x20015
	RCALL _rcv_spi
	SUBI R17,1
	RJMP _0x20014
_0x20015:
; 0001 00BD ty = 0;
	LDI  R19,LOW(0)
; 0001 00BE 	if (send_cmd(CMD0, 0) == 1) {			// Enter Idle state
	LDI  R30,LOW(64)
	RCALL SUBOPT_0x4
	CPI  R30,LOW(0x1)
	BREQ PC+2
	RJMP _0x20016
; 0001 00BF 		if (send_cmd(CMD8, 0x1AA) == 1) {	// SDv2
	LDI  R30,LOW(72)
	ST   -Y,R30
	__GETD2N 0x1AA
	RCALL _send_cmd_G001
	CPI  R30,LOW(0x1)
	BRNE _0x20017
; 0001 00C0 			for (n = 0; n < 4; n++) ocr[n] = rcv_spi();		// Get trailing return value of R7 resp
	LDI  R17,LOW(0)
_0x20019:
	CPI  R17,4
	BRSH _0x2001A
	RCALL SUBOPT_0x8
	PUSH R31
	PUSH R30
	RCALL _rcv_spi
	POP  R26
	POP  R27
	ST   X,R30
	SUBI R17,-1
	RJMP _0x20019
_0x2001A:
; 0001 00C1 if (ocr[2] == 0x01 && ocr[3] == 0xAA) {
	LDD  R26,Y+8
	CPI  R26,LOW(0x1)
	BRNE _0x2001C
	LDD  R26,Y+9
	CPI  R26,LOW(0xAA)
	BREQ _0x2001D
_0x2001C:
	RJMP _0x2001B
_0x2001D:
; 0001 00C2 				for (tmr = 12000; tmr && send_cmd(ACMD41, 1UL << 30); tmr--) ;	// Wait for leaving idle state (ACMD41 with HCS bit)
	__GETWRN 20,21,12000
_0x2001F:
	RCALL SUBOPT_0x9
	BREQ _0x20021
	LDI  R30,LOW(233)
	ST   -Y,R30
	__GETD2N 0x40000000
	RCALL SUBOPT_0xA
	BRNE _0x20022
_0x20021:
	RJMP _0x20020
_0x20022:
	__SUBWRN 20,21,1
	RJMP _0x2001F
_0x20020:
; 0001 00C3 				if (tmr && send_cmd(CMD58, 0) == 0) {		// Check CCS bit in the OCR
	RCALL SUBOPT_0x9
	BREQ _0x20024
	LDI  R30,LOW(122)
	RCALL SUBOPT_0x4
	CPI  R30,0
	BREQ _0x20025
_0x20024:
	RJMP _0x20023
_0x20025:
; 0001 00C4 					for (n = 0; n < 4; n++) ocr[n] = rcv_spi();
	LDI  R17,LOW(0)
_0x20027:
	CPI  R17,4
	BRSH _0x20028
	RCALL SUBOPT_0x8
	PUSH R31
	PUSH R30
	RCALL _rcv_spi
	POP  R26
	POP  R27
	ST   X,R30
	SUBI R17,-1
	RJMP _0x20027
_0x20028:
; 0001 00C5 ty = (ocr[0] & 0x40) ? 0x04	 | 0x08	 : 0x04	;
	LDD  R30,Y+6
	ANDI R30,LOW(0x40)
	BREQ _0x20029
	LDI  R30,LOW(12)
	RJMP _0x2002A
_0x20029:
	LDI  R30,LOW(4)
_0x2002A:
	MOV  R19,R30
; 0001 00C6 				}
; 0001 00C7 			}
_0x20023:
; 0001 00C8 		} else {							// SDv1 or MMCv3
_0x2001B:
	RJMP _0x2002C
_0x20017:
; 0001 00C9 			if (send_cmd(ACMD41, 0) <= 1) 	{
	LDI  R30,LOW(233)
	RCALL SUBOPT_0x4
	CPI  R30,LOW(0x2)
	BRSH _0x2002D
; 0001 00CA 				ty = CT_SD1; cmd = ACMD41;	// SDv1
	LDI  R19,LOW(2)
	LDI  R16,LOW(233)
; 0001 00CB 			} else {
	RJMP _0x2002E
_0x2002D:
; 0001 00CC 				ty = CT_MMC; cmd = CMD1;	// MMCv3
	LDI  R19,LOW(1)
	LDI  R16,LOW(65)
; 0001 00CD 			}
_0x2002E:
; 0001 00CE 			for (tmr = 25000; tmr && send_cmd(cmd, 0); tmr--) ;	// Wait for leaving idle state
	__GETWRN 20,21,25000
_0x20030:
	RCALL SUBOPT_0x9
	BREQ _0x20032
	ST   -Y,R16
	RCALL SUBOPT_0x7
	RCALL SUBOPT_0xA
	BRNE _0x20033
_0x20032:
	RJMP _0x20031
_0x20033:
	__SUBWRN 20,21,1
	RJMP _0x20030
_0x20031:
; 0001 00CF 			if (!tmr || send_cmd(CMD16, 512) != 0)			// Set R/W block length to 512
	RCALL SUBOPT_0x9
	BREQ _0x20035
	LDI  R30,LOW(80)
	ST   -Y,R30
	RCALL SUBOPT_0xB
	RCALL SUBOPT_0xA
	BREQ _0x20034
_0x20035:
; 0001 00D0 				ty = 0;
	LDI  R19,LOW(0)
; 0001 00D1 		}
_0x20034:
_0x2002C:
; 0001 00D2 	}
; 0001 00D3 	CardType = ty;
_0x20016:
	STS  _CardType_G001,R19
; 0001 00D4 	release_spi();
	RCALL _release_spi_G001
; 0001 00D5 
; 0001 00D6 #ifdef hardware_spi
; 0001 00D7 	//дл€ аппаратного SPI!!!--------------------------------------------
; 0001 00D8 	SPCR &= ~((1 << SPR1) | (1 << SPR0)); // убираем предделитель
	IN   R30,0xD
	ANDI R30,LOW(0xFC)
	OUT  0xD,R30
; 0001 00D9 	SPSR |= (1 << SPI2X); // удваиваем частоту
	SBI  0xE,0
; 0001 00DA 	//------------------------------------------------------------------
; 0001 00DB #endif
; 0001 00DC 
; 0001 00DD 	return ty ? 0 : STA_NOINIT;
	CPI  R19,0
	BREQ _0x20037
	LDI  R30,LOW(0)
	RJMP _0x20038
_0x20037:
	LDI  R30,LOW(1)
_0x20038:
	RCALL __LOADLOCR6
	RJMP _0x200000A
; 0001 00DE }
;//-----------------------------------------------------------------------
;// Read partial sector
;//-----------------------------------------------------------------------
;
;DRESULT disk_readp (
; 0001 00E4 	BYTE *buff,		// Pointer to the read buffer (NULL:Read bytes are forwarded to the stream)
; 0001 00E5 	DWORD lba,		// Sector number (LBA)
; 0001 00E6 	WORD ofs,		// Byte offset to read from (0..511)
; 0001 00E7 	WORD cnt		// Number of bytes to read (ofs + cnt mus be <= 512)
; 0001 00E8 )
; 0001 00E9 {
_disk_readp:
; 0001 00EA 	DRESULT res;
; 0001 00EB 	BYTE rc;
; 0001 00EC 	WORD bc;
; 0001 00ED 
; 0001 00EE //	if ((PINB&_BV(SD_INS))!=0x00) return RES_ERROR;
; 0001 00EF 
; 0001 00F0 	if (!(CardType & CT_BLOCK)) lba *= 512;		// Convert to byte address if needed
	RCALL SUBOPT_0xC
	RCALL __SAVELOCR4
;	*buff -> Y+12
;	lba -> Y+8
;	ofs -> Y+6
;	cnt -> Y+4
;	res -> R17
;	rc -> R16
;	bc -> R18,R19
	LDS  R30,_CardType_G001
	ANDI R30,LOW(0x8)
	BRNE _0x2003A
	RCALL SUBOPT_0xD
	RCALL SUBOPT_0xB
	RCALL __MULD12U
	RCALL SUBOPT_0xE
; 0001 00F1 
; 0001 00F2 	res = RES_ERROR;
_0x2003A:
	LDI  R17,LOW(1)
; 0001 00F3 	if (send_cmd(CMD17, lba) == 0) {		// READ_SINGLE_BLOCK
	LDI  R30,LOW(81)
	ST   -Y,R30
	RCALL SUBOPT_0xF
	RCALL SUBOPT_0xA
	BRNE _0x2003B
; 0001 00F4 
; 0001 00F5 		bc = 30000;
	__GETWRN 18,19,30000
; 0001 00F6 		do {							// Wait for data packet in timeout of 100ms
_0x2003D:
; 0001 00F7 			rc = rcv_spi();
	RCALL _rcv_spi
	MOV  R16,R30
; 0001 00F8 		} while (rc == 0xFF && --bc);
	CPI  R16,255
	BRNE _0x2003F
	RCALL SUBOPT_0x10
	BRNE _0x20040
_0x2003F:
	RJMP _0x2003E
_0x20040:
	RJMP _0x2003D
_0x2003E:
; 0001 00F9 
; 0001 00FA 		if (rc == 0xFE) {				// A data packet arrived
	CPI  R16,254
	BRNE _0x20041
; 0001 00FB 			bc = 514 - ofs - cnt;
	RCALL SUBOPT_0x11
	LDI  R30,LOW(514)
	LDI  R31,HIGH(514)
	SUB  R30,R26
	SBC  R31,R27
	MOVW R26,R30
	RCALL SUBOPT_0x12
	SUB  R26,R30
	SBC  R27,R31
	MOVW R18,R26
; 0001 00FC 
; 0001 00FD 			// Skip leading bytes
; 0001 00FE 			if (ofs) {
	RCALL SUBOPT_0x13
	SBIW R30,0
	BREQ _0x20042
; 0001 00FF 				do rcv_spi(); while (--ofs);
_0x20044:
	RCALL _rcv_spi
	RCALL SUBOPT_0x14
	BRNE _0x20044
; 0001 0100 			}
; 0001 0101 
; 0001 0102 			// Receive a part of the sector
; 0001 0103 			if (buff) {	// Store data to the memory
_0x20042:
	LDD  R30,Y+12
	LDD  R31,Y+12+1
	SBIW R30,0
	BREQ _0x20046
; 0001 0104 				do
_0x20048:
; 0001 0105 					*buff++ = rcv_spi();
	LDD  R30,Y+12
	LDD  R31,Y+12+1
	ADIW R30,1
	STD  Y+12,R30
	STD  Y+12+1,R31
	SBIW R30,1
	PUSH R31
	PUSH R30
	RCALL _rcv_spi
	POP  R26
	POP  R27
	ST   X,R30
; 0001 0106 				while (--cnt);
	RCALL SUBOPT_0x15
	BRNE _0x20048
; 0001 0107 			} else {	// Forward data to the outgoing stream (depends on the project)
	RJMP _0x2004A
_0x20046:
; 0001 0108 				do
_0x2004C:
; 0001 0109                 ;//uart_transmit(rcv_spi());		// (Console output)
; 0001 010A 				while (--cnt);
	RCALL SUBOPT_0x15
	BRNE _0x2004C
; 0001 010B 			}
_0x2004A:
; 0001 010C 
; 0001 010D 			// Skip trailing bytes and CRC
; 0001 010E 			do rcv_spi(); while (--bc);
_0x2004F:
	RCALL _rcv_spi
	MOVW R30,R18
	SBIW R30,1
	MOVW R18,R30
	BRNE _0x2004F
; 0001 010F 
; 0001 0110 			res = RES_OK;
	LDI  R17,LOW(0)
; 0001 0111 		}
; 0001 0112 	}
_0x20041:
; 0001 0113 
; 0001 0114 	release_spi();
_0x2003B:
	RCALL _release_spi_G001
; 0001 0115 
; 0001 0116 	return res;
	RCALL SUBOPT_0x16
	RJMP _0x2000008
; 0001 0117 }
;
;//-----------------------------------------------------------------------
;// Write partial sector
;//-----------------------------------------------------------------------
;#if _WRITE_FUNC
;
;DRESULT disk_writep (
; 0001 011F 	const BYTE *buff,	// Pointer to the bytes to be written (NULL:Initiate/Finalize sector write)
; 0001 0120 	DWORD sa			// Number of bytes to send, Sector number (LBA) or zero
; 0001 0121 )
; 0001 0122 {
_disk_writep:
; 0001 0123 	DRESULT res;
; 0001 0124 	WORD bc;
; 0001 0125 	static WORD wc;
; 0001 0126 
; 0001 0127 //	if ((PINB&_BV(SD_INS))!=0x00) return RES_ERROR;
; 0001 0128 //	if ((PINB&_BV(SD_WP))!=0x00) return RES_ERROR;
; 0001 0129 
; 0001 012A 	res = RES_ERROR;
	RCALL __PUTPARD2
	RCALL __SAVELOCR4
;	*buff -> Y+8
;	sa -> Y+4
;	res -> R17
;	bc -> R18,R19
	LDI  R17,LOW(1)
; 0001 012B 
; 0001 012C 	if (buff) {		// Send data bytes
	RCALL SUBOPT_0x17
	SBIW R30,0
	BREQ _0x20051
; 0001 012D 		bc = (WORD)sa;
	__GETWRS 18,19,4
; 0001 012E 		while (bc && wc) {		// Send data bytes to the card
_0x20052:
	RCALL SUBOPT_0x18
	BREQ _0x20055
	RCALL SUBOPT_0x19
	SBIW R30,0
	BRNE _0x20056
_0x20055:
	RJMP _0x20054
_0x20056:
; 0001 012F 			xmit_spi(*buff++);
	RCALL SUBOPT_0x1A
	LD   R30,X+
	STD  Y+8,R26
	STD  Y+8+1,R27
	MOV  R26,R30
	RCALL _xmit_spi
; 0001 0130 			wc--; bc--;
	LDI  R26,LOW(_wc_S0010007000)
	LDI  R27,HIGH(_wc_S0010007000)
	LD   R30,X+
	LD   R31,X+
	SBIW R30,1
	ST   -X,R31
	ST   -X,R30
	RCALL SUBOPT_0x10
; 0001 0131 		}
	RJMP _0x20052
_0x20054:
; 0001 0132 		res = RES_OK;
	LDI  R17,LOW(0)
; 0001 0133 	} else {
	RJMP _0x20057
_0x20051:
; 0001 0134 		if (sa) {	// Initiate sector write process
	RCALL SUBOPT_0x1B
	RCALL __CPD10
	BREQ _0x20058
; 0001 0135 			if (!(CardType & CT_BLOCK)) sa *= 512;	// Convert to byte address if needed
	LDS  R30,_CardType_G001
	ANDI R30,LOW(0x8)
	BRNE _0x20059
	RCALL SUBOPT_0x1B
	RCALL SUBOPT_0xB
	RCALL __MULD12U
	RCALL SUBOPT_0x1C
; 0001 0136 			if (send_cmd(CMD24, sa) == 0) {			// WRITE_SINGLE_BLOCK
_0x20059:
	LDI  R30,LOW(88)
	ST   -Y,R30
	RCALL SUBOPT_0x1D
	RCALL SUBOPT_0xA
	BRNE _0x2005A
; 0001 0137 				xmit_spi(0xFF); xmit_spi(0xFE);		// Data block header
	LDI  R26,LOW(255)
	RCALL _xmit_spi
	LDI  R26,LOW(254)
	RCALL _xmit_spi
; 0001 0138 				wc = 512;							// Set byte counter
	LDI  R30,LOW(512)
	LDI  R31,HIGH(512)
	STS  _wc_S0010007000,R30
	STS  _wc_S0010007000+1,R31
; 0001 0139 				res = RES_OK;
	LDI  R17,LOW(0)
; 0001 013A 			}
; 0001 013B 		} else {	// Finalize sector write process
_0x2005A:
	RJMP _0x2005B
_0x20058:
; 0001 013C 			bc = wc + 2;
	RCALL SUBOPT_0x19
	ADIW R30,2
	MOVW R18,R30
; 0001 013D 			while (bc--) xmit_spi(0);	// Fill left bytes and CRC with zeros
_0x2005C:
	MOVW R30,R18
	RCALL SUBOPT_0x10
	SBIW R30,0
	BREQ _0x2005E
	LDI  R26,LOW(0)
	RCALL _xmit_spi
	RJMP _0x2005C
_0x2005E:
; 0001 013E if ((rcv_spi() & 0x1F) == 0x05) {
	RCALL _rcv_spi
	ANDI R30,LOW(0x1F)
	CPI  R30,LOW(0x5)
	BRNE _0x2005F
; 0001 013F 				for (bc = 65000; rcv_spi() != 0xFF && bc; bc--) ;	// Wait ready
	__GETWRN 18,19,-536
_0x20061:
	RCALL _rcv_spi
	CPI  R30,LOW(0xFF)
	BREQ _0x20063
	RCALL SUBOPT_0x18
	BRNE _0x20064
_0x20063:
	RJMP _0x20062
_0x20064:
	RCALL SUBOPT_0x10
	RJMP _0x20061
_0x20062:
; 0001 0140 				if (bc) res = RES_OK;
	RCALL SUBOPT_0x18
	BREQ _0x20065
	LDI  R17,LOW(0)
; 0001 0141 			}
_0x20065:
; 0001 0142 			release_spi();
_0x2005F:
	RCALL _release_spi_G001
; 0001 0143 		}
_0x2005B:
; 0001 0144 	}
_0x20057:
; 0001 0145 
; 0001 0146 	return res;
	MOV  R30,R17
	RJMP _0x2000009
; 0001 0147 }
;#endif
;/*----------------------------------------------------------------------------/
;/  Petit FatFs - FAT file system module  R0.02                 (C)ChaN, 2009
;/-----------------------------------------------------------------------------/
;/ Petit FatFs module is an open source software to implement FAT file system to
;/ small embedded systems. This is a free software and is opened for education,
;/ research and commercial developments under license policy of following trems.
;/
;/  Copyright (C) 2009, ChaN, all right reserved.
;/
;/ * The Petit FatFs module is a free software and there is NO WARRANTY.
;/ * No restriction on use. You can use, modify and redistribute it for
;/   personal, non-profit or commercial use UNDER YOUR RESPONSIBILITY.
;/ * Redistributions of source code must retain the above copyright notice.
;/
;/-----------------------------------------------------------------------------/
;/ Jun 15,'09  R0.01a  First release. (Branched from FatFs R0.07b.)
;/
;/ Dec 14,'09  R0.02   Added multiple code page support.
;/                     Added write funciton.
;/                     Changed stream read mode interface.
;/----------------------------------------------------------------------------*/
;
;#include "pff.h"		/* Petit FatFs configurations and declarations */
;#include "diskio.h"		/* Declarations of low level disk I/O functions */
;
;
;/*--------------------------------------------------------------------------
;
;   Private Functions
;
;---------------------------------------------------------------------------*/
;
;static
;FATFS *FatFs;	/* Pointer to the file system object (logical drive) */
;
;
;
;/*-----------------------------------------------------------------------*/
;/* String functions                                                      */
;/*-----------------------------------------------------------------------*/
;
;/* Fill memory */
;static
;void mem_set (void* dst, int val, int cnt) {
; 0002 002C void mem_set (void* dst, int val, int cnt) {

	.CSEG
_mem_set_G002:
; 0002 002D 	char *d = (char*)dst;
; 0002 002E 	while (cnt--) *d++ = (char)val;
	RCALL SUBOPT_0xC
	RCALL __SAVELOCR2
;	*dst -> Y+6
;	val -> Y+4
;	cnt -> Y+2
;	*d -> R16,R17
	__GETWRS 16,17,6
_0x40003:
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	SBIW R30,1
	STD  Y+2,R30
	STD  Y+2+1,R31
	ADIW R30,1
	BREQ _0x40005
	PUSH R17
	PUSH R16
	__ADDWRN 16,17,1
	LDD  R30,Y+4
	POP  R26
	POP  R27
	ST   X,R30
	RJMP _0x40003
_0x40005:
; 0002 002F }
	RCALL __LOADLOCR2
	RJMP _0x2000007
;
;/* Compare memory to memory */
;static
;int mem_cmp (const void* dst, const void* src, int cnt) {
; 0002 0033 int mem_cmp (const void* dst, const void* src, int cnt) {
_mem_cmp_G002:
; 0002 0034 	const char *d = (const char *)dst, *s = (const char *)src;
; 0002 0035 	int r = 0;
; 0002 0036 	while (cnt-- && (r = *d++ - *s++) == 0) ;
	RCALL SUBOPT_0xC
	RCALL __SAVELOCR6
;	*dst -> Y+10
;	*src -> Y+8
;	cnt -> Y+6
;	*d -> R16,R17
;	*s -> R18,R19
;	r -> R20,R21
	__GETWRS 16,17,10
	__GETWRS 18,19,8
	__GETWRN 20,21,0
_0x40006:
	RCALL SUBOPT_0x14
	ADIW R30,1
	BREQ _0x40009
	MOVW R26,R16
	__ADDWRN 16,17,1
	LD   R0,X
	CLR  R1
	MOVW R26,R18
	__ADDWRN 18,19,1
	LD   R26,X
	CLR  R27
	MOVW R30,R0
	SUB  R30,R26
	SBC  R31,R27
	MOVW R20,R30
	SBIW R30,0
	BREQ _0x4000A
_0x40009:
	RJMP _0x40008
_0x4000A:
	RJMP _0x40006
_0x40008:
; 0002 0037 	return r;
	MOVW R30,R20
	RCALL __LOADLOCR6
	ADIW R28,12
	RET
; 0002 0038 }
;
;
;
;/*-----------------------------------------------------------------------*/
;/* FAT access - Read value of a FAT entry                                */
;/*-----------------------------------------------------------------------*/
;
;static
;CLUST get_fat (	/* 1:IO error, Else:Cluster status */
; 0002 0042 	CLUST clst	/* Cluster# to get the link information */
; 0002 0043 )
; 0002 0044 {
_get_fat_G002:
; 0002 0045 	WORD wc, bc, ofs;
; 0002 0046 	BYTE buf[4];
; 0002 0047 	FATFS *fs = FatFs;
; 0002 0048 
; 0002 0049 
; 0002 004A 	if (clst < 2 || clst >= fs->max_clust)	/* Range check */
	RCALL __PUTPARD2
	SBIW R28,6
	RCALL __SAVELOCR6
;	clst -> Y+12
;	wc -> R16,R17
;	bc -> R18,R19
;	ofs -> R20,R21
;	buf -> Y+8
;	*fs -> Y+6
	RCALL SUBOPT_0x1E
	STD  Y+6,R30
	STD  Y+6+1,R31
	RCALL SUBOPT_0x1F
	RCALL SUBOPT_0x20
	BRLO _0x4000C
	RCALL SUBOPT_0x11
	RCALL SUBOPT_0x21
	RCALL SUBOPT_0x1F
	RCALL __CPD21
	BRLO _0x4000B
_0x4000C:
; 0002 004B 		return 1;
	RJMP _0x200000C
; 0002 004C 
; 0002 004D 	switch (fs->fs_type) {
_0x4000B:
	RCALL SUBOPT_0x11
	LD   R30,X
	LDI  R31,0
; 0002 004E 	case FS_FAT12 :
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	BRNE _0x40011
; 0002 004F 		bc = (WORD)clst; bc += bc / 2;
	__GETWRS 18,19,12
	MOVW R30,R18
	LSR  R31
	ROR  R30
	__ADDWRR 18,19,30,31
; 0002 0050 		ofs = bc % 512; bc /= 512;
	MOVW R30,R18
	ANDI R31,HIGH(0x1FF)
	MOVW R20,R30
	MOVW R26,R18
	LDI  R30,LOW(512)
	LDI  R31,HIGH(512)
	RCALL __DIVW21U
	MOVW R18,R30
; 0002 0051 		if (ofs != 511) {
	LDI  R30,LOW(511)
	LDI  R31,HIGH(511)
	CP   R30,R20
	CPC  R31,R21
	BREQ _0x40012
; 0002 0052 			if (disk_readp(buf, fs->fatbase + bc, ofs, 2)) break;
	RCALL SUBOPT_0x22
	ST   -Y,R21
	ST   -Y,R20
	RCALL SUBOPT_0x23
	BREQ _0x40013
	RJMP _0x40010
; 0002 0053 		} else {
_0x40013:
	RJMP _0x40014
_0x40012:
; 0002 0054 			if (disk_readp(buf, fs->fatbase + bc, 511, 1)) break;
	RCALL SUBOPT_0x22
	LDI  R30,LOW(511)
	LDI  R31,HIGH(511)
	RCALL SUBOPT_0x24
	BREQ _0x40015
	RJMP _0x40010
; 0002 0055 			if (disk_readp(buf+1, fs->fatbase + bc + 1, 0, 1)) break;
_0x40015:
	MOVW R30,R28
	ADIW R30,9
	RCALL SUBOPT_0x25
	MOVW R30,R18
	RCALL SUBOPT_0x26
	RCALL __ADDD12
	__ADDD1N 1
	RCALL __PUTPARD1
	RCALL SUBOPT_0x0
	RCALL SUBOPT_0x24
	BREQ _0x40016
	RJMP _0x40010
; 0002 0056 		}
_0x40016:
_0x40014:
; 0002 0057 		wc = LD_WORD(buf);
	__GETWRS 16,17,8
; 0002 0058 		return (clst & 1) ? (wc >> 4) : (wc & 0xFFF);
	LDD  R30,Y+12
	ANDI R30,LOW(0x1)
	BREQ _0x40017
	MOVW R30,R16
	RCALL __LSRW4
	RJMP _0x400CD
_0x40017:
	MOVW R30,R16
	ANDI R31,HIGH(0xFFF)
_0x400CD:
	CLR  R22
	CLR  R23
	RJMP _0x200000B
; 0002 0059 
; 0002 005A 	case FS_FAT16 :
_0x40011:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x4001A
; 0002 005B 		if (disk_readp(buf, fs->fatbase + clst / 256, (WORD)(((WORD)clst % 256) * 2), 2)) break;
	MOVW R30,R28
	ADIW R30,8
	RCALL SUBOPT_0x25
	PUSH R25
	PUSH R24
	PUSH R27
	PUSH R26
	RCALL SUBOPT_0x27
	__GETD1N 0x100
	RCALL __DIVD21U
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RCALL SUBOPT_0x28
	ANDI R31,HIGH(0xFF)
	LSL  R30
	ROL  R31
	RCALL SUBOPT_0x29
	BRNE _0x40010
; 0002 005C 		return LD_WORD(buf);
	RCALL SUBOPT_0x17
	RCALL SUBOPT_0x26
	RJMP _0x200000B
; 0002 005D #if _FS_FAT32
; 0002 005E 	case FS_FAT32 :
_0x4001A:
	CPI  R30,LOW(0x3)
	LDI  R26,HIGH(0x3)
	CPC  R31,R26
	BRNE _0x40010
; 0002 005F 		if (disk_readp(buf, fs->fatbase + clst / 128, (WORD)(((WORD)clst % 128) * 4), 4)) break;
	MOVW R30,R28
	ADIW R30,8
	RCALL SUBOPT_0x25
	PUSH R25
	PUSH R24
	PUSH R27
	PUSH R26
	RCALL SUBOPT_0x27
	__GETD1N 0x80
	RCALL __DIVD21U
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RCALL SUBOPT_0x28
	ANDI R30,LOW(0x7F)
	ANDI R31,HIGH(0x7F)
	RCALL __LSLW2
	RCALL SUBOPT_0x2
	LDI  R26,LOW(4)
	RCALL SUBOPT_0x2A
	BRNE _0x40010
; 0002 0060 		return LD_DWORD(buf) & 0x0FFFFFFF;
	RCALL SUBOPT_0xD
	__ANDD1N 0xFFFFFFF
	RJMP _0x200000B
; 0002 0061 #endif
; 0002 0062 	}
_0x40010:
; 0002 0063 
; 0002 0064 	return 1;	/* An error occured at the disk I/O layer */
_0x200000C:
	__GETD1N 0x1
_0x200000B:
	RCALL __LOADLOCR6
	ADIW R28,16
	RET
; 0002 0065 }
;
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Get sector# from cluster#                                             */
;/*-----------------------------------------------------------------------*/
;
;static
;DWORD clust2sect (	/* !=0: Sector number, 0: Failed - invalid cluster# */
; 0002 0070 	CLUST clst		/* Cluster# to be converted */
; 0002 0071 )
; 0002 0072 {
_clust2sect_G002:
; 0002 0073 	FATFS *fs = FatFs;
; 0002 0074 
; 0002 0075 
; 0002 0076 	clst -= 2;
	RCALL __PUTPARD2
	RCALL SUBOPT_0x2B
;	clst -> Y+2
;	*fs -> R16,R17
	RCALL SUBOPT_0x6
	__SUBD1N 2
	RCALL SUBOPT_0x2C
; 0002 0077 	if (clst >= (fs->max_clust - 2)) return 0;		/* Invalid cluster# */
	MOVW R30,R16
	RCALL SUBOPT_0x2D
	__GETD1N 0x2
	RCALL __SWAPD12
	RCALL __SUBD12
	RCALL SUBOPT_0x2E
	RCALL __CPD21
	BRLO _0x4001E
	RCALL SUBOPT_0x2F
	RCALL __LOADLOCR2
	RJMP _0x2000006
; 0002 0078 	return (DWORD)clst * fs->csize + fs->database;
_0x4001E:
	MOVW R30,R16
	RCALL SUBOPT_0x30
	RCALL SUBOPT_0x2E
	RCALL __CWD1
	RCALL __MULD12U
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	MOVW R26,R16
	ADIW R26,20
	RCALL __GETD1P
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RCALL __ADDD12
	RCALL __LOADLOCR2
	RJMP _0x2000006
; 0002 0079 }
;
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Directory handling - Rewind directory index                           */
;/*-----------------------------------------------------------------------*/
;
;static
;FRESULT dir_rewind (
; 0002 0084 	DIR *dj			/* Pointer to directory object */
; 0002 0085 )
; 0002 0086 {
_dir_rewind_G002:
; 0002 0087 	CLUST clst;
; 0002 0088 	FATFS *fs = FatFs;
; 0002 0089 
; 0002 008A 
; 0002 008B 	dj->index = 0;
	RCALL SUBOPT_0xC
	SBIW R28,4
	RCALL SUBOPT_0x2B
;	*dj -> Y+6
;	clst -> Y+2
;	*fs -> R16,R17
	RCALL SUBOPT_0x11
	RCALL SUBOPT_0x0
	ST   X+,R30
	ST   X,R31
; 0002 008C 	clst = dj->sclust;
	RCALL SUBOPT_0x11
	ADIW R26,4
	RCALL __GETD1P
	RCALL SUBOPT_0x2C
; 0002 008D 	if (clst == 1 || clst >= fs->max_clust)	/* Check start cluster range */
	RCALL SUBOPT_0x2E
	__CPD2N 0x1
	BREQ _0x40020
	MOVW R26,R16
	RCALL SUBOPT_0x21
	RCALL SUBOPT_0x2E
	RCALL __CPD21
	BRLO _0x4001F
_0x40020:
; 0002 008E 		return FR_DISK_ERR;
	LDI  R30,LOW(1)
	RCALL __LOADLOCR2
	RJMP _0x2000007
; 0002 008F #if _FS_FAT32
; 0002 0090 	if (!clst && fs->fs_type == FS_FAT32)	/* Replace cluster# 0 with root cluster# if in FAT32 */
_0x4001F:
	RCALL SUBOPT_0x6
	RCALL __CPD10
	BRNE _0x40023
	MOVW R26,R16
	LD   R26,X
	CPI  R26,LOW(0x3)
	BREQ _0x40024
_0x40023:
	RJMP _0x40022
_0x40024:
; 0002 0091 		clst = fs->dirbase;
	MOVW R26,R16
	ADIW R26,16
	RCALL __GETD1P
	RCALL SUBOPT_0x2C
; 0002 0092 #endif
; 0002 0093 	dj->clust = clst;						/* Current cluster */
_0x40022:
	RCALL SUBOPT_0x6
	__PUTD1SNS 6,8
; 0002 0094 	dj->sect = clst ? clust2sect(clst) : fs->dirbase;	/* Current sector */
	RCALL SUBOPT_0x6
	RCALL __CPD10
	BREQ _0x40025
	RCALL SUBOPT_0x2E
	RCALL _clust2sect_G002
	RJMP _0x40026
_0x40025:
	MOVW R26,R16
	ADIW R26,16
	RCALL __GETD1P
_0x40026:
	__PUTD1SNS 6,12
; 0002 0095 
; 0002 0096 	return FR_OK;	/* Seek succeeded */
	LDI  R30,LOW(0)
	RCALL __LOADLOCR2
	RJMP _0x2000007
; 0002 0097 }
;
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Directory handling - Move directory index next                        */
;/*-----------------------------------------------------------------------*/
;
;static
;FRESULT dir_next (	/* FR_OK:Succeeded, FR_NO_FILE:End of table */
; 0002 00A2 	DIR *dj			/* Pointer to directory object */
; 0002 00A3 )
; 0002 00A4 {
_dir_next_G002:
; 0002 00A5 	CLUST clst;
; 0002 00A6 	WORD i;
; 0002 00A7 	FATFS *fs = FatFs;
; 0002 00A8 
; 0002 00A9 
; 0002 00AA 	i = dj->index + 1;
	RCALL SUBOPT_0xC
	SBIW R28,4
	RCALL SUBOPT_0x31
;	*dj -> Y+8
;	clst -> Y+4
;	i -> R16,R17
;	*fs -> R18,R19
	RCALL SUBOPT_0x1A
	RCALL __GETW1P
	ADIW R30,1
	MOVW R16,R30
; 0002 00AB 	if (!i || !dj->sect)	/* Report EOT when index has reached 65535 */
	MOV  R0,R16
	OR   R0,R17
	BREQ _0x40029
	RCALL SUBOPT_0x1A
	ADIW R26,12
	RCALL __GETD1P
	RCALL __CPD10
	BRNE _0x40028
_0x40029:
; 0002 00AC 		return FR_NO_FILE;
	LDI  R30,LOW(3)
	RJMP _0x2000009
; 0002 00AD 
; 0002 00AE 	if (!(i & (16-1))) {	/* Sector changed? */
_0x40028:
	MOVW R30,R16
	ANDI R30,LOW(0xF)
	BRNE _0x4002B
; 0002 00AF 		dj->sect++;			/* Next sector */
	RCALL SUBOPT_0x1A
	ADIW R26,12
	RCALL __GETD1P_INC
	__SUBD1N -1
	RCALL __PUTDP1_DEC
; 0002 00B0 
; 0002 00B1 		if (dj->clust == 0) {	/* Static table */
	RCALL SUBOPT_0x1A
	RCALL SUBOPT_0x21
	RCALL __CPD10
	BRNE _0x4002C
; 0002 00B2 			if (i >= fs->n_rootdir)	/* Report EOT when end of table */
	MOVW R26,R18
	ADIW R26,4
	RCALL __GETW1P
	CP   R16,R30
	CPC  R17,R31
	BRLO _0x4002D
; 0002 00B3 				return FR_NO_FILE;
	LDI  R30,LOW(3)
	RJMP _0x2000009
; 0002 00B4 		}
_0x4002D:
; 0002 00B5 		else {					/* Dynamic table */
	RJMP _0x4002E
_0x4002C:
; 0002 00B6 			if (((i / 16) & (fs->csize-1)) == 0) {	/* Cluster changed? */
	MOVW R30,R16
	RCALL __LSRW4
	MOVW R26,R30
	MOVW R30,R18
	RCALL SUBOPT_0x30
	SBIW R30,1
	AND  R30,R26
	AND  R31,R27
	SBIW R30,0
	BRNE _0x4002F
; 0002 00B7 				clst = get_fat(dj->clust);		/* Get next cluster */
	RCALL SUBOPT_0x17
	RCALL SUBOPT_0x2D
	RCALL _get_fat_G002
	RCALL SUBOPT_0x1C
; 0002 00B8 				if (clst <= 1) return FR_DISK_ERR;
	RCALL SUBOPT_0x32
	RCALL SUBOPT_0x20
	BRSH _0x40030
	LDI  R30,LOW(1)
	RJMP _0x2000009
; 0002 00B9 				if (clst >= fs->max_clust)		/* When it reached end of dynamic table */
_0x40030:
	MOVW R26,R18
	RCALL SUBOPT_0x21
	RCALL SUBOPT_0x32
	RCALL __CPD21
	BRLO _0x40031
; 0002 00BA 					return FR_NO_FILE;			/* Report EOT */
	LDI  R30,LOW(3)
	RJMP _0x2000009
; 0002 00BB 				dj->clust = clst;				/* Initialize data for new cluster */
_0x40031:
	RCALL SUBOPT_0x1B
	__PUTD1SNS 8,8
; 0002 00BC 				dj->sect = clust2sect(clst);
	RCALL SUBOPT_0x32
	RCALL _clust2sect_G002
	__PUTD1SNS 8,12
; 0002 00BD 			}
; 0002 00BE 		}
_0x4002F:
_0x4002E:
; 0002 00BF 	}
; 0002 00C0 
; 0002 00C1 	dj->index = i;
_0x4002B:
	RCALL SUBOPT_0x1A
	ST   X+,R16
	ST   X,R17
; 0002 00C2 
; 0002 00C3 	return FR_OK;
	LDI  R30,LOW(0)
_0x2000009:
	RCALL __LOADLOCR4
_0x200000A:
	ADIW R28,10
	RET
; 0002 00C4 }
;
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Directory handling - Find an object in the directory                  */
;/*-----------------------------------------------------------------------*/
;
;static
;FRESULT dir_find (
; 0002 00CF 	DIR *dj			/* Pointer to the directory object linked to the file name */
; 0002 00D0 )
; 0002 00D1 {
_dir_find_G002:
; 0002 00D2 	FRESULT res;
; 0002 00D3 	BYTE c, *dir;
; 0002 00D4 
; 0002 00D5 
; 0002 00D6 	res = dir_rewind(dj);			/* Rewind directory object */
	RCALL SUBOPT_0xC
	RCALL __SAVELOCR4
;	*dj -> Y+4
;	res -> R17
;	c -> R16
;	*dir -> R18,R19
	RCALL SUBOPT_0x33
	RCALL _dir_rewind_G002
	RCALL SUBOPT_0x34
; 0002 00D7 	if (res != FR_OK) return res;
	BREQ _0x40032
	RCALL SUBOPT_0x16
	RJMP _0x2000006
; 0002 00D8 
; 0002 00D9 	dir = FatFs->buf;
_0x40032:
	RCALL SUBOPT_0x35
; 0002 00DA 	do {
_0x40034:
; 0002 00DB 		res = disk_readp(dir, dj->sect, (WORD)((dj->index % 16) * 32), 32)	/* Read an entry */
; 0002 00DC 			? FR_DISK_ERR : FR_OK;
	ST   -Y,R19
	ST   -Y,R18
	RCALL SUBOPT_0x13
	RCALL SUBOPT_0x36
	RCALL __PUTPARD2
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	RCALL __GETW1P
	ANDI R30,LOW(0xF)
	ANDI R31,HIGH(0xF)
	LSL  R30
	RCALL __LSLW4
	RCALL SUBOPT_0x2
	LDI  R26,LOW(32)
	RCALL SUBOPT_0x2A
	BREQ _0x40036
	LDI  R30,LOW(1)
	RJMP _0x40037
_0x40036:
	LDI  R30,LOW(0)
_0x40037:
	RCALL SUBOPT_0x34
; 0002 00DD 		if (res != FR_OK) break;
	BRNE _0x40035
; 0002 00DE 		c = dir[DIR_Name];	/* First character */
	MOVW R26,R18
	LD   R16,X
; 0002 00DF 		if (c == 0) { res = FR_NO_FILE; break; }	/* Reached to end of table */
	CPI  R16,0
	BRNE _0x4003A
	LDI  R17,LOW(3)
	RJMP _0x40035
; 0002 00E0 		if (!(dir[DIR_Attr] & AM_VOL) && !mem_cmp(dir, dj->fn, 11)) /* Is it a valid entry? */
_0x4003A:
	MOVW R30,R18
	LDD  R30,Z+11
	ANDI R30,LOW(0x8)
	BRNE _0x4003C
	ST   -Y,R19
	ST   -Y,R18
	RCALL SUBOPT_0x13
	LDD  R26,Z+2
	LDD  R27,Z+3
	RCALL SUBOPT_0xC
	LDI  R26,LOW(11)
	LDI  R27,0
	RCALL _mem_cmp_G002
	SBIW R30,0
	BREQ _0x4003D
_0x4003C:
	RJMP _0x4003B
_0x4003D:
; 0002 00E1 			break;
	RJMP _0x40035
; 0002 00E2 		res = dir_next(dj);							/* Next entry */
_0x4003B:
	RCALL SUBOPT_0x33
	RCALL _dir_next_G002
	MOV  R17,R30
; 0002 00E3 	} while (res == FR_OK);
	CPI  R17,0
	BREQ _0x40034
_0x40035:
; 0002 00E4 
; 0002 00E5 	return res;
	RCALL SUBOPT_0x16
	RJMP _0x2000006
; 0002 00E6 }
;
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Read an object from the directory                                     */
;/*-----------------------------------------------------------------------*/
;#if _USE_DIR
;static
;FRESULT dir_read (
; 0002 00F1 	DIR *dj			/* Pointer to the directory object to store read object name */
; 0002 00F2 )
; 0002 00F3 {
; 0002 00F4 	FRESULT res;
; 0002 00F5 	BYTE a, c, *dir;
; 0002 00F6 
; 0002 00F7 
; 0002 00F8 	res = FR_NO_FILE;
;	*dj -> Y+6
;	res -> R17
;	a -> R16
;	c -> R19
;	*dir -> R20,R21
; 0002 00F9 	dir = FatFs->buf;
; 0002 00FA 	while (dj->sect) {
; 0002 00FB 		res = disk_readp(dir, dj->sect, (WORD)((dj->index % 16) * 32), 32)	/* Read an entry */
; 0002 00FC 			? FR_DISK_ERR : FR_OK;
; 0002 00FD 		if (res != FR_OK) break;
; 0002 00FE 		c = dir[DIR_Name];
; 0002 00FF 		if (c == 0) { res = FR_NO_FILE; break; }	/* Reached to end of table */
; 0002 0100 		a = dir[DIR_Attr] & AM_MASK;
; 0002 0101 		if (c != 0xE5 && c != '.' && !(a & AM_VOL))	/* Is it a valid entry? */
; 0002 0102 			break;
; 0002 0103 		res = dir_next(dj);				/* Next entry */
; 0002 0104 		if (res != FR_OK) break;
; 0002 0105 	}
; 0002 0106 
; 0002 0107 	if (res != FR_OK) dj->sect = 0;
; 0002 0108 
; 0002 0109 	return res;
; 0002 010A }
;#endif
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Pick a segment and create the object name in directory form           */
;/*-----------------------------------------------------------------------*/
;
;#ifdef _EXCVT
;	static const BYTE cvt[] = _EXCVT;

	.DSEG
;#endif
;
;static
;FRESULT create_name (
; 0002 0119 	DIR *dj,			/* Pointer to the directory object */
; 0002 011A 	const char **path	/* Pointer to pointer to the segment in the path string */
; 0002 011B )
; 0002 011C {

	.CSEG
_create_name_G002:
; 0002 011D 	BYTE c, d, ni, si, i, *sfn;
; 0002 011E 	const char *p;
; 0002 011F 
; 0002 0120 	/* Create file name in directory form */
; 0002 0121 	sfn = dj->fn;
	RCALL SUBOPT_0xC
	SBIW R28,4
	RCALL __SAVELOCR6
;	*dj -> Y+12
;	*path -> Y+10
;	c -> R17
;	d -> R16
;	ni -> R19
;	si -> R18
;	i -> R21
;	*sfn -> Y+8
;	*p -> Y+6
	LDD  R26,Y+12
	LDD  R27,Y+12+1
	ADIW R26,2
	RCALL __GETW1P
	STD  Y+8,R30
	STD  Y+8+1,R31
; 0002 0122 	mem_set(sfn, ' ', 11);
	RCALL SUBOPT_0x17
	RCALL SUBOPT_0x2
	LDI  R30,LOW(32)
	LDI  R31,HIGH(32)
	RCALL SUBOPT_0x2
	LDI  R26,LOW(11)
	LDI  R27,0
	RCALL _mem_set_G002
; 0002 0123 	si = i = 0; ni = 8;
	LDI  R30,LOW(0)
	MOV  R21,R30
	MOV  R18,R30
	LDI  R19,LOW(8)
; 0002 0124 	p = *path;
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	RCALL __GETW1P
	STD  Y+6,R30
	STD  Y+6+1,R31
; 0002 0125 	for (;;) {
_0x4004D:
; 0002 0126 		c = p[si++];
	RCALL SUBOPT_0x37
	LD   R17,X
; 0002 0127 		if (c <= ' ' || c == '/') break;	/* Break on end of segment */
	CPI  R17,33
	BRLO _0x40050
	CPI  R17,47
	BRNE _0x4004F
_0x40050:
	RJMP _0x4004E
; 0002 0128 		if (c == '.' || i >= ni) {
_0x4004F:
	CPI  R17,46
	BREQ _0x40053
	CP   R21,R19
	BRLO _0x40052
_0x40053:
; 0002 0129 			if (ni != 8 || c != '.') break;
	CPI  R19,8
	BRNE _0x40056
	CPI  R17,46
	BREQ _0x40055
_0x40056:
	RJMP _0x4004E
; 0002 012A 			i = 8; ni = 11;
_0x40055:
	LDI  R21,LOW(8)
	LDI  R19,LOW(11)
; 0002 012B 			continue;
	RJMP _0x4004C
; 0002 012C 		}
; 0002 012D #ifdef _EXCVT
; 0002 012E 		if (c >= 0x80)					/* To upper extended char (SBCS) */
_0x40052:
	CPI  R17,128
	BRLO _0x40058
; 0002 012F 			c = cvt[c - 0x80];
	MOV  R30,R17
	LDI  R31,0
	SUBI R30,LOW(128)
	SBCI R31,HIGH(128)
	SUBI R30,LOW(-_cvt_G002)
	SBCI R31,HIGH(-_cvt_G002)
	LD   R17,Z
; 0002 0130 #endif
; 0002 0131 		if (IsDBCS1(c) && i >= ni - 1) {	/* DBC 1st byte? */
_0x40058:
	LDI  R30,LOW(0)
	CPI  R30,0
	BREQ _0x4005A
	MOV  R30,R19
	LDI  R31,0
	SBIW R30,1
	MOV  R26,R21
	LDI  R27,0
	CP   R26,R30
	CPC  R27,R31
	BRGE _0x4005B
_0x4005A:
	RJMP _0x40059
_0x4005B:
; 0002 0132 			d = p[si++];					/* Get 2nd byte */
	RCALL SUBOPT_0x37
	LD   R16,X
; 0002 0133 			sfn[i++] = c;
	RCALL SUBOPT_0x38
	ST   Z,R17
; 0002 0134 			sfn[i++] = d;
	RCALL SUBOPT_0x38
	ST   Z,R16
; 0002 0135 		} else {						/* Single byte code */
	RJMP _0x4005C
_0x40059:
; 0002 0136 			if (IsLower(c)) c -= 0x20;	/* toupper */
	CPI  R17,97
	BRLO _0x4005E
	CPI  R17,123
	BRLO _0x4005F
_0x4005E:
	RJMP _0x4005D
_0x4005F:
	SUBI R17,LOW(32)
; 0002 0137 			sfn[i++] = c;
_0x4005D:
	RCALL SUBOPT_0x38
	ST   Z,R17
; 0002 0138 		}
_0x4005C:
; 0002 0139 	}
_0x4004C:
	RJMP _0x4004D
_0x4004E:
; 0002 013A 	*path = &p[si];						/* Rerurn pointer to the next segment */
	MOV  R30,R18
	RCALL SUBOPT_0x11
	LDI  R31,0
	ADD  R30,R26
	ADC  R31,R27
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	ST   X+,R30
	ST   X,R31
; 0002 013B 
; 0002 013C 	sfn[11] = (c <= ' ') ? 1 : 0;		/* Set last segment flag if end of path */
	CPI  R17,33
	BRSH _0x40060
	LDI  R30,LOW(1)
	RJMP _0x40061
_0x40060:
	LDI  R30,LOW(0)
_0x40061:
	__PUTB1SNS 8,11
; 0002 013D 
; 0002 013E 	return FR_OK;
	LDI  R30,LOW(0)
	RCALL __LOADLOCR6
_0x2000008:
	ADIW R28,14
	RET
; 0002 013F }
;
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Get file information from directory entry                             */
;/*-----------------------------------------------------------------------*/
;#if _USE_DIR
;static
;void get_fileinfo (		/* No return code */
; 0002 014A 	DIR *dj,			/* Pointer to the directory object */
; 0002 014B 	FILINFO *fno	 	/* Pointer to store the file information */
; 0002 014C )
; 0002 014D {
; 0002 014E 	BYTE i, c, *dir;
; 0002 014F 	char *p;
; 0002 0150 
; 0002 0151 
; 0002 0152 	p = fno->fname;
;	*dj -> Y+8
;	*fno -> Y+6
;	i -> R17
;	c -> R16
;	*dir -> R18,R19
;	*p -> R20,R21
; 0002 0153 	if (dj->sect) {
; 0002 0154 		dir = FatFs->buf;
; 0002 0155 		for (i = 0; i < 8; i++) {	/* Copy file name body */
; 0002 0156 			c = dir[i];
; 0002 0157 			if (c == ' ') break;
; 0002 0158 			if (c == 0x05) c = 0xE5;
; 0002 0159 			*p++ = c;
; 0002 015A 		}
; 0002 015B 		if (dir[8] != ' ') {		/* Copy file name extension */
; 0002 015C 			*p++ = '.';
; 0002 015D 			for (i = 8; i < 11; i++) {
; 0002 015E 				c = dir[i];
; 0002 015F 				if (c == ' ') break;
; 0002 0160 				*p++ = c;
; 0002 0161 			}
; 0002 0162 		}
; 0002 0163 		fno->fattrib = dir[DIR_Attr];				/* Attribute */
; 0002 0164 		fno->fsize = LD_DWORD(dir+DIR_FileSize);	/* Size */
; 0002 0165 		fno->fdate = LD_WORD(dir+DIR_WrtDate);		/* Date */
; 0002 0166 		fno->ftime = LD_WORD(dir+DIR_WrtTime);		/* Time */
; 0002 0167 	}
; 0002 0168 	*p = 0;
; 0002 0169 }
;#endif /* _USE_DIR */
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Follow a file path                                                    */
;/*-----------------------------------------------------------------------*/
;
;static
;FRESULT follow_path (	/* FR_OK(0): successful, !=0: error code */
; 0002 0174 	DIR *dj,			/* Directory object to return last directory and found object */
; 0002 0175 	const char *path	/* Full-path string to find a file or directory */
; 0002 0176 )
; 0002 0177 {
_follow_path_G002:
; 0002 0178 	FRESULT res;
; 0002 0179 	BYTE *dir;
; 0002 017A 
; 0002 017B 
; 0002 017C 	while (*path == ' ') path++;		/* Skip leading spaces */
	RCALL SUBOPT_0xC
	RCALL __SAVELOCR4
;	*dj -> Y+6
;	*path -> Y+4
;	res -> R17
;	*dir -> R18,R19
_0x4006E:
	RCALL SUBOPT_0x33
	LD   R26,X
	CPI  R26,LOW(0x20)
	BRNE _0x40070
	RCALL SUBOPT_0x39
	RJMP _0x4006E
_0x40070:
; 0002 017D if (*path == '/') path++;
	RCALL SUBOPT_0x33
	LD   R26,X
	CPI  R26,LOW(0x2F)
	BRNE _0x40071
	RCALL SUBOPT_0x39
; 0002 017E 	dj->sclust = 0;						/* Set start directory (always root dir) */
_0x40071:
	RCALL SUBOPT_0x11
	ADIW R26,4
	RCALL SUBOPT_0x2F
	RCALL __PUTDP1
; 0002 017F 
; 0002 0180 	if ((BYTE)*path <= ' ') {			/* Null path means the root directory */
	RCALL SUBOPT_0x33
	LD   R26,X
	CPI  R26,LOW(0x21)
	BRSH _0x40072
; 0002 0181 		res = dir_rewind(dj);
	RCALL SUBOPT_0x11
	RCALL _dir_rewind_G002
	MOV  R17,R30
; 0002 0182 		FatFs->buf[0] = 0;
	RCALL SUBOPT_0x1E
	LDD  R26,Z+6
	LDD  R27,Z+7
	RCALL SUBOPT_0x3A
; 0002 0183 
; 0002 0184 	} else {							/* Follow path */
	RJMP _0x40073
_0x40072:
; 0002 0185 		for (;;) {
_0x40075:
; 0002 0186 			res = create_name(dj, &path);	/* Get a segment */
	RCALL SUBOPT_0x13
	RCALL SUBOPT_0x2
	MOVW R26,R28
	ADIW R26,6
	RCALL _create_name_G002
	RCALL SUBOPT_0x34
; 0002 0187 			if (res != FR_OK) break;
	BRNE _0x40076
; 0002 0188 			res = dir_find(dj);				/* Find it */
	RCALL SUBOPT_0x11
	RCALL _dir_find_G002
	RCALL SUBOPT_0x34
; 0002 0189 			if (res != FR_OK) {				/* Could not find the object */
	BREQ _0x40078
; 0002 018A 				if (res == FR_NO_FILE && !*(dj->fn+11))
	CPI  R17,3
	BRNE _0x4007A
	RCALL SUBOPT_0x3B
	BREQ _0x4007B
_0x4007A:
	RJMP _0x40079
_0x4007B:
; 0002 018B 					res = FR_NO_PATH;
	LDI  R17,LOW(4)
; 0002 018C 				break;
_0x40079:
	RJMP _0x40076
; 0002 018D 			}
; 0002 018E 			if (*(dj->fn+11)) break;		/* Last segment match. Function completed. */
_0x40078:
	RCALL SUBOPT_0x3B
	BRNE _0x40076
; 0002 018F 			dir = FatFs->buf;				/* There is next segment. Follow the sub directory */
	RCALL SUBOPT_0x35
; 0002 0190 			if (!(dir[DIR_Attr] & AM_DIR)) { /* Cannot follow because it is a file */
	MOVW R30,R18
	LDD  R30,Z+11
	ANDI R30,LOW(0x10)
	BRNE _0x4007D
; 0002 0191 				res = FR_NO_PATH; break;
	LDI  R17,LOW(4)
	RJMP _0x40076
; 0002 0192 			}
; 0002 0193 			dj->sclust =
_0x4007D:
; 0002 0194 #if _FS_FAT32
; 0002 0195 				((DWORD)LD_WORD(dir+DIR_FstClusHI) << 16) |
; 0002 0196 #endif
; 0002 0197 				LD_WORD(dir+DIR_FstClusLO);
	MOVW R26,R18
	ADIW R26,20
	RCALL __GETW1P
	RCALL SUBOPT_0x26
	RCALL __LSLD16
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	MOVW R26,R18
	ADIW R26,26
	RCALL __GETW1P
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RCALL SUBOPT_0x26
	RCALL __ORD12
	__PUTD1SNS 6,4
; 0002 0198 		}
	RJMP _0x40075
_0x40076:
; 0002 0199 	}
_0x40073:
; 0002 019A 
; 0002 019B 	return res;
	RCALL SUBOPT_0x16
_0x2000007:
	ADIW R28,8
	RET
; 0002 019C }
;
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Check a sector if it is an FAT boot record                            */
;/*-----------------------------------------------------------------------*/
;
;static
;BYTE check_fs (	/* 0:The FAT boot record, 1:Valid boot record but not an FAT, 2:Not a boot record, 3:Error */
; 0002 01A7 	BYTE *buf,	/* Working buffer */
; 0002 01A8 	DWORD sect	/* Sector# (lba) to check if it is an FAT boot record or not */
; 0002 01A9 )
; 0002 01AA {
_check_fs_G002:
; 0002 01AB 	if (disk_readp(buf, sect, 510, 2))		/* Read the boot sector */
	RCALL __PUTPARD2
;	*buf -> Y+4
;	sect -> Y+0
	RCALL SUBOPT_0x3C
	LDI  R30,LOW(510)
	LDI  R31,HIGH(510)
	RCALL SUBOPT_0x29
	BREQ _0x4007E
; 0002 01AC 		return 3;
	LDI  R30,LOW(3)
	RJMP _0x2000006
; 0002 01AD 	if (LD_WORD(buf) != 0xAA55)				/* Check record signature */
_0x4007E:
	RCALL SUBOPT_0x33
	RCALL __GETW1P
	CPI  R30,LOW(0xAA55)
	LDI  R26,HIGH(0xAA55)
	CPC  R31,R26
	BREQ _0x4007F
; 0002 01AE 		return 2;
	LDI  R30,LOW(2)
	RJMP _0x2000006
; 0002 01AF 
; 0002 01B0 	if (!disk_readp(buf, sect, BS_FilSysType, 2) && LD_WORD(buf) == 0x4146)	/* Check FAT12/16 */
_0x4007F:
	RCALL SUBOPT_0x3C
	LDI  R30,LOW(54)
	LDI  R31,HIGH(54)
	RCALL SUBOPT_0x29
	BRNE _0x40081
	RCALL SUBOPT_0x3D
	BREQ _0x40082
_0x40081:
	RJMP _0x40080
_0x40082:
; 0002 01B1 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x2000006
; 0002 01B2 #if _FS_FAT32
; 0002 01B3 	if (!disk_readp(buf, sect, BS_FilSysType32, 2) && LD_WORD(buf) == 0x4146)	/* Check FAT32 */
_0x40080:
	RCALL SUBOPT_0x3C
	LDI  R30,LOW(82)
	LDI  R31,HIGH(82)
	RCALL SUBOPT_0x29
	BRNE _0x40084
	RCALL SUBOPT_0x3D
	BREQ _0x40085
_0x40084:
	RJMP _0x40083
_0x40085:
; 0002 01B4 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x2000006
; 0002 01B5 #endif
; 0002 01B6 	return 1;
_0x40083:
	LDI  R30,LOW(1)
_0x2000006:
	ADIW R28,6
	RET
; 0002 01B7 }
;
;
;
;
;/*--------------------------------------------------------------------------
;
;   Public Functions
;
;--------------------------------------------------------------------------*/
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Mount/Unmount a Locical Drive                                         */
;/*-----------------------------------------------------------------------*/
;
;FRESULT pf_mount (
; 0002 01C9 	FATFS *fs		/* Pointer to new file system object (NULL: Unmount) */
; 0002 01CA )
; 0002 01CB {
_pf_mount:
; 0002 01CC 	BYTE fmt, buf[36];
; 0002 01CD 	DWORD bsect, fsize, tsect, mclst;
; 0002 01CE 
; 0002 01CF 
; 0002 01D0 	FatFs = 0;
	RCALL SUBOPT_0xC
	SBIW R28,52
	ST   -Y,R17
;	*fs -> Y+53
;	fmt -> R17
;	buf -> Y+17
;	bsect -> Y+13
;	fsize -> Y+9
;	tsect -> Y+5
;	mclst -> Y+1
	LDI  R30,LOW(0)
	STS  _FatFs_G002,R30
	STS  _FatFs_G002+1,R30
; 0002 01D1 	if (!fs) return FR_OK;				/* Unregister fs object */
	RCALL SUBOPT_0x3E
	SBIW R30,0
	BRNE _0x40086
	RJMP _0x2000005
; 0002 01D2 
; 0002 01D3 	if (disk_initialize() & STA_NOINIT)	/* Check if the drive is ready or not */
_0x40086:
	RCALL _disk_initialize
	ANDI R30,LOW(0x1)
	BREQ _0x40087
; 0002 01D4 		return FR_NOT_READY;
	LDI  R30,LOW(2)
	RJMP _0x2000004
; 0002 01D5 
; 0002 01D6 	/* Search FAT partition on the drive */
; 0002 01D7 	bsect = 0;
_0x40087:
	LDI  R30,LOW(0)
	__CLRD1S 13
; 0002 01D8 	fmt = check_fs(buf, bsect);			/* Check sector 0 as an SFD format */
	RCALL SUBOPT_0x3F
	RCALL SUBOPT_0x40
; 0002 01D9 	if (fmt == 1) {						/* Not an FAT boot record, it may be FDISK format */
	CPI  R17,1
	BRNE _0x40088
; 0002 01DA 		/* Check a partition listed in top of the partition table */
; 0002 01DB 		if (disk_readp(buf, bsect, MBR_Table, 16)) {	/* 1st partition entry */
	RCALL SUBOPT_0x3F
	RCALL SUBOPT_0x41
	LDI  R30,LOW(446)
	LDI  R31,HIGH(446)
	RCALL SUBOPT_0x2
	LDI  R26,LOW(16)
	RCALL SUBOPT_0x2A
	BREQ _0x40089
; 0002 01DC 			fmt = 3;
	LDI  R17,LOW(3)
; 0002 01DD 		} else {
	RJMP _0x4008A
_0x40089:
; 0002 01DE 			if (buf[4]) {					/* Is the partition existing? */
	LDD  R30,Y+21
	CPI  R30,0
	BREQ _0x4008B
; 0002 01DF 				bsect = LD_DWORD(&buf[8]);	/* Partition offset in LBA */
	__GETD1S 25
	__PUTD1S 13
; 0002 01E0 				fmt = check_fs(buf, bsect);	/* Check the partition */
	RCALL SUBOPT_0x3F
	RCALL SUBOPT_0x40
; 0002 01E1 			}
; 0002 01E2 		}
_0x4008B:
_0x4008A:
; 0002 01E3 	}
; 0002 01E4 	if (fmt == 3) return FR_DISK_ERR;
_0x40088:
	CPI  R17,3
	BRNE _0x4008C
	LDI  R30,LOW(1)
	RJMP _0x2000004
; 0002 01E5 	if (fmt) return FR_NO_FILESYSTEM;	/* No valid FAT patition is found */
_0x4008C:
	CPI  R17,0
	BREQ _0x4008D
	LDI  R30,LOW(7)
	RJMP _0x2000004
; 0002 01E6 
; 0002 01E7 	/* Initialize the file system object */
; 0002 01E8 	if (disk_readp(buf, bsect, 13, sizeof(buf))) return FR_DISK_ERR;
_0x4008D:
	RCALL SUBOPT_0x3F
	RCALL SUBOPT_0x41
	LDI  R30,LOW(13)
	LDI  R31,HIGH(13)
	RCALL SUBOPT_0x2
	LDI  R26,LOW(36)
	RCALL SUBOPT_0x2A
	BREQ _0x4008E
	LDI  R30,LOW(1)
	RJMP _0x2000004
; 0002 01E9 
; 0002 01EA 	fsize = LD_WORD(buf+BPB_FATSz16-13);				/* Number of sectors per FAT */
_0x4008E:
	MOVW R30,R28
	ADIW R30,39
	RCALL SUBOPT_0x42
	RCALL __GETW1P
	RCALL SUBOPT_0x26
	RCALL SUBOPT_0x43
; 0002 01EB 	if (!fsize) fsize = LD_DWORD(buf+BPB_FATSz32-13);
	RCALL SUBOPT_0x44
	RCALL __CPD10
	BRNE _0x4008F
	MOVW R30,R28
	ADIW R30,53
	RCALL SUBOPT_0x42
	RCALL __GETD1P
	RCALL SUBOPT_0x43
; 0002 01EC 
; 0002 01ED 	fsize *= buf[BPB_NumFATs-13];						/* Number of sectors in FAT area */
_0x4008F:
	LDD  R30,Y+20
	LDI  R31,0
	RCALL SUBOPT_0xF
	RCALL SUBOPT_0x45
	RCALL __MULD12U
	RCALL SUBOPT_0x43
; 0002 01EE 	fs->fatbase = bsect + LD_WORD(buf+BPB_RsvdSecCnt-13); /* FAT start sector (lba) */
	MOVW R30,R28
	ADIW R30,31
	RCALL SUBOPT_0x42
	RCALL __GETW1P
	__GETD2S 13
	RCALL SUBOPT_0x26
	RCALL __ADDD12
	__PUTD1SNS 53,12
; 0002 01EF 	fs->csize = buf[BPB_SecPerClus-13];					/* Number of sectors per cluster */
	LDD  R30,Y+17
	__PUTB1SNS 53,1
; 0002 01F0 	fs->n_rootdir = LD_WORD(buf+BPB_RootEntCnt-13);		/* Nmuber of root directory entries */
	MOVW R30,R28
	ADIW R30,34
	RCALL SUBOPT_0x42
	RCALL __GETW1P
	__PUTW1SNS 53,4
; 0002 01F1 	tsect = LD_WORD(buf+BPB_TotSec16-13);				/* Number of sectors on the file system */
	MOVW R30,R28
	ADIW R30,36
	RCALL SUBOPT_0x42
	RCALL __GETW1P
	RCALL SUBOPT_0x26
	RCALL SUBOPT_0x46
; 0002 01F2 	if (!tsect) tsect = LD_DWORD(buf+BPB_TotSec32-13);
	__GETD1S 5
	RCALL __CPD10
	BRNE _0x40090
	MOVW R30,R28
	ADIW R30,49
	RCALL SUBOPT_0x42
	RCALL __GETD1P
	RCALL SUBOPT_0x46
; 0002 01F3 	mclst = (tsect						/* Last cluster# + 1 */
_0x40090:
; 0002 01F4 		- LD_WORD(buf+BPB_RsvdSecCnt-13) - fsize - fs->n_rootdir / 16
; 0002 01F5 		) / fs->csize + 2;
	MOVW R30,R28
	ADIW R30,31
	RCALL SUBOPT_0x42
	RCALL __GETW1P
	RCALL SUBOPT_0x1D
	RCALL SUBOPT_0x26
	RCALL __SWAPD12
	RCALL __SUBD12
	RCALL SUBOPT_0xF
	RCALL __SUBD12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x47
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RCALL SUBOPT_0x26
	RCALL __SUBD21
	RCALL SUBOPT_0x3E
	RCALL SUBOPT_0x30
	RCALL SUBOPT_0x45
	RCALL __DIVD21U
	__ADDD1N 2
	__PUTD1S 1
; 0002 01F6 	fs->max_clust = (CLUST)mclst;
	__PUTD1SNS 53,8
; 0002 01F7 
; 0002 01F8 	fmt = FS_FAT12;							/* Determine the FAT sub type */
	LDI  R17,LOW(1)
; 0002 01F9 	if (mclst >= 0xFF7) fmt = FS_FAT16;		/* Number of clusters >= 0xFF5 */
	RCALL SUBOPT_0x48
	__CPD2N 0xFF7
	BRLO _0x40091
	LDI  R17,LOW(2)
; 0002 01FA 	if (mclst >= 0xFFF7)					/* Number of clusters >= 0xFFF5 */
_0x40091:
	RCALL SUBOPT_0x48
	__CPD2N 0xFFF7
	BRLO _0x40092
; 0002 01FB #if _FS_FAT32
; 0002 01FC 		fmt = FS_FAT32;
	LDI  R17,LOW(3)
; 0002 01FD #else
; 0002 01FE 		return FR_NO_FILESYSTEM;
; 0002 01FF #endif
; 0002 0200 
; 0002 0201 	fs->fs_type = fmt;		/* FAT sub-type */
_0x40092:
	LDD  R26,Y+53
	LDD  R27,Y+53+1
	ST   X,R17
; 0002 0202 #if _FS_FAT32
; 0002 0203 	if (fmt == FS_FAT32)
	CPI  R17,3
	BRNE _0x40093
; 0002 0204 		fs->dirbase = LD_DWORD(buf+(BPB_RootClus-13));	/* Root directory start cluster */
	__GETD1S 48
	RJMP _0x400CE
; 0002 0205 	else
_0x40093:
; 0002 0206 #endif
; 0002 0207 		fs->dirbase = fs->fatbase + fsize;				/* Root directory start sector (lba) */
	RCALL SUBOPT_0x49
_0x400CE:
	__PUTD1SNS 53,16
; 0002 0208 	fs->database = fs->fatbase + fsize + fs->n_rootdir / 16;	/* Data start sector (lba) */
	RCALL SUBOPT_0x49
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x47
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RCALL SUBOPT_0x26
	RCALL __ADDD12
	__PUTD1SNS 53,20
; 0002 0209 
; 0002 020A 	fs->flag = 0;
	LDD  R26,Y+53
	LDD  R27,Y+53+1
	RCALL SUBOPT_0x4A
; 0002 020B 	FatFs = fs;
	RCALL SUBOPT_0x3E
	STS  _FatFs_G002,R30
	STS  _FatFs_G002+1,R31
; 0002 020C 
; 0002 020D 	return FR_OK;
_0x2000005:
	LDI  R30,LOW(0)
_0x2000004:
	LDD  R17,Y+0
	ADIW R28,55
	RET
; 0002 020E }
;
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Open or Create a File                                                 */
;/*-----------------------------------------------------------------------*/
;
;FRESULT pf_open (
; 0002 0218 	const char *path	/* Pointer to the file name */
; 0002 0219 )
; 0002 021A {
_pf_open:
; 0002 021B 	FRESULT res;
; 0002 021C 	DIR dj;
; 0002 021D 	BYTE sp[12], dir[32];
; 0002 021E 	FATFS *fs = FatFs;
; 0002 021F 
; 0002 0220 
; 0002 0221 	if (!fs)						/* Check file system */
	RCALL SUBOPT_0xC
	SBIW R28,60
	RCALL SUBOPT_0x31
;	*path -> Y+64
;	res -> R17
;	dj -> Y+48
;	sp -> Y+36
;	dir -> Y+4
;	*fs -> R18,R19
	RCALL SUBOPT_0x18
	BRNE _0x40095
; 0002 0222 		return FR_NOT_ENABLED;
	LDI  R30,LOW(6)
	RJMP _0x2000003
; 0002 0223 
; 0002 0224 	fs->flag = 0;
_0x40095:
	MOVW R26,R18
	RCALL SUBOPT_0x4A
; 0002 0225 	fs->buf = dir;
	MOVW R30,R28
	ADIW R30,4
	__PUTW1RNS 18,6
; 0002 0226 	dj.fn = sp;
	MOVW R30,R28
	ADIW R30,36
	STD  Y+50,R30
	STD  Y+50+1,R31
; 0002 0227 	res = follow_path(&dj, path);	/* Follow the file path */
	MOVW R30,R28
	ADIW R30,48
	RCALL SUBOPT_0x2
	__GETW2SX 66
	RCALL _follow_path_G002
	RCALL SUBOPT_0x34
; 0002 0228 	if (res != FR_OK) return res;	/* Follow failed */
	BREQ _0x40096
	MOV  R30,R17
	RJMP _0x2000003
; 0002 0229 	if (!dir[0] || (dir[DIR_Attr] & AM_DIR))	/* It is a directory */
_0x40096:
	LDD  R30,Y+4
	CPI  R30,0
	BREQ _0x40098
	LDD  R30,Y+15
	ANDI R30,LOW(0x10)
	BREQ _0x40097
_0x40098:
; 0002 022A 		return FR_NO_FILE;
	LDI  R30,LOW(3)
	RJMP _0x2000003
; 0002 022B 
; 0002 022C 	fs->org_clust =						/* File start cluster */
_0x40097:
; 0002 022D #if _FS_FAT32
; 0002 022E 		((DWORD)LD_WORD(dir+DIR_FstClusHI) << 16) |
; 0002 022F #endif
; 0002 0230 		LD_WORD(dir+DIR_FstClusLO);
	LDD  R30,Y+24
	LDD  R31,Y+24+1
	RCALL SUBOPT_0x26
	RCALL __LSLD16
	MOVW R26,R30
	MOVW R24,R22
	LDD  R30,Y+30
	LDD  R31,Y+30+1
	RCALL SUBOPT_0x26
	RCALL __ORD12
	__PUTD1RNS 18,32
; 0002 0231 	fs->fsize = LD_DWORD(dir+DIR_FileSize);	/* File size */
	__GETD1S 32
	__PUTD1RNS 18,28
; 0002 0232 	fs->fptr = 0;						/* File pointer */
	MOVW R26,R18
	ADIW R26,24
	RCALL SUBOPT_0x2F
	RCALL __PUTDP1
; 0002 0233 	fs->flag = FA_OPENED;
	MOVW R26,R18
	ADIW R26,2
	LDI  R30,LOW(1)
	ST   X,R30
; 0002 0234 
; 0002 0235 	return FR_OK;
	LDI  R30,LOW(0)
_0x2000003:
	RCALL __LOADLOCR4
	ADIW R28,63
	ADIW R28,3
	RET
; 0002 0236 }
;
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Read File                                                             */
;/*-----------------------------------------------------------------------*/
;#if _USE_READ
;
;FRESULT pf_read (
; 0002 0241 	void* buff,		/* Pointer to the read buffer (NULL:Forward data to the stream)*/
; 0002 0242 	WORD btr,		/* Number of bytes to read */
; 0002 0243 	WORD* br		/* Pointer to number of bytes read */
; 0002 0244 )
; 0002 0245 {
_pf_read:
; 0002 0246 	DRESULT dr;
; 0002 0247 	CLUST clst;
; 0002 0248 	DWORD sect, remain;
; 0002 0249 	BYTE *rbuff = buff;
; 0002 024A 	WORD rcnt;
; 0002 024B 	FATFS *fs = FatFs;
; 0002 024C 
; 0002 024D 
; 0002 024E 	*br = 0;
	RCALL SUBOPT_0xC
	SBIW R28,14
	RCALL __SAVELOCR6
;	*buff -> Y+24
;	btr -> Y+22
;	*br -> Y+20
;	dr -> R17
;	clst -> Y+16
;	sect -> Y+12
;	remain -> Y+8
;	*rbuff -> R18,R19
;	rcnt -> R20,R21
;	*fs -> Y+6
	__GETWRS 18,19,24
	RCALL SUBOPT_0x1E
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDD  R26,Y+20
	LDD  R27,Y+20+1
	RCALL SUBOPT_0x0
	ST   X+,R30
	ST   X,R31
; 0002 024F 	if (!fs) return FR_NOT_ENABLED;		/* Check file system */
	RCALL SUBOPT_0x13
	SBIW R30,0
	BRNE _0x4009A
	LDI  R30,LOW(6)
	RJMP _0x2000002
; 0002 0250 	if (!(fs->flag & FA_OPENED))		/* Check if opened */
_0x4009A:
	RCALL SUBOPT_0x13
	LDD  R26,Z+2
	ANDI R26,LOW(0x1)
	BRNE _0x4009B
; 0002 0251 		return FR_NOT_OPENED;
	LDI  R30,LOW(5)
	RJMP _0x2000002
; 0002 0252 
; 0002 0253 	remain = fs->fsize - fs->fptr;
_0x4009B:
	RCALL SUBOPT_0x13
	__GETD2Z 28
	PUSH R25
	PUSH R24
	PUSH R27
	PUSH R26
	RCALL SUBOPT_0x4B
	POP  R30
	POP  R31
	POP  R22
	POP  R23
	RCALL __SUBD12
	RCALL SUBOPT_0xE
; 0002 0254 	if (btr > remain) btr = (WORD)remain;			/* Truncate btr by remaining bytes */
	RCALL SUBOPT_0xD
	LDD  R26,Y+22
	LDD  R27,Y+22+1
	CLR  R24
	CLR  R25
	RCALL __CPD12
	BRSH _0x4009C
	RCALL SUBOPT_0x17
	STD  Y+22,R30
	STD  Y+22+1,R31
; 0002 0255 
; 0002 0256 	while (btr)	{									/* Repeat until all data transferred */
_0x4009C:
_0x4009D:
	LDD  R30,Y+22
	LDD  R31,Y+22+1
	SBIW R30,0
	BRNE PC+2
	RJMP _0x4009F
; 0002 0257 		if ((fs->fptr % 512) == 0) {				/* On the sector boundary? */
	RCALL SUBOPT_0x4B
	MOVW R30,R26
	MOVW R22,R24
	ANDI R31,HIGH(0x1FF)
	SBIW R30,0
	BREQ PC+2
	RJMP _0x400A0
; 0002 0258 			if ((fs->fptr / 512 % fs->csize) == 0) {	/* On the cluster boundary? */
	RCALL SUBOPT_0x4B
	RCALL SUBOPT_0x4C
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x13
	RCALL SUBOPT_0x30
	RCALL SUBOPT_0x45
	RCALL __MODD21U
	RCALL __CPD10
	BRNE _0x400A1
; 0002 0259 				clst = (fs->fptr == 0) ?			/* On the top of the file? */
; 0002 025A 					fs->org_clust : get_fat(fs->curr_clust);
	RCALL SUBOPT_0x4B
	RCALL __CPD02
	BRNE _0x400A2
	RCALL SUBOPT_0x11
	ADIW R26,32
	RCALL __GETD1P
	RJMP _0x400A3
_0x400A2:
	RCALL SUBOPT_0x4D
	RCALL _get_fat_G002
_0x400A3:
	__PUTD1S 16
; 0002 025B 				if (clst <= 1) goto fr_abort;
	__GETD2S 16
	RCALL SUBOPT_0x20
	BRSH _0x400A5
	RJMP _0x400A6
; 0002 025C 				fs->curr_clust = clst;				/* Update current cluster */
_0x400A5:
	__GETD1S 16
	__PUTD1SNS 6,36
; 0002 025D 				fs->csect = 0;						/* Reset sector offset in the cluster */
	RCALL SUBOPT_0x11
	ADIW R26,3
	RCALL SUBOPT_0x3A
; 0002 025E 			}
; 0002 025F 			sect = clust2sect(fs->curr_clust);		/* Get current sector */
_0x400A1:
	RCALL SUBOPT_0x4D
	RCALL _clust2sect_G002
	__PUTD1S 12
; 0002 0260 			if (!sect) goto fr_abort;
	RCALL __CPD10
	BRNE _0x400A7
	RJMP _0x400A6
; 0002 0261 			fs->dsect = sect + fs->csect++;
_0x400A7:
	RCALL SUBOPT_0x11
	RCALL SUBOPT_0x4E
	RCALL SUBOPT_0x1F
	RCALL SUBOPT_0x45
	RCALL __ADDD12
	__PUTD1SNS 6,40
; 0002 0262 		}
; 0002 0263 		rcnt = 512 - ((WORD)fs->fptr % 512);		/* Get partial sector data from sector buffer */
_0x400A0:
	RCALL SUBOPT_0x13
	LDD  R26,Z+24
	LDD  R27,Z+25
	MOVW R30,R26
	ANDI R31,HIGH(0x1FF)
	LDI  R26,LOW(512)
	LDI  R27,HIGH(512)
	SUB  R26,R30
	SBC  R27,R31
	MOVW R20,R26
; 0002 0264 		if (rcnt > btr) rcnt = btr;
	LDD  R30,Y+22
	LDD  R31,Y+22+1
	CP   R30,R20
	CPC  R31,R21
	BRSH _0x400A8
	__GETWRS 20,21,22
; 0002 0265 		dr = disk_readp(!buff ? 0 : rbuff, fs->dsect, (WORD)(fs->fptr % 512), rcnt);
_0x400A8:
	LDD  R30,Y+24
	LDD  R31,Y+24+1
	SBIW R30,0
	BRNE _0x400A9
	RCALL SUBOPT_0x0
	RJMP _0x400AA
_0x400A9:
	MOVW R30,R18
_0x400AA:
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x17
	__GETD2Z 40
	RCALL __PUTPARD2
	LDD  R26,Y+12
	LDD  R27,Y+12+1
	ADIW R26,24
	RCALL __GETW1P
	ANDI R31,HIGH(0x1FF)
	RCALL SUBOPT_0x2
	MOVW R26,R20
	RCALL _disk_readp
	RCALL SUBOPT_0x34
; 0002 0266 		if (dr) goto fr_abort;
	BRNE _0x400A6
; 0002 0267 		fs->fptr += rcnt; rbuff += rcnt;			/* Update pointers and counters */
	RCALL SUBOPT_0x13
	RCALL SUBOPT_0x4F
	MOVW R26,R30
	MOVW R24,R22
	MOVW R30,R20
	RCALL SUBOPT_0x26
	RCALL SUBOPT_0x50
	__ADDWRR 18,19,20,21
; 0002 0268 		btr -= rcnt; *br += rcnt;
	LDD  R30,Y+22
	LDD  R31,Y+22+1
	SUB  R30,R20
	SBC  R31,R21
	STD  Y+22,R30
	STD  Y+22+1,R31
	LDD  R26,Y+20
	LDD  R27,Y+20+1
	LD   R30,X+
	LD   R31,X+
	ADD  R30,R20
	ADC  R31,R21
	ST   -X,R31
	ST   -X,R30
; 0002 0269 	}
	RJMP _0x4009D
_0x4009F:
; 0002 026A 
; 0002 026B 	return FR_OK;
	LDI  R30,LOW(0)
	RJMP _0x2000002
; 0002 026C 
; 0002 026D fr_abort:
_0x400A6:
; 0002 026E 	fs->flag = 0;
	RCALL SUBOPT_0x11
	RCALL SUBOPT_0x4A
; 0002 026F 	return FR_DISK_ERR;
	LDI  R30,LOW(1)
_0x2000002:
	RCALL __LOADLOCR6
	ADIW R28,26
	RET
; 0002 0270 }
;#endif
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Write File                                                            */
;/*-----------------------------------------------------------------------*/
;#if _USE_WRITE
;
;FRESULT pf_write (
;	const void* buff,	/* Pointer to the data to be written */
;	WORD btw,			/* Number of bytes to write (0:Finalize the current write operation) */
;	WORD* bw			/* Pointer to number of bytes written */
;)
;{
;	CLUST clst;
;	DWORD sect, remain;
;	const BYTE *p = buff;
;	WORD wcnt;
;	FATFS *fs = FatFs;
;
;
;	*bw = 0;
;	if (!fs) return FR_NOT_ENABLED;		/* Check file system */
;	if (!(fs->flag & FA_OPENED))		/* Check if opened */
;		return FR_NOT_OPENED;
;
;	if (!btw) {		/* Finalize request */
;		if ((fs->flag & FA__WIP) && disk_writep(0, 0)) goto fw_abort;
;		fs->flag &= ~FA__WIP;
;		return FR_OK;
;	} else {		/* Write data request */
;		if (!(fs->flag & FA__WIP))		/* Round down fptr to the sector boundary */
;			fs->fptr &= 0xFFFFFE00;
;	}
;	remain = fs->fsize - fs->fptr;
;	if (btw > remain) btw = (WORD)remain;			/* Truncate btw by remaining bytes */
;
;	while (btw)	{									/* Repeat until all data transferred */
;		if (((WORD)fs->fptr % 512) == 0) {				/* On the sector boundary? */
;			if ((fs->fptr / 512 % fs->csize) == 0) {	/* On the cluster boundary? */
;				clst = (fs->fptr == 0) ?			/* On the top of the file? */
;					fs->org_clust : get_fat(fs->curr_clust);
;				if (clst <= 1) goto fw_abort;
;				fs->curr_clust = clst;				/* Update current cluster */
;				fs->csect = 0;						/* Reset sector offset in the cluster */
;			}
;			sect = clust2sect(fs->curr_clust);		/* Get current sector */
;			if (!sect) goto fw_abort;
;			fs->dsect = sect + fs->csect++;
;			if (disk_writep(0, fs->dsect)) goto fw_abort;	/* Initiate a sector write operation */
;			fs->flag |= FA__WIP;
;		}
;		wcnt = 512 - ((WORD)fs->fptr % 512);		/* Number of bytes to write to the sector */
;		if (wcnt > btw) wcnt = btw;
;		if (disk_writep(p, wcnt)) goto fw_abort;	/* Send data to the sector */
;		fs->fptr += wcnt; p += wcnt;				/* Update pointers and counters */
;		btw -= wcnt; *bw += wcnt;
;		if (((WORD)fs->fptr % 512) == 0) {
;			if (disk_writep(0, 0)) goto fw_abort;	/* Finalize the currtent secter write operation */
;			fs->flag &= ~FA__WIP;
;		}
;	}
;
;	return FR_OK;
;
;fw_abort:
;	fs->flag = 0;
;	return FR_DISK_ERR;
;}
;#endif
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Seek File R/W Pointer                                                 */
;/*-----------------------------------------------------------------------*/
;#if _USE_LSEEK
;
;FRESULT pf_lseek (
; 0002 02C1 	DWORD ofs		/* File pointer from top of file */
; 0002 02C2 )
; 0002 02C3 {
_pf_lseek:
; 0002 02C4 	CLUST clst;
; 0002 02C5 	DWORD bcs, sect, ifptr;
; 0002 02C6 	FATFS *fs = FatFs;
; 0002 02C7 
; 0002 02C8 
; 0002 02C9 	if (!fs) return FR_NOT_ENABLED;		/* Check file system */
	RCALL __PUTPARD2
	SBIW R28,16
	RCALL SUBOPT_0x2B
;	ofs -> Y+18
;	clst -> Y+14
;	bcs -> Y+10
;	sect -> Y+6
;	ifptr -> Y+2
;	*fs -> R16,R17
	MOV  R0,R16
	OR   R0,R17
	BRNE _0x400AD
	LDI  R30,LOW(6)
	RJMP _0x2000001
; 0002 02CA 	if (!(fs->flag & FA_OPENED))		/* Check if opened */
_0x400AD:
	MOVW R30,R16
	LDD  R26,Z+2
	ANDI R26,LOW(0x1)
	BRNE _0x400AE
; 0002 02CB 			return FR_NOT_OPENED;
	LDI  R30,LOW(5)
	RJMP _0x2000001
; 0002 02CC 
; 0002 02CD 	if (ofs > fs->fsize) ofs = fs->fsize;	/* Clip offset with the file size */
_0x400AE:
	MOVW R26,R16
	ADIW R26,28
	RCALL SUBOPT_0x51
	RCALL __CPD12
	BRSH _0x400AF
	MOVW R26,R16
	ADIW R26,28
	RCALL __GETD1P
	RCALL SUBOPT_0x52
; 0002 02CE 	ifptr = fs->fptr;
_0x400AF:
	MOVW R26,R16
	ADIW R26,24
	RCALL __GETD1P
	RCALL SUBOPT_0x2C
; 0002 02CF 	fs->fptr = 0;
	MOVW R26,R16
	ADIW R26,24
	RCALL SUBOPT_0x2F
	RCALL __PUTDP1
; 0002 02D0 	if (ofs > 0) {
	RCALL SUBOPT_0x53
	RCALL __CPD02
	BRLO PC+2
	RJMP _0x400B0
; 0002 02D1 		bcs = (DWORD)fs->csize * 512;	/* Cluster size (byte) */
	MOVW R30,R16
	RCALL SUBOPT_0x30
	RCALL SUBOPT_0x45
	RCALL SUBOPT_0xB
	RCALL __MULD12U
	__PUTD1S 10
; 0002 02D2 		if (ifptr > 0 &&
; 0002 02D3 			(ofs - 1) / bcs >= (ifptr - 1) / bcs) {	/* When seek to same or following cluster, */
	RCALL SUBOPT_0x2E
	RCALL __CPD02
	BRSH _0x400B2
	RCALL SUBOPT_0x54
	RCALL SUBOPT_0x55
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x6
	RCALL SUBOPT_0x55
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RCALL __CPD21
	BRSH _0x400B3
_0x400B2:
	RJMP _0x400B1
_0x400B3:
; 0002 02D4 			fs->fptr = (ifptr - 1) & ~(bcs - 1);	/* start from the current cluster */
	RCALL SUBOPT_0x6
	RCALL SUBOPT_0x56
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x57
	RCALL SUBOPT_0x56
	RCALL __COMD1
	RCALL __ANDD12
	__PUTD1RNS 16,24
; 0002 02D5 			ofs -= fs->fptr;
	MOVW R26,R16
	ADIW R26,24
	RCALL SUBOPT_0x51
	RCALL __SUBD21
	__PUTD2S 18
; 0002 02D6 			clst = fs->curr_clust;
	MOVW R26,R16
	ADIW R26,36
	RCALL SUBOPT_0x58
; 0002 02D7 		} else {							/* When seek to back cluster, */
	RJMP _0x400B4
_0x400B1:
; 0002 02D8 			clst = fs->org_clust;			/* start from the first cluster */
	MOVW R26,R16
	ADIW R26,32
	RCALL SUBOPT_0x58
; 0002 02D9 			fs->curr_clust = clst;
	RCALL SUBOPT_0x59
; 0002 02DA 		}
_0x400B4:
; 0002 02DB 		while (ofs > bcs) {				/* Cluster following loop */
_0x400B5:
	RCALL SUBOPT_0x57
	RCALL SUBOPT_0x53
	RCALL __CPD12
	BRSH _0x400B7
; 0002 02DC 			clst = get_fat(clst);		/* Follow cluster chain */
	RCALL SUBOPT_0x27
	RCALL _get_fat_G002
	__PUTD1S 14
; 0002 02DD 			if (clst <= 1 || clst >= fs->max_clust) goto fe_abort;
	RCALL SUBOPT_0x27
	RCALL SUBOPT_0x20
	BRLO _0x400B9
	MOVW R26,R16
	RCALL SUBOPT_0x21
	RCALL SUBOPT_0x27
	RCALL __CPD21
	BRLO _0x400B8
_0x400B9:
	RJMP _0x400BB
; 0002 02DE 			fs->curr_clust = clst;
_0x400B8:
	RCALL SUBOPT_0x59
; 0002 02DF 			fs->fptr += bcs;
	MOVW R30,R16
	RCALL SUBOPT_0x4F
	RCALL SUBOPT_0x5A
	RCALL SUBOPT_0x50
; 0002 02E0 			ofs -= bcs;
	RCALL SUBOPT_0x5A
	RCALL SUBOPT_0x54
	RCALL __SUBD12
	RCALL SUBOPT_0x52
; 0002 02E1 		}
	RJMP _0x400B5
_0x400B7:
; 0002 02E2 		fs->fptr += ofs;
	MOVW R30,R16
	RCALL SUBOPT_0x4F
	RCALL SUBOPT_0x53
	RCALL SUBOPT_0x50
; 0002 02E3 		sect = clust2sect(clst);		/* Current sector */
	RCALL SUBOPT_0x27
	RCALL _clust2sect_G002
	__PUTD1S 6
; 0002 02E4 		if (!sect) goto fe_abort;
	RCALL __CPD10
	BREQ _0x400BB
; 0002 02E5 		fs->csect = (BYTE)(ofs / 512);	/* Sector offset in the cluster */
	RCALL SUBOPT_0x53
	RCALL SUBOPT_0x4C
	__PUTB1RNS 16,3
; 0002 02E6 		if (ofs % 512)
	RCALL SUBOPT_0x54
	ANDI R31,HIGH(0x1FF)
	SBIW R30,0
	BREQ _0x400BD
; 0002 02E7 			fs->dsect = sect + fs->csect++;
	MOVW R26,R16
	RCALL SUBOPT_0x4E
	__GETD2S 6
	RCALL SUBOPT_0x45
	RCALL __ADDD12
	__PUTD1RNS 16,40
; 0002 02E8 	}
_0x400BD:
; 0002 02E9 
; 0002 02EA 	return FR_OK;
_0x400B0:
	LDI  R30,LOW(0)
	RJMP _0x2000001
; 0002 02EB 
; 0002 02EC fe_abort:
_0x400BB:
; 0002 02ED 	fs->flag = 0;
	MOVW R26,R16
	RCALL SUBOPT_0x4A
; 0002 02EE 	return FR_DISK_ERR;
	LDI  R30,LOW(1)
_0x2000001:
	RCALL __LOADLOCR2
	ADIW R28,22
	RET
; 0002 02EF }
;#endif
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Create a Directroy Object                                             */
;/*-----------------------------------------------------------------------*/
;#if _USE_DIR
;
;FRESULT pf_opendir (
; 0002 02FA 	DIR *dj,			/* Pointer to directory object to create */
; 0002 02FB 	const char *path	/* Pointer to the directory path */
; 0002 02FC )
; 0002 02FD {
; 0002 02FE 	FRESULT res;
; 0002 02FF 	BYTE sp[12], dir[32];
; 0002 0300 	FATFS *fs = FatFs;
; 0002 0301 
; 0002 0302 
; 0002 0303 	if (!fs) {				/* Check file system */
;	*dj -> Y+50
;	*path -> Y+48
;	res -> R17
;	sp -> Y+36
;	dir -> Y+4
;	*fs -> R18,R19
; 0002 0304 		res = FR_NOT_ENABLED;
; 0002 0305 	} else {
; 0002 0306 		fs->buf = dir;
; 0002 0307 		dj->fn = sp;
; 0002 0308 		res = follow_path(dj, path);			/* Follow the path to the directory */
; 0002 0309 		if (res == FR_OK) {						/* Follow completed */
; 0002 030A 			if (dir[0]) {						/* It is not the root dir */
; 0002 030B 				if (dir[DIR_Attr] & AM_DIR) {	/* The object is a directory */
; 0002 030C 					dj->sclust =
; 0002 030D #if _FS_FAT32
; 0002 030E 					((DWORD)LD_WORD(dir+DIR_FstClusHI) << 16) |
; 0002 030F #endif
; 0002 0310 					LD_WORD(dir+DIR_FstClusLO);
; 0002 0311 				} else {						/* The object is not a directory */
; 0002 0312 					res = FR_NO_PATH;
; 0002 0313 				}
; 0002 0314 			}
; 0002 0315 			if (res == FR_OK)
; 0002 0316 				res = dir_rewind(dj);			/* Rewind dir */
; 0002 0317 		}
; 0002 0318 		if (res == FR_NO_FILE) res = FR_NO_PATH;
; 0002 0319 	}
; 0002 031A 
; 0002 031B 	return res;
; 0002 031C }
;
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Read Directory Entry in Sequense                                      */
;/*-----------------------------------------------------------------------*/
;
;FRESULT pf_readdir (
; 0002 0326 	DIR *dj,			/* Pointer to the open directory object */
; 0002 0327 	FILINFO *fno		/* Pointer to file information to return */
; 0002 0328 )
; 0002 0329 {
; 0002 032A 	FRESULT res;
; 0002 032B 	BYTE sp[12], dir[32];
; 0002 032C 	FATFS *fs = FatFs;
; 0002 032D 
; 0002 032E 
; 0002 032F 	if (!fs) {				/* Check file system */
;	*dj -> Y+50
;	*fno -> Y+48
;	res -> R17
;	sp -> Y+36
;	dir -> Y+4
;	*fs -> R18,R19
; 0002 0330 		res = FR_NOT_ENABLED;
; 0002 0331 	} else {
; 0002 0332 		fs->buf = dir;
; 0002 0333 		dj->fn = sp;
; 0002 0334 		if (!fno) {
; 0002 0335 			res = dir_rewind(dj);
; 0002 0336 		} else {
; 0002 0337 			res = dir_read(dj);
; 0002 0338 			if (res == FR_NO_FILE) {
; 0002 0339 				dj->sect = 0;
; 0002 033A 				res = FR_OK;
; 0002 033B 			}
; 0002 033C 			if (res == FR_OK) {				/* A valid entry is found */
; 0002 033D 				get_fileinfo(dj, fno);		/* Get the object information */
; 0002 033E 				res = dir_next(dj);			/* Increment index for next */
; 0002 033F 				if (res == FR_NO_FILE) {
; 0002 0340 					dj->sect = 0;
; 0002 0341 					res = FR_OK;
; 0002 0342 				}
; 0002 0343 			}
; 0002 0344 		}
; 0002 0345 	}
; 0002 0346 
; 0002 0347 	return res;
; 0002 0348 }
;
;#endif /* _USE_DIR */
;

	.DSEG
_buffer:
	.BYTE 0x200
_count:
	.BYTE 0x2
_CardType_G001:
	.BYTE 0x1
_wc_S0010007000:
	.BYTE 0x2
_FatFs_G002:
	.BYTE 0x2
_cvt_G002:
	.BYTE 0x80

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x0:
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x1:
	LDI  R30,LOW(_buffer)
	LDI  R31,HIGH(_buffer)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 33 TIMES, CODE SIZE REDUCTION:30 WORDS
SUBOPT_0x2:
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x3:
	LDS  R26,_count
	LDS  R27,_count+1
	CPI  R26,LOW(0x100)
	LDI  R30,HIGH(0x100)
	CPC  R27,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x4:
	ST   -Y,R30
	__GETD2N 0x0
	RJMP _send_cmd_G001

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x5:
	RCALL _xmit_spi
	__GETD2S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x6:
	__GETD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x7:
	__GETD2N 0x0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x8:
	MOV  R30,R17
	LDI  R31,0
	MOVW R26,R28
	ADIW R26,6
	ADD  R30,R26
	ADC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x9:
	MOV  R0,R20
	OR   R0,R21
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xA:
	RCALL _send_cmd_G001
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xB:
	__GETD2N 0x200
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xC:
	ST   -Y,R27
	ST   -Y,R26
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0xD:
	__GETD1S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xE:
	__PUTD1S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0xF:
	__GETD2S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x10:
	__SUBWRN 18,19,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 17 TIMES, CODE SIZE REDUCTION:14 WORDS
SUBOPT_0x11:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x12:
	LDD  R30,Y+4
	LDD  R31,Y+4+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 18 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x13:
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x14:
	RCALL SUBOPT_0x13
	SBIW R30,1
	STD  Y+6,R30
	STD  Y+6+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x15:
	RCALL SUBOPT_0x12
	SBIW R30,1
	STD  Y+4,R30
	STD  Y+4+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x16:
	MOV  R30,R17
	RCALL __LOADLOCR4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x17:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x18:
	MOV  R0,R18
	OR   R0,R19
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x19:
	LDS  R30,_wc_S0010007000
	LDS  R31,_wc_S0010007000+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x1A:
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x1B:
	__GETD1S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x1C:
	__PUTD1S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x1D:
	__GETD2S 5
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x1E:
	LDS  R30,_FatFs_G002
	LDS  R31,_FatFs_G002+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x1F:
	__GETD2S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:16 WORDS
SUBOPT_0x20:
	__CPD2N 0x2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x21:
	ADIW R26,8
	RCALL __GETD1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x22:
	MOVW R30,R28
	ADIW R30,8
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x17
	__GETD2Z 12
	MOVW R30,R18
	CLR  R22
	CLR  R23
	RCALL __ADDD12
	RCALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x23:
	LDI  R26,LOW(2)
	LDI  R27,0
	RCALL _disk_readp
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x24:
	RCALL SUBOPT_0x2
	LDI  R26,LOW(1)
	LDI  R27,0
	RCALL _disk_readp
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x25:
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x17
	__GETD2Z 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 13 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x26:
	CLR  R22
	CLR  R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x27:
	__GETD2S 14
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x28:
	RCALL __ADDD12
	RCALL __PUTPARD1
	LDD  R30,Y+18
	LDD  R31,Y+18+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x29:
	RCALL SUBOPT_0x2
	RJMP SUBOPT_0x23

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x2A:
	LDI  R27,0
	RCALL _disk_readp
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x2B:
	RCALL __SAVELOCR2
	__GETWRMN 16,17,0,_FatFs_G002
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x2C:
	__PUTD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x2D:
	__GETD2Z 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x2E:
	__GETD2S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x2F:
	__GETD1N 0x0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x30:
	LDD  R30,Z+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x31:
	RCALL __SAVELOCR4
	__GETWRMN 18,19,0,_FatFs_G002
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x32:
	__GETD2S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x33:
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x34:
	MOV  R17,R30
	CPI  R17,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x35:
	LDS  R26,_FatFs_G002
	LDS  R27,_FatFs_G002+1
	ADIW R26,6
	LD   R18,X+
	LD   R19,X
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x36:
	__GETD2Z 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x37:
	MOV  R30,R18
	SUBI R18,-1
	RCALL SUBOPT_0x11
	LDI  R31,0
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x38:
	MOV  R30,R21
	SUBI R21,-1
	RCALL SUBOPT_0x1A
	LDI  R31,0
	ADD  R30,R26
	ADC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x39:
	RCALL SUBOPT_0x12
	ADIW R30,1
	STD  Y+4,R30
	STD  Y+4+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3A:
	LDI  R30,LOW(0)
	ST   X,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x3B:
	RCALL SUBOPT_0x11
	ADIW R26,2
	RCALL __GETW1P
	LDD  R30,Z+11
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x3C:
	RCALL SUBOPT_0x12
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x6
	RCALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x3D:
	RCALL SUBOPT_0x33
	RCALL __GETW1P
	CPI  R30,LOW(0x4146)
	LDI  R26,HIGH(0x4146)
	CPC  R31,R26
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x3E:
	LDD  R30,Y+53
	LDD  R31,Y+53+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x3F:
	MOVW R30,R28
	ADIW R30,17
	RJMP SUBOPT_0x2

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x40:
	__GETD2S 15
	RCALL _check_fs_G002
	MOV  R17,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x41:
	__GETD1S 15
	RCALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x42:
	SBIW R30,13
	MOVW R26,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x43:
	__PUTD1S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x44:
	__GETD1S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x45:
	RCALL __CWD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x46:
	__PUTD1S 5
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x47:
	RCALL SUBOPT_0x3E
	LDD  R26,Z+4
	LDD  R27,Z+5
	MOVW R30,R26
	RCALL __LSRW4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x48:
	__GETD2S 1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x49:
	RCALL SUBOPT_0x3E
	RCALL SUBOPT_0x36
	RCALL SUBOPT_0x44
	RCALL __ADDD12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x4A:
	ADIW R26,2
	RJMP SUBOPT_0x3A

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x4B:
	RCALL SUBOPT_0x13
	__GETD2Z 24
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x4C:
	__GETD1N 0x200
	RCALL __DIVD21U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x4D:
	RCALL SUBOPT_0x13
	__GETD2Z 36
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x4E:
	ADIW R26,3
	LD   R30,X
	SUBI R30,-LOW(1)
	ST   X,R30
	SUBI R30,LOW(1)
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x4F:
	ADIW R30,24
	MOVW R0,R30
	MOVW R26,R30
	RCALL __GETD1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x50:
	RCALL __ADDD12
	MOVW R26,R0
	RCALL __PUTDP1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x51:
	RCALL __GETD1P
	__GETD2S 18
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x52:
	__PUTD1S 18
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x53:
	__GETD2S 18
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x54:
	__GETD1S 18
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x55:
	__SUBD1N 1
	MOVW R26,R30
	MOVW R24,R22
	__GETD1S 10
	RCALL __DIVD21U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x56:
	__SUBD1N 1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x57:
	__GETD1S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x58:
	RCALL __GETD1P
	__PUTD1S 14
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x59:
	__GETD1S 14
	__PUTD1RNS 16,36
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5A:
	__GETD2S 10
	RET


	.CSEG
__ADDD12:
	ADD  R30,R26
	ADC  R31,R27
	ADC  R22,R24
	ADC  R23,R25
	RET

__SUBD12:
	SUB  R30,R26
	SBC  R31,R27
	SBC  R22,R24
	SBC  R23,R25
	RET

__SUBD21:
	SUB  R26,R30
	SBC  R27,R31
	SBC  R24,R22
	SBC  R25,R23
	RET

__ANDD12:
	AND  R30,R26
	AND  R31,R27
	AND  R22,R24
	AND  R23,R25
	RET

__ORD12:
	OR   R30,R26
	OR   R31,R27
	OR   R22,R24
	OR   R23,R25
	RET

__LSRD12:
	TST  R30
	MOV  R0,R30
	MOVW R30,R26
	MOVW R22,R24
	BREQ __LSRD12R
__LSRD12L:
	LSR  R23
	ROR  R22
	ROR  R31
	ROR  R30
	DEC  R0
	BRNE __LSRD12L
__LSRD12R:
	RET

__LSLW4:
	LSL  R30
	ROL  R31
__LSLW3:
	LSL  R30
	ROL  R31
__LSLW2:
	LSL  R30
	ROL  R31
	LSL  R30
	ROL  R31
	RET

__LSRW4:
	LSR  R31
	ROR  R30
__LSRW3:
	LSR  R31
	ROR  R30
__LSRW2:
	LSR  R31
	ROR  R30
	LSR  R31
	ROR  R30
	RET

__LSRD16:
	MOV  R30,R22
	MOV  R31,R23
	LDI  R22,0
	LDI  R23,0
	RET

__LSLD16:
	MOV  R22,R30
	MOV  R23,R31
	LDI  R30,0
	LDI  R31,0
	RET

__CWD1:
	MOV  R22,R31
	ADD  R22,R22
	SBC  R22,R22
	MOV  R23,R22
	RET

__COMD1:
	COM  R30
	COM  R31
	COM  R22
	COM  R23
	RET

__MULD12U:
	MUL  R23,R26
	MOV  R23,R0
	MUL  R22,R27
	ADD  R23,R0
	MUL  R31,R24
	ADD  R23,R0
	MUL  R30,R25
	ADD  R23,R0
	MUL  R22,R26
	MOV  R22,R0
	ADD  R23,R1
	MUL  R31,R27
	ADD  R22,R0
	ADC  R23,R1
	MUL  R30,R24
	ADD  R22,R0
	ADC  R23,R1
	CLR  R24
	MUL  R31,R26
	MOV  R31,R0
	ADD  R22,R1
	ADC  R23,R24
	MUL  R30,R27
	ADD  R31,R0
	ADC  R22,R1
	ADC  R23,R24
	MUL  R30,R26
	MOV  R30,R0
	ADD  R31,R1
	ADC  R22,R24
	ADC  R23,R24
	RET

__DIVW21U:
	CLR  R0
	CLR  R1
	LDI  R25,16
__DIVW21U1:
	LSL  R26
	ROL  R27
	ROL  R0
	ROL  R1
	SUB  R0,R30
	SBC  R1,R31
	BRCC __DIVW21U2
	ADD  R0,R30
	ADC  R1,R31
	RJMP __DIVW21U3
__DIVW21U2:
	SBR  R26,1
__DIVW21U3:
	DEC  R25
	BRNE __DIVW21U1
	MOVW R30,R26
	MOVW R26,R0
	RET

__DIVD21U:
	PUSH R19
	PUSH R20
	PUSH R21
	CLR  R0
	CLR  R1
	CLR  R20
	CLR  R21
	LDI  R19,32
__DIVD21U1:
	LSL  R26
	ROL  R27
	ROL  R24
	ROL  R25
	ROL  R0
	ROL  R1
	ROL  R20
	ROL  R21
	SUB  R0,R30
	SBC  R1,R31
	SBC  R20,R22
	SBC  R21,R23
	BRCC __DIVD21U2
	ADD  R0,R30
	ADC  R1,R31
	ADC  R20,R22
	ADC  R21,R23
	RJMP __DIVD21U3
__DIVD21U2:
	SBR  R26,1
__DIVD21U3:
	DEC  R19
	BRNE __DIVD21U1
	MOVW R30,R26
	MOVW R22,R24
	MOVW R26,R0
	MOVW R24,R20
	POP  R21
	POP  R20
	POP  R19
	RET

__MODD21U:
	RCALL __DIVD21U
	MOVW R30,R26
	MOVW R22,R24
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	SBIW R26,1
	RET

__GETD1P:
	LD   R30,X+
	LD   R31,X+
	LD   R22,X+
	LD   R23,X
	SBIW R26,3
	RET

__GETD1P_INC:
	LD   R30,X+
	LD   R31,X+
	LD   R22,X+
	LD   R23,X+
	RET

__PUTDP1:
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	RET

__PUTDP1_DEC:
	ST   -X,R23
	ST   -X,R22
	ST   -X,R31
	ST   -X,R30
	RET

__PUTPARD1:
	ST   -Y,R23
	ST   -Y,R22
	ST   -Y,R31
	ST   -Y,R30
	RET

__PUTPARD2:
	ST   -Y,R25
	ST   -Y,R24
	ST   -Y,R27
	ST   -Y,R26
	RET

__SWAPD12:
	MOV  R1,R24
	MOV  R24,R22
	MOV  R22,R1
	MOV  R1,R25
	MOV  R25,R23
	MOV  R23,R1

__SWAPW12:
	MOV  R1,R27
	MOV  R27,R31
	MOV  R31,R1

__SWAPB12:
	MOV  R1,R26
	MOV  R26,R30
	MOV  R30,R1
	RET

__CPD10:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	RET

__CPD02:
	CLR  R0
	CP   R0,R26
	CPC  R0,R27
	CPC  R0,R24
	CPC  R0,R25
	RET

__CPD12:
	CP   R30,R26
	CPC  R31,R27
	CPC  R22,R24
	CPC  R23,R25
	RET

__CPD21:
	CP   R26,R30
	CPC  R27,R31
	CPC  R24,R22
	CPC  R25,R23
	RET

__SAVELOCR6:
	ST   -Y,R21
__SAVELOCR5:
	ST   -Y,R20
__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR6:
	LDD  R21,Y+5
__LOADLOCR5:
	LDD  R20,Y+4
__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

;END OF CODE MARKER
__END_OF_CODE:
