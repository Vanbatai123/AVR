
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "E:\K-30\Hoc\AVR\SD CARD\pfsample\avr_boot\pff.c"


void flash_erase (DWORD);				/* Erase a flash page (asmfunc.S) */
void flash_write (DWORD, const BYTE*);	/* Program a flash page (asmfunc.S) */

FATFS Fatfs;				/* Petit-FatFs work area */
BYTE Buff[SPM_PAGESIZE];	/* Page data buffer */


int main (void)
{
	DWORD fa;	/* Flash address */
	WORD br;	/* Bytes read */


	pf_mount(&Fatfs);	/* Initialize file system */
	if (pf_open("app.bin") == FR_OK) {	/* Open application file */
		for (fa = 0; fa < 100; fa += SPM_PAGESIZE) {	/* Update all application pages */
			flash_erase(fa);					/* Erase a page */
			memset(Buff, 0xFF, SPM_PAGESIZE);	/* Clear buffer */
			pf_read(Buff, SPM_PAGESIZE, &br);	/* Load a page data */
			if (br) flash_write(fa, Buff);		/* Write it if the data is available */
		}
	}

	if (pgm_read_word(0) != 0xFFFF)		/* Start application if exist */
	((void(*)(void))0)();

	for (;;) ;	/* No application, Halt. */
}

