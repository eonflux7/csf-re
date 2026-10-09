#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Maps a CCharWeaponFsm state id (0..7) to its debug name; any other value yields "UNKNOWN".
static const char* __stdcall WeaponFsm_GetStateName(int state) {
    switch (state) {
    case 0:
        return CSF_STR(0x008331F8, "STATE_READY");
    case 1:
        return CSF_STR(0x008331E4, "STATE_CHANGE_ARMA");
    case 2:
        return CSF_STR(0x008331D0, "STATE_SACAR_ARMA");
    case 3:
        return CSF_STR(0x008331C0, "STATE_ATTACK");
    case 4:
        return CSF_STR(0x008331B0, "STATE_RELOAD");
    case 5:
        return CSF_STR(0x00833198, "STATE_WAIT_ARMA_READY");
    case 6:
        return CSF_STR(0x00833180, "STATE_SOLICITAR_CUERPO");
    case 7:
        return CSF_STR(0x00833168, "STATE_SACAR_PROYECTIL");
    default:
        return CSF_STR(0x00833204, "UNKNOWN");
    }
}

HOOK(0x00480740, WeaponFsm_GetStateName, "auto");
