#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Original 0x004DB1D0: __thiscall with one stack argument (ret 4).
CSF_FUNC(void, __fastcall, sub_4DB1D0, 0x004DB1D0, (void* self, void* edx, int a2));

// 0x00421270: __thiscall, one stack argument (ret 4). The argument a2 is unused;
// the original pushes a literal 0 and forwards it to 0x004DB1D0 with the same `this`.
static void __fastcall TFncGirar_ResetWithZeroArg(TFncGirar* self, void* /*edx*/, int /*a2*/)
{
    sub_4DB1D0(self, nullptr, 0);
}

HOOK(0x421270, TFncGirar_ResetWithZeroArg, "auto");
