#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: HAnim_FindEntryIndexByKey (__cdecl)
CSF_FUNC(int, __cdecl, HAnim_FindEntryIndexByKey, 0x006E8120, (int table, int key));

// Looks up the entry index for a key in the table stored at +0x04 of this object.
static int __fastcall C_00863430_FindEntryIndexByKey(C_00863430* self, void* /*edx*/, int key)
{
    int table = static_cast<int>(self->field_0004);
    return HAnim_FindEntryIndexByKey(table, key);
}

HOOK(0x708440, C_00863430_FindEntryIndexByKey, "auto");
