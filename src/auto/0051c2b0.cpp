#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

static bool __stdcall FxParticleManager_IsExplosionParticle(std::uint32_t* particle)
{
    const int type = CSF_AT(int, particle, 0x04);
    if (type == 14 || type == 15)
        return true;

    if (type == 4 && CSF_AT(int, particle, 0x10) >= 0)
    {
        const int resolved = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(particle) + 0x08));
        if (resolved && CSF_AT(int, reinterpret_cast<void*>(resolved), 0x50) == 4)
            return CSF_AT(std::uint32_t, particle, 0x60) == 12;
    }
    return false;
}

HOOK(0x0051C2B0, FxParticleManager_IsExplosionParticle, "auto");
