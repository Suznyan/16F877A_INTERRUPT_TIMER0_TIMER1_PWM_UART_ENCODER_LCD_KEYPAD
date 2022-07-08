//PWM function

long frequency;
int PWM_Max_Duty()
{
    return(_XTAL_FREQ/(frequency*TMR2PRESCALE));
}

void PWM1_init(long freq)
{
    PR2 = (_XTAL_FREQ/(freq*4*TMR2PRESCALE)) - 1;
    frequency = freq;
}

void PWM2_init(long freq)
{
    PR2 = (_XTAL_FREQ/(freq*4*TMR2PRESCALE)) - 1;
    frequency = freq;
}

void PWM1_Set_duty(unsigned int duty)
{
    duty = ((float)duty/100) * 1023; //percentage;
    if(duty<1024)
      {
        duty = ((float)duty/1023)*PWM_Max_Duty();
        CCP1CONbits.CCP1X = duty & 0x02;
        CCP1CONbits.CCP1Y = duty & 0x01;
        CCPR1L = duty>>0x02;
      }

  }

void PWM2_Set_duty(unsigned int duty)
{
    duty = ((float)duty/100) * 1023; //percentage;
    if(duty<1024)
    {
      duty = ((float)duty/1023)*PWM_Max_Duty();
      CCP2CONbits.CCP2X = duty & 0x02;
      CCP2CONbits.CCP2Y = duty & 0x01;
      CCPR2L = duty>>0x02;
    }
}

void PWM1_start()
{
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;

    #if TMR2PRESCALAR == 1
      T2CONbits.T2CKPS0 = 0;
      T2CONbits.T2CKPS1 = 0;
    #elif TMR2PRESCALAR == 4
      T2CONbits.T2CKPS0 = 1;
      T2CONbits.T2CKPS1 = 0;
    #elif TMR2PRESCALAR == 16
      T2CONbits.T2CKPS0 = 1;
      T2CONbits.T2CKPS1 = 1;
    #endif

    T2CONbits.TMR2ON = 1;
    TRISC2 = 0;
} 

PWM1_stop()
{
    CCP1CONbits.CCP1M3 = 0;
    CCP1CONbits.CCP1M2 = 0;
}

PWM2_start()
{
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;

    #if TMR2PRESCALAR == 1 
      //T2CKPS0 = 0;
      //T2CKPS1 = 0;
    #elif TMR2PRESCALAR == 4
      T2CONbits.T2CKPS0 = 1;
      T2CONbits.T2CKPS1 = 0;
    #elif TMR2PRESCALAR == 16 
      //T2CKPS0 = 1;
      //T2CKPS1 = 1;
    #endif
      T2CONbits.TMR2ON = 1;
      TRISC1 = 0;
}

PWM2_stop()
{
    CCP2CONbits.CCP2M3 = 0;
    CCP2CONbits.CCP2M2 = 0;
}
//---End PWM function