#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetOrientationCode, 0x004092A0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, RefreshDerivedState, 0x00409300, (void* self, void* edx));

// Stores a2 (negated and scaled by field_0020 unless GetOrientationCode returns 1)
// into field_0010 and refreshes derived state when a3 is nonzero, otherwise into
// field_0014 and marks field_001c as set.
static void __fastcall C_0082d844_SetDirectionalValue(C_0082d844* self, void* /*edx*/, float a2, char a3)
{
    float value = a2;
    if (GetOrientationCode(self, nullptr) != 1)
        value = -(value * self->field_0020);

    if (a3) {
        self->field_0010 = value;
        RefreshDerivedState(self, nullptr);
    } else {
        self->field_0014 = value;
        self->field_001c = 1;
    }
}

HOOK(0x004093F0, C_0082d844_SetDirectionalValue, "auto");
