#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001"; //These must be the same in both transmitter and receiver

const int device_address = 0x48; //this is the address to the temperature meter
const int temperature_register = 0x00; // This is the address to the temperature register


void setup() {
  // put your setup code here, to run once:
  
  Wire.begin();
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}




double read_temp_sensor(void) { //This function is called to read a value from the temperature sensor using I2C

  uint8_t data[2];
  int16_t datac;
  
  Wire.beginTransmission(device_address); //Connect to temperature meter
  Wire.write(temperature_register); // read from temperature register
  Wire.endTransmission();

  //delay(5); // give some prosess time

  Wire.requestFrom(device_address, 2);
  if (Wire.available() <= 2) {
    data[0] = Wire.read();
    data[1] = Wire.read();
    
    datac = ((data[0] << 8 | data[1]));

    return datac*0.0078125;

  }
  else {
    return 404; // just an easy way to indicate error
    }

  
  
  }

void loop() {
  // put your main code here, to run repeatedly:
  
  double temp = read_temp_sensor(); //
  if (temp != 404) {
  radio.write(&temp, sizeof(temp));
  }
  else {
    const char text[] = "ERROR";
    radio.write(&text, sizeof(text));
    }
    
  //const char text[] = "Hello World";
  //radio.write(&text, sizeof(text));
  delay(500);
}
