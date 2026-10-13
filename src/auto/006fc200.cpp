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

// Stores the given task collection pointer at this+0x34. The slot is declared
// as float in auto_classes.h, but the original writes the raw pointer bits
// there (LODWORD), so the store goes through CSF_AT with a pointer type.
static void __fastcall C_00862e88_SetExecutingCollection(C_00862e88* self, void* /*edx*/, void* collection) {
    CSF_AT(void*, self, 0x34) = collection;
}

HOOK(0x006FC200, C_00862e88_SetExecutingCollection, "auto");
