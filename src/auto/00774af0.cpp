#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x0051AC90, __fastcall (ecx = this), clears the BDD table entry list owned by the block.
CSF_FUNC(int, __fastcall, ClearBddTableEntryList, 0x0051AC90, (void* self, void*));

// Original: 0x00691110, game heap free (__cdecl).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Original: 0x00774AF0 (C_00871a18 destructor body). ret 4 => one stack argument (a2, flags; bit 0 = free this).
static int** __fastcall C_00871a18_Destroy(C_00871a18* self, void* /*edx*/, char flags)
{
    uint32_t tableBlock = self->field_0004;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00871A18);

    if (tableBlock) {
        ClearBddTableEntryList(reinterpret_cast<void*>(tableBlock), nullptr);
        Mem_Free(reinterpret_cast<void*>(tableBlock));
        self->field_0004 = 0;
    }

    if ((flags & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<int**>(self);
}

HOOK(0x774AF0, C_00871a18_Destroy, "auto");
