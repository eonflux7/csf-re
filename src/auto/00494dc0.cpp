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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, CPlayerCharacter_SetInteractionState, 0x0048EEC0, (CPlayerCharacter* self, void*, int state));

// Stores a (handle, generation) request record into the object and updates the
// interaction state of the handle it resolves to.
static uint32_t* __fastcall C_008342e8_SetHandleAndUpdateInteraction(C_008342e8* self, void* /*edx*/, int a2)
{
    uint8_t* request = reinterpret_cast<uint8_t*>(a2);
    int result;

    if (CSF_AT(int, request, 8) < 0)
    {
        // Negative request: operate on the currently stored handle slot.
        result = CSF_AT(int, self, 0x2DC);
        if (result >= 0)
        {
            int handleSlot = static_cast<int>(reinterpret_cast<uintptr_t>(self) + 0x2D4);
            int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleSlot);
            result = resolved;
            if (resolved != 0)
            {
                result = CPlayerCharacter_SetInteractionState(
                    reinterpret_cast<CPlayerCharacter*>(resolved), nullptr, 1);
            }
        }
    }
    else
    {
        // Valid request: resolve the handle passed in and clear its interaction state.
        int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, a2);
        result = resolved;
        if (resolved != 0)
        {
            result = CPlayerCharacter_SetInteractionState(
                reinterpret_cast<CPlayerCharacter*>(resolved), nullptr, 0);
        }
    }

    CSF_AT(uint32_t, self, 0x2D8) = CSF_AT(uint32_t, request, 4);
    CSF_AT(uint32_t, self, 0x2DC) = CSF_AT(uint32_t, request, 8);
    return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(result));
}

HOOK(0x00494DC0, C_008342e8_SetHandleAndUpdateInteraction, "auto");
