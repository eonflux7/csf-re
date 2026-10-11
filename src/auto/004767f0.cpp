#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall WeaponFirearms_CanFire(void* self, void* /*edx*/)
{
    if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x9C / 4])(self, nullptr))
        return 0;

    if (CSF_AT(std::uint32_t, self, 0x1C) != 0)
        return 1;

    return reinterpret_cast<bool(__fastcall*)(void*, void*)>(
               CSF_AT(void**, self, 0)[0x68 / 4])(self, nullptr)
               ? 1
               : 0;
}

HOOK(0x004767F0, WeaponFirearms_CanFire, "auto");
