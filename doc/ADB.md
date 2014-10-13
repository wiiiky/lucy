ADB
====
本文介绍lucy中是如何 使用adb完成所需要的功能的。
Google官方对adb的介绍：[ADB](https://developer.android.com/tools/help/adb.html) 

##TCP FORWARD
adb提供了一个tcp forward的功能。使用方式是

		adb forward <local> <remote>

比如

		adb forward tcp:38766 tcp:12345

该功能必须在Android设备正常链接下才可以使用。

成功执行上述命令后，向本地TCP端口38766发起的链接，将被重定向到所连接的Android设备中的TCP端口12345。

也就是说当Android设备上有应用监听TCP端口12345时，它将接收到向PC主机TCP端口38766发起的连接。

另一方面，PC主机上往127.0.0.1:38766发起的连接，其实连接到了Android设备中监听12345端口的应用程序上。

##TCP FORWARD IN LUCY
lucy和lily正是通过这种tcp forward机制实现了套接字连接，并可以像一般TCP套接字一样读写，交换数据。

注意，在tcp forward中，本地PC上的端口号，和Android系统上的端口号是可以一致的，而且为了方便。在lucy中都使用37859

		adb forward tcp:37859 tcp:37859

##CONNECTION
在这里简单说明lucy和lily的连接过程，这里只讲解正常的流程。

当lucy要连接lily时，他先启动adb服务器。

然后设置tcp forward

		adb forward tcp:37859 tcp:37859

接着调用adb功能启动Android系统上的lily，

		adb shell am start org.wl.ll/.MainActivity

如果启动成功，说明lily早已安装。lily启动的时候会监听TCP端口。

如果没有成功启动lily，那么就调用

		adb install lily.apk

将lily先安装到系统中。然后启动lily。

lily启动后，lucy会向lily发起一个tcp连接，连接成功后发送第一个请求

		version\n

lily返回它的当前版本号，如果和lucy所知道的匹配，那么就认为lucy和lily建立了链接。