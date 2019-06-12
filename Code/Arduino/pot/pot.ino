#include <Servo.h>
Servo myservo;
int potpin = 0;
int val;
int limit;
int a=0;
int b=0;

void setup()
{
Serial.begin(9600);
myservo.attach(3);
pinMode(10,INPUT); 
pinMode(11,INPUT); 

//pinMode(13,OUTPUT);

//digitalWrite(13, LOW);
}


void loop()
  {
    a = digitalRead(10);
    b = digitalRead(11);
  //  digitalWrite(13, LOW);
    
    if(a==LOW and b==HIGH)
      {
        val = analogRead(A0);
        val = map(val, 0, 1023, 0, 90); 
        myservo.write(val);
        
        Serial.print("\n");
        Serial.print("Tracking");
    //    digitalWrite(13, LOW);  
      }
        else if(a==HIGH and b==HIGH)
          {
            val = analogRead(A0);
            val = map(val, 0, 1023, 0, 0); 
            myservo.write(val);
            Serial.print("\n");
            Serial.print("Limiting");
      //      digitalWrite(13, HIGH);
          }
            else
              {
                val = analogRead(A0);
                val = map(val, 0, 1023, 0, 0); 
                myservo.write(val);
                Serial.print("\n");
                Serial.print("Limiting");
        //        digitalWrite(13, HIGH);
              }
  }
