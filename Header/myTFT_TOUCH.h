#define NUMSAMPLES 2

#define DDR_DATA DDRB
#define PORT_DATA PORTB
#define PIN_DATA PINB

#define _xm			2
#define _yp			3

#define _xp			6	
#define _ym			7		

#define PIN_ADC PINA
#define DDR_ADC DDRA
#define PORT_ADC PORTA

int _rxplate=300;
int x,y,z;

void TSPoint(int16_t x0, int16_t y0, int16_t z0) 
{
  x = x0;
  y = y0;
  z = z0;
}

#if (NUMSAMPLES > 2)
static void insert_sort(int array[], uint8_t size) 
{
	uint8_t j;
	int save;
  
	for (int i = 1; i < size; i++) 
	{
		save = array[i];
		for (j = i; j >= 1 && save < array[j - 1]; j--)
			array[j] = array[j - 1];
		array[j] = save;
	}
}
#endif

void getPoint(void) 
{
	int samples[NUMSAMPLES];
	uint8_t i, valid;
	valid = 1;
	clrb(DDR_ADC,_yp);
	clrb(DDR_DATA,_ym);
	PORT_ADC &= ~(1<<_yp);
	PORT_DATA &= ~(1<<_ym);
	
	setb(DDR_DATA,_xp);
	setb(DDR_ADC,_xm);
	
	PORT_DATA |= (1<<_xp);
	PORT_ADC &= ~(1<<_xm);
   
	for (i=0; i<NUMSAMPLES; i++) 
		samples[i] = read_ADC(_yp);
		
	#if NUMSAMPLES > 2
		insert_sort(samples, NUMSAMPLES);
	#endif
	
	#if NUMSAMPLES == 2
		if (samples[0] != samples[1]) 
			valid = 0;
	#endif
	
	x = (1023-samples[NUMSAMPLES/2]);

	clrb(DDR_DATA,_xp);
	clrb(DDR_ADC,_xm);
	PORT_DATA &= ~(1<<_xp);
   
	setb(DDR_ADC,_yp);
	PORT_ADC |= (1<<_yp);
	setb(DDR_DATA,_ym);
  
	for (i=0; i<NUMSAMPLES; i++) 
		samples[i] = read_ADC(_xm);
		
	#if NUMSAMPLES > 2
	insert_sort(samples, NUMSAMPLES);
	#endif
	
	#if NUMSAMPLES == 2
 	if (samples[0] != samples[1]) 
			valid = 0; 
	#endif
	
	y = (1023-samples[NUMSAMPLES/2]);

	// Set X+ to ground
	setb(DDR_DATA,_xp);
	PORT_DATA &= ~(1<<_xp);
  
	// Set Y- to VCC
	PORT_DATA |= (1<<_ym); 
  
	// Hi-Z X- and Y+
	PORT_ADC &= ~(1<<_yp);
	clrb(DDR_ADC,_yp);
  
	int z1 = read_ADC(_xm); 
	int z2 = read_ADC(_yp);

	if (_rxplate != 0) 
	{
		 // now read the z 
		 float rtouch;
		 rtouch = z2;
		 rtouch /= z1;
		 rtouch -= 1;
		 rtouch *= x;
		 rtouch *= _rxplate;
		 rtouch /= 1024;
     
		z = rtouch;
	} 
	else 
	{
		z = (1023-(z2-z1));
	}

	if (! valid) 
	{
		z = 0;
	}
	
}


/*
void TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym) 
{
	_yp = yp;
	_xm = xm;
	_ym = ym;
	_xp = xp;
	_rxplate = 0;
	pressureThreshhold = 10;
}



void TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym, uint16_t rxplate) 
{
	_yp = yp;
	_xm = xm;
	_ym = ym;
	_xp = xp;
	_rxplate = rxplate;

	pressureThreshhold = 10;
}
*/
/*
int TouchScreen::readTouchX(void) 
{
	pinMode(_yp, INPUT);
	pinMode(_ym, INPUT);
	digitalWrite(_yp, LOW);
	digitalWrite(_ym, LOW);
   
	pinMode(_xp, OUTPUT);
	digitalWrite(_xp, HIGH);
	pinMode(_xm, OUTPUT);
	digitalWrite(_xm, LOW);
   
	return (1023-analogRead(_yp));
}


int TouchScreen::readTouchY(void) 
{
	pinMode(_xp, INPUT);
	pinMode(_xm, INPUT);
	digitalWrite(_xp, LOW);
	digitalWrite(_xm, LOW);
   
	pinMode(_yp, OUTPUT);
	digitalWrite(_yp, HIGH);
	pinMode(_ym, OUTPUT);
	digitalWrite(_ym, LOW);
   
	return (1023-analogRead(_xm));
}


uint16_t TouchScreen::pressure(void) 
{
	// Set X+ to ground
	pinMode(_xp, OUTPUT);
	digitalWrite(_xp, LOW);
  
	// Set Y- to VCC
	pinMode(_ym, OUTPUT);
	digitalWrite(_ym, HIGH); 
  
	// Hi-Z X- and Y+
	digitalWrite(_xm, LOW);
	pinMode(_xm, INPUT);
	digitalWrite(_yp, LOW);
	pinMode(_yp, INPUT);
  
	int z1 = analogRead(_xm); 
	int z2 = analogRead(_yp);

	if (_rxplate != 0) 
	{
		// now read the x 
		float rtouch;
		rtouch = z2;
		rtouch /= z1;
		rtouch -= 1;
		rtouch *= readTouchX();
		rtouch *= _rxplate;
		rtouch /= 1024;
		
		return rtouch;
	} 
	else 
	{
		return (1023-(z2-z1));
	}
}
*/
