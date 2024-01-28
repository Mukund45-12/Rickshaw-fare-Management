float fare() {
 
//  uint16_t minutes = timer();   //this indicates the system on time
//  int displayMinute = minutes;  //for displaying the value in lcd
 
  float unit_price=1;//change this value for wheel rotation 
  float fare = new_wheelRotation * 0.042;
 
//  float windFare = 10000 * new_avg_wind_speed;  //additional fare for windSpeed
  float tempHum =  new_tempHum;         //additional fare for temperature and humidity
  
  float steepFare = 0.01 * new_steepness; //change this value for steepness     //additional fare for sttepness

  
  
  float totalFare = tempHum+steepFare+fare ;  //total fare considering all factors
  Serial.println("Temp Hum :"+String(tempHum));
  Serial.println("steepFare :"+String(steepFare));
  Serial.println("Steepness :"+String(new_steepness));
  Serial.println("Wheel :"+String(fare));
  Serial.println("Total :"+String(totalFare));

  //sd card read code starts
  String line;

  myFile = SD.open("test.txt");
  String ride="";
  if (myFile) {
    while (myFile.available()) {
      line = myFile.readStringUntil('\n');
      int commaIndex = line.indexOf(',');
      String part1 = line.substring(0, commaIndex);
      String part2 = line.substring(commaIndex + 1);
      ride=part1;
    }
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
 
//  sd card read code ends
  
  
  //sd card write code starts
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    myFile.print(String(ride.toInt()+1)+","+String(totalFare));
    myFile.close();
    Serial.println("Wrote to file test.txt");
  } else {
    Serial.println("error opening test.txt");
  }
//  sd card write code ends
  current_ride=ride.toInt()+1;
  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("Time: ");
// 
//  lcd.setCursor(7, 1);
//  lcd.print(displayMinute);
 
  lcd.setCursor(0, 0);
  lcd.print("Ride "+String(ride.toInt()+1));

  lcd.setCursor(0, 1);
  lcd.print("Fare: "+String(totalFare));

  // lcd.setCursor(9, 0);
  // lcd.print()
}
