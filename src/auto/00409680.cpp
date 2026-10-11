#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082d844_QueryState, 0x004092A0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0082d844_UpdateDependents, 0x00409300, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_0082d844_RecomputeField0010(C_0082d844* self, void* /*edx*/)
{
    if (C_0082d844_QueryState(self, nullptr) == 1)
        self->field_0010 = 0.0f;
    else
        self->field_0010 = self->field_0020 * g_fZero;

    C_0082d844_UpdateDependents(self, nullptr);
}

HOOK(0x00409680, C_0082d844_RecomputeField0010, "auto");
