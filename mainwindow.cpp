#include "mainwindow.h"
#include <time.h>
#include<unistd.h>
#include <string>
#include <iostream>
#include <QApplication>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowTitle("RSD Project");

    ui->tabWidget->setTabText(0, "PACKML");
    ui->tabWidget->setTabText(1, "OEE");

    //color the buttons

    ui->start_pushButton->setStyleSheet(QString("background-color: green; color:white;"));
    ui->reset_pushButton->setStyleSheet(QString("background-color: green; color:white;"));
    ui->stop_pushButton->setStyleSheet(QString("background-color: green; color:white;"));
    ui->Hold_pushButton->setStyleSheet(QString("background-color: green; color:white;"));
    ui->quit_pushButton->setStyleSheet(QString("background-color: blue; color:white;"));



    //color the QLineEdit


        ui->stopped_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));

        ui->starting_lineEdit->setStyleSheet(QString("background-color: blue; color:white; "));

        ui->idle_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));

        ui->suspended_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));

        ui->executing_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));

        ui->stopping_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));

        ui->aborting_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));


        ui->aborted_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));


        ui->holding_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));

        ui->held_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));


        ui->restting_lineEdit->setStyleSheet(QString("background-color: blue; color:white; "));


        ui->Suspending_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));


        ui->unsuspend_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));

        ui->clearing_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));


        ui->unheld_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));

        ui->completting_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));

        ui->complete_lineEdit->setStyleSheet(QString("background-color: blue; color:white;"));



        //connect(ui->stop_pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked));



        //connect the pressed buttons to change the stateName machines


	//connect(ui->stop_pushButton, SIGNAL(clicked()), this, SLOT(MainWindow::on_stop_pushButton_clicked()));
	connect(ui->start_pushButton, SIGNAL(clicked()), this, SLOT(MainWindow::on_start_pushButton_clicked()));
	//connect(ui->Hold_pushButton, SIGNAL(clicked()), this, SLOT(MainWindow::on_Hold_pushButton_clicked()));
	connect(ui->reset_pushButton, SIGNAL(clicked()), this, SLOT(MainWindow::on_reset_pushButton_clicked()));
        connect(ui->quit_pushButton, SIGNAL(clicked()), this, SLOT(MainWindow::on_quit_pushButton_clicked()));

        connect(ui->stop_pushButton, SIGNAL(toggled(bool)), this, SLOT(on_stop_pushButton_toggled(bool)));
        connect(ui->Hold_pushButton, SIGNAL(toggled(bool)), this, SLOT(on_Hold_pushButton_toggled(bool)));
	


    currentTimer = new QTimer(this);

    connect(currentTimer, SIGNAL(&QTimer::timeout), this , SLOT(&MainWindow::showTime));
    currentTimer->start(1000);

    showTime();

    //////////////////////////////////////

    run_time_orders = getRun_time_orders();

    total_time = run_time_orders.at(0);

    total_orders = run_time_orders.at(1);
    succesful_orders = run_time_orders.at(2);
    faults_orders = total_orders - succesful_orders;
    run_time = run_time_orders.at(3);

    stateNum = getStateValue();
    stateName = implementPackml(stateNum);
 
    
   // total_time = getTotalTimeValue();
    int total_time_minuts = convertSecondsToMinuts(total_time);
    int total_time_hours = convertSecondsToHours(total_time);
    QFont littleFont("Courier New", 25);
    ui->totalTime_lineEdit->setFont(littleFont);
    ui->totalTime_lineEdit_2->setFont(littleFont);
    ui->totalTime_lineEdit->insert(QString::fromStdString(std::to_string(total_time_hours)));
    ui->totalTime_lineEdit_2->insert(QString::fromStdString(std::to_string(total_time_minuts)));
    
    //total_orders = getTotalTotalOrdersValue();
    ui->totalOrdersCount_lcd->display(total_orders);

    //succesful_orders = getSuccesfulOrdersValue();
    ui->SuccesfulOrdersCount_lcd->display(succesful_orders);

   // faults_orders = getFaultsOrdersValue();
    ui->faultsOrdersCount_lcd->display(faults_orders);

   // run_time = getRunTimeValue();
    int run_time_minuts = convertSecondsToMinuts(run_time);
    int run_time_hours = convertSecondsToHours(run_time);
    ui->runTime_lineEdit->setFont(littleFont);
    ui->runtime_lineEdit_2->setFont(littleFont);
    ui->runTime_lineEdit->insert(QString::fromStdString(std::to_string(run_time_hours)));
    ui->runtime_lineEdit_2->insert(QString::fromStdString(std::to_string(run_time_minuts)));


   ////////////////////// 

    availabity = calculateAvailability(run_time,  plannedProductionTime);
    performance = calculatePerformance( total_orders,  idealCycleTime,  realCycleTime);
    quality = calculateQuality( faults_orders,  total_orders);
    OEE = calculateOEE( availabity,  performance,  quality);

    ui->availabilty_lcd->display(availabity);
    ui->availabilty_progressBar->setValue(availabity);

    ui->performance_lcd->display(performance);
    ui->performance_progressBar->setValue(performance);
    ui->quality_lcd->display(quality);
    ui->quality_progressBar->setValue(quality);
    ui->oee_lcd->display(OEE);
    ui->oee_progressBar->setValue(OEE);


   // setStyleSheetPrograssBar( availabity);
    //setStyleSheetPrograssBar( performance);
    //setStyleSheetPrograssBar( quality);
    //setStyleSheetPrograssBar( OEE);

}

void MainWindow::runHMI(int argc, char **argv){
	ros::init(argc, argv, "HMI");
	ros::NodeHandle nH;
	stateMachinePub = nH.advertise<std_msgs::Int32>(packMLTopic, 1); //State Topic
        guiPub = nH.advertise<std_msgs::Int32>(guiTopic,1);



	stateMachineSub = nH.subscribe(packMLTopic, 1, &MainWindow::callbackSetStateValue, this);
	guiSub = nH.subscribe(guiTopic, 1, &MainWindow::callbackSetButtonValue, this);
	runSub = nH.subscribe(runTopic, 10, &MainWindow::callbackSetRunInformatinValue, this);
	
	
	/*totalTimeSub = nH.subscribe(totalTimeTopic, 1, &MainWindow::callbackSetTotalTime, this);
	totalOrdersSub = nH.subscribe(totalOrdersTopic, 1, &MainWindow::callbackSetTotalOrders, this);
	succesfulOrdersSub = nH.subscribe(succesfulOrdersTopic, 1, &MainWindow::callbackSetSuccesfulOrders, this);
	faultsOrdersSub = nH.subscribe(faultsTopic, 1, &MainWindow::callbackSetFaultsOrders, this);
	runTimeSub = nH.subscribe(runTimeTopic, 1, &MainWindow::callbackSetRunTime, this);*/

        ros::Rate loop_rate(10);
}

int MainWindow::getStateValue(){
    return stateNum;
}


int MainWindow::getTotalTimeValue(){
	return total_time;
}

int MainWindow::getTotalTotalOrdersValue(){
	return total_orders;

}

int MainWindow::getSuccesfulOrdersValue(){
	return succesful_orders;
}

int MainWindow::getFaultsOrdersValue(){
	return faults_orders;

}

int MainWindow::getRunTimeValue(){
	return run_time;
}

std::vector<int> MainWindow::getRun_time_orders(){

	return run_time_orders;
} 

void MainWindow::callbackSetStateValue(const std_msgs::Int8 &_stateM){
    stateNum = _stateM.data;
    //emit stateName = implementPackml(stateNum);
}

/*void MainWindow::callbackSetTotalTime(const std_msgs::Int32 &_totalTime){
    total_time = _totalTime.data;
}

void MainWindow::callbackSetTotalOrders(const std_msgs::Int16 &_totalOrders){
    total_orders = _totalOrders.data;
}

void MainWindow::callbackSetSuccesfulOrders(const std_msgs::Int16 &_succesfulOrders){
    succesful_orders = _succesfulOrders.data;
}

void MainWindow::callbackSetFaultsOrders(const std_msgs::Int16 &_faultsOrders){
    faults_orders = _faultsOrders.data;
} 


void MainWindow::callbackSetRunTime(const std_msgs::Int32 &_runTime){
    run_time = _runTime.data;
} */


void MainWindow::callbackSetButtonValue(const std_msgs::Int8 &_button){
	if (_button.data == 1){
		emit ui->Hold_pushButton->setChecked(true);

	}else if (_button.data == 2){
        	emit ui->stop_pushButton->setChecked(true);
        }

}

void MainWindow::callbackSetRunInformatinValue(const std_msgs::Int32MultiArray &_array){
	run_time_orders = _array.data;
}


string  MainWindow::implementPackml(int s){


    ui->stopped_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->starting_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->aborted_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->aborting_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->held_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->holding_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->suspended_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->Suspending_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->clearing_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->idle_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->executing_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->unheld_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->unsuspend_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->complete_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey; color:white;"));
    ui->completting_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey;color:white;"));
    ui->stopping_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey;color:white;"));
    ui->restting_lineEdit->setStyleSheet(QString("background-color: blue; border: 1px solid grey;color:white;"));


    switch(s)
    {
        case 0:
            stateName = "UNDEFINED";
	    break;
	case 1:
	    stateName = "OFF";
	    break;
        case STOPPED:
            stateName = "STOPPED";

            ui->stopped_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red; color:white;"));
            break;

        case STARTING:
            stateName = "STARTING";
            ui->starting_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));

            break;

        case IDLE:
	    stateName = "IDLE";
            ui->idle_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));
            break;

        case SUSPENDED:
	    stateName = "SUSPENDED";
            ui->suspended_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));
            break;

        case EXECUTE:
	    stateName = "EXECUTE";
            ui->executing_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));

            break;

        case STOPPING:
	    stateName = "STOPPING";
            ui->stopping_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));
            break;

        case ABORTING:
            stateName = "ABORTING";
            ui->aborting_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));

            break;

        case ABORTED:
            stateName = "ABORTED";
            ui->aborted_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));

            break;

        case HOLDING:
	    stateName = "HOLDING";
            ui->holding_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));

            break;

        case HELD:
	    stateName = "HELD";
            ui->held_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));

            break;

        case RESETTING:
	    stateName = "RESETTING";
            ui->restting_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));

            break;

        case SUSPENDING:
	    stateName = "Suspending";
            ui->Suspending_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));

            break;

        case UNSUSPENDING:
	    stateName = "UNSUSPENDING";
            ui->unsuspend_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));
            break;

        case CLEARING:
	    stateName = "CLEARING";
            ui->clearing_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));

            break;

        case UNHOLDING:
	    stateName = "UNHOLDING";
            ui->unheld_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));
            break;

        case COMPLETING:
	    stateName = "COMPLETING";
            ui->completting_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));

            break;

        case COMPLETE:
            stateName = "COMPLETE";	
            ui->complete_lineEdit->setStyleSheet(QString("background-color: blue; border: 4px solid red;color:white;"));
            break;

        default:
            break;
    }

    return stateName;

}

MainWindow::~MainWindow()
{
    delete ui;
    
}

void MainWindow::showTime(){

    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if((time.second() % 2) == 0)
        text[2] = ' ';

    ui->CurrentTime_lcd->display(text);

}

void MainWindow::rosSpinOnce(){

	ros::spinOnce();
}


void MainWindow::on_start_pushButton_clicked()
{
    this->stateNum = 0;
    std_msgs::Int32 state;
    state.data = STARTING;
    stateMachinePub.publish(state);	

    std_msgs::Int32 guiPress;
    guiPress.data = PUSH_START;
    guiPub.publish(guiPress);
    rosSpinOnce();
    

    //emit stateName = this->implementPackml(3);

}

/*void MainWindow::on_stop_pushButton_clicked()
{
    this->stateNum = 0;
    std_msgs::Int32 stateName;
    stateName.data = 2;
    stateMachinePub.publish(stateName);	
    rosSpinOnce();
    
    //emit stateName = this->implementPackml(2);

}*/

void MainWindow::on_reset_pushButton_clicked()
{

	    this->stateNum = 0;

	    std_msgs::Int32 state;
	    state.data = RESETTING;
	    stateMachinePub.publish(state);

            std_msgs::Int32 guiPress;
    	    guiPress.data = PUSH_RESET;
    	    guiPub.publish(guiPress);
	    rosSpinOnce();
    
    //emit stateName = this->implementPackml(100);
}

/*void MainWindow::on_Hold_pushButton_clicked()
{
    this->stateNum = 0;
    std_msgs::Int32 stateName;
    std_msgs::Int32 guiPress;
    guiPress.data = TOGGLE_HOLD;
    stateName.data = TOGGLE_HOLD;
    guiPub.publish(guiPress);
    stateMachinePub.publish(stateName);	
    rosSpinOnce();
    
    //emit stateName = this->implementPackml(10);
}*/

void MainWindow::on_quit_pushButton_clicked()
{

}


void MainWindow::on_stop_pushButton_toggled(bool checked)
{
    if (checked == true){
	    this->stateNum = 0;
	    std_msgs::Int32 state;
            state.data = STOPPING;
            stateMachinePub.publish(state);
	    
            std_msgs::Int32 guiPress;
    	    guiPress.data = TOGGLE_STOP;
    	    guiPub.publish(guiPress);
	    rosSpinOnce();
    }else if (checked == false){

	   /* this->stateNum = 0;
	    std_msgs::Int32 state;
            state.data = STOPPING;
            stateMachinePub.publish(state);*/

            std_msgs::Int32 guiPress;
    	    guiPress.data = UNTOGGLE_STOP;
    	    guiPub.publish(guiPress);
	    rosSpinOnce();
    }
}

void MainWindow::on_Hold_pushButton_toggled(bool checked)
{
     if (checked == true){
	this->stateNum = 0;

	std_msgs::Int32 state;
        state.data = HOLDING;
        stateMachinePub.publish(state);

    	std_msgs::Int32 guiPress;
    	guiPress.data = TOGGLE_HOLD;
    	guiPub.publish(guiPress);
   	rosSpinOnce();

    }else if (checked == false){
	
        this->stateNum = 0;

	std_msgs::Int32 state;
        state.data = EXECUTE;
        stateMachinePub.publish(state);

        sleep(2);

    	std_msgs::Int32 guiPress;
    	guiPress.data = UNTOGGLE_HOLD;
    	guiPub.publish(guiPress);
   	rosSpinOnce();
    }
}


float MainWindow::calculateAvailability(int uptime, int plannedProductionTime){

	float availabity = uptime / plannedProductionTime;

	return availabity;

}
float MainWindow::calculatePerformance(int allUnits, int idealCycleTime, int realCycleTime){

	float performance = (allUnits * idealCycleTime) / realCycleTime;
	return performance;

}
float MainWindow::calculateQuality(int faults, int totalAmountOfUnits){
	
	float quality = (totalAmountOfUnits - faults) / totalAmountOfUnits;
	return quality;

}
float MainWindow::calculateOEE(float availability, float performance, float quality){
	
	float OEE = availability * performance * quality;
	return OEE;
}

int MainWindow::convertSecondsToHours(int seconds){

	int  hours;
	hours = seconds / 3600;
	
	return hours;
}


int MainWindow::convertSecondsToMinuts(int seconds){

	int  minuts;
	minuts = (seconds % 3600)/ 60;
	
	return minuts;
}

std::vector<int> MainWindow::setColor(int R, int B, int G){

	B = 0;
	int normalizeR = (R * 100)/255;
	int normalizeB = (B * 100)/255;
	int normalizeG = (G * 100)/255; 
        
	int newR = R - normalizeR;
	int newB = 0;
	int newG = G + normalizeG;

	std::vector<int> color;
        color.push_back(R);
	color.push_back(B);
	color.push_back(G);

	return color;
}

void MainWindow::setStyleSheetPrograssBar(int value){
   switch(value)
    {
        case 0 ... 10:

		qApp->setStyleSheet("QProgressBar {background-color: white;"
                                            "color: black;"
                                            "border-style: outset;"
                                            "border-width: 2px;"
                                            "border-color: grey;"
                                            "border-radius: 7px;"
                                            "text-align: left; }"

                                            "QProgressBar::chunk {"
                                            "background-color: #FF0608; }");
		break;

	case 11 ... 20:

		qApp->setStyleSheet("QProgressBar {background-color: white;"
				                            "color: black;"
				                            "border-style: outset;"
				                            "border-width: 2px;"
				                            "border-color: grey;"
				                            "border-radius: 7px;"
				                            "text-align: left; }"

				                            "QProgressBar::chunk {"
				                            "background-color: #D85708; }");
		break;
	
	case 21 ... 30:

		qApp->setStyleSheet("QProgressBar {background-color: white;"
				                            "color: black;"
				                            "border-style: outset;"
				                            "border-width: 2px;"
				                            "border-color: grey;"
				                            "border-radius: 7px;"
				                            "text-align: left; }"

				                            "QProgressBar::chunk {"
				                            "background-color: #AAC308; }");

		break;
	
	case 31 ... 40:
		qApp->setStyleSheet("QProgressBar {background-color: white;"
                                            "color: black;"
                                            "border-style: outset;"
                                            "border-width: 2px;"
                                            "border-color: grey;"
                                            "border-radius: 7px;"
                                            "text-align: left; }"

                                            "QProgressBar::chunk {"
                                            "background-color: #AADB08; }");
		break;

	case 41 ... 50:
		qApp->setStyleSheet("QProgressBar {background-color: white;"
                                            "color: black;"
                                            "border-style: outset;"
                                            "border-width: 2px;"
                                            "border-color: grey;"
                                            "border-radius: 7px;"
                                            "text-align: left; }"

                                            "QProgressBar::chunk {"
                                            "background-color: #4BD908; }");
		break;

	case 51 ... 60:
		qApp->setStyleSheet("QProgressBar {background-color: white;"
                                            "color: black;"
                                            "border-style: outset;"
                                            "border-width: 2px;"
                                            "border-color: grey;"
                                            "border-radius: 7px;"
                                            "text-align: left; }"

                                            "QProgressBar::chunk {"
                                            "background-color: #5BB208; }");
		break;

	case 61 ... 70:
		qApp->setStyleSheet("QProgressBar {background-color: white;"
                                            "color: black;"
                                            "border-style: outset;"
                                            "border-width: 2px;"
                                            "border-color: grey;"
                                            "border-radius: 7px;"
                                            "text-align: left; }"

                                            "QProgressBar::chunk {"
                                            "background-color: ##329F08; }");
		break;

	case 71 ... 80:
		qApp->setStyleSheet("QProgressBar {background-color: white;"
                                            "color: black;"
                                            "border-style: outset;"
                                            "border-width: 2px;"
                                            "border-color: grey;"
                                            "border-radius: 7px;"
                                            "text-align: left; }"

                                            "QProgressBar::chunk {"
                                            "background-color: #328808; }");
		break;
		
	case 81 ... 90:
		qApp->setStyleSheet("QProgressBar {background-color: white;"
                                            "color: black;"
                                            "border-style: outset;"
                                            "border-width: 2px;"
                                            "border-color: grey;"
                                            "border-radius: 7px;"
                                            "text-align: left; }"

                                            "QProgressBar::chunk {"
                                            "background-color: #257608; }");
		break;

	case 91 ... 100:
		qApp->setStyleSheet("QProgressBar {background-color: white;"
                                            "color: black;"
                                            "border-style: outset;"
                                            "border-width: 2px;"
                                            "border-color: grey;"
                                            "border-radius: 7px;"
                                            "text-align: left; }"

                                            "QProgressBar::chunk {"
                                            "background-color: ##257608; }");
		break;
}
}

