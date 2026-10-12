#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when the two stored float values differ (also true for NaN),
// 0 only when they compare equal. Matches the original fucompp/test ah,0x44 sequence.
static int __fastcall CCamera_IsProjectionValueChanged(CCamera* self, void* /*edx*/) {
    return self->field_0050 != self->field_005c ? 1 : 0;
}

HOOK(0x006D2AC0, CCamera_IsProjectionValueChanged, "auto");
