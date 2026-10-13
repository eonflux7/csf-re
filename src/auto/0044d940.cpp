#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static double __fastcall S_0044d940_ComputeMarginAdjusted(S_0044d940* self, void* /*edx*/, int arg0, int a2, int a4)
{
    float margin = 2.0f;
    uint8_t* arg0Ptr = reinterpret_cast<uint8_t*>(arg0);
    if (!CSF_AT(uint8_t, arg0Ptr, 0x8))
        margin = 4.0f;
    if (a4 != 4) {
        int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, a2);
        if (resolved) {
            uint8_t* obj = reinterpret_cast<uint8_t*>(resolved);
            if (!CSF_AT(int, obj, 0x50))
                margin = 0.5f;
        }
    }
    return (double)(self->field_0004 - margin);
}

HOOK(0x0044D940, S_0044d940_ComputeMarginAdjusted, "auto");
