#include <dht.h>

#define INTERNAL_TIME 500      //try to get_down the text commands about 1s 
#define INTERNAL_TIME2 5000    //check if temperature is too high about every 5s
#define INTERNAL_TIME3 10000   //props temperature and huimidity timely about every 10s 

#define TEMP_THRESHOLD 32
#define HUMI_THRESHOLD 65

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
//pin of relay
int relayPin = 3;

long startTime,startTime2,startTime3;
long currentTime,currentTime2,currentTime3;

void setup()
{
    Serial.begin(115200);
    Serial1.begin(115200);
    //init rgb
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    setColor(0,0,0);
    pinMode(relayPin,OUTPUT);
    
    startTime = millis();
    currentTime = millis();
    startTime2 = millis();
    currentTime2 = millis();
    startTime3 = millis();
    currentTime3 = millis();
    
}
void loop()
{
    //get comand from cloud
    currentTime = millis();
    if(currentTime - startTime > INTERNAL_TIME)
    {
        startTime = currentTime;
        Serial1.println("get_down");
        Serial.println("#get_down");
        if(miioStringComplete)
        {
            stringsHandle();
        }
    }

    //event if temperature or humidity is too high
    currentTime2 = millis();
    if(currentTime2 - startTime2 > INTERNAL_TIME2)
    {
        startTime2 = currentTime2;
        if(eventDHT())
        {
            if(miioStringComplete)
            {
                stringsHandle();
            }
        }
    }

    //props temperature and huimidity timely.
    currentTime3 = millis();
    if(currentTime3 - startTime3 > INTERNAL_TIME3)
    {
        startTime3 = currentTime3;
        propDHT();
        if(miioStringComplete)
        {
            stringsHandle();
        }
    }
}

//get Strings from MIIO
void serialEvent1()
{
    //receive strings from MIIO
    while (Serial1.available())
    {
        if(Serial1.peek() != '\r')
        {
            miioString += (char)Serial1.read();
            //Serial.println(miioString);
        }
        else
        {
            miioString += (char)Serial1.read();
            miioStringComplete = true;
        }
    }
}
//process strings from MIIO
void stringsHandle()
{
    Serial.println(miioString);
    if(miioString.substring(5,8).equals("rgb"))
    {
        setRGB(miioString.substring(9));
    }
    else if(miioString.substring(5,10).equals("relay"))
    {
        setRelay(miioString.substring(11));
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
        Serial1.print("props temperature ");
        Serial1.println(DHT.temperature,1);
        Serial.print("#props temperature ");
        Serial.println(DHT.temperature,1);
        Serial1.print("props humidity ");
        Serial1.println(DHT.humidity,1);
        Serial.print("#props humidity ");
        Serial.println(DHT.humidity,1);
        break;
    case -1:
        Serial.println("propDHT:Checksum error");
        break;
    case -2:
        Serial.println("propDHT:Time out error");
        break;
    default:
        Serial.println("propDHT:Unknown error");
        break;
    }
}
//generic an event or not
int eventDHT()
{
    int chk = DHT.read11(dhtPin);
    switch (chk)
    {
    case 0:
        if(DHT.temperature > TEMP_THRESHOLD)
        {
            Serial1.print("event so_hot ");
            Serial1.println(DHT.temperature,1);
            Serial.print("#event so_hot ");
            Serial.println(DHT.temperature,1);
            return 1;
        }
        else if(DHT.humidity > HUMI_THRESHOLD)
        {
            Serial1.print("event so_wet ");
            Serial1.println(DHT.humidity,1);
            Serial.print("#event so_wet ");
            Serial.println(DHT.humidity,1);
            return 1;
        }
        break;
    case -1:
        Serial.println("eventDHT:Checksum error");
        break;
    case -2:
        Serial.println("eventDHT:Time out error");
        break;
    default:
        Serial.println("eventDHT:Unknown error");
        break;
    }
    return 0;
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

//setRelay
void setRelay(String str)
{
    String valueString = "";
//Serial.println(str);

    int i = 0;
    int len = str.length();
    while(str[i] != '\"'&& i < len)++i;
    ++i;
    while(str[i] != '\"' && i < len)
    {
        valueString += str[i];
        i++;
    }
//Serial.println(valueString);
    if(valueString.equals("on"))
    {
        digitalWrite(relayPin,HIGH);
        //Serial.println(digitalRead(relayPin));
    }
    else if(valueString.equals("off"))
    {
        digitalWrite(relayPin,LOW);
    }
}
