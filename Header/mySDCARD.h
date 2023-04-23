#define setb(thanhghi,bit)	thanhghi=thanhghi|(1<<bit)
#define clrb(thanhghi,bit)  thanhghi=thanhghi&(~(1<<bit))
#define inbit(thanhghi,bit) (thanhghi&(1<<bit))>>bit
//Dinh nghia cac chan SPI, hay thay doi tuy theo cau hinh that cua chip
//cau hinh ben duoi danh cho chip atmega16
#define SPI_PORT	PORTB
#define SPI_DDR		DDRB
#define	SCK_PIN		7
#define	MISO_PIN	6
#define	MOSI_PIN	5
#define	SS_PIN		4
//---------------------------------

#define Block_len	512 //dinh nghia do dai 1 sector

//*************************KHONG THAY DOI BAT CU DONG NAO KE TU DONG NAY*****************************
//Dinh nghia mot so lenh co ban giao tiep voi MMC
#define CMD0	0x00	//GO_IDLE_STATE, reset va dua vao trang thai nghi
#define CMD1	0x01	//SEND_OP_COND, Yeu cau Card tra loi ve trang thai cua CARD
#define CMD16	0x10	//SET_BLOCKLEN, Dat do dai (tinh theo byte) cho 1 khoi du lieu (data block)
#define CMD17	0x11	//READ_SINGLE_BLOCK, doc 1 block du lieu cua CARD
#define CMD24	0x18	//WRITE_BLOCK, ghi 1 mang du lieu vao 1 block cua CARD

char mmc_status=0; //cac trang thai tra ve khi thao tac voi card
//mmc_status=0;
//0: ok
//1: loi khi reset, timeout khi reset 
//2: loi xay ra luc goi lenh CMD1
//3: loi khi set blocklen
//4: timeout ghi goi lend writeblock CMD24
//5: timeout trong qua trinh write data
//6: timeout do card dang ban
//7: loi khi goi lenh readblock, CMD17
//8: loi khi kiem tra token cua lenh doc du lieu

//cai dat cho  SPI o che do Master------------------
/*void SPI_MasterInit(void);
void SPI_tByte(int data);
int SPI_rByte(void);
// end of SPI---------------------------------------

//cac lenh giao tiep mmc***********************************************
int mmc_rResponse(int Response);	//yeu cau Response (dap ung) tu card
void mmc_tCommand(int Cmd, int arg); //goi 1 lenh den card
char mmc_init(void); //khoi dong card
char mmc_writeblock(int LBAddress, int *buff); //ghi 1 sector (1 khoi du lieu)
char mmc_readblock(int LBAddress, int *buff);  //doc 1 sector (1 khoi du lieu)*/
//**************************************************************************************************
void SPI_MasterInit(void)
{
	SPI_DDR |=(1<<SCK_PIN)|(1<<MOSI_PIN)|(1<<SS_PIN);
	SPI_PORT|=(1<<MISO_PIN); //dien tro keo len cho chan MISO
	SPCR=(1<<SPIE)|(1<<SPE)|(1<<MSTR);  //chu y la nen set CPOL=0, CPHA=0	(MMC lam viec tot o mode nay)
	//SPIE: interrupt, SPE: enable, MSTR: Master mode, SPR1:0: prescaler=128
}

void SPI_tByte(char data)//transmite one byte
{ 
	SPDR=data;
	while (inbit(SPSR,SPIF)==0); //cho den khi bit SPIF duoc set, qua trinh truyen ket thuc
}

int SPI_rByte(void)//receive one byte
{ 
	int data;
	SPDR=0xFF;//dummy value
	while (inbit(SPSR,SPIF)==0); //cho den khi bit SPIF duoc set, qua trinh truyen dummy ket thuc
	data=SPDR;
	return data;
}
// end of SPI--------------------------------------------------------------------------------------------

// for mmc***********************************************
//nhan va so sanh Response tu mmc
int mmc_rResponse(int Response)
{
	char Timeout=0x0fff;
	char Res;
	while((Timeout--)>0)
	{
		Res=SPI_rByte();
		if (Res==Response) 
			break; //escape from while
	}
	if (Timeout==0) 
		return 1;    //tra ve 1 neu timeout
	else return 0;               // ko co loi, tra ve 0
}

void mmc_tCommand(int Cmd, long arg){
	//command la 48 bit lien tiep trong do
	//bit47=0 (start bit)
	//bit46=1 (transmission bit)
	//bit45:40 la ma lenh
	//bit39:8  la argument cua lenh
	//bit7:1   Cyclic Redundancy Check (CRC)
	//bit0=1 la end bit
	
	clrb(SPI_PORT, SS_PIN); //kich hoat duong SS cua SPI, MMC duoc chon
	SPI_tByte(0xFF); //dummy, 1 lenh luon bat dau 0 nen send FF thi MMC ko dap ung
	SPI_tByte(Cmd | 0x40); //0x40=01000000 la ma bat buoc khi goi lenh
	
	SPI_tByte((int)(arg	>>24));
	SPI_tByte((int)(arg >>16));
	SPI_tByte((int)(arg >> 8));
	SPI_tByte((int) arg);
	
	SPI_tByte(0x95); //CRC, cho lan dau nhung neu cac lan sau send 0x95 cung ko van de;
	SPI_tByte(0xFF); //ko quan cam return
}

char mmc_init(void) // khoi dong giao tiep SD card	
{
	int i;
	SPI_MasterInit();
	setb(SPI_PORT, SS_PIN); //disable SPI MMC
	for (i=0; i<10; i++) 
		SPI_tByte(0xFF);  //MMC se vao SPI mode neu duoc nhan nhieu hon 80 clock tren SCK
	clrb(SPI_PORT, SS_PIN); //cho phep MMC hoat dong
	
	mmc_tCommand(CMD0,0); //lenh CMD0. reset mmc
	if(mmc_rResponse(0x01) == 1) //co loi kiem tra response
	{
		mmc_status=1; // timeout khi reset
		setb(SPI_PORT, SS_PIN); //disable SPI MMC
		return mmc_status;
	}
	
	//goi lenh CMD1 cho den khi nhan duoc response =0 hoac timeout
	i=0xffff; //max timeout
	do
	{
		mmc_tCommand(CMD1,0); //lenh CMD1
		i--;
	} 
	while((SPI_rByte()!=0) && i>0);
	
	if(i == 1)		//co loi khi goi CMD1
	{ 
		mmc_status=2; // loi CMD1
		setb(SPI_PORT, SS_PIN); //disable SPI MMC
		return mmc_status;
	}
	
	mmc_tCommand(CMD16,Block_len); //lenh CMD16, set do dai sector,Block_len=512
	if(mmc_rResponse(0x00) == 1) //co loi kiem tra response
	{	
		mmc_status=3; // timeout khi set len
		setb(SPI_PORT, SS_PIN); //disable SPI MMC
		return mmc_status;
	}
	
	setb(SPI_PORT, SS_PIN); //disable SPI MMC
	return 0; //no error
}

char mmc_writeblock(long LBAddress, char *buff)
{
	long tempA;
	long i;
	tempA=512*LBAddress;
	
	//cach bo tri dia lba cho cho lenh write xxxxxxxx-xxxxxxxx , xxxxxxx0-00000000
	//trong do x la cac bit cua dia chi, nhu vay 9 bit dau cua argument low ko dung
	clrb(SPI_PORT, SS_PIN); //cho phep MMC hoat dong
	mmc_tCommand(CMD24,tempA); //goi lenh cho phep ghi single sector, chu y cach set dia chi
	
	if(mmc_rResponse(0x00) == 1) //co loi kiem tra response
	{	
		mmc_status=4; // timeout khi goi lenh write block
		setb(SPI_PORT, SS_PIN); //disable SPI MMC
		return mmc_status;
	}

	//goi dau hieu (token) bao cho mmc biet sap co data den, dau hieu=0xFE
	SPI_tByte(0xFE);
	
	//goi Block_len=512 byte data
	for (i=0; i<Block_len; i++)
		SPI_tByte(buff[i]);
	
	//goi 2 byte checksum
	SPI_tByte(0xFF);
	SPI_tByte(0xFF);
	
	//doc trai thai response, phai la 0x05
	int status;
	status=SPI_rByte();
	if((status&0x0F) != 0x05) //co loi kiem tra response
	{	
		mmc_status=5; // loi khi goi datablock
		setb(SPI_PORT, SS_PIN); //disable SPI MMC
		return mmc_status;
	}
	
	//cho mmc het busy
	i = 0xffff;						// max timeout
	while(!SPI_rByte() && (--i)); // wait until we are not busy
	if (i==0) 
	{
		mmc_status=6; // loi timeout khi cho busy
		setb(SPI_PORT, SS_PIN); //disable SPI MMC
		return mmc_status;
	}
	
	setb(SPI_PORT, SS_PIN); //disable SPI MMC
	return 0;
}

char mmc_readblock(int LBAddress, char *buff)
{
	int tempA;
	uint16_t i;
	tempA=512*LBAddress;
	
	//cach bo tri dia lba cho cho lenh write xxxxxxxx-xxxxxxxx , xxxxxxx0-00000000
	//trong do x la cac bit cua dia chi, nhu vay 9 bit dau cua argument low ko dung
	clrb(SPI_PORT, SS_PIN); //cho phep MMC hoat dong
	mmc_tCommand(CMD17,tempA); //goi lenh cho phep doc single sector, chu y cach set dia chi
	
	if(mmc_rResponse(0x00) == 1) //co loi kiem tra response
	{	
		mmc_status=7; // timeout khi goi len read block
		setb(SPI_PORT, SS_PIN); //disable SPI MMC
		return mmc_status;
	}
	
	//kiem tra dau hieu (token) read
	if(mmc_rResponse(0xFE) == 1) //co loi kiem tra response
	{	
		mmc_status=8; // timeout khi goi len read block
		setb(SPI_PORT, SS_PIN); //disable SPI MMC
		return mmc_status;
	}
	
	//sau day la goi Block_len=512 byte data
	for (i=0; i<Block_len; i++) 
		buff[i]=SPI_rByte();
	
	//goi 2 byte checksum
	SPI_tByte(0xFF);
	SPI_tByte(0xFF);
	
	setb(SPI_PORT, SS_PIN); //disable SPI MMC
	return 0;
}
