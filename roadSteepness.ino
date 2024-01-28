
float roadsteepness() {
 
  mpu6050.update();

  
  float angle_x = mpu6050.getAngleX();
  
  
  if (millis() - startTime >= 1000) {
    if(angle_x+20<0){
  angle_x=-20;
 }
    // Your code here
  total_steepness=total_steepness+20+angle_x;
  startTime=millis();
  countSteep=countSteep+1;
//  Serial.println(angle_x);
//  Serial.println("average: "+String(total_steepness/countSteep));
  }
  
//  delay(100);
  return total_steepness/countSteep;
  
  
}
