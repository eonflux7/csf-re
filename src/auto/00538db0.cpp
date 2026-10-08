#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// int __userpurge sub_538A50(int *a1@<ecx>, int a2@<esi>, float a3)  -- ret 4
CSF_FUNC(int, __fastcall, CMenuLoading_SetFadeValue, 0x538A50, (CMenuLoading* self, void* edx, float value));

// Copies field_0048 into field_0030 and field_0034, then drives two fade
// sub-values to 0.0 through CMenuLoading_SetFadeValue; returns the second call's result.
static int __fastcall CMenuLoading_SyncFadeFields(CMenuLoading* self, void* /*edx*/)
{
    float v = self->field_0048;
    self->field_0030 = v;
    self->field_0034 = v;
    CMenuLoading_SetFadeValue(self, nullptr, 0.0f);
    return CMenuLoading_SetFadeValue(self, nullptr, 0.0f);
}

HOOK(0x538DB0, CMenuLoading_SyncFadeFields, "auto");
