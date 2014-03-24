#include <xc.h>
#include <stdio.h>
#include "disp.h"
#include "other.h"

char tx_buffer[50];
unsigned long r_now, r_last, a, b;

void initAdc1(void);
unsigned int i;
unsigned int b1[SAMPLE_LEN],b2[SAMPLE_LEN];
int arr_u[SAMPLE_LEN], arr_i[SAMPLE_LEN];
unsigned int resistance, period;
long calc_temp;
int x0,y0,x1,y1;


int main (void)
{
main_init();
initAdc1();
lcd_init();
dsp_single_colour(0x00,0x00);

resistance = 100;
period = 30;
CTRL_CHARGE=0;
CTRL_DIS=0;
CTRL_CHARGE_T=0;
CTRL_DIS_T=0;

CTRL_CHARGE=1;
delay_ms(200);
CTRL_CHARGE=0;
delay_ms(10);
CTRL_DIS=1;
for (i=0;i<SAMPLE_LEN;i++)
	{
	AD1CON1bits.SAMP=1; //START SAMPLING
	while (IFS0bits.AD1IF==0);
	b1[i] = ADC1BUF1;
	b2[i] = ADC1BUF0;
	delay_100us(period);
	}
CTRL_DIS=0;
for (i=0;i<SAMPLE_LEN;i++)
	{
	calc_temp = b2[i];
	calc_temp = ((calc_temp*UREF)/(ADC_LEVELS-1))*DIV;
	calc_temp = calc_temp / 10;
	arr_u[i] = calc_temp;
	
	if (b1[i]>b2[i]) calc_temp = b1[i] - b2[i];
	else	calc_temp = 0;
	calc_temp = ((calc_temp*UREF*1000)/(ADC_LEVELS-1))*DIV;
	calc_temp = calc_temp / resistance;
	calc_temp = calc_temp / 100;
	arr_i[i] = calc_temp;
	}
for (i=3;i<SAMPLE_LEN-3;i++)
	{
	arr_u[i] = get_avg(arr_u,i-3,i+3);	
	arr_i[i] = get_avg(arr_i,i-3,i+3);	
	}
for (i=0;i<SAMPLE_LEN;i++)
	{
	sprintf(tx_buffer,"%d\t%d\t%d\r\n",i,arr_u[i],arr_i[i]);
	tx_str(tx_buffer);
	}
for (i=0;i<159;i=i+25) draw_line(0,i,127,i,GREEN);
for (i=0;i<127;i=i+25) draw_line(i,0,i,160,BLUE);
draw_line(0,0,0,159,WHITE);
draw_line(0,0,127,0,WHITE);

for (i=1;i<SAMPLE_LEN-5;i=i+1)
	{
	x0=arr_i[i-1]/20;
	y0=arr_u[i-1]/20;
	x1=arr_i[i]/20;
	y1=arr_u[i]/20;
	draw_line(x0,y0,x1,y1,RED);
	}

CTRL_DIS=0;

while (1);


}


void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
 IFS0bits.DMA0IF = 0;		//Clear the DMA0 Interrupt Flag
}

void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void)
{
unsigned char rcvd_char;
IFS0bits.U1RXIF = 0;
rcvd_char = U1RXREG;
}





