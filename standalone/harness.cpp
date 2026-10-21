// Test harness for unattended standalone runs (tools/dryrun.py). Everything is opt-in through environment variables
// and installed before the game's WinMain; without them the game runs unchanged.
//
//   CSF_DISPLAY=offscreen  windowed 1280x960, parked outside the desktop: no taskbar entry, never activated, no focus
//                          or foreground changes, game input only from the harness (DirectInput non-exclusive and
//                          background, real events dropped), movies without sound. Screenshots come from the back buffer.
//   CSF_DISPLAY=window     windowed 1280x960 on screen, otherwise unchanged (for a person to watch).
//   CSF_SKIP_INTRO=1       the intro page (logos, opening movie) hands over to the next page at once (0x535900).
//   CSF_GOTO=P_Option      once the main menu is up, open that page (menu manager slot 0x58), as its button would.
//   CSF_MISSION=Maps/FR01/Ransom.scn
//                          once the main menu is up, select the scenario and open the in-game page, as the scenario
//                          select page does (0x52FC60).
//   CSF_SHOTS=5,12 CSF_SHOT_DIR=C:\..\shots CSF_SHOT_TAG=menu
//                          back-buffer PNGs at those seconds after the first frame: DIR\TAG-T.png (D3DX).
//   CSF_QUIT_AFTER=40      close the window that many seconds after the first frame (WinMain returns normally).
//
// The game's functions are reached through csf::fn_slot<ADDR>::p (globals.h); redirecting a slot here makes every
// caller in other translation units reach the harness wrapper, which calls the original through the saved pointer.
#include <windows.h>
#include <d3d9.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "globals.h"

extern "C" void (*csf_present_hook)(void* device);  // src/boundary/rw/device.cpp, before every Present

CSF_FUNC(int, __cdecl, Harness_GetConfig, 0x00753A30, ());          // +0x11C full screen
CSF_FUNC(int, __cdecl, Harness_GetVideoSettings, 0x0075FDF0, ());   // +0xB0/+0xB4 width/height (float)
CSF_FUNC(int, __cdecl, Harness_GetGlobalMenu, 0x004FDBB0, ());      // 0x8B0568: +0x5C scenario accepted
CSF_FUNC(void, __fastcall, Harness_SelectScenario, 0x00501990, (void* self, void* edx, const char* name, char apply));
CSF_GLOBAL(void*, g_HarnessMenuManager, 0x008B0570);
CSF_GLOBAL(const char*, g_HarnessPageJuego, 0x00899AF0);

using CreateWindowFn = HWND (WINAPI*)(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
using ShowWindowFn = BOOL (WINAPI*)(HWND, int);
using SetFocusFn = HWND (WINAPI*)(HWND);
using SetForegroundFn = BOOL (WINAPI*)(HWND);
using MoveWindowFn = BOOL (WINAPI*)(HWND, int, int, int, int, BOOL);
using BinkSetSoundTrackFn = void (WINAPI*)(unsigned, unsigned*);
using BinkSetSoundSystemFn = int (WINAPI*)(void*, void*);
CSF_GLOBAL(CreateWindowFn, g_HarnessCreateWindowExA, 0x0082D2D8);
CSF_GLOBAL(ShowWindowFn, g_HarnessShowWindow, 0x0082D2D4);
CSF_GLOBAL(SetFocusFn, g_HarnessSetFocus, 0x0082D2CC);
CSF_GLOBAL(SetForegroundFn, g_HarnessSetForegroundWindow, 0x0082D28C);
CSF_GLOBAL(MoveWindowFn, g_HarnessMoveWindow, 0x0082D2B8);
CSF_GLOBAL(BinkSetSoundTrackFn, g_HarnessBinkSetSoundTrack, 0x0082D3B0);
CSF_GLOBAL(BinkSetSoundSystemFn, g_HarnessBinkSetSoundSystem, 0x0082D3B8);

bool csf_harness_offscreen;  // read by game_plain.cpp's DirectInput wrapper
// With a harness display, CSF_INPUT times count from the main menu (30 frames after its page is requested), so
// scripts do not depend on start-up and intro length; 0 until then (game_plain.cpp waits for it).
bool csf_harness_menu_clock;
DWORD csf_harness_menu_tick;

namespace {
enum class Display { Game, Window, Offscreen };
Display display = Display::Game;
constexpr int kWidth = 1280, kHeight = 960;
HWND gameWindow;
WNDPROC gameWindowProc;
CreateWindowFn realCreateWindow;
ShowWindowFn realShowWindow;
DWORD firstFrame;
struct ShotTime { double t; bool fromMenu; std::string label; };
std::vector<ShotTime> shotTimes;
std::string shotDir, shotTag = "shot", gotoPage, mission;
double quitAfter;
bool menuUp, actionDone;
int menuFrames;
void* realRefreshIntro;
void* realRequestMenu;

std::string Env(const char* name) {
    char value[1024];
    const DWORD n = GetEnvironmentVariableA(name, value, sizeof value);
    return n && n < sizeof value ? std::string(value, n) : std::string();
}

template <uintptr_t A> void* Redirect(void* wrapper) {
    void** slot = const_cast<void**>(&::csf::fn_slot<A>::p);
    DWORD old;
    VirtualProtect(slot, sizeof(void*), PAGE_READWRITE, &old);
    void* original = *slot;
    *slot = wrapper;
    VirtualProtect(slot, sizeof(void*), old, &old);
    return original;
}

// A virtual slot of a generated vtable (the vtables hold the slot addresses read at start-up, not fn_slot).
void* RedirectVirtual(uintptr_t vtable, int offset, void* wrapper) {
    void** slot = reinterpret_cast<void**>(vtable) + offset / 4;
    DWORD old;
    VirtualProtect(slot, sizeof(void*), PAGE_READWRITE, &old);
    void* original = *slot;
    *slot = wrapper;
    VirtualProtect(slot, sizeof(void*), old, &old);
    return original;
}

// Offscreen: deactivation would pause the game (0x508D40 WM_ACTIVATE / WM_ACTIVATEAPP); the window is never active,
// but other windows' activation still sends it these.
LRESULT CALLBACK OffscreenProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_ACTIVATE || message == WM_ACTIVATEAPP || message == WM_MOUSEACTIVATE)
        return message == WM_MOUSEACTIVATE ? MA_NOACTIVATE : 0;
    return CallWindowProcA(gameWindowProc, window, message, wParam, lParam);
}

RECT OuterRect(DWORD style, DWORD exStyle) {
    RECT r{0, 0, kWidth, kHeight};
    AdjustWindowRectEx(&r, style, FALSE, exStyle);
    return r;
}

HWND WINAPI HarnessCreateWindow(DWORD exStyle, LPCSTR cls, LPCSTR title, DWORD style, int x, int y, int w, int h,
                                HWND parent, HMENU menu, HINSTANCE instance, LPVOID param) {
    if (parent || gameWindow)
        return realCreateWindow(exStyle, cls, title, style, x, y, w, h, parent, menu, instance, param);
    // The renderer (0x506F70, right after this window) takes full screen from the settings (+0x11C) and the size
    // from the video settings (+0xB0/+0xB4): windowed at the harness size.
    CSF_AT(uint8_t, reinterpret_cast<void*>(Harness_GetConfig()), 0x11C) = 0;
    CSF_AT(float, reinterpret_cast<void*>(Harness_GetVideoSettings()), 0xB0) = static_cast<float>(kWidth);
    CSF_AT(float, reinterpret_cast<void*>(Harness_GetVideoSettings()), 0xB4) = static_cast<float>(kHeight);
    style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;  // 0xCA0000, the game's own windowed style
    exStyle = 0;
    if (display == Display::Offscreen) {
        style = WS_POPUP;
        exStyle = WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE;
    }
    const RECT r = OuterRect(style, exStyle);
    w = r.right - r.left;
    h = r.bottom - r.top;
    if (display == Display::Offscreen) {
        x = GetSystemMetrics(SM_XVIRTUALSCREEN) - w - 4000;
        y = GetSystemMetrics(SM_YVIRTUALSCREEN) - h - 4000;
    } else {
        x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
        y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
        if (y < 0) y = 0;
    }
    gameWindow = realCreateWindow(exStyle, cls, title, style, x, y, w, h, parent, menu, instance, param);
    if (gameWindow && display == Display::Offscreen)
        gameWindowProc = reinterpret_cast<WNDPROC>(
            SetWindowLongPtrA(gameWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&OffscreenProc)));
    std::fprintf(stderr, "csf: harness window %s %dx%d at %d,%d\n",
                 display == Display::Offscreen ? "offscreen" : "on screen", kWidth, kHeight, x, y);
    return gameWindow;
}
// The game re-centres its window on the client size it computed (0x508F50); the harness window keeps its place.
BOOL WINAPI HarnessMoveWindow(HWND window, int, int, int, int, BOOL) { return window == gameWindow; }
BOOL WINAPI HarnessShowWindow(HWND window, int command) {
    return realShowWindow(window, display == Display::Offscreen && window == gameWindow ? SW_SHOWNOACTIVATE : command);
}
HWND WINAPI HarnessSetFocus(HWND) { return nullptr; }
BOOL WINAPI HarnessSetForeground(HWND) { return TRUE; }
// No sound system and no audio track: the movies play silently.
int WINAPI HarnessBinkSetSoundSystem(void*, void*) { return 0; }
void WINAPI HarnessBinkSetSoundTrack(unsigned, unsigned*) {}

// 0x535900: with no override record the intro plays the five table pages (+0x20 counts them), then the opening
// movie from the settings once (+0x28). Both done: the original goes on with 0x535860 (the next page).
void* __fastcall HarnessRefreshIntro(void* self, void* edx) {
    if (!CSF_AT(void*, self, 0x24)) {
        CSF_AT(uint32_t, self, 0x20) = 5;
        CSF_AT(uint8_t, self, 0x28) = 0;
    }
    return reinterpret_cast<void*(__fastcall*)(void*, void*)>(realRefreshIntro)(self, edx);
}

int __fastcall HarnessRequestMenu(void* self, void* edx, char* name) {
    if (name && !_stricmp(name, "Menus/P_MePrin.fbs")) menuUp = true;
    return reinterpret_cast<int(__fastcall*)(void*, void*, char*)>(realRequestMenu)(self, edx, name);
}

template <typename Fn> Fn Slot(void* obj, int offset) { return reinterpret_cast<Fn>(CSF_AT(void**, obj, 0)[offset / 4]); }

// After the main menu has drawn a few frames, on the game thread (inside the present).
void RunAction() {
    void* manager = g_HarnessMenuManager;
    if (!manager) return;
    if (!mission.empty()) {
        void* menu = reinterpret_cast<void*>(Harness_GetGlobalMenu());
        Harness_SelectScenario(menu, nullptr, mission.c_str(), 1);
        const int accepted = CSF_AT(int, reinterpret_cast<void*>(Harness_GetGlobalMenu()), 0x5C);
        std::fprintf(stderr, "csf: harness mission %s (accepted %d)\n", mission.c_str(), accepted);
        if (accepted == 1) Slot<void(__fastcall*)(void*, void*, const char*)>(manager, 0x58)(manager, nullptr, g_HarnessPageJuego);
    } else if (!gotoPage.empty()) {
        std::string page = gotoPage;
        if (page.find('/') == std::string::npos) page = "Menus/" + page;
        if (page.find('.') == std::string::npos) page += ".fbs";
        std::fprintf(stderr, "csf: harness goto %s\n", page.c_str());
        Slot<void(__fastcall*)(void*, void*, const char*)>(manager, 0x58)(manager, nullptr, page.c_str());
    }
}

using SaveSurfaceFn = HRESULT (WINAPI*)(LPCSTR, DWORD, IDirect3DSurface9*, const PALETTEENTRY*, const RECT*);
void Shot(IDirect3DDevice9* device, const std::string& label) {
    static SaveSurfaceFn save = [] {
        HMODULE m = LoadLibraryA("d3dx9_43.dll");
        return m ? reinterpret_cast<SaveSurfaceFn>(GetProcAddress(m, "D3DXSaveSurfaceToFileA")) : nullptr;
    }();
    IDirect3DSurface9* back = nullptr;
    char path[MAX_PATH];
    std::snprintf(path, sizeof path, "%s\\%s-%s.png", shotDir.c_str(), shotTag.c_str(), label.c_str());
    HRESULT hr = E_FAIL;
    if (save && SUCCEEDED(device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back))) {
        hr = save(path, 3 /* D3DXIFF_PNG */, back, nullptr, nullptr);
        back->Release();
    }
    std::fprintf(stderr, "csf: harness shot %s %s\n", path, SUCCEEDED(hr) ? "ok" : "FAILED");
}

void OnPresent(void* device) {
    const DWORD now = GetTickCount();
    if (!firstFrame) firstFrame = now ? now : 1;
    const double t = (now - firstFrame) / 1000.0;
    if (menuUp && !actionDone && ++menuFrames >= 30) {
        actionDone = true;
        csf_harness_menu_tick = now ? now : 1;
        std::fprintf(stderr, "csf: harness main menu at %.1f s\n", t);
        RunAction();
    }
    for (ShotTime& shot : shotTimes) {
        const double since = shot.fromMenu ? (csf_harness_menu_tick ? (now - csf_harness_menu_tick) / 1000.0 : -1) : t;
        if (shot.t >= 0 && since >= shot.t) {
            Shot(static_cast<IDirect3DDevice9*>(device), shot.label);
            shot.t = -1;
        }
    }
    if (quitAfter > 0 && t >= quitAfter && gameWindow) {
        std::fprintf(stderr, "csf: harness quit at %.1f s\n", t);
        PostMessageA(gameWindow, WM_CLOSE, 0, 0);
        quitAfter = 0;
    }
}
}  // namespace

void csf_harness_install() {
    const std::string mode = Env("CSF_DISPLAY");
    display = mode == "offscreen" ? Display::Offscreen : mode == "window" ? Display::Window : Display::Game;
    csf_harness_offscreen = display == Display::Offscreen;
    if (display != Display::Game) {
        realCreateWindow = g_HarnessCreateWindowExA;
        realShowWindow = g_HarnessShowWindow;
        g_HarnessCreateWindowExA = &HarnessCreateWindow;
        g_HarnessShowWindow = &HarnessShowWindow;
        g_HarnessMoveWindow = &HarnessMoveWindow;
    }
    if (display == Display::Offscreen) {
        g_HarnessSetFocus = &HarnessSetFocus;
        g_HarnessSetForegroundWindow = &HarnessSetForeground;
        g_HarnessBinkSetSoundSystem = &HarnessBinkSetSoundSystem;
        g_HarnessBinkSetSoundTrack = &HarnessBinkSetSoundTrack;
    }
    if (Env("CSF_SKIP_INTRO") == "1")
        realRefreshIntro = Redirect<0x00535900>(reinterpret_cast<void*>(&HarnessRefreshIntro));
    gotoPage = Env("CSF_GOTO");
    mission = Env("CSF_MISSION");
    csf_harness_menu_clock = display != Display::Game;
    if (csf_harness_menu_clock || !gotoPage.empty() || !mission.empty())
        realRequestMenu = RedirectVirtual(CSF_ADDR_VT(0x00864508), 0x58, reinterpret_cast<void*>(&HarnessRequestMenu));
    shotDir = Env("CSF_SHOT_DIR");
    if (!Env("CSF_SHOT_TAG").empty()) shotTag = Env("CSF_SHOT_TAG");
    const std::string shots = Env("CSF_SHOTS");
    for (const char* p = shots.c_str(); *p;) {  // "5,m3": 5 s after the first frame, 3 s after the main menu
        const bool fromMenu = *p == 'm';
        char* end;
        const double t = std::strtod(p + fromMenu, &end);
        if (end == p + fromMenu) break;
        shotTimes.push_back({t, fromMenu, std::string(p, static_cast<const char*>(end))});
        p = *end == ',' ? end + 1 : end;
    }
    if (shotDir.empty()) shotTimes.clear();
    quitAfter = std::atof(Env("CSF_QUIT_AFTER").c_str());
    csf_present_hook = &OnPresent;
}
