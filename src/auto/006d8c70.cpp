#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(uint32_t*, __cdecl, CreateParticleObject, 0x006C9BE0, (int a1));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(int, __fastcall, GfxParticleEmitters_ResolveParticleResource, 0x006E15F0, (void* self, void* , C_0082d410* a2));
CSF_FUNC(int, __fastcall, SetParticleResource, 0x006EFD30, (void* self, void* edx, int a1));
CSF_FUNC(void*, __fastcall, GfxPtank_GetPlacementState, 0x006F3FE0, (void* self, void* edx, void* a2));

static int __fastcall C_008611d4_UpdateThirdParticleStream(
    C_008611d4* self, void* /*edx*/, int resource)
{
    alignas(8) uint8_t placementBytes[0x28];

    uint32_t* particle = CreateParticleObject(6);
    void* resourceManager = reinterpret_cast<void*>(GetValue_8BB738());
    int particleResource = GfxParticleEmitters_ResolveParticleResource(
        resourceManager, nullptr, reinterpret_cast<C_0082d410*>(resource));
    SetParticleResource(particle, nullptr, particleResource);

    void** particleVtable = CSF_AT(void**, particle, 0);
    CSF_AT(uint8_t, particle, 0xCC) = 0;
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        particleVtable[0x20 / 4])(particle, nullptr);

    void* placementSubobject = reinterpret_cast<uint8_t*>(particle) + 0x20;
    void* placementOwner =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, placementSubobject, 0)[0x8 / 4])(
                placementSubobject, nullptr);
    GfxPtank_GetPlacementState(placementOwner, nullptr, placementBytes);

    float* placementHeight = CSF_AT(float*, placementBytes, 0x24);
    if (*placementHeight < CSF_AT(float, self, 0x9C))
        *placementHeight = CSF_AT(float, self, 0x9C);

    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, particle, 0)[0x28 / 4])(particle, nullptr, 1);

    auto* particles = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 0xB8);

    if (CSF_AT(uint32_t, self, 0xC0) >= CSF_AT(uint32_t, self, 0xBC))
    {
        // Retain the packet's required binding for the growth target.
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            particles, nullptr, particles->field_000c);
    }

    uint32_t count = particles->field_0008;
    auto* destination = reinterpret_cast<uint32_t**>(
        particles->field_0000 + 4 * count);
    uint32_t newCount = count + 1;
    particles->field_0008 = newCount;
    *destination = particle;
    return static_cast<int>(newCount);
}

HOOK(0x006D8C70, C_008611d4_UpdateThirdParticleStream, "auto");
