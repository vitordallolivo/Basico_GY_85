#ifndef AHRS_GY85
#define AHRS_GY85


float invSqrt(float x);


void begin(float sampleFrequencia);


void update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);


void updateIMU(float gx, float gy, float gz, float ax, float ay, float az);

void ComputarAngulos();

float getRoll();
float getYaw();
float getPitch();

struct PassFilter{
    double RC=1;
    double data_k=0;
    double yk=0;
};

struct Global_acceleration
{
    double An; // Aceleração em relação ao norte
    double Al; // Aceleração em relação ao leste
    double Ab; // Aceleração para baixo

    float gravidade[3];
};


Global_acceleration linear_acceleration(float accel[3]);


double LowpassFilter(float data, PassFilter *LOWPASS);
double HighpassFilter(float data,PassFilter *HIGHPASS);
double MedianFilter(float data, PassFilter *Median);




#endif

