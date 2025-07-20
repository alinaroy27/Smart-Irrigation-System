#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,2);
int sensor_pin = A0;
int sensor_ctrl_pin=3;
int relay_pin=7;
float moisture_content;
float sensor_threshold=1024;
String s="";
int led=8;
void setup() 
{
  
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setBacklight(HIGH);
  pinMode(sensor_pin,INPUT);
  pinMode(sensor_ctrl_pin,OUTPUT);
  pinMode(relay_pin, OUTPUT);
  pinMode(led, OUTPUT);
  
}

void loop() 
{

  digitalWrite(relay_pin,HIGH);
  digitalWrite(sensor_ctrl_pin,HIGH);
  int sensor_data=analogRead(sensor_pin);
  Serial.print("Sensor Data");
  Serial.print(sensor_data);
  moisture_content=((sensor_threshold-sensor_data)/sensor_threshold)*100;
  
  String str=(String)moisture_content;
  s=str +"% moist" ;
  
  Serial.print("\t | ");
  digitalWrite(sensor_ctrl_pin,LOW);

  if (sensor_data < 300)
  {
    digitalWrite(led,LOW);
    Serial.println("Soil is Wet");
     //digitalWrite(relay_pin,LOW);

    lcd.setCursor(0,0);
    lcd.print(s);
    delay(20);
    lcd.setCursor(0,1); 
    lcd.print("Soil is wet      ");
    lcd.clearWriteError();
    
    

  }
  else if(sensor_data >=300 && sensor_data<=750)
  { 
    digitalWrite(led,LOW);
    Serial.println("There is some moisture,soil is partly wet");
    // digitalWrite(relay_pin,LOW);
    lcd.setCursor(0,0);
    lcd.print(s);
    delay(20);
    lcd.setCursor(0,1); 
    lcd.print("Soil is semi wet");
    lcd.clearWriteError();

    
    

  }
  else if(sensor_data > 750)
  { 
    digitalWrite(led,HIGH);
    Serial.println("Soil is Dry           ");
    lcd.setBacklight(LOW);
    delay(20);
    digitalWrite(relay_pin,LOW);
    delay(500);
    digitalWrite(relay_pin,HIGH);
    lcd.init();
    lcd.clear();         
    lcd.backlight();
    lcd.setBacklight(HIGH);
    lcd.setCursor(0,0);
    lcd.print(s);
    delay(20);
    lcd.setCursor(0,1);
    lcd.print("Soil is Dry                ");
    delay(5000);

     lcd.clearWriteError();
    
  }

  delay(3000);
  
}
