#include <mpu.h>
#include <MPU6050.h>
#include <I2Cdev.h>
#include <Wire.h>

int IMU::init()
{
    Wire.begin();
    Wire.setClock(400000); // 设置I2C时钟频率为400kHz
    mpuA.initialize();
    if (!mpuA.testConnection()) {
        return -1; // 连接失败
    }
    
    return 0; // 成功
}


void IMU::update()
{
    mpuA.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    temp = mpuA.getTemperature();
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