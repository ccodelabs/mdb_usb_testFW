#include "mbed.h"

AnalogIn current_value(A0);
AnalogIn usb_value(A1);

DigitalOut led(LED1);

Serial pc(USBTX, USBRX);

#define IOS (0xA0) // PA_5 + PA_7
 
PortOut myIOs(PortA, IOS);

InterruptIn eventA1(PA_3);
InterruptIn eventA3(PA_4);

int riseA1=0,fallA1=0,riseA3=0,fallA3=0,val=0;

void fall_A1()
{
   fallA1++;
}

void rise_A1()
{
   if(val==0)
   {
       val++;
   }
   else
   {
       riseA1++;
   }
}

void rise_A3()
{
   riseA3++;
}

void fall_A3()
{
   fallA3++;
}

int main()
{

    int meas_cur; //const int base = 240*5;

    eventA1.fall(&fall_A1);
    eventA1.rise(&rise_A1);
    eventA3.rise(&rise_A3);
    eventA3.fall(&fall_A3);
    
    myIOs = myIOs ^ IOS;

    while(1) {

        char c = pc.getc(); // Read hyperterminal
        
        if(c == '1') {
            meas_cur = current_value.read()*3300;
            pc.printf("measure:%d\n", meas_cur);
        }
        else if(c == '2')
        {
            myIOs = IOS; // Toggle IOs level
            printf("RELAY OFF\n");
            wait(0.5); // 500 ms
        }
        else if(c == '6')
        {
            myIOs = 0x00; // Toggle IOs level
            printf("RELAY ON\n");
            wait(0.5); // 500 ms
        }
        else if (c == '3')
        {
           riseA1=0, fallA1=0, riseA3=0, fallA3=0, val=0;
           pc.printf("Reset\r\n");
        }
        else if(c == '4')
        {
           printf("Values:%d;%d;%d;%d  |\r\n", val, riseA1, fallA1, riseA3, fallA3);
           val++;
        }
        else if(c == '5')
        {
            meas_cur = usb_value.read()*3300;
            pc.printf("USBVALUE:%d\n", meas_cur);    
        }
        wait(1.0); // 1 second
    }
}