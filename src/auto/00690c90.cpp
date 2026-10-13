#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Helper metric evaluated against the object's sub-block at +8 and a 64-bit pair from the caller.
CSF_FUNC(double, __fastcall, sub_6903E0, 0x006903E0, (void* self, void* edx, int a2, int a3, uint64_t a4));

// Returns the squared residual between a float sample and the metric computed by sub_6903E0.
static double __fastcall MemDebug_SquaredResidual(void* self, void* /*edx*/, int a2, float a3, int a4, uint32_t* a5)
{
    // The metric is evaluated with a4 in the ecx slot, a2 forwarded, this+8 as the third argument,
    // and the 64-bit value formed by a5[0] (low) and a5[1] (high) as the last argument.
    uint64_t pairValue = static_cast<uint64_t>(a5[0]) | (static_cast<uint64_t>(a5[1]) << 32);
    int subBlockAddress = static_cast<int>(reinterpret_cast<uintptr_t>(static_cast<uint8_t*>(self) + 8));

    double metric = sub_6903E0(
        reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a4))),
        nullptr,
        a2,
        subBlockAddress,
        pairValue);

    double residual = static_cast<double>(a3) - metric;
    return residual * residual;
}

HOOK(0x690C90, MemDebug_SquaredResidual, "auto");
