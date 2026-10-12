#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x007013B0 (__thiscall, one stack arg: entry pointer, ret 4).
CSF_FUNC(int, __fastcall, sub_7013B0, 0x007013B0, (void* self, void* edx, int a2));

// Vtable referenced next to the call to 0x007013B0 (no runtime effect).
[[maybe_unused]] static const auto kTouchCachedEntryVtbl = CSF_VTBL(0x007013B0);

// For each slot index in self->field_000c[0..self->field_0014), look up the entry
// in a2's table at a2->field_0008 (16-byte records). An index of -1, or an entry
// whose owner is not self, makes the result false. Otherwise the entry's stamp at
// +8 is set from a2->field_0034, and if the entry's +4 is not -1 the cached entry
// is touched. The result is false when the entry's +12 value is 0.
static char __fastcall ParticleSystemDefs_ValidateAndTouchSlots(S_00701d40* self, void* /*edx*/, S_007013b0* a2)
{
    const uint32_t slotCount = self->field_0014;
    char result = 1;

    for (uint32_t i = 0; i < slotCount && result; ++i)
    {
        const int slotIndex = reinterpret_cast<int*>(self->field_000c)[i];
        if (slotIndex == -1)
        {
            result = 0;
            continue;
        }

        const uint32_t entryAddr = a2->field_0008 + 16u * static_cast<uint32_t>(slotIndex);
        uint8_t* entry = reinterpret_cast<uint8_t*>(entryAddr);

        if (CSF_AT(S_00701d40*, entry, 0x0) != self)
        {
            result = 0;
            continue;
        }

        const bool entryNotNegOne = CSF_AT(int, entry, 0x4) != -1;
        CSF_AT(int, entry, 0x8) = static_cast<int>(a2->field_0034);
        const int flag = CSF_AT(int, entry, 0xC);

        if (entryNotNegOne)
        {
            sub_7013B0(static_cast<void*>(a2), nullptr, static_cast<int>(entryAddr));
        }

        if (flag == 0)
        {
            result = 0;
        }
    }

    return result;
}

HOOK(0x00701D40, ParticleSystemDefs_ValidateAndTouchSlots, "auto");
