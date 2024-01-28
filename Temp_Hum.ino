float tempHum() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  float x = temperature * price + humidity * price_h;
  
  return x;

  // BTserial.print("H:");
  // BTserial.print(humidity);
  // BTserial.print(" T:");
  // BTserial.println(temperature);
}
