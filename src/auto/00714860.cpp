#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the number of 4-byte elements in the [field_01ac, field_01b0) range,
// or 0 when the range is empty (field_01ac is null).
static int __fastcall CGuiListBox_GetItemCount(CGuiListBox* self, void* /*edx*/)
{
    uint32_t pBegin = self->field_01ac;
    if (!pBegin)
        return 0;

    return static_cast<int>(self->field_01b0 - pBegin) >> 2;
}

// LTCG callers (0x716FE0) keep a value in ecx across the call: the original never writes ecx.
HOOK_KEEP_ECX(0x714860, CGuiListBox_GetItemCount, 0, "auto");
