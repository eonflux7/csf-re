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

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(unsigned int, __fastcall, sub_6E6C70, 0x006E6C70, (void* self, void* edx));

// Destructor body for C_00861d48: resets the derived vtable, releases the two
// owned blocks (+0x3C, then +0x28), and finishes with the base vtable.
static int __fastcall C_00861d48_Destroy(C_00861d48* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00861D48);

    sub_6E6C70(self, nullptr);

    if (CSF_AT(void*, self, 0x3C))
    {
        CoreMemory_Free(CSF_AT(void*, self, 0x3C));
        CSF_AT(void*, self, 0x3C) = nullptr;
    }

    int result = CSF_AT(int, self, 0x28);
    if (result)
    {
        result = CoreMemory_Free(CSF_AT(void*, self, 0x28));
        CSF_AT(void*, self, 0x28) = nullptr;
    }

    *(void***)self = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x6E70A0, C_00861d48_Destroy, "auto");
