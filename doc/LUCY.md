LUCY
=================

##目录结构
在此以源代码目录为当前目录进行说明

*	**src/**：Lucy的源代码目录
*	**res/**：Lucy所需要的资源文件目录
*	**src/libadb**：Lucy所依赖的adb源代码目录

##编码风格
Lucy的编码风格继承自GTK+。这里做简单介绍。

*	**文件名**：全小写，不用任何字符分割，如lcapplicationview.c
*	**函数名**：全小写，用下划线分割，如lc_protocol_application_new
*	**变量名**：全小写，用下划线分割，如user_data
*	**类型名**（包括结构体、枚举等）：首字母大写，用大写字母分割，如LcApplication。
*	宏定义：一般全大写，如果希望宏看上去像函数，可以用函数名的命名方式。
>与Lucy相关的所有函数、类型名都是以lc、Lc或者LC开头的

##源代码
这里介绍Lucy的源代码结构。Lucy采用了GTK+所使用的面向对象方式。着下面的表述中我将使用类这个词。.[c,h]表示同一个名称下后缀分别为.c和.h的两个文件。

*	**libadb/**：该目录下面是adb的源代码，是从android项目中分离出来的，在这里被打包成了一个静态库的形式。一般不会需要更改。

*	**lucy.c**：Lucy的主函数入口，主要创建Lucy的主窗口。如果以adb为名字来执行Lucy，那么Lucy的行为将与一个adb完全一样。
>    if (g_strcmp0(argv[0], "adb") == 0) {
        /*
         * if argv[0] is "adb", lucy runs as adb
         */
        return adb_commandline(argc - 1, argv + 1);
    }

*	**lcmainwindow**.[c,h]：主窗口类，继承GtkWindow。主界面以及菜单栏，同时它协调各个子界面。

*	**lcapplicationrow**.[c,h]：显示一条应用信息的类，继承GtkEventBox。显示应用的图标、名称、类型等信息。

*	**lcapplicationview**.[c,h]：显示所有应用的类，继承GtkScrolledWindow。可滚动，包含了多个LcApplicationRow。

*	**lcmyphone**.[c,h]：显示“我的手机”的界面，继承GtkStack。可以切换显示内容，未连接显示“Connect”按钮，连接完成显示用户的Android设备信息。

*	**lctoolstack**.[c,h]：统一管理不同界面元素。负责不同界面的切换，比如LcApplicationView界面和LcMyPhone界面的切换。

*	**lcprotocol**.[c,h]：定义了Lucy和Lily通信相关的协议结构，以及数据解析。

*	**lcaboutdialog**.[c,h]：“关于”菜单。显示Lucy的信息。

*	**lcutil**.[c,h]：一些“实用”函数。

*	**lcadb**.[c,h]：adb函数的异步调用封装。adb本是一个命令行程序，所有函数调用都是同步阻塞的。使用GTask进行异步执行并回调。

*	**lcsocket**.[c,h]：处理Lucy和Lily通信的套接字，保证通信正确；注意，通信正确是表示接收到了完整的数据（不多不少），至于数据本事是正确还是错误的并不处理。

*	**lccommander**.[c,h]：处理Lucy和Lily的通信，它维护一个LcSocket的列表，如果列表中有空闲的LcSocket则使用，否则创建新的LcSocket进行通信。同时LcCommander会合并一些组合请求，比如初始化链接*lc_commander_init_async*。

*	**lcinstalldialog**.[c,h]：一个对话框，提示用户选择一个本地的APK文件安装到Android系统中。

*	**其他**：src/目录下可能还有一些其他文件，很可能是临时的，或者是未完成的代码，也可能是没有被添加到本文中的。如果对此有疑问，请联系相关人员。

##代码层次
1. lucy.c为函数入口创建主界面LcMainWindow
2. LcMainWindow创建菜单栏以及LcToolStack
3. LcToolStack管理LcMyPhone和LcApplicationView
4. LcApplicationView包含多个LcApplicationRow
5. 通过LcAdb进行adb可以完成的操作，比如安装和卸载程序。
6. 不能通过adb完成的操作需要与Lily通信，使用LcCommander。
7. LcCommander调用LcAdb和LcSocket。