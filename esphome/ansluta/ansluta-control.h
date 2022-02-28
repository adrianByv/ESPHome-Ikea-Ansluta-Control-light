/*
  Ansulta Control
	
	created 28.2.2022
	by adrianByv

  Based on https://github.com/matlen67/ansluta-control
	
	Hardware:
		* NodeMCU Lolin V3
		* CC2500 (WLC240)
		
	Wired:
           NodeMCU                   CC2500
  
		SPI     GPIO   Board
		----------------------         --------
		SLC    GPIO14  (D5)             SCLK
		MISO   GPIO12  (D6)             MISO
		MOSI   GPIO13  (D7)             MOSI
		CSx    GPIO15  (D8)             CSN				
*/
#ifndef ANSLUTACONTROL_H
#define ANSLUTACONTROL_H

#include "cc2500_REG.h"
#include "cc2500_VAL.h"
#include "cc2500_CMD.h"

#define CS 15                     // ChipSelect NodeMCU Pin15

// #define CS 15                     // ChipSelect NodeMCU Pin15

#define Light_OFF       0x01      // Command to turn the light off
#define Light_ON_50     0x02      // Command to turn the light on 50%
#define Light_ON_100    0x03      // Command to turn the light on 100%


#include <SPI.h>

class AnslutaControl {
 public:
  void initialize() {
    pinMode(CS,OUTPUT);

    // SPI config 
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV64);
    digitalWrite(CS,HIGH);
    
    SendStrobe(CC2500_SRES); //0x30 SRES Reset chip.  
    init_CC2500();
    
    //  SendStrobe(CC2500_SPWD); //Enter power down mode    -   Not used in the prototype
    WriteReg(0x3E,0xFF);  //Maximum transmit power - write 0xFF to 0x3E (PATABLE)
  }

  void SendStrobe(byte strobe){
    digitalWrite(CS,LOW);
    delayMicroseconds(1);             // can't wait for digitalRead(MISO)==HIGH! Don't work in SPI mode
    SPI.write(strobe);
    digitalWrite(CS,HIGH); 
    delayMicroseconds(2);
  }

  void SendCommand(byte AddressByteA, byte AddressByteB, byte Command){

         
    for(byte i=0;i<50;i++){       	//Send 50 times
      
      SendStrobe(CC2500_SFTX);      // 0x3B
      SendStrobe(CC2500_SIDLE);     // 0x36
      
      digitalWrite(CS,LOW);
      delayMicroseconds(1);         // can't wait for digitalRead(MISO)==HIGH! Don't work in SPI mode
      
      SPI.transfer(0x7F);           // activate burst data
      delayMicroseconds(2);
      
      SPI.transfer(0x06);           // send 6 data bytes
      delayMicroseconds(2);
     
      SPI.transfer(0x55);           // ansluta data byte 1
      delayMicroseconds(2);
      
      SPI.transfer(0x01);           // ansluta data byte 2         
      delayMicroseconds(2);
      
      SPI.transfer(AddressByteA);   // ansluta data address byte A
      delayMicroseconds(2);
      
      SPI.transfer(AddressByteB);   // ansluta data address byte B
      delayMicroseconds(2);
      
      SPI.transfer(Command);        // ansluta data command 0x01=Light OFF 0x02=50% 0x03=100% 0xFF=Pairing
      delayMicroseconds(2);
      
      SPI.transfer(0xAA);           // ansluta data byte 6
      delayMicroseconds(2);

      digitalWrite(CS,HIGH);        // end transfer
             
      SendStrobe(CC2500_STX);      // 0x35  transmit data in TX
      
      delayMicroseconds(1600);
      
    }  

   
    //delay(20);
    //SendStrobe(CC2500_SPWD);      // 0x39  cc2500 sleep
    //delayMicroseconds(2);
  }

  void WriteReg(byte addr, byte value){
    digitalWrite(CS,LOW);
    delayMicroseconds(1);         // can't wait for digitalRead(MISO)==HIGH! Don't work in SPI mode
    SPI.transfer(addr);
    delayMicroseconds(1);
    SPI.transfer(value);
    delayMicroseconds(1);
    digitalWrite(CS,HIGH);  
  }
  
  void init_CC2500(){
    WriteReg(REG_IOCFG2,VAL_IOCFG2);
    WriteReg(REG_IOCFG0,VAL_IOCFG0);
    WriteReg(REG_PKTLEN,VAL_PKTLEN);
    WriteReg(REG_PKTCTRL1,VAL_PKTCTRL1);
    WriteReg(REG_PKTCTRL0,VAL_PKTCTRL0);
    WriteReg(REG_ADDR,VAL_ADDR);
    WriteReg(REG_CHANNR,VAL_CHANNR);
    WriteReg(REG_FSCTRL1,VAL_FSCTRL1);
    WriteReg(REG_FSCTRL0,VAL_FSCTRL0);
    WriteReg(REG_FREQ2,VAL_FREQ2);
    WriteReg(REG_FREQ1,VAL_FREQ1);
    WriteReg(REG_FREQ0,VAL_FREQ0);
    WriteReg(REG_MDMCFG4,VAL_MDMCFG4);
    WriteReg(REG_MDMCFG3,VAL_MDMCFG3);
    WriteReg(REG_MDMCFG2,VAL_MDMCFG2);
    WriteReg(REG_MDMCFG1,VAL_MDMCFG1);
    WriteReg(REG_MDMCFG0,VAL_MDMCFG0);
    WriteReg(REG_DEVIATN,VAL_DEVIATN);
    WriteReg(REG_MCSM2,VAL_MCSM2);
    WriteReg(REG_MCSM1,VAL_MCSM1);
    WriteReg(REG_MCSM0,VAL_MCSM0);
    WriteReg(REG_FOCCFG,VAL_FOCCFG);
    WriteReg(REG_BSCFG,VAL_BSCFG);
    WriteReg(REG_AGCCTRL2,VAL_AGCCTRL2);
    WriteReg(REG_AGCCTRL1,VAL_AGCCTRL1);
    WriteReg(REG_AGCCTRL0,VAL_AGCCTRL0);
    WriteReg(REG_WOREVT1,VAL_WOREVT1);
    WriteReg(REG_WOREVT0,VAL_WOREVT0);
    WriteReg(REG_WORCTRL,VAL_WORCTRL);
    WriteReg(REG_FREND1,VAL_FREND1);
    WriteReg(REG_FREND0,VAL_FREND0);
    WriteReg(REG_FSCAL3,VAL_FSCAL3);
    WriteReg(REG_FSCAL2,VAL_FSCAL2);
    WriteReg(REG_FSCAL1,VAL_FSCAL1);
    WriteReg(REG_FSCAL0,VAL_FSCAL0);
    WriteReg(REG_RCCTRL1,VAL_RCCTRL1);
    WriteReg(REG_RCCTRL0,VAL_RCCTRL0);
    WriteReg(REG_FSTEST,VAL_FSTEST);
    WriteReg(REG_TEST2,VAL_TEST2);
    WriteReg(REG_TEST1,VAL_TEST1);
    WriteReg(REG_TEST0,VAL_TEST0);
    WriteReg(REG_DAFUQ,VAL_DAFUQ);
  }
};

#endif