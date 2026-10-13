#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Vtable of the grow-array helper's class, referenced by the original's call sites.
[[maybe_unused]] static void* const kGrowArrayVtbl = CSF_VTBL(0x0041E5F0);

// Constructs C_0085e5c4: initializes its own fields, allocates a 16-byte
// helper array, grows the helper and the pointer list to their minimum
// capacities, then appends the helper to the list. Returns this.
static C_0085e5c4* __fastcall C_0085e5c4_Construct(C_0085e5c4* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0085E5C4);

    // Embedded array header at this+0x04 (count/capacity fields live at +0x08..+0x10).
    S_0041e5f0* listHeader = reinterpret_cast<S_0041e5f0*>(&self->field_0004);
    S_0041e5f0* helper = nullptr;

    self->field_0004 = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 10;

    helper = static_cast<S_0041e5f0*>(Mem_Alloc(0x10));
    if (helper) {
        helper->field_0000 = 0;
        helper->field_0004 = 0;
        helper->field_0008 = 0;
        helper->field_000c = 10;
        // helper stays as the allocated object (helper = v4 in the original)
    } else {
        helper = nullptr;
    }

    unsigned int helperCount = helper->field_0004;
    if (helperCount < 1000)
        S_0041e5f0_GrowArrayCapacity(helper, nullptr, static_cast<int>(1000 - helperCount));

    unsigned int listCount = listHeader->field_0004;
    if (listCount < 50)
        S_0041e5f0_GrowArrayCapacity(listHeader, nullptr, static_cast<int>(50 - listCount));

    if (listHeader->field_0008 >= listHeader->field_0004)
        S_0041e5f0_GrowArrayCapacity(listHeader, nullptr, static_cast<int>(listHeader->field_000c));

    uint32_t index = listHeader->field_0008;
    S_0041e5f0** slot = reinterpret_cast<S_0041e5f0**>(listHeader->field_0000 + 4 * index);
    listHeader->field_0008 = index + 1;
    *slot = helper;

    return self;
}

HOOK(0x00691A20, C_0085e5c4_Construct, "auto");
