#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the number of 16-byte entries in the [field_0220, field_0224) range,
// or 0 when the range is empty (field_0220 is null).
static int __fastcall CGuiListBox_GetItemCount(CGuiListBox* self, void* /*edx*/)
{
    uint32_t pBegin = self->field_0220;
    if (!pBegin)
        return 0;

    return static_cast<int>(self->field_0224 - pBegin) >> 4;
}

HOOK(0x722E40, CGuiListBox_GetItemCount, "auto");
