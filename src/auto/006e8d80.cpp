#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callees (bound by address, names/signatures per packet)
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentAnim, 0x0061F050, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8BBB28, 0x006F7820, ());
CSF_FUNC(int*, __fastcall, HAnimInstance_Create, 0x006F7880, (void* self, void* edx, uint32_t* a2, int a3, int a4));

// Returns the cached instance pointer in a2[1], creating it on first use, and seeds the
// interpolator's current animation from this->field_0020 when that interpolator has none yet.
static uint32_t* __fastcall HAnim_CreateInstanceAndSetInitialAnim(uint32_t* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t* result = reinterpret_cast<uint32_t*>(a2[1]);
    if (result)
        return result;

    // Gather creation parameters before the factory call (same read order as the original).
    uint32_t* paramA = reinterpret_cast<uint32_t*>(a2[0]);
    int paramB = static_cast<int>(CSF_AT(uint32_t, self, 0x18));
    int paramC = static_cast<int>(a2[6]);

    void* factoryContext = reinterpret_cast<void*>(GetValue8BBB28());
    uint32_t* created = reinterpret_cast<uint32_t*>(
        HAnimInstance_Create(factoryContext, nullptr, paramA, paramB, paramC));
    a2[1] = reinterpret_cast<uintptr_t>(created);

    // Interpolator of the new instance: created->[8]->[0x20]
    uint32_t* interpolator = CSF_AT(uint32_t*, CSF_AT(uint32_t*, created, 8), 0x20);
    if (*interpolator == 0)
        RtAnimInterpolatorSetCurrentAnim(interpolator, reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x20)));

    // Reload the cached instance (the callee may have changed it) and its interpolator: [a2[1]+12]->[0x20]
    result = CSF_AT(uint32_t*, CSF_AT(uint32_t*, reinterpret_cast<uint32_t*>(a2[1]), 12), 0x20);
    if (*result == 0)
        return reinterpret_cast<uint32_t*>(
            RtAnimInterpolatorSetCurrentAnim(result, reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x20))));

    return result;
}

HOOK(0x6E8D80, HAnim_CreateInstanceAndSetInitialAnim, "auto");
