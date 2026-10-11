#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: 0x755F50, __thiscall with 4 stack args (ret 12).
// Call site pushes (a4, 1, a3), so the callee's a1 = a3, a3 = 1, arg8 = a4.
CSF_FUNC(unsigned int, __fastcall, RefreshEntryRange, 0x00755F50,
         (void* self, void* edx, char* a1, unsigned int a3, int arg8));

// Given a pointer a3 and a 80-byte sub-entry index computed from the table's
// begin/end pointers, refreshes the range and writes the address of the
// matching 80-byte entry to *out. Returns out.
static uint32_t* __fastcall S_00756260_ResolveEntryAddress(S_00756260* self, void* /*edx*/,
                                                           uint32_t* out, char* a3, int a4)
{
    uint32_t entryBegin = self->field_0004;
    int entryIndex = 0;

    if (entryBegin != 0) {
        int entryCount = (int)(self->field_0008 - entryBegin) / 80;
        if (entryCount != 0) {
            entryIndex = (int)((uint32_t)reinterpret_cast<uintptr_t>(a3) - entryBegin) / 80;
        }
    }

    RefreshEntryRange(self, nullptr, a3, 1u, a4);

    *out = self->field_0004 + 80u * (uint32_t)entryIndex;
    return out;
}

HOOK(0x00756260, S_00756260_ResolveEntryAddress, "auto");
