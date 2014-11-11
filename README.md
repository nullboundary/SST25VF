```
Name    : SST25VF Library                         
Author  : Noah Shibley, http://socialhardware.net                       
Date    : Aug 17th, 2013                                  
Version : 0.1                                               
Notes   : For control of  Nor Serial Flash 16Mbit (2Mbytes) - S71295 (SST) 2097152 bytes.
Dependencies:	 Arduino & SST25VF004, SST25VF016, SST25VF064, etc Nor Serial Flash Chips   
```

Much of the code in the library is Based on SST code from: (Rugged Circuits and Wusik)
Should work for SST25VF004, SST25VF016, SST25VF064, etc. 

### List of Functions

```cpp
void begin(int chipSelect,int writeProtect,int hold);
void update();
void readID();
void totalErase();
void sectorErase(uint8_t sectorAddress);
	
void readInit(uint32_t address);
uint8_t readNext();
void readFinish();
void readArray(uint32_t address,uint8_t dataBuffer[],uint16_t dataLength);
	
void writeByte(uint32_t address, uint8_t data);
uint32_t writeArray(uint32_t address,const uint8_t dataBuffer[],uint16_t dataLength);
```

