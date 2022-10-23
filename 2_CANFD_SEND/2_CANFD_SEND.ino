#include <mcp2518fd_can.h>
#include <mcp2518fd_can_dfs.h>
#include <mcp_can_fd.h>

/*  MCP2517/8 send a can fd frame

    CAN FD Shield - https://www.longan-labs.cc/1030012.html
    CANBed FD - https://www.longan-labs.cc/1030009.html
    
    can-fd baud rate:

    CAN_125K_500K
    CAN_250K_500K
    CAN_250K_750K
    CAN_250K_1M
    CAN_250K_1M5
    CAN_250K_2M
    CAN_250K_3M
    CAN_250K_4M
    CAN_500K_1M
    CAN_500K_2M
    CAN_500K_3M
    CAN_500K_4M
    CAN_1000K_4M
*/

#include <SPI.h>
#include <stdint.h>
#define MAX_DATA_SIZE 64

// pin for CAN-FD Shield
const int SPI_CS_PIN = 10;
const int CAN_INT_PIN = 2;

// pin for CANBed FD
//const int SPI_CS_PIN = 17;
//const int CAN_INT_PIN = 7;

mcp2518fd CAN(SPI_CS_PIN); // Set CS pin

unsigned char stmp[MAX_DATA_SIZE] = {0};

void keyschedule(uint64_t k[33]);
void encryption(uint64_t x[33],uint64_t y[33],uint64_t k[33]);
uint64_t plaintext[2] = {0x123456789abcdef1,0xfedcba987654321f};   //平文
uint64_t key[4] = {0x12345678,0x9abcdef1,0x11335577,0x22446688};  //  鍵
int alpha=8,beta=3;

void keyschedule(uint64_t k[33])
{
    uint64_t l[35];
    l[0]=key[2];
    k[0]=key[3]; 
    l[1]=key[1];
    l[2]=key[0];
  
    int i;
    for(int i=0; i < 32; i++)     //32rounds
    {
        l[i+3] = (((l[i] >> alpha) | (l[i] << (32-alpha))) + k[i]) ^ i;
        k[i+1] = ((k[i] << beta) | (k[i] >> (32-beta))) ^ l[i+3];
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

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
    
    for(int i=0; i<MAX_DATA_SIZE; i++)
    {
        stmp[i] = i;
    }
}

void encryption(uint64_t x[33],uint64_t y[33],uint64_t k[33])
{
    x[0]=plaintext[0];
    y[0]=plaintext[1];
    k[0]=key[3];
    
    keyschedule(k);

    int i;
    for(int i=0; i < 32; i++)     //32rounds
    {
        x[i+1] = (((x[i] >> alpha) | (x[i] << (32-alpha))) + y[i]) ^ k[i];
        y[i+1] = ((y[i] << beta) | (y[i] >> (32-beta))) ^ x[i+1];
    }
}

void loop() 
{   
    uint64_t x[33];     
    uint64_t y[33];     
    uint64_t k[33];     
    uint8_t *p;
    uint8_t *t;
    uint8_t text_x[8];
    uint8_t text_y[8];
    int i,j;

    encryption(x,y,k);
    p = (uint8_t *) (x+32);
    t = (uint8_t *) (y+32);

    for(i=0; i<8; i++)
    {
        text_x[i]= *(p+i);
    }

    for(j=0; j<8; j++)
    {
        text_y[j]= *(t+j);
    }

    for(i=0; i<8; i++)
    {
        stmp[i] = text_x[i];
    }
    
    for(i=8; i<16; i++)
    {
        stmp[i] = text_y[i];
    }


    // send data:  id = 0x00, standrad frame, data len = 64, stmp: data buf
    CAN.sendMsgBuf(0x01, 0, CANFD::len2dlc(MAX_DATA_SIZE), stmp);
    delay(10);
    CAN.sendMsgBuf(0x04, 0, CANFD::len2dlc(MAX_DATA_SIZE), stmp);
    delay(500);                       // send data per 100ms
    Serial.println("CAN BUS sendMsgBuf ok!");
}

// END FILE
