#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address: returns -1 when the value is not present in the container at 0x44D9B0.
CSF_FUNC(int, __fastcall, FindIndexInContainer, 0x0044D9B0, (void* self, void* edx, unsigned int a2));

// Bound by address: grows the S_0041e5f0 array storage by the given additional capacity.
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Vtable of S_0041e5f0, referenced by the original at the grow call site.
[[maybe_unused]] static const auto kGrowArrayVtbl = CSF_VTBL(0x0041E5F0);

// a2 is a { uint32_t* data; ...; uint32_t count; } descriptor. For each nonzero value that the
// owner's container recognises (FindIndexInContainer != -1), appends it to the owner's growable list.
static int __fastcall AppendFilteredValuesToList(void* self, void* /*edx*/, uint32_t* a2)
{
    const uint32_t count = a2[2];
    for (uint32_t i = 0; i < count; ++i)
    {
        // The original re-reads the data pointer on every iteration.
        uint32_t* values = reinterpret_cast<uint32_t*>(a2[0]);
        const uint32_t value = values[i];
        if (value == 0)
            continue;

        C_008311f0* owner = CSF_AT(C_008311f0*, self, 0x1c);
        if (FindIndexInContainer(owner, nullptr, value) == -1)
            continue;

        const uint32_t used = owner->field_001c;
        const uint32_t capacity = CSF_AT(uint32_t, owner, 0x18);
        S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(&owner->field_0014);

        if (used >= capacity)
            S_0041e5f0_GrowArrayCapacity(list, nullptr, static_cast<int>(list->field_000c));

        const uint32_t slot = list->field_0008;
        uint32_t* dst = reinterpret_cast<uint32_t*>(list->field_0000 + 4u * slot);
        list->field_0008 = slot + 1;
        *dst = value;
    }
    return static_cast<int>(count);
}

HOOK(0x0044E360, AppendFilteredValuesToList, "auto");
