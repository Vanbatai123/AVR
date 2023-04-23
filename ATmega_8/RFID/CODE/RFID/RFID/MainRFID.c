/*
* RFID.c
*
* Created: 9/27/2017 11:14:57 AM
*  Author: Van_BasTai
*/


#include "mySTART.h"
#include "myUART.h"
#include "lib\spi.h"
#include "include\RFID_reg.h"
#include "include\RFID_cmd.h"
#include "lib\RFID.c"



//////////////////////////////////////////////////////////////////////////
// Ham Doc Gia Tri Thanh Ghi
//////////////////////////////////////////////////////////////////////////

uint8_t RFID_qq(uint8_t reg)
{
	uint8_t data;
	//ENABLE_CHIP() (SPI_PORT &= (~(1<<SPI_SS)))
	ENABLE_CHIP();
	spi_transmit(((reg<<1)&0x7E)|0x80);
	data = spi_transmit(((reg<<1)&0x7E)|0x80);
	DISABLE_CHIP();
	return data;
}

uint8_t ReadReg(uint8_t RegAddr)
{
	uint8_t i, Addr;
	uint8_t Result=0;

	// 	RFID_SCK = 0;
	clrb(SPI_PORT, SPI_SCK);
	clrb(SPI_PORT, SPI_SS);
	// 	RFID_NSS = 0;
	Addr = ((RegAddr<<1)&0x7E)|0x80;

	for(i=8;i>0;i--)    // Truyen dia chi
	{
		// 		RFID_MOSI = ((Addr&0x80)==0x80);
		if ((Addr&0x80)==0x80)
		setb(SPI_PORT, SPI_MOSI);
		else
		clrb(SPI_PORT, SPI_MOSI);
		// 		RFID_SCK = 1;
		setb(SPI_PORT, SPI_SCK);
		Addr <<= 1;
		//		RFID_SCK = 0;
		clrb(SPI_PORT, SPI_SCK);
	}

	for(i=8;i>0;i--)    // Doc gia tri thanh ghi
	{
		// 		RFID_SCK = 1;
		setb(SPI_PORT, SPI_SCK);
		Result <<= 1;
		//		Result|=RFID_MISO;
		Result|=inbit(SPI_PIN,SPI_MISO);
		//		RFID_SCK = 0;
		clrb(SPI_PORT, SPI_SCK);
	}

	//	RFID_NSS = 1;
	//	RFID_SCK = 1;
	setb(SPI_PORT, SPI_SS);
	setb(SPI_PORT, SPI_SCK);
	return Result;
}

//////////////////////////////////////////////////////////////////////////
// Ham Main
//////////////////////////////////////////////////////////////////////////

int main(void)
{
	_delay_ms(1500);
	uint16_t byte;
	uint8_t str[MAX_LEN];
	spi_init();
	RFID_init();
	init_UART();
	println_UART("RFID Reader - RC522");
	byte = RFID_read(VersionReg);
	print_UART("Version: ");
	vietso_hex_UART(byte);
	putchar_UART('\r');
	byte = RFID_read(ComIEnReg);
	RFID_write(ComIEnReg,byte|0x20);
	byte = RFID_read(DivIEnReg);
	RFID_write(DivIEnReg,byte|0x80);
	while(1)
	{
		byte = RFID_request(PICC_REQALL,str);
		if (byte == CARD_FOUND)
		{
			print_UART("* Dectect Card: ");
			byte = RFID_get_card_serial(str);
			for(byte=0;byte<8;byte++)
			{
				vietso_hex_UART(str[byte]);
				space_UART(5);
			}
			newline_UART();
		}
		_delay_ms(123);
	}
}