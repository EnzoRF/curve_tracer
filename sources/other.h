void delay_ms(unsigned int number);
void delay_100us(unsigned int number);
void tx_write (char chr);
void tx_str (char * data);
int get_avg (int * arr, int first, int last);
void main_init (void);


#define	CTRL_CHARGE		LATAbits.LATA2			//A
#define	CTRL_CHARGE_T	TRISAbits.TRISA2
#define	CTRL_DIS		LATAbits.LATA3			//B
#define	CTRL_DIS_T		TRISAbits.TRISA3

#define SCE     LATBbits.LATB12
#define RST     LATBbits.LATB14
#define D_C     LATBbits.LATB13
#define MOSI    LATBbits.LATB10
#define SCLK    LATBbits.LATB11

#define	FCY		7372800
#define	PFCY	FCY/2

#define  MAX_CHNUM	 			1		// Highest Analog input number in Channel Scan
#define  SAMP_BUFF_SIZE	 		4		// Size of the input buffer per analog input
#define  NUM_CHS2SCAN			2		// Number of channels enabled for channel scan

#define	SAMPLE_LEN		600
#define	R1				560
#define	R2				39
#define	DIV				(R2+R1)/R2
#define	UREF			3300
#define	ADC_LEVELS		1024

#define	BAUD	38400


