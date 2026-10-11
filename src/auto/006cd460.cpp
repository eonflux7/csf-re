#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowSlotArray, 0x006CD2D0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// S_006cd2d0's vtable is referenced at the GrowSlotArray call site (0x006CD47C).
[[maybe_unused]] static const auto kSlotArrayVtable = CSF_VTBL(0x006CD2D0);

// Appends a new 28-byte slot to the array, growing it first when full, then copies the
// string from a2 into the slot. Returns the slot.
static C_0082d410* __fastcall AppendNamedSlot(S_006cd2d0* self, void* /*edx*/, C_0082d410* a2, int a3)
{
    if (self->field_0008 >= self->field_0004)
    {
        int growBy = a3;
        if (!growBy)
            growBy = static_cast<int>(self->field_000c);
        GrowSlotArray(self, nullptr, growBy);
    }

    // Re-read the count and base after a possible growth.
    std::uint32_t index = self->field_0008;
    auto* slot = reinterpret_cast<C_0082d410*>(self->field_0000 + index * 28u);
    self->field_0008 = index + 1;

    AssignStringSubstring(slot, nullptr, a2, 0u, 0xFFFFFFFFu);
    return slot;
}

HOOK(0x006CD460, AppendNamedSlot, "auto");
