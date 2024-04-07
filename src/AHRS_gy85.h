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
























#endif

