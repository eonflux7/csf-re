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

CSF_FUNC(char, __fastcall, sub_70A5D0, 0x0070A5D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, sub_81BB40, 0x0081BB40, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, sub_6FF970, 0x006FF970, (void* self, void* edx));

// Releases the water shader state owned by C_00863588: frees the child
// resource at +0x04, and the RwMatrix/buffer block at +0x34, then runs the
// base-class teardown.
static void __fastcall C_00863588_ReleaseResources(C_00863588* self, void* /*edx*/)
{
    const uint8_t needsExtraCleanup = self->field_0011;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00863588);

    if (needsExtraCleanup)
        sub_70A5D0(self, nullptr);

    if (self->field_0004)
    {
        sub_81BB40(static_cast<int>(self->field_0004));
        self->field_0004 = 0;
    }

    uint32_t blockAddress = self->field_0034;
    if (blockAddress)
    {
        if (CSF_AT(uint8_t, blockAddress, 4))
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(CSF_AT(uint32_t, blockAddress, 0)));
        Mem_Free(reinterpret_cast<void*>(blockAddress));
        self->field_0034 = 0;
    }

    sub_6FF970(self, nullptr);
}

HOOK(0x0070A780, C_00863588_ReleaseResources, "auto");
