#include "tiny-AES-c/aes.h"
#include "tiny-AES-c/aes.c"
#include<stdint.h>
const unsigned char encrypt_key[16]= { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
const unsigned char PROGMEM iv[16]= { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };
unsigned char plain[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
void AES_ctx(unsigned char * text,int length);
static void phex(uint8_t* str,int len);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("Serial OK!"));
  phex(plain,8);
  AES_ctr(plain,8);
  phex(plain,8);
  AES_ctr(plain,8);
  phex(plain,8);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void AES_ctr(unsigned char * text,int length)
{
  struct AES_ctx ctx;
  AES_init_ctx_iv(&ctx, encrypt_key, iv);
  AES_CTR_xcrypt_buffer(&ctx, text,length);
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
