#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fEpsilon, 0x861B98);

// Returns 1 when the float at +0x10 is not less than the epsilon constant (0.000001f).
// A NaN operand yields 0, matching the original's fcomp/fnstsw test of C0.
static int __fastcall C_00861bd8_IsFieldAtLeastEpsilon(C_00861bd8* self, void* /*edx*/) {
    return self->field_0010 >= g_fEpsilon ? 1 : 0;
}

HOOK(0x6E50A0, C_00861bd8_IsFieldAtLeastEpsilon, "auto");
