#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, DynamicObj_ApplyGlobalStateParams, 0x004427C0,
         (void* self, void* edx, unsigned int a2, int a3, int a4, int a5, int a6));

static int __fastcall DynamicObj_PrepareGlobalState(S_004427c0* self, void* /*edx*/)
{
    return DynamicObj_ApplyGlobalStateParams(self, nullptr, 0x23u, 0, 0, 0, 10);
}

HOOK(0x00442A00, DynamicObj_PrepareGlobalState, "auto");
