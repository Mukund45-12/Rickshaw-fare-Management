//required libraries
//with time elapse and distance
#include <LiquidCrystal.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <DHT.h>
#include <SD.h>
#include <SPI.h>
// #include <SoftwareSerial.h>
//LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 


//Global variables
//fdsfsad

//time
//new
// int buttonPin = 2;
unsigned long shuruTime = 0;
unsigned long elapsedTime = 0;
bool isTiming = false;
// STOP BUTTON
// const int stopButtonPin = 6;
// bool stopButtonPressed = false; 

//new
float minutes = 0;
uint32_t prevTime;
uint32_t startTime;
//LCD
int adc_key_in = 0;

//wheelRotation // IR SENSOR
int irPin = 2;
int count = 0;
// WHEEL CIRCUMFERENCE
const float wheelCircumference = 0.67;
// WHEEL ROTATION
float new_wheelRotation=0;
// INITIALIZES THE VARIABLES
float previousDistance = 0.0;
float currentDistance = 0.0;
float totalDistance = 0.0;
float distance = 0.0;
// int distance = 0;       // distance traveled by cycle
// int previousDistance = 0; // previous distance
// int count = 0;          // number of revolutions
// int circumference = 200;  // circumference of the wheel in mm (you may need to adjust this value for your wheel size)
// unsigned long previousMillis = 0;  // previous time
// const float WHEEL_CIRCUMFERENCE = 2.0; //kilometer
// // // for showing the distance travelled
// unsigned int kmCounter = 0;
// float distanceDurotto = 0.0;
//  const int LEFT_BUTTON = 4;

//Temp_Hum

#define DHTPIN 3

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float new_tempHum = 0;
File myFile;
const int chipSelect = 10;

float temperature, humidity;
float sumT = 0;
float sumAvg = 0;
float sumH = 0;
float sumAvg_h = 0;
float price = 0.02; //change this for temperature //for max 35 degree C
float price_h = 0.01; //change this for humidity //for max 80% Humidity
int current_ride=0;
float x = -1;
float state = 0;

//int ride=0;
//windSpeed

//const int IR_sensor_pin = 10;  // pin for the IR sensor
//float new_avg_wind_speed = 0;

//roadSteepness

MPU6050 mpu6050(Wire);
float new_steepness = 0;
float total_steepness=0;
float countSteep=0;

//fare

int initial_start_flag=true;

int startButton=true;
int stopButton=true;
int startFlag=false;

// SoftwareSerial BTserial(15,16); // RX | TX

void setup() {

  
  
  
  // start serial communication
  Serial.begin(9600);
  
  //sd card code starts
  while (!Serial) {
    ; 
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1); 
  }
  Serial.println("card initialized.");
  // Serial.println("Ready to connect to Bluetooth module!");
  // BTserial.begin(9600);
  
  // configureHM10();
  
  //sd card code ends
  lcd.begin(16, 2);     // start the library
  lcd.setCursor(0, 0);  // set the LCD cursor position
  lcd.print("Fare Meter");
  // pinMode(buttonPin, INPUT_PULLUP); //new
  pinMode(irPin, INPUT_PULLUP);
  // pinMode(stopButtonPin, INPUT_PULLUP);

    // Display the initial distance on the LCD screen
  lcd.setCursor(0, 1);
  lcd.print("Distance: ");
  lcd.print(0);
  lcd.print(" km");
  // pinMode(irPin, INPUT);
//  SD.begin(chipSelect);
//wheelRotatiion
//  pinMode(irPin, INPUT);
//tempHum
  
  dht.begin();


//windSpeed
//  pinMode(IR_sensor_pin, INPUT);  // set the IR sensor pin as an input

  //  roadSteepness
  Wire.begin();
  mpu6050.begin();
  mpu6050.setGyroOffsets(-0.3,0.15, 0.03);// set this value for gyro and calibrate it after installation
//    mpu6050.calcGyroOffsets(true);
//
  prevTime = millis();
  startTime = millis();
   

}

 

void loop() {

   // listen for data from the Bluetooth module and print it to the Serial Monitor
 
  // unsigned long new_wheel_rotation = wheelRotation();

  // hm10Serial.println("AT+NAMEMyModule");
  adc_key_in = analogRead(0);
  if(initial_start_flag==true){
    initial_start_flag=false;
    prevTime = millis();
    }
   
  // btnRIGHT:  if the right btn is pressed
//  Serial.println("adc"+String(adc_key_in));
//  Serial.println("millis"+String(millis()));
//  Serial.println("prevTime"+String(prevTime));
  delay(50);
//  if (adc_key_in < 50 && millis() - prevTime > 100) {
if (adc_key_in < 50 ) {
    startTime = millis();
    startFlag=true;
     
    Serial.println("start");
  }
 //if start button is pressed then enter this if statement
if(startFlag==true){
   
//    Serial.println("working");
    lcd.clear();
     
    // lcd.setCursor(0, 0);
    // lcd.print("Meter Running");
    // lcd.setCursor(0, 1);
    
//new
    // if (digitalRead(buttonPin) == LOW && !isTiming) {
    if (!isTiming){
      shuruTime = millis();
      isTiming = true;
  }

    if (isTiming) {
      elapsedTime = millis() - shuruTime;
      int minutes = elapsedTime / 60000;
      int seconds = (elapsedTime / 1000) % 60;
      // Serial.print("Sec");
      // Serial.print(seconds);
      lcd.setCursor(0, 0);
      lcd.print("T.e: ");
      lcd.print(minutes);
      lcd.print(" m  ");
      lcd.print(seconds);
      lcd.print(" s");
       
       

      
    }
     // Check for a magnet passing by the IR sensor
  if (digitalRead(irPin) == LOW) {
    count++;
   }

  // Calculate the distance travelled in kilometers
  float distance = (float)count * wheelCircumference / 1000.0;

    // Calculate the distance travelled since the last measurement
   float distanceTravelled = currentDistance - previousDistance;
    // Add the distance travelled to the total distance
    totalDistance += distanceTravelled;
  
  // Add the distance travelled to the trip distance and total distance
  // if (!stopButtonPressed) {
  //   tripDistance += distanceTravelled;
  //   totalDistance += distanceTravelled;
  // }

  // Update the LCD screen if the distance has changed
    float prevDistance = -1.0;
  if (distance != prevDistance) {
    lcd.setCursor(0, 1);
    lcd.print("Dis: ");
    lcd.setCursor(5, 1);
    lcd.print(distance, 2);
    lcd.print(" km");
  }
  prevDistance = distance;
  delay(100);

 

  // Delay for 100ms before repeating the loop
  // delay(100);
//new

 

    new_wheelRotation = wheelRotation();
//    Serial.println(new_wheelRotation);
    new_tempHum = tempHum();
    new_steepness = roadsteepness();
    startButton=false;
    stopButton=true;
    prevTime = millis();
  }
// Kilometer or Distance travelled
  // int sensorValue = digitalRead(irPin);
  // if (sensorValue == HIGH) {
  //   kmCounter++;
  //   distanceDurotto = kmCounter * WHEEL_CIRCUMFERENCE;
  //   lcd.setCursor(10, 0);
  //   lcd.print(distanceDurotto);
  //   lcd.print(" m       ");
  // }

  // if (LEFT_BUTTON == LOW) {
  //   // Display the distance on the LCD keypad shield
  //   lcd.setCursor(10, 0);
  //   lcd.print(distanceDurotto);
  //   lcd.print(" m       ");
  // }
  // delay(10);
  

  //btnLEFT: if the left button is pressed

//  if (adc_key_in > 80 && adc_key_in < 650 && millis() - prevTime > 1000) {
//if stop button is pressed then enter this if statement
  if (adc_key_in > 450 && adc_key_in < 650  && stopButton==true &&startFlag==true) {
    Serial.println("stop");
    startFlag=false;
    x = 0;
    state = 0;
    sumT = 0;
    sumAvg = 0;
    sumH = 0;
    countSteep=0;
    sumAvg_h = 0;
    startButton=true;
    stopButton=false;
    isTiming = false;
     
    total_steepness=0;
//    Serial.println("left");
    elapsedTime = millis() - shuruTime;
    fare();
   

  //  if (digitalRead(buttonPin) == LOW){
     float distance = (float)count * wheelCircumference / 1000.0;
    

    prevTime = millis();
    
    

   
    count = 0;
     

    myFile = SD.open("test.txt", FILE_WRITE);
    if (myFile) {
      Serial.println("time"+String(elapsedTime)+",");
    
      myFile.print(","+String(elapsedTime/60000)+",");
      // myFile.print("time");
      myFile.close();
      Serial.println("Wrote to file test.txt");
     } else {
      Serial.println("error opening test.txt");
     }

      myFile = SD.open("test.txt", FILE_WRITE);
     if (myFile) {
      Serial.println("distance"+String(distance)+",");
      myFile.print(String(distance));
      myFile.println();
      // myFile.print("distance");
      myFile.close();
      Serial.println("Wrote to file test.txt");
     } else {
      Serial.println("error opening test.txt");
     }
    lcd.setCursor(9, 0);
    // lcd.print("Distance: ");
    lcd.print(distance, 2);
    lcd.print(" km");
    // // reset the count and distance for the next trip
     
    lcd.setCursor(11, 1);
    lcd.print(elapsedTime / 60000);
    lcd.print(" m");
   
 

   

  // Delay for 10 milliseconds to avoid double-counting
  // delay(10); } 
  }
  if (adc_key_in > 250 && adc_key_in < 350 && millis() - prevTime > 1000 ) {
   
   
  String lineDown;
  myFile = SD.open("test.txt");
//  String rideDown="";
//  String rideNew="";
  if (myFile) {
//    while (myFile.available()) {
//      lineDown = myFile.readStringUntil('\n');
//      int comma = lineDown.indexOf(',');
//      String part1 = lineDown.substring(0, comma);
//      rideNew=part1;
//    }
//    current_ride=rideNew.toInt()+1;
    
    while (myFile.available()) {
      lineDown = myFile.readStringUntil('\n');
       
      int commaIndex1 = lineDown.indexOf(',');
    int commaIndex2 = lineDown.indexOf(',', commaIndex1 + 1);
    int commaIndex3 = lineDown.indexOf(',', commaIndex2 + 1);
    String part1 = lineDown.substring(0, commaIndex1);
    String part2 = lineDown.substring(commaIndex1 + 1, commaIndex2);
    String part3 = lineDown.substring(commaIndex2 + 1, commaIndex3);
    String part4 = lineDown.substring(commaIndex3 + 1);
    
      if(current_ride-1==part1.toInt()){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ride:"+String(part1)+"    D:"+String(part4)+" ");
        lcd.setCursor(0, 1);
        lcd.print("Fare :"+String(part2)+" T:"+String(part3));
        lcd.setCursor(10, 1);
        lcd.print(" ");//do not touch this
      }
     
    }
    if(current_ride>0){
      current_ride=current_ride-1;
      }
    
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }

    
    Serial.println("down");
    prevTime = millis();
  
  }
  
  if (adc_key_in > 80 && adc_key_in < 200 && millis() - prevTime > 1000 ) {
    String lineUp;
  myFile = SD.open("test.txt");
 
  if (myFile) {
    int countMaxRide=0;
    while (myFile.available()) {
      lineUp = myFile.readStringUntil('\n');
       
      int commaIndex1 = lineUp.indexOf(',');
    int commaIndex2 = lineUp.indexOf(',', commaIndex1 + 1);
    int commaIndex3 = lineUp.indexOf(',', commaIndex2 + 1);
    String part1 = lineUp.substring(0, commaIndex1);
    String part2 = lineUp.substring(commaIndex1 + 1, commaIndex2);
    String part3 = lineUp.substring(commaIndex2 + 1, commaIndex3);
    String part4 = lineUp.substring(commaIndex3 + 1);
      
      if(current_ride+1==part1.toInt()){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ride:"+String(part1)+"    D:"+String(part4)+" ");
        lcd.setCursor(0, 1);
        lcd.print("Fare: "+String(part2)+" T:"+String(part3));
        lcd.setCursor(10, 1);
        lcd.print(" ");//do not touch this
      }
      countMaxRide=countMaxRide+1;
    }
    if(current_ride<countMaxRide-1){
      current_ride=current_ride+1;
      }

    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
    
    Serial.println("up");
    prevTime = millis();
  }

  // Send the AT command to set the module to advertising mode
   
  

  // Wait for the module to respond
  delay(100);
  
  
 

}