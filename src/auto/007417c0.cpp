#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x64B250 (lib): __thiscall-style method named via ecx, no stack args (ret 0).
CSF_FUNC(int, __fastcall, Sub_64B250, 0x64B250, (void* self, void* edx));

// Adjustor thunk at 0x7417C0:
//   add ecx, 0xFFFFF950   ; ecx = this - 0x6B0  (secondary base -> complete object)
//   jmp 0x64B250          ; tail-call the real method with the adjusted `this`
static int __fastcall AdjustThisMinus6B0Thunk(void* self, void* /*edx*/)
{
    return Sub_64B250(reinterpret_cast<uint8_t*>(self) - 0x6B0, nullptr);
}

HOOK(0x7417C0, AdjustThisMinus6B0Thunk, "auto");
