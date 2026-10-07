#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the number of 4-byte elements stored in the vector that hangs off
// entry `index` (16 bytes per entry) of the sub-item table at +0x1FC.
// 0 is returned when the entry's vector is empty (begin == nullptr).
static int __fastcall CGuiTextureListBox_GetSubItemCount(CGuiTextureListBox* self, void* /*edx*/, int index)
{
    uint32_t table = self->field_01fc;
    uint32_t entryOffset = static_cast<uint32_t>(index) * 16u;

    uint32_t begin = *reinterpret_cast<uint32_t*>(table + entryOffset + 4);
    if (begin)
    {
        uint32_t end = *reinterpret_cast<uint32_t*>(table + entryOffset + 8);
        return static_cast<int>(end - begin) >> 2;   // (end - begin) / 4, arithmetic shift
    }
    return 0;
}

HOOK(0x730950, CGuiTextureListBox_GetSubItemCount, "auto");
