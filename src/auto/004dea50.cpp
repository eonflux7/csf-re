#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, C_0083e124_DestroyInnerA, 0x004DE660, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e124_DestroyInnerB, 0x004E00F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e124_FinalizeBase, 0x004DE880, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the two owned sub-blocks (each destroyed, then freed through Mem_Free),
// then forwards to the base finalizer and returns its result.
static int __fastcall C_0083e124_ReleaseOwnedAndFinalize(C_0083e124* self, void* /*edx*/)
{
    (void)CSF_VTBL(0x004DE880);

    uint32_t first = self->field_0018;
    if (first) {
        C_0083e124_DestroyInnerA(reinterpret_cast<void*>(first), nullptr);
        Mem_Free(reinterpret_cast<void*>(first));
        self->field_0018 = 0;
    }

    uint32_t second = self->field_001c;
    if (second) {
        C_0083e124_DestroyInnerB(reinterpret_cast<void*>(second), nullptr);
        Mem_Free(reinterpret_cast<void*>(second));
        self->field_001c = 0;
    }

    // Tail jump to 0x4DE880 with ecx = this, no stack arguments.
    return C_0083e124_FinalizeBase(reinterpret_cast<void*>(self), nullptr);
}

HOOK(0x004DEA50, C_0083e124_ReleaseOwnedAndFinalize, "auto");
