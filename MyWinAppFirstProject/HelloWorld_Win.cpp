// ���� D3D12 ���鸽¼A��д�Ĵ���
// P699ҳ
// ���� Direct3D �������� Win32 ������

// ����windowsͷ�ļ��������б�д Windows Ӧ�ó������������ Win32 API �ṹ�塢���������Լ�����������
#include <Windows.h>

// ����ָ�������������ڵľ��
// HWND ���ͱ�ʾ��ĳ�����ڵľ�������� Windows ����У�����ͨ������ Windows ϵͳ���ڲ�Ϊÿ������ά���ľ����������Ӧ�Ķ���
// �����ʾ���У�����ʹ�õľ��� Windows ϵͳΪӦ�ó���������ά���� HWND ���
HWND ghMainWnd = 0;

// ��װ��ʼ�� Windows Ӧ�ó�������Ĵ��롣�����ʼ���ɹ����ú�������true�����򷵻�false
bool InitWindowsApp(HINSTANCE instanceHandle, int show);

// ��װ��Ϣѭ������
int Run();

// ���ڹ��̻ᴦ���������յ�����Ϣ
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ��WindowsӦ�ó����� WinMain �������൱�ڴ���������е� main() ����
// ����˵������Ϊ�����д����� /SUBSYSTEM:WINDOWS������� startup code ����Ϊ WinMainCRTStartup ������ִ�� WinMain ������
// ��һ��ĺ����� startup ����Ϊ mainCRTStartup ��ִ�� main ������ �μ�������
// https://docs.microsoft.com/en-us/cpp/build/reference/entry-entry-point-symbol?redirectedfrom=MSDN&view=msvc-160
// hInstance����ǰӦ�ó����ʵ�������pCmdLine�����д˳������õ������в����ַ�����nCmdShow���˲���ָ����Ӧ�ó���������ʾ
// WINAPI ���궨�壬ʵ����Ϊ __stdcall

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	// ���Ƚ� hInstance �� nCmdShow ��Ϊ���������÷�װ������ InitWindowsApp ��,
	// ���Դ����ͳ�ʼ��Ӧ�ó����������
	if (!InitWindowsApp(hInstance, nCmdShow))
		return 0;

	// һ���������õ�Ӧ�ó����ʼ����Ϻ����ǾͿ��Կ�����Ϣѭ���ˡ������Ϣѭ���ͻ������ת��
	// ֱ�����յ���Ϣ WM_QUIT �����������ʾ��Ӧ�ó��򱻹رգ�����ֹ������
	return Run();
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{
	// ��һ���������ͨ����д WNDCLASS �ṹ�壬��������������������������һ������
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW; // ָ���˴��������ʽ�������� CS_HREDRAW �� CS_VREDRAW ������ʽ�����
	wc.lpfnWndProc = WndProc; // ָ����� WNDCLASS ʵ����������ڹ��̺�����ָ�롣
	wc.cbClsExtra = 0; // ���ڱ�д�ĳ�����Ҫ�����Ŀռ䣬��˽� cbClsExtra �� cbWndExtra ����Ϊ 0
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle; // ��ǰӦ��ʵ���ľ��
	wc.hIcon = LoadIcon(0, IDI_APPLICATION); // ����ͨ���������Ϊ�Դ˴����ഴ���Ĵ����ṩָ��һ��ͼ��ľ������仰���õ���Ĭ�ϵ�Ӧ�ó���ͼ��
	wc.hCursor = LoadCursor(0, IDC_ARROW); // ����ͨ���������ָ���ڹ���Թ����ڹ�����ʱ�����ֵ���ʽ�ľ����������õ��Ǳ�׼�ġ���ͷ�����
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ָ����ˢ��brush���ľ�����Դ�ָ�����ڹ������ı�����ɫ������ͨ�� GetStockObject ��������һ�����õİ�ɫ��ˢ�����
	wc.lpszMenuName = 0; // ָ�����ڵĲ˵������ڸ�ʾ��û�в˵�������Ϊ 0
	wc.lpszClassName = L"BasicWndClass"; // ָ��������������ṹ������֡�����������д���������ֺ����ǾͿ����ں�����Ҫ�˴�����ṹ���ʱ�򷽱��������

	// ��һ��������Ҫ�� Windows ϵͳ��Ϊ���� WNDCLASS ע��һ��ʵ��������һ�������ɾݴ˴�������
	// ����������ͨ�� RegisterClass ��������ע�ᵽ Windows ϵͳ�����������ָ����ע��� WNDCLASS �ṹ���ָ��Ϊ��������ע��ʧ���򷵻� 0
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}

	// ע��� WNDCLASS ʵ��֮�����Ǿ����� CreateWindow ����������һ��������
	// �˺������صľ����������ڵľ����һ�� HWND ���͵���ֵ���� �������ʧ�ܣ����صľ����ֵΪ0.
	// ���ھ����һ�ִ��ڵ����÷�ʽ��Windows ϵͳ�����ڲ��Դ˽��й���
	// �����ڵĴ��� Win32 API ��������Ҫ�� HWND ������Ϊ�������Դ���ȷ��Ҫ����һ������ִ����Ӧ�Ĳ���

	ghMainWnd = CreateWindowW(
		L"BasicWndClass", // �����˴��ڲ��õ���ǰ��ע��� WNDCLASS ʵ��
		L"Win32Basic", // ���ڵı��⣬������ʾ�ڴ��ڵı�������
		WS_OVERLAPPEDWINDOW, // ���ڵ���ʽ��־
		CW_USEDEFAULT, // �������Ͻǵĳ�ʼλ��������Ļ����ϵ�е� x ����
		CW_USEDEFAULT, // �������Ͻǵĳ�ʼλ��������Ļ����ϵ�е� y ����
		CW_USEDEFAULT, // ������Ϊ��λ��ʾ�Ĵ��ڿ�ȡ�����ָ��Ϊ CW_USEDEFAULT ��Windows���Զ�ѡ���ʵ���Ĭ��ֵ
		CW_USEDEFAULT, // ������Ϊ��λ��ʾ�Ĵ��ڸ߶ȡ�����ָ��Ϊ CW_USEDEFAULT ��Windows���Զ�ѡ���ʵ���Ĭ��ֵ
		0, // ���������ڵĸ����ھ�����������ڴ����Ĵ��ڲ����и����ڣ�������Ϊ 0
		0, // �˵�������������ǵĳ�������˵�����˽�������Ϊ 0
		instanceHandle, // ��˴����������Ӧ�ó���ʵ�����
		0 // һ��ָ���û��������ݵ�ָ�룬������ WM_CREATE ��Ϣ�� lpParam ������
	); // ���ڴ�����һЩ�����������õ���������

	if (ghMainWnd == 0)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	// ���ܴ����Ѿ�������ϣ�����Ȼû����ʾ��������ˣ����һ�����ǵ���������������������ոմ����Ĵ�����ʾ�������������и��¡�
	// ���Կ���������Ϊ�����������������˴��ھ��������һ�������Ǿ�֪����Ҫչʾ�Լ����µĴ�������һ��
	// ����������Ҳ�� InitWindowsApp ������Ϊ����ʾ���ڶ�������õ����ֺ�����
	// show ��ʵ�� WinMain �Ĵ������ nCmdShow ����ʵ���Ǹ� ShowWindow ��������һ�������Ŵ��ڳ�����ʾģʽ��������С������󻯣�������ֵ��
	// UpdateWindow ���Ƕ�չʾ�Ĵ���ˢ�¡� �����������´��ڵľ����
	ShowWindow(ghMainWnd, show);
	UpdateWindow(ghMainWnd);

	return true;
}

// ����ĺ��ģ���Ϣѭ���� ����������У���Ϣѭ������װ��һ����Ϊ Run �ĺ�����
int Run()
{
	// ��һ���£�Ϊ��ʾ Windows ��Ϣ�� MSG ���ʹ���һ����Ϊ msg �ı���ʵ�������������P708��
	MSG msg = { 0 };

	// ��������������뵽��Ϣѭ�����֡� GetMessage ���������Ϣ�����м�����Ϣ�������ݽػ����Ϣϸ����д msg �Ĳ�����
	// �ڻ�ȡ WM_QUIT ��Ϣ֮ǰ���ú�����һֱ����ѭ����GetMessage ����ֻ�����յ� MW_QUIT ��Ϣʱ�Ż᷵��0��false����������ѭ����ֹ��
	// ����������������᷵�� -1 . ����ע���һ���ǣ���δ����Ϣ����֮ʱ��GetMessage ���������Ӧ�ó����߳̽�������״̬
	BOOL bRet = 1;
	while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			MessageBox(0, L"GetMessage FAILED", L"Error", MB_OK);
		}
		else
		{
			// ��� GetMessage �õ���ֵ���� -1�������Ŵ���Ҳ���� 0�������Ž��յ� MW_QUIT������ô������ִ���������������
			// TranslateMessage ����ʵ���˼��̰�����ת�����ر��ǽ��������Ϣת��Ϊ�ַ���Ϣ
			// DispatchMessageW ����������Ϣ���ɸ���Ӧ�Ĵ��ڹ���
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return (int)msg.wParam;
}

// �����ڴ��ڹ����б�д�Ĵ�������Դ��ڽ��յ�����Ϣ��������Ӧ�Ĵ���������������ǽ�������Ϊ WndProc
// LRESULT ��ʵ��һ��������һ��һ�����￴��Ķ��壬���� WIN64 ���� long
// CALLBACK ��ʶ��ָ������һ���ص���������ζ�� Windows ϵͳ���ڴ˳���Ĵ���ռ�֮����øú�����
// �����������һ�������Ǵ�û��������ʾ�ص���������ڹ��̣�������Ϊ Windows ϵͳ������Ҫ������Ϣ��ʱ���Զ�Ϊ���ǵ��ô˴��ڹ��̣������� WNDCLASS ʵ���н��й��󶨣�
// hWnd�����մ���Ϣ�Ĵ��ھ����msg����ʶ�ض���Ϣ��Ԥ��ֵ��Ԥ����Ĵ�����Ϣ���ϰ��֣�����ɲο� MSDN �⣻
// wParam �� lParam���������Ϣ��صĶ�����Ϣ��MSDN ��Ϊÿһ�� Windows ��Ϣ�����г��˶�Ӧ�� wParam �� lParam ������Ϣ
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ����һЩ�ض�����Ϣ��ע�⣬�ڴ�����һ����Ϣ֮������Ӧ������ 0
	switch (msg)
	{
		// WM ��ʵ�� Window Message ����˼
		// ���û��ڴ��ڵĹ��������������ʱ����ᷢ��һ�� WM_LBUTTONDOWN ��Ϣ��
		// �ڰ����������󣬵���һ����Ϣ�� 
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello, World", L"Hello", MB_OK);
		return 0;

		// ���зǼ��̼�������ʱ���ͻ�����е�ǰ����Ĵ��ڷ��� WM_KEYDOWN ��Ϣ
		// �ڰ��� Esc ��֮��ͨ�� DestroyWindow ��������Ӧ�ó����������
		// ��ʱ�����봰�ڹ��̵� wParam ������Ϊ�û������ض���������������루virtual key code�������ǿ�����Ϊ�����ض����ı�ʶ����
		// Windowsͷ�ļ�����һϵ������ȷ����������������롣�����������������볣�� VK_ESCAPE�����֪���û����µ��Ƿ�Ϊ Esc ��
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(ghMainWnd);
		}
		return 0;

		// �����ڱ�����ʱ����ᷢ�� WM_DESTROY ��Ϣ
		// ����������Ϣ�ķ����Ƿ����˳���Ϣ������һ�������ֹ��Ϣѭ��
		// PostQuitMessage �������ú�������ֹ��Ϣѭ�������� WM_QUIT ��Ϣ
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	// ������û�д������Ϣת����Ĭ�ϵĴ��ڹ��̡�
	// ע�⣬�����Լ�����д�Ĵ��ڹ���һ��Ҫ���� DefWindowProc �����ķ���ֵ
	// �������� P697 ҳ����������ΪҪ��Ŀ�괰�ڲ��������Ϣת���� DefWindowProc ������ȥ�����Ӧ�Ĵ��������Լ���д�Ĵ��ڹ���һ��Ҫ���� DefWindowProc �����ķ���ֵ
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
	���䣺��Ϣ���� MessageBox ������
	ͨ��Դ�뿴����������
	MessageBoxW(
		_In_opt_ HWND hWnd, 
		_In_opt_ LPCWSTR lpText,
		_In_opt_ LPCWSTR lpCaption,
		_In_ UINT uType);
	hWnd������Ϣ���������ڵľ��������ָ��Ϊ NULL
	lpText����Ϣ������ʾ���ı�
	lpCaption����Ϣ��ı����ı�
	uType����Ϣ�����ʽ
	��Ϣ��ķ���ֵ������������Ϣ��ľ������͡�
*/
