#ifndef MIROBOT_H
#define MIROBOT_H
#include "stdio.h"
#include <unistd.h>
#include "wiringPi.h"
#include "wiringSerial.h"
#include <string>
#include <iostream>
#include "time.h"
#include <cstdlib>
#include <regex>


using namespace std;

string doubleToString(double &dbNum);
void getMatch(std::string instr,double data[]);

struct MirobotJoint
{
    int Joint1 = 1;
    int Joint2 = 2;
    int Joint3 = 3;
    int Joint4 = 4;
    int Joint5 = 5;
    int Joint6 = 6;
};
struct RevolveDirection
{
    int cw = 1;
    int ccw = 2;
};
struct MoveDirection
{
    int dforward = 1;
    int dbackward = 2;
    int dup = 3;
    int ddown = 4;
    int dright = 5;
    int dleft = 6;
};
struct MoveMode
{
    int relative = 1; //相对位置G91
    int absolute = 2; //绝对位置G90
};
struct Motion
{
    int MOVJ = 1;//关节运动
    int MOVL = 2;//直线运动
    int JUMP = 3;//门型轨迹运动
};
struct Status
{
    bool moving = false;//机械臂空闲状态
    //机械臂各轴角度值
    double A;
    double B;
    double C;
    double D;
    double X;
    double Y;
    double Z;
    //机械臂末端的位置和姿态
    double CCX;
    double CCY;
    double CCZ;
    double CCRX;
    double CCRY;
    double CCRZ;
    char* time;
    //两组PWM的值
    int PumpPWM;
    int ValvePWM;
};


class Mirobot
{
    private:
        bool debug;
        string port;
        Motion motiontype;
        MoveMode movemode;
        RevolveDirection revolvedirection;
        MirobotJoint mirobotjoint;
        MoveDirection movedirection;
        Status status;
        char time_temp[32];
    public:
    int fd;
    Mirobot(string myport="/dev/ttyUSB0",bool mydebug=false);
    string readline();
    string read_status();
    void get_time();
    //send a message
    void send_msg(string msg,bool wait=false);
    void waitForEnd();
    void dataflush();

    //每个轴复位
    //home each axis individually
    void home_individual();

    //复位
    //home all axes simultaneously
    void home_simultaneous();

    //解锁
    //unlock the shaft
    void unlock_shaft();

    //设定运动速度
    //set speed
    void set_speed(int speed);

    //关节旋转到
    //send all axes to a specific position
    void go_to_axis(double a1,double a2,double a3,double a4,double a5,double a6);

    //回归零位
    //send all axes to their respective zero positions
    void go_to_zero();

    //G1参数速度
    //G0最大速度
    //increment all axes a specified amount
    void increment_axis(double a1,double a2,double a3,double a4,double a5,double a6);

    //移动到坐标位置
    //linear move to a cartesian position
    void go_to_cartesian_lin(int m,double x,double y,double z,double a,double b,double c);

    //笛卡尔空间中的线性增量
    //linear increment in cartesian space
    void increment_cartesian_lin(int m,double x,double y,double z,double a,double b,double c);

    //吸盘开
    void suction_cup_on();

    //吸盘关
    void suction_cup_off();

    //吸盘吹
    void suction_cup_blow();

    //夹具开
    void gripper_on();

    //夹具关
    void gripper_off();

    //set the pwm of the air pump
    void set_air_pump(int pwm);

    // set the pwm of the gripper
    void set_gripper(int pwm);

    //延时执行 秒
    //set the pwm of the gripper
    void set_delay_time(int t);

    //滑轨移动到
    void slider_move_to(double x,int speed);

    //传送带移动到
    void conveyor_move_to(int c,double n,int speed);

    //关节移动
    void move_to_axis(int joint,int revolve,double n);

    //定向移动
    void direction_mobility(int d,double n);

    //门型轨迹移动 只用于笛卡尔模式
    void jump_move(int m,double x,double y,double z,double a,double b,double c);

    void set_arc_move(int mode,int revolve,double x,double y,double z,double r);

    void get_status();

    ~Mirobot();

};

void mirobot_reset(Mirobot &m);

#endif // MIROBOT_H
