#include <xc.h>
#include "other.h"


void main_init (void)
{
ANSELB = 0;
TRISB = 0;
U1STAbits.URXISEL = 0;
IFS0bits.U1RXIF = 0;
IEC0bits.U1RXIE = 1;
IPC2bits.U1RXIP = 3;
U1BRG = ((PFCY)/(16*BAUD)) - 1;
U1MODEbits.UARTEN = 1;
U1STAbits.UTXEN = 1;

SPI2CON1 = 0x033F;
SPI2STAT = 0x8000;

__builtin_write_OSCCONL(OSCCON & 0xbf); //unlock Pin Re-map
RPINR18bits.U1RXR = 41;		//rx
RPOR3bits.RP40R = 1;		//tx

RPOR4bits.RP42R = 8;	//sdo2
RPOR4bits.RP43R = 9;	//sck2
__builtin_write_OSCCONL(OSCCON | 0x40); //lock Pin Re-map
}

int get_avg (int * arr, int first, int last)
{
long temp;
unsigned int i;
temp = 0;
for (i=first;i<last;i++)
	{
	temp = temp + arr[i];
	}
temp = temp / (last - first);
return ((int) (temp));
}


void delay_100us(unsigned int number)
{
int i;
TMR4 = 0;
PR4 = PFCY/10000;
IFS1bits.T4IF = 0;
T4CONbits.TON = 1;
for (i=0;i<number;i++) 
	{
	IFS1bits.T4IF = 0;
	while (IFS1bits.T4IF == 0);
	}
T4CONbits.TON = 0;
}

void delay_ms(unsigned int number)
{
int i;
TMR4 = 0;
PR4 = PFCY/1000;
IFS1bits.T4IF = 0;
T4CONbits.TON = 1;
for (i=0;i<number;i++) 
	{
	IFS1bits.T4IF = 0;
	while (IFS1bits.T4IF == 0);
	}
T4CONbits.TON = 0;
}

void initAdc1(void)
{

AD1CON1bits.FORM = 0b00; // Data Output Format: integer
AD1CON1bits.SSRC = 7; // Auto conversion - first sample and after start conversion
AD1CON1bits.ASAM = 0; // start sample when SAMP is set
AD1CON1bits.AD12B = 0; // 10-bit ADC operation
AD1CON1bits.SIMSAM = 1; // Samples multiple channels simultaneously in sequence
AD1CON2bits.CHPS = 1; // Converts CH0,CH1 Channel
AD1CON2bits.BUFM = 0; // start filling buffer from the beginning
AD1CON2bits.CSCNA = 0; //
AD1CON2bits.VCFG=0; // VREF+ =Vdd VREF- =Vss
AD1CON2bits.ALTS=0; // solo un mux viene fatto
AD1CON3=0x9F00;
//AD1CHS0: A/D Input Select Register
AD1CHS0=0x0101;
//AD1CHS123: A/D Input Select Register
AD1CHS123=0;
AD1CON1bits.ADDMABM = 1; // buffer in order of conversion
AD1CON2bits.SMPI = 0; // increment the dma address or generate interrupt after
// completition of every sample/conversion operation
AD1CON4= 0; // allocates 1 word of bufferto each analog input
IFS0bits.AD1IF = 0; // Clear the A/D interrupt flag bit
IEC0bits.AD1IE = 0; // Do Not Enable A/D interrupt
AD1CON1bits.ADON = 1; // Turn on the A/D converter
} 



void initDma0(void)
{
}

void tx_write (char chr)
{
U1TXREG = chr;
while (!(U1STAbits.TRMT)) {}
}

void tx_str (char * data)
{
while (*data!=0x00)
	tx_write(*data++);
}

