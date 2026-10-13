#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x0044FC30 CHandleTable::Resolve (__thiscall, ret 4).
// The scene handle table is passed as `this`.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Original 0x0049CF10 C_008342e8_GetDrawState (__thiscall, ret 0).
CSF_FUNC(int, __fastcall, C_008342e8_GetDrawState, 0x0049CF10, (C_008342e8* self, void*));

// Current scene pointer used as the handle table for lookups.
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Float constant 200.0f added to the draw-state value.
CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);

// Returns the link weight for the object whose handle sits at offset +0x0C.
// Yields 100.0 when no valid linked object is found, otherwise draw-state value + 200.0.
static double __fastcall GetLinkedPositionWeight(int* self, void* /*edx*/)
{
    if (self[5] < 0)
        return 100.0;

    int handle = (int)(self + 3);
    int v2 = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
    if (v2 != 0)
    {
        int v3 = CSF_AT(int, reinterpret_cast<uint8_t*>(v2), 0x50);
        if (v3 == 14 || v3 == 15 || v3 == 16 || v3 == 9)
        {
            C_008342e8* v4 = reinterpret_cast<C_008342e8*>(CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
            if (v4 != nullptr)
            {
                int v5 = v4->field_0050;
                if (v5 == 9 || (v5 > 13 && v5 <= 16))
                {
                    int drawState = C_008342e8_GetDrawState(v4, nullptr);
                    float weight = CSF_AT(float, reinterpret_cast<uint8_t*>(drawState), 0x120) + g_Const200_0f;
                    return weight;
                }
            }
        }
    }
    return 100.0;
}

HOOK(0x004AD850, GetLinkedPositionWeight, "auto");
