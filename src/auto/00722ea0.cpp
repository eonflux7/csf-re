#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiListBox_GetRowElementCount(
    CGuiListBox* self, void* /*edx*/, int rowIndex)
{
    const uint32_t recordAddress =
        self->field_0220 + (static_cast<uint32_t>(rowIndex) << 4);
    const void* record = reinterpret_cast<const void*>(recordAddress);

    const uint32_t begin = CSF_AT(uint32_t, record, 0x4);
    if (begin == 0)
        return 0;

    const uint32_t end = CSF_AT(uint32_t, record, 0x8);
    return static_cast<int32_t>(end - begin) / 60;
}

HOOK(0x00722EA0, CGuiListBox_GetRowElementCount, "auto");
