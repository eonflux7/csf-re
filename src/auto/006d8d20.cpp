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

// Bound callees (addresses from the packet)
CSF_FUNC(uint32_t*, __cdecl, CreateParticleObject, 0x006C9BE0, (int a1));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(int, __fastcall, GfxParticleEmitters_ResolveParticleResource, 0x006E15F0, (void* self, void* , C_0082d410* a2));
CSF_FUNC(int, __fastcall, SetParticleResource, 0x006EFD30, (void* self, void* edx, int a1));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedValue, 0x006C4BF0, (void* self, void* , int key, unsigned int value));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

// Vtable referenced by the original (S_0041e5f0)
[[maybe_unused]] static const auto kS0041e5f0Vtbl = CSF_VTBL(0x0041E5F0);

static int __fastcall C_008611d4_UpdateFourthParticleStream(C_008611d4* self, void* /*edx*/, int a2)
{
    C_0086754c* particleObject = reinterpret_cast<C_0086754c*>(CreateParticleObject(6));

    // Result of the getter is the 'this' for the resolver call.
    int resourceOwnerValue = GetValue_8BB738();
    int resourceId = GfxParticleEmitters_ResolveParticleResource(
        reinterpret_cast<void*>(static_cast<uintptr_t>(resourceOwnerValue)),
        nullptr,
        reinterpret_cast<C_0082d410*>(a2));
    SetParticleResource(particleObject, nullptr, resourceId);

    CSF_AT(uint8_t, particleObject, 0xCC) = 0;

    // vtable slot 8 (0x20), no args
    reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, particleObject, 0)[8])(particleObject, nullptr);

    // vtable slot 10 (0x28), arg 1
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, particleObject, 0)[10])(particleObject, nullptr, 1);

    int currentItemValue = GetCurrentItemCount(particleObject, nullptr);
    void* currentItem = reinterpret_cast<void*>(static_cast<uintptr_t>(currentItemValue));

    // vtable slot 7 (0x1c) getter on currentItem; its result is the 'this' for GfxSkin_SetKeyedValue
    void* skinHandle = reinterpret_cast<void*(__fastcall*)(void*, void*)>(CSF_AT(void**, currentItem, 0)[7])(currentItem, nullptr);
    GfxSkin_SetKeyedValue(skinHandle, nullptr, 6, 1u);

    uint32_t v7 = CSF_AT(uint32_t, self, 0xE0);
    uint32_t v8 = CSF_AT(uint32_t, self, 0xDC);
    S_0041e5f0* array = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0xD8);
    if (v7 >= v8)
        S_0041e5f0_GrowArrayCapacity(array, nullptr, static_cast<int>(array->field_000c));

    uint32_t v10 = array->field_0008;
    C_0086754c** slot = reinterpret_cast<C_0086754c**>(static_cast<uintptr_t>(array->field_0000) + 4u * v10);
    int result = static_cast<int>(v10 + 1);
    array->field_0008 = result;
    *slot = particleObject;
    return result;
}

HOOK(0x006D8D20, C_008611d4_UpdateFourthParticleStream, "auto");
