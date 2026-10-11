#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static char __fastcall FxParticleManager_CopyFourWords(
    void* self, void* /*edx*/, const void* source)
{
    CSF_AT(uint32_t, self, 0x00) = CSF_AT(uint32_t, source, 0x00);
    CSF_AT(uint32_t, self, 0x04) = CSF_AT(uint32_t, source, 0x04);
    CSF_AT(uint32_t, self, 0x08) = CSF_AT(uint32_t, source, 0x08);
    CSF_AT(uint32_t, self, 0x0C) = CSF_AT(uint32_t, source, 0x0C);
    return 1;
}

HOOK(0x0051F6A0, FxParticleManager_CopyFourWords, "auto");
