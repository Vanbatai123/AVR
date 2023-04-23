#ifndef __VAR_H
#define __VAR_H
typedef struct 
{
	unsigned char* font;
	unsigned char x_size;
	unsigned char y_size;
	unsigned char offset;
	unsigned char numchars;
}current_font;
int fcolorr,fcolorg,fcolorb;
int bcolorr,bcolorg,bcolorb;
int orient;
int display_model, display_transfer_mode, display_serial_mode;
int fch,fcl,bch,bcl;
#endif