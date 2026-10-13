#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Visitor callback type for RpClumpForAllAtomics: called per atomic with the user data.
using AtomicVisitorFn = void* (__cdecl*)(void* atomic, void* data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));

// Visits every atomic of the clump stored at field_0004, using the
// visitor at 0x6F71D0 with a one-byte flag as user data. Returns that flag.
static char __fastcall C_00862900_ForEachClumpAtomicFlag(C_00862900* self, void* /*edx*/)
{
    char visitFlag = 0;
    RpClumpForAllAtomics(static_cast<int>(self->field_0004),
                         reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006F71D0)),
                         reinterpret_cast<int>(&visitFlag));
    return visitFlag;
}

HOOK(0x006F7200, C_00862900_ForEachClumpAtomicFlag, "auto");
