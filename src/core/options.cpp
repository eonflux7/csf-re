// Global options object (*0x8BAF8C). Fields named by offset until their meaning is known.
#include <cstddef>
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

struct COptions {
    uint8_t unk0;
    uint8_t flag1;      // set by sub_697300
    uint8_t unk2[2];
    legacy::string str4;
    uint8_t unk20[0x41 - 0x20];
    uint8_t flag41;     // set by sub_697390
};
static_assert(offsetof(COptions, str4) == 4);
static_assert(offsetof(COptions, flag41) == 0x41);

CSF_GLOBAL(COptions*, g_pOptions, 0x8BAF8C);

char __cdecl Options_SetFlag1(char v) {
    g_pOptions->flag1 = v;
    return v;
}
HOOK(0x697300, Options_SetFlag1, "core");

const char* __cdecl Options_GetStr4() {
    return g_pOptions->str4.c_str();
}
HOOK(0x697450, Options_GetStr4, "core");
