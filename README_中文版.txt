miio_tc_demo
该demo旨在展示MCU与MIIO 芯片使用文本命令进行交互的过程。

1.芯片:
MCU芯片这里选用Arduino Mega，MIIO芯片为MIIO1.0.

2.功能:
a.该demo展示了Arduino 如何通过UART1 从MIIO芯片获取云端命令，并对RGB LEDs和Relay 进行控制
b.该demo展示了Arduino 如何通过UART1 从MIIO芯片向云端上报传感器(比如DHT11)的状态。

3.连线:
a.连接Arduino与MIIO芯片： UART1(pin 18 ,pin 19 and pin GND).
b.连接Arduino 与DHT11 ：pin 4(DHT),5V and GND.
c.连接Arduino 与 RGB leds ：pin 11(R),pin 10(G),pin 9(B) and GND.
d.连接Arduino 与 Relay ：pin 3,3.3v and GND.

4.运行:
a.下载Arduino软件.(http://arduino.cc/en/Main/Software/).
b.导入库文件：dht.cpp and dht.h.
c.烧写代码：miio_tc_demo.ino.

5.举例:
Example--下行

a.Arduino 每隔100ms 去get_down 一下云端命令;

(1)在云端set RGB: rgb [0,0,8]
   为避免RGB灯被烧坏，rgb的值取值范围最好在0~10
(2)在云端set relay on: relay ["on"]
   参数如果改为 ["off"] 就可以 set relay off.

Example--上行

a.Arduino 每隔10s向云端报告一下DHT的状态;
b.Arduino 每隔5s检查一下DHT的状态，温度是不是过高，是不是上报event

6.调试：
  通过UART0连接PC与Arduino，可以观察交互过程.
