#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Reads the entry-table value stored at +4 of the object pointed to by this->field_003c.
// Returns 0 when that value is null.
CSF_FUNC(int, __cdecl, GfxObjectProps_ReadEntryTableValue, 0x006B8880, (int a1));

static int __fastcall C_00860288_GetEntryTableValue(C_00860288* self, void* /*edx*/) {
    void* propsObject = reinterpret_cast<void*>(self->field_003c);
    int entryTableValue = static_cast<int>(CSF_AT(uint32_t, propsObject, 0x4));
    int result = 0;
    if (entryTableValue) {
        return GfxObjectProps_ReadEntryTableValue(entryTableValue);
    }
    return result;
}

HOOK(0x6B2DA0, C_00860288_GetEntryTableValue, "auto");
