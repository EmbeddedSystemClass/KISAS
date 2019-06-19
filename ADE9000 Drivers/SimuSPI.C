/*******************************************************************************
Copyright(c) 2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.
--------------------------------------------------------------------------------
Filename			: SimuSPI.c
Abstract			: SimuSPI routine
Author				: Yiyi Zhao
Write date			: 2014-10-30(YYYY-MM-DD)
*******************************************************************************/
//!Note: Below defination is not for the system,
//Will not used in the source code
#define ADE9000_Reset_Delay_Value 10000
#define ADE9000_SS_Delay_Value 1000
#define ADE9000_Comu_Delay_Value 1
#define ADE9000_Start_Delay_Value 10






#include "stm32f10x.h"
#include "ADE9000.h"



#define ADE9000_Power_On		GPIO_ResetBits(GPIOE, GPIO_Pin_15)
#define ADE9000_Power_Off 		GPIO_SetBits(GPIOE, GPIO_Pin_15)


#define ADE_MISO_Data   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)

#define Enable_ADE_CS	GPIOA->BRR = GPIO_Pin_4
#define Disable_ADE_CS	GPIOA->BSRR = GPIO_Pin_4
#define High_ADE_SCLK	GPIOA->BSRR = GPIO_Pin_5			
#define Low_ADE_SCLK	GPIOA->BRR = GPIO_Pin_5
#define High_ADE_MOSI	GPIOA->BSRR = GPIO_Pin_7
#define Low_ADE_MOSI	GPIOA->BRR = GPIO_Pin_7
#define High_ADE_PM0	GPIOE->BSRR = GPIO_Pin_7
#define Low_ADE_PM0	GPIOE->BRR = GPIO_Pin_7
#define High_ADE_PM1	GPIOB->BSRR = GPIO_Pin_1
#define Low_ADE_PM1	GPIOB->BRR = GPIO_Pin_1
#define High_ADE_RST	GPIOA->BSRR = GPIO_Pin_3
#define Low_ADE_RST	GPIOA->BRR = GPIO_Pin_3

 


#define ADE_MISO_As_In   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;GPIO_Init(GPIOB, &GPIO_InitStructure)


extern uint32_t ul_DeReduceCounter;

uint8_t uc_ADEChipID;

static unsigned char SPI_MCUReadByte(void);
static void SPI_MCUWriteByte(unsigned char wByte);
static void SPI_Delay(unsigned int n);
static void Fast_SPI_MCUWriteByte(unsigned char wByte);
static void Fast_SPI_FramWriteByte(uint8_t uc_wByte) ;
static uint8_t Fast_SPI_FramReadByte(void);
static void Delay_X_10ms(uint32_t ul_DelayTime);
static void Init_ADE9000_SPI(void);


//!The subroutine simulate SPI port, especially for ADE90xx. 
//Read byte based on simulating SPI port by I/O
//Return data read from SPI port
static unsigned char SPI_MCUReadByte(void) 
{ 
        unsigned char i,rByte=0;  

        for(i=0;i<8;i++) //8 bit read
        {  
                Low_ADE_SCLK;  //Simulate clock signal 
                SPI_Delay(ADE9000_Comu_Delay_Value); 
                High_ADE_SCLK;  
				SPI_Delay(ADE9000_Comu_Delay_Value);
                rByte<<=1;  
                rByte|=ADE_MISO_Data; //get read bit from MISO port 
        }  
        return rByte; //return data
		 
}  

//Delay subroutine
static void Delay_X_10ms(uint32_t ul_DelayTime)
{
	ul_DeReduceCounter = ul_DelayTime;
	while(ul_DeReduceCounter)
		{															     

	}


}

static void SPI_Delay (unsigned int n)
{

  unsigned int i;
  i = n;
  while(i!=0)
  {
    i--;
  }

}

//static void Fast_SPI_Delay (void)
//{
//}


static void SPI_MCUWriteByte(unsigned char wByte) 
{ 
        unsigned char i;  

        for(i=0;i<8;i++) 
        {  
                if((wByte<<i)&0x80){High_ADE_MOSI;}  
                else{Low_ADE_MOSI;}  

                Low_ADE_SCLK;
                SPI_Delay(ADE9000_Comu_Delay_Value);  
                High_ADE_SCLK;  
				SPI_Delay(ADE9000_Comu_Delay_Value);  
        }  
} 


//!The subroutine simulate SPI port, especially for ADE90xx. 
//It is fast mode for simlulating SPI. No delay between high to low or low to high
//Write byte based on simulating SPI port by I/O
static void Fast_SPI_MCUWriteByte(unsigned char wByte) 
{ 
        unsigned char i;  

        for(i=0;i<8;i++) 
        {  
                if((wByte<<i)&0x80){High_ADE_MOSI;}  
                else{Low_ADE_MOSI;}  

                Low_ADE_SCLK;
                //Fast_SPI_Delay();  
                High_ADE_SCLK;  
			//Fast_SPI_Delay();  
        }  
} 


uint16_t  Checkcrc16( uint8_t puc_pCheck_Data[],  uint16_t  us_Nr_Bytes)
    {
          uint8_t  uc_b  =   0 ;
          uint16_t  us_crc  =   0xffff ;
          uint16_t  uc_i, uc_j;
 
          for (uc_i = 0 ; uc_i < us_Nr_Bytes; uc_i ++ )
            {        
                  for (uc_j = 0 ; uc_j < 8 ; uc_j ++ )
                    {
                        uc_b  =  ((puc_pCheck_Data[uc_i] << uc_j) & 0x80 )  ^  ((us_crc & 0x8000 ) >> 8 );
                         
                         us_crc <<= 1 ;
 
                          if (uc_b != 0 )        us_crc ^= 0x1021 ;
                 }  
         }  
 
         return us_crc;
 } 


/*********************************************************************
 Below section related to ADE90XX driver, including
 1. initial ADE90XX	 (maybe changed on future development)
 2. Reset ADE90XX
 3. Read data from ADE90XX
 4. Write data from ADE90XX
**********************************************************************/

void Init_ADE9000(void)
{
	ADE_REGISTER_UNION ADE9000TempData;
	Init_ADE9000_SPI();

	RestoreFramToRegister();
	ADE9000TempData.ul_Register= 1;
	Write_ADE9000_SPI(ADDR_RUN, 2, ADE9000TempData.uc_Register);
	Write_ADE9000_SPI(ADDR_EP_CFG, 2, ADE9000TempData.uc_Register);
	uc_ADEChipID = IsADE9078OrADE9000();
	

}



//!The subroutine is used for reseting ADE90xx. 
//System have 2 I/O pins for low mode controling. Before reseting ADE90xx, Need to set the chip in PM0 mode
//! Delay time is uncetain before finial datasheet released.
void Init_ADE9000_SPI(void)
{
	Low_ADE_PM0;
	Low_ADE_PM1;

	ADE9000_Power_On;
	Delay_X_10ms(0x10);

	High_ADE_RST;
	Delay_X_10ms(0x4);
	Disable_ADE_CS;

	
	High_ADE_SCLK;
	High_ADE_MOSI;
	Delay_X_10ms(0x2);


	
}




void Read_ADE9000_SPI(uint16_t  us_ADE_Addr, uint16_t us_Nr_Bytes, uint8_t *puc_Reg_Data)
{
	uint16_t us_iCounter;
	uint16_t us_iAddress; 
	uint8_t *puc_TempData;
	uint8_t  uc_LS_Addr,uc_MS_Addr;

	us_iAddress = us_ADE_Addr;
	us_iAddress = (us_iAddress << 4);
	us_iAddress = (us_iAddress | 0x08);
	puc_TempData = puc_Reg_Data;

  	uc_LS_Addr = (uint8_t) us_iAddress;
	uc_MS_Addr =(uint8_t) (us_iAddress >> 8);
	
	SPI_Delay(ADE9000_Start_Delay_Value); 
	Enable_ADE_CS;	
	SPI_Delay(ADE9000_Start_Delay_Value); 

	SPI_MCUWriteByte(uc_MS_Addr);

	
	//send LS byte of address
	SPI_MCUWriteByte(uc_LS_Addr);

	High_ADE_MOSI;
	//ADE90xx comm data format: MSB--LSB ; So need send out the MSB first.
	puc_TempData=puc_TempData+(us_Nr_Bytes-1);

	for (us_iCounter=0;us_iCounter<us_Nr_Bytes;us_iCounter++)
	{
		*puc_TempData=SPI_MCUReadByte();
		puc_TempData--;
	 }
	High_ADE_MOSI;
	Disable_ADE_CS;;
	SPI_Delay(ADE9000_Comu_Delay_Value);  
	//enable back the interrupts because the communcation has finished
	//__enable_interrupt();
	
}

uint8_t IsADE9078OrADE9000(void)
{

	uint8_t uc_Read_Data[4];
	uint8_t uc_iChipID = ItIsADE9078;

	Read_ADE9000_SPI(0x472,0x4,uc_Read_Data);

	if(uc_Read_Data[2] == 0x10)
		uc_iChipID = ItIsADE9000;
		

	
	return uc_iChipID;

}


void Read_OTest_ADE9000_SPI(uint16_t  us_ADE_Addr, uint16_t us_Nr_Bytes, uint8_t *puc_Reg_Data)
{
	uint16_t us_iCounter;
	uint16_t us_iAddress; 
	uint8_t *puc_TempData;
	uint8_t  uc_LS_Addr,uc_MS_Addr;

	us_iAddress = us_ADE_Addr;
	us_iAddress = (us_iAddress << 4);
	us_iAddress = (us_iAddress | 0x08);
	puc_TempData = puc_Reg_Data;

  	uc_LS_Addr = (uint8_t) us_iAddress;
	uc_MS_Addr =(uint8_t) (us_iAddress >> 8);
	
	SPI_Delay(ADE9000_Start_Delay_Value); 
	Enable_ADE_CS;	
	//send first the ADE90xx address byte
	SPI_Delay(ADE9000_Start_Delay_Value); 


	//send MS byte of address
	SPI_MCUWriteByte(uc_MS_Addr);

	
	//send LS byte of address
	SPI_MCUWriteByte(uc_LS_Addr);


	//ADE90xx comm data format: MSB--LSB ; So need send out the MSB first.

	for (us_iCounter=0;us_iCounter<us_Nr_Bytes;us_iCounter++)
	{
		*puc_TempData=SPI_MCUReadByte();
		puc_TempData++;
	 }
	High_ADE_MOSI;
	Disable_ADE_CS;;
	SPI_Delay(ADE9000_Comu_Delay_Value);  
	//enable back the interrupts because the communcation has finished
	//__enable_interrupt();
	
}


uint16_t Read_ADE9000_CRC_SPI(uint16_t  us_ADE_Addr, uint16_t us_Nr_Bytes, uint8_t *puc_Reg_Data)
{
	uint16_t us_iCounter;
	uint16_t us_iAddress;  	
	uint8_t  uc_LS_Addr,uc_MS_Addr;
	uint8_t uc_Read_Dara[4];

	us_iAddress = us_ADE_Addr;
	us_iAddress = (us_iAddress << 4);
	us_iAddress = (us_iAddress | 0x08);
	

  	uc_LS_Addr = (uint8_t) us_iAddress;
	uc_MS_Addr =(uint8_t) (us_iAddress >> 8);
	
	SPI_Delay(ADE9000_Start_Delay_Value); 
	Enable_ADE_CS;	
	//send first the ADE90xx address byte
	SPI_Delay(ADE9000_Start_Delay_Value); 

	//Send Read Command
	SPI_MCUWriteByte(uc_MS_Addr);

	
	//send LS byte of address
	SPI_MCUWriteByte(uc_LS_Addr);


	//ADE90xx comm data format: MSB--LSB ; So need send out the MSB first.
	puc_Reg_Data=puc_Reg_Data+(us_Nr_Bytes-1);

	for (us_iCounter=0;us_iCounter<us_Nr_Bytes;us_iCounter++)
	{
		*puc_Reg_Data=SPI_MCUReadByte();
		puc_Reg_Data--;
	 }
	High_ADE_MOSI;
	Disable_ADE_CS;;
	SPI_Delay(ADE9000_Comu_Delay_Value);  
	//enable back the interrupts because the communcation has finished
	//__enable_interrupt();

	puc_Reg_Data=puc_Reg_Data+us_Nr_Bytes;
	for (us_iCounter=0;us_iCounter<us_Nr_Bytes-2;us_iCounter++)
	{
		uc_Read_Dara[us_iCounter] = *puc_Reg_Data;
		puc_Reg_Data--;
	 }
	
	us_iAddress = Checkcrc16(uc_Read_Dara, us_Nr_Bytes-2);

	return us_iAddress;
}


void Write_ADE9000_SPI(uint16_t us_ADE_Addr, uint8_t uc_Nr_Bytes, uint8_t *puc_Reg_Data)
{
	uint8_t uc_iCounter;
	uint16_t us_iAddress;  	
  	uint8_t uc_MS_Addr;
  	uint8_t uc_LS_Addr;

	us_iAddress = us_ADE_Addr;
	us_iAddress = (us_iAddress << 4);
	us_iAddress = (us_iAddress & 0xFFF7);


  	uc_LS_Addr = (uint8_t) us_iAddress;
	uc_MS_Addr =(uint8_t) (us_iAddress >> 8);
	
	SPI_Delay(ADE9000_Start_Delay_Value); 
	  Enable_ADE_CS;
	  
	  SPI_Delay(ADE9000_Start_Delay_Value); 
	  
	
	  //send MS byte of address
	  SPI_MCUWriteByte(uc_MS_Addr);
	
	  //send LS byte of address
	  SPI_MCUWriteByte(uc_LS_Addr);


	  //ADE90xx comm data format: MSB--LSB ; So need send out the MSB first.
	  puc_Reg_Data=puc_Reg_Data+(uc_Nr_Bytes-1);
	  for(uc_iCounter=0;uc_iCounter<uc_Nr_Bytes;uc_iCounter++)
	  {
		SPI_MCUWriteByte(*puc_Reg_Data);
		puc_Reg_Data--;
	  }
  	
	Disable_ADE_CS;
	SPI_Delay(ADE9000_Start_Delay_Value);  
  	//enable back the interrupts because the communcation has finished
   	//__enable_interrupt();
	
}









