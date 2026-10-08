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

// Original function at 0x539BD0: __thiscall (this in ecx, one char argument on the
// stack, callee pops 4 bytes).
CSF_FUNC(std::uint32_t*, __fastcall, Sub_539BD0, 0x539BD0, (std::uint32_t* self, void* edx, char a2));

// 0x539B10 is an adjustor thunk. Its disassembly is:
//     sub ecx, 4
//     jmp 0x539BD0
// i.e. it rebases the incoming `this` (a sub-object pointer sitting 4 bytes into its
// enclosing object) back to the enclosing object and tail-forwards to 0x539BD0 without
// touching the stack. Reproduced here as a __thiscall-compatible method (ecx = this,
// one stack argument popped by the callee) so the `ret 4` cleanup matches.
static std::uint32_t* __fastcall MenuLoading_AdjustThisMinus4(void* self, void* /*edx*/, char a1)
{
    return Sub_539BD0(reinterpret_cast<std::uint32_t*>(reinterpret_cast<uint8_t*>(self) - 4), nullptr, a1);
}

HOOK(0x539B10, MenuLoading_AdjustThisMinus4, "auto");
