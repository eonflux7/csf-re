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

CSF_FUNC(int, __fastcall, sub_747CE0, 0x00747CE0, (void* self, void* edx, void* a2));

// Forwards the second stack argument (a2) to the handler at 0x747CE0, using the
// first stack argument (a1) as `this`. The original loads ecx from [ebp+8] and
// pushes [ebp+0xc] as the single argument, then returns the callee's eax with
// `ret 8`.
static int __stdcall NetProyectilDdl_ForwardToHandlerWithSecondArg(std::uint32_t* a1, C_008622e8* a2)
{
    // The original returns the callee's eax unchanged (no `xor eax, eax`).
    return sub_747CE0(a1, nullptr, a2);
}

HOOK(0x747E70, NetProyectilDdl_ForwardToHandlerWithSecondArg, "auto");
