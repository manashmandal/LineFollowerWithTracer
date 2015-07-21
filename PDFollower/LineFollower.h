/*
	DESCRIPTION:
	===========================================
	Proportional Derivative based Line follower 

	Author:         Manash
			Khulna University of Engineering & Technology, EEE
			e-mail: manashmndl[at]gmail[dot]com

	Github Link: http://www.github.com/manashmndl/LineFollowerWithTracer

	Core Contributor:                 Towsif Taher
					  Bangladesh University of Engineering & Technology, EEE
					  e-mail: taher[dot]towfi2012[at]gmai­l[dot]com
	LICENSE: MIT
*/



#include <Arduino.h>
#include <math.h>
#include <SoftwareSerial.h>


#ifndef LINEFOLLOWER_H
#define LINEFOLLOWER_H

namespace pdlf{
	class Robot{
		
	private:
		//Arrays for motors 
		byte left_motor[2];
		byte right_motor[2];

		//Pointer variable for sensor pins
		byte *sensor_pin;

		//analog values of sensors 
		int *s_analog_reading;
		//Applying threshold to convert analog values into digital
		byte *s_digital_reading;

		//Variable for holding number of sensors
		byte sensor_number;

		//Sensor weights, size depends on the number of sensors
		int *weight;

		//Weighted value pointer
		int *weighted_value;

		//constant kp and kd
		double kp, kd;

		//Active sensor number
		size_t active_sensors;

		//Error and previous error variable
		double error, previous_error;


    
	public:
                      SoftwareSerial *bluetooth;

		/*
			[Constructor]
			DESCRIPTION:
			===========
			Initializes everything 
		*/

		Robot(byte *lm, byte *rm, byte *s, byte num_sensor, char mcu);
		

		/*
		DESCRIPTION:
		============
		Additional helper variable to distinguish between white and black line
		
		inverted_logic = true ---> white line, other surroundings
		inverted_logic = false ---> black line
		, other surroundings

		ASSUMPTION:
		============
		Analog IR sensor gives higher values on black surface

		DEFAULT VALUE:
		==============
		false

		SETTER STATEMENT:
		-----------------
		pdlf::Robot::inverted_logic = true;
		*/
		static bool inverted_logic;

		/*
		DESCRIPTION:
		==============
		Threshold variable for converting analog values into digital [1/0]

		DEFAULT VALUE:
		==============
		200

		SETTER STATEMENT:
		==================
		pdlf::Robot::THRESHOLD  = value;
		
		*/

		static size_t THRESHOLD;


		/*
			DESCRIPTION:
			============
			Function for printing analog values of sensors

			ARGUMENTS:
			==========
			byte val1, byte val2
			val1 ------> short dalay, how much  the program will be delayed to print a value of an individual sensor

			val2 ----> long delay, how much much the program will be delayed after printing all analog values of the sensors

			DEFAULT ARGUMENT:
			=================
			val1 ---------> 100ms
			val2 ---------> 1500ms [1.5s]

			REQUIREMENTS:
			=============
			Must initialize Serial

			SAMPLE OUTPUT:
			=============
			*********** SENSOR ANALOG VALUE BEGIN ************
			sensor[0] = 100
			sensor[1] = 120
			...............
			*********** SENSOR ANALOG VALUE END **************

		*/


		void printAnalog(byte sdel = 100, int ldel = 1500);

		/*
		DESCRIPTION:
		===========

		Static variable for storing global speed of motor

		*/

		static int global_speed;


		/*
			DESCRIPTION:
			=================
			Function for printing digital values of the sensors 

			PRINTING FLOW:
			================
			From left pins to right pins

			ARGUMENT:
			=========
			byte val

			DEFAULT ARGUMENT:
			================
			val ----------> 200ms

			how much the program will be delayed after printing all digital values in a row

			REQUIREMENTS:
			=============
			Must Initialize Serial

			SAMPLE OUTPUT:
			==============
			------------begin----------------------
			00001110
			--------------end----------------------
		*/

		void printDigital(byte del = 200);

		/*
		DESCRIPTION:
		============
		Returns number of sensor
		*/
		byte sensorNumber(void) const;

		/*
		DESCRIPTION:
		============
		Sets motors as outputs and sensors as inputs
		*/
		void initializeComponents(void);

		/*
		Enumeration for directions
		*/
		enum dir {Forward, Backward, Left, Right, Clockwise, AntiClockwise, BackwardLeft, BackwardRight ,Nowhere};
		dir Direction;

		/*

		DESCRIPTION:
		============
		Motor driving functions to meet your needs. Directions are described here.

		[Left arrow for left motor and right arrow for right motor, left and right motors are defined when the robot is faced towards forward]

		Forward: ↑ ↑
		Backward: ↓ ↓
		Left: . ↑
		Right: ↑ .
		BackwardLeft: . ↓
		BackwardRight: ↓ .
		Clockwise: ↑ ↓
		AntiClockwise: ↓ ↑
		Nowhere: . .

		*/
		void run(int left_speed, int right_speed, dir left_direction, dir right_direction);
		void run(int left_speed, int right_speed, dir rotation_direction);
		void run(dir direction = Nowhere);

		/*
		 DESCRIPTION:
		 ===========
		 Setter functions for setting values of kp and kd
		*/

		void setKp(double p)  { kp = p; }
		void setKd(double d)  { kd = d; }

		/*
		DESCRIPTION:
		============
		Fetch functions for getting values of kp and kd
		*/
		double Kp (void) const { return kp; }
		double Kd	(void) const	{ return kd; }

		/*
		DESCRIPTION:
		============
			When the functions are called they update sensor readings,
			updateAnalogRead reads analog values and put in the s_analog_reading array
			updateDigitalRead calls updateAnalogRead but then it converts the analog values into digital depending on the value of THRESHOLD

			updateWeightedValue updates the weighted_value array by multiplying the weight and digital values
		*/
		void updateAnalogRead(void);
		void updateDigitalRead(void);
		void updateWeightedValue(void);
		int weightedValueSum(void);

		/*
			DESCRIPTION:
			===========
			Generates weight depending on the number of sensors:
			let's say number of sensors are 7, so 7/2 = 3.5 then take it as 3 
			For odd number of sensors: -3, -2, -1, 0, 1, 2, 3
			For even number of sensors: -3, -2, -1, 1, 2, 3
		*/

		void generateWeight(int n);

		/*
			DESCRIPTION:
			===========
			Prints weight

			OUTPUT SAMPLE:
			=============
			If the number of sensor is 8:
			-4	-3	-2	-1	1	2	3	4
			If the number of sensor is 7:
			-3	-2	-1	0	1	2	3	

			REQUIREMENTS:
			=============
			Must initialize Serial

		*/

		void printWeight(void);
		/*
			DESCRIPTION:
			============
			Prints weighted value
			Weighted value = digital_reading * corresponding weight;

			OUTPUT SAMPLE:
			==============
			If the digital values are: 011100
			Weighted value: 0	-2	-1	1	0	0
			*/

		void printWeightedValue(void);

		/*
			DESCRIPTION:
			============
			Takes kp and kd as input and calculate error
		*/

		void pdLineFollow(void);

    /*  
      DESCRIPTION:
      ===========
      Added new functions for line tracing capability
    */

		int positionTracker(void);
		

	};
}

#endif
