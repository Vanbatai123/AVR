/***Configuration**********

  Device atmega32
  Freq 4.0MHz
  UART: BAUDRATE 19200

*********************/

#include <string.h>
#include <avr/io.h>
# define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "integer.h"
#include "UART_routines.c"
#include "diskio.c"
#include "ff.c"

#define xmit(a) {transmitString((unsigned char*)a);TX_NEWLINE;}

WCHAR buf[100];
static FRESULT f_err_code;
static FATFS FATFS_Obj;
UINT ByteWrite = 0;
char* var="Здарова из программы";


DWORD get_fattime ()
{
	return	  ((DWORD)(/*rtc.year*/2010 - 1980) << 25)
			| ((DWORD)/*rtc.month*/10 << 21)
			| ((DWORD)/*rtc.mday*/7 << 16)
			| ((DWORD)/*rtc.hour*/1 << 11)
			| ((DWORD)/*rtc.min*/26 << 5)
			| ((DWORD)/*rtc.sec*/6 >> 1);
}

int main(void)
{
	uart0_init();
	xmit("********FatFs: write data demo********"); 

	/*initialize and mount*/
	BYTE status = disk_initialize(0);

	sprintf((char*)buf,"initialize return: %X",status);
	xmit(buf);

	f_err_code = f_mount(0, &FATFS_Obj);
	sprintf((char*)buf,"f_mount return: %X",f_err_code);
	xmit(buf);

	FIL fil_obj;

	/*open*/
	f_err_code = f_open(&fil_obj, "foo.txt", FA_WRITE);
	sprintf((char*)buf,"f_open return: %X",f_err_code);
	xmit(buf);

	/*write*/
	f_err_code = f_lseek(&fil_obj,fil_obj.fsize);//смещаем указатель на кол-во байт, равное размеру файла. Т.е. переходим в конец файла.
	sprintf((char*)buf,"f_lseek return: %X, size of file: %d",f_err_code,(int)fil_obj.fsize); 
	xmit(buf);

	f_err_code = f_write(&fil_obj,var,strlen(var),&ByteWrite); //записываем в конец файла строку var
	sprintf((char*)buf,"f_write return: %X, byte wrote: %d",f_err_code,ByteWrite); //возвратит FR_OK=0 в случае успеха, также выводим сколько байтов удалось записать
	xmit(buf);

	/*close*/
	f_close(&fil_obj); //закрываем файл*/

	while(1)
	{
		_delay_ms(1000);
	}

}
