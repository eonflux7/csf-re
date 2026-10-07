#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x00714830
// Returns the signed count of 12-byte column entries stored between the
// array begin/end pointers at +0x19C / +0x1A0, or 0 when the array is empty.
static int __fastcall CGuiTextureListBox_GetColumnCount(CGuiTextureListBox* self, void* /*edx*/)
{
    const uint32_t begin = self->m_columnsBegin;
    if (begin == 0)
        return 0;
    return static_cast<int>(self->m_columnsEnd - begin) / 12;
}

HOOK(0x00714830, CGuiTextureListBox_GetColumnCount, "auto");
