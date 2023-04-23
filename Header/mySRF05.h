#define SRF05_DDR DDRD
#define SRF05_PORT PORTD
#define SRF05_PIN PIND
#define TRIG 6
#define ECHO 7
void init_SRF05()
{
	setb(SRF05_DDR,TRIG);
	clrb(SRF05_DDR,ECHO);
}
float SRF05()
{
	long biendem;
	float distance;
	setb(SRF05_PORT,TRIG);
	_delay_us(20);
	clrb(SRF05_PORT,TRIG);
	while(inbit(SRF05_PIN,ECHO)==0);
	TCNT1=0;
	while(inbit(SRF05_PIN,ECHO)==1);
	biendem=TCNT1;
	distance=(float)(343*biendem/4000);
	return distance;
}