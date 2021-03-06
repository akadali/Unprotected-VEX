#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in3,    liftpotenright, sensorPotentiometer)
#pragma config(Sensor, in4,    liftpotenleft,  sensorPotentiometer)
#pragma config(Sensor, dgtl1,  rightencoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftencoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  clawrightencoder, sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  clawleftencoder, sensorQuadEncoder)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           right,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           left,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           lift1,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           lift2,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           lift3,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           clawright,     tmotorVex393TurboSpeed_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port8,           clawleft,      tmotorVex393TurboSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(DatalogSeries, 0, "", Sensors, Sensor, dgtl7, 50)
#pragma config(DatalogSeries, 1, "", Sensors, Sensor, dgtl5, 50)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma DebuggerWindows("Globals")
#pragma DebuggerWindows("Motors")
#pragma DebuggerWindows("debugStream")
#define PID_SENSOR_SCALE    1

#define PID_MOTOR_SCALE     -1

#define PID_DRIVE_MAX       127
#define PID_DRIVE_MIN     (-127)

#define PID_INTEGRAL_LIMIT  50
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform

#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*  pid control task                                                           */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
// These could be constants but leaving
// as variables allows them to be modified in the debugger "live"
float  pid_Kp = 1;
float  pid_Ki = 0.1;
float  pid_Kd = 0.0;

static int   pidRunning = 1;
static float pidRequestedValue;


task pidControllerRight()
{
    float  pidSensorCurrentValue;
    float  pidError;
    float  pidLastError;
    float  pidIntegral;
    float  pidDerivative;
    float  pidDrive;


   SensorValue[clawleftencoder] = 0;
   SensorValue[clawrightencoder] = 0;

    // Init the variables
    pidLastError  = 0;
    pidIntegral   = 0;

    while( true )
        {
        // Is PID control active ?
        if( pidRunning )
            {
            // Read the sensor value and scale
            pidSensorCurrentValue = SensorValue[clawrightencoder] * PID_SENSOR_SCALE;

            // calculate error
            pidError = pidSensorCurrentValue - pidRequestedValue;

            // integral - if Ki is not 0
            if( pid_Ki != 0 )
                {
                // If we are inside controlable window then integrate the error
                if( abs(pidError) < PID_INTEGRAL_LIMIT )
                    pidIntegral = pidIntegral + pidError;
                else
                    pidIntegral = 0;
                }
            else
                pidIntegral = 0;

            // calculate the derivative
            pidDerivative = pidError - pidLastError;
            pidLastError  = pidError;

            // calculate drive
            pidDrive = (pid_Kp * pidError) + (pid_Ki * pidIntegral) + (pid_Kd * pidDerivative);

            // limit drive
            if( pidDrive > PID_DRIVE_MAX )
                pidDrive = PID_DRIVE_MAX;
            if( pidDrive < PID_DRIVE_MIN )
                pidDrive = PID_DRIVE_MIN;

            // send to motor
            motor[clawright] = pidDrive * PID_MOTOR_SCALE;
            }
        else
            {
            // clear all
            pidError      = 0;
            pidLastError  = 0;
            pidIntegral   = 0;
            pidDerivative = 0;
            motor[clawright] = 0;
            }

        // Run at 50Hz
        wait1Msec( 25 );
        }
}

task pidControllerLeft()
{
    float  pidSensorCurrentValue;
    float  pidError;
    float  pidLastError;
    float  pidIntegral;
    float  pidDerivative;
    float  pidDrive;


   SensorValue[clawleftencoder] = 0;

    // Init the variables
    pidLastError  = 0;
    pidIntegral   = 0;

    while( true )
        {
        // Is PID control active ?
        if( pidRunning )
            {
            // Read the sensor value and scale
            pidSensorCurrentValue = SensorValue[clawleftencoder] * PID_SENSOR_SCALE;

            // calculate error
            pidError = pidSensorCurrentValue - pidRequestedValue;

            // integral - if Ki is not 0
            if( pid_Ki != 0 )
                {
                // If we are inside controlable window then integrate the error
                if( abs(pidError) < PID_INTEGRAL_LIMIT )
                    pidIntegral = pidIntegral + pidError;
                else
                    pidIntegral = 0;
                }
            else
                pidIntegral = 0;

            // calculate the derivative
            pidDerivative = pidError - pidLastError;
            pidLastError  = pidError;

            // calculate drive
            pidDrive = (pid_Kp * pidError) + (pid_Ki * pidIntegral) + (pid_Kd * pidDerivative);

            // limit drive
            if( pidDrive > PID_DRIVE_MAX )
                pidDrive = PID_DRIVE_MAX;
            if( pidDrive < PID_DRIVE_MIN )
                pidDrive = PID_DRIVE_MIN;

            // send to motor
            motor[clawleft] = pidDrive * PID_MOTOR_SCALE;
            }
        else
            {
            // clear all
            pidError      = 0;
            pidLastError  = 0;
            pidIntegral   = 0;
            pidDerivative = 0;
            motor[clawleft] = 0;
            }

        // Run at 50Hz
        wait1Msec( 25 );
        }
}


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

//LCD Display code.
const short leftButton = 1;
const short centerButton = 2;
const short rightButton = 4;
//Declare count variable to keep track of our choice
int count = 0;
//Wait for Press--------------------------------------------------
void waitForPress()
{
	while(nLCDButtons == 0){}
	wait1Msec(5);
}
//----------------------------------------------------------------

//Wait for Release------------------------------------------------
void waitForRelease()
{
	while(nLCDButtons != 0){}
	wait1Msec(5);
}


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
	//------------- Beginning of User Interface Code ---------------
	//Clear LCD
	clearLCDLine(0);
	clearLCDLine(1);
	bLCDBacklight = true;
	//Loop while center button is not pressed
	while(nLCDButtons != centerButton)
	{
		//Switch case that allows the user to choose from 4 different options
		switch(count){
		case 0:
			//Display first choice
			displayLCDCenteredString(0, "Red Left");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count = 3;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 1:
			//Display second choice
			displayLCDCenteredString(0, "Red Right");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 2:
			//Display third choice
			displayLCDCenteredString(0, "Blue Left");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 3:
			//Display fourth choice
			displayLCDCenteredString(0, "Blue Right");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count = 0;
			}
			break;
		default:

			count = 0;

			break;
		}
	}
}

//FUNCTIONS: Convert chasis rotations to inches and vise versa for easy autonomous coding. Do this by having functions which before being
//called are given a value (inches) to move or rotate.
int inchesmoved = 0;
int inchestomove = 0;
int rightinches = 0;
int leftinches = 0;
int rightinchestomove = 0;
int leftinchestomove = 0;


void moveforward()
{
	motor[right] = 127;
	motor[left] = 127;
	waitUntil(abs(inchesmoved) < inchestomove);
	motor[left] = 0;
	motor[right] = 0;
}

void movebackward()
{
	motor[left] = -127;
	motor[right] = -127;
	waitUntil(abs(inchesmoved) < inchestomove);
	motor[right] = 0;
	motor[left] = 0;
}
void turnright()
{
	motor[left] = 127;
	motor[right] = -127;
	waitUntil(abs(rightinches) < rightinchestomove);
	motor[right] = 0;
	motor[left] = 0;
}
void turnleft()
{
	motor[left] = -127;
	motor[right] = 127;
	waitUntil(abs(leftinches) < leftinchestomove);
	motor[left] = 0;
	motor[right] = 0;
}
void inchreset()
{
	SensorValue[rightencoder] = 0;
	SensorValue[leftencoder] = 0;
	rightinches = 0;
	leftinches = 0;
	inchesmoved = 0;
}

task algo()
{
	while(true)
	{
		rightinches = SensorValue[rightencoder] * 12.56 / 360;
		leftinches = SensorValue[leftencoder] * -12.56 / 360;
		inchesmoved = (leftinches + rightinches)/2;
		wait1Msec(10);
		//writeDebugStreamLine("left encoder count is %d", SensorValue[leftencoder]*-1 );
		//writeDebugStreamLine("right encoder count is %d", SensorValue[rightencoder]*-1 );
		//writeDebugStreamLine("inches moved is %d", inchesmoved);
		//writeDebugStreamLine("right inches moved is %d", rightinches);
		//writeDebugStreamLine("step count is %d", nextstep);
	}
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
	startTask (algo);
	//Clear LCD
	clearLCDLine(0);
	clearLCDLine(1);
	//Switch Case that actually runs the user choice
	switch(count){
	case 0:
		//If count = 0, run the code correspoinding with choice 1
		displayLCDCenteredString(0, "Red Left");
		SensorValue[leftencoder] = 0;
		SensorValue[rightencoder] = 0;
		inchestomove = 26;
		motor[clawleft] = 80;
		motor[clawright] = 80;
		moveforward();
		inchestomove = 0;
		inchreset();
		rightinchestomove = 6;
		turnright();
		motor[clawright] = 0;
		motor[clawleft] = 0;
		rightinchestomove = 0;
		inchreset();
		inchestomove = 28;
		moveforward();
		inchestomove = 0;
		inchreset();
		leftinchestomove = 4.7;
		motor[clawright] = 127;
		motor[clawleft] = 127;
		wait1Msec(600);
		motor[clawleft] = 80;
		motor[clawright] = 80;
		wait1Msec(400);
		motor[lift1] = 127;
		motor[lift2] = 127;
		wait1Msec(100);
		turnleft();
		leftinchestomove = 0;
		inchreset();
		inchestomove = 24;
		moveforward();
		inchestomove = 0;
		inchreset();
		inchestomove = -20;
		motor[clawleft] = -127;
		motor[clawright] = -127;
		wait1Msec(500);
		motor[clawright] = 80;
		motor[clawleft] = 80;
		movebackward();
		motor[clawright] = 0;
		motor[clawleft] = 0;
		motor[lift1] = 0;
		motor[lift2] = 0;
		movebackward();
		inchestomove = 0;
		inchreset();
		break;
	case 1:
		//If count = 1, run the code correspoinding with choice 2
		displayLCDCenteredString(0, "Red Right");
		displayLCDCenteredString(1, "is running!");



		break;
	case 2:
		//If count = 2, run the code correspoinding with choice 3
		displayLCDCenteredString(0, "Blue Left");
		displayLCDCenteredString(1, "is running!");

		break;

	case 3:
		//If count = 3, run the code correspoinding with choice 4
		displayLCDCenteredString(0, "Blue Right");
		displayLCDCenteredString(1, "is running!");


		break;
	default:
		displayLCDCenteredString(0, "No valid choice");
		displayLCDCenteredString(1, "was made!!");
		break;
	}
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
//ticks 261.333 pre rev turbo, standard 627.2
task usercontrol()
{
  // User control code here, inside the loop
	// change for constant speed if u ever want idk me from the future
    pidRequestedValue = 0;

    // start the PID task
    startTask(pidControllerLeft);
    startTask(pidControllerRight);
  while (true)
  {
  	//PID CONTROL GOAL: Calculate error, if negative, increase power and use deadband to stop joystick
  	// maximum change for pidRequestedValue will be 127/4*20, around 640 counts per second
    // free spinning motor is 100rmp so 1.67 rotations per second
    // 1.67 * 360 counts is 600
    pidRequestedValue = pidRequestedValue + (vexRT[Ch3Xmtr2]/4);
    if (pidRequestedValue > 0)
    {
    pidRequestedValue = pidRequestedValue - 3;
    }
    if (pidRequestedValue < 0)
    {
    pidRequestedValue = pidRequestedValue + 3;
    }
  	//Right side of the robot is controlled by the right joystick, Y-axis
		motor[right] = vexRT[Ch2];
		//Left side of the robot is controlled by the left joystick, Y-axis
		motor[left]  = vexRT[Ch3];
		motor[lift1] = vexRT[Ch2Xmtr2];
		motor[lift2] = vexRT[Ch2Xmtr2];
		motor[lift3] = vexRT[Ch2Xmtr2];
		//Debugging
		//writeDebugStreamLine("claw count is right %d left %d", clawencoderrightvalue, clawencoderleftvalue);
		wait1Msec(20);
  }
}
