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

// Forwards `arg` to the virtual slot at byte offset 0xAC (slot 43) of the
// object's vtable, then returns `arg` unchanged to the caller.
static int __fastcall C_00833b88_ForwardToSlotAcAndReturnArg(C_00833b88* self, void* /*edx*/, int arg) {
    using SlotAcFn = void(__fastcall*)(void* self, void* edx, uint32_t* arg);
    SlotAcFn slotAc = reinterpret_cast<SlotAcFn>(CSF_AT(void**, self, 0)[0xAC / 4]);
    slotAc(self, nullptr, reinterpret_cast<uint32_t*>(arg));
    return arg;
}

HOOK(0x492170, C_00833b88_ForwardToSlotAcAndReturnArg, "auto");
