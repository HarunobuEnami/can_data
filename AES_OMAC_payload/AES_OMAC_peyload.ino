  //Arduino no.1
#include <SPI.h>
#include "mcp2518fd_can.h"
#include "aes-omac1.h"
#include <time.h>
#define MAX_DATA_SIZE 64
#define p_size 8
#include <Crypto.h>
#include <AES.h>
#include <CTR.h>
#include <string.h>

#define MAX_PLAINTEXT_SIZE  36
#define MAX_CIPHERTEXT_SIZE 36
// pin for CAN-FD Shield  
const int SPI_CS_PIN = 10;
const int CAN_INT_PIN = 2;

//--------SIMON------

mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
byte data[9] = {0};
byte buf[16] = {0};
const unsigned char buf2[16]={0};
unsigned long int  Sq = {0x00000000};
unsigned long int  Sq_1={0},Sq_2={0};

uint16_t count=1 ;

unsigned long start_time, end_time, ave_time, sum_time;
unsigned long keyup_start_time, keyup_end_time;

byte msg[MAX_DATA_SIZE]={0};
byte id=0x00;

u8 key[AES_BLOCK_SIZE];
u8 key_sub1[AES_BLOCK_SIZE];
//u8 key_sub2[AES_BLOCK_SIZE];

u8 MAC[AES_BLOCK_SIZE];
unsigned char MAC_c1[AES_BLOCK_SIZE];
//u8 MAC_c2[AES_BLOCK_SIZE];

//u8 cbc[AES_BLOCK_SIZE], pad[AES_BLOCK_SIZE];
//u8 AES_En[AES_BLOCK_SIZE];
const unsigned int BIT_FLAG_8 =(1 << 8);
/*
struct TestVector
{
    const char *name;
    byte key[16];
    byte plaintext[MAX_PLAINTEXT_SIZE];
    byte ciphertext[MAX_CIPHERTEXT_SIZE];
    byte iv[16];
    size_t size;
};
static TestVector const testVectorAES128CTR1 = {
    .name        = "AES-128-CTR #1",
    .key         = {0xAE, 0x68, 0x52, 0xF8, 0x12, 0x10, 0x67, 0xCC,
                    0x4B, 0xF7, 0xA5, 0x76, 0x55, 0x77, 0xF3, 0x9E},
    .plaintext   = {0x53, 0x69, 0x6E, 0x67, 0x6C, 0x65, 0x20, 0x62,
                    0x6C, 0x6F, 0x63, 0x6B, 0x20, 0x6D, 0x73, 0x67},
    .ciphertext  = {0xE4, 0x09, 0x5D, 0x4F, 0xB7, 0xA7, 0xB3, 0x79,
                    0x2D, 0x61, 0x75, 0xA3, 0x26, 0x13, 0x11, 0xB8},
    .iv          = {0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
    .size        = 16
};
CTR<AES128> ctraes128;

void perfCipherEncrypt(const char *name, Cipher *cipher, const struct TestVector *test)
{
    unsigned long start;
    unsigned long elapsed;
    int count;

    Serial.print(name);
    Serial.print(" ... ");

    cipher->setKey(test->key, cipher->keySize());
    cipher->setIV(test->iv, cipher->ivSize());
    start = micros();
    for (count = 0; count < 500; ++count) {
        cipher->encrypt(MAC_c1, MAC_c1, sizeof(MAC_c1));
    }
    elapsed = micros() - start;

    Serial.print(elapsed / (sizeof(MAC_c1) * 500.0));
    Serial.print("us per byte, ");
    Serial.print((sizeof(MAC_c1) * 500.0 * 1000000.0) / elapsed);
    Serial.println(" bytes per second");
}
*/
void setup() {
    Serial.begin(115200);
    while (!Serial) {}
//perfCipherEncrypt("AES-128-CTR Encrypt", MAC_c1[16], &testVectorAES128CTR1);
Serial.print("AES");
for(int i=0;i<16;i++){
  Serial.print(MAC_c1[i]);
}
Serial.println("");
    CAN.setMode(CAN_NORMAL_MODE);

    // init can bus : arbitration bitrate = 500k, data bitrate = 1M
    while (0 != CAN.begin(CAN_500K_1M)) {
        Serial.println("CAN init fail, retry...");
        delay(100);
    }
    Serial.println("CAN init ok!");

    byte mode = CAN.getMode();
    Serial.print("CAN mode = ");
    Serial.println(mode);


    
    for(int i=0; i<8; i++)
    {
        data[i] = i;
    }
     init_Rnd_cdm(0x0af0);
  for(int i=0;i<AES_BLOCK_SIZE;i++){
    key[i]=Rnd_byte(); }
    //buf[0] = id;
  
  delay(1000);
}



void loop() 
{
  
  delay(100);
    // send data:  id = 0x00, standrad frame, data len = 32byte, buf: data buf
  Serial.write(1);
    /* preparation*/
 // aes_encrypt(void *ctx const u8 plane,const u8 crypt);
  
 
  /*
  uint16_t x[42];
  x[1]=data[1];
  x[0]=data[0];
  uint16_t k[33];
  k[0]=key[0];
  */

  
  Serial.println("----------------------------------------");
    /*
    Serial.print("planetext=");
    for(int i=0;i<8;i++){
      Serial.print(data[i],HEX);
     }
    Serial.print("\n");
    Serial.print("ciphertext=");
    Serial.print(x[33],HEX);
    Serial.println(x[32],HEX);
    */
  Sq   = count;
  //Sq_s   = Sq & (0x0011);
 /*
  Serial.print("Sequence number:");
  
  Serial.print(Sq,BIN);
  Serial.println("");
  /*
  Serial.print("Sq_s            :");  
  Serial.println(Sq_s,BIN);
  */
  Serial.print("Get Data From id: ");
  Serial.println(id,HEX);
  Serial.print("Len =         ");
  Serial.println(CANFD::len2dlc(16),HEX);

  
  for(int i=0;i <8;i++){
    buf[i] = data[i];
  }
  buf[8] = Sq>>8;
  buf[9] = Sq;
  Serial.print("K              ");
    for(int i=0;i<AES_BLOCK_SIZE;i++){
      Serial.print(key[i],HEX);
      Serial.print("|");
    if((i%4) == 3)
      Serial.print(" ");
  }
  Serial.println("");

  
  Serial.print("message.data   ");
    for(int i=0;i<p_size;i++){
      Serial.print(buf[i],HEX);
      Serial.print("|");
      if((i%4) == 3)
          Serial.print(" ");
    }
  Serial.println(" ");


//-------------------暗号化処理-----------------
  
  //keyup_start_time=millis(); 
  
  aes_encrypt(key, buf, data);
  
  //keyup_end_time=millis();
  //ave_time=(keyup_end_time)-(keyup_start_time);
  //Serial.print(ave_time);
  //Serial.println("ms");

//-------確認--------
  Serial.print("enc   ");
    for(int i=0;i<p_size;i++){
      Serial.print(data[i],HEX);
      Serial.print("|");
      if((i%4) == 3)
          Serial.print(" ");
    }
  Serial.println(" ");
------------------------------------------------  

  
   
  //omac1_aes_vector(key,16,1,data[1],8,MAC_c1);
  keyup_start_time=micros(); 
  /*
  void *ctx;
  ctx=aes_encrypt_init(key,16);
  aes_encrypt(ctx, msg, MAC_c1);
  aes_encrypt_deinit(ctx);
  keyup_end_time=micros();
  ave_time=(keyup_end_time)-(keyup_start_time);
  Serial.print(ave_time);
  Serial.println("us");
  
  Serial.print("ctx:");
  for(int i=0;i<8;i++){
  Serial.print(MAC_c1[i],HEX);  
  Serial.print(",");
  }
  Serial.println("");
 */
   /*
  omac1_aes_128(key,data,8,MAC_c1);
  Serial.print("ctx:");
  for(int i=0;i<8;i++){
  Serial.print(MAC_c1[i],HEX);  
  Serial.print(",");
  }
  Serial.println("");
  */
    //MAC
  //omac1_aes_128(key,buf,8,MAC);
    
   start_time=micros(); 
   omac1_aes_128(key,buf,8,MAC);
   //omac1_aes_128(key,MAC_c1,8,MAC);
   end_time=micros();
   ave_time=end_time-start_time;
   Serial.print("Time:");
   Serial.print(ave_time);
   Serial.println("us");
 /*
  Serial.print("MAC:");
  for(int i=0;i<8;i++){
  Serial.print(MAC[i],HEX);  
  Serial.print(",");
  }
  Serial.println("");
 */

 /*
  Serial.println("k=");
  for(int i=0;i<AES_BLOCK_SIZE;i++){
  Serial.print(k[i],HEX);
  Serial.print("|");
   if((i%4) == 3)
   Serial.print(" ");
  }
  Serial.println("");
  */
  Serial.print("MAC          ");
  for(int i=0;i<p_size;i++){
    Serial.print(MAC[i],HEX);
    Serial.print("|");
    if((i%4) == 3)
      Serial.print(" ");
    }
  Serial.println("");

    for(int i=0;i<8;i++){
      //msg[i]=MAC_c1[i];
      msg[i]=data[i];
    }
    msg[8]=buf[8];
    msg[9]=buf[9];
    
    for(int i=10;i<18;i++){
      msg[i]=MAC[i-(p_size+2)];
    }
    
for(int i=0;i<18;i++){
      Serial.print(msg[i],HEX);
      Serial.print("|");
      if((i%4) == 3)
        Serial.print(" ");
    }

  //Send message
  CAN.sendMsgBuf(id, 0, CANFD::len2dlc(64), msg);
  Serial.println("");

  /*↓---------共通鍵を更新(送信側と共通)----------------↓*/
  
  
  
 if(buf[9] == 0){
  Serial.println("---------------------------------");
  Serial.print("Common key change");
  Serial.println();
  keyup_start_time=micros(); 
  for(int i=0;i<AES_BLOCK_SIZE;i++){
      key[i] = key[i] ^  Rnd_byte();
    }
  keyup_end_time=micros();
  //ave_time=(keyup_end_time)-(keyup_start_time);
  Serial.print("new key         ");
  for(int i=0;i<AES_BLOCK_SIZE;i++){
    Serial.print(key[i],HEX);
    Serial.print("|"); 
    if((i%4) == 3)
     Serial.print(" ");
  }
  Serial.println("");
  /*
  Serial.print("keychangetime:");
  Serial.print(ave_time);
  Serial.println("us");
  */
  Serial.println("---------------------------------");

 }
 

/*↑----------共通鍵を更新(送信側と共通)---------------↑*/
  Serial.println("--------------------------------------------------");
  
  count++;
  delay(500);
  
}

// END FILE