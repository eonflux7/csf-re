#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns one of four integer parameters stored in the object, selected by id.
// Unknown ids return 0.
static int __fastcall MpGameModes_GetIntegerParameter(void* self, void* /*edx*/, int parameterId)
{
    int result = 0;
    switch (parameterId)
    {
    case 20:
        result = CSF_AT(int, self, 0x40);
        break;
    case 21:
        result = CSF_AT(int, self, 0x48);
        break;
    case 29:
        result = CSF_AT(int, self, 0x3C);
        break;
    case 30:
        result = CSF_AT(int, self, 0x44);
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x744240, MpGameModes_GetIntegerParameter, "auto");
