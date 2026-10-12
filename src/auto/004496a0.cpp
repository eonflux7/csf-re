#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, ForwardRangeWithContext, 0x004491A0,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

static uint32_t* __fastcall S_004482c0_ForwardRange(
    S_004482c0* self, void* /*edx*/,
    uint32_t* first, uint32_t* last, uint32_t* destination)
{
    return ForwardRangeWithContext(
        first, last, destination,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(destination));
}

HOOK(0x004496A0, S_004482c0_ForwardRange, "auto");
