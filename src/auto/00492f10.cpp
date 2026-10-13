#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Linked object at +0x34 has a virtual predicate at vtable byte offset 0xB0 (slot 44).
static bool __fastcall CallLinkedObjectPredicate(void* linkedObj)
{
    using PredicateFn = bool(__fastcall*)(void* self, void* edx);
    PredicateFn predicate = reinterpret_cast<PredicateFn>(CSF_AT(void**, linkedObj, 0)[0xB0 / 4]);
    return predicate(linkedObj, nullptr);
}

static int __fastcall C_00833b88_IsReadyForNewCommand(C_00833b88* self, void* /*edx*/)
{
    C_008672e4* linkedObj = reinterpret_cast<C_008672e4*>(self->field_0034);
    int result = 0;

    if (!linkedObj || !CallLinkedObjectPredicate(linkedObj))
    {
        // Sign bit of the flags word must be set.
        int flags = CSF_AT(int, self, 0x1BC);
        if (flags < 0 && self->field_0080 == 0)
        {
            uint32_t mode = CSF_AT(uint32_t, self, 0x1D8);
            if (mode == 0 && CSF_AT(int, self, 0x1C4) != 1)
            {
                return 1;
            }
            if (mode == 2)
            {
                return 1;
            }
        }
    }
    return result;
}

HOOK(0x492F10, C_00833b88_IsReadyForNewCommand, "auto");
