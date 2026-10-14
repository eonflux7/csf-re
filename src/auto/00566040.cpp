#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, CMenuMpSessions_Destruct, 0x00561930, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Scalar deleting destructor: CMenuMpSessionsPC vtables (+0 0x845D48, +4 0x845CF8), the CMenuMpSessions base
// destructor, then free when flags & 1.
static void* __fastcall CMenuMpSessionsPC_ScalarDelete(void* self, void* /*edx*/, unsigned flags)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_ADDR_VT(0x00845D48);
    CSF_AT(std::uint32_t, self, 0x04) = CSF_ADDR_VT(0x00845CF8);
    CMenuMpSessions_Destruct(self, nullptr);
    if (flags & 1)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x00566040, CMenuMpSessionsPC_ScalarDelete, "auto");
