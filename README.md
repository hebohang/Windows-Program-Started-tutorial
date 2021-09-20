# Windows-Program-Started-tutorial
Windows编程入门教程

# 前言
主要参考龙书 《DirectX 12 3D 游戏开发实战》 的附录A
为个人的笔记整理

# 流程分析
这里分析的是我们的示例代码 HelloWorld_Win.cpp

程序入口：WinMain，相当于大多数语言中的 main() 函数，内调用两个函数：InitWindowsApp 和 Run，前者是为了初始化，后者封装了消息循环代码，下面来进行分析

1. InitWindowsApp(HINSTANCE instanceHandle, int show)
1.1 为 WNDCLASS（window class，窗口类） 注册一个实例，绑定好窗口过程函数，指定该实例的名字。这个实例也是我们示例程序中的唯一窗口实例，即为主窗口
1.2 通过 RegisterClass 在 Windows 系统中为上述 WNDCLASS 注册一个实例
1.3 通过 CreateWindow 函数来为1.1中的实例创建一个窗口（通过参数指定为1.1步实例的名字）， 返回所创建窗口的句柄。因此这里返回的就是所创建主窗口的句柄。
1.4 通过 ShowWindow 函数与 UpdateWindow 函数来显示与刷新我们这里要展示的主窗口
2. Run()
2.1 为表示 Windows 消息的 MSG 类型创建一个名为 msg 的变量实例
2.2 开始消息循环，通过 GetMessage 函数来从从消息队列中检索消息，并根据截获的消息细节填写 msg 的参数。
2.3 通过 GetMessage 函数的返回值来判断消息类型，这时可以通过 TranslateMessage 函数来实现键盘按键的转换，特别是将虚拟键消息转换为字符消息；通过 DispatchMessageW 函数来把消息分派给对应的窗口过程

个人思考他们之间的联系：
可以看到，消息循环与之前的窗口之间的联系是通过 GetMessage 这个函数获取消息来做到的。这样才把 Run 函数与之前所创立的窗口联系了起来。
除了 GetMessage 函数，还有 PeekMessage 函数等。

消息的处理是这样做到的：
- 消息的真正处理函数是通过窗口过程做到的。
- 而窗口过程被绑定在窗口类实例上。
- 该实例在 CreateWindowW 函数中返回出它对应的句柄，句柄主要可以当作很多函数的参数。
- 通过在消息循环的代码中（这里在 Run 中）的对应函数（这里是 GetMessage ）把消息的一些信息填充到了所创建的消息实例中，其中就包括了该消息对应窗口的句柄。然后通过 DispatchMessageW 函数（参数是该消息实例的地址，这里是 &msg）去分派给该窗口对应的窗口过程
