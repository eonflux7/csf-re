#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards the call to the object's slot +0xC virtual method with a zero first argument.
// Original: push ebp; mov ebp,esp; ...; push a2; push 0; call [vtbl+0xC]; ret 4
static int __fastcall C_00861bd8_ForwardToSlotCZero(C_00861bd8* self, void* /*edx*/, int a2) {
    // Slot +0xC is index 3 in the vtable. Arguments are passed as raw dwords (0, a2),
    // matching the original push sequence.
    using SlotCFn = int(__fastcall*)(void* self, void* edx, uint32_t zero, int value);
    SlotCFn slotC = reinterpret_cast<SlotCFn>(CSF_AT(void**, self, 0)[0xC / 4]);
    return slotC(self, nullptr, 0u, a2);
}

HOOK(0x6E5070, C_00861bd8_ForwardToSlotCZero, "auto");
