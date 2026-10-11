#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
static CSF_FUNC(int, __fastcall, ParticleManager_ProcessParticleId, 0x005200D0, (void* self, void* edx, int a2));

static int __fastcall ParticleManager_ProcessReferencedParticle(void* self, void* /*edx*/)
{
    const int particleId = CSF_AT(int, self, 0);
    void* manager = reinterpret_cast<void*>(GetGlobalValue_51F740());
    return ParticleManager_ProcessParticleId(manager, nullptr, particleId);
}

HOOK(0x0051F700, ParticleManager_ProcessReferencedParticle, "auto");
