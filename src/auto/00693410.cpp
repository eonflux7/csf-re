#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Growable pointer list embedded at this+8 (data at +8, capacity at +12, count at +16, growth step at +20).
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

// Vtable of the embedded S_0041e5f0 list object (referenced by the original's call site).
[[maybe_unused]] static void* const kGrowableListVtable = CSF_VTBL(0x0041E5F0);

static unsigned int __fastcall CorePaths_AppendNode(void* self, void* /*edx*/, int node)
{
    // Node count at +0x40; fixed inline slots at +0x18 hold the first 10 nodes.
    const uint32_t slotCount = CSF_AT(uint32_t, self, 0x40);
    if (slotCount < 0xA)
    {
        CSF_AT(int, self, 0x18 + slotCount * 4) = node;
        CSF_AT(uint32_t, self, 0x40) = slotCount + 1;
        return slotCount;
    }

    // Overflow: append to the growable list at this+8.
    S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(static_cast<uint8_t*>(self) + 8);
    if (CSF_AT(uint32_t, self, 0x10) >= CSF_AT(uint32_t, self, 0x0C))
    {
        S_0041e5f0_GrowArrayCapacity(list, nullptr, CSF_AT(int, self, 0x14));
    }

    const uint32_t index = CSF_AT(uint32_t, self, 0x10);
    int* elements = reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x08));
    CSF_AT(uint32_t, self, 0x10) = index + 1;
    elements[index] = node;
    return static_cast<unsigned int>(node);
}

HOOK(0x00693410, CorePaths_AppendNode, "auto");
