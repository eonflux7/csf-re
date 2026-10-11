#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall FxParticleManager_PrepareSceneResources(void* self, void* /*edx*/)
{
    const uint32_t resourceCount = CSF_AT(uint32_t, self, 0x14);
    int result = 0;
    for (uint32_t i = 0; i < resourceCount; ++i)
    {
        void* resource = CSF_AT(void**, self, 0x0C)[i];
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, resource, 0)[0x10 / 4])(resource, nullptr);
    }
    return result;
}

HOOK(0x0051FAD0, FxParticleManager_PrepareSceneResources, "auto");
