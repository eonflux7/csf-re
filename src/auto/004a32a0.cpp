#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x004A2DA0: __thiscall, 2 stack args, ret 8. Returns a char flag (test al, al).
CSF_FUNC(char, __fastcall, sub_4A2DA0, 0x004A2DA0, (void* self, void* edx, int a2, unsigned int a3));

// 0x004A2CD0: __thiscall, 3 stack args, ret 12.
CSF_FUNC(int, __fastcall, sub_4A2CD0, 0x004A2CD0, (void* self, void* edx, int a2, int a3, int a4));

// Returns true (1) when the slot at this + a3*16 + 0xC already equals a2. Otherwise it
// asks sub_4A2DA0 whether the change is allowed; if it is, it applies the change via
// sub_4A2CD0 and returns true. Returns false when sub_4A2DA0 refuses.
static char __fastcall C_008344f0_TrySetSlotValue(C_008344f0* self, void* /*edx*/, int a2, int a3, unsigned int a4)
{
    // Slot entries are 16 bytes apart, starting at +0xC.
    int* slotValue = reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + a3 * 16 + 0xC);
    if (a2 == *slotValue)
        return 1;

    if (!sub_4A2DA0(self, nullptr, a3, a4))
        return 0;

    sub_4A2CD0(self, nullptr, a2, a3, a4);
    return 1;
}

HOOK(0x004A32A0, C_008344f0_TrySetSlotValue, "auto");
