#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ObjectivePanel_Assign, 0x005150F0, (void* self, void* edx, int value));

// Entry array is [field_0008, field_000c) with 80-byte records. Returns false when the
// array is empty or index is out of range; otherwise assigns value into entry[index].
static bool __fastcall C_00840af8_AssignIndexedEntry(C_00840af8* self, void* /*edx*/, uint32_t index, int value, int /*unused*/)
{
    if (self->field_0008 == 0)
        return false;

    int entryCount = (int)(self->field_000c - self->field_0008) / 80;
    if (index >= (uint32_t)entryCount)
        return false;

    void* entry = reinterpret_cast<void*>(self->field_0008 + 80 * index);
    ObjectivePanel_Assign(entry, nullptr, value);
    return true;
}

HOOK(0x00516780, C_00840af8_AssignIndexedEntry, "auto");
