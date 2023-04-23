
/*******************************************************************************
* font
* filename: E:/AVR/ATmega_16/LCD320x240_RX8835/Font/KPas.xml
* name: TaiFont
* family: Arial Narrow
* size: 13
* style: Bold
* included characters: sKPa
* antialiasing: no
* type: proportional
* encoding: 037
* unicode bom: no
*
* preset name: Monochrome
* data block size: 8 bit(s), uint8_t
* RLE compression enabled: no
* conversion type: Monochrome, Diffuse Dither 128
* bits per pixel: 1
*
* preprocess:
*  main scan direction: top_to_bottom
*  line scan direction: forward
*  inverse: yes
*******************************************************************************/

/*
 typedef struct {
     long int code;
     const tImage *image;
     } tChar;
 typedef struct {
     int length;
     const tChar *chars;
     } tFont;
*/

#include <stdint.h>



static const uint8_t image_data_TaiFont_0x81[16] = {
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x38, 
    0x6c, 
    0x0c, 
    0x3c, 
    0x6c, 
    0x6c, 
    0x3c, 
    0x00, 
    0x00
};
static const tImage TaiFont_0x81 = { image_data_TaiFont_0x81, 7, 16};

static const uint8_t image_data_TaiFont_0xa2[16] = {
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x38, 
    0x6c, 
    0x60, 
    0x38, 
    0x0c, 
    0x6c, 
    0x38, 
    0x00, 
    0x00
};
static const tImage TaiFont_0xa2 = { image_data_TaiFont_0xa2, 7, 16};

static const uint8_t image_data_TaiFont_0xd2[32] = {
    0x00, 0x00, 
    0x00, 0x00, 
    0x00, 0x00, 
    0x00, 0x00, 
    0x00, 0x00, 
    0x33, 0x00, 
    0x36, 0x00, 
    0x36, 0x00, 
    0x3c, 0x00, 
    0x3e, 0x00, 
    0x36, 0x00, 
    0x33, 0x00, 
    0x33, 0x00, 
    0x33, 0x00, 
    0x00, 0x00, 
    0x00, 0x00
};
static const tImage TaiFont_0xd2 = { image_data_TaiFont_0xd2, 9, 16};

static const uint8_t image_data_TaiFont_0xd7[16] = {
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x00, 
    0x3e, 
    0x33, 
    0x33, 
    0x33, 
    0x33, 
    0x3e, 
    0x30, 
    0x30, 
    0x30, 
    0x00, 
    0x00
};
static const tImage TaiFont_0xd7 = { image_data_TaiFont_0xd7, 8, 16};


static const tChar TaiFont_array[] = {

  // character: 'a'
  {0x81, &TaiFont_0x81},

  // character: 's'
  {0xa2, &TaiFont_0xa2},

  // character: 'K'
  {0xd2, &TaiFont_0xd2},

  // character: 'P'
  {0xd7, &TaiFont_0xd7}

};


const tFont TaiFont = { 4, TaiFont_array };

