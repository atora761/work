
#include "../include/Laser.hpp"
#include "../include/Device.hpp"
Laser::Laser(){}
Laser::~Laser(){}

int Laser::init(){
    
    if (wiringPiSetupGpio() == -1) {
        return 1;
    }
    pinMode(18, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(375);
    pwmSetRange(1024);
    
    return 0;
}

int Laser::run(int duty){
    pwmWrite(18, 10.24*duty);
    //printf("duty=%d\n",duty);
    return 0;
}

int Laser::stop(){

    pwmWrite(18, 10.24*0);
    return 0;
}
