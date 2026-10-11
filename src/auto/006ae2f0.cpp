#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_6ADA10, 0x006ADA10, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
[[maybe_unused]] static const auto kBasisSetterReference = CSF_VTBL(0x006ADA10);

static int __fastcall S_006ada10_SetRotation(
    S_006ada10* self, void* /*edx*/, float angleX, float angleY, float angleZ)
{
    const float cosX = std::cos(angleX);
    const float sinX = std::sin(angleX);
    const float cosY = std::cos(angleY);
    const float sinY = std::sin(angleY);
    const float cosZ = std::cos(angleZ);
    const float sinZ = std::sin(angleZ);

    const float sinZSinY = sinZ * sinY;
    const float cosZCosY = cosZ * cosY;
    const float sinZCosY = sinZ * cosY;

    float right[3] = {
        sinZSinY * sinX + cosZCosY,
        sinZ * cosX,
        sinZCosY * sinX - cosZ * sinY
    };
    float up[3] = {
        cosZ * sinY * sinX - sinZCosY,
        cosZ * cosX,
        cosZCosY * sinX + sinZSinY
    };
    float forward[3] = {
        sinY * cosX,
        -sinX,
        cosY * cosX
    };

    return sub_6ADA10(
        self, nullptr,
        reinterpret_cast<uint32_t*>(right),
        reinterpret_cast<uint32_t*>(up),
        reinterpret_cast<uint32_t*>(forward),
        reinterpret_cast<uint32_t*>(&g_ZeroVectorX));
}

HOOK(0x006AE2F0, S_006ada10_SetRotation, "auto");
