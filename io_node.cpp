
#include "ros/ros.h"
#include <string>
#include "std_msgs/Int32.h"
#include "std_msgs/Bool.h"
#include <rtde_io_interface.h>
//Lights
#define RED_SOLID 0
#define RED_FLASH 1
#define YELLOW_SOLID 6
#define YELLOW_FLASH 2
#define GREEN_SOLID 4
#define GREEN_FLASH 3
#define GREEN_YELLOW_FLASH 5
#define ALL_FLASH 7
#define GRIPPER 8

using namespace ur_rtde;

int color = 9;
int blink = 0;
std::string ip = "192.168.0.201";
RTDEIOInterface rtdIO(ip);
bool flash = true;
bool black = false;
bool cb_flag = false;
bool flag;
int oldcolor;

void callbackLight(const std_msgs::Int32 _light)
{
    if(_light.data != color)
        black = true;

    color = _light.data;
}

void callbackGripper(const std_msgs::Bool _flag)
{
   //std::cout << "Disc" << std::endl; 
    flag = _flag.data;
    oldcolor = color;
    color = GRIPPER;
}

void turnOffLights()
{
    rtdIO.setStandardDigitalOut(0, false);
    rtdIO.setStandardDigitalOut(1, false);
    rtdIO.setStandardDigitalOut(2, false);
}



int main(int argc,char* argv[]){


    ros::init(argc, argv, "RSD2019_IO");
    ros::NodeHandle _n;
    std::string lightTopic = "lightTopic";
    ros::Subscriber lightSub = _n.subscribe(lightTopic, 1, &callbackLight);

    std::string gripperTopic = "gripperTopic";
    ros::Subscriber gripperSub = _n.subscribe(gripperTopic, 1, &callbackGripper);

    //rtdIO.setStandardDigitalOut(7, true); 
    std::cout << color << std::endl;
    float freq = 15000;
    //turnOffLights();
    ros::Rate rate(200);

    //rtdIO.setStandardDigitalOut(1, true);

    while(ros::ok())
    {

        switch(color)
        {
            case RED_FLASH: //Aborting, aborted, clearing
            
            if(black)
            {
                turnOffLights();
                black = false;
            }
            rtdIO.setStandardDigitalOut(2,flash);
            if(blink >= freq)
            {
                flash = !flash;
                blink = 0;
            }
            break;

            case RED_SOLID: //Stopping, stopped
            if(black)
            {
                turnOffLights();
                black = false;
            }
            rtdIO.setStandardDigitalOut(2,true);
            break;

            case YELLOW_FLASH: //Resetting
            if(black)
            {
                turnOffLights();
                black = false;
            }
            rtdIO.setStandardDigitalOut(1,flash);
            if(blink >= freq)
            {
                flash = !flash;
                blink = 0;
            }
            break;

            case GREEN_FLASH: //Idle
            if(black)
            {
                turnOffLights();
                black = false;
            }
            rtdIO.setStandardDigitalOut(0,flash);
            if(blink >= freq)
            {
                flash = !flash;
                blink = 0;
            }
            break;

            case GREEN_SOLID: //Starting, executing
            if(black)
            {
                turnOffLights();
                black = false;
            }
            rtdIO.setStandardDigitalOut(0,true);
            break;

            case GREEN_YELLOW_FLASH: //Holding, held
            if(black)
            {
                turnOffLights();
                black = false;
            }
            rtdIO.setStandardDigitalOut(1, flash);
            rtdIO.setStandardDigitalOut(0, flash);
              if(blink >= freq)
            {
                flash = !flash;
                blink = 0;
            }
            break;


            case YELLOW_SOLID: //Suspending, suspended
            if(black)
            {
                turnOffLights();
                black = false;
            }
            rtdIO.setStandardDigitalOut(1, true);
            break;

            case 9:
            turnOffLights();
            break;

            case GRIPPER:
                rtdIO.setStandardDigitalOut(4,flag);
                color = oldcolor;
                break;


        }

        blink++;
        ros::spinOnce();

    }



    return 0;
}
