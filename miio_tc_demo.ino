#include <dht.h>

#define INTERNAL_GET_DOWN 100      //try to get_down the text commands about 100ms 
#define INTERNAL_CHECK_DHT 1000   //check temperature and huimidity timely about every 1s 
#define INTERNAL_PROP_DHT 300000//props temperature and humidity timely every 5min
#define INTERNAL_BUTTON_PRESS 200 //if button is pressed more than this
#define INTERNAL_BUTTON_LONG_PRESS 4000 //if button is pressed more than this
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

//model & mcu_version sended flag
int model_sended = 0;
int mcu_version_sended = 0;

//count timer
long startTime,currentTime,startTime2,currentTime2,startTime3,currentTime3,startTime4,currentTime4;
void setup()
{
    Serial.begin(115200);
    //init rgb
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    setColor(0,0,0);
    
    //init button
    pinMode(buttonPin,INPUT);
   
    //init timer    
    startTime = millis();
    startTime2 = millis();    
    startTime3 = millis();
    startTime4 = millis(); 
}
void loop()
{
//**************send model and mcu_version**********************************************************
    if(model_sended < 2){//hasn't received ack
      if(model_sended < 1){//hasn't sended
         Serial.println("model xiaomi.demo.v2");
         model_sended = 1;//sended
      }
       if(miioStringComplete) {
         if(0 == stringsHandle()) model_sended = 2;//received ack
         else model_sended = 0;
      }
    }
    if(mcu_version_sended < 2){//hasn't received ack
      if(mcu_version_sended < 1){//hasn't sended
          Serial.println("mcu_version 0002");
          mcu_version_sended = 1;//sended
      }
      if(miioStringComplete) {
        if(0 == stringsHandle()) mcu_version_sended = 2; //received ack
        else mcu_version_sended = 0;
      }
    }
//***************************************************************************************************    

//****************get command from cloud**************************************************************
    currentTime = millis();
    if(currentTime - startTime > INTERNAL_GET_DOWN){
        startTime = currentTime;
        Serial.println("get_down");
        if(miioStringComplete) stringsHandle();
    }
//***************************************************************************************************     

//********************check temperature and huimidity timely. if changed,props*************************
    currentTime2 = millis();
    if(currentTime2 - startTime2 > INTERNAL_CHECK_DHT){
        startTime2 = currentTime2;
        checkDHT();
    }
//****************************************************************************************************    

//*******************props temperature and humidity timely********************************************
    currentTime3 = millis();
    if(currentTime3 - startTime3 > INTERNAL_PROP_DHT){
        startTime3 = currentTime3;
        propDHT();
        if(miioStringComplete) stringsHandle();
    }
//****************************************************************************************************

//********************report event.button_pressed or event.button_long_pressed**********************
    currentTime4 = millis();
    if(digitalRead(buttonPin)){//button is pressed now
        if(!button_is_pressed){//detect button is pressed first time
          button_is_pressed = 1;
          startTime4 = currentTime4;
          has_reported = 0;
          has_reported_long = 0;
        }
        else{
          if(currentTime4 - startTime4 > INTERNAL_BUTTON_PRESS  && !has_reported){
          Serial.println("event button_pressed "); 
          has_reported = 1;
          }
          if(currentTime4 - startTime4 > INTERNAL_BUTTON_LONG_PRESS && !has_reported_long){
            Serial.println("event button_long_pressed "); 
            has_reported_long = 1;
          }
       }
    }
    else{//button is not pressed now
     button_is_pressed = 0;
     startTime4 = currentTime4; 
    }
//****************************************************************************************************
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
int stringsHandle()
{
    int ret = 0;
    if(miioString.substring(5,13).equals("get_prop"))
    {
      int rst = 0;
      int index = miioString.indexOf('\"') + 1;
      while(index){
        if(miioString.substring(index,index + 3).equals("rgb")){
           if(rst == 0)Serial.print("result ");
           Serial.print(rgbValue,1);
           Serial.print(" ");
           rst ++;
         }
         
        if(miioString.substring(index,index + 8).equals("humidity")){
           if(humValue < 0){
             rst = -1;
             break;
           }
           if(rst == 0)Serial.print("result ");
           Serial.print(humValue,1);
           Serial.print(" ");
           rst ++;
         }
         
         if(miioString.substring(index,index + 11).equals("temperature")){
           if(tmpValue < 0){
             rst = -1;
             break;
           }
           if(rst == 0)Serial.print("result ");
           Serial.print(tmpValue,1);
           Serial.print(" ");
           rst ++;
         }
         index = miioString.indexOf('\"',index) + 1;
         index = miioString.indexOf('\"',index) + 1;
      } 
      if (rst == 0)Serial.println("error \"not found\" -5003");
      else if(rst == -1)Serial.println("error \"get failed\" -5002");
      else Serial.println("");
    }
    else if(miioString.substring(5,12).equals("set_rgb"))
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
        Serial.print("result ");
        Serial.println(tmpValue,1);
    }
    else if(miioString.substring(5,17).equals("get_humidity"))
    {
        Serial.print("result ");
        Serial.println(humValue,1);
    }
    else if(miioString.substring(0,2).equals("ok")){
        ret = 0; 
    }
    else if(miioString.substring(0,5).equals("error")){
        ret =  -1; 
    }
    miioString = "";
    miioStringComplete = false;
    
    return ret;
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
      else{
        tmpValue = -1;
        humValue = -1;
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
