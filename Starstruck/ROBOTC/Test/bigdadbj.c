#pragma config(Sensor, dgtl1,  rightencoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftencoder,    sensorQuadEncoder)
#pragma config(Motor,  port2,           right,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           left,   tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           lift1,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           lift2,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           lift3,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           claw1,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           claw2,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           hang,          tmotorVex393_MC29, openLoop)
task main()
{
while(true){
		motor[right] = vexRT[Ch2];
		motor[left]  = vexRT[Ch3];
		//Left side of the robot is controlled by the left joystick, Y-axis
		motor[lift1] = vexRT[Ch2Xmtr2];
		motor[lift2] = vexRT[Ch2Xmtr2];
		motor[lift3] = vexRT[Ch2Xmtr2];
		motor[claw1] = vexRT[Ch3Xmtr2];
		motor[claw2] = vexRT[Ch3Xmtr2];
}


}