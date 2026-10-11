#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static std::uint32_t* __fastcall FxParticleManager_InitializeEntry(
    std::uint32_t* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x0) = 0;
    CSF_AT(std::int32_t, self, 0x4) = -1;
    CSF_AT(std::uint32_t, self, 0xC) = 0;
    CSF_AT(std::uint32_t, self, 0x8) = 0;
    return self;
}

HOOK(0x0051F670, FxParticleManager_InitializeEntry, "auto");
