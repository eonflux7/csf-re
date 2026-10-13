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

// Forwards `firstArg` to the virtual slot at byte offset 0xA4 (slot 41) of the
// object's vtable, then returns `firstArg` unchanged to the caller. The second
// stack argument is popped by the callee (ret 8) but never used.
static int __fastcall C_00833b88_ForwardToSlotA4AndReturnFirstArg(C_00833b88* self, void* /*edx*/, int firstArg, int /*secondArg*/) {
    using SlotA4Fn = void(__fastcall*)(void* self, void* edx, int value);
    SlotA4Fn slotA4 = reinterpret_cast<SlotA4Fn>(CSF_AT(void**, self, 0)[0xA4 / 4]);
    slotA4(self, nullptr, firstArg);
    return firstArg;
}

HOOK(0x492890, C_00833b88_ForwardToSlotA4AndReturnFirstArg, "auto");
