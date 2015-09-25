//Differential Drive Function

{
//	update_reading();
 read_line();
// println(100, 3000);
	//sensor_weighted_value = new unsigned long int [getSensorNumber()];
	unsigned long int weighted_values = 0;
	unsigned long int total_values = 0;

	int calibrated_pos = 0;

	for (int i = 0; i < getSensorNumber(); i++){
		weighted_values += line[i] * i * 1000;
	}

 total_values = active_sensors;

// Serial.println(weighted_values);
// Serial.println(total_values);

	calibrated_pos = weighted_values / total_values;

//	return calibrated_pos;

  // Calculate calibrated position and experiment for which value speed needs to be changed by how much

  
  if (calibrated_pos >= 3000 && calibrated_pos <= 4000){
    motor(forward, 150, forward, 150);
  } 

  else  if (calibrated_pos >= 2000 && calibrated_pos < 3000){
    motor(forward, 140, forward, 160);
  }

  else if (calibrated_pos >= 1000 && calibrated_pos < 2000){
    motor (backward, 140, forward, 150);
  }

  else if (calibrated_pos > 4000 && calibrated_pos <= 5000){
    motor (forward, 150, backward, 140);
  }
  
  else Stop();

  
//  Serial.println("Calibrated value: " + String(calibrated_pos));
//  delay(100);
}
