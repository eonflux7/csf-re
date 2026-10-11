#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when the stored count (field_000c) equals the sum computed by the
// object's virtual SumFields slot (vtbl[0x10 / 4]); 0 otherwise.
static int __fastcall C_0085ee28_IsFieldSumMatching(C_0085ee28* self, void* /*edx*/) {
    return self->field_000c == self->vSumFields();
}

HOOK(0x6A2E10, C_0085ee28_IsFieldSumMatching, "auto");
