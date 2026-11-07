#include <windows.h>
#include <windowsx.h>
#include <algorithm>
#include <random>
#include <sstream>
#include <string>

static HWND g_hwnd{};
static std::mt19937 g_rng{ std::random_device{}() };
static int g_x = 500, g_y = 300, g_radius = 28;
static int g_hits = 0, g_shots = 0, g_streak = 0, g_best = 0;

static int Rand(int lo, int hi) {
    if (hi <= lo) return lo;
    return std::uniform_int_distribution<int>(lo, hi)(g_rng);
}
static void SpawnTarget() {
    RECT r{}; GetClientRect(g_hwnd, &r);
    g_x = Rand(70, (r.right > 140 ? r.right - 70 : 70));
    g_y = Rand(125, (r.bottom > 140 ? r.bottom - 70 : 140));
}
static void DrawTextLine(HDC dc, int x, int y, const std::wstring& s, COLORREF color, int size=20) {
    SetTextColor(dc, color);
    HFONT f = CreateFontW(size, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH, L"Segoe UI");
    HGDIOBJ old = SelectObject(dc, f);
    TextOutW(dc, x, y, s.c_str(), static_cast<int>(s.size()));
    SelectObject(dc, old); DeleteObject(f);
}
static void Paint(HDC dc) {
    RECT rc{}; GetClientRect(g_hwnd, &rc);
    HBRUSH bg = CreateSolidBrush(RGB(18, 20, 29));
    FillRect(dc, &rc, bg); DeleteObject(bg);

    DrawTextLine(dc, 24, 18, L"AIM TRAINER | TREINO INDEPENDENTE", RGB(190,150,255), 25);
    std::wstringstream info;
    info << L"Acertos: " << g_hits << L"    Disparos: " << g_shots
         << L"    Precisao: " << (g_shots ? (g_hits * 100 / g_shots) : 0)
         << L"%    Sequencia: " << g_streak << L"    Recorde: " << g_best;
    DrawTextLine(dc, 26, 62, info.str(), RGB(225,228,240), 18);
    DrawTextLine(dc, 26, 94, L"CLIQUE NO ALVO | R: reiniciar | + / -: tamanho | ESC: sair",
                 RGB(150,160,180), 15);

    HBRUSH outer = CreateSolidBrush(RGB(155, 75, 245));
    HBRUSH inner = CreateSolidBrush(RGB(245, 245, 250));
    HBRUSH core = CreateSolidBrush(RGB(245, 75, 105));
    HGDIOBJ old = SelectObject(dc, outer);
    Ellipse(dc, g_x-g_radius, g_y-g_radius, g_x+g_radius, g_y+g_radius);
    SelectObject(dc, inner);
    Ellipse(dc, g_x-g_radius/2, g_y-g_radius/2, g_x+g_radius/2, g_y+g_radius/2);
    SelectObject(dc, core);
    Ellipse(dc, g_x-g_radius/6, g_y-g_radius/6, g_x+g_radius/6, g_y+g_radius/6);
    SelectObject(dc, old);
    DeleteObject(outer); DeleteObject(inner); DeleteObject(core);

    DrawTextLine(dc, 26, rc.bottom-36,
        L"Treino local: nao interage com CS2 ou outros jogos.", RGB(135,145,165), 14);
}
static void Reset() {
    g_hits = g_shots = g_streak = g_best = 0;
    SpawnTarget();
    InvalidateRect(g_hwnd, nullptr, TRUE);
}
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE: g_hwnd = hwnd; SpawnTarget(); return 0;
    case WM_LBUTTONDOWN: {
        int mx = GET_X_LPARAM(lp), my = GET_Y_LPARAM(lp);
        ++g_shots;
        int dx = mx-g_x, dy = my-g_y;
        if (dx*dx + dy*dy <= g_radius*g_radius) {
            ++g_hits; ++g_streak; g_best = std::max(g_best, g_streak); SpawnTarget();
        } else g_streak = 0;
        InvalidateRect(hwnd, nullptr, TRUE); return 0;
    }
    case WM_KEYDOWN:
        if (wp == 'R') Reset();
        else if (wp == VK_ADD || wp == VK_OEM_PLUS) g_radius = (std::min)(70, g_radius+3);
        else if (wp == VK_SUBTRACT || wp == VK_OEM_MINUS) g_radius = (std::max)(10, g_radius-3);
        else if (wp == VK_ESCAPE) DestroyWindow(hwnd);
        InvalidateRect(hwnd, nullptr, TRUE); return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps{}; HDC dc = BeginPaint(hwnd, &ps); Paint(dc); EndPaint(hwnd, &ps); return 0;
    }
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}
int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    const wchar_t CLASS_NAME[] = L"AimTrainerWindow";
    WNDCLASSW wc{}; wc.lpfnWndProc = WndProc; wc.hInstance = instance;
    wc.lpszClassName = CLASS_NAME; wc.hCursor = LoadCursor(nullptr, IDC_CROSS);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW+1);
    if (!RegisterClassW(&wc)) return 1;
    HWND hwnd = CreateWindowExW(0, CLASS_NAME, L"AimTrainer - Windows x64 Release",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 700,
        nullptr, nullptr, instance, nullptr);
    if (!hwnd) return 2;
    ShowWindow(hwnd, show);
    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg); DispatchMessageW(&msg);
    }
    return static_cast<int>(msg.wParam);
}
