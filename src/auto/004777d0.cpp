#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_InitializeObjectOptions, 0x006B2750, (void* self, void* edx));

static uint32_t* __fastcall WeaponFirearms_InitializeSceneObjectOptions(
    uint32_t* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x00) = 0;
    SceneMissionSetup_InitializeObjectOptions(
        reinterpret_cast<uint8_t*>(self) + 0x04, nullptr);

    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(uint32_t, self, 0x70) = 0;
    CSF_AT(uint32_t, self, 0x74) = 0;
    CSF_AT(uint32_t, self, 0x8C) = 0;
    CSF_AT(uint32_t, self, 0x90) = 15;
    CSF_AT(uint8_t, self, 0x7C) = 0;
    CSF_AT(uint32_t, self, 0x98) = 0;
    CSF_AT(uint32_t, self, 0x9C) = 0;
    CSF_AT(uint32_t, self, 0xA0) = 0;
    CSF_AT(uint8_t, self, 0xA8) = 0;
    CSF_AT(uint32_t, self, 0xAC) = 0;
    CSF_AT(uint32_t, self, 0xB0) = 0;
    CSF_AT(uint32_t, self, 0xB4) = 0;
    CSF_AT(uint8_t, self, 0x64) = 0;
    CSF_AT(uint8_t, self, 0x65) = 0;
    CSF_AT(uint8_t, self, 0x66) = 0;
    CSF_AT(uint32_t, self, 0x00) = 224;
    CSF_AT(int32_t, self, 0x68) = -1;
    return self;
}

HOOK(0x004777D0, WeaponFirearms_InitializeSceneObjectOptions, "auto");
