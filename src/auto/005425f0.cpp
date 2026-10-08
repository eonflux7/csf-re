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

// 0x0050D190 - returns the global game state pointer (used to read the active flag).
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x50D190, ());

// 0x0073C020 - returns the global object whose preview is updated (the "this" of the update).
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x73C020, ());

// 0x0073EAA0 - __thiscall: (float* this, float factor), callee pops 4 bytes.
CSF_FUNC(int, __fastcall, MenuCharSelect_AdvancePreviewTimer, 0x73EAA0, (float* self, void* edx, float a2));

// Forwards the supplied float to the preview update, but only when the global
// state's activity flag (byte at +0xD0) is set. Otherwise it just returns the state.
static int __stdcall MenuMpCharacterSelect_UpdatePreviewIfActive(float a1)
{
    int state = GetGlobalState_50D190();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(state), 0xd0) != 0)
    {
        return MenuCharSelect_AdvancePreviewTimer(reinterpret_cast<float*>(GetGlobalDword_73C020()), nullptr, a1);
    }
    return state;
}

HOOK(0x005425F0, MenuMpCharacterSelect_UpdatePreviewIfActive, "auto");
