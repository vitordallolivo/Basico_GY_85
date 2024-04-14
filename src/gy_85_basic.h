#ifndef GY_85_BASIC
#define GY_85_BASIC


#include <Arduino.h>
#include <Wire.h>

//Acelerometro

struct ADX{
  int ADRESS = 0x53;
  byte Register_ID =0;
  byte Register_2D =0x2D;
  byte Register_X0 =0x32;
  byte Register_X1 =0x33;
  byte Register_Y0 =0x34;
  byte Register_Y1 =0x35;

  byte Register_Z0 =0x36;
  byte Register_Z1 =0x37;


  int X_out; // Byte de saida
  int Y_out; // Byte de saida
  int Z_out; // Byte de saida

  double Xg,Yg,Zg; // Acelerações em [g]


};

//  Bussola magnetica

struct HMC{
  int ADRESS = 0x1E;

  byte Conf_A = 0x00;
  byte Conf_B = 0x01;

  // CONFIGURAÇÕES DE A

  // Media de samples
  byte MA = 0x00;


  /*
    Select number of samples averaged (1 to 8) per
    measurement output.
    00 - 1 
    01 - 2
    10 - 4
    11 - 8


  */

  // Frequência de saida 
  byte DO = 0x04;


  /*
  The Table below shows all selectable output rates in continuous measurement mode. All three channels shall be
  measured within a given output rate. Other output rates with maximum rate of 160 Hz can be achieved by monitoring
  DRDY interrupt pin in single measurement mode.

  
  DO    hz
  000 - 0.75
  001 - 1.5
  010 - 3
  011 - 7.5
  100 - 15
  101 - 30
  110 - 75
  111 - Indisponivel
  
  */


  // Modos de medida
  byte MS = 0x00;

  /*
    00  - Normal measurement configuration (Default). In normal measurement
          configuration the device follows normal measurement flow. The positive and
          negative pins of the resistive load are left floating and high impedance. 
    01  - Positive bias configuration for X, Y, and Z axes. In this configuration, a positive
          current is forced across the resistive load for all three axes.
    10  - Negative bias configuration for X, Y and Z axes. In this configuration, a negative
        current is forced across the resistive load for all three axes..
    11  - indisponivel
  
  */

  byte A_CONFIG = MA<<5|DO<<2|MS;


  // CONFIGURAÇÃO B

  byte CRB;


  /*  CRB       RANGE(Ga) |  Ganho (LSB/Gayss) |    Digital Resolution (mg/LSB)
      0 0 0  -  0.88      |     1370           |            0.73
      0 0 1  -  1.3       |     1090           |            0.92
      0 1 0  -  1.9       |     820            |            1.22
      0 1 1  -  2.5       |     660            |            1.52
      1 0 0  -  4.0       |     440            |            2.27
      1 0 1  -  4.7       |     390            |            2.56
      1 1 0  -  5.6       |     330            |            3.03
      1 1 1  -  8.1       |     230            |            4.35
  
  */


  byte B_CONFIG = CRB <<5;

  byte MODE_REGISTER = 0x02;
  byte MODE_type =0x00;

  byte X_MSB = 0x03;
  byte X_LSB = 0x04;

  byte Z_MSB = 0x05;
  byte Z_LSB = 0x06;

  byte Y_MSB = 0x07;
  byte Y_LSB = 0x08;

  byte STATUS_REGISTER = 0x09;

  double X,Y,Z;


};

// Giroscopio
struct ITG{
  int ADRESS = 0x68;
   byte Register_WHO_IAM  = 0X00;

   byte SMPLRT_DIV_REGISTER= 0x15;
  byte SMPLRT_DIV = 0x00;

  /*
    F_sample = F_internal /(SMPLRT_DIV +1)


    SMPLRT_DIV vai de 0 a 255

  */

   byte DLPF_REGISTER = 0x16;
  byte DLPF_CFG = 0x00;
  byte DLPF = 0x18 | DLPF_CFG; // 0x18 é obrigatorio



  /*
  DLPFG_CFG - LOW PASS filter  - Internal Sample Rate
      0     -      256  hz     -        8khz
      1     -      188  hz     -        1khz
      2     -       98  hz     -        1khz
      3     -       42  hz     -        1khz
      4     -       20  hz     -        1khz
      5     -       10  hz     -        1khz
      6     -        5  hz     -        1khz
      7     -    indisponivel  -     indisponivel

  
  */

   byte TEMP_MSB_REGISTER =  0x1B;
   byte Gx_MSB_REGISTER =    0x1D;
   byte Gy_MSB_REGISTER =    0x1F;
   byte Gz_MSB_REGISTER =    0x21;

  double x,y,z,temp;

};

struct GY_85{
    ADX adx;
    ITG itg;
    HMC hmc;


    double acceleration[3]={0.0f,0.0f,0.0f};
    double gyro[3]={0.0f,0.0f,0.0f};
    double bussola[3]={0.0f,0.0f,0.0f};
};


struct offset{
  float gx,gy,gz;
  float ax,ay,az;
  float mx,my,mz;
};

void init_data(GY_85 gy85, offset OFF);
GY_85 read_data(GY_85 gy85);

#endif