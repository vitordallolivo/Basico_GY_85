#include "gy_85_basic.h"

void init_data(GY_85 gy85)
{
  Wire.begin();

  Wire.beginTransmission(gy85.itg.ADRESS);
  Wire.write(gy85.itg.DLPF_REGISTER);
  Wire.write(gy85.itg.DLPF);
  Wire.endTransmission();

  Wire.beginTransmission(gy85.itg.ADRESS);
  Wire.write(gy85.itg.SMPLRT_DIV_REGISTER);
  Wire.write(gy85.itg.SMPLRT_DIV);
  Wire.endTransmission();

  Wire.beginTransmission(gy85.adx.ADRESS);
  Wire.write(gy85.adx.Register_2D);
  Wire.write(8);
  Wire.endTransmission();

  Wire.beginTransmission(gy85.hmc.ADRESS);
  Wire.write(gy85.hmc.MODE_REGISTER);
  Wire.write(gy85.hmc.MODE_type);
  Wire.endTransmission();

}

void ADX_read(GY_85 *gy){


  Wire.beginTransmission(gy->adx.ADRESS); // transmit to device
  Wire.write(gy->adx.Register_X0);
  Wire.write(gy->adx.Register_X1);
  Wire.endTransmission();
  Wire.requestFrom(gy->adx.ADRESS,2); 
  if(Wire.available()<=2)   
  {
    gy->adx.X_out = Wire.read()<<8 | Wire.read();
  }
  //------------------Y
  Wire.beginTransmission(gy->adx.ADRESS); // transmit to device
  Wire.write(gy->adx.Register_Y0);
  Wire.write(gy->adx.Register_Y1);
  Wire.endTransmission();
  Wire.requestFrom(int(gy->adx.ADRESS),0x02); 
  if(Wire.available()<=2)   
  {
    gy->adx.Y_out =Wire.read()<<8 | Wire.read();
  }
  //------------------Z
  Wire.beginTransmission(gy->adx.ADRESS); // transmit to device
  Wire.write(gy->adx.Register_Z0);
  Wire.write(gy->adx.Register_Z1);
  Wire.endTransmission();
  Wire.requestFrom((gy->adx.ADRESS),2); 
  if(Wire.available()<=2)   
  {
    gy->adx.Z_out = Wire.read()<<8 | Wire.read();
  }

  gy->adx.Xg = double(gy->adx.X_out/256.0);
  gy->adx.Yg = double(gy->adx.Y_out/256.0);
  gy->adx.Zg = double(gy->adx.Z_out/256.0);

}

void ITG_read(GY_85 *gy){
    Wire.beginTransmission(gy->itg.ADRESS);
    Wire.write(gy->itg.Gx_MSB_REGISTER);
    Wire.endTransmission();

    Wire.requestFrom(gy->itg.ADRESS,8);

    gy->itg.x =  ((Wire.read() << 8) | Wire.read())/14.375;
    gy->itg.y =  ((Wire.read() << 8) | Wire.read())/14.375;
    gy->itg.z =  ((Wire.read() << 8) | Wire.read())/14.375;
    gy->itg.temp =  35+(double)(((Wire.read() << 8) | Wire.read())+13200)/280;
}

void HMC_read(GY_85 *gy){
    Wire.beginTransmission(gy->hmc.ADRESS);
    Wire.write(gy->hmc.X_MSB);
    Wire.endTransmission();

    Wire.requestFrom(gy->hmc.ADRESS,6);

    if(6<=Wire.available()){
        gy->hmc.X = Wire.read()<<8 | Wire.read();
        gy->hmc.Y = Wire.read()<<8 | Wire.read();
        gy->hmc.Z = Wire.read()<<8 | Wire.read();
    }
}


void read_data(GY_85 *gy85)
{
    ADX_read(gy85);
    ITG_read(gy85);
    HMC_read(gy85);   
}
