/*----------------------------------------------------------------------*/
/* FAT file system sample project for FatFs            (C)ChaN, 2014    */
/*----------------------------------------------------------------------*/

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdio.h>
//#include "E:\AVR\Header\myLCD.h"
#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
#define daobit(thanhghi,bit) thanhghi=(thanhghi^(1<<bit))

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "C:\Users\Administrator\Downloads\Compressed\ffsample\avr\uart.h"
#include "C:\Users\Administrator\Downloads\Compressed\ffsample\avr\xitoa.h"
#include "C:\Users\Administrator\Downloads\Compressed\ffsample\avr\ff.h"
#include "C:\Users\Administrator\Downloads\Compressed\ffsample\avr\diskio.h"
#include "C:\Users\Administrator\Downloads\Compressed\ffsample\avr\rtc.h"
#include "C:\Users\Administrator\Downloads\Compressed\ffsample\avr\sound.h"


FUSES = {0xAF, 0xC3, 0xFF};		/* ATmega64 fuses: Low, High, Extended.
This is the fuse settings for this project. The fuse bits will be included
in the output hex file with program code. However some old flash programmers
cannot load the fuse bits from hex file. If it is the case, remove this line
and use these values to program the fuse bits. */



DWORD AccSize;				/* Work register for fs command */
WORD AccFiles, AccDirs;
FILINFO Finfo;
#if _USE_LFN
char Lfname[_MAX_LFN+1];
#endif

char Line[80];				/* Console input buffer */
BYTE Buff[1024];			/* Working buffer */

FATFS FatFs[_VOLUMES];		/* File system object for each logical drive */
FIL File[2];				/* File object */
DIR Dir;					/* Directory object */


BYTE RtcOk;					/* RTC is available */

volatile UINT Timer;		/* Performance timer (100Hz increment) */



/*---------------------------------------------------------*/
/* 100Hz timer interrupt generated by OC2                  */
/*---------------------------------------------------------*/


ISR(TIMER2_COMP_vect)
{
	Timer++;			/* Performance counter for this module */
	disk_timerproc();	/* Drive timer procedure of low level disk I/O module */
}



/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */
/* This is not required in read-only configuration.        */


DWORD get_fattime (void)
{
	RTC rtc;


	if (!RtcOk) return 0;

	/* Get local time */
	rtc_gettime(&rtc);

	/* Pack date and time into a DWORD variable */
	return	  ((DWORD)(rtc.year - 1980) << 25)
	| ((DWORD)rtc.month << 21)
	| ((DWORD)rtc.mday << 16)
	| ((DWORD)rtc.hour << 11)
	| ((DWORD)rtc.min << 5)
	| ((DWORD)rtc.sec >> 1);
}


/*--------------------------------------------------------------------------*/
/* Monitor                                                                  */


static
void put_dump (const BYTE *buff, DWORD ofs, BYTE cnt)
{
	BYTE i;


	xprintf(PSTR("%08lX:"), ofs);

	for(i = 0; i < cnt; i++)
	xprintf(PSTR(" %02X"), buff[i]);

	xputc(' ');
	for(i = 0; i < cnt; i++)
	xputc((buff[i] >= ' ' && buff[i] <= '~') ? buff[i] : '.');

	xputc('\n');
}


static
void get_line (char *buff, int len)
{
	BYTE c;
	int i = 0;


	for (;;) {
		c = uart_getc();
		if (c == '\r') break;
		if ((c == '\b') && i) {
			i--;
			uart_putc(c);
			continue;
		}
		if (c >= ' ' && i < len - 1) {	/* Visible chars */
			buff[i++] = c;
			xputc(c);
		}
	}
	buff[i] = 0;
	uart_putc('\n');
}


static
FRESULT scan_files (
char* path		/* Pointer to the working buffer with start path */
)
{
	DIR dirs;
	FRESULT res;
	int i;
	char *fn;

	res = f_opendir(&dirs, path);
	if (res == FR_OK) {
		i = strlen(path);
		while (((res = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
			if (_FS_RPATH && Finfo.fname[0] == '.') continue;
			#if _USE_LFN
			fn = *Finfo.lfname ? Finfo.lfname : Finfo.fname;
			#else
			fn = Finfo.fname;
			#endif
			if (Finfo.fattrib & AM_DIR) {
				AccDirs++;
				path[i] = '/'; strcpy(path+i+1, fn);
				res = scan_files(path);
				path[i] = '\0';
				if (res != FR_OK) break;
				} else {
				//				xprintf(PSTR("%s/%s\n"), path, fn);
				AccFiles++;
				AccSize += Finfo.fsize;
			}
		}
	}

	return res;
}



static
void put_rc (FRESULT rc)
{
	const prog_char *p;
	static const prog_char str[] =
	"OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
	"INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
	"INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
	"LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
	FRESULT i;

	for (p = str, i = 0; i != rc && pgm_read_byte_near(p); i++) {
		while(pgm_read_byte_near(p++));
	}
	xprintf(PSTR("rc=%u FR_%S\n"), rc, p);
}




static
void ioinit (void)
{
	/* Pull-up all GPIO pins */
	PORTA = 0b11111111;
	PORTB = 0b11111111;
	PORTC = 0b11111111;
	PORTD = 0b11111111;
	//PORTE = 0b11111111;
	//PORTF = 0b11111111;
	//PORTG = 0b00011111;

	/* Start 100Hz system timer (TC2.OC) */
	OCR2 = F_CPU / 1024 / 100 - 1;
	TCCR2 = 0b00001101;
	TIMSK |= _BV(OCIE2);

	sei();
}



/*-----------------------------------------------------------------------*/
/* Main                                                                  */


int main (void)
{
	char *ptr, *ptr2;
	long p1, p2, p3;
	BYTE res, b1, *bp;
	UINT s1, s2, cnt;
	DWORD ofs, sect = 0;
	FATFS *fs;
	RTC rtc;


	ioinit();				/* Initialize port settings and start system timer process */
	uart_init(115200);		/* Initialize UART driver */
	xdev_out(uart_putc);	/* Register uart_putc() to xitoa module as console output */
	xputs(PSTR("\nFatFs module test monitor for Media \n"));
	xputs(_USE_LFN ? PSTR("LFN Enabled") : PSTR("LFN Disabled"));
	xprintf(PSTR(", Code page: %u\n"), _CODE_PAGE);
	#if _USE_LFN
	Finfo.lfname = Lfname;
	Finfo.lfsize = sizeof Lfname;
	#endif

	if (rtc_init() && rtc_gettime(&rtc)) {		/* Initialize RTC */
		RtcOk = 1;
		xprintf(PSTR("Current time: %u/%u/%u %02u:%02u:%02u\n"), rtc.year, rtc.month, rtc.mday, rtc.hour, rtc.min, rtc.sec);
		} else {
		xputs(PSTR("RTC is not available.\n"));
	}


	for (;;) {
		xputc('>');
		ptr = Line;

		get_line(ptr, sizeof Line);
		switch (*ptr++) {

			case 'T' :
			while (*ptr == ' ') ptr++;

			/* Quick test space */

			break;

			case 'd' :
			switch (*ptr++) {
				case 'd' :	/* dd <pd#> [<sector>] - Dump secrtor */
				if (!xatoi(&ptr, &p1)) break;
				if (!xatoi(&ptr, &p2)) p2 = sect;
				res = disk_read((BYTE)p1, Buff, p2, 1);
				if (res) { xprintf(PSTR("rc=%d\n"), res); break; }
				sect = p2 + 1;
				xprintf(PSTR("Sector:%lu\n"), p2);
				for (bp=Buff, ofs = 0; ofs < 0x200; bp+=16, ofs+=16)
				put_dump(bp, ofs, 16);
				break;

				case 'i' :	/* di <pd#> - Initialize disk */
				if (!xatoi(&ptr, &p1)) break;
				xprintf(PSTR("rc=%d\n"), disk_initialize((BYTE)p1));
				break;

				case 's' :	/* ds <pd#> - Show disk status */
				if (!xatoi(&ptr, &p1)) break;
				if (disk_ioctl((BYTE)p1, GET_SECTOR_COUNT, &p2) == RES_OK)
				{ xprintf(PSTR("Drive size: %lu sectors\n"), p2); }
				if (disk_ioctl((BYTE)p1, GET_BLOCK_SIZE, &p2) == RES_OK)
				{ xprintf(PSTR("Erase block: %lu sectors\n"), p2); }
				if (disk_ioctl((BYTE)p1, MMC_GET_TYPE, &b1) == RES_OK)
				{ xprintf(PSTR("Card type: %u\n"), b1); }
				if (disk_ioctl((BYTE)p1, MMC_GET_CSD, Buff) == RES_OK)
				{ xputs(PSTR("CSD:\n")); put_dump(Buff, 0, 16); }
				if (disk_ioctl((BYTE)p1, MMC_GET_CID, Buff) == RES_OK)
				{ xputs(PSTR("CID:\n")); put_dump(Buff, 0, 16); }
				if (disk_ioctl((BYTE)p1, MMC_GET_OCR, Buff) == RES_OK)
				{ xputs(PSTR("OCR:\n")); put_dump(Buff, 0, 4); }
				if (disk_ioctl((BYTE)p1, MMC_GET_SDSTAT, Buff) == RES_OK) {
					xputs(PSTR("SD Status:\n"));
					for (s1 = 0; s1 < 64; s1 += 16) put_dump(Buff+s1, s1, 16);
				}
				if (disk_ioctl((BYTE)p1, ATA_GET_MODEL, Line) == RES_OK)
				{ Line[40] = '\0'; xprintf(PSTR("Model: %s\n"), Line); }
				if (disk_ioctl((BYTE)p1, ATA_GET_SN, Line) == RES_OK)
				{ Line[20] = '\0'; xprintf(PSTR("S/N: %s\n"), Line); }
				break;

				case 'c' :	/* Disk ioctl */
				switch (*ptr++) {
					case 's' :	/* dcs <pd#> - CTRL_SYNC */
					if (!xatoi(&ptr, &p1)) break;
					xprintf(PSTR("rc=%d\n"), disk_ioctl((BYTE)p1, CTRL_SYNC, 0));
					break;
				}
				break;
			}
			break;

			case 'b' :
			switch (*ptr++) {
				case 'd' :	/* bd <addr> - Dump R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				for (bp=&Buff[p1], ofs = p1, cnt = 32; cnt; cnt--, ptr+=16, ofs+=16)
				put_dump(bp, ofs, 16);
				break;

				case 'e' :	/* be <addr> [<data>] ... - Edit R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				if (xatoi(&ptr, &p2)) {
					do {
						Buff[p1++] = (BYTE)p2;
					} while (xatoi(&ptr, &p2));
					break;
				}
				for (;;) {
					xprintf(PSTR("%04X %02X-"), (WORD)p1, Buff[p1]);
					get_line(Line, sizeof Line);
					ptr = Line;
					if (*ptr == '.') break;
					if (*ptr < ' ') { p1++; continue; }
					if (xatoi(&ptr, &p2))
					Buff[p1++] = (BYTE)p2;
					else
					xputs(PSTR("???\n"));
				}
				break;

				case 'r' :	/* br <pd#> <sector> [<n>] - Read disk into R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				if (!xatoi(&ptr, &p2)) break;
				if (!xatoi(&ptr, &p3)) p3 = 1;
				xprintf(PSTR("rc=%u\n"), disk_read((BYTE)p1, Buff, p2, p3));
				break;

				case 'w' :	/* bw <pd#> <sector> [<n>] - Write R/W buffer into disk */
				if (!xatoi(&ptr, &p1)) break;
				if (!xatoi(&ptr, &p2)) break;
				if (!xatoi(&ptr, &p3)) p3 = 1;
				xprintf(PSTR("rc=%u\n"), disk_write((BYTE)p1, Buff, p2, p3));
				break;

				case 'f' :	/* bf <n> - Fill working buffer */
				if (!xatoi(&ptr, &p1)) break;
				memset(Buff, (BYTE)p1, sizeof Buff);
				break;

			}
			break;

			case 'f' :
			switch (*ptr++) {

				case 'i' :	/* fi <ld#> [<mount>]- Initialize logical drive */
				if (!xatoi(&ptr, &p1) || (UINT)p1 > 9) break;
				if (!xatoi(&ptr, &p2)) p2 = 0;
				xsprintf(Line, PSTR("%u:"), (UINT)p1);
				put_rc(f_mount(&FatFs[p1], Line, (BYTE)p2));
				break;

				case 's' :	/* fs [<path>] - Show logical drive status */
				while (*ptr == ' ') ptr++;
				ptr2 = ptr;
				res = f_getfree(ptr, (DWORD*)&p2, &fs);
				if (res) { put_rc(res); break; }
				xprintf(PSTR("FAT type = %u\nBytes/Cluster = %lu\nNumber of FATs = %u\n"
				"Root DIR entries = %u\nSectors/FAT = %lu\nNumber of clusters = %lu\n"
				"FAT start (lba) = %lu\nDIR start (lba,clustor) = %lu\nData start (lba) = %lu\n\n"),
				fs->fs_type, (DWORD)fs->csize * 512, fs->n_fats,
				fs->n_rootdir, fs->fsize, fs->n_fatent - 2,
				fs->fatbase, fs->dirbase, fs->database
				);
				#if _USE_LABEL
				res = f_getlabel(ptr2, (char*)Buff, (DWORD*)&p1);
				if (res) { put_rc(res); break; }
				xprintf(Buff[0] ? PSTR("Volume name is %s\n") : PSTR("No volume label\n"), Buff);
				xprintf(PSTR("Volume S/N is %04X-%04X\n"), (WORD)((DWORD)p1 >> 16), (WORD)(p1 & 0xFFFF));
				#endif
				xputs(PSTR("..."));
				AccSize = AccFiles = AccDirs = 0;
				strcpy((char*)Buff, ptr);
				res = scan_files((char*)Buff);
				if (res) { put_rc(res); break; }
				xprintf(PSTR("\r%u files, %lu bytes.\n%u folders.\n"
				"%lu KB total disk space.\n%lu KB available.\n"),
				AccFiles, AccSize, AccDirs,
				(fs->n_fatent - 2) * (fs->csize / 2), p2 * (fs->csize / 2)
				);
				break;

				case 'l' :	/* fl [<path>] - Directory listing */
				while (*ptr == ' ') ptr++;
				res = f_opendir(&Dir, ptr);
				if (res) { put_rc(res); break; }
				p1 = s1 = s2 = 0;
				for(;;) {
					res = f_readdir(&Dir, &Finfo);
					if ((res != FR_OK) || !Finfo.fname[0]) break;
					if (Finfo.fattrib & AM_DIR) {
						s2++;
						} else {
						s1++; p1 += Finfo.fsize;
					}
					xprintf(PSTR("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s"),
					(Finfo.fattrib & AM_DIR) ? 'D' : '-',
					(Finfo.fattrib & AM_RDO) ? 'R' : '-',
					(Finfo.fattrib & AM_HID) ? 'H' : '-',
					(Finfo.fattrib & AM_SYS) ? 'S' : '-',
					(Finfo.fattrib & AM_ARC) ? 'A' : '-',
					(Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
					(Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63,
					Finfo.fsize, &(Finfo.fname[0]));
					#if _USE_LFN
					for (p2 = strlen(Finfo.fname); p2 < 14; p2++)
					xputc(' ');
					xprintf(PSTR("%s\n"), Lfname);
					#else
					xputc('\n');
					#endif
				}
				if (res == FR_OK) {
					xprintf(PSTR("%4u File(s),%10lu bytes total\n%4u Dir(s)"), s1, p1, s2);
					if (f_getfree(ptr, (DWORD*)&p1, &fs) == FR_OK)
					xprintf(PSTR(", %10luK bytes free\n"), p1 * fs->csize / 2);
				}
				if (res) put_rc(res);
				break;

				case 'o' :	/* fo <mode> <name> - Open a file */
				if (!xatoi(&ptr, &p1)) break;
				while (*ptr == ' ') ptr++;
				put_rc(f_open(&File[0], ptr, (BYTE)p1));
				break;

				case 'c' :	/* fc - Close a file */
				put_rc(f_close(&File[0]));
				break;

				case 'e' :	/* fe - Seek file pointer */
				if (!xatoi(&ptr, &p1)) break;
				res = f_lseek(&File[0], p1);
				put_rc(res);
				if (res == FR_OK)
				xprintf(PSTR("fptr = %lu(0x%lX)\n"), File[0].fptr, File[0].fptr);
				break;

				case 'r' :	/* fr <len> - read file */
				if (!xatoi(&ptr, &p1)) break;
				p2 = 0;
				cli(); Timer = 0; sei();
				while (p1) {
					if (p1 >= sizeof Buff)	{ cnt = sizeof Buff; p1 -= sizeof Buff; }
					else 			{ cnt = (WORD)p1; p1 = 0; }
					res = f_read(&File[0], Buff, cnt, &s2);
					if (res != FR_OK) { put_rc(res); break; }
					p2 += s2;
					if (cnt != s2) break;
				}
				cli(); s2 = Timer; sei();
				xprintf(PSTR("%lu bytes read with %lu bytes/sec.\n"), p2, s2 ? (p2 * 100 / s2) : 0);
				break;

				case 'd' :	/* fd <len> - read and dump file from current fp */
				if (!xatoi(&ptr, &p1)) break;
				ofs = File[0].fptr;
				while (p1) {
					if (p1 >= 16)	{ cnt = 16; p1 -= 16; }
					else 			{ cnt = (WORD)p1; p1 = 0; }
					res = f_read(&File[0], Buff, cnt, &cnt);
					if (res != FR_OK) { put_rc(res); break; }
					if (!cnt) break;
					put_dump(Buff, ofs, cnt);
					ofs += 16;
				}
				break;

				case 'w' :	/* fw <len> <val> - write file */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				memset(Buff, (BYTE)p2, sizeof Buff);
				p2 = 0;
				cli(); Timer = 0; sei();
				while (p1) {
					if (p1 >= sizeof Buff)	{ cnt = sizeof Buff; p1 -= sizeof Buff; }
					else 			{ cnt = (WORD)p1; p1 = 0; }
					res = f_write(&File[0], Buff, cnt, &s2);
					if (res != FR_OK) { put_rc(res); break; }
					p2 += s2;
					if (cnt != s2) break;
				}
				cli(); s2 = Timer; sei();
				xprintf(PSTR("%lu bytes written with %lu bytes/sec.\n"), p2, s2 ? (p2 * 100 / s2) : 0);
				break;

				case 'v' :	/* fv - Truncate file */
				put_rc(f_truncate(&File[0]));
				break;

				case 'n' :	/* fn <old_name> <new_name> - Change file/dir name */
				while (*ptr == ' ') ptr++;
				ptr2 = strchr(ptr, ' ');
				if (!ptr2) break;
				*ptr2++ = 0;
				while (*ptr2 == ' ') ptr2++;
				put_rc(f_rename(ptr, ptr2));
				break;

				case 'u' :	/* fu <name> - Unlink a file or dir */
				while (*ptr == ' ') ptr++;
				put_rc(f_unlink(ptr));
				break;

				case 'k' :	/* fk <name> - Create a directory */
				while (*ptr == ' ') ptr++;
				put_rc(f_mkdir(ptr));
				break;

				case 'a' :	/* fa <atrr> <mask> <name> - Change file/dir attribute */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				while (*ptr == ' ') ptr++;
				put_rc(f_chmod(ptr, p1, p2));
				break;

				case 't' :	/* ft <year> <month> <day> <hour> <min> <sec> <name> */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				Finfo.fdate = ((p1 - 1980) << 9) | ((p2 & 15) << 5) | (p3 & 31);
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				Finfo.ftime = ((p1 & 31) << 11) | ((p2 & 63) << 5) | ((p3 >> 1) & 31);
				while (*ptr == ' ') ptr++;
				put_rc(f_utime(ptr, &Finfo));
				break;

				case 'x' : /* fx <src_name> <dst_name> - Copy file */
				while (*ptr == ' ') ptr++;
				ptr2 = strchr(ptr, ' ');
				if (!ptr2) break;
				*ptr2++ = 0;
				while (*ptr2 == ' ') ptr2++;
				xprintf(PSTR("Opening \"%s\""), ptr);
				res = f_open(&File[0], ptr, FA_OPEN_EXISTING | FA_READ);
				if (res) {
					put_rc(res);
					break;
				}
				xprintf(PSTR("\nCreating \"%s\""), ptr2);
				res = f_open(&File[1], ptr2, FA_CREATE_ALWAYS | FA_WRITE);
				if (res) {
					put_rc(res);
					f_close(&File[0]);
					break;
				}
				xprintf(PSTR("\nCopying..."));
				cli(); Timer = 0; sei();
				p1 = 0;
				for (;;) {
					res = f_read(&File[0], Buff, sizeof Buff, &s1);
					if (res || s1 == 0) break;   /* error or eof */
					res = f_write(&File[1], Buff, s1, &s2);
					p1 += s2;
					if (res || s2 < s1) break;   /* error or disk full */
				}
				if (res) put_rc(res);
				cli(); s2 = Timer; sei();
				xprintf(PSTR("\n%lu bytes copied with %lu bytes/sec.\n"), p1, p1 * 100 / s2);
				f_close(&File[0]);
				f_close(&File[1]);
				break;
				#if _FS_RPATH
				case 'g' :	/* fg <path> - Change current directory */
				while (*ptr == ' ') ptr++;
				put_rc(f_chdir(ptr));
				break;
				#if _VOLUMES >= 2
				case 'j' :	/* fj <path> - Change current drive */
				while (*ptr == ' ') ptr++;
				put_rc(f_chdrive(ptr));
				break;
				#endif
				#if _FS_RPATH >= 2
				case 'q' :	/* fq - Show current dir path */
				res = f_getcwd(Line, sizeof Line);
				if (res)
				put_rc(res);
				else
				xprintf(PSTR("%s\n"), Line);
				break;
				#endif
				#endif
				#if _USE_LABEL
				case 'b' :	/* fb <name> - Set volume label */
				while (*ptr == ' ') ptr++;
				put_rc(f_setlabel(ptr));
				break;
				#endif
				#if _USE_MKFS
				case 'm' :	/* fm <ld#> <part type> <bytes/clust> - Create file system */
				if (!xatoi(&ptr, &p1) || (UINT)p1 > 9 || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				xprintf(PSTR("The drive %u will be formatted. Are you sure? (Y/n)="), (WORD)p1);
				get_line(Line, sizeof Line);
				if (*ptr == 'Y') {
					xsprintf(Line, PSTR("%u:"), (UINT)p1);
					put_rc(f_mkfs(Line, (BYTE)p2, (WORD)p3));
				}
				break;
				#endif
			}
			break;
			#ifdef SOUND_DEFINED
			case 'p' :	/* p <wavfile> - Play RIFF-WAV file (upto data rate of 48kB/sec) */
			while (*ptr == ' ') ptr++;
			res = f_open(&File[0], ptr, FA_READ);
			if (res) {
				put_rc(res);
				} else {
				load_wav(&File[0], "**** WAV PLAYER ****", Buff, sizeof Buff);
				f_close(&File[0]);
			}
			break;
			#endif
			case 't' :	/* t [<year> <mon> <mday> <hour> <min> <sec>] */
			if (!RtcOk) break;
			if (xatoi(&ptr, &p1)) {
				rtc.year = (WORD)p1;
				xatoi(&ptr, &p1); rtc.month = (BYTE)p1;
				xatoi(&ptr, &p1); rtc.mday = (BYTE)p1;
				xatoi(&ptr, &p1); rtc.hour = (BYTE)p1;
				xatoi(&ptr, &p1); rtc.min = (BYTE)p1;
				if (!xatoi(&ptr, &p1)) break;
				rtc.sec = (BYTE)p1;
				rtc_settime(&rtc);
			}
			rtc_gettime(&rtc);
			xprintf(PSTR("%u/%u/%u %02u:%02u:%02u\n"), rtc.year, rtc.month, rtc.mday, rtc.hour, rtc.min, rtc.sec);
			break;

			case '?' :	/* Show Command List */
			xputs(PSTR(
			"[Disk contorls]\n"
			" di <pd#> - Initialize disk\n"
			" dd [<pd#> <sect>] - Dump a secrtor\n"
			" ds <pd#> - Show disk status\n"
			" dcs <pd#> - ioctl(CTRL_SYNC)\n"
			"[Buffer controls]\n"
			" bd <ofs> - Dump working buffer\n"
			" be <ofs> [<data>] ... - Edit working buffer\n"
			" br <pd#> <sect> [<num>] - Read disk into working buffer\n"
			" bw <pd#> <sect> [<num>] - Write working buffer into disk\n"
			" bf <val> - Fill working buffer\n"
			"[File system controls]\n"
			" fi <ld#> [<mount>] - Force initialized the volume\n"
			" fs [<path>] - Show volume status\n"
			" fl [<path>] - Show a directory\n"
			" fo <mode> <file> - Open a file\n"
			" fc - Close the file\n"
			" fe <ofs> - Move fp in normal seek\n"
			" fd <len> - Read and dump the file\n"
			" fr <len> - Read the file\n"
			" fw <len> <val> - Write to the file\n"
			" fn <org name> <new name> - Rename an object\n"
			" fu <obj name> - Unlink an object\n"
			" fv - Truncate the file at current fp\n"
			" fk <dir name> - Create a directory\n"
			" fa <atrr> <mask> <object name> - Change object attribute\n"
			" ft <year> <month> <day> <hour> <min> <sec> <object name> - Change timestamp of an object\n"
			" fx <src file> <dst file> - Copy a file\n"
			" fg <path> - Change current directory\n"
			" fj <path> - Change current drive\n"
			" fq - Show current directory\n"
			" fm <ld#> <rule> <cluster size> - Create file system\n"
			"[Misc commands]\n"
			" p <wavfile> - Play RIFF-WAVE file\n"
			" t [<year> <month> <mday> <hour> <min> <sec>] - Set/Show current time\n"
			"\n"));
			break;

		}
	}

}

