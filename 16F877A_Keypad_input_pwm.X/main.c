/* 
 * File:   main.c
 * Author: SuZuHa
 *
 * Created on 07 April 2021, 17:37
 */

#include <stdio.h>
#include <stdlib.h>

#define Baud_rate 9600
#define _XTAL_FREQ 20000000
#define TMR2PRESCALE 4

//Configure Keypad pins
#define X_1 RC3
#define X_2 RB1
#define X_3 RB2
#define X_4 RB3
#define Y_1 RB4
#define Y_2 RB5
#define Y_3 RB6
#define Y_4 RB7

#define Keypad_PORT PORTB
//--------------------

//Configure LCD pins
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
//---------------------

#include "config.h"
#include "uart.h"
#include "pwm.h"
#include "keypad.h"
#include "lcd.h"

#define in1 RC0
#define in2 RC4
float tocdo;
unsigned int xungdem , xungdo;
char str[20];
char k = 'n';   
int input = 0;
int enter = 0;

/// tao thoi gian trich mau 
void timer1()
{
    T1CONbits.TMR1CS=0;// Dùng xung n?i
    //Ch?n b? chia t? l? 1:8
    T1CONbits.T1CKPS1=1;
    T1CONbits.T1CKPS0=1;
    TMR1L=34286&0X00FF; // L?y giá tr? c?a 8 bit th?p
    TMR1H=34286>>8; //Tách ra ???c 8 bit cao.TMR1H=34286>>8; //
    T1CONbits.TMR1ON=1; // B?t timer1
    PIR1bits.TMR1IF = 0; //Xóa c? tràn
    PIE1bits.TMR1IE = 1; //Cho phép ngat timer1
    INTCONbits.PEIE = 1; //Cho phép ngat ngoai vi
    INTCONbits.GIE = 1; //Cho phép ng?t toàn c?c
}
///// tao ham ngat cho chan RB0
void ngat_init()
{
    OPTION_REG = 0b00000000;   // Enables PULL UPs
    GIE=1;                     //Enable Global Interrupt
    PEIE=1;                    //Enable the Peripheral Interrupt
    INTE = 1;                  //Enable RB0 as external Interrupt pin
    INTEDG = 0;
}

/// timer 0
void delay_timer0_ms(unsigned int t)
{
        while(t--) // l?p l?i t l?n cho t?i khi nào nó =0 thì thoát kh?i vòng l?p while
    {
    // delay 1ms dùng timer0
    TMR0=100;// n?p giá tr? ban ??u cho timer0
    TMR0IF=0;//xóa c? tràn
    while(!TMR0IF);
    }
}
void timer0_init()
{
    //khoi tao timer
    OPTION_REGbits.T0CS=0;//dem xung noi
    OPTION_REGbits.PSA=0;//bo chia duoc su dung cho timer
    //ch?n b? chia 1:32
    OPTION_REGbits.PS2=1;
    OPTION_REGbits.PS1=0;
    OPTION_REGbits.PS0=0;
}
//ham ngat dem xung encoder
void interrupt NGAT(void)
{
    if(INTCONbits.INTF==1)   
    {
        xungdem=xungdem+1;
        INTCONbits.INTF=0;
    }
    if(PIR1bits.TMR1IF==1) //n?u x?y ra ng?t timer1
    {
        xungdo = xungdem; 
        PIE1bits.TMR1IE = 0; //D?ng ng?t timer1
        INTCONbits.INTE = 0; //D?ng ng?t ngoài.
        //L?y giá tr? xung ??m ???c trong kho?ngtimer1 ??m
        xungdem=0;
         //Cho ng?t ngoài ??m l?i t? ??u
        INTCONbits.INTE = 1; //Bât ng?t ngoài lên l?i
        PIE1bits.TMR1IE = 1; //B?t ng?t timer1 l?i
        TMR1L=34286&0X00FF;
        TMR1H=34286>>8; //N?p giá tr? l?i cho timer 1
        PIR1bits.TMR1IF = 0; 
    }
}

long convert(long b){
    long a = 1;
    for(int i = 0;i < b;i++)
    {
        a*=10;
    }
    return a;
}

int inputValue()
{
    unsigned char k;
    int arr[10];
    arr[0]=0;
    int dem = 0;
    
    T:while(1)
	{         
		k = switch_press_scan();		
        if(k == '0')
        {
            Lcd_Write_Char(k);
            UART_send_char(k);
            arr[dem++] = 0;
        }
		if(k == '1')
        {
            Lcd_Write_Char(k);
            UART_send_char(k);
            arr[dem++] = 1;
        }
        if(k == '2')
        {
            Lcd_Write_Char(k);
            UART_send_char(k);
            arr[dem++] = 2;
        }
        if(k == '3')
        {
            Lcd_Write_Char(k);
            UART_send_char(k);
            arr[dem++] = 3;
        }
        if(k == '4')
        {
            Lcd_Write_Char(k);
            UART_send_char(k);
            arr[dem++] = 4;
        }
        if(k == '5')
        {
            Lcd_Write_Char(k);
            UART_send_char(k);
            arr[dem++] = 5;
        }
        if(k == '6')
        {
            Lcd_Write_Char(k);
            UART_send_char(k);
            arr[dem++] = 6;
        }
        if(k == '7')
        {
            Lcd_Write_Char(k);
            UART_send_char(k);
            arr[dem++] = 7;
        }
        if(k == '8')
        {
            Lcd_Write_Char(k);
            UART_send_char(k);
            arr[dem++] = 8;
        }
        if(k == '9')
        {
            Lcd_Write_Char(k);
            UART_send_char(k);
            arr[dem++] = 9;
        }   
        if(k == 'D')
        {
            enter++;        
            break;
        }
	}
    int inputInt = 0;
    for(int i = 0;i<dem;i++)
    {
        inputInt += arr[i]*convert((dem-1)-i);
    }
    return inputInt;
}

void Speed()
{
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Enter value from");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("0-100: ");

    input = inputValue();
    if(input <= 100)
    {
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Speed updated");
        __delay_ms(50);
        Lcd_Clear();
    }
    else
    {
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Invalid value");
        __delay_ms(50);
    }
}
void SwitchMode(){
    k = switch_press_scan();
    if(k == 'A')
    {
        in1 = 0;
        in2 = 1;
        UART_send_string(" -Clockwise ");
        Lcd_Clear();
        Lcd_Write_String(" Quay thuan");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(str);
    }
    if(k == 'B')
    {
        in1 = 1;
        in2 = 0;
        UART_send_string(" -Anti-clockwise ");
        Lcd_Clear();
        Lcd_Write_String(" Quay nguoc");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(str);
    }
    if(k == 'C')
    {
        Lcd_Clear();
        Speed();
    }
    if(k == 'D')
    {
        input = 0;
        in1 = 0;
        in2 = 0;
    }
}
void main() 
{
    TRISC = 0x00;
    TRISD = 0x00;
    TRISB0 = 1;
    TRISC5 = 1;
    PORTC = 0;
    
    Keypad_init();
    Lcd_Init();
    PWM1_init(5000);
    PWM1_start();
    Lcd_Clear();
    UART_init();
    Speed();
    timer1();
    ngat_init();
    timer0_init();
    
    Lcd_Write_String(" Press a char");
    UART_send_string("  A.Quay thuan | B.Quay nghich | C.Change speed | D.Emergency Stop|");
    SwitchMode();
    
    while(1)
    {
        tocdo = (1000.0f*xungdo*60)/(360*50); 
        sprintf(str," Toc do = %0.0f v/p " ,tocdo);
        if(RC5==1)
        {
            UART_send_string("  A.Quay thuan | B.Quay nghich | C.Change speed | D.Emergency Stop|");
            Lcd_Clear();
            Lcd_Write_String(" Bam 1 ky tu");
            SwitchMode();
        }
        
        PWM1_Set_duty(input);
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(str);
        UART_send_string(str);
        __delay_ms(200);
    }
}