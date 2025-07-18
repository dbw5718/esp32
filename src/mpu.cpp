#include <mpu.h>
#include <MPU6050.h>
#include <I2Cdev.h>
#include <Wire.h>


void IMU::update()
{
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    temp = mpu.getTemperature();
}

int16_t IMU::getAccelX()
{
    return ax;
}  
int16_t IMU::getAccelY()
{
    return ay;
}
int16_t IMU::getAccelZ()
{
    return az;
}   
int16_t IMU::getGyroX()
{
    return gx;
}
int16_t IMU::getGyroY()
{
    return gy;
}
int16_t IMU::getGyroZ()
{
    return gz;
}
int16_t IMU::getTemperature()
{   
    return temp;
} 