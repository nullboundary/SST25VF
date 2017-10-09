/************************************************************************************
 * 	
 * 	Name    : SST25VF.cpp                        
 * 	Author  : Noah Shibley                         
 * 	Date    : Aug 17th, 2013                                   
 * 	Version : 0.1                                              
 * 	Notes   : Based on SST code from: www.Beat707.com design. (Rugged Circuits and Wusik)      
 * 
 * 	
 * 
 ***********************************************************************************/

#include "SST25VF.h"
 

SST25VF::SST25VF(){
 
}
 

void SST25VF::begin(int chipSelect,int writeProtect,int hold){
	
	//set pin #s
	FLASH_SSn = chipSelect;
	FLASH_Wp = writeProtect;
	FLASH_Hold = hold; 
	
  pinMode(FLASH_Wp, OUTPUT); 
  digitalWrite(FLASH_Wp, HIGH); //write protect off

  pinMode(FLASH_Hold, OUTPUT); 
  digitalWrite(FLASH_Hold, HIGH); //mem hold off

  pinMode(FLASH_SSn, OUTPUT); //chip select 
  digitalWrite(FLASH_SSn, HIGH);
  digitalWrite(FLASH_SSn, LOW);
  
  sstSPISettings = SPISettings(SST25VF_SPI_CLOCK, SST25VF_SPI_BIT_ORDER, SST25VF_SPI_MODE);
  SPI.begin();
	SPI.usingInterrupt(255);
  init(); 
	readID();
}
 

void SST25VF::update(){
	

}

// ======================================================================================= //

void SST25VF::waitUntilDone()
{
  uint8_t data = 0;
  while (1)
  {
    
    digitalWrite(FLASH_SSn,LOW);
    (void) SPI.transfer(0x05);
    data = SPI.transfer(0);
    digitalWrite(FLASH_SSn,HIGH);
    if (!bitRead(data,0)) break;
    nop();
  }

}

// ======================================================================================= //

void SST25VF::init()
{
  SPI.beginTransaction(sstSPISettings);
  
  digitalWrite(FLASH_SSn,LOW);
  SPI.transfer(0x50); //enable write status register instruction
  digitalWrite(FLASH_SSn,HIGH);
  delay(50);
  digitalWrite(FLASH_SSn,LOW);
  SPI.transfer(0x01); //write the status register instruction
  SPI.transfer(0x00);//value to write to register - xx0000xx will remove all block protection
  digitalWrite(FLASH_SSn,HIGH);
  delay(50);
  SPI.endTransaction();

  
}

// ======================================================================================= //

void SST25VF::readID()
{
  uint8_t id, mtype, dev;
  SPI.beginTransaction(sstSPISettings);
  digitalWrite(FLASH_SSn,LOW);
  (void) SPI.transfer(0x9F); // Read ID command
  id = SPI.transfer(0);
  mtype = SPI.transfer(0);
  dev = SPI.transfer(0);
  char buf[16] = {0};
  sprintf(buf, "%02X %02X %02X", id, mtype, dev);
  Serial.print("SPI ID ");
  Serial.println(buf);
  digitalWrite(FLASH_SSn,HIGH);
  SPI.endTransaction();;
}

// ======================================================================================= //

void SST25VF::totalErase()
{
  SPI.beginTransaction(sstSPISettings);
  digitalWrite(FLASH_SSn,LOW);
  SPI.transfer(0x06);//write enable instruction
  digitalWrite(FLASH_SSn,HIGH);
  nop();
  digitalWrite(FLASH_SSn, LOW); 
  (void) SPI.transfer(0x60); // Erase Chip //
  digitalWrite(FLASH_SSn, HIGH);
  waitUntilDone();
  SPI.endTransaction();;
}

// ======================================================================================= //

void SST25VF::setAddress(uint32_t addr)
{
  (void) SPI.transfer(addr >> 16);
  (void) SPI.transfer(addr >> 8);  
  (void) SPI.transfer(addr);
}

// ======================================================================================= //

void SST25VF::readInit(uint32_t address)
{
  SPI.beginTransaction(sstSPISettings);
  digitalWrite(FLASH_SSn,LOW);
  (void) SPI.transfer(0x03); // Read Memory - 25/33 Mhz //
  setAddress(address);
}

// ======================================================================================= //

uint8_t SST25VF::readNext() { 
	return SPI.transfer(0); 
	
}

// ======================================================================================= //

void SST25VF::readFinish()
{
  digitalWrite(FLASH_SSn,HIGH);
  SPI.endTransaction();;
}

// ======================================================================================= //

void SST25VF::writeByte(uint32_t address, uint8_t data)
{
  SPI.beginTransaction(sstSPISettings);
  digitalWrite(FLASH_SSn,LOW);
  SPI.transfer(0x06);//write enable instruction
  digitalWrite(FLASH_SSn,HIGH);
  nop();
  digitalWrite(FLASH_SSn,LOW);
  (void) SPI.transfer(0x02); // Write Byte //
  setAddress(address);
  (void) SPI.transfer(data);
  digitalWrite(FLASH_SSn,HIGH);
  waitUntilDone();
  SPI.endTransaction();;
}

uint32_t SST25VF::writeArray(uint32_t address,const uint8_t dataBuffer[],uint16_t dataLength)
{
	for(uint16_t i=0;i<dataLength;i++)
  {
    writeByte((uint32_t)address+i,dataBuffer[i]);
	
  }
	return address + dataLength;
}

void SST25VF::readArray(uint32_t address,uint8_t dataBuffer[],uint16_t dataLength)
{
	readInit((address));

    for (uint16_t i=0; i<dataLength; ++i)
    {
      dataBuffer[i] = readNext();
    }

    readFinish();

}

// ======================================================================================= //

void SST25VF::sectorErase(uint8_t sectorAddress)
{
  SPI.beginTransaction(sstSPISettings);
  digitalWrite(FLASH_SSn,LOW);
  SPI.transfer(0x06);//write enable instruction
  digitalWrite(FLASH_SSn,HIGH);
  nop();
  digitalWrite(FLASH_SSn,LOW);
  (void) SPI.transfer(0x20); // Erase 4KB Sector //
  setAddress(4096UL*long(sectorAddress));
  digitalWrite(FLASH_SSn,HIGH);
  waitUntilDone();
  SPI.endTransaction();;
}