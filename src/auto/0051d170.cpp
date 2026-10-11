#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static void* __fastcall FxParticleManager_InitializeParticleParameters(
    void* self, void* /*edx*/,
    const std::uint32_t* firstPair,
    const std::uint32_t* secondPair,
    int parameter,
    const std::uint32_t* firstVector,
    const std::uint32_t* secondVector)
{
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint32_t, self, 0x18) = 15;
    CSF_AT(std::uint8_t, self, 0x04) = 0;
    CSF_AT(std::uint32_t, self, 0x1C) = firstPair[0];
    CSF_AT(std::uint32_t, self, 0x20) = firstPair[1];
    CSF_AT(std::uint32_t, self, 0x24) = secondPair[0];
    CSF_AT(std::uint32_t, self, 0x28) = secondPair[1];
    CSF_AT(int, self, 0x2C) = parameter;
    CSF_AT(std::uint32_t, self, 0x30) = firstVector[0];
    CSF_AT(std::uint32_t, self, 0x34) = firstVector[1];
    CSF_AT(std::uint32_t, self, 0x38) = firstVector[2];
    CSF_AT(std::uint32_t, self, 0x3C) = secondVector[0];
    CSF_AT(std::uint32_t, self, 0x40) = secondVector[1];
    CSF_AT(std::uint32_t, self, 0x44) = secondVector[2];
    return self;
}

HOOK(0x0051D170, FxParticleManager_InitializeParticleParameters, "auto");
