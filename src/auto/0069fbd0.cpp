#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_GLOBAL(int, g_008bb400, 0x008BB400);

// Scalar-deleting destructor: resets the global, installs the base vtable,
// and frees the block through CoreMemory_Free when bit 0 of a2 is set.
static void* __fastcall C_0085ebdc_Destruct(void* self, void* /*edx*/, char a2)
{
    g_008bb400 = 0;
    CSF_AT(void**, self, 0) = CSF_VTBL(0x0085EBDC);
    if ((a2 & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x0069FBD0, C_0085ebdc_Destruct, "auto");
