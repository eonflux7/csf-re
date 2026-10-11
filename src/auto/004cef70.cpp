#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: int __thiscall sub_4CEF70(C_0083b038* this, std::uint32_t* a2)   ret 4
// If the request code at a2[0] is 25, forwards (a2[1], a2[2], -1, 0) to the
// object at this->field_0014 through vtable slot 0x90 (36). Otherwise returns a2.
static int __fastcall C_0083b038_ForwardRequestIfType25(C_0083b038* self, void* /*edx*/, uint32_t* a2) {
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(a2));
    if (a2[0] == 25) {
        void* target = reinterpret_cast<void*>(self->field_0014);
        uint32_t arg2 = a2[2];
        uint32_t arg1 = a2[1];
        // Virtual slot 0x90 / 4 = 36: vForwardRequest_90(arg1, arg2, -1, 0)
        using ForwardFn = int(__fastcall*)(void*, void*, uint32_t, uint32_t, int, int);
        ForwardFn fn = reinterpret_cast<ForwardFn>(CSF_AT(void**, target, 0)[0x90 / 4]);
        return fn(target, nullptr, arg1, arg2, -1, 0);
    }
    return result;
}

HOOK(0x4CEF70, C_0083b038_ForwardRequestIfType25, "auto");
