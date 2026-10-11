#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bound by address: forwards the state-transition request for `this`.
CSF_FUNC(int, __fastcall, sub_4DB440, 0x004DB440, (void* self, void* edx, int a2));

// __thiscall method written as __fastcall with a dummy edx. The original pops
// one stack argument (ret 4) but never reads it, so it is left unnamed here.
// Returns the state code reported by the owner's actor object. When that code
// is not 11, forwards `this` plus (field_000c + 32) to sub_4DB440.
static int __fastcall C_00834fa8_ForwardUnlessState11(C_00834fa8* self, void* /*edx*/, int /*unusedArg*/) {
    // Owner pointer stored at this+0x08.
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));

    // Actor object pointer stored at owner+0x14.
    uint8_t* actor = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14));

    // Virtual call at vtable offset 0x30 (slot 12), no arguments.
    int result = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, actor, 0)[12])(actor, nullptr);

    if (result != 11) {
        int target = static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 0x20);
        return sub_4DB440(self, nullptr, target);
    }
    return result;
}

HOOK(0x420BF0, C_00834fa8_ForwardUnlessState11, "auto");
