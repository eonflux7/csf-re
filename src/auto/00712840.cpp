// Reimplementation of 0x00712840: refreshes the cached gain float (object +0x3c)
// from the first dword of the parameter block at +0x50 (block offset +0x2C), then
// forwards the float's bit pattern to 0x7E1380 when the sub-object at +0x30 is set.
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

CSF_FUNC(uint32_t*, __cdecl, sub_7E1380, 0x007E1380, (uint32_t* a1, int a2, int a3, ...));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static uint32_t* __fastcall C_00863dd0_RefreshEffectGain(C_00863dd0* self, void* /*edx*/) {
    uint8_t* sub = reinterpret_cast<uint8_t*>(self->field_0004);
    uint32_t paramBlockAddr = CSF_AT(uint32_t, sub, 0x50);
    if (paramBlockAddr) {
        uint8_t* paramBlock = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(paramBlockAddr));

        // Only the first dword of the 7-dword block is used by the original.
        int32_t raw = CSF_AT(int32_t, paramBlock, 0x2C);

        // Signed conversion; negative values get the unsigned bias added.
        float gain = static_cast<float>(raw);
        if (raw < 0) {
            gain += g_UnsignedIntegerBias;
        }

        CSF_AT(float, sub, 0x3C) = gain;

        if (CSF_AT(uint32_t, sub, 0x30)) {
            int32_t gainBits = 0;
            std::memcpy(&gainBits, &gain, sizeof(gainBits));
            sub_7E1380(reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, sub, 0x30)), 4, 1, gainBits);
        }
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00712840, C_00863dd0_RefreshEffectGain, "auto");
