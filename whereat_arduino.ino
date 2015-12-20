#include <SPI.h>
#include <avr/pgmspace.h>

// Registers
#define REG_Product_ID                           0x00
#define REG_Revision_ID                          0x01
#define REG_Motion                               0x02
#define REG_Delta_X_L                            0x03
#define REG_Delta_X_H                            0x04
#define REG_Delta_Y_L                            0x05
#define REG_Delta_Y_H                            0x06
#define REG_SQUAL                                0x07
#define REG_Pixel_Sum                            0x08
#define REG_Maximum_Pixel                        0x09
#define REG_Minimum_Pixel                        0x0a
#define REG_Shutter_Lower                        0x0b
#define REG_Shutter_Upper                        0x0c
#define REG_Frame_Period_Lower                   0x0d
#define REG_Frame_Period_Upper                   0x0e
#define REG_Configuration_I                      0x0f
#define REG_Configuration_II                     0x10
#define REG_Frame_Capture                        0x12
#define REG_SROM_Enable                          0x13
#define REG_Run_Downshift                        0x14
#define REG_Rest1_Rate                           0x15
#define REG_Rest1_Downshift                      0x16
#define REG_Rest2_Rate                           0x17
#define REG_Rest2_Downshift                      0x18
#define REG_Rest3_Rate                           0x19
#define REG_Frame_Period_Max_Bound_Lower         0x1a
#define REG_Frame_Period_Max_Bound_Upper         0x1b
#define REG_Frame_Period_Min_Bound_Lower         0x1c
#define REG_Frame_Period_Min_Bound_Upper         0x1d
#define REG_Shutter_Max_Bound_Lower              0x1e
#define REG_Shutter_Max_Bound_Upper              0x1f
#define REG_LASER_CTRL0                          0x20
#define REG_Observation                          0x24
#define REG_Data_Out_Lower                       0x25
#define REG_Data_Out_Upper                       0x26
#define REG_SROM_ID                              0x2a
#define REG_Lift_Detection_Thr                   0x2e
#define REG_Configuration_V                      0x2f
#define REG_Configuration_IV                     0x39
#define REG_Power_Up_Reset                       0x3a
#define REG_Shutdown                             0x3b
#define REG_Inverse_Product_ID                   0x3f
#define REG_Motion_Burst                         0x50
#define REG_SROM_Load_Burst                      0x62
#define REG_Pixel_Burst                          0x64


#define PIXEL_COUNT 900

byte initComplete=0;
byte testctr=0;
unsigned long currTime;
unsigned long timer;
unsigned long pollTimer;
const int ncs = 10;
volatile byte pixels[PIXEL_COUNT];


void setup() {
    Serial.begin(115200);

    pinMode (ncs, OUTPUT);


    SPI.begin();
    SPI.setDataMode(SPI_MODE3);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(8);

    adns_com_end(); // ensure that the serial port is reset
    adns_com_begin(); // ensure that the serial port is reset
    adns_com_end(); // ensure that the serial port is reset

    initializeFrameCapture();
    Serial.println("Whereat?");
    dispRegisters();
    delay(5000);
    initComplete=9;

}

void adns_com_begin(){
    digitalWrite(ncs, LOW);
}

void adns_com_end(){
    digitalWrite(ncs, HIGH);
}

byte adns_read_reg(byte reg_addr){
    adns_com_begin();

    // send adress of the register, with MSBit = 0 to indicate it's a read
    SPI.transfer(reg_addr & 0x7f );
    delayMicroseconds(100); // tSRAD
    // read data
    byte data = SPI.transfer(0);

    delayMicroseconds(1); // tSCLK-NCS for read operation is 120ns
    adns_com_end();
    delayMicroseconds(19); //  tSRW/tSRR (=20us) minus tSCLK-NCS

    return data;
}

void adns_write_reg(byte reg_addr, byte data){
    adns_com_begin();

    //send adress of the register, with MSBit = 1 to indicate it's a write
    SPI.transfer(reg_addr | 0x80 );
    //sent data
    SPI.transfer(data);

    delayMicroseconds(20); // tSCLK-NCS for write operation
    adns_com_end();
    delayMicroseconds(100); // tSWW/tSWR (=120us) minus tSCLK-NCS.
    // Could be shortened, but is looks like a safe lower bound 
}

void initializeFrameCapture(void){
    adns_write_reg(REG_Power_Up_Reset, 0x5a); // force reset
    delay(50); // wait for it to reboot

    //enable laser(bit 0 = 0b), in normal mode (bits 3,2,1 = 000b)
    // reading the actual value of the register is important because the real
    // default value is different from what is said in the datasheet, and if you
    // change the reserved bytes (like by writing 0x00...) it would not work.
    byte laser_ctrl0 = adns_read_reg(REG_LASER_CTRL0);
    adns_write_reg(REG_LASER_CTRL0, laser_ctrl0 & 0xf0 );
}

void startFrameCapture(void){
    adns_write_reg(REG_Frame_Capture, 0x93);
    adns_write_reg(REG_Frame_Capture, 0xc5);

    /*delay(1); // "Wait for two frames."*/
}

bool isFrameReady(void){
    byte motion = adns_read_reg(REG_Motion);
    /*bool isReady = motion & 0x01;*/
    /*return isReady;*/
    return motion;
}

void dispRegisters(void){
    int oreg[7] = {
        0x00,0x3F,0x2A,0x02  };
    char* oregname[] = {
        "Product_ID","Inverse_Product_ID","SROM_Version","Motion"  };
    byte regres;

    digitalWrite(ncs,LOW);

    int rctr=0;
    for(rctr=0; rctr<4; rctr++){
        SPI.transfer(oreg[rctr]);
        delay(1);
        Serial.println("---");
        Serial.println(oregname[rctr]);
        Serial.println(oreg[rctr],HEX);
        regres = SPI.transfer(0);
        Serial.println(regres,BIN);  
        Serial.println(regres,HEX);  
        delay(1);
    }
    digitalWrite(ncs,HIGH);
}


int convTwosComp(int b){
    //Convert from 2's complement
    if(b & 0x80){
        b = -1 * ((b ^ 0xff) + 1);
    }
    return b;
}

void loop() {
    int i;
    byte frameReady;

    Serial.println("Starting loop()...");

    startFrameCapture();

    frameReady = isFrameReady();

    Serial.print("Is frame ready? ");
    Serial.println(frameReady, HEX);

    Serial.println("Downloading pixels...");

    adns_com_begin();
    SPI.transfer(REG_Pixel_Burst);
    for(i=0; i<PIXEL_COUNT; i++){
        pixels[i] = SPI.transfer(0);
    }
    adns_com_end();

    Serial.println("...done!");

    Serial.println("Pixels:");
    Serial.println("((");
    for(i=0; i<PIXEL_COUNT; i++){
        Serial.print(pixels[i]);
        Serial.print(",");
        if((i + 1) % 30 == 0){
            Serial.println("),(");
        }

    }
    Serial.println("))");
    Serial.println("END");


    delay(1000);

}

