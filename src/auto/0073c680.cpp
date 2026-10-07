#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint8_t, g_FloatTripletReceiver, 0x008BABFC);

CSF_FUNC(int, __fastcall, ForwardFloatTriplet, 0x0068F6D0,
         (void* self, void* edx, float first, float second, float third));

static int __fastcall ForwardFloatTripletToGlobalObject(
    void* /*ecx*/, void* edx, float first, float second, float third)
{
    [[clang::musttail]]
    return ForwardFloatTriplet(
        &g_FloatTripletReceiver, edx, first, second, third);
}

HOOK(0x0073C680, ForwardFloatTripletToGlobalObject, "auto");
