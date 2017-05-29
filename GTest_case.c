#include <gtest/gtest.h>
#include "modbus.h"

/**************************************************************/
TEST(Modbus_tests,ascii2dec)
{
    EXPECT_EQ(0,ascii2dec('0'));
    EXPECT_EQ(1,ascii2dec('1'));
    EXPECT_EQ(2,ascii2dec('2'));
    EXPECT_EQ(3,ascii2dec('3'));
    EXPECT_EQ(4,ascii2dec('4'));
    EXPECT_EQ(5,ascii2dec('5'));
    EXPECT_EQ(6,ascii2dec('6'));
    EXPECT_EQ(7,ascii2dec('7'));
    EXPECT_EQ(9,ascii2dec('9'));
    EXPECT_EQ(2,ascii2dec('2'));
    EXPECT_EQ(-1,ascii2dec('g'));
    EXPECT_EQ(-1,ascii2dec('\0'));

}

TEST(Modbus_tests,dec2ascii)
{
    EXPECT_EQ('0',dec2ascii(0));
    EXPECT_EQ('1',dec2ascii(1));
    EXPECT_EQ('2',dec2ascii(2));
    EXPECT_EQ('A',dec2ascii(10));
    EXPECT_EQ('B',dec2ascii(11));
    EXPECT_EQ('C',dec2ascii(12));
    EXPECT_EQ(-1,dec2ascii(16));
    EXPECT_EQ(-1,dec2ascii(-1));
}

/**************************************************************/

TEST(Modbus_tests,LRC_Positive1)
{
    unsigned char mb_data[128];
    unsigned char lrc;
    unsigned int dummy=0x4321;

    mb_data[0]=1;  //addr
    mb_data[1]=4;  //func
    mb_data[2]=0; // put H length as KKP
    mb_data[3]=1; // put L length as KKP
    mb_data[4]=(dummy >> 8);  // put fake return data in
    mb_data[5]=(dummy & 0x00ff);  // put fake return data in

    lrc = MB_count_LRC(mb_data,5);

    EXPECT_EQ(0xB7,lrc);
}
/**************************************************************/

TEST(Modbus_tests,createframe)
{
    unsigned char mb_frame[255];
    unsigned char mb_data[128];
    unsigned char mb_length;

    mb_data[0]=1;  //addr
    mb_data[1]=4;  //func
    mb_data[2]=0; // put H length as KKP
    mb_data[3]=1; // put L length as KKP
    mb_data[4]=(0x4321 >> 8);  // put fake return data in
    mb_data[5]=(0x4321 & 0x00ff);  // put fake return data in
    mb_data[6] = MB_count_LRC(mb_data,5);
    mb_length=7;


    MB_create_frame(mb_frame,mb_data,mb_length);

    EXPECT_STREQ((char *)&mb_frame[0],":010400014321B7\r\n");
}
