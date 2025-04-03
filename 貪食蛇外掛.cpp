#include <Windows.h>
#include <array>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cassert>
using std::array;
using std::vector;
struct SnakeData {
	POINT apple;
	POINT head;
	array<array<LONG, 17>, 15> snake;
}data;
enum UserMessageConstants {
	RequestData=WM_USER + 1,
	GetData,
};
static void CoreAlgorithm(HWND game) {// 不在參數列表中的另一引數是全域變數data
	// 在這裡編寫你的程式碼，然後將WindowProc中對CoreAlgorithm_example的呼叫改為CoreAlgorithm
	// 除非你確實了解Windows訊息迴圈機制，否則請不要任意更改CoreAlgorithm以外的程式碼
	// 請根據data決定蛇的前進方向，你基本上不會需要存取data以外的變數
	// 所需的資訊都包含在全域變數data中，包含apple蘋果位置、head蛇頭位置、snake蛇身狀態
	// 二維陣列snake裡的各個元素代表地圖上的各個格子，0代表空地，而大於0的數字代表蛇身，以及再走幾步後該格會變成空地
	// 當然，這代表1就是蛇尾，而值最大之處是蛇頭(即snake[head.y][head.x]==蛇的長度)

	// 決定好應前進的方向後，透過PostMessage進行輸出
	// 格式如下:PostMessageW(game, WM_KEYDOWN, (方向), NULL);
	// 其中，(方向)應為 VK_LEFT/VK_UP/VK_RIGHT/VK_DOWN 中的其中之一
	// 'W' 'S' 'A' 'D' 也可以
	// 輸出方向後，以PostMessageW(game, WM_KEYDOWN, VK_RETURN, NULL);前進一格
	// 你可以選擇在每次前進一格前都輸出一次方向，或是當方向沒有改變時就不重新輸出方向，不影響。這也是data中沒有包含當前前進方向的原因
	// 建議的做法：規劃好前往蘋果的路徑，然後將對應的操作發送出去，然後從CoreAlgorithm中傳回，接著下次CoreAlgorithm被呼叫時data將包含更新後的資料，重複以上流程。
	// 流程：CoreAlgorithm被呼叫→(規畫路徑)→(發送操作)→(return)→更新data→呼叫CoreAlgorithm→CoreAlgorithm被呼叫　(括號部分為你應處理的部分)
	
	// 不用在意為何沒有與WM_KEYDOWN匹配的WM_KEYUP，因為WM_KEYUP消息直接被我扔給了DefWindowProc
}
static void CoreAlgorithm_example(HWND game) {//此為範例程式碼
	// 此範例程式碼採用最笨的方法，循著固定的路徑前進，最簡單但也最沒效率
	// 請試著編寫更有效率的演算法

	constexpr BYTE 左 = VK_LEFT;
	constexpr BYTE 上 = VK_UP;
	constexpr BYTE 右 = VK_RIGHT;
	constexpr BYTE 下 = VK_DOWN;
	static array<array<BYTE, 17>, 15> path = { {
		{下,左,左,左,左,左,左,左,左,左,左,左,左,左,左,左,左},
		{右,右,右,右,右,右,右,右,右,右,右,右,右,右,右,下,上},
		{下,左,左,左,左,左,左,左,左,左,左,左,左,左,左,左,上},
		{右,右,右,右,右,右,右,右,右,右,右,右,右,右,右,下,上},
		{下,左,左,左,左,左,左,左,左,左,左,左,左,左,左,左,上},
		{右,右,右,右,右,右,右,右,右,右,右,右,右,右,右,下,上},
		{下,左,左,左,左,左,左,左,左,左,左,左,左,左,左,左,上},
		{右,右,右,右,右,右,右,右,右,右,右,右,右,右,右,下,上},
		{下,左,左,左,左,左,左,左,左,左,左,左,左,左,左,左,上},
		{右,右,右,右,右,右,右,右,右,右,右,右,右,右,右,下,上},
		{下,左,左,左,左,左,左,左,左,左,左,左,左,左,左,左,上},
		{右,右,右,右,右,右,右,右,右,右,右,右,右,右,右,下,上},
		{下,左,左,左,左,左,左,左,左,左,左,左,左,左,左,左,上},
		{下,右,下,右,下,右,下,右,下,右,下,右,下,右,下,右,上},
		{右,上,右,上,右,上,右,上,右,上,右,上,右,上,右,右,上}
		} };
	path[14][15] = data.apple.x == 15 && data.apple.y == 13 ? 上 : 右;
	int example12 = GetTickCount() % 2;
	if (example12) {
		//範例1：直接將操作發送給遊戲
		while (data.head.x != data.apple.x || data.head.y != data.apple.y) {
			PostMessageW(game, WM_KEYDOWN, path[data.head.y][data.head.x], NULL);
			PostMessageW(game, WM_KEYDOWN, VK_RETURN, NULL);
			UpdateWindow(game);// Sleep(1);//強制貪食蛇遊戲刷新窗口影像，非必要
			switch (path[data.head.y][data.head.x]) {
			case VK_LEFT:
				--data.head.x;
				break;
			case VK_UP:
				--data.head.y;
				break;
			case VK_RIGHT:
				++data.head.x;
				break;
			case VK_DOWN:
				++data.head.y;
				break;
			}
		}
	}
	else {
		//範例2：將操作累積起來，最後再一口氣全部發送出去
		static vector<BYTE> steps;
		while (data.head.x != data.apple.x || data.head.y != data.apple.y) {
			steps.push_back(path[data.head.y][data.head.x]);
			switch (path[data.head.y][data.head.x]) {
			case VK_LEFT:
				--data.head.x;
				break;
			case VK_UP:
				--data.head.y;
				break;
			case VK_RIGHT:
				++data.head.x;
				break;
			case VK_DOWN:
				++data.head.y;
				break;
			}
		}
		for (auto i : steps) {
			PostMessageW(game, WM_KEYDOWN, i, NULL);
			PostMessageW(game, WM_KEYDOWN, VK_RETURN, NULL);
			UpdateWindow(game); Sleep(10);//除了強制遊戲刷新窗口影像之外，也可以稍做等待以看得更清楚些
		}
		steps.clear();
	}
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_COPYDATA: {
		const COPYDATASTRUCT* pdata = reinterpret_cast<COPYDATASTRUCT*>(lParam);
		assert(pdata->cbData == sizeof(data));
		memcpy(&data, pdata->lpData, sizeof(data));
		if (pdata->dwData) {
			if (pdata->dwData == 1) {
				//gameover
				std::cout.flags(std::ios::right);
				std::cout << "Failed!\n"
					"apple:(" << data.apple.x << ',' << data.apple.y << ")\n"
					"head:(" << data.head.x << ',' << data.head.y << ")\n";
				for (const auto& i : data.snake) {
					for (auto j = i.begin(); j < i.end() - 1; ++j) {
						std::cout << std::setw(3) << *j << ',';
					}
					std::cout << std::setw(3) << i.back() << '\n';
				}
			}
			else {
				std::cout << "Completed!\n";
			}
			PostMessageW(hwnd, WM_CLOSE, wParam, NULL);
			return 0;
		}
		PostMessageW(hwnd, GetData, wParam, NULL);
		return 0;
	}
	case GetData:
		CoreAlgorithm_example(reinterpret_cast<HWND>(wParam));
		//CoreAlgorithm(reinterpret_cast<HWND>(wParam));
		PostMessageW((HWND)wParam, RequestData, (WPARAM)hwnd, NULL);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}
}
int main() {
	HWND game = FindWindowW(L"SnakeGameClass", L"Snake Game");
	if (!game) {
		std::cout << "FindWindow failed!";
		return -1;
	}
	const wchar_t* ClassName = L"GetDataWindowClass";
	WNDCLASSW wc{};
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = ClassName;
	RegisterClassW(&wc);
	HWND MessageWindow= CreateWindowExW(NULL, ClassName, L"GetDataWindow", NULL, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
	if (!MessageWindow) {
		std::cout << "Create window Failed!";
		return -2;
	}
	SetForegroundWindow(game);
	PostMessageW(game, RequestData, (WPARAM)MessageWindow, NULL);
	MSG msg;
	while (GetMessageW(&msg,nullptr,NULL,NULL)){
		DispatchMessageW(&msg);
	}
	return msg.wParam;
}