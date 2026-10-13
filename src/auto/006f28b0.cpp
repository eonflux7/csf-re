#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards to the virtual slot at +0x38 of this object, passing the address
// (*a2 + 0x30) as the single argument.
static int __fastcall C_00862598_ForwardSlot38WithOffset30(C_00862598* self, void* /*edx*/, uint32_t* a2) {
    uint32_t basePointer = *a2;
    uint32_t* argumentAddress = reinterpret_cast<uint32_t*>(basePointer + 0x30);
    using SlotFn = int(__fastcall*)(void*, void*, uint32_t*);
    SlotFn slot38 = reinterpret_cast<SlotFn>(CSF_AT(void**, self, 0)[0x38 / 4]);
    return slot38(self, nullptr, argumentAddress);
}

HOOK(0x6F28B0, C_00862598_ForwardSlot38WithOffset30, "auto");
