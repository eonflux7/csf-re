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

// Bound by address; signatures copied from the packet.
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Vtable of S_00492b20, referenced by the original (see 0x004ABE54).
[[maybe_unused]] static const void* const kS00492b20Vtbl = CSF_VTBL(0x00492B20);

// Returns 1 when the cover task is in an active state (type 5, 6 or 19), the
// object at +0x34 reports nonzero from virtual slot 0xD0, and the linked owner's
// descriptor value matches this task's slot at +0x20; otherwise 0.
// Returned as a full 32-bit BOOL (int), matching the original's zeroed eax.
static int __stdcall CoverTask_IsActiveAndLinkedToSlot(void* a1, void* a2)
{
    const int taskType = CSF_AT(int, a2, 0x50);
    if (taskType < 5)
        return 0;
    if (taskType > 6 && taskType != 19)
        return 0;

    void* statusObj = CSF_AT(void*, a2, 0x34);
    const uint8_t statusResult =
        ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, statusObj, 0)[0xD0 / 4])(statusObj, nullptr);
    if (statusResult == 0)
        return 0;

    const int linkedValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    const int ownerAddress = linkedValue ? linkedValue - 4 : 0;
    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(ownerAddress));

    void* linkedPtr = CSF_AT(void*, owner, 0x2C);
    char* descriptor = S_00492b20_GetPtr0x1B4(linkedPtr, nullptr);
    const int descriptorSlot = reinterpret_cast<int*>(descriptor)[1];

    const int taskSlot = CSF_AT(int, a2, 0x20);
    return (descriptorSlot == taskSlot) ? 1 : 0;
}

HOOK(0x004ABE10, CoverTask_IsActiveAndLinkedToSlot, "auto");
