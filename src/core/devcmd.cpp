// Developer command line. The game reads its "command line" from Juego.cfg (.commandLine), not from the process
// arguments, and parses -scene <name>, -log, -instance (CDevCommandLine::Parse, 0x4FB850).
// csf_hook.ini `cmdline <args>` overrides it (`windowed 1` forces windowed mode), e.g. `cmdline -scene Parachut` for unattended smoke tests.
#include <windows.h>
#include "globals.h"
#include "hook.h"
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
    // `windowed 1`: the window takes the size of the game's video mode (Settings > resolution); a smaller
    // window than the mode fails (RW: "Failed to create the Frame Buffer").
    if (const char* w = hook::ini_string("windowed"); w && *w && *w != '0') {
        VideoOptions_Get()[284] = 0;
        hook::log("devcmd: windowed mode forced");
    }
    String_Assign(self, nullptr, text, (unsigned int)strlen(text));
    return DevCommandLine_Parse(self, nullptr);
}
}  // namespace

HOOK(0x4FB8F0, DevCommandLine_Set, "core");
