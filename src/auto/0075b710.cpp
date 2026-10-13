#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x0051BF70, __fastcall (ecx = this), clears the record-68 list of the table block.
CSF_FUNC(int, __fastcall, BddTables_ClearRecord68List, 0x0051BF70, (void* self, void*));

// Original: 0x00691110, game heap free (__cdecl).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Original: 0x0075B710 (C_0086c1e0 destructor body). ret 4 => one stack argument (a2, flags; bit 0 = free this).
static int** __fastcall C_0086c1e0_Destroy(C_0086c1e0* self, void* /*edx*/, char flags)
{
    uint32_t recordTable = self->field_0004;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0086C1E0);

    if (recordTable) {
        BddTables_ClearRecord68List(reinterpret_cast<void*>(recordTable), nullptr);
        Mem_Free(reinterpret_cast<void*>(recordTable));
        self->field_0004 = 0;
    }

    if ((flags & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<int**>(self);
}

HOOK(0x75B710, C_0086c1e0_Destroy, "auto");
