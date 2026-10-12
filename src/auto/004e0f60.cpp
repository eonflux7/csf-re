#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original vtable reference listed by the packet (0x004E0C20 is also the callee address).
[[maybe_unused]] static const auto S_004e0c20_VtblRef = CSF_VTBL(0x004E0C20);

CSF_FUNC(double, __fastcall, ComputeVectorDistance, 0x004024D0, (void* self, void* edx, float* a2));
CSF_FUNC(char, __fastcall, TraceSubObjectHitToPoint, 0x004E0C20, (void* self, void* edx, uint32_t* a2, float* a3));

static double __fastcall S_004e0c20_DistanceToPoint(S_004e0c20* self, void* /*edx*/, float* a2)
{
    float v6 = 100000000.0f;

    if (self->field_0008 == 3)
    {
        return ComputeVectorDistance(reinterpret_cast<uint8_t*>(self) + 0x10, nullptr, a2);
    }

    alignas(8) uint8_t rayBytes[0x30];
    float* a3 = reinterpret_cast<float*>(rayBytes);
    std::memset(a3, 0, 12);
    a3[3] = 1.0f;
    a3[4] = 0.0f;
    a3[5] = 0.0f;

    [[maybe_unused]] uint8_t v5 = 1;

    if (TraceSubObjectHitToPoint(self, nullptr, reinterpret_cast<uint32_t*>(a2), a3))
    {
        return ComputeVectorDistance(rayBytes, nullptr, a2);
    }

    return static_cast<double>(v6);
}

HOOK(0x004E0F60, S_004e0c20_DistanceToPoint, "auto");
