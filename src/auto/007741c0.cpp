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

// Callee: clears the BDD animation table entry list owned by the handle (__fastcall, ecx = table).
CSF_FUNC(int, __fastcall, ClearBddTableEntryList, 0x0051AC90, (void* self, void*));
// Callee: game heap free (__cdecl, caller pops).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Teardown for C_00871a18: restores the class vtable, then clears and frees the
// animation table entry list held at +0x4 (if any) and zeroes the slot.
static int __fastcall C_00871a18_Teardown(C_00871a18* self, void* /*edx*/)
{
    // Vtable store happens first, as in the original.
    CSF_AT(void*, self, 0) = CSF_VTBL(0x00871A18);

    uint32_t tableHandle = self->field_0004;
    int result = 0;
    if (tableHandle)
    {
        ClearBddTableEntryList(reinterpret_cast<void*>(tableHandle), nullptr);
        result = Mem_Free(reinterpret_cast<void*>(tableHandle));
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x7741C0, C_00871a18_Teardown, "auto");
