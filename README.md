|时间|	内容|	作者|
|----|---	|	----|
|2014年10月10日|	拟定初版	|苏本昌
------		
###1.背景介绍：
MCU可以通过串口与MIIO1.0连接，使用文本命令进行交互。MIIO1.0通过WiFi与云端互联。这样MCU可以通过MIIO1.0 实现与云端的交互。

该demo以Arduino为例，展示了Arduino通过MIIO1.0芯片与云端进行交互的过程。

###2.交互过程：
(1)下行过程:  Cloud->MIIO->Arduino->Device

Arduino 通过UART从MIIO芯片获取云端命令，并执行命令，对Device(比如RGB LEDS)进行操作。

(2) 上行过程:  Device->Arduino->MIIO->Cloud

Arduino 通过UART 从MIIO芯片向云端上报Device(比如DHT11)的状态。

###3.过程示例:
(1)下行过程示例：
```seq
Cloud->MIIO:rgb [0,0,8]
MIIO->Arduino:down rgb 0,0,8
```




>   a.云端向MIIO下发命令来设置RGB: rgb  [0,0,8] 
>   b.MIIO将云端发来的包含method的json串解析为文本命令，供Arduino获取
>   c.Arduino 对MIIO每隔100ms发送文本命令，来获取云端命令：get_down 
>   d.Arduino获取从MIIO串口发来的rgb文本命令: down rgb 0,0,8
>   e.Arduino执行命令，设置RGB LEDS，将蓝灯点亮。
>   f.整个过程为：Cloud->MIIO1.0->Arduino->RGB LEDS

> (2)上行过程示例：

>   a.Arduino 每隔5s检测DHT11的状态(即温度和湿度) 
>   b.Arduino将获取的DHT11状态，以文本命令形式通过串口传给MIIO:
     props temperature 24      props humidity 56
>   c.MIIO将温度和湿度放入json串，上报云端
>   d.整个过程为：DHT11->Arduino->MIIO1.0->Cloud

> 注：更多详细文本命令见文档《MIIO芯片接口交互设计20140904.docx》

###4.硬件连线:
> (1)连接Arduino与PC：使用USB线连接(用来供电、烧写代码)

> (2)连接Arduino与MIIO芯片： Arduino的UART(pin 1 ,pin 0 ,pin GND)->MIIO 的UART1(UA1_RXD, UA1_TXD,GND)

> (3)连接Arduino 与DHT11 ：Arduino的5V ,pin 4,GND->DHT11的V,D,G

> (4)连接Arduino 与 RGB LEDS: Arduino的pin 9,pin 10,pin 11,GND->RGB LEDS的R,G,B,V-

###5.烧写运行:
> (1)下载Arduino软件 (下载地址：http://arduino.cc/en/Main/Software/).

> (2)导入库文件：dht.cpp 和 dht.h 
   a.库文件下载地址：https://github.com/xuemulong/miio_tc_demo
   b.库文件导入方法：http://arduino.cc/en/Guide/Libraries)

> (3)用Arduino软件烧写代码：miio_tc_demo.ino
   a.烧写代码时断开Arduino与MIIO的串口连接，烧写完成后重新连接
   b.代码下载地址：https://github.com/xuemulong/miio_tc_demo

> (4) MIIO 上电启动运行、快连进入网络；在云端下发命令、查看上报信息。
