#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: 0x46F390, __fastcall (ecx = self, one stack arg), returns a table slot index.
CSF_FUNC(int, __fastcall, LookupEntryIndex, 0x0046F390, (void* self, void* edx, int a2));

// Original: char __thiscall sub_463600(int this, int a2, int a3, int a4); ret 12.
// Clears the flag at +0x81, then if the flag byte pointed to by *(*a4 + 4) + 1 is zero,
// resolves the table entry for the current index at +0x1C and stores it back there.
static char __fastcall RefreshActiveEntryIndex(void* self, void* /*edx*/, int a2, int a3, uint32_t a4)
{
    (void)a2;
    (void)a3;

    *CSF_AT(uint8_t*, self, 0x81) = 0;

    uint32_t descriptorA = *reinterpret_cast<uint32_t*>(a4);
    uint32_t descriptorB = *reinterpret_cast<uint32_t*>(descriptorA + 4);
    uint8_t flagByte = *reinterpret_cast<uint8_t*>(descriptorB + 1);

    if (flagByte == 0)
    {
        int currentIndex = *CSF_AT(int*, self, 0x1C);
        void* owner = *CSF_AT(void**, self, 0x10);
        int slot = LookupEntryIndex(owner, nullptr, currentIndex);

        // Owner is reloaded after the call: the callee may have changed it.
        owner = *CSF_AT(void**, self, 0x10);
        uint32_t tableBase = *CSF_AT(uint32_t*, owner, 0x60);
        int resolvedValue = *reinterpret_cast<int*>(tableBase + static_cast<uint32_t>(slot) * 8u + 4u);

        *CSF_AT(int*, self, 0x1C) = resolvedValue;
        return static_cast<char>(resolvedValue);
    }

    return static_cast<char>(flagByte);
}

HOOK(0x463600, RefreshActiveEntryIndex, "auto");
