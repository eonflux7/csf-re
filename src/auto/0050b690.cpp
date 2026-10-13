#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable referenced by the original's call into S_0050a440's method (0x0050A440).
[[maybe_unused]] static auto s_S0050a440Vtbl = CSF_VTBL(0x0050A440);

CSF_GLOBAL(void*, g_pMenuManager, 0x008B0570);

CSF_FUNC(int, __fastcall, OptionsFile_RefreshState, 0x0050A440, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(char, __cdecl, OptionsFile_ApplyBoolFlag, 0x00743960, (char a1));

static uint8_t* __fastcall OptionsFile_SyncMenuVisibility(S_0050a440* self, void* /*edx*/)
{
    OptionsFile_RefreshState(self, nullptr);

    uint8_t* result = reinterpret_cast<uint8_t*>(g_pMenuManager);
    if (g_pMenuManager) {
        result = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
        if (CSF_AT(uint8_t, result, 0xD0)) {
            char applied = OptionsFile_ApplyBoolFlag(static_cast<char>(self->field_0010 == 0));
            return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint8_t>(applied)));
        }
    }
    return result;
}

HOOK(0x0050B690, OptionsFile_SyncMenuVisibility, "auto");
