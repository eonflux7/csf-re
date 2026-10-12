#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Constructor of the screen-effect object (__fastcall: ecx=this, edx unused).
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* edx));

// Allocates a 196-byte screen-effect object, runs its base constructor and
// installs the vtables and zeroed member state. Returns nullptr if allocation fails.
static C_00860178* __cdecl ScreenEffects_CreateInstance()
{
    C_00860178* instance = reinterpret_cast<C_00860178*>(Mem_Alloc(196));
    if (!instance)
        return nullptr;

    C_00860178_Construct(instance, nullptr);

    CSF_AT(std::uint32_t, instance, 0x04) = CSF_ADDR_DATA(0x0082D428);
    CSF_AT(std::uint8_t, instance, 0x08) = 0;
    CSF_AT(std::uint32_t, instance, 0x54) = 0;
    CSF_AT(std::uint32_t, instance, 0x58) = 0;
    CSF_AT(std::uint32_t, instance, 0x5C) = 0;
    *reinterpret_cast<void***>(instance) = CSF_VTBL(0x00862C74);
    CSF_AT(void**, instance, 0x04) = CSF_VTBL(0x00862C58);

    CSF_AT(std::uint32_t, instance, 0x88) = 0;
    CSF_AT(std::uint32_t, instance, 0x8C) = 0;
    CSF_AT(std::uint32_t, instance, 0x90) = 0;
    CSF_AT(std::uint32_t, instance, 0x98) = 0;
    CSF_AT(std::uint32_t, instance, 0x9C) = 0;
    CSF_AT(std::uint32_t, instance, 0xA0) = 0;
    CSF_AT(std::uint32_t, instance, 0xA8) = 0;
    CSF_AT(std::uint32_t, instance, 0xAC) = 0;
    CSF_AT(std::uint32_t, instance, 0xB0) = 0;
    CSF_AT(std::uint32_t, instance, 0xB8) = 0;
    CSF_AT(std::uint32_t, instance, 0xBC) = 0;
    CSF_AT(std::uint32_t, instance, 0xC0) = 0;

    return instance;
}

HOOK(0x006FB580, ScreenEffects_CreateInstance, "auto");
