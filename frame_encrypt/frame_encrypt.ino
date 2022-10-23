#include <mcp_can_fd.h>
#include <mcp2518fd_can_dfs.h>
#include <mcp2518fd_can.h>
#include <SPI.h>
#include <avr/pgmspace.h>
#include "tiny-AES-c/aes.h"
#include "tiny-AES-c/aes.c"
#include<stdint.h>
const char  origin_dlc[] PROGMEM ={0,1,2,3,4,5,6,7,8,-1,-1,-1,9,-1,-1,-1,10,-1,-1,-1,11-1,-1,-1,12-1,-1,-1,-1,-1,-1,-1,13}; //n番目がn byteのときのDLCの値を示す．無効なbyte長は-1を返す
uint8_t encrypt_key[16];
const uint8_t  iv[16]PROGMEM = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };
void setup() {
  // put your setup code here, to run once:
  Serial.begin(112500);
  Serial.println(F("Serial OK!"));
}

void loop() {
  // put your main code here, to run repeatedly:

}
