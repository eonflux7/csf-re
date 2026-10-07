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

// Trivial accessor: loads the signed byte stored at offset 0x23c of the object.
static char __fastcall C_GetByte023C(char* self, void* /*edx*/)
{
    return CSF_AT(char, self, 0x23c);
}

HOOK(0x495490, C_GetByte023C, "auto");
