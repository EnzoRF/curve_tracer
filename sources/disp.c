#include 	<xc.h>
#include	"disp.h"
#include	"other.h"


#define	write_data	LCD_DataWrite_IC
#define write_command	LCD_CtrlWrite_IC

int abs (int val);
unsigned char write_spi (unsigned char data);


void lcd_init (void)
{
RST=0;
delay_ms(20);
RST=1;
delay_ms(20);

write_command(0x11);//Sleep exit  
delay_ms(200);
//ST7735R Frame Rate
write_command(0xB1); 
write_data(0x01); write_data(0x2C); write_data(0x2D); 
write_command(0xB2); 
write_data(0x01); write_data(0x2C); write_data(0x2D); 
write_command(0xB3); 
write_data(0x01); write_data(0x2C); write_data(0x2D); 
write_data(0x01); write_data(0x2C); write_data(0x2D); 

write_command(0xB4); //Column inversion 
write_data(0x07); 
 
//ST7735R Power Sequence
write_command(0xC0); 
write_data(0xA2); write_data(0x02); write_data(0x84); 
write_command(0xC1); write_data(0xC5); 
write_command(0xC2); 
write_data(0x0A); write_data(0x00); 
write_command(0xC3); 
write_data(0x8A); write_data(0x2A); 
write_command(0xC4); 
write_data(0x8A); write_data(0xEE); 
 
write_command(0xC5); //VCOM 
write_data(0x0E); 
 
write_command(0x36); //MX, MY, RGB mode 
write_data(0xC8); 

//ST7735R Gamma Sequence
write_command(0xe0); 
write_data(0x0f); write_data(0x1a); 
write_data(0x0f); write_data(0x18); 
write_data(0x2f); write_data(0x28); 
write_data(0x20); write_data(0x22); 
write_data(0x1f); write_data(0x1b); 
write_data(0x23); write_data(0x37); write_data(0x00); 

write_data(0x07); 
write_data(0x02); write_data(0x10); 
write_command(0xe1); 
write_data(0x0f); write_data(0x1b); 
write_data(0x0f); write_data(0x17); 
write_data(0x33); write_data(0x2c); 
write_data(0x29); write_data(0x2e); 
write_data(0x30); write_data(0x30); 
write_data(0x39); write_data(0x3f); 
write_data(0x00); write_data(0x07); 
write_data(0x03); write_data(0x10);  

write_command(0x2a);
write_data(0x00);write_data(0x00);
write_data(0x00);write_data(0x7f);
write_command(0x2b);
write_data(0x00);write_data(0x00);
write_data(0x00);write_data(0x9f);

write_command(0xF0); //Enable test command  
write_data(0x01); 
write_command(0xF6); //Disable ram power save mode 
write_data(0x00); 
 
write_command(0x3A); //65k mode 
write_data(0x05); 
 

write_command(0x29);//Display on

}

void dsp_single_colour(unsigned char DH,unsigned char DL)
{
unsigned char  i,j;
//RamAdressSet();
write_command(0x2C);
for (i=0;i<160;i++)
	for (j=0;j<128;j++)
    	{
        LCD_DataWrite(DH,DL);
        }
}

/*
void disp_send(unsigned char cmd, unsigned char type)
	{
    char x;
    if (type!=0)
    	D_C = 1;
    else
       	D_C = 0;
    SCE = 0;
    for(x=0;x<8;x++){       //clock the byte out
        SCLK = 0;
        MOSI = 0;
        if(cmd & 0x80)
            MOSI = 1;
        SCLK = 1;
        cmd <<= 1;
    }
    SCE = 1;
}
*/
void disp_send(unsigned char cmd, unsigned char type)
{
char x;
if (type!=0)
  	D_C = 1;
else
   	D_C = 0;
SCE = 0;
write_spi(cmd);
SCE = 1;
}

void LCD_CtrlWrite_IC(unsigned char c)
{
disp_send(c,0);
}

void  LCD_DataWrite_IC(unsigned char c)
{
disp_send(c,1);
}

void LCD_DataWrite(unsigned char LCD_DataH,unsigned char LCD_DataL)
{
LCD_DataWrite_IC(LCD_DataH);
LCD_DataWrite_IC(LCD_DataL);
}

void PutPixel(int x,int y,int colour)
{
LCD_SetArea(x,y,x+1,y+1);
LCD_CtrlWrite_IC(0x2C);
LCD_DataWrite_IC(colour >> 8);
LCD_DataWrite_IC(colour & 0xFF);
}

void LCD_SetArea(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
LCD_CtrlWrite_IC(0x2A);
LCD_DataWrite_IC(x1 >> 8);
LCD_DataWrite_IC(x1 & 0xFF);
LCD_DataWrite_IC(x2 >> 8);
LCD_DataWrite_IC(x2 & 0xFF);
LCD_CtrlWrite_IC(0x2B);
LCD_DataWrite_IC(y1 >> 8);
LCD_DataWrite_IC(y1 & 0xFF);
LCD_DataWrite_IC(y2 >> 8);
LCD_DataWrite_IC(y2 & 0xFF);
}

void draw_line (unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color)
{
int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
int err = (dx>dy ? dx : -dy)/2, e2;
for(;;)
	{
	PutPixel(x0,y0,color);
	if (x0==x1 && y0==y1) break;
	e2 = err;
	if (e2 >-dx) { err -= dy; x0 += sx; }
	if (e2 < dy) { err += dx; y0 += sy; }
	}
}


int abs (int val)
{
if (val<0) return val*-1;
else return val;
}

unsigned char write_spi (unsigned char data)
{
unsigned char test;
IFS2bits.SPI2IF=0;
SPI2BUF = data;
while(IFS2bits.SPI2IF==0);
test = SPI2BUF;
return test;
}

