#include <cstdint>
#include "hook.h"

static std::uint32_t* __fastcall ParticleManager_InitializeEmptyWord(
    std::uint32_t* self, void* /*edx*/)
{
    *self = 0;
    return self;
}

HOOK(0x0051F690, ParticleManager_InitializeEmptyWord, "auto");
