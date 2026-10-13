#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Scalar deleting destructor for C_00845368 (menu_mp_accounts).
// Resets the vtable, clears the byte flag at +0x4 if set, and optionally
// frees the object through the game heap.

CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* a1));

static C_00845368* __fastcall C_00845368_Destroy(C_00845368* self, void* /*edx*/, char a2)
{
    uint8_t flag = self->field_0004;

    *reinterpret_cast<uint8_t**>(self) = reinterpret_cast<uint8_t*>(CSF_VTBL(0x00845368));

    if (flag)
        self->field_0004 = 0;

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x0055BD60, C_00845368_Destroy, "auto");
