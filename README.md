miio_tc_demo
============
It's a demo for the mcu communicates with the chip of miio through text commands. 
###1.Chip:
    The mcu here we adopt is Arduino mega and the miio chip is mc200.
###2.Function:
>    With the demo,we show how the arduino to get down cloud text commands from the miio chip through UART1 and to control the RGB leds and set the relay.

>    With the demo ,we show how to update the state of sensors,for example DHT11, to the cloud by the miio chip through UART1.

###3.Connect Lines:
    a.Connect arduino with the miio chip through UART1(pin 18 ,pin 19 and pin GND).
    b.Connect arduino with DHT11 through pin 4(DHT),5V and GND.
    c.Connect arduino with the RGB leds through pin 11(R),pin 10(G),pin 9(B) and GND.
    d.Connect arduino with the relay through pin 3,3.3v and GND.
###4.Run:
    a.Get the software for downloading the code to Arduino.(http://arduino.cc/en/Main/Software/). 
    b.Import the Arduino libary files :dht.cpp and dht.h. 
    c.Download the code to the arduino mega.
###5.Example:
>  Example--down
>>    a.the arduino will try to get down the command every 1s;
>>>     (1)set RGB:       rgb  [0,0,8]
       rgb value is restricted to 0~10 to avoid burned.
>>>    (2)set relay on:  relay  ["on"]
     the param also can be ["off"] to set relay off.
     
>  Example--up
>>  b.the arduino will update the state of DHT11 every 100s;
>>  c.the arduino will try to check the DHT every 10s to make sure if there is an event occurr.

###6.Display：
  You can see the process of communicates on the computer if you connect Arduino with computer through UART0. 
