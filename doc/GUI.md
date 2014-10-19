GUI
===
本文介绍lucy的界面结构。

##LcMainWindow
LcMainWindow继承自GtkWindow。它用GtkBox包含了两个子控件；一个是菜单栏GtkMenuBar，一个是LcToolStack。

LcMyAndroid和LcApplicationView被放置到LcToolStack上，由LcToolStack控制两者的切换。

##LcToolStack
LcToolStack继承自GtkGrid。它分为两个部分，一个是上部分的GtkToolbar，一个是下部分的GtkStack。

GtkStack是一个可以切换界面的控件。GtkToolbar中包含多个GtkToolButton。GtkStack界面切换通过GtkToolbar中的按钮控制。点击不同按钮切换到不同界面。

##LcMyAndroid
LcMyAndroid继承自GtkStack。它分为两个界面，一个是“未连接”界面，一个是连接完成显示Android设备信息的界面。显示哪个界面由当前连接状态决定。

“未连接”界面的容器是GtkGrid。目前只有一个Connect按钮，以及一个背景图片，背景图片通过GTK+的CSS引擎设置。res/css/disconnected.css。

Android设备信息界面容器也是GtkGrid。目前只是添加了几个GtkLabel显示手机品牌、手机型号和手机号码。

##LcApplicationView
LcApplicationView继承自GtkStack，它应该也可以显示多个类型的界面，不过目前只有一个界面，就是应用列表界面。应用列表界面的根容器是一个GtkGrid，上面放置了一个GtkToolbar和GtkScrolledWindow，前者用于显示一些控制按钮，后者用滚动形式显示应用列表。GtkScrolledWindow内部用一个GtkGrid容器管理大量LcApplicationRow。每个LcApplicationRow表示一个Android应用。

##LcApplicationRow
LcApplicationRow继承自GtkEventBox。内部使用一个GtkGrid来管理各个子控件。比如显示图标的GtkImage，显示应用名称、应用版本的GtkLabel等。


##更新
Wiky L(wiiiky@yeah.net)

Sunday, 19. October 2014 09:39PM 
