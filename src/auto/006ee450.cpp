#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_008605d8_ResolveOrDelegate, 0x006BBFA0, (C_008605d8* self, void* , uint32_t* a2, int* a3));

static int __fastcall C_00862084_ResolveOrDelegate(
    C_00862084* self, void* /*edx*/, uint32_t* query, int* result)
{
    if (static_cast<uint8_t>(Hud_CheckFlagAgainstGlobal(
            query, nullptr,
            reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBAE0)))))
    {
        *result = reinterpret_cast<int>(self);
        return self != nullptr;
    }

    C_008605d8_ResolveOrDelegate(
        reinterpret_cast<C_008605d8*>(self), nullptr, query, result);
    return *result != 0;
}

HOOK(0x006EE450, C_00862084_ResolveOrDelegate, "auto");
