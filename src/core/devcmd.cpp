// Developer command line. The game reads its "command line" from Juego.cfg (.commandLine), not from the process
// arguments, and parses -scene <name>, -log, -instance (CDevCommandLine::Parse, 0x4FB850).
// csf_hook.ini `cmdline <args>` overrides it (`windowed 1` forces windowed mode), e.g. `cmdline -scene Parachut` for unattended smoke tests.
#include <windows.h>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"
#include <cstdio>
#include <cstring>

namespace {
CSF_FUNC(void, __fastcall, String_Assign, 0x429B60, (void* str, void* edx, const char* p, unsigned int len));
CSF_FUNC(char, __fastcall, DevCommandLine_Parse, 0x4FB850, (void* self, void* edx));
CSF_FUNC(uint8_t*, __cdecl, VideoOptions_Get, 0x753A30, ());  // +284: bFullScreen (read by window creation)

// 0x4FB8F0: CDevCommandLine::Set(const char*) — store the text (std::string at +0), then parse it.
char __fastcall DevCommandLine_Set(void* self, void* /*edx*/, const char* text) {
    if (const char* over = hook::ini_string("cmdline")) {
        hook::log("devcmd: command line overridden: %s", over);
        text = over;
    }
    // Runs during startup before the window is created: also the place for `windowed 1`.
    // `windowed 1` (desktop-sized) or `windowed 1280x960` (see RwEngineGetVideoModeInfo_Dev).
    if (const char* w = hook::ini_string("windowed"); w && *w && *w != '0') {
        VideoOptions_Get()[284] = 0;
        hook::log("devcmd: windowed mode forced");
    }
    String_Assign(self, nullptr, text, (unsigned int)strlen(text));
    return DevCommandLine_Parse(self, nullptr);
}
CSF_FUNC(char, __fastcall, GameFlow_SetLoadGame, 0x501A00, (void* self, void* edx, legacy::string* path));
CSF_FUNC(uint8_t*, __cdecl, GameFlow_Get, 0x4FDBB0, ());
CSF_FUNC(void, __cdecl, StlFree, 0x690E60, (void* p));

CSF_FUNC(int, __fastcall, Menus_GotoScreen, 0x71C2A0, (void* self, void* edx, const char* screen));
CSF_FUNC(uint8_t*, __cdecl, App_Get, 0x50D190, ());
CSF_FUNC(uint8_t*, __cdecl, Game_Get, 0x506BD0, ());              // +4: save manager (vtable 0x840920)
CSF_FUNC(uint8_t*, __cdecl, Render_GetSystem, 0x6B1380, ());
CSF_GLOBAL(const char*, g_ScreenGame, 0x899AF0);  // "Menus/P_Juego.fbs": the game screen (app+24, CGameScreen)

template <typename R, typename... A>
R vcall(void* obj, int off, A... a) {
    return ((R(__fastcall*)(void*, void*, A...))(*(void***)obj)[off / 4])(obj, nullptr, a...);
}

CSF_FUNC(void, __fastcall, GameFlow_SetScene, 0x501990, (void* self, void* edx, const char* scene, char readPrp));

legacy::string make_string(const char* s) {
    legacy::string r{};
    r.res = 15;
    String_Assign(&r, nullptr, s, (unsigned int)strlen(s));
    return r;
}
void free_string(legacy::string& s) {
    if (s.res >= 16)
        StlFree(s.bx.ptr);
}

// Menu manager (vtable 0x864508) slot 22 = GotoScreen(name), 0x71C2A0, wrapped for unattended in-game smoke tests.
// csf_hook.ini:
//   scene PATH      start a mission from the beginning, e.g. Maps/FR01/Parachut.scn (the .scn inside Maps/*.pak)
//   loadsave 1.sav  load that savegame (full path, or a name found in Documents\CSF Data\Games\[Games<N>\])
// On the first screen change (the intro) the scene/save is set on CGameFlow (state 1) and the menus go to the game
// screen P_Juego (app+24, CGameScreen) instead, whose update (0x537E60) prepares (0x501B90) and starts
// (0x504970) the scene when state == 1. The briefing is skipped (GameScreen_HasBriefing): no intro, no menus.
bool dev_start() { return hook::ini_string("scene") || hook::ini_string("loadsave"); }

void* g_menus;  // the menu manager (vtable 0x864508), captured on its first GotoScreen; used by the menu tour

int __fastcall Menus_GotoScreen_Dev(void* self, void* /*edx*/, const char* screen) {
    static bool first = true;
    g_menus = self;
    if (first && hook::ini_string("crashtest"))  // deliberate fault, to test crash triage (tools/dbg.sh crash)
        *(volatile int*)nullptr = 0x0BADC0DE;
    if (!first || !dev_start())
        return Menus_GotoScreen(self, nullptr, screen);
    first = false;
    if (const char* scene = hook::ini_string("scene")) {
        GameFlow_SetScene(GameFlow_Get(), nullptr, scene, 1);
    } else {
        // Relative names resolve against the profile's save folder (CSaveManager +92: std::wstring directory);
        // the Load screen also passes full paths.
        char full[MAX_PATH];
        const char* sav = hook::ini_string("loadsave");
        void* saves = CSF_AT(void*, Game_Get(), 4);
        if (saves && !strchr(sav, ':') && sav[0] != '\\') {
            uint8_t* ws = (uint8_t*)saves + 92;
            const wchar_t* dir = CSF_AT(uint32_t, ws, 24) < 8 ? (const wchar_t*)(ws + 4) : CSF_AT(wchar_t*, ws, 4);
            // At intro time this is the base folder (...\\Games\\); profiles live in Games<N>\\ below it.
            snprintf(full, sizeof full, "%ls%s", dir, sav);
            if (GetFileAttributesA(full) == INVALID_FILE_ATTRIBUTES) {
                char pat[MAX_PATH];
                snprintf(pat, sizeof pat, "%lsGames*", dir);
                WIN32_FIND_DATAA fd;
                HANDLE h = FindFirstFileA(pat, &fd);
                for (bool more = h != INVALID_HANDLE_VALUE; more; more = FindNextFileA(h, &fd)) {
                    snprintf(full, sizeof full, "%ls%s\\%s", dir, fd.cFileName, sav);
                    if (GetFileAttributesA(full) != INVALID_FILE_ATTRIBUTES)
                        break;
                }
                if (h != INVALID_HANDLE_VALUE)
                    FindClose(h);
            }
            sav = full;
        }
        legacy::string path = make_string(sav);
        if (void* saves = CSF_AT(void*, Game_Get(), 4))
            vcall<char>(saves, 44, &path);  // CSaveManager::SelectForLoad -> CGameFlow::SetLoadGame
        free_string(path);
    }
    uint32_t state = CSF_AT(uint32_t, GameFlow_Get(), 92);
    hook::log("devcmd: dev start (%s %s) instead of %s -> state %u", hook::ini_string("scene") ? "scene" : "loadsave",
              hook::ini_string("scene") ? hook::ini_string("scene") : hook::ini_string("loadsave"), screen, state);
    int r = Menus_GotoScreen(self, nullptr, state == 1 ? g_ScreenGame : screen);
    return r;
}

// 0x5364D0: CGameScreen::HasBriefing (vtable 0x842038 slot 35) — the briefing panel with "Play Mission" is shown
// when the screen has one, app->+28->+80 is set and the renderer isn't in state +4. Dev start: never.
BOOL __fastcall GameScreen_HasBriefing(uint8_t* self, void* /*edx*/) {
    if (dev_start()) {
        hook::log("devcmd: starting scene '%s'", ((legacy::string*)(GameFlow_Get() + 124))->c_str());
        return FALSE;
    }
    return CSF_AT(uint32_t, self, 0x138) && CSF_AT(uint8_t, CSF_AT(uint8_t*, App_Get(), 0x1C), 0x50) &&
           !CSF_AT(uint8_t, Render_GetSystem(), 4);
}
// RwEngineGetVideoModeInfo (RW, 0x626BE0). `windowed WxH`: windowed (non-exclusive) modes report W x H. The game
// sizes its window from the mode and RW sizes the windowed back buffer from the client area, so both follow.
struct RwVideoMode { int32_t width, height, depth, flags, refRate, format; };
RwVideoMode*(__cdecl* g_RwGetVideoModeInfo)(RwVideoMode*, int);

RwVideoMode* __cdecl RwEngineGetVideoModeInfo_Dev(RwVideoMode* mode, int index) {
    RwVideoMode* r = g_RwGetVideoModeInfo(mode, index);
    int w, h;
    const char* win = hook::ini_string("windowed");
    if (r && win && sscanf(win, "%dx%d", &w, &h) == 2 && !(r->flags & 1 /*rwVIDEOMODEEXCLUSIVE*/))
        r->width = w, r->height = h;
    return r;
}
// Render device (vtable 0x861460) slot 25 = HasModeFor(desired {w, h}) 0x6DC600, called by SetupVideoMode
// (0x6DC000) with the size from the options at device+4/+8. `windowed WxH` replaces that size, so the windowed
// mode (reported as WxH above) is the one selected.
CSF_FUNC(bool, __fastcall, Device_HasModeFor, 0x6DC600, (void* self, void* edx, int32_t* desired));

bool __fastcall Device_HasModeFor_Dev(void* self, void* /*edx*/, int32_t* desired) {
    int w, h;
    const char* win = hook::ini_string("windowed");
    if (win && sscanf(win, "%dx%d", &w, &h) == 2) {
        desired[0] = w, desired[1] = h;
        hook::log("devcmd: video mode %dx%d", w, h);
    }
    return Device_HasModeFor(self, nullptr, desired);
}
// 0x508F50: size the windowed main window so its client area is w x h (<= 0: the render device's size), centered
// horizontally at the top of the screen. `windowed WxH` overrides w x h (matches the video mode chosen above).
CSF_FUNC(void*, __fastcall, Render_GetDevice, 0x652160, (void* sys, void* edx));

BOOL __cdecl Window_FitClientArea(HWND wnd, LONG w, LONG h) {
    int ow, oh;
    const char* win = hook::ini_string("windowed");
    if (win && sscanf(win, "%dx%d", &ow, &oh) == 2)
        w = ow, h = oh;
    RECT r{0, 0, 0, 0};
    r.right = w > 0 ? w : vcall<LONG>(Render_GetDevice(Render_GetSystem(), nullptr), 32);
    r.bottom = h > 0 ? h : vcall<LONG>(Render_GetDevice(Render_GetSystem(), nullptr), 36);
    AdjustWindowRect(&r, 0xCA0000, FALSE);
    int ww = r.right - r.left, hh = r.bottom - r.top;
    return MoveWindow(wnd, (GetSystemMetrics(SM_CXSCREEN) - ww) / 2, 0, ww, hh, TRUE);
}
// Menu tour (coverage of GUI code in unattended smokes). csf_hook.ini:
//   menutour 1500                     open every single-player menu screen in turn, 1500 ms apart
//   menutour 1500 P_Option P_Video    only these (Menus/<name>.fbs)
// Screens are switched once per frame from RwCameraEndUpdate (0x628E40) on the game thread, never from another
// thread (GotoScreen loads the screen). Logs "menutour: <screen>" per step and "menutour: done" at the end.
const char* const kTour[] = {"P_MePrin", "P_Campan", "P_Scenes", "P_LoGame", "P_Profil", "P_Option", "P_Video",
                             "P_VideoA", "P_Sound", "P_Contro", "P_Help", "P_Tactic", "P_TVirtu",
                             "P_SShots", "P_Credits", "P_OptIng", "P_SoundI", "P_VideoI", "P_ConIng", "P_SaGame",
                             "P_Carga", "P_ProgBr", "P_SalDem", "P_Edit", "P_EditG", "P_MsgBox", "P_MePrin"};
// Left out: P_Idioma, P_MeJueg, P_MeCaPa crash the ORIGINAL game when opened out of context (only_module core);
// the multiplayer lobby screens (P_Lb*, P_MeMult, ...) need a NetZ session.
void* (__cdecl* g_RwCameraEndUpdate)(void*);

void menutour_step() {
    static bool parsed, busy;
    static char names[32][24];
    static int n, next_i;
    static DWORD period, next_t;
    if (!parsed) {
        parsed = true;
        const char* v = hook::ini_string("menutour");
        if (!v)
            return;
        period = strtoul(v, const_cast<char**>(&v), 10);
        for (char tok[24]; n < 32 && sscanf(v, " %23s", tok) == 1; v = strstr(v, tok) + strlen(tok))
            strcpy(names[n++], tok);
        if (!n)
            for (const char* t : kTour)
                strcpy(names[n++], t);
        next_t = GetTickCount() + period;
    }
    if (!n || busy || !g_menus || next_i > n || (int)(GetTickCount() - next_t) < 0)
        return;
    if (next_i == n) {
        hook::log("menutour: done");
        ++next_i;
        return;
    }
    char path[64];
    snprintf(path, sizeof path, "Menus/%s.fbs", names[next_i++]);
    hook::log("menutour: %s", path);
    busy = true;  // loading a screen renders frames (re-enters RwCameraEndUpdate)
    Menus_GotoScreen(g_menus, nullptr, path);
    busy = false;
    next_t = GetTickCount() + period;
}

void* __cdecl RwCameraEndUpdate_Dev(void* camera) {
    void* r = g_RwCameraEndUpdate(camera);
    menutour_step();
    return r;
}
}  // namespace

HOOK_TRAMP(0x628E40, RwCameraEndUpdate_Dev, g_RwCameraEndUpdate, "core");
HOOK(0x508F50, Window_FitClientArea, "core");
HOOK_VSLOT(0x861460, 25, &Device_HasModeFor_Dev, "core");
HOOK_TRAMP(0x626BE0, RwEngineGetVideoModeInfo_Dev, g_RwGetVideoModeInfo, "core");
HOOK(0x4FB8F0, DevCommandLine_Set, "core");
HOOK_VSLOT(0x864508, 22, &Menus_GotoScreen_Dev, "core");
HOOK(0x5364D0, GameScreen_HasBriefing, "core");
