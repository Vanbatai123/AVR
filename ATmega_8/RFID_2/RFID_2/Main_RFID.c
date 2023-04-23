/*
* RFID_2.c
*
* Created: 2/10/2017 7:14:53 PM
*  Author: Van_BasTai
*/


#include "mySTART.h"
#include "myUART.h"

#define RFID_PORT	PORTB
#define RFID_DDR	DDRB
#define RFID_PIN	PINB

#define RFID_SS		2
#define RFID_MOSI	3
#define RFID_MISO	4
#define RFID_SCK	5
/************************************************************************/
/*	FileName: myRFID.h													*/
/*	Created: 10/4/2017 7:20:57 PM										*/
/*  Author: Van_BasTai													*/
/* 																		*/
/*	Description: File chua danh sach thanh ghi va lenh cua RFID			*/
/*																		*/
/************************************************************************/


/************************************************************************/
/*	Register                                                            */
/************************************************************************/

//Page 0 ==> Command and Status
#define Page0_Reserved_1 	0x00
#define CommandReg			0x01
#define ComIEnReg			0x02
#define DivIEnReg			0x03
#define ComIrqReg			0x04
#define DivIrqReg			0x05
#define ErrorReg			0x06
#define Status1Reg			0x07
#define Status2Reg			0x08
#define FIFODataReg			0x09
#define FIFOLevelReg		0x0A
#define WaterLevelReg		0x0B
#define ControlReg			0x0C
#define BitFramingReg		0x0D
#define CollReg				0x0E
#define Page0_Reserved_2	0x0F

//Page 1 ==> Command
#define Page1_Reserved_1	0x10
#define ModeReg				0x11
#define TxModeReg			0x12
#define RxModeReg			0x13
#define TxControlReg		0x14
#define TxASKReg			0x15
#define TxSelReg			0x16
#define RxSelReg			0x17
#define RxThresholdReg		0x18
#define	DemodReg			0x19
#define Page1_Reserved_2	0x1A
#define Page1_Reserved_3	0x1B
#define MfTxReg				0x1C
#define MfRxReg				0x1D
#define Page1_Reserved_4	0x1E
#define SerialSpeedReg		0x1F

//Page 2 ==> CFG
#define Page2_Reserved_1	0x20
#define CRCResultReg_1		0x21
#define CRCResultReg_2		0x22
#define Page2_Reserved_2	0x23
#define ModWidthReg			0x24
#define Page2_Reserved_3	0x25
#define RFCfgReg			0x26
#define GsNReg				0x27
#define CWGsPReg			0x28
#define ModGsPReg			0x29
#define TModeReg			0x2A
#define TPrescalerReg		0x2B
#define TReloadReg_1		0x2C
#define TReloadReg_2		0x2D
#define TCounterValReg_1	0x2E
#define TCounterValReg_2 	0x2F

//Page 3 ==> TestRegister
#define Page3_Reserved_1 	0x30
#define TestSel1Reg			0x31
#define TestSel2Reg			0x32
#define TestPinEnReg		0x33
#define TestPinValueReg		0x34
#define TestBusReg			0x35
#define AutoTestReg			0x36
#define VersionReg			0x37
#define AnalogTestReg		0x38
#define TestDAC1Reg			0x39
#define TestDAC2Reg			0x3A
#define TestADCReg			0x3B
#define Page3_Reserved_2 	0x3C
#define Page3_Reserved_3	0x3D
#define Page3_Reserved_4	0x3E
#define Page3_Reserved_5	0x3F

/************************************************************************/
/*	Command                                                             */
/************************************************************************/

//command set
#define Idle_CMD 				0x00
#define Mem_CMD					0x01
#define GenerateRandomId_CMD	0x02
#define CalcCRC_CMD				0x03
#define Transmit_CMD			0x04
#define NoCmdChange_CMD			0x07
#define Receive_CMD				0x08
#define Transceive_CMD			0x0C
#define Reserved_CMD			0x0D
#define MFAuthent_CMD			0x0E
#define SoftReset_CMD			0x0F

void RFID_write(uint8_t RegAddr,uint8_t Value)
{
	uint8_t i, Addr;
	//RFID_SCK = 0;
	clrb(RFID_PORT, RFID_SCK);
	//RFID_NSS = 0;
	clrb(RFID_PORT, RFID_SS);
	Addr = ((RegAddr<<1)&0x7E);

	for(i=8;i>0;i--)    // Truyen dia chi
	{
		//RFID_MOSI = ((Addr&0x80)==0x80);
		if ((Addr&0x80)==0x80)
		{
			setb(RFID_PORT, RFID_MOSI);
		}
		else
		{
			clrb(RFID_PORT, RFID_MOSI);
		}
		//RFID_SCK = 1;
		setb(RFID_PORT, RFID_SCK);
		Addr <<= 1;
		//RFID_SCK = 0;
		clrb(RFID_PORT, RFID_SCK);
	}
	for(i=8;i>0;i--)    // Truyen gia tri can ghi vao
	{
		//RFID_MOSI = ((Value&0x80)==0x80);
		if ((Addr&0x80)==0x80)
		{
			setb(RFID_PORT, RFID_MOSI);
		}
		else
		{
			clrb(RFID_PORT, RFID_MOSI);
		}
		//RFID_SCK = 1;
		setb(RFID_PORT, RFID_SCK);
		Value <<= 1;
		//RFID_SCK = 0;
		clrb(RFID_PORT, RFID_SCK);
	}
	//RFID_NSS = 1;
	//RFID_SCK = 1;
	setb(RFID_PORT, RFID_SS);
	setb(RFID_PORT, RFID_SCK);
}
/*******************************************************************************
Noi Dung    : Doc gia tri thanh ghi bat ky cua RFID.
Tham Bien   : RegAddr : Dia chi thanh ghi.
Tra Ve      : Gia tri cua thanh ghi.
********************************************************************************/

uint8_t RFID_read(uint8_t RegAddr)
{
	uint8_t i, Addr;
	uint8_t Result=0;
	//RFID_SCK = 0;
	clrb(RFID_PORT, RFID_SCK);
	//RFID_NSS = 0;
	clrb(RFID_PORT, RFID_SS);
	Addr = ((RegAddr<<1)&0x7E)|0x80;
	for(i=8;i>0;i--)    // Truyen dia chi
	{
		//RFID_MOSI = ((Addr&0x80)==0x80);
		if ((Addr&0x80)==0x80)
		{
			setb(RFID_PORT, RFID_MOSI);
		}
		else
		{
			clrb(RFID_PORT, RFID_MOSI);
		}
		//RFID_SCK = 1;
		setb(RFID_PORT, RFID_SCK);
		Addr <<= 1;
		//RFID_SCK = 0;
		clrb(RFID_PORT, RFID_SCK);
	}
	for(i=8;i>0;i--)    // Doc gia tri thanh ghi
	{
		//RFID_SCK = 1;
		setb(RFID_PORT, RFID_SCK);
		Result <<= 1;
		//Result|=RFID_MISO;
		if (inbit(RFID_PIN, RFID_MISO))
		{
			Result|=0x01;
		}
		//RFID_SCK = 0;
		clrb(RFID_PORT, RFID_SCK);
	}
	//RFID_NSS = 1;
	//RFID_SCK = 1;
	setb(RFID_PORT, RFID_SS);
	setb(RFID_PORT, RFID_SCK);
	return Result;
}
void RFID_reset()
{
	RFID_write(CommandReg,SoftReset_CMD);
}

void RFID_init()
{
	uint8_t byte;
	RFID_reset();
	
	RFID_write(TModeReg, 0x8D);
	RFID_write(TPrescalerReg, 0x3E);
	RFID_write(TReloadReg_1, 30);
	RFID_write(TReloadReg_2, 0);
	RFID_write(TxASKReg, 0x40);
	RFID_write(ModeReg, 0x3D);
	
	byte = RFID_read(TxControlReg);
	if(!(byte&0x03))
	{
		RFID_write(TxControlReg,byte|0x03);
	}
}
void RFID_ClearBitMask(uint8_t RegAddr,uint8_t Mask)
{
	uint8_t BuffValue = 0x00;
	BuffValue = RFID_read(RegAddr);
	RFID_write(RegAddr, (BuffValue & ~Mask));
}
#define MI_OK                       0
#define MI_NOTAGERR                 1
#define MI_ERR                      2

#define MAXRLEN						18
void RFID_SetBitMask(uint8_t RegAddr,uint8_t Mask)
{
	uint8_t BuffValue = 0x00;
	BuffValue = RFID_read(RegAddr);
	RFID_write(RegAddr,(BuffValue | Mask));
}

uint8_t RFID_ComRFID(uint8_t Command,uint8_t *InData,uint8_t InLenByte,uint8_t *OutData, uint16_t  *OutLenBit)
{
	uint8_t Status = MI_ERR;
	uint8_t IrqEn   = 0x00;
	uint8_t WaitFor = 0x00;
	uint8_t LastBits;
	uint8_t n;
	uint16_t i;

	switch (Command)
	{
		case MFAuthent_CMD:
		{
			IrqEn   = 0x12;
			WaitFor = 0x10;
		}
		break;
		case Transceive_CMD:
		{
			IrqEn   = 0x77;
			WaitFor = 0x30;
		}
		break;
		default:
		break;
	}
	RFID_write(ComIEnReg,IrqEn|0x80);
	RFID_ClearBitMask(ComIrqReg,0x80);
	RFID_write(CommandReg,Idle_CMD);
	RFID_SetBitMask(FIFOLevelReg,0x80);
	
	for (i=0; i<InLenByte; i++)    // Ghi du lieu vao bo nho FIFO
	{
		RFID_write(FIFODataReg, InData[i]);
	}
	RFID_write(CommandReg, Command);
	if (Command == Transceive_CMD)
	{
		RFID_SetBitMask(BitFramingReg,0x80);
	}
	i = 600;    //25ms
	do
	{
		n = RFID_read(ComIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x01) && !(n&WaitFor));
	RFID_ClearBitMask(BitFramingReg,0x80);
	if (i!=0)
	{
		if(!(RFID_read(ErrorReg)&0x1B))
		{
			Status = MI_OK;
			if (n & IrqEn & 0x01)
			{
				Status = MI_NOTAGERR;
			}
			if (Command == Transceive_CMD)
			{
				n = RFID_read(FIFOLevelReg);
				LastBits = RFID_read(ControlReg) & 0x07;
				if (LastBits)
				{
					*OutLenBit = (n-1)*8 + LastBits;
				}
				else
				{
					*OutLenBit = n*8;
				}
				if (n == 0)
				{
					n = 1;
				}
				if (n > MAXRLEN)
				{
					n = MAXRLEN;
				}
				for (i=0; i<n; i++)
				{
					OutData[i] = RFID_read(FIFODataReg);
				}
			}
		}
		else
		{
			Status = MI_ERR;
		}
	}
	RFID_SetBitMask(ControlReg,0x80);                    // stop timer now
	RFID_write(CommandReg,Idle_CMD);
	return Status;
}


uint8_t RFID_Request(uint8_t ReqCode,uint8_t *TagType)  
{	
	uint8_t Status=MI_ERR;  
	uint16_t Length;
	uint8_t Buffer[MAXRLEN]; 

	RFID_ClearBitMask(Status2Reg,0x08);
	RFID_write(BitFramingReg,0x07);
	RFID_SetBitMask(TxControlReg,0x03);
		
	Buffer[0] = ReqCode;
	Status = RFID_ComRFID(Transceive_CMD,Buffer,1,Buffer,&Length);
	if ((Status == MI_OK) && (Length == 0x10))
	{    
		*TagType     = Buffer[0];
		*(TagType+1) = Buffer[1];
	}
	else
	{   
		Status = MI_ERR;   
	}
	return Status;
}

int main(void)
{
	RFID_DDR|=(1<<RFID_SS)|(1<<RFID_MOSI)|(1<<RFID_SCK)|(0<<RFID_MISO);
	init_UART();
	RFID_init();
	uint8_t str[MAXRLEN];	
	print_UART("RFID Reader - RC522");
	int byte = RFID_read(VersionReg);
	print_UART("Version: ");
	vietso_hex_UART(byte);
	putchar_UART('\r');

	while(1)
	{
		byte = RFID_Request(0x52, str);
		vietso_dec_UART(byte);
		putchar_UART(13);
		_delay_ms(1000);
	}
}