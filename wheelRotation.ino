float wheelRotation() {
  int counter = digitalRead(irPin);
//  Serial.println("ir"+String(counter));
  if (state == 0) {
    switch (counter) {
      case 1:
        state = 1;
        x = x + 1;
        break;
      case 0: state = 0; break;
      
      
    }
  }
 
  if (counter == LOW) {
    state = 0;
  }
 
//  delay(20);
  return x;
}
