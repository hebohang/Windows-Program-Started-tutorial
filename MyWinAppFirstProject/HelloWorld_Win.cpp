// 按照 D3D12 龙书附录A编写的代码
// P699页
// 详述 Direct3D 编程所需的 Win32 最简代码

// 包含windows头文件；其中有编写 Windows 应用程序所需的所有 Win32 API 结构体、数据类型以及函数的声明
#include <Windows.h>

// 用于指认所创建主窗口的句柄
// HWND 类型表示“某个窗口的句柄”。在 Windows 编程中，我们通常采用 Windows 系统在内部为每个对象维护的句柄来处理相应的对象。
// 在这个示例中，我们使用的就是 Windows 系统为应用程序主窗口维护的 HWND 句柄
HWND ghMainWnd = 0;

// 封装初始化 Windows 应用程序所需的代码。如果初始化成功，该函数返回true，否则返回false
bool InitWindowsApp(HINSTANCE instanceHandle, int show);

// 封装消息循环代码
int Run();

// 窗口过程会处理窗口所接收到的消息
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 在Windows应用程序中 WinMain 函数就相当于大多数语言中的 main() 函数
// 补充说明：因为命令行带参数 /SUBSYSTEM:WINDOWS，程序的 startup code 函数为 WinMainCRTStartup ，所以执行 WinMain 函数；
// 而一般的函数的 startup 函数为 mainCRTStartup ，执行 main 函数。 参见官网：
// https://docs.microsoft.com/en-us/cpp/build/reference/entry-entry-point-symbol?redirectedfrom=MSDN&view=msvc-160
// hInstance：当前应用程序的实例句柄；pCmdLine：运行此程序所用的命令行参数字符串；nCmdShow：此参数指定了应用程序该如何显示
// WINAPI 被宏定义，实际上为 __stdcall

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	// 首先将 hInstance 和 nCmdShow 作为参数来调用封装函数（ InitWindowsApp ）,
	// 用以创建和初始化应用程序的主窗口
	if (!InitWindowsApp(hInstance, nCmdShow))
		return 0;

	// 一旦将创建好的应用程序初始化完毕后，我们就可以开启消息循环了。随后，消息循环就会持续运转，
	// 直至接收到消息 WM_QUIT ————这表示此应用程序被关闭，该终止运行了
	return Run();
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{
	// 第一项任务便是通过填写 WNDCLASS 结构体，并根据其中描述的特征来创建一个窗口
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW; // 指定了窗口类的样式。这里是 CS_HREDRAW 和 CS_VREDRAW 两种样式的组合
	wc.lpfnWndProc = WndProc; // 指向与此 WNDCLASS 实例相关联窗口过程函数的指针。
	wc.cbClsExtra = 0; // 现在编写的程序不需要这额外的空间，因此将 cbClsExtra 和 cbWndExtra 设置为 0
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle; // 当前应用实例的句柄
	wc.hIcon = LoadIcon(0, IDI_APPLICATION); // 可以通过这个参数为以此窗口类创建的窗口提供指定一个图标的句柄。这句话采用的是默认的应用程序图标
	wc.hCursor = LoadCursor(0, IDC_ARROW); // 可以通过这个参数指定在光标略过窗口工作区时所呈现的样式的句柄。这里采用的是标准的“箭头”光标
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 指出画刷（brush）的句柄，以此指定窗口工作区的背景颜色。这里通过 GetStockObject 函数返回一个内置的白色画刷句柄。
	wc.lpszMenuName = 0; // 指定窗口的菜单。由于该示例没有菜单，设置为 0
	wc.lpszClassName = L"BasicWndClass"; // 指定所创建窗口类结构体的名字。可以随意填写，有了名字后，我们就可以在后续需要此窗口类结构体的时候方便地引用它

	// 下一步，我们要在 Windows 系统中为上述 WNDCLASS 注册一个实例，这样一来，即可据此创建窗口
	// 所以这里是通过 RegisterClass 函数将它注册到 Windows 系统，这个函数以指向欲注册的 WNDCLASS 结构体的指针为参数。若注册失败则返回 0
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}

	// 注册过 WNDCLASS 实例之后，我们就能用 CreateWindow 函数来创建一个窗口了
	// 此函数返回的就是所建窗口的句柄（一个 HWND 类型的数值）。 如果创建失败，返回的句柄数值为0.
	// 窗口句柄是一种窗口的引用方式，Windows 系统会在内部对此进行管理。
	// 处理窗口的大量 Win32 API 函数都需要把 HWND 类型作为参数，以此来确定要对哪一个窗口执行相应的操作

	ghMainWnd = CreateWindowW(
		L"BasicWndClass", // 创建此窗口采用的是前面注册的 WNDCLASS 实例
		L"Win32Basic", // 窗口的标题，它将显示在窗口的标题栏中
		WS_OVERLAPPEDWINDOW, // 窗口的样式标志
		CW_USEDEFAULT, // 窗口左上角的初始位置在于屏幕坐标系中的 x 坐标
		CW_USEDEFAULT, // 窗口左上角的初始位置在于屏幕坐标系中的 y 坐标
		CW_USEDEFAULT, // 以像素为单位表示的窗口宽度。这里指定为 CW_USEDEFAULT ，Windows会自动选择适当的默认值
		CW_USEDEFAULT, // 以像素为单位表示的窗口高度。这里指定为 CW_USEDEFAULT ，Windows会自动选择适当的默认值
		0, // 所创建窗口的父窗口句柄。这里由于创建的窗口不具有父窗口，故设置为 0
		0, // 菜单句柄。由于我们的程序无需菜单，因此将它设置为 0
		instanceHandle, // 与此窗口相关联的应用程序实例句柄
		0 // 一个指向用户定义数据的指针，可用作 WM_CREATE 消息的 lpParam 参数。
	); // 可在此设置一些创建窗口所用的其他参数

	if (ghMainWnd == 0)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	// 尽管窗口已经创建完毕，但仍然没有显示出来。因此，最后一步便是调用下面的两个函数，将刚刚创建的窗口显示出来并对它进行更新。
	// 可以看出，我们为这两个函数都传入了窗口句柄，这样一来，它们就知道需要展示以及更新的窗口是哪一个
	// 这两个函数也是 InitWindowsApp 函数中为了显示窗口而必须调用的两种函数。
	// show 其实是 WinMain 的传入参数 nCmdShow 。其实就是给 ShowWindow 函数传入一个定义着窗口初次显示模式（例如最小化、最大化）的整数值。
	// UpdateWindow 就是对展示的窗口刷新。 参数是欲更新窗口的句柄。
	ShowWindow(ghMainWnd, show);
	UpdateWindow(ghMainWnd);

	return true;
}

// 程序的核心：消息循环。 在这个程序中，消息循环被封装在一个名为 Run 的函数内
int Run()
{
	// 第一件事：为表示 Windows 消息的 MSG 类型创建一个名为 msg 的变量实例。（详见龙书P708）
	MSG msg = { 0 };

	// 接下来，程序进入到消息循环部分。 GetMessage 函数会从消息队列中检索消息，并根据截获的消息细节填写 msg 的参数。
	// 在获取 WM_QUIT 消息之前，该函数会一直保持循环。GetMessage 函数只有在收到 MW_QUIT 消息时才会返回0（false），这会造成循环终止；
	// 而若发生错误，它便会返回 -1 . 还需注意的一点是，在未有消息到来之时，GetMessage 函数会令此应用程序线程进入休眠状态
	BOOL bRet = 1;
	while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			MessageBox(0, L"GetMessage FAILED", L"Error", MB_OK);
		}
		else
		{
			// 如果 GetMessage 得到的值不是 -1（代表着错误）也不是 0（代表着接收到 MW_QUIT），那么将继续执行下面的两个函数
			// TranslateMessage 函数实现了键盘按键的转换，特别是将虚拟键消息转换为字符消息
			// DispatchMessageW 函数则会把消息分派给对应的窗口过程
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return (int)msg.wParam;
}

// 我们在窗口过程中编写的代码是针对窗口接收到的消息而进行相应的处理。这个程序中我们将其命名为 WndProc
// LRESULT 其实是一个整数，一层一层往里看宏的定义，其在 WIN64 下是 long
// CALLBACK 标识符指明这是一个回调函数。意味着 Windows 系统会在此程序的代码空间之外调用该函数。
// 就像这个程序一样，我们从没有主动显示地调用这个窗口过程，这是因为 Windows 系统会在需要处理消息的时候自动为我们调用此窗口过程（我们在 WNDCLASS 实例中进行过绑定）
// hWnd：接收此消息的窗口句柄；msg：标识特定消息的预定值。预定义的窗口消息有上百种，具体可参考 MSDN 库；
// wParam 和 lParam：与具体消息相关的额外信息。MSDN 库为每一种 Windows 消息都罗列出了对应的 wParam 和 lParam 参数信息
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 处理一些特定的消息。注意，在处理完一个消息之后，我们应当返回 0
	switch (msg)
	{
		// WM 其实是 Window Message 的意思
		// 当用户在窗口的工作区点击鼠标左键时，便会发送一次 WM_LBUTTONDOWN 消息。
		// 在按下鼠标左键后，弹出一个消息框。 
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello, World", L"Hello", MB_OK);
		return 0;

		// 当有非键盘键被按下时，就会向具有当前焦点的窗口发送 WM_KEYDOWN 消息
		// 在按下 Esc 键之后，通过 DestroyWindow 函数销毁应用程序的主窗口
		// 此时，传入窗口过程的 wParam 参数即为用户按下特定按键的虚拟键代码（virtual key code），我们可以认为它是特定键的标识符。
		// Windows头文件含有一系列用于确定按键的虚拟键代码。这里检测的是虚拟键代码常量 VK_ESCAPE，便可知晓用户按下的是否为 Esc 键
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(ghMainWnd);
		}
		return 0;

		// 当窗口被销毁时，便会发送 WM_DESTROY 消息
		// 处理销毁消息的方法是发送退出消息，这样一来便会终止消息循环
		// PostQuitMessage 函数（该函数会终止消息循环）发出 WM_QUIT 消息
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	// 将上面没有处理的消息转发给默认的窗口过程。
	// 注意，我们自己所编写的窗口过程一定要返回 DefWindowProc 函数的返回值
	// 根据龙书 P697 页，我想是因为要将目标窗口不处理的消息转发至 DefWindowProc ，让它去完成相应的处理，所以自己编写的窗口过程一定要返回 DefWindowProc 函数的返回值
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
	补充：消息框函数 MessageBox 函数。
	通过源码看到其声明：
	MessageBoxW(
		_In_opt_ HWND hWnd, 
		_In_opt_ LPCWSTR lpText,
		_In_opt_ LPCWSTR lpCaption,
		_In_ UINT uType);
	hWnd：该消息框所属窗口的句柄，可以指定为 NULL
	lpText：消息框中显示的文本
	lpCaption：消息框的标题文本
	uType：消息框的样式
	消息框的返回值依赖于所用消息框的具体类型。
*/
