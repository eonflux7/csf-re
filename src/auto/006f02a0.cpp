#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(std::uint32_t*, __cdecl, CreateParticleObject, 0x006C9BE0, (int type));
CSF_FUNC(int, __fastcall, GfxParticlePtank_SelectSkinRegion, 0x006E0390, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxParticlePtank_GetPreviousParticleObject, 0x006F01B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GfxParticlePtank_ConfigureParticleObject, 0x006F0540, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static int __fastcall GfxParticlePtank_CreateAndAttachParticleObject(
    void* self, void* /*edx*/, int parameter)
{
    // Address classified as a vtable reference by the packet.
    (void)CSF_VTBL(0x0041E5F0);

    auto* bytes = static_cast<std::uint8_t*>(self);
    const int registry = SceneLoader_GetObjectRegistry(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    void* particle;
    if (CSF_AT(std::uint8_t, self, 0xAC))
    {
        particle = reinterpret_cast<void*>(
            GfxParticlePtank_SelectSkinRegion(
                reinterpret_cast<void*>(registry), nullptr, 7));
    }
    else
    {
        particle = CreateParticleObject(7);
    }

    if (particle)
    {
        int previousParticle = 0;
        if (CSF_AT(std::uint32_t, self, 0x94))
        {
            previousParticle = GfxParticlePtank_GetPreviousParticleObject(
                bytes - 0x20, nullptr);
        }

        GfxParticlePtank_ConfigureParticleObject(
            particle, nullptr,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(bytes + 4)),
            previousParticle);

        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, particle, 0)[0x20 / 4])(particle, nullptr);

        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, particle, 0)[0x38 / 4])(
                particle, nullptr, parameter);

        if (!reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                CSF_AT(void**, bytes - 0x20, 0)[0x34 / 4])(
                    bytes - 0x20, nullptr))
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, particle, 0)[0x28 / 4])(
                    particle, nullptr, 1);
        }

        auto* particles = reinterpret_cast<S_0041e5f0*>(bytes + 0x8C);
        if (CSF_AT(std::uint32_t, self, 0x94) >=
            CSF_AT(std::uint32_t, self, 0x90))
        {
            S_0041e5f0_GrowArrayCapacity(
                particles, nullptr, particles->field_000c);
        }

        const std::uint32_t index = particles->field_0008;
        auto* destination =
            reinterpret_cast<std::uint32_t*>(particles->field_0000) + index;
        particles->field_0008 = index + 1;
        *destination = static_cast<std::uint32_t>(
            reinterpret_cast<std::uintptr_t>(particle));
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(particle));
}

HOOK(0x006F02A0, GfxParticlePtank_CreateAndAttachParticleObject, "auto");
