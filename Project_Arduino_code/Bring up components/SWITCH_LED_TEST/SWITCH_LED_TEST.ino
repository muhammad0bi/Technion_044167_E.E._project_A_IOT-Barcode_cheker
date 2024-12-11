
const int redLed = 18; //IO 18
const int greenLed = 21; //IO 21
const int golden_switch = 36;
const int optional_switch = 35;
const int work_switch = 34;

 const int delayTime = 1000;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(redLed,OUTPUT);
pinMode(greenLed,OUTPUT);
pinMode(golden_switch,INPUT);
pinMode(optional_switch,INPUT);
pinMode(work_switch,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
if (digitalRead(golden_switch) == LOW)
{
  Serial.println("golden_switch pressed");
}
if (digitalRead(optional_switch) == LOW)
{
  Serial.println("optional_switch pressed");
}
if (digitalRead(work_switch) == LOW)
{
  Serial.println("work_switch pressed");
}

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
