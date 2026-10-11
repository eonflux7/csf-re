#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// CMenuScreen teardown: re-stamps the intermediate and derived vtable slots at +0x4,
// then the primary CMenuScreen vtable at +0x0, and frees the block when bit 0 of the
// flags argument is set. Returns the object pointer (eax = this).
static void* __fastcall CMenuScreen_ScalarDestroy(CMenuScreen* self, void* /*edx*/, char releaseFlags)
{
    CSF_AT(void*, self, 0x4) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00830488));
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x00830438);
    *(void***)self = CSF_VTBL(0x0083042C);

    if ((releaseFlags & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x0042B2F0, CMenuScreen_ScalarDestroy, "auto");
