#include <dht.h>

#define INTERNAL_TIME 100      //try to get_down the text commands about 100ms 
#define INTERNAL_TIME2 5000   //props temperature and huimidity timely about every 5s 

//strings from MIIO
String miioString = "";
boolean miioStringComplete = false;

//pins of rgb
int redPin = 9;
int greenPin = 10;
int bluePin = 11;
//pins of DHT
int dhtPin = 4;
dht DHT;

long startTime,currentTime,startTime2,currentTime2;
void setup()
{
    Serial.begin(115200);
    //init rgb
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    setColor(0,0,0);
    
    startTime = millis();
    currentTime = millis();
    startTime2 = millis();
    currentTime2 = millis();
    
}
void loop()
{
    //get comand from cloud
    currentTime = millis();
    if(currentTime - startTime > INTERNAL_TIME)
    {
        startTime = currentTime;
        Serial.println("get_down");
        if(miioStringComplete) stringsHandle();
    }
    //props temperature and huimidity timely.
    currentTime2 = millis();
    if(currentTime2 - startTime2 > INTERNAL_TIME2)
    {
        startTime2 = currentTime2;
        propDHT();
        if(miioStringComplete) stringsHandle();
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
    if(miioString.substring(5,8).equals("rgb"))
    {
        setRGB(miioString.substring(9));
    }
    miioString = "";
    miioStringComplete = false;
}
//props temperature or humidy
void propDHT()
{
    int chk = DHT.read11(dhtPin);
    switch (chk)
    {
    case 0:
        Serial.print("props temperature ");
        Serial.println(DHT.temperature,1);
        Serial.print("props humidity ");
        Serial.println(DHT.humidity,1);
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
    String valueString = "";
    int r = 0;
    int g = 0;
    int b = 0;

    //get values of r,g,b
    int len = str.length();
    int cntComma = 0;
    for(int i = 0; i < len; ++i)
    {
        if(str[i] == ',')
        {
            cntComma ++;
            if(cntComma == 1)
            {
                r = valueString.toInt();
                valueString = "";
            }
            else if(cntComma == 2)
            {
                g = valueString.toInt();
                valueString = "";
            }
        }
        else if(isDigit(str[i]))
        {
            valueString += str[i];
        }
    }
    b = valueString.toInt();

    //set values of r,g,b
    setColor(r,g,b);
}
void setColor(int red, int green, int blue)
{
    analogWrite(redPin, red);
    analogWrite(greenPin,green);
    analogWrite(bluePin, blue);
}
