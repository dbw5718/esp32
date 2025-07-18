#ifndef MPU_H
#define mpu.h

#include <MPU6050.h>


class IMU
{
private:
    /* data */
    MPU6050 mpuA;  //实例化MPU6050对象
    int16_t ax, ay, az; //加速度计数据
    int16_t gx, gy, gz; //陀螺仪数据
    int16_t temp; //温度数据
public:
    IMU(/* args */);
    ~IMU();
    int init(); //初始化MPU6050
    void update(); //更新传感器数据
    int16_t getAccelX();
    int16_t getAccelY();
    int16_t getAccelZ() ;
    int16_t getGyroX() ;    
    int16_t getGyroY() ;
    int16_t getGyroZ() ;
    int16_t getTemperature() ;

};

#endif