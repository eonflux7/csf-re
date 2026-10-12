#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));
CSF_FUNC(int, __fastcall, C_0085e7b8_AttachWidget, 0x006C8420, (C_0085e7b8* self, void*, C_0085e7b8* widget));

// Attaches the widget embedded at self+8 to the global HUD, notifies the
// object referenced at self+4 through vtable slot 2, then commits the HUD.
static char __fastcall AttachHudWidgetAndCommit(void* self, void* /*edx*/)
{
    uint8_t* globals = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
    C_0085e7b8* hud = CSF_AT(C_0085e7b8*, globals, 4);

    C_0085e7b8_AttachWidget(hud, nullptr, reinterpret_cast<C_0085e7b8*>(static_cast<uint8_t*>(self) + 8));

    void* linkedObject = CSF_AT(void*, self, 4);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, linkedObject, 0)[2])(linkedObject, nullptr);

    return GlobalHud_Commit_6C7C30(hud, nullptr);
}

HOOK(0x006B1AC0, AttachHudWidgetAndCommit, "auto");
