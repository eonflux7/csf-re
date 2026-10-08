#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Menu string name forwarded to the video-options notifier object.
CSF_GLOBAL(const char*, g_pszMenu_P_VideoI, 0x899AB4);

// Original: obj := [this+0x14]; edx := g_pszMenu_P_VideoI; eax := [obj];
//           push edx; call [eax + 0x58]  -> obj's virtual method (vtable slot 22)
//           receives the menu string and returns its result.
static int __fastcall CMenuOptionsPC_PlayVideoMenuSound(CMenuOptionsPC* self, void* /*edx*/)
{
    void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014));
    void** vtbl = *reinterpret_cast<void***>(obj);
    return ((int(__fastcall*)(void*, void*, const char*))vtbl[0x58 / 4])(
        obj, nullptr, g_pszMenu_P_VideoI);
}

HOOK(0x53B8A0, CMenuOptionsPC_PlayVideoMenuSound, "auto");
