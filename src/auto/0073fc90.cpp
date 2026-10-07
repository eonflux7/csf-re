#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Target 0x0073FC90 (8 bytes):
//   0073fc90  add ecx, -0x70
//   0073fc93  jmp 0x64b250
//
// This is a thin forwarding thunk: it shifts the incoming `this` pointer
// (ecx) down by 0x70 bytes and tail-calls sub_64B250, which is a __thiscall
// method that pops no stack arguments (ret 0).

// Callee 0x0064B250: __thiscall, `ret 0`.
CSF_FUNC(int, __fastcall, Sub_64B250, 0x64B250, (void* self, void* edx));

static int __fastcall AdjustThisOffsetAndDispatch(void* self, void* /*edx*/)
{
    // The original subtracts 0x70 from ecx before jumping to the callee.
    return Sub_64B250(reinterpret_cast<uint8_t*>(self) - 0x70, nullptr);
}

HOOK(0x73FC90, AdjustThisOffsetAndDispatch, "auto");
