
/*******************************************************************************
* font
* filename: E:/AVR/ATmega_16/LCD320x240_RX8835/Font/font_v1.0.xml
* name: TaiFont
* family: Arial Narrow
* size: 23
* style: Bold
* included characters: ()ABCDEFGHIJKLMNOPQRSTUVWXYZ
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



static const uint8_t image_data_TaiFont_0x4d[16] = {
    0x0c, 
    0x1c, 
    0x18, 
    0x38, 
    0x38, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x38, 
    0x38, 
    0x18, 
    0x1c, 
    0x0c
};
static const tImage TaiFont_0x4d = { image_data_TaiFont_0x4d, 6, 16};

static const uint8_t image_data_TaiFont_0x5d[16] = {
    0x80, 
    0xc0, 
    0xe0, 
    0x70, 
    0x70, 
    0x30, 
    0x38, 
    0x38, 
    0x38, 
    0x38, 
    0x38, 
    0x70, 
    0x70, 
    0xe0, 
    0xc0, 
    0x80
};
static const tImage TaiFont_0x5d = { image_data_TaiFont_0x5d, 6, 16};

static const uint8_t image_data_TaiFont_0xc1[32] = {
    0x07, 0x00, 
    0x0f, 0x80, 
    0x0f, 0x80, 
    0x0f, 0x80, 
    0x1d, 0xc0, 
    0x1d, 0xc0, 
    0x1d, 0xc0, 
    0x18, 0xc0, 
    0x38, 0xe0, 
    0x38, 0xe0, 
    0x3f, 0xe0, 
    0x7f, 0xf0, 
    0x7f, 0xf0, 
    0x70, 0x70, 
    0xe0, 0x38, 
    0xe0, 0x38
};
static const tImage TaiFont_0xc1 = { image_data_TaiFont_0xc1, 14, 16};

static const uint8_t image_data_TaiFont_0xc2[32] = {
    0x7f, 0xc0, 
    0x7f, 0xe0, 
    0x7f, 0xf0, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x7f, 0xe0, 
    0x7f, 0xc0, 
    0x7f, 0xe0, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x7f, 0xf0, 
    0x7f, 0xe0, 
    0x7f, 0xc0
};
static const tImage TaiFont_0xc2 = { image_data_TaiFont_0xc2, 14, 16};

static const uint8_t image_data_TaiFont_0xc3[32] = {
    0x07, 0xc0, 
    0x1f, 0xe0, 
    0x3f, 0xf0, 
    0x3c, 0x78, 
    0x38, 0x38, 
    0x70, 0x30, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x30, 
    0x78, 0x78, 
    0x3c, 0x78, 
    0x3f, 0xf0, 
    0x1f, 0xe0, 
    0x07, 0xc0
};
static const tImage TaiFont_0xc3 = { image_data_TaiFont_0xc3, 14, 16};

static const uint8_t image_data_TaiFont_0xc4[32] = {
    0x7f, 0xc0, 
    0x7f, 0xe0, 
    0x7f, 0xf0, 
    0x70, 0xf0, 
    0x70, 0x78, 
    0x70, 0x38, 
    0x70, 0x38, 
    0x70, 0x38, 
    0x70, 0x38, 
    0x70, 0x38, 
    0x70, 0x38, 
    0x70, 0x78, 
    0x70, 0xf0, 
    0x7f, 0xf0, 
    0x7f, 0xe0, 
    0x7f, 0x80
};
static const tImage TaiFont_0xc4 = { image_data_TaiFont_0xc4, 14, 16};

static const uint8_t image_data_TaiFont_0xc5[32] = {
    0x7f, 0xe0, 
    0x7f, 0xe0, 
    0x7f, 0xe0, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x7f, 0xe0, 
    0x7f, 0xe0, 
    0x7f, 0xe0, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x7f, 0xe0, 
    0x7f, 0xe0, 
    0x7f, 0xe0
};
static const tImage TaiFont_0xc5 = { image_data_TaiFont_0xc5, 13, 16};

static const uint8_t image_data_TaiFont_0xc6[32] = {
    0x7f, 0xe0, 
    0x7f, 0xe0, 
    0x7f, 0xe0, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x7f, 0xc0, 
    0x7f, 0xc0, 
    0x7f, 0xc0, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00
};
static const tImage TaiFont_0xc6 = { image_data_TaiFont_0xc6, 12, 16};

static const uint8_t image_data_TaiFont_0xc7[32] = {
    0x07, 0xe0, 
    0x0f, 0xf0, 
    0x1f, 0xf8, 
    0x3c, 0x3c, 
    0x38, 0x1c, 
    0x70, 0x10, 
    0x70, 0x00, 
    0x70, 0xfc, 
    0x70, 0xfc, 
    0x70, 0xfc, 
    0x70, 0x1c, 
    0x38, 0x1c, 
    0x3c, 0x3c, 
    0x1f, 0xfc, 
    0x0f, 0xf8, 
    0x07, 0xe0
};
static const tImage TaiFont_0xc7 = { image_data_TaiFont_0xc7, 15, 16};

static const uint8_t image_data_TaiFont_0xc8[32] = {
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x7f, 0xf0, 
    0x7f, 0xf0, 
    0x7f, 0xf0, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70
};
static const tImage TaiFont_0xc8 = { image_data_TaiFont_0xc8, 14, 16};

static const uint8_t image_data_TaiFont_0xc9[16] = {
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70, 
    0x70
};
static const tImage TaiFont_0xc9 = { image_data_TaiFont_0xc9, 5, 16};

static const uint8_t image_data_TaiFont_0xd1[32] = {
    0x03, 0x80, 
    0x03, 0x80, 
    0x03, 0x80, 
    0x03, 0x80, 
    0x03, 0x80, 
    0x03, 0x80, 
    0x03, 0x80, 
    0x03, 0x80, 
    0x03, 0x80, 
    0x03, 0x80, 
    0x63, 0x80, 
    0xe3, 0x80, 
    0xf3, 0x80, 
    0xff, 0x80, 
    0x7f, 0x00, 
    0x3e, 0x00
};
static const tImage TaiFont_0xd1 = { image_data_TaiFont_0xd1, 10, 16};

static const uint8_t image_data_TaiFont_0xd2[32] = {
    0x70, 0x38, 
    0x70, 0x70, 
    0x70, 0xe0, 
    0x71, 0xc0, 
    0x71, 0xc0, 
    0x73, 0x80, 
    0x77, 0x00, 
    0x7f, 0x80, 
    0x7f, 0x80, 
    0x79, 0xc0, 
    0x71, 0xc0, 
    0x70, 0xe0, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x38, 
    0x70, 0x38
};
static const tImage TaiFont_0xd2 = { image_data_TaiFont_0xd2, 14, 16};

static const uint8_t image_data_TaiFont_0xd3[32] = {
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x7f, 0xe0, 
    0x7f, 0xe0, 
    0x7f, 0xe0
};
static const tImage TaiFont_0xd3 = { image_data_TaiFont_0xd3, 12, 16};

static const uint8_t image_data_TaiFont_0xd4[32] = {
    0x78, 0x3c, 
    0x7c, 0x7c, 
    0x7c, 0x7c, 
    0x7c, 0x7c, 
    0x7c, 0x7c, 
    0x74, 0x5c, 
    0x76, 0xdc, 
    0x76, 0xdc, 
    0x76, 0xdc, 
    0x76, 0xdc, 
    0x76, 0xdc, 
    0x72, 0x9c, 
    0x73, 0x9c, 
    0x73, 0x9c, 
    0x73, 0x9c, 
    0x73, 0x9c
};
static const tImage TaiFont_0xd4 = { image_data_TaiFont_0xd4, 16, 16};

static const uint8_t image_data_TaiFont_0xd5[32] = {
    0x70, 0x70, 
    0x78, 0x70, 
    0x78, 0x70, 
    0x7c, 0x70, 
    0x7c, 0x70, 
    0x7e, 0x70, 
    0x7e, 0x70, 
    0x77, 0x70, 
    0x77, 0x70, 
    0x73, 0x70, 
    0x73, 0xf0, 
    0x71, 0xf0, 
    0x71, 0xf0, 
    0x70, 0xf0, 
    0x70, 0xf0, 
    0x70, 0x70
};
static const tImage TaiFont_0xd5 = { image_data_TaiFont_0xd5, 14, 16};

static const uint8_t image_data_TaiFont_0xd6[32] = {
    0x07, 0x80, 
    0x1f, 0xe0, 
    0x3f, 0xf0, 
    0x3c, 0x70, 
    0x78, 0x78, 
    0x70, 0x38, 
    0x70, 0x38, 
    0x70, 0x38, 
    0x70, 0x38, 
    0x70, 0x38, 
    0x70, 0x38, 
    0x78, 0x78, 
    0x3c, 0xf0, 
    0x3f, 0xf0, 
    0x1f, 0xe0, 
    0x07, 0x80
};
static const tImage TaiFont_0xd6 = { image_data_TaiFont_0xd6, 15, 16};

static const uint8_t image_data_TaiFont_0xd7[32] = {
    0x7f, 0xc0, 
    0x7f, 0xe0, 
    0x7f, 0xf0, 
    0x70, 0xf0, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0xf0, 
    0x7f, 0xe0, 
    0x7f, 0xe0, 
    0x7f, 0xc0, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00, 
    0x70, 0x00
};
static const tImage TaiFont_0xd7 = { image_data_TaiFont_0xd7, 13, 16};

static const uint8_t image_data_TaiFont_0xd8[32] = {
    0x07, 0xc0, 
    0x1f, 0xf0, 
    0x3f, 0xf8, 
    0x3c, 0x78, 
    0x78, 0x3c, 
    0x70, 0x1c, 
    0x70, 0x1c, 
    0x70, 0x1c, 
    0x70, 0x1c, 
    0x70, 0x1c, 
    0x70, 0x9c, 
    0x79, 0xdc, 
    0x3c, 0xf8, 
    0x3f, 0xf8, 
    0x1f, 0xfc, 
    0x07, 0xde
};
static const tImage TaiFont_0xd8 = { image_data_TaiFont_0xd8, 15, 16};

static const uint8_t image_data_TaiFont_0xd9[32] = {
    0x7f, 0xc0, 
    0x7f, 0xe0, 
    0x7f, 0xf0, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x7f, 0xf0, 
    0x7f, 0xe0, 
    0x7f, 0x80, 
    0x71, 0xc0, 
    0x71, 0xe0, 
    0x70, 0xe0, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x38, 
    0x70, 0x1c
};
static const tImage TaiFont_0xd9 = { image_data_TaiFont_0xd9, 14, 16};

static const uint8_t image_data_TaiFont_0xe2[32] = {
    0x1f, 0x00, 
    0x3f, 0xc0, 
    0x7f, 0xc0, 
    0x71, 0xe0, 
    0x70, 0xe0, 
    0x70, 0x00, 
    0x7e, 0x00, 
    0x3f, 0x80, 
    0x1f, 0xc0, 
    0x03, 0xe0, 
    0x00, 0xe0, 
    0x70, 0xe0, 
    0x79, 0xe0, 
    0x3f, 0xe0, 
    0x3f, 0xc0, 
    0x0f, 0x80
};
static const tImage TaiFont_0xe2 = { image_data_TaiFont_0xe2, 13, 16};

static const uint8_t image_data_TaiFont_0xe3[32] = {
    0xff, 0xe0, 
    0xff, 0xe0, 
    0xff, 0xe0, 
    0x0e, 0x00, 
    0x0e, 0x00, 
    0x0e, 0x00, 
    0x0e, 0x00, 
    0x0e, 0x00, 
    0x0e, 0x00, 
    0x0e, 0x00, 
    0x0e, 0x00, 
    0x0e, 0x00, 
    0x0e, 0x00, 
    0x0e, 0x00, 
    0x0e, 0x00, 
    0x0e, 0x00
};
static const tImage TaiFont_0xe3 = { image_data_TaiFont_0xe3, 12, 16};

static const uint8_t image_data_TaiFont_0xe4[32] = {
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x78, 0xf0, 
    0x3f, 0xe0, 
    0x3f, 0xe0, 
    0x0f, 0x80
};
static const tImage TaiFont_0xe4 = { image_data_TaiFont_0xe4, 14, 16};

static const uint8_t image_data_TaiFont_0xe5[32] = {
    0xe0, 0x38, 
    0xe0, 0x38, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x38, 0xe0, 
    0x38, 0xe0, 
    0x38, 0xe0, 
    0x3d, 0xe0, 
    0x1d, 0xc0, 
    0x1d, 0xc0, 
    0x1d, 0xc0, 
    0x0f, 0x80, 
    0x0f, 0x80, 
    0x0f, 0x80, 
    0x07, 0x00
};
static const tImage TaiFont_0xe5 = { image_data_TaiFont_0xe5, 13, 16};

static const uint8_t image_data_TaiFont_0xe6[48] = {
    0xe3, 0xe3, 0x80, 
    0xe3, 0xe3, 0x80, 
    0xe3, 0xe3, 0x80, 
    0x63, 0xe3, 0x00, 
    0x73, 0x67, 0x00, 
    0x73, 0x77, 0x00, 
    0x77, 0x77, 0x00, 
    0x77, 0x77, 0x00, 
    0x77, 0x77, 0x00, 
    0x77, 0x77, 0x00, 
    0x36, 0x36, 0x00, 
    0x36, 0x36, 0x00, 
    0x3e, 0x3e, 0x00, 
    0x3e, 0x3e, 0x00, 
    0x3e, 0x3e, 0x00, 
    0x3e, 0x3e, 0x00
};
static const tImage TaiFont_0xe6 = { image_data_TaiFont_0xe6, 18, 16};

static const uint8_t image_data_TaiFont_0xe7[32] = {
    0xe0, 0x38, 
    0x70, 0x70, 
    0x38, 0xe0, 
    0x38, 0xe0, 
    0x1d, 0xc0, 
    0x1d, 0xc0, 
    0x0f, 0x80, 
    0x07, 0x00, 
    0x07, 0x00, 
    0x0f, 0x80, 
    0x1d, 0xc0, 
    0x1d, 0xc0, 
    0x38, 0xe0, 
    0x38, 0xe0, 
    0x70, 0x70, 
    0xe0, 0x38
};
static const tImage TaiFont_0xe7 = { image_data_TaiFont_0xe7, 13, 16};

static const uint8_t image_data_TaiFont_0xe8[32] = {
    0xe0, 0x38, 
    0x70, 0x70, 
    0x70, 0x70, 
    0x38, 0xe0, 
    0x38, 0xe0, 
    0x1d, 0xc0, 
    0x1f, 0xc0, 
    0x0f, 0x80, 
    0x07, 0x00, 
    0x07, 0x00, 
    0x07, 0x00, 
    0x07, 0x00, 
    0x07, 0x00, 
    0x07, 0x00, 
    0x07, 0x00, 
    0x07, 0x00
};
static const tImage TaiFont_0xe8 = { image_data_TaiFont_0xe8, 13, 16};

static const uint8_t image_data_TaiFont_0xe9[32] = {
    0x7f, 0xe0, 
    0x7f, 0xe0, 
    0x7f, 0xe0, 
    0x01, 0xc0, 
    0x01, 0xc0, 
    0x03, 0x80, 
    0x07, 0x00, 
    0x0e, 0x00, 
    0x1c, 0x00, 
    0x1c, 0x00, 
    0x38, 0x00, 
    0x70, 0x00, 
    0xe0, 0x00, 
    0xff, 0xe0, 
    0xff, 0xe0, 
    0xff, 0xe0
};
static const tImage TaiFont_0xe9 = { image_data_TaiFont_0xe9, 12, 16};


static const tChar TaiFont_array[] = {

  // character: '('
  {0x4d, &TaiFont_0x4d},

  // character: ')'
  {0x5d, &TaiFont_0x5d},

  // character: 'A'
  {0xc1, &TaiFont_0xc1},

  // character: 'B'
  {0xc2, &TaiFont_0xc2},

  // character: 'C'
  {0xc3, &TaiFont_0xc3},

  // character: 'D'
  {0xc4, &TaiFont_0xc4},

  // character: 'E'
  {0xc5, &TaiFont_0xc5},

  // character: 'F'
  {0xc6, &TaiFont_0xc6},

  // character: 'G'
  {0xc7, &TaiFont_0xc7},

  // character: 'H'
  {0xc8, &TaiFont_0xc8},

  // character: 'I'
  {0xc9, &TaiFont_0xc9},

  // character: 'J'
  {0xd1, &TaiFont_0xd1},

  // character: 'K'
  {0xd2, &TaiFont_0xd2},

  // character: 'L'
  {0xd3, &TaiFont_0xd3},

  // character: 'M'
  {0xd4, &TaiFont_0xd4},

  // character: 'N'
  {0xd5, &TaiFont_0xd5},

  // character: 'O'
  {0xd6, &TaiFont_0xd6},

  // character: 'P'
  {0xd7, &TaiFont_0xd7},

  // character: 'Q'
  {0xd8, &TaiFont_0xd8},

  // character: 'R'
  {0xd9, &TaiFont_0xd9},

  // character: 'S'
  {0xe2, &TaiFont_0xe2},

  // character: 'T'
  {0xe3, &TaiFont_0xe3},

  // character: 'U'
  {0xe4, &TaiFont_0xe4},

  // character: 'V'
  {0xe5, &TaiFont_0xe5},

  // character: 'W'
  {0xe6, &TaiFont_0xe6},

  // character: 'X'
  {0xe7, &TaiFont_0xe7},

  // character: 'Y'
  {0xe8, &TaiFont_0xe8},

  // character: 'Z'
  {0xe9, &TaiFont_0xe9}

};


const tFont TaiFont = { 28, TaiFont_array };

