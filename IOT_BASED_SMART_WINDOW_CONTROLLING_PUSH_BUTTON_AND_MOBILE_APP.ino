#define BLYNK_TEMPLATE_ID "TMPL2kGSbJFEz"
#define BLYNK_TEMPLATE_NAME "IoT Window For Blinds System"
#define BLYNK_AUTH_TOKEN "60ZIeuVxG2-jJnz_FuZ-pswZ7NnR-z23"

//#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Stepper.h> // Include the header file
#include <LiquidCrystal_I2C.h> // need to install
LiquidCrystal_I2C lcd(0x27, 16,2); // 0x27 is the i2c address might different;you can check with Scanner
#define STEPS 64

Stepper stepper(STEPS, D4, D6, D7, D3);
boolean closed = false;
boolean opened = true; 
void disable_motor() //turn off the motor when done to avoid heating

{
  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);
//  digitalWrite(redled,LOW);
//  digitalWrite(buzzer,LOW);
}


//#define  yellowled D10
#define  redled D0
#define  buzzer D8
#define buttonpin D5
int buttonread;
int ledstate =0;
int ledstate1 = 0;
int dt = 500;
int buttonOld = 0;
// You should get Auth Token in the Blynk App.
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SETSOM";   // your ssid 
char pass[] = "614444243"; // your pass

BlynkTimer timer;

#define VPIN_BUTTON_1    V0

BLYNK_WRITE(VPIN_BUTTON_1) {
  int toggleState_1 = param.asInt();
  if(toggleState_1 == 1){
    Serial.println("Opening Blinds");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("...IOT SYSTEM....");
    lcd.setCursor(0,1);
    lcd.print("Blind is Opening......");
    if (closed == true)
    {
    for (int cc_val = 0; cc_val >= -130; cc_val--) //rotate in Clockwise for opening
    {
      digitalWrite(redled,HIGH);
      digitalWrite(buzzer,HIGH);
      stepper.step(cc_val);
      yield();
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("...IOT SYSTEM....");
    lcd.setCursor(0,1);
    lcd.print("Window is Opened......");
    digitalWrite(redled,LOW);
    digitalWrite(buzzer,LOW);
    opened = true;
    closed = false;
    }
    disable_motor(); // always desable stepper motors after use to reduce power consumption and heating
  }
  else { 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("...IOT SYSTEM....");
    lcd.setCursor(0,1);
    lcd.print("Blind is Closing......");
    if (opened == true)
    {
    for (int c_val = 0; c_val <= 130; c_val++) //rotate in Counter-Clockwise for closing
    {
      digitalWrite(redled,HIGH);
      digitalWrite(buzzer,HIGH);
      stepper.step(c_val);
      yield();
    }
    digitalWrite(redled,LOW);
    digitalWrite(buzzer,LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("...IOT SYSTEM....");
    lcd.setCursor(0,1);
    lcd.print("Window is Closed......");
    closed = true;
    opened = false;
    disable_motor(); // always desable stepper motors after use to reduce power consumption and heating
    }
  }
}


void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  lcd.begin();
//  lcd.init();
  lcd.backlight();
 stepper.setSpeed(500);
 pinMode(buttonpin, INPUT);
 pinMode(redled,OUTPUT);
// pinMode(yellowled,OUTPUT);
 pinMode(buzzer,OUTPUT);
//digitalWrite(yellowled,HIGH);
  // Setup a function to be called every second
  timer.setInterval(100L, buttoninside);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IoT Blinds Window");
  lcd.setCursor(0,1);
  lcd.print("Monitoring System");
  delay(2000);
}

void loop()
{
  digitalWrite(redled,LOW);
  digitalWrite(buzzer,LOW);
//buttoninside();
  Blynk.run();
  timer.run();
  buttonOld = buttonread;
  delay(dt);
}

void buttoninside(){
 buttonread = digitalRead(buttonpin);
 Serial.println(buttonread);
 
  if (buttonOld==0 && buttonread==1){   
    if(ledstate==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("...IOT SYSTEM....");
    lcd.setCursor(0,1);
    lcd.print("Blind is Opening......");
    Serial.println("Opening Blinds");
    if (closed == true)
    {
    for (int cc_val = 0; cc_val >= -130; cc_val--) //rotate in Clockwise for opening
    {
      digitalWrite(redled,HIGH);
      digitalWrite(buzzer,HIGH);
      stepper.step(cc_val);
      yield();
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("...IOT SYSTEM....");
    lcd.setCursor(0,1);
    lcd.print("Window is Opened......");
    digitalWrite(redled,LOW);
    digitalWrite(buzzer,LOW);
    opened = true;
    closed = false;
    }
    disable_motor(); // always desable stepper motors after use to reduce power consumption and heating
    ledstate=1;
    }

//-----------------    
    else if (ledstate1 ==0){
    Serial.println("Closing Blinds");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("...IOT SYSTEM....");
    lcd.setCursor(0,1);
    lcd.print("Blind is Closing......");
    if (opened == true)
    {
    for (int c_val = 0; c_val <= 130; c_val++) //rotate in Counter-Clockwise for closing
    {
      digitalWrite(redled,HIGH);
      digitalWrite(buzzer,HIGH);
      stepper.step(c_val);
      yield();
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("...IOT SYSTEM....");
    lcd.setCursor(0,1);
    lcd.print("Window is Closed......");
    digitalWrite(redled,LOW);
    digitalWrite(buzzer,LOW);
    closed = true;
    opened = false;
    disable_motor(); // always desable stepper motors after use to reduce power consumption and heating
    ledstate1=1;
    }
   }
    else {
    digitalWrite(redled,LOW);
    digitalWrite(buzzer,LOW);
      ledstate=0;
      ledstate1=0;
    } 
}
}
