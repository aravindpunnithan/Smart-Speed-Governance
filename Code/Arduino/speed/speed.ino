#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//#include <Servo.h>

//Servo myservo;
//int potpin = 0;
//int val;

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

int spd = 0;
int limit = 50;
int a=0;
int b=0;
int c=0;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  //myservo.attach(5);
  pinMode(7,INPUT);
  pinMode(8,INPUT); 
  pinMode(9,INPUT); 
  
  pinMode(10,OUTPUT); 
  pinMode(11,OUTPUT);
  pinMode(13,OUTPUT);
  

  digitalWrite(10,LOW);
  digitalWrite(11, LOW);
  digitalWrite(13, LOW); 

  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
  {
    gps.encode(ss.read());
    if (gps.location.isUpdated())
      {    
      // Latitude in degrees (double)
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat());
      Serial.print("\n");      
      // Longitude in degrees (double)
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng()); 
      Serial.print("\n");
      Serial.print("Speed in km/h = "); 
      Serial.println(gps.speed.kmph());
      spd=(gps.speed.kmph());
      Serial.print("\n\n");
      delay(100);
      }
  }

  a = digitalRead( 7 );
  b = digitalRead( 8 );
  c = digitalRead( 9 );

  if(a==HIGH and b==LOW and c==LOW)
      {      
        limit=20;
        Serial.print("\n");
        Serial.print("Limit Speed = ");
        Serial.print(limit);
        Serial.print("\n");
      
      }
      
      if(a==HIGH and b==LOW and c==HIGH)
        {
            limit=30;
            Serial.print("\n");
            Serial.print("Limit Speed = ");
            Serial.print(limit);
            Serial.print("\n");
            
         }
         
         if(a==HIGH and b==HIGH and c==LOW)
          { 
            limit=40;
            Serial.print("\n");
            Serial.print("Limit Speed = ");
            Serial.print(limit);
            Serial.print("\n");
            
           }
           
            //{
              //limit=60;
            // Serial.print("\n\n");
             //Serial.print("Limit Speed = ");
             //Serial.print(limit);
              //Serial.print("\n\n");
           // }
    
  if (spd<limit)
    { 
      //val = analogRead(potpin);  
      //val = map(val, 0, 1023, 0, 90); 
      //myservo.write(val);
      
      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);
      digitalWrite(13, LOW);

      Serial.print("\n");
      Serial.print("Tracking");
    }


  if (spd>limit)
    { 
      //val = analogRead(potpin);  
      //val = map(val, 0, 1023, 0, 0); 
      //myservo.write(val);
      
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(13, HIGH); 

      Serial.print("\n");
      Serial.print("Limiting");
    }  
  
}
  
    
