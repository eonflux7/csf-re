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

// Callee: clears the record-68 list owned by a BddTables sound table (__fastcall, ecx = table).
CSF_FUNC(int, __fastcall, BddTables_ClearRecord68List, 0x0051BF70, (void* self, void*));
// Callee: game heap free (__cdecl, caller pops).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets C_0086c1e0 to its base state: restores the class vtable, then releases the
// owned sound table stored at +0x4 (if any) and clears the slot.
static int __fastcall C_0086c1e0_ResetBaseState(C_0086c1e0* self, void* /*edx*/)
{
    // Vtable store happens first, as in the original.
    CSF_AT(void*, self, 0) = CSF_VTBL(0x0086C1E0);

    S_0075b5e0* ownedTable = reinterpret_cast<S_0075b5e0*>(self->field_0004);
    int result = 0;
    if (ownedTable)
    {
        BddTables_ClearRecord68List(ownedTable, nullptr);
        result = Mem_Free(ownedTable);
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x0075B650, C_0086c1e0_ResetBaseState, "auto");
