#include "hook.h"

static int __cdecl WeaponFirearms_MapWeaponType(int weaponType)
{
    switch (weaponType)
    {
    case 1:
    case 6:
    case 21:
        return 8;
    case 7:
        return 9;
    case 8:
        return 10;
    case 10:
        return 11;
    case 16:
        return 14;
    case 17:
        return 15;
    case 18:
        return 16;
    case 19:
        return 17;
    case 24:
        return 20;
    case 25:
        return 21;
    default:
        return -1;
    }
}

HOOK(0x00478880, WeaponFirearms_MapWeaponType, "auto");
