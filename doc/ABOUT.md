概况
=============

##Lucy & Lily
本项目主要分为两个部分，运行在linux主机上的是Lucy，运行在Android设备上的是Lily（不要问我为什么取这两个名字）。
Lucy和Lily互相通信完成所需的功能。

*	Lucy采用GTK+/C开发，集成了adb的源代码。关于Lucy的详细信息请看文件LUCY.md

*	Lily是一般的Android应用程序，负责为Lucy“采集”所需要的数据。关于Lily的详细信息请看文件LILY.md

*	Lucy和Lily通过adb提供的TCP端口重定向实现USB连接。关于adb的介绍请看文件ADB.md

*	Lucy和Lily通过一个协议进行数据交换，关于该协议的详细信息请看PROTOCOL.md文件

##更新
>Wiky L(wiiiky@yeah.net)	Friday, 10. October 2014 11:13PM 
