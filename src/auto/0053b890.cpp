#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Sound-name string forwarded to the sound manager.
CSF_GLOBAL(const char*, g_pszMenu_P_SoundI, 0x899AC8);

// Member object stored at CMenuOptionsPC::field_0014 (this+0x14) exposes a virtual
// sound/notify method at vtable slot 22 (byte offset 0x58).
// Original: load obj := [this+0x14], push g_pszMenu_P_SoundI, call [obj->vtbl+0x58].
static int __fastcall CMenuOptionsPC_PlaySoundI(CMenuOptionsPC* self, void* /*edx*/)
{
    void* soundManager = reinterpret_cast<void*>(self->field_0014);
    void** vtbl = *reinterpret_cast<void***>(soundManager);
    return ((int(__fastcall*)(void*, void*, const char*))vtbl[0x58 / 4])(
        soundManager, nullptr, g_pszMenu_P_SoundI);
}

HOOK(0x53B890, CMenuOptionsPC_PlaySoundI, "auto");
