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

// Copies the dword pointed to by a2 into this+0x38 and the dword pointed to
// by a3 into this+0x3C, then returns a3. Callee pops 8 bytes (two stack args).
static uint32_t* __fastcall C_008673b0_CopyTwoDwordsAtOffsets38And3C(
    C_008673b0* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    self->field_0038 = *a2;
    self->field_003c = *a3;
    return a3;
}

HOOK(0x738BE0, C_008673b0_CopyTwoDwordsAtOffsets38And3C, "auto");
