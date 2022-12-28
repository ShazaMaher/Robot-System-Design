
#include <cstdlib>
#include <iostream>
#include "ros/ros.h"
#include <string>
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "global_def.h"
#include <ctime>


int state;
int guiSig;

void callbackPackML(const std_msgs::Int32 &_state) {
    state = _state.data;
}

void callbackGUI(const std_msgs::Int32 &_guiSig)
{
    guiSig = _guiSig.data;
}

int getState()
{
    return state;
}

void wait(double sec, int state)
{
    clock_t endwait;
    endwait = clock() + sec * CLOCKS_PER_SEC;
    while (clock() < endwait) {}
    ros::spinOnce();
    int states = getState();
    std::cout << state << std::endl;
    if(states == state)
        wait(sec, state);
}


int main(int argc,char* argv[]){

    ros::init(argc, argv, "PackML");

    ros::NodeHandle _n;
    //Pubs
    ros::Publisher statePub = _n.advertise<std_msgs::Int32>("packMLTopic", 1);
    ros::Publisher lightPub = _n.advertise<std_msgs::Int32>("lightTopic", 1);
    
    //Subs
    ros::Subscriber stateSub = _n.subscribe("packMLTopic", 1, &callbackPackML);
    ros::Subscriber gugiSub = _n.subscribe("guiTopic", 1, &callbackGUI);
    ros::Rate loop_rate(200);
    std::cout << "PackML started" << std::endl; 

    while(ros::ok()) {
        state = getState();
        std::cout << state << std::endl; 
        switch (state) {
            case STOPPED: {
                std_msgs::Int32 light;
                light.data = RED_SOLID;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case RESETTING: {
                std_msgs::Int32 light;
                light.data = YELLOW_FLASH;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case STARTING: {
                std_msgs::Int32 light;
                light.data = GREEN_SOLID;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case IDLE: {
                std_msgs::Int32 light;
                light.data = GREEN_FLASH;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case SUSPENDING: {
                std_msgs::Int32 light;
                light.data = YELLOW_SOLID;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case SUSPENDED: {
                std_msgs::Int32 light;
                light.data = YELLOW_SOLID;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case UNSUSPENDING: {
                std_msgs::Int32 light;
                light.data = GREEN_SOLID;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case EXECUTE: {
                std_msgs::Int32 light;
                light.data = GREEN_SOLID;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case STOPPING: {
                std_msgs::Int32 light;
                light.data = RED_SOLID;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case ABORTING: {
                std_msgs::Int32 light;
                light.data = RED_FLASH;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case ABORTED: {
                std_msgs::Int32 light;
                light.data = RED_FLASH;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case CLEARING: {
                std_msgs::Int32 light;
                light.data = RED_FLASH;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case HOLDING: {
                std_msgs::Int32 light;
                light.data = GREEN_YELLOW_FLASH;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case HELD: {
                std_msgs::Int32 light;
                light.data = GREEN_YELLOW_FLASH;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case UNHOLDING: {
                std_msgs::Int32 light;
                light.data = GREEN_SOLID;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case COMPLETING: {
                std_msgs::Int32 light;
                light.data = ALL_FLASH;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
            case COMPLETE: {
                std_msgs::Int32 light;
                light.data = ALL_FLASH;
                lightPub.publish(light);
                ros::spinOnce();
                break;
            }
        }
        ros::spinOnce();
    }

    std::cout << "System has been shutdown!"<< std::endl;
    return 0;
}
