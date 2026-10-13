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

// Forwards `value` to the virtual slot at byte offset 0xA4 (slot 41) of the
// object's vtable, then returns `value` unchanged to the caller.
static int __fastcall C_00833b88_ForwardToSlotA4AndReturnArg(C_00833b88* self, void* /*edx*/, int value) {
    using SlotA4Fn = void(__fastcall*)(void* self, void* edx, int value);
    SlotA4Fn slotA4 = reinterpret_cast<SlotA4Fn>(CSF_AT(void**, self, 0)[0xA4 / 4]);
    slotA4(self, nullptr, value);
    return value;
}

HOOK(0x492150, C_00833b88_ForwardToSlotA4AndReturnArg, "auto");
