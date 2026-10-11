#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

static bool __stdcall FxParticleManager_AreEffectHandlesActive(std::uint32_t* effect)
{
    const int type = CSF_AT(int, effect, 0x04);
    if ((type != 4 && type != 5) || CSF_AT(int, effect, 0x10) < 0)
        return false;

    bool firstActive = true;
    const int firstObject = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(effect) + 0x08));
    if (firstObject)
        firstActive = CSF_AT(std::uint32_t, reinterpret_cast<void*>(firstObject), 0x50) != 0;

    if (CSF_AT(int, effect, 0x1C) >= 0)
    {
        const int secondObject = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(effect) + 0x14));
        if (secondObject &&
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(secondObject), 0x50) == 0)
            return false;
    }

    return firstActive;
}

HOOK(0x0051DB70, FxParticleManager_AreEffectHandlesActive, "auto");
