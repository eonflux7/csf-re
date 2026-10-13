#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

// Vtable of the array helper struct used at this+0x58 (referenced by the original's call site).
[[maybe_unused]] static const void* kGrowArrayVtblRef = CSF_VTBL(0x0041E5F0);

// Returns the index of a2 if already present in the list; otherwise appends a2 and
// returns the new element count (old count + 1).
static unsigned int __fastcall C_00862e88_FindOrAppendId(C_00862e88* self, void* /*edx*/, int a2)
{
    S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(&self->field_0058);

    uint32_t count = list->field_0008;
    if (count != 0)
    {
        uint32_t* data = reinterpret_cast<uint32_t*>(list->field_0000);
        for (unsigned int index = 0; index < count; ++index)
        {
            if (data[index] == static_cast<uint32_t>(a2))
                return index;
        }
    }

    if (list->field_0008 >= list->field_0004)
        S_0041e5f0_GrowArrayCapacity(list, nullptr, list->field_000c);

    uint32_t slot = list->field_0008;
    uint32_t* data = reinterpret_cast<uint32_t*>(list->field_0000);
    uint32_t* target = data + slot;
    unsigned int newCount = slot + 1;
    list->field_0008 = newCount;
    *target = static_cast<uint32_t>(a2);
    return newCount;
}

HOOK(0x006FCA20, C_00862e88_FindOrAppendId, "auto");
