#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_004ddf20_ApplyArgument, 0x004DE010, (void* self, void* edx, int a2));

static int __fastcall S_004ddf20_InvokeBothIfEnabled(S_004ddf20* self, void* /*edx*/, int /*a2*/, int a3, int a4)
{
    int result = self->field_001c;
    if (result)
    {
        S_004ddf20_ApplyArgument(self, nullptr, a3);
        return S_004ddf20_ApplyArgument(self, nullptr, a4);
    }
    return result;
}

HOOK(0x004DE0D0, S_004ddf20_InvokeBothIfEnabled, "auto");
