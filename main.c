#include <stdio.h>
#include "modbus.h"

int main(void)
{
    unsigned char mb_frame[255]=":010402020001F7\r\n";
    unsigned char mb_data[128];
    unsigned char mb_length;
    unsigned char mb_count_lrc;

    unsigned char MB_device_id=1;
    unsigned int MB_fake_data=0x4321;

    USART_init(9600);

#ifdef TEST
    MB_receive_frame(mb_frame);
#else
    MB_transmit_frame(mb_frame);
#endif

    mb_length=MB_decode_frame(mb_frame,mb_data);
    mb_count_lrc=MB_count_LRC(mb_data,mb_length-1);

    if((mb_count_lrc-mb_data[6])!=0) return ERROR; //KKP
    if(mb_data[0]!=MB_device_id) return ERROR; //KKP

// Create new frame
    mb_data[0]=1;  //addr
    mb_data[1]=4;  //func
    mb_data[2]=0; // put H length as KKP
    mb_data[3]=1; // put L length as KKP
    mb_data[4]=(MB_fake_data >> 8);  // put fake return data in
    mb_data[5]=(MB_fake_data & 0x00ff);  // put fake return data in
    mb_data[6] = MB_count_LRC(mb_data,5);
    mb_length=7;

    MB_create_frame(mb_frame,mb_data,mb_length);

    MB_transmit_frame(mb_frame);

    fprintf(stderr,"Done\n");

    return 0;
}

