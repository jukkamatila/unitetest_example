#include <stdio.h>
#include "modbus.h"
#include <avr/io.h>

#define TEST

/***************************************************/
/***************************************************/
void USART_init(unsigned int baud)
{
    UBBRH=0x51;

}
/*-------------------------------------------------*/
unsigned char USART_receive_byte(void)
{
    return OK;
}
/*-------------------------------------------------*/

unsigned char USART_transmit_byte(unsigned char byte)
{
    if(byte=='\r')
        fprintf(stdout,"\\r");
    else if(byte=='\n')
        fprintf(stdout,"\\n\n");
    else
        fprintf(stdout,"%c",byte);

    return OK;
}

/***************************************************/
/***************************************************/
unsigned char MB_receive_frame(unsigned char *mb)
{
    int i=0;
    unsigned char byte;

    while((byte=USART_receive_byte())!=':');
    *mb=byte;

    do
    {
        byte=USART_receive_byte();
        *(mb+i)=byte;
        i++;
    }
    while(byte!='\r');

    return i/2;

}

/***************************************************/
char ascii2dec(unsigned char c)
{
    char i=0;
    unsigned char ascii[]="0123456789ABCDEF";
    while(c!=ascii[i++])
		if(i>15) return -1;
    return i-1;
}

/***************************************************/
unsigned char MB_decode_frame(unsigned char *mb, unsigned char *mb_d)
{
    int i=1,j=0;

    while ((*(mb+i)!='\n') && (*(mb+i)!='\r'))
    {
        *(mb_d+j) = (ascii2dec(*(mb+i)))<<4 | ascii2dec(*(mb+i+1));

        i=i+2;
        j++;
    }
    return j-1;
}

/***************************************************/
unsigned char MB_count_LRC(unsigned char *mb_data, unsigned char mb_l)
{
    unsigned char lrc=0,i=0;

    for (i=0; i<mb_l; i++)
        lrc+=*(mb_data+i);

    lrc=~lrc+1;

    return lrc;
}

/***************************************************/
char dec2ascii(unsigned char d)
{
    unsigned char ascii[]="0123456789ABCDEF";
    if (d>15)
        return -1;
    else
        return ascii[d];
}

/***************************************************/
void MB_create_frame(unsigned char *mb_frame, unsigned char *mb_data, unsigned char mb_length)
{
    unsigned char i=0;

    for (i=0; i<mb_length*2; i++)
    {

        *(mb_frame+i*2+1) = dec2ascii((mb_data[i]>>4));
        *(mb_frame+i*2+2) = dec2ascii((mb_data[i]&0x0f));

    }
    i++;
    *(mb_frame)=':';
    *(mb_frame+i++)='\r';
    *(mb_frame+i++)='\n';
    *(mb_frame+i)='\0';

}

/***************************************************/
void MB_transmit_frame(unsigned char *mb_frame)
{
    int i=0;

    while (*(mb_frame+i)!='\0')
    {
        USART_transmit_byte(*(mb_frame+i));
        i++;
    }

}

/***************************************************/
/***************************************************/

