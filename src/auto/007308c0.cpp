#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the number of 16-byte entries in the [field_01fc, field_0200) range,
// or 0 when the range is empty (field_01fc is null).
static int __fastcall CGuiTextureListBox_GetItemCount(CGuiTextureListBox* self, void* /*edx*/)
{
    uint32_t pBegin = self->field_01fc;
    if (!pBegin)
        return 0;

    return static_cast<int>(self->field_0200 - pBegin) >> 4;
}

HOOK(0x7308C0, CGuiTextureListBox_GetItemCount, "auto");
