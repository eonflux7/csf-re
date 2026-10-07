#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x004835A0 - __thiscall: forwards to virtual slot at vtable offset 0x134 (slot 77)
// with the fixed arguments (9, 0) and returns its result.
static int __fastcall C_008334f0_InvokeVirtual308(C_008334f0* self, void* /*edx*/)
{
    void** vtbl = reinterpret_cast<void**>(self->vtbl);
    auto fn = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(vtbl[0x134 / 4]);
    return fn(self, nullptr, 9, 0);
}

HOOK(0x4835A0, C_008334f0_InvokeVirtual308, "auto");
