miio_tc_demo
该demo旨在展示MCU与MIIO 芯片使用文本命令进行交互的过程。

1.芯片:
MCU芯片这里选用Arduino Mega，MIIO芯片为MIIO1.0.

2.功能:
a.该demo展示了Arduino 如何通过UART1 从MIIO芯片获取云端命令，并对RGB LEDs和Relay 进行控制
b.该demo展示了Arduino 如何通过UART1 从MIIO芯片向云端上报传感器(比如DHT11)的状态。

3.连线:
a.Connect arduino with the miio chip through UART1(pin 18 ,pin 19 and pin GND).

b.Connect arduino with DHT11 through pin 4(DHT),5V and GND.

c.Connect arduino with the RGB leds through pin 11(R),pin 10(G),pin 9(B) and GND.

d.Connect arduino with the relay through pin 3,3.3v and GND.
4.Run:

a.Get the software for downloading the code to Arduino.(http://arduino.cc/en/Main/Software/).

b.Import the Arduino libary files :dht.cpp and dht.h.

c.Download the code to the arduino mega.
5.Example:

Example--down

a.the arduino will try to get down the command every 1s;

(1)set RGB: rgb [0,0,8]

rgb value is restricted to 0~10 to avoid burned.

(2)set relay on: relay ["on"]

the param also can be ["off"] to set relay off.

Example--up

a.the arduino will update the state of DHT11 every 100s;

b.the arduino will try to check the DHT every 10s to make sure if there is an event occurr.
6.Display：

You can see the process of communicates on the computer if you connect Arduino with computer through UART0.
