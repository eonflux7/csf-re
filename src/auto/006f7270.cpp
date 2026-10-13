#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Visitor callback type used by RpClumpForAllAtomics. Not declared in the project headers,
// so it is defined here, file-local. Real RenderWare signature: RpAtomic* (*)(RpAtomic*, void*).
using AtomicVisitorFn = void* (__cdecl*)(void* atomic, void* data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));

static int __fastcall C_00862900_ApplyVisitorToClumpAtomics(C_00862900* self, void* /*edx*/, char visitorArg) {
    // The visitor receives &visitorArg as its data pointer, as the original pushes the address of the argument.
    return RpClumpForAllAtomics(static_cast<int>(self->field_0004),
                                reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006F7230)),
                                reinterpret_cast<int>(&visitorArg));
}

HOOK(0x006F7270, C_00862900_ApplyVisitorToClumpAtomics, "auto");
