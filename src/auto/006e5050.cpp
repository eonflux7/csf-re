#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards the call to the object's slot +0xC virtual method with a leading 1.0f argument.
// Original: push ebp; mov ebp,esp; mov edx,[ebp+8]; mov eax,[ecx]; push edx; push 0x3f800000; call [eax+0xC]; pop ebp; ret 4
static int __fastcall C_00861bd8_ForwardToSlotCUnitFloat(C_00861bd8* self, void* /*edx*/, int a2) {
    // Slot +0xC is index 3 in the vtable. Arguments: float 1.0f (0x3f800000), then a2.
    using SlotCFn = int(__fastcall*)(void* self, void* edx, float unit, int value);
    SlotCFn slotC = reinterpret_cast<SlotCFn>(CSF_AT(void**, self, 0)[0xC / 4]);
    return slotC(self, nullptr, 1.0f, a2);
}

HOOK(0x6E5050, C_00861bd8_ForwardToSlotCUnitFloat, "auto");
