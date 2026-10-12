#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
static CSF_FUNC(uint32_t*, __cdecl, CreateParticleObject, 0x006C9BE0, (int a1));
static CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, (int a2));
static CSF_FUNC(int, __fastcall, GfxParticleEmitters_ResolveParticleResource, 0x006E15F0, (void* self, void* , int a2));
static CSF_FUNC(int, __fastcall, SetParticleResource, 0x006EFD30, (void* self, void* edx, int a1));

static int __fastcall C_008611d4_UpdateSecondParticleStream(
    C_008611d4* self, void* /*edx*/, int a2)
{
    uint32_t* particle = CreateParticleObject(6);
    int manager = GetValue_8BB738(a2);
    int resolved = GfxParticleEmitters_ResolveParticleResource(
        reinterpret_cast<void*>(manager), nullptr, a2);
    SetParticleResource(particle, nullptr, resolved);

    void** particleVtable = CSF_AT(void**, particle, 0);
    CSF_AT(uint8_t, particle, 0xCC) = 0;
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        particleVtable[0x20 / 4])(particle, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, particle, 0)[0x28 / 4])(particle, nullptr, 1);

    S_0041e5f0* particles = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 0xA8);
    if (CSF_AT(uint32_t, self, 0xB0) >= CSF_AT(uint32_t, self, 0xAC)) {
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            particles, nullptr, particles->field_000c);
    }

    uint32_t count = particles->field_0008;
    uint32_t* destination =
        reinterpret_cast<uint32_t*>(particles->field_0000) + count;
    uint32_t newCount = count + 1;
    particles->field_0008 = newCount;
    *destination = reinterpret_cast<uint32_t>(particle);
    return static_cast<int>(newCount);
}

HOOK(0x006D8BF0, C_008611d4_UpdateSecondParticleStream, "auto");
