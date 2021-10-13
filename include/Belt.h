#ifndef BELT_H_INCLUDED
#define BELT_H_INCLUDED

#include  "stdio.h"
#include  <unistd.h>
#include  <iostream>
#include  <malloc.h>
#include  <string.h>
#include  <modbus/modbus.h>
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-version.h>
#include <wiringPi.h>

#define SENSOR 1
#define MOTOR_OUT1 10
#define MOTOR_OUT2 11
volatile int stick_status = 1; // 0: down; 1: up

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

        Belt(string myport="/dev/ttyS0",int mybaudrate=38400,int address=21)
        {   port=myport;
            const char *tmp=port.data();
            int tmp1=0,tmp2=0,tmp3=0;

            //connect and set debug,slave
            ctx=modbus_new_rtu(tmp,mybaudrate, 'N', 8, 1);
            tmp1 = modbus_set_slave(ctx,address);
            tmp2 = modbus_set_debug(ctx, TRUE);
            tmp3 = modbus_connect(ctx);

            if ((tmp1!=-1)&&(tmp2!=-1)&&(tmp3!=-1))
            {printf("connect successfully\n");}
        }


        void reset(int address)
        {
            /*
            reset the motor position to zero
            Usually used when motor is just boot up
            not recommended when position 0 is need for reset function is quite slow,it's beetter to use setpos(0)
            */
            slave_address = address;
            modbus_write_register(ctx,2000,0);
            return ;
        }

        void stop(int address)
        {
            slave_address=address;
            modbus_write_register(ctx,2001,0);
            return ;
        }

        Result readpos(int address)
        {
            Result  result;
            slave_address=address;
            int ret=-1;
            uint16_t* res=NULL;
            res=(uint16_t*)malloc(5*sizeof(uint16_t));
            if(res==NULL)
            {
                throw "malloc failed!\n";
            }
            else
            {
                memset(res,0,5*sizeof(uint16_t));
            }
            ret=modbus_read_registers(ctx,1000,5,res);//read registers
            if(ret!=5)
            {
                printf("error modbus_read_registers:%d\n",ret);
            }
            //read succeed
            else
            {
                result.cur=(res[0]<<16)+res[1];
                if((res[0]>>15)==1)
                {
                    result.cur=(-1)*((~result.cur+1)&(0xFFFFFFFF));
                }
                result.tar=(res[2]<<16)+res[3];
                if(res[2]>>15==1)
                {
                    result.tar=(-1)*((~result.tar+1)&(0xFFFFFFFF));
                }
                result.state=res[4];
            }
            free(res);
            return result;
        }

        void setpos(int address,int tar)
        {
            //set motor to an absolute position
            slave_address=address;
            int ret=-1;
            uint16_t* res=NULL;
            res=(uint16_t*)malloc(2*sizeof(uint16_t));
            if(res==NULL)
            {
                fprintf(stderr,"malloc failed!\n");
                return ;
            }
            else
            {
                memset(res,0,2*sizeof(uint16_t));
            }
            if(tar<0)
            {
                tar=tar&(0xFFFFFFFF);
            }
            if(tar>65535)
            {
                res[0]=tar>>16;
                res[1]=tar&(0xFFFF);
            }
            else
            {
                res[1]=tar;
            }
            ret=modbus_write_registers(ctx,2002,2,res);
            if(ret!=2)
            {
                printf("error modbus_write_registers:%d\n",ret);
            }
            free(res);
        }

        void formove(int address,int tar)
        {
            slave_address=address;
            int ret=-1;
            uint16_t* res=NULL;
            res=(uint16_t*)malloc(2*sizeof(uint16_t));
            if(res==NULL)
            {
                fprintf(stderr,"malloc failed!\n");
                return ;
            }
            else
            {
                memset(res,0,2*sizeof(uint16_t));
            }
            if(tar>65535)
            {
                res[0]=tar>>16;
                res[1]=tar&(0xFFFF);
            }
            else
            {
                res[1]=tar;
            }
            ret=modbus_write_registers(ctx,2004,2,res);
            if(ret!=2)
            {
                printf("error modbus_write_registers:%d\n",ret);
            }
            free(res);
        }

        void backmove(int address,int tar)
        {
            slave_address=address;
            int ret=-1;
            uint16_t* res=NULL;
            res=(uint16_t*)malloc(2*sizeof(uint16_t));
            if(res==NULL)
            {
                fprintf(stderr,"malloc failed!\n");
                return ;
            }
            else
            {
                memset(res,0,2*sizeof(uint16_t));
            }
            if(tar>65535)
            {
                res[0]=tar>>16;
                res[1]=tar&(0xFFFF);
            }
            else
            {
                res[1]=tar;
            }
            ret=modbus_write_registers(ctx,2006,2,res);
            if(ret!=2)
            {
                printf("error modbus_write_registers:%d\n",ret);
            }
            free(res);
        }

        void set_speed(int address,int duty,int step)
        {
            slave_address=address;
            int ret=-1;
            uint16_t* res=NULL;
            res=(uint16_t*)malloc(2*sizeof(uint16_t));
            if(res==NULL)
            {
                fprintf(stderr,"malloc failed!\n");
                return ;
            }
            else
            {
                memset(res,0,2*sizeof(uint16_t));
            }
            if(duty>65535)
            {
                res[0]=duty>>16;
                res[1]=duty&(0xFFFF);
            }
            else
            {
                res[1]=duty;
            }
            ret=modbus_write_registers(ctx,2008,2,res);
            if(ret!=2)
            {
                printf("error modbus_write_registers:%d\n",ret);
            }
            modbus_write_register(ctx,3001,step);
            modbus_write_register(ctx,4002,0);
            free(res);
        }
        void set_rpm(int address,int rpm)
        {
            slave_address=address;
            int ret=-1;
            uint16_t* res=NULL;
            res=(uint16_t*)malloc(2*sizeof(uint16_t));
            if(res==NULL)
            {
                fprintf(stderr,"malloc failed!\n");
                return ;
            }
            else
            {
                memset(res,0,2*sizeof(uint16_t));
            }
            if(rpm>65535)
            {
                res[0]=rpm>>16;
                res[1]=rpm&(0xFFFF);
            }
            else
            {
                res[1]=rpm;
            }
            ret=modbus_write_registers(ctx,2010,2,res);
            if(ret!=2)
            {
                printf("error modbus_write_registers:%d\n",ret);
            }
            free(res);
        }

        void disconnect()
        {
            modbus_close(ctx);
            modbus_free(ctx);
        }
};

void senser_setup(void)
{
    wiringPiSetup();
    pinMode(SENSOR, INPUT);
    pinMode(MOTOR_OUT1, OUTPUT);
    pinMode(MOTOR_OUT2, OUTPUT);
}

void stickUp (int delaytime=4000)
{
    digitalWrite(MOTOR_OUT1, LOW);
    digitalWrite(MOTOR_OUT2, HIGH);
    delay(delaytime);
    //delay(100);
    digitalWrite(MOTOR_OUT1, HIGH);
    stick_status = 1;
}

void stickDown (int delaytime=4200)
{
    digitalWrite(MOTOR_OUT1, HIGH);
    digitalWrite(MOTOR_OUT2, LOW);
    delay(delaytime);
    //delay(100);
    digitalWrite(MOTOR_OUT1, LOW);
    stick_status = 0;
}

void senser_control_motor(void)
{
    int sensor_flag = 0;
    for (;;)
    {
        sensor_flag = digitalRead(SENSOR);
        if (sensor_flag == 1 && stick_status == 0)
        {
            stickUp();
        }
        if (sensor_flag == 0 && stick_status == 1)
        {
            stickDown();
        }
        cout << sensor_flag << endl;
    }
}

#endif // BELT_H_INCLUDED
