#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, sub_46F390, 0x0046F390, (void* self, void* edx, int a2));

// Reads the index stored at +0x1C, asks sub_46F390 for a slot number using
// that index together with the owner at +0x10, then loads the 32-bit value at
// offset +4 of the 8-byte entry in the table at owner->+0x60 and stores it back
// into +0x1C. Returns that value. The stack arguments (a2..a4) are unused.
static int __fastcall AdvanceIndexedSlotValue(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, int /*a4*/)
{
    // push [self+0x1C]; ecx = [self+0x10]; call sub_46F390
    int slotIndex = sub_46F390(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10)),
                               nullptr,
                               CSF_AT(int, self, 0x1C));

    // ecx reloaded from [self+0x10] after the call; edx = [owner+0x60]
    uint32_t tableBase = CSF_AT(uint32_t, reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10)), 0x60);

    // result = *(table + slotIndex*8 + 4)
    int result = CSF_AT(int, reinterpret_cast<void*>(tableBase),
                        static_cast<uint32_t>(slotIndex) * 8u + 4u);

    CSF_AT(int, self, 0x1C) = result;
    return result;
}

HOOK(0x00463640, AdvanceIndexedSlotValue, "auto");
