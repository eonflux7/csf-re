#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, TypeRegistry_ForwardRangeWithContext, 0x0054D870,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

static uint32_t* __fastcall S_0054d8e0_ForwardRange(
    S_0054d8e0* self, void* /*edx*/,
    uint32_t* first, uint32_t* second, uint32_t* third)
{
    return TypeRegistry_ForwardRangeWithContext(
        first, second, third,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(third));
}

HOOK(0x0054D8E0, S_0054d8e0_ForwardRange, "auto");
