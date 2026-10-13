#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// C_0082f9f8_StartMovement: __fastcall, one stack arg (ret 4)
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovement_4DB140, 0x004DB140, (C_0082f9f8* self, void* /*edx*/, int movement));

// Starts the two movements on this object when the flag at *flagPtr is clear.
// The first movement targets the sub-object at +0x340; the second targets the
// sub-object at +0x50 of the first call's returned object.
static uint32_t* __fastcall C_0083c198_StartMovementsIfFlagClear(C_0083c198* self, void* /*edx*/, uint32_t* flagPtr)
{
    uint32_t* result = flagPtr;
    if (*flagPtr == 0)
    {
        C_0082f9f8* started = reinterpret_cast<C_0082f9f8*>(static_cast<uintptr_t>(
            static_cast<uint32_t>(C_0082f9f8_StartMovement_4DB140(
                reinterpret_cast<C_0082f9f8*>(self), nullptr,
                static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x340))))));

        int second = C_0082f9f8_StartMovement_4DB140(
            started, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(started) + 0x50)));
        return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(second)));
    }
    return result;
}

HOOK(0x004D4EA0, C_0083c198_StartMovementsIfFlagClear, "auto");
