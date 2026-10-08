#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

// Releases the object owned at +0x3C through the helper at 0x6923C0 (ecx = ptr + 4),
// and the two interface objects at +0x2C and +0x54 via their vtable slot 0 (deleting call, arg 1).
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x6923C0, (void* self, void* edx));

static int __fastcall CMenuLoading_ReleaseOwnedObjects(CMenuLoading* self, void* /*edx*/)
{
    // Step 1: object at +0x2C, virtual slot 0 with argument 1, then clear the field.
    if (self->field_002c)
    {
        void* owned = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_002c));
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, owned, 0)[0])(owned, nullptr, 1);
        self->field_002c = 0;
    }

    // Step 2: the result starts as the value at +0x3C. If non-null, release the block at +0x3C + 4
    // and store its return value as the result, then clear the field.
    int result = static_cast<int>(self->field_003c);
    if (result)
    {
        void* block = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_003c + 4));
        result = ReleaseObjectReference(block, nullptr);
        self->field_003c = 0;
    }

    // Step 3: object at +0x54, virtual slot 0 with argument 1; its return value becomes the result.
    if (self->field_0054)
    {
        void* owned = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0054));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, owned, 0)[0])(owned, nullptr, 1);
        self->field_0054 = 0;
    }

    return result;
}

HOOK(0x005384D0, CMenuLoading_ReleaseOwnedObjects, "auto");
