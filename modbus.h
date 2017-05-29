enum {ERROR,OK};


void USART_init(unsigned int baud);
unsigned char USART_receive_byte(void);
unsigned char USART_transmit_byte(unsigned char byte);
unsigned char MB_receive_frame(unsigned char *mb);
char ascii2dec(unsigned char c);
unsigned char MB_decode_frame(unsigned char *mb, unsigned char *mb_d);
unsigned char MB_count_LRC(unsigned char *mb_data, unsigned char mb_l);
char dec2ascii(unsigned char d);
void MB_create_frame(unsigned char *mb_frame, unsigned char *mb_data, unsigned char mb_length);
void MB_transmit_frame(unsigned char *mb_frame);

