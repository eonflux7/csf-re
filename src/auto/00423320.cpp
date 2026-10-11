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

CSF_FUNC(int, __fastcall, sub_41EE60, 0x0041EE60, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, sub_4220F0, 0x004220F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4DB440, 0x004DB440, (void* self, void* edx, int a2));

// If the owner's current state differs from the cached one, reset the timer counter,
// refresh the two sub-objects, then forward to the base state handler.
static uint32_t* __fastcall C_00834fa8_RefreshIfStateChanged(C_00834fa8* self, void* /*edx*/)
{
    uint32_t* result = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x8));

    uint32_t* owner = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, result, 0x14));
    if (CSF_AT(uint32_t, owner, 0x1c4) != CSF_AT(uint32_t, result, 0x2c0))
    {
        CSF_AT(uint32_t, result, 0x270) = 0;
        sub_4220F0(result, nullptr);

        float blendTime = CSF_AT(float, result, 0x200);
        sub_41EE60(static_cast<void*>(reinterpret_cast<uint8_t*>(result) + 0x29c), nullptr, blendTime);

        int baseArg = static_cast<int>(CSF_AT(uint32_t, self, 0xc) + 20);
        int handled = sub_4DB440(self, nullptr, baseArg);
        return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(handled));
    }
    return result;
}

HOOK(0x423320, C_00834fa8_RefreshIfStateChanged, "auto");
