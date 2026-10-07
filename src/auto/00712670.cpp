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

// Returns the float stored at offset 0x70 of the object pointed to by field_0004,
// widened to the double return value the original ABI produces (fld dword).
static double __fastcall C_00863dd0_GetFloatAt70(C_00863dd0* self, void* /*edx*/)
{
    uint32_t inner = self->field_0004;
    void* inner_ptr = reinterpret_cast<void*>(inner);
    return CSF_AT(float, inner_ptr, 0x70);
}

HOOK(0x00712670, C_00863dd0_GetFloatAt70, "auto");
