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

// Stores the sound config string into the sub-object at +0x22A4.
CSF_FUNC(char, __fastcall, GameConfig_SetSoundConfigString, 0x00756950, (void* self, void* edx, int a2));

// Applies one sound config entry (value/address, pad at +8, flag 1); __thiscall, ret 12.
CSF_FUNC(char, __fastcall, GameConfig_ApplySoundEntry, 0x0069DB80, (void* self, void* edx, int a2, int a3, unsigned int a4));

CSF_GLOBAL(const char*, g_pszConfig_sonidos, 0x008A8AEC);

static char __fastcall C_0086a3d8_ApplySoundConfigAndRefresh(C_0086a3d8* self, void* /*edx*/)
{
    GameConfig_SetSoundConfigString(self->pad_22a4, nullptr, reinterpret_cast<int>(g_pszConfig_sonidos));

    if (self->field_22a0 < 0x10)
    {
        return GameConfig_ApplySoundEntry(self, nullptr,
            reinterpret_cast<int>(&self->field_228c),
            reinterpret_cast<int>(self->pad_0008),
            1u);
    }
    else
    {
        return GameConfig_ApplySoundEntry(self, nullptr,
            static_cast<int>(self->field_228c),
            reinterpret_cast<int>(self->pad_0008),
            1u);
    }
}

HOOK(0x756A40, C_0086a3d8_ApplySoundConfigAndRefresh, "auto");
