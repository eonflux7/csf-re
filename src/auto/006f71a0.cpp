#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Visitor invoked by RpClumpForAllAtomics for each atomic in a clump.
using AtomicVisitorFn = int (__cdecl*)(int);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));

// __thiscall with one stack argument (char): picks the atomic visitor by flag
// and walks every atomic of the clump stored at +0x004.
static int __fastcall C_00862900_ApplyAtomicVisitor(C_00862900* self, void* /*edx*/, char useAlternateVisitor)
{
    AtomicVisitorFn visitor = reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006F7140));
    if (!useAlternateVisitor)
        visitor = reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006F7170));

    return RpClumpForAllAtomics(static_cast<int>(self->field_0004), visitor, 0);
}

HOOK(0x006F71A0, C_00862900_ApplyAtomicVisitor, "auto");
