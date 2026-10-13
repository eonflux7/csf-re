#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(unsigned int, __fastcall, SceneEntryList_RemoveValueAndNotify, 0x006B1840, (void* self, void* /*edx*/, int item));

// Removes every entry id stored in the object's id array from the scene entry list
// (notifying the list each time), then resets the stored count to zero.
static unsigned int __fastcall C_00840d54_RemoveAllEntriesAndReset(C_00840d54* self, void* /*edx*/)
{
    unsigned int result = self->field_0010;
    for (unsigned int i = 0; i < result; ++i)
    {
        int entryId = static_cast<int>(reinterpret_cast<uint32_t*>(self->field_0008)[i]);
        void* listOwner = reinterpret_cast<void*>(static_cast<intptr_t>(GetGlobalInt()));
        SceneEntryList_RemoveValueAndNotify(listOwner, nullptr, entryId);
        result = self->field_0010;
    }
    self->field_0010 = 0;
    return result;
}

HOOK(0x519810, C_00840d54_RemoveAllEntriesAndReset, "auto");
