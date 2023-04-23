#define LCD_PORT		PORTD
#define LCD_DDR			DDRD

#define CS	4 //Chon chip	  ss
#define SDA	5 //Data SPI
#define AO	6 //Chot du lieu Command va Data (C/D)
#define SCK	7 //Clock SPI	   ck
/*

#define CS	2 //Chon chip	  ss
#define SDA	3 //Data SPI
#define AO	4 //Chot du lieu Command va Data (C/D)
#define SCK	5 //Clock SPI	   ck*/
const unsigned char  font[][6] PROGMEM =
{	//Font 5x7
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // 20
	,{0x00, 0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
	,{0x00, 0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
	,{0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
	,{0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
	,{0x00, 0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
	,{0x00, 0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
	,{0x00, 0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
	,{0x00, 0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
	,{0x00, 0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
	,{0x00, 0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
	,{0x00, 0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
	,{0x00, 0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
	,{0x00, 0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
	,{0x00, 0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
	,{0x00, 0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
	,{0x00, 0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
	,{0x00, 0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
	,{0x00, 0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
	,{0x00, 0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
	,{0x00, 0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
	,{0x00, 0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
	,{0x00, 0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
	,{0x00, 0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
	,{0x00, 0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
	,{0x00, 0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
	,{0x00, 0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
	,{0x00, 0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
	,{0x00, 0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
	,{0x00, 0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
	,{0x00, 0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
	,{0x00, 0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
	,{0x00, 0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
	,{0x00, 0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
	,{0x00, 0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
	,{0x00, 0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
	,{0x00, 0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
	,{0x00, 0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
	,{0x00, 0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
	,{0x00, 0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
	,{0x00, 0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
	,{0x00, 0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
	,{0x00, 0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
	,{0x00, 0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
	,{0x00, 0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
	,{0x00, 0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
	,{0x00, 0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
	,{0x00, 0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
	,{0x00, 0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
	,{0x00, 0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
	,{0x00, 0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
	,{0x00, 0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
	,{0x00, 0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
	,{0x00, 0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
	,{0x00, 0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
	,{0x00, 0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
	,{0x00, 0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
	,{0x00, 0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
	,{0x00, 0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
	,{0x00, 0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
	,{0x00, 0x02, 0x04, 0x08, 0x10, 0x20} // 5c �
	,{0x00, 0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
	,{0x00, 0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
	,{0x00, 0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
	,{0x00, 0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
	,{0x00, 0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
	,{0x00, 0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
	,{0x00, 0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
	,{0x00, 0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
	,{0x00, 0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
	,{0x00, 0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
	,{0x00, 0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
	,{0x00, 0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
	,{0x00, 0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
	,{0x00, 0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
	,{0x00, 0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
	,{0x00, 0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
	,{0x00, 0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
	,{0x00, 0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
	,{0x00, 0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
	,{0x00, 0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
	,{0x00, 0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
	,{0x00, 0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
	,{0x00, 0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
	,{0x00, 0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
	,{0x00, 0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
	,{0x00, 0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
	,{0x00, 0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
	,{0x00, 0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
	,{0x00, 0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
	,{0x00, 0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
	,{0x00, 0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
	,{0x00, 0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
	,{0x00, 0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
	,{0x00, 0x10, 0x08, 0x08, 0x10, 0x08} // 7e ->
	,{0x00, 0x78, 0x46, 0x41, 0x46, 0x78} // 7f <-
};
const unsigned char anh[][128] PROGMEM =
{	
	 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 192, 224, 224, 240, 248, 254, 254, 255, 251, 250, 254, 252, 252, 255, 255, 255, 254, 252, 254, 250, 251, 255, 254, 254, 248, 224, 224, 192, 192, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } // 20
	,{0, 0, 0, 128, 192, 160, 240, 120, 240, 224, 240, 248, 252, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127, 127, 127, 127, 63, 63, 63, 63, 63, 127, 127, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 252, 248, 240, 224, 240, 248, 240, 160, 64, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } 
	,{0, 0, 231, 246, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127, 63, 31, 255, 255, 255, 255, 254, 254, 254, 254, 254, 254, 254, 126, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 31, 63, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 255, 255, 246, 231, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }
	,{0, 192, 193, 231, 239, 255, 255, 255, 255, 255, 159, 31, 15, 7, 3, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 231, 239, 255, 255, 255, 255, 255, 239, 247, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 1, 3, 7, 15, 31, 159, 255, 255, 255, 255, 255, 239, 231, 193, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }
	,{63, 127, 255, 255, 255, 255, 255, 255, 255, 255, 191, 158, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 142, 191, 255, 255, 255, 255, 255, 255, 255, 255, 127, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }
	,{0, 0, 0, 0, 17, 255, 255, 255, 255, 255, 255, 255, 255, 254, 252, 248, 240, 224, 224, 223, 159, 191, 127, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 255, 255, 255, 127, 127, 191, 159, 223, 224, 224, 240, 248, 252, 254, 255, 255, 255, 255, 255, 255, 255, 255, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }
	,{ 0, 0, 0, 0, 0, 0, 3, 7, 15, 31, 63, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 255, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 253, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127, 63, 31, 15, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }
	,{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 3, 7, 7, 15, 15, 15, 31, 127, 95, 255, 255, 127, 127, 127, 191, 191, 255, 255, 63, 127, 127, 255, 95, 127, 63, 15, 15, 15, 7, 7, 3, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }	
	,
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 192, 64, 96, 32, 48, 28, 30, 223, 255, 111, 110, 76, 12, 238, 239, 15, 238, 236, 12, 206, 207, 223, 223, 30, 60, 48, 96, 96, 192, 192, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	,{ 0, 0, 128, 192, 224, 224, 248, 192, 192, 96, 48, 24, 12, 230, 227, 97, 192, 140, 188, 120, 105, 15, 158, 216, 199, 79, 108, 44, 38, 48, 55, 55, 49, 55, 39, 32, 111, 111, 74, 202, 128, 152, 54, 63, 155, 198, 232, 125, 59, 114, 166, 204, 152, 112, 192, 192, 248, 240, 224, 192, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	,{ 0, 0, 227, 243, 31, 15, 7, 19, 49, 112, 223, 142, 12, 24, 131, 199, 113, 24, 13, 254, 255, 7, 7, 134, 198, 230, 134, 182, 254, 182, 134, 182, 254, 182, 134, 182, 254, 182, 134, 6, 6, 7, 7, 254, 12, 25, 48, 100, 206, 27, 83, 100, 49, 249, 236, 57, 55, 15, 31, 19, 243, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	,{ 0, 128, 193, 199, 108, 248, 240, 112, 208, 144, 24, 12, 6, 3, 1, 0, 0, 0, 0, 255, 255, 251, 251, 123, 59, 59, 59, 251, 251, 139, 91, 27, 75, 155, 251, 59, 251, 11, 251, 59, 251, 251, 255, 255, 0, 0, 0, 0, 1, 3, 6, 12, 24, 25, 144, 240, 240, 240, 248, 110, 195, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	, {30, 55, 123, 187, 221, 237, 252, 251, 247, 175, 159, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 254, 254, 255, 224, 0, 127, 51, 0, 3, 0, 129, 199, 214, 146, 0, 254, 254, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 132, 191, 238, 246, 250, 252, 190, 221, 107, 119, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	,{ 0, 0, 0, 0, 16, 254, 131, 1, 1, 1, 1, 1, 3, 6, 12, 24, 48, 96, 64, 207, 159, 63, 63, 127, 255, 255, 255, 252, 252, 0, 0, 76, 248, 240, 224, 195, 255, 192, 255, 127, 127, 63, 159, 159, 192, 96, 48, 48, 24, 12, 6, 3, 1, 1, 1, 1, 131, 254, 124, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	,{ 0, 0, 0, 0, 0, 0, 1, 7, 12, 24, 48, 96, 192, 128, 128, 48, 124, 239, 98, 48, 157, 201, 227, 122, 246, 134, 197, 229, 79, 15, 236, 236, 12, 47, 239, 239, 37, 4, 254, 254, 107, 105, 1, 0, 126, 98, 98, 118, 60, 128, 192, 96, 48, 24, 12, 6, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 6, 6, 4, 12, 9, 24, 56, 123, 119, 113, 112, 48, 112, 247, 247, 240, 240, 55, 55, 112, 112, 113, 121, 57, 9, 12, 12, 4, 6, 2, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

	
	,{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}
	,{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127, 63, 159, 207, 207, 231, 231, 247, 115, 115, 123, 123, 123, 123, 123, 123, 115, 115, 247, 231, 231, 207, 223, 159, 63, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}
	,{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 15, 1, 112, 124, 255, 255, 15, 195, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 15, 255, 254, 124, 32, 3, 31, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}
	,{ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 207, 31, 63, 127, 255, 255, 255, 254, 128, 0, 0, 128, 255, 255, 240, 143, 128, 207, 223, 31, 15, 7, 96, 120, 120, 98, 7, 15, 15, 143, 206, 128, 143, 200, 255, 255, 128, 0, 0, 128, 254, 255, 255, 255, 127, 63, 31, 207, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}
	,{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 252, 248, 241, 227, 199, 143, 15, 31, 63, 191, 127, 255, 255, 255, 224, 195, 159, 24, 12, 60, 24, 28, 56, 12, 24, 158, 195, 248, 255, 255, 255, 127, 191, 63, 31, 15, 143, 199, 227, 241, 248, 252, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}
	,{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 159, 159, 159, 30, 60, 253, 250, 114, 245, 165, 171, 171, 87, 87, 47, 175, 175, 87, 87, 87, 171, 171, 165, 117, 114, 248, 253, 60, 158, 159, 159, 159, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}
	,{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 249, 240, 245, 255, 248, 250, 248, 248, 249, 254, 224, 199, 205, 204, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 204, 205, 198, 240, 254, 249, 249, 248, 249, 252, 255, 244, 240, 249, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}
	,{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}							
};
void lcd_write(int cd, char byte)//SPI 4Line
{
	clrb(LCD_PORT,CS);
	if(cd)
		setb(LCD_PORT,AO);
	else
		clrb(LCD_PORT,AO);
	for(int h=0;h<8;h++)
	{
		if(!(byte&0x80))
			clrb(LCD_PORT,SDA);
		else
			setb(LCD_PORT,SDA);
		clrb(LCD_PORT,SCK);
		setb(LCD_PORT,SCK);
		byte<<=1;
	}
	daobit(LCD_PORT,AO);
	clrb(LCD_PORT,CS);
}
void move_LCD(unsigned char x, unsigned char y)
{
	y--;
	x--;
	y=y*6;//Graphic Mode thi xoa dong nay di, Text thi de lai
	lcd_write(0,(y&0x0f));
	lcd_write(0,((y>>4)&0x07)|0x10);
	lcd_write(0,(x&0x0f)|0xb0);
}
void clr_LCD()
{
	for(int u=0;u<8;u++)
	{
		move_LCD(u+1,1);
		for(int w=0;w<130;w++)
		{
			lcd_write(1,0);
		}
	}
	move_LCD(1,1);
}
void putchar_LCD(int dat)
{
	if((dat >= 0x20) && (dat <= 0x7f))
	{
		dat=dat-32;
		for(int v=0;v<6;v++)
		lcd_write(1, pgm_read_byte(&font[dat][v]));
	}
}
void print_LCD(char *s)
{
	while(*s)
	putchar_LCD(*s++);
}
void init_LCD()//Khoi tao GLCD
{
	LCD_DDR=0xFF;
	lcd_write(0,0xE2); //Reset
	_delay_ms(100);	   //Delay 100ms
	lcd_write(0,0xAF); //Display ON, 0xAE - OFF
	lcd_write(0,0x2F); //Power control
	clr_LCD(); //Xoa man hinh
}
void vietso_dec_LCD(long num)
{
	char dis[10];
	sprintf(dis, "%li", num);
	print_LCD(dis);
}

void vietsothapphan_LCD(float num)
{
	int x;
	x=(num*1000)/1000;
	vietso_dec_LCD(x);
	putchar_LCD('.');
	x=(int)(num*1000);
	x=x%1000;
	vietso_dec_LCD(x);
}
void vietso_bin_LCD(unsigned long num)
{
	int mask=0X80;
	//hien thi byte trang thai duong ma nhi phan
	for (int i=0;i<8;i++)
	{
		if ((mask&num))
		vietso_dec_LCD(1);
		else
		vietso_dec_LCD(0);
		mask>>=1;
	}
}
void vietso_hex_LCD(long num)
{
	char dis[10];
	sprintf(dis, "0x%lx", num);
	print_LCD(dis);
}
void move_1pixel(unsigned char x, unsigned char y)
{
	y--;
	x--;
	//y=y*6;//Graphic Mode thi xoa dong nay di, Text thi de lai
	lcd_write(0,(y&0x0f));
	lcd_write(0,((y>>4)&0x07)|0x10);
	lcd_write(0,(x&0x0f)|0xb0);
}void picture(int dat)
{
	//if((dat >= 0x20) && (dat <= 0x7f))
	//{
		//dat=dat+8;
		for(int v=0;v<128;v++)
		lcd_write(1, pgm_read_byte(&anh[dat][v]));
	//}
}
void space_LCD(char num)
{
	for (;num>0;num--)
	{
		putchar_LCD(32);
	}
}