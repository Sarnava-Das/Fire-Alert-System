#include <DHT.h> //library for temperature and humidity sensor(DHT11 sensor)
#include <LedControl.h>//library for led 8*8 matrix 


#define DHTPIN 2 // Define the pin to which the sensor is connected
#define DHTTYPE DHT11 // Change this to DHT11 if you're using that sensor

DHT dht(DHTPIN,DHTTYPE); //function definiton

// Define the pins for the MAX7219 module
#define DIN_PIN 9  // Data in
#define CS_PIN 10   // Chip select
#define CLK_PIN 8  // Clock

// Create an instance of the LedControl library
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);  

byte zero[]   = {B00111100, B01000010, B01000010, B00111100};
byte one[]    = {B00000000, B01000010, B01111110, B01000000};
byte two[]    = {B01100010, B01010010, B01001010, B01000110};
byte three[]  = {B00100010, B01000010, B01001010, B00110100};
byte four[]   = {B00011000, B00010100, B00010010, B01111111};
byte five[]   = {B00100111, B01000101, B01000101, B00111001};
byte six[]    = {B00111100, B01001010, B01001010, B00110000};
byte seven[]  = {B00000011, B01100001, B00010001, B00001111};
byte eight[]  = {B00110110, B01001001, B01001001, B00110110};
byte nine[]   = {B00100110, B01000101, B01000101, B00111110};



//pins for led,buzzer
const int redLed = 13; 
const int greenLed = 12                   ;
const int buzzer = 11;

//pins for smoke,flame sensors
const int smoke_A0 = A0;
const int flamepin=A2;

const int MQ2threshold =100;//  threshold value for MQ2
const int flamethreshold = 100;//  threshold value for flame sensor

const int relayPin = 6; // Pin connected to the relay control


void setup() {
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relayPin,OUTPUT);

  pinMode(smoke_A0, INPUT);
  pinMode(flamepin, INPUT);

  Serial.begin(9600);
  dht.begin();

  // Initialize the MAX7219 module
  lc.shutdown(0, false);  // Wake up the MAX7219
  lc.setIntensity(0, 7); // Set the brightness (0-15)
  lc.clearDisplay(0);     // Clear the display
}

void loop() {
   //Wait a few seconds between measurements
   //delay(2000);
 
 
  int smoke_sensor_value = analogRead(smoke_A0);
  int flame_sensor_value =analogRead(flamepin);

 
  // Read humidity and temperature from the sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Print the sensor data to the serial monitor
  Serial.print("Smoke:");
  Serial.println(smoke_sensor_value);
  
  Serial.print("Flame:");
  Serial.println(flame_sensor_value);

  Serial.print("Temperature:");
  Serial.println(temperature);

  Serial.print("Humidity:");
  Serial.println(humidity);


//function calls for checking sensor values with threshold
 check_smoke(smoke_sensor_value);
  check_flame(flame_sensor_value);
 
 

  if (!isnan(temperature))
       // Display temperature on the LED matrix
    displayTemperature(temperature);
  delay(1000);
  

  

 
}
//function to check smoke sensor
void check_smoke(int smoke) 
{
  // Checks if it has reached the threshold value
if (smoke > MQ2threshold)
  {
    digitalWrite(relayPin,HIGH);
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(buzzer,HIGH);
    delay(1000);
   
  }
  else
  {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(buzzer,LOW);
    digitalWrite(relayPin,LOW);
  }
}
//function to check flame sensor
void check_flame(int flame) 
{
if (flame<flamethreshold) 
  { // compares reading from flame sensor with the threshold value
    digitalWrite(redLed,HIGH); 
    digitalWrite(greenLed, LOW);
    digitalWrite(buzzer,HIGH);
    digitalWrite(relayPin,HIGH);
    delay(1000);
   
  }
  else
  {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(buzzer,LOW);
    digitalWrite(relayPin,LOW);
    //delay(1000);
  }
}

// Function to display a temperature on the LED matrix
void displayTemperature(float temp) {
  int tempInt = int(temp);  // Extract the integer part of the temperature
  int tensDigit = tempInt / 10;
  int onesDigit = tempInt % 10;
  switch (tensDigit) {
    case 0: for (int c=0;c<4;c++) { lc.setRow(0,c,zero[c]);} break;
    case 1: for (int c=0;c<4;c++) { lc.setRow(0,c,one[c]);} break;
    case 2: for (int c=0;c<4;c++) { lc.setRow(0,c,two[c]);} break;
    case 3: for (int c=0;c<4;c++) { lc.setRow(0,c,three[c]);} break;
    case 4: for (int c=0;c<4;c++) { lc.setRow(0,c,four[c]);} break;
    case 5: for (int c=0;c<4;c++) { lc.setRow(0,c,five[c]);} break;
    case 6: for (int c=0;c<4;c++) { lc.setRow(0,c,six[c]);} break;
    case 7: for (int c=0;c<4;c++) { lc.setRow(0,c,seven[c]);} break;
    case 8: for (int c=0;c<4;c++) { lc.setRow(0,c,eight[c]);} break;
    case 9: for (int c=0;c<4;c++) { lc.setRow(0,c,nine[c]);} break;
  }
  int k=0;
  switch (onesDigit) {
    case 0: for (int c=4;c<8;c++) { lc.setRow(0,c,zero[k]); k=k+1;} break;
    case 1: for (int c=4;c<8;c++) { lc.setRow(0,c,one[k]); k=k+1;} break;
    case 2: for (int c=4;c<8;c++) { lc.setRow(0,c,two[k]); k=k+1;} break;
    case 3: for (int c=4;c<8;c++) { lc.setRow(0,c,three[k]); k=k+1;} break;
    case 4: for (int c=4;c<8;c++) { lc.setRow(0,c,four[k]); k=k+1;} break;
    case 5: for (int c=4;c<8;c++) { lc.setRow(0,c,five[k]); k=k+1;} break;
    case 6: for (int c=4;c<8;c++) { lc.setRow(0,c,six[k]); k=k+1;} break;
    case 7: for (int c=4;c<8;c++) { lc.setRow(0,c,seven[k]); k=k+1;} break;
    case 8: for (int c=4;c<8;c++) { lc.setRow(0,c,eight[k]); k=k+1;} break;
    case 9: for (int c=4;c<8;c++) { lc.setRow(0,c,nine[k]); k=k+1;} break;
  }
}





