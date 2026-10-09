#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, WeaponDefs_IsFireModeSpecial, 0x007612C0, (int a1));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static bool __stdcall WeaponDefs_IsEntryValid(void* weaponDef)
{
    uint8_t* def = static_cast<uint8_t*>(weaponDef);
    bool valid = true;

    if (!CompareStringSubstring(def + 0x3C, nullptr, 0, CSF_AT(unsigned int, def, 0x50),
                                reinterpret_cast<const char*>(CSF_ADDR_DATA(0x82E65C)), 0))
        valid = false;

    if (!CompareStringSubstring(def + 0x58, nullptr, 0, CSF_AT(unsigned int, def, 0x6C),
                                reinterpret_cast<const char*>(CSF_ADDR_DATA(0x82E65C)), 0))
        valid = false;

    int fireMode = CSF_AT(int, def, 0x74);
    if (fireMode == -1)
        valid = false;
    if (CSF_AT(int, def, 0x7C) == -1)
        valid = false;
    if (CSF_AT(int, def, 0xD4) == -1)
        valid = false;
    if (CSF_AT(int, def, 0xD8) == -1)
        valid = false;

    if (CSF_AT(float, def, 0xE8) < g_fZero || CSF_AT(float, def, 0xEC) <= g_fZero)
        valid = false;

    if (CSF_AT(int, def, 0x104) <= 0)
        valid = false;

    if (CSF_AT(float, def, 0x128) <= g_fZero)
        valid = false;

    if ((uint8_t)WeaponDefs_IsFireModeSpecial(fireMode)
        && CSF_AT(float, def, 0x140) == CSF_AT(float, def, 0x148)
        && CSF_AT(float, def, 0x144) == CSF_AT(float, def, 0x14C))
    {
        valid = false;
    }

    return CSF_AT(int, def, 0x150) != -1 && valid;
}

HOOK(0x783430, WeaponDefs_IsEntryValid, "auto");
