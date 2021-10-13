#ifndef BELT_H
#define BELT_H

#include  <modbus/modbus.h>


using namespace std;

struct Result{
    int cur;
    int tar;
    int state;
};

class Belt{
    private:

        string port;
        int slave_address;
        int baudrate;
        modbus_t *ctx=NULL;


    public:

        Belt(string myport="/dev/ttyS0",int mybaudrate=38400,int address=21);
        void reset(int address);
        void stop(int address);
        Result readpos(int address);
        void setpos(int address,int tar);
        void formove(int address,int tar);
        void backmove(int address,int tar);
        void set_speed(int address,int duty,int step);
        void set_rpm(int address,int rpm);
        void disconnect();

//        void senser_setup(void)
//        {
//            wiringPiSetup();
//            pinMode(SENSOR, INPUT);
//            pinMode(MOTOR_OUT1, OUTPUT);
//            pinMode(MOTOR_OUT2, OUTPUT);
//        }

        void stickUp (int delaytime);
        void stickDown (int delaytime);
        void senser_control_motor(void);

};




#endif // BELT_H
