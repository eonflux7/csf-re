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

CSF_FUNC(int, __fastcall, OptionsMenu_ResetEntries, 0x0050DF20, (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszMenu_P_LoGame, 0x00899B08);
CSF_GLOBAL(const char*, g_pszMenu_P_SoundI, 0x00899AC8);
CSF_GLOBAL(const char*, g_pszMenu_P_SaGame, 0x00899B10);
CSF_GLOBAL(const char*, g_pszMenu_P_VideoI, 0x00899AB4);
CSF_GLOBAL(const char*, g_pszMenu_P_ConIng, 0x00899AAC);

static int __fastcall C_008407a8_LoadOptionsMenuLabels(C_008407a8* self, void* /*edx*/)
{
    OptionsMenu_ResetEntries(self, nullptr);

    self->vSlot2C(static_cast<int>(self->field_0110),
                  static_cast<int>(reinterpret_cast<uintptr_t>(g_pszMenu_P_LoGame)));
    self->vSlot2C(static_cast<int>(self->field_0124),
                  static_cast<int>(reinterpret_cast<uintptr_t>(g_pszMenu_P_SoundI)));
    self->vSlot2C(static_cast<int>(self->field_0118),
                  static_cast<int>(reinterpret_cast<uintptr_t>(g_pszMenu_P_SaGame)));
    self->vSlot2C(static_cast<int>(self->field_012c),
                  static_cast<int>(reinterpret_cast<uintptr_t>(g_pszMenu_P_VideoI)));
    // The original's last call is the slot +0x2C one, and the function returns its eax (no `return 0`).
    void** vtbl = self->vtbl;
    return reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(vtbl[0x2c / 4])(
        self, nullptr, static_cast<int>(self->field_0138),
        static_cast<int>(reinterpret_cast<uintptr_t>(g_pszMenu_P_ConIng)));
}

HOOK(0x00511AF0, C_008407a8_LoadOptionsMenuLabels, "auto");
