#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __fastcall, GfxParticleEmitters_InitializeSettings, 0x006DE210,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxParticleEmitters_FinalizeSettings, 0x006DE360,
         (void* self, void* edx));

static S_006dcf90* __cdecl GfxParticleEmitters_CreateDefaultSettings(
    S_006dcf90* destination)
{
    alignas(8) uint8_t settingsBytes[0x154];
    S_006dcf90& settings = *reinterpret_cast<S_006dcf90*>(settingsBytes);

    GfxParticleEmitters_InitializeSettings(&settings, nullptr);
    settings.field_0008 = 1;
    settings.field_000c = 1;
    settings.field_0014 = 1065353216;
    settings.field_0044 = 1132396544;
    settings.field_0050 = 1132396544;
    settings.field_0054 = 1132396544;
    settings.field_0060 = 0;
    settings.field_0070 = 1;
    settings.field_013c = static_cast<uint32_t>(-1027080192);
    settings.field_0140 = static_cast<uint32_t>(-1027080192);
    settings.field_0144 = static_cast<uint32_t>(-1027080192);
    settings.field_0148 = 1120403456;
    settings.field_014c = 1120403456;
    settings.field_0150 = 1120403456;

    std::memcpy(destination, &settings, 0x154);
    GfxParticleEmitters_FinalizeSettings(&settings, nullptr);
    return destination;
}

HOOK(0x006E0820, GfxParticleEmitters_CreateDefaultSettings, "auto");
