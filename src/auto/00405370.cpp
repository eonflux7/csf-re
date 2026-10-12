#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_405180, 0x00405180, (void* self, void* edx, float* a2, float* a3, char a4, char a5, int* a6));

// Vtable address the original references (no store is made here).
[[maybe_unused]] static const void* const s_S00405180VtblRef = CSF_VTBL(0x00405180);

static char __fastcall S_00405180_TestVerticalProbe(S_00405180* self, void* /*edx*/, float* a2, float* a3)
{
    int a6 = -1;
    return sub_405180(self, nullptr, a2, a3, 0, 1, &a6);
}

HOOK(0x00405370, S_00405180_TestVerticalProbe, "auto");
