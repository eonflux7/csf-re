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

// Returns the payload stored under key 5 in the table exposed by the object at field_0040.
// Original: vcall slot 7 (+0x1C) on field_0040 with 5 pushed beforehand; the vcall takes no
// stack args (ret 0), so the 5 stays on the stack and is consumed by FindEntryPayloadByKey (ret 4).
// Returns *payload if found, else 0.
static int __fastcall C_00861ce0_GetPayloadValueForKey5(C_00861ce0* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0040);

    // Virtual call through the owner's vtable slot 0x1C (index 7); no stack args.
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

HOOK(0x6E5560, C_00861ce0_GetPayloadValueForKey5, "auto");
