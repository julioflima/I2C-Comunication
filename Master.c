#include <18F2550.h>
#fuses HSPLL, NOWDT, NOPROTECT, NOLVP, NODEBUG, USBDIV, PLL5, CPUDIV1, VREGEN, NOXINST
#use delay(clock=48000000)
#use i2c(MASTER,SDA=PIN_B0,FAST=450000,SCL=PIN_B1,FORCE_HW)
#use standard_io(b)
#use standard_io(c)
#include <STDLIB.H>
#include<float.h>
#include <string.h> 
#include<usb_cdc.h>

#ZERO_RAM

#define slave_1 0xa0
#define slave_2 0xb0
#define slave_3 0xc0
#define slave_4 0xd0
#define slave_5 0xe0
#define slave_6 0x10
#define slave_7 0x20
#define slave_8 0x30
#define slave_9 0x40



int8 chip_v[10][26];
int16 check_two = 0;
int answer = 0;
int rcv_chip;
int volt_c, chip_c, rcv_i, aux=0;

void Random(void);
void Threat_Data(int);

void Send_I2C(int,int);

void main()
{
   setup_timer_0(RTCC_INTERNAL|RTCC_8_bit); //set the real time o clock
   
   usb_cdc_init();
   usb_init();
   for(;;)
   {
      usb_task();
      if(usb_enumerated())
      {
      /*---------------------------------------------------------------------
         MAIN OF PROJECT
      ---------------------------------------------------------------------*/
      
         Random();
         
      /*---------------------------------------------------------------------
         END THE MAIN OF PROJECT
      ---------------------------------------------------------------------*/
      }
   }
}

void Random(void)
{
   for(chip_c=0;chip_c<10;chip_c++) //generate random values correspondent of chips
   {
      switch(chip_c)
      {
         case 0:
         {  
            srand(get_rtcc()); //seed the rand
            
            check_two = 0;
            for(volt_c=0 ; volt_c<23 ; volt_c++) //generate random values of correspondent voltimeters
            {
               chip_v[chip_c][volt_c] = rand()%256;//volt_c;
               
               if(volt_c >= 3 && volt_c<23)
               {
                  check_two += chip_v[chip_c][volt_c];
               }
               if(volt_c == 1)
               {
                  chip_v[chip_c][2] = chip_v[chip_c][0] + chip_v[chip_c][1];
                  volt_c++;
               }
            }
            
            Threat_Data(chip_c);
            break;
            }

         case 1:
         {
            Send_I2C(chip_c,slave_1);
            Threat_Data(chip_c);
            break;
         }
         
         case 2:
         {
            Send_I2C(chip_c,slave_2);
            Threat_Data(chip_c);
            break;
         }
         case 3:
         {
            Send_I2C(chip_c,slave_3);
            Threat_Data(chip_c);
            break;
         }
         case 4:
         {
            Send_I2C(chip_c,slave_4);
            Threat_Data(chip_c);
            break;
         }
         case 5:
         {
            Send_I2C(chip_c,slave_5);
            Threat_Data(chip_c);
            break;
         }
         case 6:
         {
            Send_I2C(chip_c,slave_6);
            Threat_Data(chip_c);
            break;
         }
         case 7:
         {
            Send_I2C(chip_c,slave_7);
            Threat_Data(chip_c);
            break;
         }
         case 8:
         {
            Send_I2C(chip_c,slave_8);
            Threat_Data(chip_c);
            break;
         }
         case 9:
         {
            Send_I2C(chip_c,slave_9);
            Threat_Data(chip_c);
            break;
         }
      }
   }
}

void Threat_Data(int rcv_chip) //MI DDDD CK ID dddd dddd dddd dddd dddd dddd dddd dddd dddd dddd c ck AA55AA
{
   if(rcv_chip == 0)
   {
      printf(usb_cdc_putc,"MI%X%X%X",chip_v[rcv_chip][0],chip_v[rcv_chip][1],chip_v[rcv_chip][2]);
   }

   printf(usb_cdc_putc,"%X",rcv_chip +1);

   for(rcv_i=3;rcv_i<23;rcv_i++)
   {
      if((rcv_i+1)%2 == 0)
      {
         //printf(usb_cdc_putc,"");
      }
      printf(usb_cdc_putc,"%X",chip_v[rcv_chip][rcv_i]);
   }
   
   printf(usb_cdc_putc,"%X%X",check_two/256,check_two);
   
   if(rcv_chip == 9)
   {
      printf(usb_cdc_putc,"%X%X%X\n",170,85,170);
   }
}

void Send_I2C(int chip_c, int address)
{
   i2c_start ();                             //begin communication
   i2c_write (address);                      //will send to slave address
   i2c_write (1);                            //send data to slave 0xa0
   i2c_stop();                               //stop write cycle to shift to read cycle
   for(volt_c=0 ; volt_c<26 ; volt_c++)      //generate random values of correspondent voltimeters
   {
      i2c_start ();
      i2c_write (address +1);                //add 1 to the address to send a write bit
      chip_v[chip_c][volt_c] = i2c_read(0);
   }
   i2c_stop ();                              //terminate communication
   
   if(chip_v[chip_c][0] == 66)
   {
      for(volt_c=0 ; volt_c<27 ; volt_c++)   //generate random values of correspondent voltimeters
      {
         chip_v[chip_c][volt_c] = chip_v[chip_c][volt_c+1];
      };
   }
   
   check_two = chip_v[chip_c][23]*256 + chip_v[chip_c][24];
}


