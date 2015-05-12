#include <dht.h>

#define INTERNAL_TIME 100      //try to get_down the text commands about 100ms 
#define INTERNAL_TIME2 1000   //check temperature and huimidity timely about every 1s 
#define INTERNAL_TIME3 300000//props temperature and humidity timely every 5min
#define INTERNAL_TIME4 200 //if button is pressed more than this
#define INTERNAL_TIME5 4000 //if button is pressed more than this
//strings from MIIO
String miioString = "";
boolean miioStringComplete = false;

//pins of rgb
int redPin = 9;
int greenPin = 10;
int bluePin = 11;
long rgbValue = 0;

//pins of DHT
int dhtPin = 4;
dht DHT;
double tmpValue=0;
double humValue=0;
//pin of BUTTON
int buttonPin = 7;
int button_is_pressed = 0;
int has_reported = 0;
int has_reported_long = 0;

long startTime,currentTime,startTime2,currentTime2,startTime3,currentTime3,startTime4,currentTime4;
void setup()
{
    Serial.begin(115200);
    //init rgb
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    pinMode(buttonPin,INPUT);
    setColor(0,0,0);
    
    startTime = millis();
    startTime2 = millis();    
    startTime3 = millis();
    startTime4 = millis();
}
void loop()
{
    //get comand from cloud
    currentTime = millis();
    if(currentTime - startTime > INTERNAL_TIME){
        startTime = currentTime;
        Serial.println("get_down");
        if(miioStringComplete) stringsHandle();
    }
    //check temperature and huimidity timely.
    currentTime2 = millis();
    if(currentTime2 - startTime2 > INTERNAL_TIME2){
        startTime2 = currentTime2;
        checkDHT();
    }
    //props temperature and humidity timely
    currentTime3 = millis();
    if(currentTime3 - startTime3 > INTERNAL_TIME3){
        startTime3 = currentTime3;
        propDHT();
        if(miioStringComplete) stringsHandle();
    }
    //report event.button_pressed or event.button_long_pressed
    currentTime4 = millis();
    if(digitalRead(buttonPin)){
        if(!button_is_pressed){
          button_is_pressed = 1;
          startTime4 = currentTime4;
          has_reported = 0;
          has_reported_long = 0;
        }
        else{
          if(currentTime4 - startTime4 > INTERNAL_TIME4  && !has_reported){
          Serial.println("event button_pressed "); 
          has_reported = 1;
        }
          if(currentTime4 - startTime4 > INTERNAL_TIME5 && !has_reported_long){
            Serial.println("event button_long_pressed 4"); 
            has_reported_long = 1;
          }
        }
    }
    else{
     button_is_pressed = 0;
     startTime4 = currentTime4; 
    }
}

//get Strings from MIIO
void serialEvent()
{
    //receive strings from MIIO
    while (Serial.available())
    {
        if(Serial.peek() != '\r')
        {
            miioString += (char)Serial.read();
        }
        else
        {
            miioString += (char)Serial.read();
            miioStringComplete = true;
        }
    }
}
//process strings from MIIO
void stringsHandle()
{
    if(miioString.substring(5,12).equals("set_rgb"))
    {
        setRGB(miioString.substring(13));
    }
    else if(miioString.substring(5,12).equals("get_rgb"))
    {
        Serial.print("result ");
        Serial.println(rgbValue,1);
    }
    else if(miioString.substring(5,20).equals("get_temperature"))
    {
      if(DHT.read11(dhtPin) == 0){
        Serial.print("result ");
        Serial.println(DHT.temperature,0);
      }
      else{
        Serial.println("error \"get failed\" -5002"); 
      }
    }
    else if(miioString.substring(5,17).equals("get_humidity"))
    {
      if(DHT.read11(dhtPin) == 0){
        Serial.print("result ");
        Serial.println(DHT.humidity,0);
      }
      else{
        Serial.println("error \"get failed\" -5002"); 
      }
    }
    miioString = "";
    miioStringComplete = false;
}
void checkDHT(){
     if(DHT.read11(dhtPin) == 0){
          if(fabs(tmpValue - DHT.temperature) >= 1){
            Serial.print("props temperature ");
            Serial.println(DHT.temperature,0);
            tmpValue = DHT.temperature;
          }
          if(fabs(humValue - DHT.humidity) >= 1){
            Serial.print("props humidity ");
            Serial.println(DHT.humidity,0);
            humValue = DHT.humidity;
          }
      }
}
//props temperature or humidy
void propDHT()
{
    int chk = DHT.read11(dhtPin);
    switch (chk)
    {
    case 0:
        Serial.print("props temperature ");
        Serial.println(DHT.temperature,0);
        Serial.print("props humidity ");
        Serial.println(DHT.humidity,0);
        break;
    case -1:
        break;
    case -2:
        break;
    default:
        break;
    }
}
//set RGB color
void setRGB(String str)
{
    long rgb = str.toInt();
   
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;
   
    //set values of r,g,b
    if(setColor(r,g,b)){
     rgbValue = rgb;
     Serial.println("result \"ok\""); 
    }
    else {
     Serial.println("error \"set failed\" -5001"); 
    }
}
int setColor(int red, int green, int blue)
{
    analogWrite(redPin, red);
    analogWrite(greenPin,green);
    analogWrite(bluePin, blue);
    
    return 1;
}
