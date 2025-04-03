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
static void CoreAlgorithm(HWND game) {// ���b�ѼƦC�����t�@�޼ƬO�����ܼ�data
	// �b�o�̽s�g�A���{���X�A�M��NWindowProc����CoreAlgorithm_example���I�s�אּCoreAlgorithm
	// ���D�A�T��F��Windows�T���j�����A�_�h�Ф��n���N���CoreAlgorithm�H�~���{���X
	// �Юھ�data�M�w�D���e�i��V�A�A�򥻤W���|�ݭn�s��data�H�~���ܼ�
	// �һݪ���T���]�t�b�����ܼ�data���A�]�tappleī�G��m�Bhead�D�Y��m�Bsnake�D�����A
	// �G���}�Csnake�̪��U�Ӥ����N��a�ϤW���U�Ӯ�l�A0�N��Ŧa�A�Ӥj��0���Ʀr�N��D���A�H�ΦA���X�B��Ӯ�|�ܦ��Ŧa
	// ��M�A�o�N��1�N�O�D���A�ӭȳ̤j���B�O�D�Y(�Ysnake[head.y][head.x]==�D������)

	// �M�w�n���e�i����V��A�z�LPostMessage�i���X
	// �榡�p�U:PostMessageW(game, WM_KEYDOWN, (��V), NULL);
	// �䤤�A(��V)���� VK_LEFT/VK_UP/VK_RIGHT/VK_DOWN �����䤤���@
	// 'W' 'S' 'A' 'D' �]�i�H
	// ��X��V��A�HPostMessageW(game, WM_KEYDOWN, VK_RETURN, NULL);�e�i�@��
	// �A�i�H��ܦb�C���e�i�@��e����X�@����V�A�άO���V�S�����ܮɴN�����s��X��V�A���v�T�C�o�]�Odata���S���]�t��e�e�i��V����]
	// ��ĳ�����k�G�W���n�e��ī�G�����|�A�M��N�������ާ@�o�e�X�h�A�M��qCoreAlgorithm���Ǧ^�A���ۤU��CoreAlgorithm�Q�I�s��data�N�]�t��s�᪺��ơA���ƥH�W�y�{�C
	// �y�{�GCoreAlgorithm�Q�I�s��(�W�e���|)��(�o�e�ާ@)��(return)����sdata���I�sCoreAlgorithm��CoreAlgorithm�Q�I�s�@(�A���������A���B�z������)
	
	// ���Φb�N����S���PWM_KEYDOWN�ǰt��WM_KEYUP�A�]��WM_KEYUP���������Q�ڥ����FDefWindowProc
}
static void CoreAlgorithm_example(HWND game) {//�����d�ҵ{���X
	// ���d�ҵ{���X�ĥγ̲ª���k�A�`�۩T�w�����|�e�i�A��²����]�̨S�Ĳv
	// �иյ۽s�g�󦳮Ĳv���t��k

	constexpr BYTE �� = VK_LEFT;
	constexpr BYTE �W = VK_UP;
	constexpr BYTE �k = VK_RIGHT;
	constexpr BYTE �U = VK_DOWN;
	static array<array<BYTE, 17>, 15> path = { {
		{�U,��,��,��,��,��,��,��,��,��,��,��,��,��,��,��,��},
		{�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�U,�W},
		{�U,��,��,��,��,��,��,��,��,��,��,��,��,��,��,��,�W},
		{�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�U,�W},
		{�U,��,��,��,��,��,��,��,��,��,��,��,��,��,��,��,�W},
		{�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�U,�W},
		{�U,��,��,��,��,��,��,��,��,��,��,��,��,��,��,��,�W},
		{�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�U,�W},
		{�U,��,��,��,��,��,��,��,��,��,��,��,��,��,��,��,�W},
		{�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�U,�W},
		{�U,��,��,��,��,��,��,��,��,��,��,��,��,��,��,��,�W},
		{�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�k,�U,�W},
		{�U,��,��,��,��,��,��,��,��,��,��,��,��,��,��,��,�W},
		{�U,�k,�U,�k,�U,�k,�U,�k,�U,�k,�U,�k,�U,�k,�U,�k,�W},
		{�k,�W,�k,�W,�k,�W,�k,�W,�k,�W,�k,�W,�k,�W,�k,�k,�W}
		} };
	path[14][15] = data.apple.x == 15 && data.apple.y == 13 ? �W : �k;
	int example12 = GetTickCount() % 2;
	if (example12) {
		//�d��1�G�����N�ާ@�o�e���C��
		while (data.head.x != data.apple.x || data.head.y != data.apple.y) {
			PostMessageW(game, WM_KEYDOWN, path[data.head.y][data.head.x], NULL);
			PostMessageW(game, WM_KEYDOWN, VK_RETURN, NULL);
			UpdateWindow(game);// Sleep(1);//�j��g���D�C����s���f�v���A�D���n
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
		//�d��2�G�N�ާ@�ֿn�_�ӡA�̫�A�@�f������o�e�X�h
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
			UpdateWindow(game); Sleep(10);//���F�j��C����s���f�v�����~�A�]�i�H�y�����ݥH�ݱo��M����
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