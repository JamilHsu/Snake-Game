// 貪食蛇遊戲.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//
#include <string>
#include <algorithm>
#include <iostream>
#include <array>
#include <bit>
#include <Windows.h>
#include <uxtheme.h>
//#include "resource.h"
#pragma comment(lib,"Imm32.lib")
#pragma comment(lib,"UxTheme.lib")
constexpr int width = 17;
constexpr int height = 15;
int cellsize = 55;
using std::array;
DWORD timerPeriod = 500;
struct SnakeData {
    POINT apple;
    POINT head;
    array<array<LONG, 17>, 15> snake;
};
enum UserMessageConstants {
    RequestData = WM_USER + 1,
    GetData,
    ReinitGame = WM_USER + 10
};
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int snakelen = 3;
    static constexpr auto initsnake= []() {
        array<array<LONG, width>, height> s{};
        s[height / 2][0] = 1;
        s[height / 2][1] = 2;
        s[height / 2][2] = 3;
        return s;
        };
    static SnakeData snakedata = { { width / 2,height / 2 } ,{ 2,height / 2 },initsnake() };
    static array<array<LONG, width>, height>& snake = snakedata.snake;
    static POINT& head = snakedata.head;
    static POINT& apple= snakedata.apple;
    const static HBRUSH DCBrush = (HBRUSH)GetStockObject(DC_BRUSH);
    const static HPEN DCPen = (HPEN)GetStockObject(DC_PEN);
    static auto make_backgroundBursh = []() {
        HDC hdc = GetDC(0);
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP bitmap = CreateCompatibleBitmap(hdc, cellsize * 2, cellsize * 2);
        ReleaseDC(0, hdc);
        SelectObject(memDC, bitmap);
        RECT rect = { 0,0,cellsize * 2 ,cellsize * 2 };
        SetDCBrushColor(memDC, RGB(162, 209, 73));
        FillRect(memDC, &rect, DCBrush);
        SetDCBrushColor(memDC, RGB(170, 215, 81));
        rect = { 0,0,cellsize,cellsize };
        FillRect(memDC, &rect, DCBrush);
        rect = { cellsize,cellsize,cellsize * 2,cellsize * 2 };
        FillRect(memDC, &rect, DCBrush);
        HBRUSH background = CreatePatternBrush(bitmap);
        DeleteDC(memDC);
        DeleteObject(bitmap);
        return background;
        };
    static HBRUSH backgroundBursh = make_backgroundBursh();
    const static HBITMAP originapplesnake = []() {
        static_assert(0x00030201==std::bit_cast<DWORD>(RGBQUAD{1,2,3}),"An error occurred while converting DWORD to RGBQUAD. If this assertion is ignored the program will still run correctly but will display incorrect colors.");
        constexpr DWORD data[11 * 11*5] = {
            0xffffff,0xffffff,0x22b14c,0x22b14c,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,
            0xffffff,0xffffff,0x22b14c,0x22b14c,0x22b14c,0x804040,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,
            0xffffff,0xffffff,0xffffff,0x22b14c,0x22b14c,0x804040,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,
            0xffffff,0xffffff,0xffffff,0xed1c24,0xed1c24,0x804040,0xed1c24,0xed1c24,0xffffff,0xffffff,0xffffff,
            0xffffff,0xffffff,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xffffff,0xffffff,
            0xffffff,0xed1c24,0xed1c24,0xff7d7d,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xffffff,
            0xffffff,0xed1c24,0xff7d7d,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xffffff,
            0xffffff,0xed1c24,0xff7d7d,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xffffff,
            0xffffff,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xffffff,
            0xffffff,0xffffff,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xed1c24,0xffffff,0xffffff,
            0xffffff,0xffffff,0xffffff,0xed1c24,0xed1c24,0x804040,0xed1c24,0xed1c24,0xffffff,0xffffff,0xffffff,

            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0xffffff,0x0,0xffffff,0x5180ff,
            0x5180ff,0x3655aa,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x3655aa,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0xffffff,0x0,0xffffff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,

            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x3655aa,0x5180ff,0x5180ff,0x5180ff,0x3655aa,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,0x5180ff,0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,
            0x5180ff,0xffffff,0x0,0xffffff,0x5180ff,0x5180ff,0x5180ff,0xffffff,0x0,0xffffff,0x5180ff,
            0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,0x5180ff,0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0xffffff,0x0,0xffffff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x3655aa,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x3655aa,0x5180ff,
            0x5180ff,0xffffff,0x0,0xffffff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,

            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,0x5180ff,0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,
            0x5180ff,0xffffff,0x0,0xffffff,0x5180ff,0x5180ff,0x5180ff,0xffffff,0x0,0xffffff,0x5180ff,
            0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,0x5180ff,0x5180ff,0xffffff,0xffffff,0xffffff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x3655aa,0x5180ff,0x5180ff,0x5180ff,0x3655aa,0x5180ff,0x5180ff,0x5180ff,
            0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,0x5180ff,
        };
        return CreateBitmap(11, 11*5, 4, 8, data);
        }();
    static auto make_applesnake = []() {
        HDC hdc = GetDC(0);
        HDC orignappleDC = CreateCompatibleDC(hdc);
        HDC appleDC = CreateCompatibleDC(hdc);
        HBITMAP applebitmap = CreateCompatibleBitmap(hdc, cellsize, cellsize * 5);
        ReleaseDC(0, hdc);
        SelectObject(orignappleDC, originapplesnake);
        SelectObject(appleDC, applebitmap);
        StretchBlt(appleDC, 0, 0, cellsize, cellsize * 5, orignappleDC, 0, 0, 11, 11 * 5, SRCCOPY);
        DeleteDC(orignappleDC);
        return appleDC;
        };
    static HDC applesnake = make_applesnake();
    static auto make_maskapple = [] {
        HDC maskDC = CreateCompatibleDC(applesnake);
        HBITMAP mask = CreateBitmap(cellsize, cellsize, 1, 1, nullptr);
        SelectObject(maskDC, mask);
        BitBlt(maskDC, 0, 0, cellsize, cellsize, applesnake, 0, 0, SRCCOPY);
        DeleteDC(maskDC);
        return mask;
        };
    static HBITMAP maskapple = make_maskapple();
    static int left_offset = cellsize;
    static BYTE lastDirection = VK_RIGHT;
    static BYTE direction = VK_RIGHT;
    static int gameover = 0;
    static int step_count = 0;
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        HPAINTBUFFER paintbuffer = BeginBufferedPaint(ps.hdc, &ps.rcPaint, BPBF_COMPATIBLEBITMAP, nullptr,&hdc);
        if (paintbuffer == nullptr) {
            hdc = ps.hdc;
        }
        SetDCBrushColor(hdc, RGB(87, 138, 52));
        FillRect(hdc, &ps.rcPaint, DCBrush);
        RECT rect={ left_offset,cellsize,left_offset+cellsize *width,cellsize * (height + 1) };
        SetBrushOrgEx(hdc, left_offset, cellsize, nullptr);
        FillRect(hdc, &rect, backgroundBursh);
        //Draw apple
        MaskBlt(hdc, left_offset + apple.x * cellsize, (apple.y + 1)* cellsize, cellsize, cellsize, applesnake, 0, 0, maskapple, 0, 0, MAKEROP4(0x00AA0029, SRCCOPY));//0x00AA0029:dest=dest
        //Draw snake
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                const int ij = snake[i][j];
                if (ij > 0&&ij != snakelen) {
                    rect = {
                        left_offset+j * cellsize + (j == 0 || !snake[i][j - 1]||abs(ij - snake[i][j - 1]) > 1),
                        (i + 1) * cellsize + (i == 0 || !snake[i - 1][j]||abs(ij - snake[i - 1][j]) > 1),
                        left_offset+(j + 1) * cellsize - (j == width - 1 || !snake[i][j + 1]||abs(ij - snake[i][j + 1]) > 1),
                        (i + 2) * cellsize - (i == height - 1 || !snake[i + 1][j]||abs(ij - snake[i + 1][j]) > 1)
                    };
                    const int mul = (snake[i][j] + snakelen / 2);
                    const int div = (snakelen + snakelen / 2);
                    SetDCBrushColor(hdc, RGB(81*mul/div, 128*mul/div, 255 * mul / div));
                    FillRect(hdc, &rect, DCBrush);
                }
            }
        }
        //Draw head
        BitBlt(hdc, left_offset+head.x* cellsize, (head.y + 1)* cellsize, cellsize, cellsize, applesnake, 0,cellsize*(1+direction-VK_LEFT), SRCCOPY);
        if (gameover) {
            if (gameover == 1) {
                SetDCBrushColor(hdc, RGB(0, 0, 0));
                SetDCPenColor(hdc, RGB(255, 0, 0));
                SelectObject(hdc, DCBrush);
                SelectObject(hdc, DCPen);
                Rectangle(hdc, left_offset + head.x * cellsize, (head.y + 1) * cellsize, left_offset + (head.x + 1) * cellsize, (head.y + 2) * cellsize);
            }
            if (paintbuffer) {
                EndBufferedPaint(paintbuffer, TRUE);
            }
            EndPaint(hwnd, &ps);
            if (GetLastActivePopup(hwnd) != hwnd) {//In MessageBox
                return 0;
            }
            if (gameover == 1) {
                std::wstring message = L"Score:" + std::to_wstring(snakelen) + L"\nPlay again?";
                if (IDYES == MessageBoxW(hwnd, message.c_str(), L"Game Over!", MB_YESNO | MB_ICONWARNING)) {
                    WindowProc(hwnd, ReinitGame, NULL, NULL);
                }
                else {
                    PostMessageW(hwnd, WM_CLOSE, NULL, NULL);
                }
            }
            else if (gameover == 2) {
                std::wstring message = L"Steps spent:" + std::to_wstring(step_count) + L"\nPlay again?";
                if (IDYES == MessageBoxW(hwnd, message.c_str(), L"Completed!!!", MB_YESNO | MB_ICONINFORMATION)) {
                    WindowProc(hwnd, ReinitGame, NULL, NULL);
                }
                else {
                    gameover = 0;
                }
            }
        }
        else {
            if (paintbuffer) {
                EndBufferedPaint(paintbuffer, TRUE);
            }
            EndPaint(hwnd, &ps);
        }
        return 0;
    }
    case ReinitGame:
        snake = initsnake();
        snakelen = 3;
        head = { 2,height / 2 };
        apple = { width / 2,height / 2 };
        lastDirection = direction = VK_RIGHT;
        gameover = false;
        step_count = 0;
        InvalidateRect(hwnd, nullptr, FALSE);
        return 0;
    case WM_TIMER:
        wParam = VK_RETURN;
        [[fallthrough]];
    case WM_KEYDOWN: {
        if (gameover) {
            return 0;
        }
        switch (BYTE nextdirection=wParam) {
        case 'A':nextdirection = VK_LEFT; goto caseLURD;
        case 'W':nextdirection = VK_UP; goto caseLURD;
        case 'D':nextdirection = VK_RIGHT; goto caseLURD;
        case 'S':nextdirection = VK_DOWN; goto caseLURD;
        caseLURD:
        case VK_LEFT:
        case VK_UP:
        case VK_RIGHT:
        case VK_DOWN:
            if (nextdirection != direction) {
                if (abs(lastDirection - nextdirection) != 2) {
                    direction = nextdirection;
                    HDC hdc = GetDC(hwnd);
                    BitBlt(hdc, left_offset+head.x* cellsize, (head.y + 1)* cellsize, cellsize, cellsize, applesnake, 0, (1+direction-VK_LEFT)*cellsize, SRCCOPY);
                    ReleaseDC(hwnd, hdc);
                }
            }
            return 0;
        case VK_RETURN:
        case VK_SPACE:
            ++step_count;
            lastDirection = direction;
            switch (direction)
            {
            case VK_LEFT:
                --head.x;
                break;
            case VK_UP:
                --head.y;
                break;
            case VK_RIGHT:
                ++head.x;
                break;
            case VK_DOWN:
                ++head.y;
                break;
            }
            if (head.x == apple.x && head.y == apple.y) {
                //吃到蘋果了
                ++snakelen;
                if (snakelen == width * height) {
                    gameover = 2;
                    snake[head.y][head.x] = snakelen;
                    auto i = snake.data()->data();
                    int nextapple = std::min_element(i, i + width * height)-i;
                    apple = { nextapple % width,nextapple / width };
                    InvalidateRect(hwnd, nullptr, FALSE);
                    UpdateWindow(hwnd);
                    return 0;
                }
                snake[head.y][head.x] = -1;
                int nextapple = rand() % (width * height - snakelen);
                for (int i = 0; i < height; ++i) {
                    for (int j = 0; j < width; ++j) {
                        if (snake[i][j] == 0) {
                            if (nextapple-- == 0) {
                                apple.x = j;
                                apple.y = i;
                                //break*2
                                i = 0x7fff;
                                j = 0x7fff;
                            }
                        }
                    }
                }
            }
            else {
                for (auto i = snake.data()->data(), end = i + width * height; i != end; ++i) {
                    if ((*i) > 0) {
                        --(*i);
                    }
                }
            }
            
            if (head.x<0 || head.x>=width || head.y<0 || head.y>=height||snake[head.y][head.x]>0) {
                gameover = 1;
            }
            else {
                snake[head.y][head.x] = snakelen;
            }
            InvalidateRect(hwnd, nullptr, FALSE);
            if (timerPeriod && uMsg == WM_KEYDOWN) {
                SetTimer(hwnd, timerPeriod, timerPeriod, nullptr);
            }
            return 0;
        default:
            return 0;
        }
    }
    case RequestData: {
        COPYDATASTRUCT data = {
            gameover,
            sizeof(snakedata),
            &snakedata
        };
        SendMessageW((HWND)wParam, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)(&data));
        return 0;
    }
    case WM_SIZE:
        cellsize = (std::min)(LOWORD(lParam) / (width + 2), HIWORD(lParam)/(height+2));
        DeleteObject(backgroundBursh);
        DeleteObject(SelectObject(applesnake, GetStockObject(21)));
        DeleteDC(applesnake);
        DeleteObject(maskapple);
        backgroundBursh = make_backgroundBursh();
        applesnake = make_applesnake();
        maskapple = make_maskapple();
        left_offset = (LOWORD(lParam) - cellsize * width) / 2;
        return 0;
    case WM_DESTROY: {
        DeleteObject(backgroundBursh);
        DeleteObject(originapplesnake);
        DeleteObject(SelectObject(applesnake, GetStockObject(21)));
        DeleteDC(applesnake);
        DeleteObject(maskapple);
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}

int main(int argc, char* argv[]) {
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
    srand(GetTickCount());

    if (argc > 1) {
        timerPeriod = atoi(argv[1]);
    }
    else {
        std::cout << "Please enter the timer period in milliseconds.\n"
            "Enter 0 to disable the timer.\n";
        std::cin >> timerPeriod;
    }
    if (timerPeriod) {
        std::cout << "Timer period: " << timerPeriod << '\n'
            << "Speed: " << 1000.0 / timerPeriod;
    }
    else {
        std::cout << "Timer disabled.\n"
            "Speed: 0";
    }
    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = L"SnakeGameClass";
    //wc.hIcon = (HICON)LoadImageW(GetModuleHandle(nullptr), MAKEINTRESOURCEW(IDI_ICON_SNAKE), IMAGE_ICON, 110, 110, LR_DEFAULTCOLOR);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    ImmDisableIME(0);//停用輸入法
    RegisterClassW(&wc);
    RECT rect{ 0,0, (width+2) * cellsize , (height+2) * cellsize };
    AdjustWindowRectExForDpi(&rect, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,NULL,NULL,GetDpiForSystem());
    HWND hwnd = CreateWindowExW(
        NULL, L"SnakeGameClass", L"Snake Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right-rect.left,rect.bottom-rect.top,
        NULL, NULL, NULL, NULL
    );
    if (!hwnd) return -1;

    if (timerPeriod) {
        SetTimer(hwnd, timerPeriod, timerPeriod, nullptr);
    }
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
    BufferedPaintInit();
    MSG msg;
    while (GetMessageW(&msg, NULL, NULL, NULL)) {
        DispatchMessageW(&msg);
    }
    BufferedPaintUnInit();
    return msg.wParam;
}


// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
