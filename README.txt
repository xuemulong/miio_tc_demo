  该demo旨在展示MCU与MIIO 芯片使用文本命令进行交互的过程。
1.芯片:
  MCU这里选用Arduino(Mega2560、UNO R3等)为例，MIIO芯片为MIIO 1.0.

2.功能: 
(1) 下行过程:  Cloud->MIIO->MCU->Device
    该demo展示了Arduino 如何通过UART从MIIO芯片获取云端命令，并执行命令，对Device(比如RGB LEDS)进行操作。 
(2) 上行过程:  Device->MCU->MIIO->Cloud
    该demo展示了Arduino 如何通过UART 从MIIO芯片向云端上报Device(比如DHT11)的状态。 

3.连线:
(1) 连接Arduino与PC：使用USB线连接(用来供电、烧写代码)
(2) 连接Arduino与MIIO芯片： Arduino的UART(pin 1 ,pin 0 ,pin GND)->MIIO 的UART1(UA1_RXD, UA1_TXD,GND)
(3) 连接Arduino 与DHT11 ：Arduino的5V ,pin 4,GND->DHT11的V,D,G
(4) 连接Arduino 与 RGB LEDS: Arduino的pin 9,pin 10,pin 11,GND->RGB LEDS的R,G,B,V- 

4.运行:
(1) 下载Arduino软件 (下载地址：http://arduino.cc/en/Main/Software/).
(2) 导入库文件：dht.cpp 和 dht.h 
    a.库文件下载地址：https://github.com/xuemulong/miio_tc_demo
    b.库文件导入方法：http://arduino.cc/en/Guide/Libraries)
(3) 用Arduino软件烧写代码：miio_tc_demo.ino
    a.烧写代码时断开与MIIO的串口连接，烧写完成后重新连接
    b.代码下载地址：https://github.com/xuemulong/miio_tc_demo 
(4) MIIO 上电启动运行、快连进入网络；在云端下发命令、查看上报信息。 

5.举例: 
(1)下行过程示例：    
   a.在云端向MIIO下发命令来设置RGB: rgb  [0,0,8] 
   b.MIIO将云端发来的包含method的json串解析为文本命令，供Arduino获取
   c.Arduino 对MIIO每隔100ms发送“get_down”文本命令，获取从MIIO串口发来的rgb文本命令“down rgb 0,0,8”，执行命令，操作RGB LEDS，将蓝灯点亮。
   d.整个过程描述为：Cloud->MIIO->MCU->Device 
(2)上行过程示例：    
   a.Arduino 每隔5s检测DHT11的状态(即温度和湿度)
   b.Arduino将获取的DHT11状态，以文本命令形式通过串口传给MIIO:
     props temperature 24      props humidity 56
   c.MIIO将温度和湿度放入json串，上报云端
   d.整个过程描述为：Device->MCU->MIIO->Cloud
