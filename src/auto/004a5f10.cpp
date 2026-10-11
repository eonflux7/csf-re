#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4A2FD0, 0x004A2FD0, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(void*, __fastcall, sub_4A5D20, 0x004A5D20, (void* self, void* edx));

static int __fastcall C_00834598_ForwardAndRefreshOnSuccess(C_00834598* self, void* /*edx*/, int a2, int a3, int a4, int a5)
{
    int result = sub_4A2FD0(self, nullptr, a2, a3, a4, a5);
    if (result != -1)
        sub_4A5D20(self, nullptr);
    return result;
}

HOOK(0x004A5F10, C_00834598_ForwardAndRefreshOnSuccess, "auto");
