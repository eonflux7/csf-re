#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Tail-call target: __fastcall(ecx = self, edx unused, one stack arg), returns eax.
CSF_FUNC(void*, __fastcall, sub_459570, 0x00459570, (void* self, void* edx, char a2));

// Original entry: ecx carries an incoming object pointer, a1 is the stack argument.
// The original adjusts ecx by -4 and tail-jumps, so the callee pops the 4-byte argument (ret 4).
static int __fastcall HudStatusIcons_DispatchIcon(void* self, void* /*edx*/, char a1)
{
    uint8_t* adjustedSelf = static_cast<uint8_t*>(self) - 4;
    void* result = sub_459570(adjustedSelf, nullptr, a1);
    return static_cast<int>(reinterpret_cast<uintptr_t>(result));
}

HOOK(0x004590C0, HudStatusIcons_DispatchIcon, "auto");
