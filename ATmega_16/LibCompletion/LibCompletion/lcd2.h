//#include <avr/io.h>
//#include <avr/delay.h>

#define LCD_RS 0
#define LCD_RW 2
#define LCD_E 1
// LCD_putchar writes a character to the LCD at the current address, no busy flag check is done before or after
//the character is written!
//usage: LCD_putchar('A'); or LCD_putchar(0x55);
void LCD_putchar(char data)
{
	//PortD is output
	DDRD = 0xFF;
	//put data on bus
	PORTD = data;
	//RW low, E low
	PORTC &= ~((1<<LCD_RW)|(1<<LCD_E));
	//RS high, strobe E
	PORTC |= ((1<<LCD_RS)|(1<<LCD_E));
	//the number of nops required varies with your clock frequency, try it out!
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	//RS low again, E low (belongs to strobe)
	PORTC &= ~((1<<LCD_RS)|(1<<LCD_E));
	//release bus
	DDRD = 0;
}
//LCD_getaddress reads the address counter and busy flag. For the address only, mask off bit7 of the return
//value.
char LCD_getaddr(void)
{
	//make var for the return value
	char address;
	//PortD is input
	DDRD = 0;
	//RW high, strobe enable
	PORTC |= ((1<<LCD_RW)|(1<<LCD_E));
	asm volatile ("nop");
	asm volatile ("nop");
	//while E is high, get data from LCD
	address = PIND;
	//reset RW to low, E low (for strobe)
	PORTC &= ~((1<<LCD_RW)|(1<<LCD_E));
	//return address and busy flag
	return address;
}
//LCD_wait reads the address counter (which contains the busy flag) and loops until the busy flag is cleared.
void LCD_wait(void)
{
	//get address and busy flag
	//and loop until busy flag cleared
	while((LCD_getaddr() & 0x80) == 0x80)
}
//LCD_command works EXACTLY like LCD_putchar, but takes RS low for accessing the command reg
//see LCD_putchar for details on the code
void LCD_command(char command)
{
	DDRD = 0xFF;
	PORTD = command;
	PORTC &= ~((1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_E));
	PORTC |= (1<<LCD_E);
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	PORTC &= ~(1<<LCD_E);
	DDRD = 0;
}
/*LCD_init initialises the LCD with the following paramters:
8 bit mode, 5*7 font, 2 lines (also for 4 lines)
auto-inc cursor after write and read
cursor and didsplay on, cursor blinking.
*/
void LCD_init(void)
{
	//setup the LCD control signals on PortC
	DDRC |= ((1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_E));
	PORTC = 0x00;
	//if called right after power-up, we'll have to wait a bit (fine-tune for faster execution)
	_delay_loop_2(0xFFFF);
	//tell the LCD that it's used in 8-bit mode 3 times, each with a delay inbetween.
	LCD_command(0x30);
	_delay_loop_2(0xFFFF);
	LCD_command(0x30);
	_delay_loop_2(0xFFFF);
	LCD_command(0x30);
	_delay_loop_2(0xFFFF);
	//now: 8 bit interface, 5*7 font, 2 lines.
	LCD_command(0x38);
	//wait until command finished
	LCD_wait();
	//display on, cursor on (blinking)
	LCD_command(0x0F);
	LCD_wait();
	//now clear the display, cursor home
	LCD_command(0x01);
	LCD_wait();
	//cursor auto-inc
	LCD_command(0x06);
}
//now it's time for a simple function for showing strings on the LCD. It uses the low-level functions above.
//usage example: LCD_write("Hello World!");
void LCD_write(char* dstring)
{
	//is the character pointed at by dstring a zero? If not, write character to LCD
	while(*dstring)
	{
		//if the LCD is bus, let it finish the current operation
		LCD_wait();
		//the write the character from dstring to the LCD, then post-inc the dstring is pointing at.
		LCD_putchar(*dstring++);
	}
}