#include "LineFollower.h"

//Setting inverted logic false
bool pdlf::Robot::inverted_logic = false;

//Setting the default threshold value

size_t pdlf::Robot::THRESHOLD = 200;

//Setting global speed

int pdlf::Robot::global_speed = 200; //was 150

//Initializing components
void pdlf::Robot::initializeComponents(void)
{
	for (int i = 0; i < sensor_number; i++){
		pinMode(sensor_pin[i], INPUT);
	}

	for (int i = 0; i < 2; i++){
		pinMode(left_motor[i], OUTPUT);
		pinMode(right_motor[i], OUTPUT);
	}
}

//Returns number of sensor
byte pdlf::Robot::sensorNumber(void) const
{
	return Robot::sensor_number;
}

//Robot constructor
pdlf::Robot::Robot(byte *lm, byte *rm, byte *s, byte num_sensor, char mcu)
{
	//Setting motor pins
	for (int i = 0; i < 2; i++){
		left_motor[i] = lm[i];
		right_motor[i] = rm[i];
	}

	if (mcu == 'u' && num_sensor > 6) sensor_number = 6;
	if (mcu == 'm' && num_sensor > 16) sensor_number = 16;

	sensor_number = num_sensor;

	//Allocating memory  
	sensor_pin = new byte[sensor_number];
	weight = new int[sensor_number];
	s_analog_reading = new int[sensor_number];
	s_digital_reading = new byte[sensor_number];
	weighted_value = new int[sensor_number];

	for (int i = 0; i < sensor_number; i++){
		sensor_pin[i] = s[i];
	}

	initializeComponents();

	error = 0;


	kd = 1.0; //was 0
	kp = 18.0; //was 10

  //Initializing bluetooth
  bluetooth = new SoftwareSerial(50, 51);
  bluetooth->begin(9600);
  bluetooth->println("Begin Transmission");
  Serial.begin(9600);
}

void pdlf::Robot::run(int left_speed, int right_speed, Robot::dir left_dir, Robot::dir right_dir)
{
	if (left_dir == Forward && right_dir == Forward){
		analogWrite(left_motor[0], left_speed);
		analogWrite(left_motor[1], 0);
		analogWrite(right_motor[0], right_speed);
		analogWrite(right_motor[1], 0);
	}

	else if (left_dir == Backward && right_dir == Backward){
		analogWrite(left_motor[0], 0);
		analogWrite(left_motor[1], left_speed);
		analogWrite(right_motor[0], 0);
		analogWrite(right_motor[1], right_speed);
	}

	else run(Nowhere);
}

void pdlf::Robot::run(int left_speed, int right_speed, dir direction){
	if (direction == Clockwise){
		analogWrite(left_motor[0], left_speed);
		analogWrite(left_motor[1], 0);
		analogWrite(right_motor[0], 0);
		analogWrite(right_motor[1], right_speed);
	}
	else if (direction == AntiClockwise){
		analogWrite(left_motor[1], left_speed);
		analogWrite(left_motor[0], 0);
		analogWrite(right_motor[0], right_speed);
		analogWrite(right_motor[1], 0);
	}

	else if (direction == Right){
		analogWrite(left_motor[0], left_speed);
		analogWrite(left_motor[1], 0);
		analogWrite(right_motor[0], 0);
		analogWrite(right_motor[1], 0);
	}

	else if (direction == Left){
		analogWrite(left_motor[0], 0);
		analogWrite(left_motor[1], 0);
		analogWrite(right_motor[0], right_speed);
		analogWrite(right_motor[1], 0);
	}
	else if (direction == BackwardLeft){
		analogWrite(left_motor[0], 0);
		analogWrite(left_motor[1], left_speed);
		analogWrite(right_motor[0], 0);
		analogWrite(right_motor[1], 0);
	}

	else if (direction == BackwardRight){
		analogWrite(left_motor[0], 0);
		analogWrite(left_motor[1], 0);
		analogWrite(right_motor[0], 0);
		analogWrite(right_motor[1], right_speed);
	}

	else run(Nowhere);
}

void pdlf::Robot::run(dir direction){

	for (int i = 0; i < 2; i++){
		analogWrite(left_motor[i], 0);
		analogWrite(right_motor[i], 0);
	}
	
}

void pdlf::Robot::updateAnalogRead(void)
{
	for (int i = 0; i < sensor_number; i++){
		s_analog_reading[i] = analogRead(sensor_pin[i]);
	}
}

void pdlf::Robot::updateDigitalRead(void)
{
	updateAnalogRead();
	if (!inverted_logic){
		for (int i = 0; i < sensor_number; i++){
			if (s_analog_reading[i] > THRESHOLD) s_digital_reading[i] = 1;
			else s_digital_reading[i] = 0;
		}
	}
	else {
		for (int i = 0; i < sensor_number; i++){
			if (s_analog_reading[i] < THRESHOLD) s_digital_reading[i] = 1;
			else s_digital_reading[i] = 0;
		}
	}
}


void pdlf::Robot::printAnalog(byte short_del, int long_del){
	updateAnalogRead();
	Serial.println("************* SENSOR ANALOG VALUE BEGIN **********");
	for (int i = 0; i < sensor_number; i++){
		Serial.println("sensor [" + String(i) + "] = " + String(s_analog_reading[i]));
		delay(short_del);
	}
	Serial.println("\n ************* SENSOR ANALOG VALUE END*************");
	delay(long_del);
}

void pdlf::Robot::printDigital(byte del)
{
	updateDigitalRead();
	Serial.println("----------- begin ----------");
	for (int i = 0; i < sensor_number; i++) Serial.print(String(s_digital_reading[i]) + " ");
	Serial.println("\n----------- end ------------");
	delay(del);
}

void pdlf::Robot::generateWeight(int s){

	int n = int(floor(s / 2));
	Serial.println(n);
	if (s % 2 == 0){
		int index = 0;
		for (int i = -n; i < 0; i++, index++) weight[index] = i;
		for (int i = 1; i <= n; i++, ++index) weight[index] = i;
	}
	else if (s%2 != 0) {
		int index = 0;
		for (int i = -n; i <= 0; i++, index++) weight[index] = i;
		for (int i = 1; i <= n; i++, index++)
			weight[index] = i;
	}
}

void pdlf::Robot::printWeight(void)
{
	Serial.println("-------- weight begin ---------");
	for (int i = 0; i < sensor_number; i++){
		Serial.print(String(weight[i]) + " ");
	}
	Serial.println("\n ------- weight end ----------");
}

void pdlf::Robot::updateWeightedValue(void)
{
	updateDigitalRead();
	for (int i = 0; i < sensor_number; i++){
		weighted_value[i] = weight[i] * s_digital_reading[i];
	}
}

void pdlf::Robot::printWeightedValue(void)
{
	updateWeightedValue();
	Serial.println("-------- weighted value ------");
	for (int i = 0; i < sensor_number; i++){
		Serial.print(String(weighted_value[i]) + " ");
	}
	Serial.println("\n------ end weighted value ------");
}

int pdlf::Robot::weightedValueSum(void) 
{
	updateWeightedValue();
	int x = 0;
	for (int i = 0; i < sensor_number; i++){
		x += weighted_value[i];
	}
	return x;
}

void pdlf::Robot::pdLineFollow(void)
{
	int perfect_value = 2;

	previous_error = error;
	error = perfect_value - weightedValueSum();

	int first_weighted_position = positionTracker();
	int second_weighted_position = positionTracker();

	if (first_weighted_position != second_weighted_position) bluetooth->println(second_weighted_position);

  //Added new lines to print weighted value to bluetooth
  //works but slow bluetooth->println(sum_of_weighted_value);

  //bluetooth->println(weightedValueSum());

	double add_value = kp * error + kd * (previous_error - error);

	if (add_value == 0.0) run(global_speed, global_speed, Forward, Forward);

	else if (add_value < 0.0){
		run(global_speed, global_speed + add_value, Forward, Forward);
	}
	
	else if (add_value > 0.0){
		run(global_speed - add_value, global_speed, Forward, Forward);
	}

  //Print stops to stop collecting data
    int count = 1;
	while (weightedValueSum() == 0){
    count--;
    if (count == 0){
      bluetooth->println("stop");
    }
		run(Nowhere);
	}

}

//Returns the numeric value of the position of the sensor array and hence position of the robot

int pdlf::Robot::positionTracker(void)
{
	updateDigitalRead();
	int active_sensors = 0;
	int weighted_reading = 0;
	for (int i = 0; i < sensorNumber() ; i++){
		weighted_reading += s_digital_reading[i] * i * 10;
		if (s_digital_reading[i] == 1) active_sensors++;
	}
	if (weighted_reading) return weighted_reading / active_sensors;
	else return 0;
}