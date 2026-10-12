#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, TypeRegistry_ProcessRange, 0x0054C240,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

static uint32_t* __fastcall TypeRegistry_ForwardRange(
    void* self, void* /*edx*/,
    uint32_t* first, uint32_t* second, uint32_t* third)
{
    return TypeRegistry_ProcessRange(
        first, second, third,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(third));
}

HOOK(0x0054CFF0, TypeRegistry_ForwardRange, "auto");
