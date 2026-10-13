#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Visitor callback type for RpClumpForAllAtomics: called per atomic with the user data.
using AtomicVisitorFn = void* (__cdecl*)(void* atomic, void* data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));

// Visits every atomic of the clump stored at field_0004 with the visitor at 0x6F7290,
// using a one-byte flag as user data. Returns that flag.
static char __fastcall C_00862900_VisitClumpAtomicsWithSecondVisitor(C_00862900* self, void* /*edx*/)
{
    char visitFlag = 0;
    RpClumpForAllAtomics(static_cast<int>(self->field_0004),
                         reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006F7290)),
                         reinterpret_cast<int>(&visitFlag));
    return visitFlag;
}

HOOK(0x006F72D0, C_00862900_VisitClumpAtomicsWithSecondVisitor, "auto");
