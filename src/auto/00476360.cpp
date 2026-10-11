#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall WeaponFirearms_GetState(
    void* self, void* /*edx*/,
    std::uint32_t* value, std::uint32_t* vector, std::uint8_t* flag)
{
    *value = CSF_AT(std::uint32_t, self, 0x54);
    vector[0] = CSF_AT(std::uint32_t, self, 0x58);
    vector[1] = CSF_AT(std::uint32_t, self, 0x5C);
    const int result = CSF_AT(int, self, 0x60);
    vector[2] = static_cast<std::uint32_t>(result);
    *flag = CSF_AT(std::uint8_t, self, 0x15);
    return result;
}

HOOK(0x00476360, WeaponFirearms_GetState, "auto");
