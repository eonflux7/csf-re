#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Replaces the sub-object held at +0x10 with `newObj` (may be null).
// The previous holder gets vtable slot 148/4 = 37 called on it; the new
// holder gets its back-pointer and its vtable slot 140/4 = 35 called.
static C_0082f9f8* __fastcall S_004223f0_ReplaceSubObject(S_004223f0* self, void* /*edx*/, int newObj)
{
    // Release the previous sub-object, if any.
    uintptr_t previousBits = static_cast<uintptr_t>(self->field_0010);
    if (previousBits != 0)
    {
        void* previous = reinterpret_cast<void*>(previousBits);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, previous, 0)[37])(previous, nullptr);
    }

    // Store the new sub-object (even when null).
    self->field_0010 = static_cast<uint32_t>(newObj);

    if (newObj != 0)
    {
        void* created = reinterpret_cast<void*>(static_cast<uintptr_t>(newObj));

        // Give the new object our field_0008 value at +8.
        CSF_AT(uint32_t, created, 0x08) = self->field_0008;

        // Give the new object a back-pointer to us at +12.
        void* current = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0010));
        CSF_AT(S_004223f0*, current, 0x0C) = self;

        // Notify the new object via vtable slot 140/4 = 35.
        current = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0010));
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, current, 0)[35])(current, nullptr);
    }

    return reinterpret_cast<C_0082f9f8*>(newObj);
}

HOOK(0x4DB190, S_004223f0_ReplaceSubObject, "auto");
