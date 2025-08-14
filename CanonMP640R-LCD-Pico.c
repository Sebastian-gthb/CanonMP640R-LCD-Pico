#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_LCDEnabled 3
#define PIN_LCDDotClock 4
#define PIN_LCDHSyncPin 5
#define PIN_LCDVSyncPin 6
#define PIN_LCDData0Pin 8
#define PIN_LCDData1Pin 9
#define PIN_LCDData2Pin 10
#define PIN_LCDData3Pin 11
#define PIN_LCDData4Pin 12
#define PIN_LCDData5Pin 13
#define PIN_LCDData6Pin 14
#define PIN_LCDData7Pin 15
#define PIN_LCDDataMask 0x0000FF00  //GPIO 8-15


//const uint8_t DisplayInit[150]={0x00,0x01,0x21,0x03,0xEF,0x03,0x80,0x00,0x81,0x03,0x01,0x02,0x80,0x01,0x21,0x02,0xE1,0x02,0xA0,0x01,0x17,0x02,0x01,0x02,0x60,0x00,0x01,0x02,0x41,0x03,0x60,0x01,0x01,0x02,0xE1,0x03,0xE0,0x00,0x69,0x03,0x01,0x02,0x10,0x01,0x01,0x02,0x01,0x02,0x90,0x00,0x01,0x02,0x35,0x02,0xD0,0x00,0xF3,0x03,0x63,0x02,0xD0,0x01,0x01,0x02,0x41,0x02,0xF0,0x00,0x01,0x02,0x01,0x02,0x28,0x00,0x01,0x02,0xC1,0x02,0x10,0x00,0x01,0x02,0xF7,0x02,0xA8,0x00,0x01,0x03,0xC3,0x03,0x18,0x00,0x41,0x03,0x81,0x02,0x18,0x01,0x01,0x02,0xC1,0x03,0x98,0x00,0x01,0x02,0x01,0x02,0x98,0x01,0x41,0x02,0x01,0x03,0x58,0x00,0xC1,0x03,0xC1,0x03,0x58,0x01,0x01,0x02,0x81,0x02,0xD8,0x00,0x41,0x03,0xC1,0x03,0xD8,0x01,0x01,0x03,0x41,0x02,0xB8,0x00,0x51,0x03,0x41,0x02,0xB8,0x01,0x01,0x03,0x21,0x02};
//const uint16_t DisplayInit[150]={0x0001,0x2103,0xEF03,0x8000,0x8103,0x0102,0x8001,0x2102,0xE102,0xA001,0x1702,0x0102,0x6000,0x0102,0x4103,0x6001,0x0102,0xE103,0xE000,0x6903,0x0102,0x1001,0x0102,0x0102,0x9000,0x0102,0x3502,0xD000,0xF303,0x6302,0xD001,0x0102,0x4102,0xF000,0x0102,0x0102,0x2800,0x0102,0xC102,0x1000,0x0102,0xF702,0xA800,0x0103,0xC303,0x1800,0x4103,0x8102,0x1801,0x0102,0xC103,0x9800,0x0102,0x0102,0x9801,0x4102,0x0103,0x5800,0xC103,0xC103,0x5801,0x0102,0x8102,0xD800,0x4103,0xC103,0xD801,0x0103,0x4102,0xB800,0x5103,0x4102,0xB801,0x0103,0x2102};
const uint16_t DisplayInit[150]={0x0080,0x84C0,0xF7C0,0x0100,0x81C0,0x8040,0x0180,0x8440,0x8740,0x0580,0xE840,0x8040,0x0600,0x8040,0x83C0,0x0680,0x8040,0x87C0,0x0700,0x96C0,0x8040,0x0880,0x8040,0x8040,0x0900,0x8040,0xAC40,0x0B00,0xCFC0,0xC640,0x0B80,0x8040,0x8240,0x0F00,0x8040,0x8040,0x1400,0x8040,0x8340,0x0800,0x8040,0xEF40,0x1500,0x80C0,0xC3C0,0x1800,0x82C0,0x8140,0x1880,0x8040,0x83C0,0x1900,0x8040,0x8040,0x1980,0x8240,0x80C0,0x1A00,0x83C0,0x83C0,0x1A80,0x8040,0x8140,0x1B00,0x82C0,0x83C0,0x1B80,0x80C0,0x8240,0x1D00,0x8AC0,0x8240,0x1D80,0x80C0,0x8440};
const uint16_t DisplayOff[6]={0x0880,0x8040,0x80C0};

uint16_t PixelX = 0;
uint16_t PixelY = 0;
uint8_t ColorRed = 0;
uint8_t ColorGreen = 0;
uint8_t ColorBlue = 0;
uint32_t ColorGPIOOutput = 0;

uint8_t ColorMono = 0;
uint8_t FrameCount = 0;



int main()
{
  stdio_init_all();

  gpio_init(PIN_LCDEnabled);
  gpio_init(PIN_LCDDotClock);
  gpio_init(PIN_LCDHSyncPin);
  gpio_init(PIN_LCDVSyncPin);
  gpio_init(PIN_LCDData0Pin);
  gpio_init(PIN_LCDData1Pin);
  gpio_init(PIN_LCDData2Pin);
  gpio_init(PIN_LCDData3Pin);
  gpio_init(PIN_LCDData4Pin);
  gpio_init(PIN_LCDData5Pin);
  gpio_init(PIN_LCDData6Pin);  
  gpio_init(PIN_LCDData7Pin);
 
  gpio_set_dir(PIN_LCDEnabled, GPIO_OUT);
  gpio_set_dir(PIN_LCDDotClock, GPIO_OUT);
  gpio_set_dir(PIN_LCDHSyncPin, GPIO_OUT);
  gpio_set_dir(PIN_LCDVSyncPin, GPIO_OUT);
  gpio_set_dir(PIN_LCDData0Pin, GPIO_OUT);
  gpio_set_dir(PIN_LCDData1Pin, GPIO_OUT);
  gpio_set_dir(PIN_LCDData2Pin, GPIO_OUT);
  gpio_set_dir(PIN_LCDData3Pin, GPIO_OUT);
  gpio_set_dir(PIN_LCDData4Pin, GPIO_OUT);
  gpio_set_dir(PIN_LCDData5Pin, GPIO_OUT);
  gpio_set_dir(PIN_LCDData6Pin, GPIO_OUT);
  gpio_set_dir(PIN_LCDData7Pin, GPIO_OUT);
 
  gpio_put(PIN_LCDEnabled, 0);
  gpio_put(PIN_LCDDotClock, 0);
  gpio_put(PIN_LCDHSyncPin, 0);
  gpio_put(PIN_LCDVSyncPin, 0);
  gpio_put(PIN_LCDData0Pin, 0);
  gpio_put(PIN_LCDData1Pin, 0);
  gpio_put(PIN_LCDData2Pin, 0);
  gpio_put(PIN_LCDData3Pin, 0);
  gpio_put(PIN_LCDData4Pin, 0);
  gpio_put(PIN_LCDData5Pin, 0);
  gpio_put(PIN_LCDData6Pin, 0);
  gpio_put(PIN_LCDData7Pin, 0);


  // SPI initialisation.
  // Chip select is active-low, so we'll initialise it to a driven-high state
  gpio_init(PIN_CS);
  gpio_set_dir(PIN_CS, GPIO_OUT);
  gpio_put(PIN_CS, 1);

  // The LCD display in the Canon printer has ha SPI clock rate from nearly 1,666MHz.
  // I have the command sniffed as LSB first. So we must send this as SPI_LSB_FIRST. BUT! The Pico can only MSB_FIRST. So we must convert the data!
  // The clock signal pulse to positive (SPI_CPOL_0) and the data is set after the falling edge and will read on the rising edge (SPI_CPHA_0). This is SPI_MODE0 on a ATMEGA.
  spi_init(SPI_PORT, 1666000);
  spi_set_format(SPI_PORT, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
  gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
  gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
  gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    

  //Pico need 23ms from power on to here.
    
  // differen timings to set some lines up on startup
  // maybe the startup of the ATMega needs 30ms?
  //  90ms after Vcc H+Vsync line become high (90-30=50)
  sleep_ms(50);
  gpio_put(PIN_LCDHSyncPin, 1);
  gpio_put(PIN_LCDVSyncPin, 1);
  // 130ms after Vcc SPI CS goes high (130-90=40)
  sleep_ms(40);
  //digitalWrite(LCDSPICSPin,   HIGH);
  // 340ms after Vcc SPI EN goes high and SPI data transfer starts (340-130=210)
  sleep_ms(210);
  gpio_put(PIN_LCDEnabled, 1);


  //init the LCD Display
  uint16_t SendingWord[1] = {0xAA13};
  for (uint8_t i = 0; i < 75; i++) {

    gpio_put(PIN_CS, 0);   //prepare to sending commands

    SendingWord[0] = DisplayInit[i];              //copy 2 bytes (1 word) in the sending buffer

    spi_write16_blocking(spi0, SendingWord, 1);   //sending 2 bytes as one 16bit word from the buffer (i hope)
  
    gpio_put(PIN_CS, 1);   //sending commands finished

    sleep_ms(1);  //between each word is a delay of 1ms  
  }



  // draw test pattern
  while (true) {
    gpio_put(PIN_LCDDotClock,0);
    gpio_put(PIN_LCDDotClock,1);
   
    /*
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");

    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    */


    //sending red
    ColorGPIOOutput = ColorRed << 8;
    gpio_put_masked (PIN_LCDDataMask, ColorGPIOOutput);   

    gpio_put(PIN_LCDDotClock,0);
    gpio_put(PIN_LCDDotClock,1);
    
    /*
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");

    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    */

    //sending green
    ColorGPIOOutput = ColorGreen << 8;
    gpio_put_masked (PIN_LCDDataMask, ColorGPIOOutput);   

    gpio_put(PIN_LCDDotClock,0);
    gpio_put(PIN_LCDDotClock,1);

    /*
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    __asm volatile ("nop\n");
    */


    //sending blue
    ColorGPIOOutput = ColorBlue << 8;
    gpio_put_masked (PIN_LCDDataMask, ColorGPIOOutput);   
  
    PixelX++;
    ColorRed++;
 

    if (PixelX==318)
    {
      gpio_put(PIN_LCDHSyncPin,0);
    }

    if (PixelX == 319)
    {
      gpio_put(PIN_LCDHSyncPin,1);
      PixelY++;
      ColorGreen++;
      PixelX=0;


      if (PixelY == 198)
      {
        gpio_put(PIN_LCDVSyncPin,0);
      }

      if (PixelY == 199)
      {
        gpio_put(PIN_LCDVSyncPin,1);
        ColorBlue++;
        PixelY=0;

        //FrameCount++;
        //ColorMono = FrameCount;

        //ColorGPIOOutput = ColorMono << 8;
        //gpio_put_masked (PIN_LCDDataMask, ColorGPIOOutput);   
      }

    }
  }
}


/*

Ideen:
Often, a state machine is only transferring data in one direction. In this case the SHIFTCTRL_FJOIN option can merge the
two FIFOs into a single 8-entry FIFO going in one direction only. This is useful for high-bandwidth interfaces such as DPI.



*/