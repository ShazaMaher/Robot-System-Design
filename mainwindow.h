#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTime>
#include <QTimer>
#include <string>
#include <QColor>
#include <QString>
#include <QPushButton>
#include <QStateMachine>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Int32MultiArray.h"
#include "../global_def.h"



using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    string implementPackml(int state);
    void runHMI(int argc, char **argv);
    int getStateValue();
    int getTotalTimeValue();
    int getTotalTotalOrdersValue();
    int getSuccesfulOrdersValue();
    int getFaultsOrdersValue();
    int getRunTimeValue();
    float calculateAvailability(int uptime, int plannedProductionTime);
    float calculatePerformance(int allUnits, int idealCycleTime, int realCycleTime);
    float calculateQuality(int faults, int totalAmountOfUnits);
    float calculateOEE(float availability, float performance, float quality);
    int convertSecondsToHours(int seconds);
    int convertSecondsToMinuts(int seconds);
    std::vector<int> setColor(int r, int b, int g);
    std::vector<int> getRun_time_orders();
    void setStyleSheetPrograssBar(int value);

    void callbackSetStateValue(const std_msgs::Int8 &_stateM);
    void callbackSetButtonValue(const std_msgs::Int8 &_button);
    void callbackSetRunInformatinValue(const std_msgs::Int32MultiArray &_array);

    void callbackSetTotalTime(const std_msgs::Int32 &_totalTime);
    void callbackSetTotalOrders(const std_msgs::Int16 &_totalOrders);
    void callbackSetSuccesfulOrders(const std_msgs::Int16 &_succesfulOrders);
    void callbackSetFaultsOrders(const std_msgs::Int16 &_faultsOrders);
    void callbackSetRunTime(const std_msgs::Int32 &_runTime);
    void rosSpinOnce();
	

private:  
    Ui::MainWindow *ui;
    std::string totalTimeTopic = "totalTimeTopic";
    std::string totalOrdersTopic = "totalOrdersTopic";
    std::string succesfulOrdersTopic = "succesfulOrdersTopic";
    std::string faultsTopic = "faultsTopic";
    std::string runTimeTopic = "runTimeTopic";
    std::string packMLTopic = "packMLTopic";
    std::string guiTopic = "guiTopic";
    std::string runTopic = "runTopic";


    //Pubs
    ros::Publisher stateMachinePub;
    ros::Publisher guiPub;
    

    //Subs
    ros::Subscriber stateMachineSub;
    ros::Subscriber guiSub;
    ros::Subscriber runSub;

/*
    ros::Subscriber totalTimeSub;
    ros::Subscriber totalOrdersSub;
    ros::Subscriber succesfulOrdersSub;
    ros::Subscriber faultsOrdersSub;
    ros::Subscriber runTimeSub;

*/

    QTimer *currentTimer;
    int stateNum;
    string stateName;
    int total_time = 7900;
    int total_orders = 79;
    int succesful_orders = 55;
    int faults_orders = 24;
    int run_time = 7000;
    float availabity;
    float performance;
    float quality;
    float OEE;
    int idealCycleTime = 6;
    int realCycleTime = 10;
    int plannedProductionTime = 1440; // 24 * 60 // 24 hours
    std::vector<int> run_time_orders = {7900,6,79,55,24,7000,7,8,9,10};



private slots:
      void on_start_pushButton_clicked();
      //void on_stop_pushButton_clicked();
      void on_reset_pushButton_clicked();
      //void on_Hold_pushButton_clicked();
      void on_quit_pushButton_clicked();
      void on_stop_pushButton_toggled(bool checked);
      void on_Hold_pushButton_toggled(bool checked);



      void showTime();

};

#endif // MAINWINDOW_H
