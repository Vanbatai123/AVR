//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// Author : VAN BA TAI	                                                         ;
// Email:   VANBATAI1357@gmail.com													 ;		
// Date :   20/1/2016	                                                             ;
// Version: 1.0                                                                      ;
// Title:   myTWI                                                                    ;
// Description: thu vien Dieu khien module TWI cua AVR                               ;
// - ghi/doc che do Master		                                                     ;
// - ghi/doc che do Slave       							                         ;
//                                                                                   ; 
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#define read 1
#define write 0
int data_received[50],write_data=107;
//------------master------------------------
void TWI_init_master(void) // Function to initialize master(KHOI TAO CHUC NANG MASTER)
{
	TWBR=0x01;    // Bit rate(TOC DO)
	TWSR=(0<<TWPS1)|(0<<TWPS0);    // Setting prescalar bits
	// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
}

void TWI_start(void)
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI(XOA CO NGAT TWI,DAT DIEU KIEN BAT DAU SDA,Enable TWI)
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted(CHO DEN KHI DIEU KIEN BAT DAU DUOC TRUYEN)
	//while((TWSR & 0xF8)!= 0x08); // Check for the acknowledgement(KIEM TRA ACK)
}

void TWI_repeated_start(void)
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); // wait till restart condition is transmitted
	//while((TWSR & 0xF8)!= 0x10); // Check for the acknoledgement
}

void TWI_write_address(unsigned char data)
{
	TWDR=data;    // Address and write instruction(DATA=ADDRESS+WRITE)
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted(CHO TOI KHI 1 BYTE TRONG TWDR DUOC TRUYEN THANH CONG  )
	//while((TWSR & 0xF8)!= 0x18);  // Check for the acknoledgement   (CHECK ACK)
}

void TWI_read_address(unsigned char data)
{
	TWDR=data;    // Address and read instruction(DATA=ADDRESS+READ)
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte received(CHO TOI KHI 1 BYTE DUOC NHAN THANH CONG)
	while((TWSR & 0xF8)!= 0x40);  // Check for the acknoledgement
}

void TWI_write_data(int data[])
{
	TWDR=data[i];    // put data in TWDR
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	//while((TWSR & 0xF8) != 0x28); // Check for the acknoledgement
}

void TWI_read_data(void)
{
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8) != 0x58); // Check for the acknoledgement
	data_received[i]=TWDR;
	
}

void TWI_stop(void)
{
	// Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	//while(!(TWCR & (1<<TWSTO)));  // Wait till stop condition is transmitted
}
void TWI_Master_Send(int add,int data[])
{
	TWI_start();
	TWI_write_address(add+write);
	TWI_write_data(data);
	TWI_stop();
	_delay_ms(1);
}
//------------SLAVE------------------------
void TWI_init_slave(unsigned char add) // Function to initilaize slave
{
	TWAR=add;    // Fill slave address to TWAR
	TWCR=((0<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE));
}

void TWI_write_slave(void) // Function to write data
{
	TWDR= write_data;             // Fill TWDR register whith the data to be sent
	TWCR= (1<<TWEN)|(1<<TWINT);   // Enable TWI, Clear TWI interrupt flag
	while((TWSR & 0xF8) != 0xC0); // Wait for the acknowledgement
}

void TWI_match_write_slave(void) //Function to match the slave address and slave dirction bit(write)
{
	while((TWSR & 0xF8)!= 0xA8)    // Loop till correct acknoledgement have been received
	{
		// Get acknowlegement, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
		while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
	}
}

void TWI_read_slave(void)
{
	
	// Clear TWI interrupt flag,Get acknowlegement, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	while (!(TWCR & (1<<TWINT)));    // Wait for TWINT flag
	while((TWSR & 0xF8)!=0x80);    // Wait for acknowledgement
	if (data_received[i]!=TWDR)
	{
		i++;
	}
	data_received[i]=TWDR;
}

void TWI_match_read_slave(void) //Function to match the slave address and slave direction bit(read)
{
	while((TWSR & 0xF8)!= 0x60)  // Loop till correct acknoledgement have been received
	{
		// Get acknowlegement, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT)|(1<<TWIE);
		while (!(TWCR & (1<<TWINT)));
		// Wait for TWINT flag
	}
}
								