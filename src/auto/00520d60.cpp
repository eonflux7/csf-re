#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, FxParticleManager_ResolveUnindexedEntry, 0x005206D0,
         (void* self, void* edx, int arg0));
CSF_FUNC(int, __fastcall, FxParticleManager_ResolveIndexedEntry, 0x00520830,
         (void* self, void* edx, uint32_t* a2));

static int __fastcall S_005206d0_ResolveGlobalEntryToOutput(
    S_005206d0* self, void* /*edx*/, uint32_t* output)
{
    if (CSF_AT(int32_t, output, 0x2C) == -1)
        return FxParticleManager_ResolveUnindexedEntry(
            self, nullptr, static_cast<int>(reinterpret_cast<intptr_t>(output)));

    return FxParticleManager_ResolveIndexedEntry(self, nullptr, output);
}

HOOK(0x00520D60, S_005206d0_ResolveGlobalEntryToOutput, "auto");
