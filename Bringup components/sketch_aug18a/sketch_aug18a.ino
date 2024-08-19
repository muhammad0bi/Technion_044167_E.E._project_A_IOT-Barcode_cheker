const int redLed = 17; //IO 17
const int greenLed = 21; //IO 17

 const int delayTime = 1000;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(redLed,OUTPUT);
pinMode(greenLed,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(redLed,HIGH);
digitalWrite(greenLed,HIGH);
delay(delayTime);

digitalWrite(redLed,LOW);
digitalWrite(greenLed,LOW);
delay(delayTime);

digitalWrite(redLed,HIGH);
digitalWrite(greenLed,LOW);
delay(delayTime);

digitalWrite(redLed,LOW);
digitalWrite(greenLed,HIGH);
delay(delayTime);
}
