#include <SST25VF.h>
#include <SPI.h>


#define MemCs 6 //chip select
#define MemWp 4 //write protection
#define MemHold 14 //hold

SST25VF flash;

#define DL 11
uint8_t buffer[DL]; //20130903T2046Z

void setup(){
  Serial.begin(38400);

  flash.begin(MemCs,MemWp,MemHold);

}

void loop(){

  Serial.println("writing bytes....");
  for(uint8_t i=0;i<11;i++)
  {
    flash.writeByte((uint32_t)i,i);
    Serial.print("address: ");
    Serial.println(i);
  }
  Serial.println("reading bytes....");

  long x=0;
  flash.readInit((4096UL*x));

  boolean sectorEmpty = true;
  for (int q=0; q<11; q++)
  {
    uint8_t result = flash.readNext();
    Serial.println(result,DEC);
    if (result == 0xFF)
    {

      break;
    }
  }

  flash.readFinish();
//-----------------------------------------------
  Serial.println("erasing...");
  flash.sectorErase(0);

  Serial.println("writing array...");

  for(uint8_t i=0;i<DL;i++)
  {
    buffer[i] = i;
  }

  for(uint8_t j=0;j<10;j++)
  {
    flash.writeArray(DL*j, buffer,DL);
  }

  //-------------------------------------------

  Serial.println("reading Array Written bytes....");

  for(uint8_t j=0;j<10;j++)
  {
    flash.readInit((DL*j));


    for (int q=0; q<DL; q++)
    {
      uint8_t result = flash.readNext();
      Serial.println(result);
      buffer[q] = result;

    }

    flash.readFinish();


    Serial.println("--------------------------------");
     for(uint8_t i=0;i<DL;i++)
     {
        Serial.print(" ");
        Serial.print(buffer[i]);
     }
  }
  Serial.println(".");
  flash.sectorErase(x);
  delay(200);
}
