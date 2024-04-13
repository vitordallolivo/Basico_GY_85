#include <gy_85_basic.h>

#define MADGWICK_AHRS
 

#ifdef MADGWICK_AHRS
  #include <AHRS_gy85.h>
  #include <TimerOne.h>
#endif

// #define SD_CARD_WRITE

#ifdef SD_CARD_WRITE
  #include <SD.h>
  File arquivo;
#endif


// Define se vai ter serial, se comentar não tem output

#define SERIAL_WRITE_A

GY_85 gy;
offset OFF;


/* 

CASO TENHA QUE CONFIGURAR UM FILTRO DIFERENTE OU ALGUMA COISA ASSIM

Por exemplo mudar o filtro passa baixa do ITG para 188hz ( faixa de corte)

gy.itg.DLPF_CFG = 0x01;

*/



#ifdef MADGWICK_AHRS

PassFilter PASS_FILTER;



byte flag_uptade_AHRS=0;

float pitch1=0,yaw1=0,roll1=0;

float acceleration[3]={0,0,1};
float gyro[3]={0,0,0};
float magnometer[3]={0,0,0};
float velocity[3]={0,0,0};


double time;

void _UPTADE_AHRS(){
  flag_uptade_AHRS = 1;
}



#endif

void setup(){

  PASS_FILTER.RC =0.65f;


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
    if(!SD.begin(10)){
        #ifdef SERIAL_WRITE_A
          Serial.println("SD CARD NAO ENCONTRADO");
        #endif
        asm volatile("jmp 0");
    }

    arquivo = SD.open("example.txt",FILE_WRITE);

    arquivo.println("sensoriamento iniciando;");
    arquivo.println("ax;ay;az;gx;gy;gz;mx;my;mz;r;p;y;")

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
      update(gy.itg.x,gy.itg.y,gy.itg.z,gy.adx.Xg,gy.adx.Yg,gy.adx.Zg,gy.hmc.X,gy.hmc.Y,gy.hmc.Z);

      roll1  =  getRoll();
      pitch1 =  getPitch();
      yaw1   =  getYaw();
      
    for(int j = 0 ;j <3;j++){
        acceleration[j] = MedianFilter(gy.acceleration[j],&PASS_FILTER);
        velocity[j] = acceleration[j]/100;
        gyro[j] = gy.gyro[j];
        magnometer[j] = gy.bussola[j];
    }

      flag_uptade_AHRS = 0;

      time = millis();

    }
  #endif

  
  #ifdef MADGWICK_AHRS
  #ifdef SERIAL_WRITE_A
  
    Serial.print(acceleration[0]);
    Serial.print(F("\t"));
    Serial.print(acceleration[1]);
    Serial.print(F("\t"));
    Serial.print(acceleration[2]);
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
    arquivo.print(acceleration[0]);
    arquivo.print(F(";"));
    arquivo.print(acceleration[1]);
    arquivo.print(F(";"));
    arquivo.print(acceleration[2]);
    arquivo.print(F(";"));
    arquivo.print(gyro[0]);
    arquivo.print(F(";"));
    arquivo.print(gyro[1]);
    arquivo.print(F(";"));
    arquivo.print(gyro[2]);
    arquivo.print(F(";"));
    arquivo.print(magnometer[0]);
    arquivo.print(F(";"));
    arquivo.print(magnometer[1]);
    arquivo.print(F(";"));
    arquivo.print(magnometer[2]);
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
