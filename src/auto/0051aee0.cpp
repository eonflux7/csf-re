#include <cstdint>
#include "globals.h"
#include "hook.h"

static CSF_FUNC(uint8_t*, __cdecl, BddTables_DispatchByteRange, 0x0051ADD0,
               (uint8_t* a1, uint8_t* a2, uint8_t* a3, int a4, int a5));

static uint8_t* __fastcall BddTables_ForwardByteRange(
    void* self, void* /*edx*/, uint8_t* first, uint8_t* second, uint8_t* third)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    return BddTables_DispatchByteRange(
        first, second, third, static_cast<int>(context),
        static_cast<int>(context & 0xFFFFFF00u));
}

HOOK(0x0051AEE0, BddTables_ForwardByteRange, "auto");
