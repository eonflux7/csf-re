#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall FxParticleManager_GetParticleEntryResult(void* self, void* /*edx*/, int index)
{
    auto* entries = CSF_AT(std::uint8_t*, self, 0x2C);
    void* particle = CSF_AT(void*, entries, static_cast<std::uint32_t>(index) * 20u + 8u);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, particle, 0)[0x2C / 4])(particle, nullptr, 0);
}

HOOK(0x0051F830, FxParticleManager_GetParticleEntryResult, "auto");
