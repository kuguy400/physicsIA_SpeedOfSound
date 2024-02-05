//distance in meters that the reflection surface is away from the ultrasonic sensor

float distance = 3;
const int trigPin = 11;
const int echoPin = 10;

float durationMS, durationS, speed;
//---------------------------------------------------------------------------------------------------------------------------------------------

//Temperature and humidity set up.
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

float temperature;
float humidity;

//---------------------------------------------------------------------------------------------------------------------------------------------
#include "IRremote.h"
int receiver = 8;
IRrecv irrecv(receiver);  

//--------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);    
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  if (irrecv.decode()) // have we received an IR signal?
  {
    translateIR();
    irrecv.resume(); // receive the next value
  }

}

void formating(){
  getTemp();
  getTime();
  Serial.println();
}

void getTemp(){
  if( measure_environment( &temperature, &humidity ) == true )
  {
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C |  H = " );
    Serial.print( humidity, 1 );
    Serial.print( "% | " );
  }else{
    Serial.print("No temp + humidity value could be gathered | ");
  }
}

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}


void translateIR() // takes action based on IR code received
{
  //map the IR code to the remote key
  switch (irrecv.decodedIRData.decodedRawData)
  {
    case 0xBA45FF00: formating(); break;
    
    default:
      Serial.println("Press the power button to record data.");
  }// End Case
  delay(500); // Do not get immediate repeat
}

void getTime(){
  digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);  

  durationMS = pulseIn(echoPin, HIGH); 
  durationS = durationMS/1000000;
  speed = 2*distance/durationS;
  Serial.print("Time = ");
  Serial.print(durationS);
  Serial.print("s | Speed = ");
  Serial.print(speed);
  Serial.print(" m/s | ");



}