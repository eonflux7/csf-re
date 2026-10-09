#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of the 48-byte entry at index entryId in the control's entry array
// (field_00cc = begin, field_00d0 = end), or 0 if the array is empty or the index is out of range.
static int __fastcall CGuiControl_FindEntryById(CGuiControl* self, void* /*edx*/, unsigned int entryId)
{
    uint32_t entryBegin = self->field_00cc;
    if (entryBegin == 0)
        return 0;

    // Signed element count: (end - begin) / 48
    int32_t entryCount = static_cast<int32_t>(self->field_00d0 - entryBegin) / 48;

    // Unsigned comparison against the count, as in the original jae.
    if (entryId >= static_cast<uint32_t>(entryCount) || entryId == 0xFFFFFFFFu)
        return 0;

    return static_cast<int>(entryBegin + 48u * entryId);
}

HOOK_KEEP_ECX(0x00452A30, CGuiControl_FindEntryById, 1, "auto");
