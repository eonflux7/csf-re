#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x0073F750, releases the spawn-object reference held at this+0x14.
CSF_FUNC(int, __fastcall, ReleaseSpawnObjectReference, 0x0073F750, (void* self, void* edx));

// Original: 0x0049CF60, base-class teardown reached by a tail jump (ecx = this, no stack args).
CSF_FUNC(int, __fastcall, C_00834198_ChainToBaseTeardown, 0x0049CF60, (void* self, void* edx));

static int __fastcall C_00834198_ReleaseSpawnRefThenChainTeardown(C_00834198* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x18) != 0) {
        ReleaseSpawnObjectReference(reinterpret_cast<void*>(&self->field_0014), nullptr);
    }
    return C_00834198_ChainToBaseTeardown(self, nullptr);
}

HOOK(0x0049E1C0, C_00834198_ReleaseSpawnRefThenChainTeardown, "auto");
