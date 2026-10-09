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

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __fastcall, ApplyStateToggleFade_50DC30, 0x0050DC30, (void* self, void* edx, char a2, float a3));

// Sets the global-state fade (on with 0.04f, off with 0.0f) from the flag
// stored in the object referenced by a3, then records the same on/off value
// in the global state at offset 200.
static int __stdcall UpdateGlobalStateFadeFromObjectFlag(int a1, int a2, int a3)
{
    int result;

    uint32_t* outerSlot = reinterpret_cast<uint32_t*>(a3);
    uint32_t* owner = reinterpret_cast<uint32_t*>(*outerSlot);
    uint8_t* ownerSettings = reinterpret_cast<uint8_t*>(owner[1]);

    if (ownerSettings[1])
    {
        ApplyStateToggleFade_50DC30(reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 1, 0.039999999f);
        result = GetGlobalState_50D190();
        reinterpret_cast<uint8_t*>(result)[200] = 1;
    }
    else
    {
        ApplyStateToggleFade_50DC30(reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 0, 0.0f);
        result = GetGlobalState_50D190();
        reinterpret_cast<uint8_t*>(result)[200] = 0;
    }
    return result;
}

HOOK(0x0045BA60, UpdateGlobalStateFadeFromObjectFlag, "auto");
