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

CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));

// Returns the payload stored under key 5 in the table exposed by the object at field_0068.
// Original: vcall slot 7 (+0x1C) on field_0068 with the key 5 left on the stack, then
// FindEntryPayloadByKey(table, 5); returns *payload if found, else 0.
static int __fastcall C_00860288_GetPayloadValueForKey5(C_00860288* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0068);

    // Virtual call through the owner's vtable slot 0x1C (index 7); takes no stack args
    // (the key 5 pushed by the original is consumed by FindEntryPayloadByKey below).
    auto getTable = reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, owner, 0)[7]);
    C_0085e7b8* table = reinterpret_cast<C_0085e7b8*>(getTable(owner, nullptr));

    const int payloadKey = 5;
    int found = C_0085e7b8_FindEntryPayloadByKey(table, nullptr, payloadKey);
    if (found != 0)
    {
        int* payload = reinterpret_cast<int*>(found);
        return *payload;
    }
    return 0;
}

HOOK(0x6B29F0, C_00860288_GetPayloadValueForKey5, "auto");
