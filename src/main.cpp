#include <gy_85_basic.h>

#define MADGWICK_AHRS
 

#ifdef MADGWICK_AHRS
  #include <AHRS_gy85.h>
  #include <TimerOne.h>
#endif

// #define SD_CARD_WRITE

#ifdef SD_CARD_WRITE
  #include <SD.h>
  #define PIN_SD_CARD 10
  File arquivo;
#endif


// Define se vai ter serial, se comentar não tem output

// #define SERIAL_WRITE_A

GY_85 gy; // Struct do sensor especifico
offset OFF;  // Struct dos offsets
Global_acceleration Accel; // Aceleração e gravidade de acordo com NED ( north, east and down )

/* 

CASO TENHA QUE CONFIGURAR UM FILTRO DIFERENTE OU ALGUMA COISA ASSIM

Por exemplo mudar o filtro passa baixa do ITG para 188hz ( faixa de corte)

gy.itg.DLPF_CFG = 0x01;

*/



#ifdef MADGWICK_AHRS

  PassFilter PASS_FILTER; // Struct dos filtros usados

  byte flag_uptade_AHRS=0;

  float pitch1=0,yaw1=0,roll1=0;

  float acceleration[3]={0,0,1};
  float gyro[3]={0,0,0};
  float magnometer[3]={0,0,0};


  double time;

  void _UPTADE_AHRS(){
    flag_uptade_AHRS = 1;
  }

#endif

void setup(){

  PASS_FILTER.RC =0.65f; // Tem que testar um valor que fique bom


  OFF.gx = 0.00f;
  OFF.gy = 0.00f;
  OFF.gz = 0.00f;
  OFF.ax = 0.00f;
  OFF.ay = 0.00f;
  OFF.az = 0.00f;
  OFF.mx = 0.00f;
  OFF.my = 0.00f;
  OFF.mz = 0.00f;

  #ifdef SERIAL_WRITE_A
    Serial.begin(115200);
  #endif

  gy.itg.SMPLRT_DIV = 0x00; // Frequencia dos samples F_sample = (1khz) /(SMPLT_DIV +1)
  init_data(gy,OFF);

  #ifdef SD_CARD_WRITE
    if(!SD.begin(PIN_SD_CARD)){
        #ifdef SERIAL_WRITE_A
          Serial.println("SD CARD NAO ENCONTRADO");
        #endif
        asm volatile("jmp 0");
    }

    arquivo = SD.open("example.txt",FILE_WRITE);

    arquivo.println("aN;aL;aB;mx;my;mz;r;p;y;");

  #endif


  
  #ifdef MADGWICK_AHRS

    begin(100); // 100 hz
    Timer1.initialize(100000); // 100 hz timer
    Timer1.attachInterrupt(_UPTADE_AHRS);
  #endif

}


void loop(){
  gy=read_data(gy);
  
  #ifdef MADGWICK_AHRS  
    if(flag_uptade_AHRS){

      roll1  =  getRoll();
      pitch1 =  getPitch();
      yaw1   =  getYaw();
      
    for(int j = 0 ;j <3;j++){
        acceleration[j] = MedianFilter(gy.acceleration[j],&PASS_FILTER);
        gyro[j] = gy.gyro[j];
        magnometer[j] = gy.bussola[j];
    }
      
      update(gyro[0],gyro[1],gyro[2],acceleration[0],acceleration[1],acceleration[2],magnometer[0],magnometer[1],magnometer[2]);
      
      Accel = linear_acceleration(acceleration);

      time = millis();

      flag_uptade_AHRS = 0;

      

    }
  #endif

  
  #ifdef MADGWICK_AHRS
  #ifdef SERIAL_WRITE_A
  
    Serial.print(Accel.An);
    Serial.print(F("\t"));
    Serial.print(Accel.Al);
    Serial.print(F("\t"));
    Serial.print(Accel.Ab);
    Serial.print(F("\t"));
    Serial.print(gyro[0]);
    Serial.print(F("\t"));
    Serial.print(gyro[1]);
    Serial.print(F("\t"));
    Serial.print(gyro[2]);
    Serial.print(F("\t"));
    Serial.print(magnometer[0]);
    Serial.print(F("\t"));
    Serial.print(magnometer[1]);
    Serial.print(F("\t"));
    Serial.print(magnometer[2]);
    Serial.print(F("\t"));
    Serial.print(roll1);
    Serial.print(F("\t"));
    Serial.print(pitch1);
    Serial.print(F("\t"));
    Serial.print(yaw1);
    Serial.print(F("\t"));
    Serial.print(time);

    Serial.println();
  #endif

  #ifdef SD_CARD_WRITE
    arquivo.print(Accel.An);
    arquivo.print(F(";"));
    arquivo.print(Accel.Al);
    arquivo.print(F(";"));
    arquivo.print(Accel.Ab);
    arquivo.print(F(";"));
    arquivo.print(Accel.gravidade[0]);
    arquivo.print(F(";"));
    arquivo.print(Accel.gravidade[1]);
    arquivo.print(F(";"));
    arquivo.print(Accel.gravidade[2]);
    arquivo.print(F(";"));
    arquivo.print(roll1);
    arquivo.print(F(";"));
    arquivo.print(pitch1);
    arquivo.print(F(";"));
    arquivo.print(yaw1);
    arquivo.println(";");
  #endif
  #endif
}
