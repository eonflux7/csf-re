#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __stdcall HudStringShow_UpdateScaledDistance(int objectAddress)
{
    void* object = reinterpret_cast<void*>(objectAddress);
    void* source = CSF_AT(void*, object, 0x78);
    void* measurement =
        reinterpret_cast<void*>(
            (reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, source, 0)[0x28 / 4]))(source, nullptr));

    float divisor = 0.029999999f;
    if (measurement)
    {
        int value =
            (reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, measurement, 0)[0x18 / 4]))(
                    measurement, nullptr, 1);
        if (value)
            divisor = static_cast<float>(value);
    }

    const std::uint32_t difference =
        CSF_AT(std::uint32_t, object, 0x68) -
        CSF_AT(std::uint32_t, object, 0x58);
    const float distance = static_cast<float>(static_cast<std::int32_t>(difference));

    const int globalsAddress = GetGlobalValue_8BC1E0();
    CSF_AT(float, object, 0x48) =
        std::fabs(distance) *
        (CSF_AT(float, reinterpret_cast<void*>(globalsAddress), 0x18EC) / divisor);
    return globalsAddress;
}

HOOK(0x004F1D00, HudStringShow_UpdateScaledDistance, "auto");
