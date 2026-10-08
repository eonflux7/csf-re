#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// int sub_538A50(int* a1@<ecx>, int a2@<esi>, float a3)  — rets 4
// Second argument lives in esi; it is forwarded untouched from our caller's esi.
CSF_FUNC(int, __fastcall, CMenuLoading_Sub538A50, 0x538A50, (int* a1, void* edx, float a3));

// Adds the delta a3 to the running accumulator (field_0030) and, when the
// running total first exceeds the previously reported peak (field_0034),
// records the new peak and notifies the helper with a 0.0f delta.
static void __fastcall CMenuLoading_AccumulateProgress(CMenuLoading* self, void* /*edx*/, float a3)
{
    float accumulated = a3 + self->field_0030;
    self->field_0030 = accumulated;
    if (accumulated > self->field_0034)
    {
        self->field_0034 = accumulated;
        CMenuLoading_Sub538A50(reinterpret_cast<int*>(self), nullptr, 0.0f);
    }
}

HOOK(0x538D80, CMenuLoading_AccumulateProgress, "auto");
