#include <mcp_can_fd.h>
#include <mcp2518fd_can_dfs.h>
#include <mcp2518fd_can.h>
#include <avr/pgmspace.h>
#include "aes-omac1.h"
#include "./tiny-AES-c/aes.h"
#include "./tiny-AES-c/aes.c"
#include <SPI.h>
#define MAX_DATA_SIZE 64
#define MESSAGE_SIZE 4
#define MAC_SIZE 8
/*  MCP2517/8 send a can fd frame

    CAN FD Shield - https://www.longan-labs.cc/1030012.html
    CANBed FD - https://www.longan-labs.cc/1030009.html
    
    can-fd baud rate:

    CAN_125K_500K
    CAN_250K_500K
    CAN_250K_750K
    CAN_250K_1M
    CAN_250K_1M
    CAN_250K_2M
    CAN_250K_3M
    CAN_250K_4M
    CAN_500K_1M
    CAN_500K_2M
    CAN_500K_3M
    CAN_500K_4M
    CAN_1000K_4M
*/
#define SPI_CS_PIN 10
 int CAN_INT_PIN = 2;
mcp2518fd CAN(SPI_CS_PIN); 
const char  origin_dlc[] PROGMEM ={0,1,2,3,4,5,6,7,8,-1,-1,-1,9,-1,-1,-1,10,-1,-1,-1,11-1,-1,-1,12-1,-1,-1,-1,-1,-1,-1,13}; //n番目がn byteのときのDLCの値を示す．無効なbyte長は-1を返す
const uint8_t   encrypt_key[16]PROGMEM = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
const uint8_t  iv[16]PROGMEM = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };
uint32_t seq;
struct AES_ctx ctx;
void macgen(unsigned char *key,char * plain,int length,unsigned char *MAC);
int makeframe(unsigned char *frame,unsigned char * plain,int plain_byte);
static void phex(uint8_t* str,int len);
void setup() {
 Serial.begin(115200);
 while (!Serial) {}
  CAN.setMode(CAN_NORMAL_MODE);
  while(!Serial.available())
  
 while (0 != CAN.begin(CAN_500K_1M));
   init_Rnd_cdm(0x01d0);
    Serial.println(F("CAN init ok!"));
    byte mode = CAN.getMode();
    Serial.print(F("CAN mode = "));
    Serial.println(mode);
    AES_init_ctx_iv(&ctx, encrypt_key, iv);
    seq = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char frame[MAX_DATA_SIZE];
  unsigned char plain[MESSAGE_SIZE] = {0x00,0x01,0x02,0x03};

  int frame_length;

  
  frame_length = makeframe(frame,plain,MESSAGE_SIZE);
 // AES_CTR_xcrypt_buffer(&ctx,frame,frame_length -4);
  CAN.sendMsgBuf(0x01, 0, CANFD::len2dlc(frame_length), frame);
  Serial.println(seq);
  delay(500);
  ++seq;
}


void macgen(unsigned char *key,unsigned char * plain,int len,unsigned char *MAC)
{
   if(seq==0)
  {
    for(int i=0;i<AES_BLOCK_SIZE;i++){
		key[i]=Rnd_byte();
   
    }
     Serial.println("key init");
	}
 
   if(((seq&0x000000ff)==0xff)&&seq!=0){
     Serial.println("key update");
    for(int i=0;i<AES_BLOCK_SIZE;i++){
		key[i]=Rnd_byte()^key[i];
    }
  }
   omac1_aes_128(key,plain,len,MAC);
}

int makeframe(unsigned char *frame,unsigned char * plain,int plain_byte) //フレームを作成し，その合計長をバイト単位で返す
{
  static unsigned char key[AES_BLOCK_SIZE];
  unsigned char MAC[AES_BLOCK_SIZE];

  int i;
 memcpy(frame,plain,plain_byte);
 macgen(key,plain,plain_byte,MAC);
   
for(i=plain_byte;i<plain_byte+8;++i)
{
  frame[i] = MAC[i];
}
	frame[(plain_byte+8)+0] = (uint8_t )0x000000ff&seq;
	frame[(plain_byte+8)+1] =  (uint8_t )((0x0000ff00&seq)>>8) ;
	frame[(plain_byte+8)+2] =  (uint8_t )((0x00ff0000&seq)>>16) ;
	frame[(plain_byte+8)+3] = pgm_read_byte(origin_dlc+plain_byte);
 
  return plain_byte+8+4;
}
static void phex(uint8_t* str,int len)
{
    unsigned char i;
    char buff[8];
    for (i = 0; i < len; ++i)
        {sprintf(buff,"%.2x ", str[i]);
        Serial.print(buff);}
    Serial.print("\n");
}
