#include "LPC11XX.H"

#define KEY1_DOWN() ((LPC_GPIO3->DATA & 0x01) != 0x01)
#define KEY2_DOWN() ((LPC_GPIO3->DATA & 0x02) != 0x02)
#define STEP_VALUE 500;
unsigned int cmpvalue = 2000;

void pwm_init()
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);
	LPC_IOCON->R_PIO1_2 &= ~(0x07);
	LPC_IOCON->R_PIO1_2 |= 0x03;
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);
	LPC_TMR32B1->TCR = 0x02;
	LPC_TMR32B1->PR  = 0;
	LPC_TMR32B1->MCR = 0x02 << 9;
	LPC_TMR32B1->PWMC = 0x02;
	LPC_TMR32B1->MR1 = SystemCoreClock / 2000;
	LPC_TMR32B1->MR3 = SystemCoreClock / 1000;
	LPC_TMR32B1->TCR = 0x01;
}

void pwm_set(unsigned int cmp)
{
	LPC_TMR32B1->MR1 = SystemCoreClock / cmp;
}

int main()
{
	pwm_init();
	
	  LPC_GPIO3->DIR = 0x0;
    LPC_GPIO3->IE |= 0x3; 
    LPC_GPIO3->IS &= 0x0; 
    LPC_GPIO3->IEV &= 0x0;
    NVIC_EnableIRQ(EINT3_IRQn);
	
	//pwm_set(4000);
	
	while(1);
}

void PIOINT3_IRQHandler()
{
	if((LPC_GPIO3->MIS & 0x1)==0x1)
	{
		while(KEY1_DOWN());
		cmpvalue+=STEP_VALUE;
		pwm_set(cmpvalue);
		LPC_GPIO3->IC = 0x1; //ÇåÖÐ¶Ï
	}
	
	if((LPC_GPIO3->MIS & 0x2)==0x2)
	{
		while(KEY2_DOWN());
		cmpvalue-=STEP_VALUE;
		pwm_set(cmpvalue);
		LPC_GPIO3->IC = 0x2; 
	}
}
