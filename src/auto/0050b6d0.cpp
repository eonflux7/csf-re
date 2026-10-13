#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_50A590, 0x0050A590, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, sub_50B0D0, 0x0050B0D0, (void* self, void* edx, void* a2));

static char __fastcall OptionsFile_ValidateThenApply(S_0050a360* self, void* /*edx*/, int a2, int a3)
{
    if (sub_50A590(self, nullptr, a2, a3) && a3 == 8)
    {
        return sub_50B0D0(self, nullptr, reinterpret_cast<void*>(static_cast<uintptr_t>(a2)));
    }
    return 0;
}

HOOK(0x0050B6D0, OptionsFile_ValidateThenApply, "auto");
