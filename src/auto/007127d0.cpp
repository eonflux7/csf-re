#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: DirectSound/SOUNDMAX audio effect helper. Reads the first dword of the
// config block at owner+0x50 (+0x2C), scales it by a float factor, stores the
// result at owner+0x3C, and forwards it to the owner's +0x30 handle when present.

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

CSF_FUNC(uint32_t*, __cdecl, Audio_PushEffectParam, 0x007E1380,
         (uint32_t* a1, int a2, int a3, ...));

static uint32_t* __fastcall C_00863dd0_ApplyVolumeScale(C_00863dd0* self, void* /*edx*/, float a2)
{
    uint32_t* result = reinterpret_cast<uint32_t*>(self);

    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0004);
    uint32_t configPtr = CSF_AT(uint32_t, owner, 0x50);
    if (configPtr == 0)
        return result;

    // Only the first dword of the config block is used.
    uint8_t* configBlock = reinterpret_cast<uint8_t*>(configPtr);
    int32_t rawSigned = static_cast<int32_t>(CSF_AT(uint32_t, configBlock, 0x2C));

    // Signed int -> float; negative values are corrected by the unsigned bias.
    float scale = static_cast<float>(rawSigned);
    if (rawSigned < 0)
        scale += g_UnsignedIntegerBias;

    float scaledValue = scale * a2;
    CSF_AT(float, owner, 0x3C) = scaledValue;

    uint32_t handle = CSF_AT(uint32_t, owner, 0x30);
    if (handle != 0) {
        uint32_t valueBits;
        std::memcpy(&valueBits, &scaledValue, sizeof(valueBits));
        return Audio_PushEffectParam(reinterpret_cast<uint32_t*>(handle), 4, 1, valueBits);
    }

    return result;
}

HOOK(0x007127D0, C_00863dd0_ApplyVolumeScale, "auto");
