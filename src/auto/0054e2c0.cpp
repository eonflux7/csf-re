#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, StringFormat_GetWideTextLength, 0x00695560, (uint16_t* text));

static int __fastcall C_008443c8_WriteWideTextIfCapacityAllows(C_008443c8* self, void* /*edx*/, int a2, int /*a3*/)
{
    int wideTextLength = StringFormat_GetWideTextLength(reinterpret_cast<uint16_t*>(a2));

    // Ask the object held in field_0004 (vtable slot 0x40 / 4 = 16) whether it can take this many characters.
    void* capacityTarget = reinterpret_cast<void*>(self->field_0004);
    int result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, capacityTarget, 0)[16])(
        capacityTarget, nullptr, wideTextLength);

    if ((uint8_t)result != 0)
    {
        // Byte count of the wide text (two bytes per character).
        return self->vSlot60(a2, 2 * wideTextLength);
    }
    return result;
}

HOOK(0x0054E2C0, C_008443c8_WriteWideTextIfCapacityAllows, "auto");
