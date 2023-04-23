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
	if (data_received[j]!=TWDR)
	{
		j++;
	}
	data_received[j]=TWDR;
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