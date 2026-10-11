#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));

// Returns the object id stored at this+0x2C through out-param a2 if its low 22 bits are set.
// Otherwise raises status notice 0xE000000E and writes 0.
static uint32_t* __fastcall C_00858180_GetObjectId(C_00858180* self, void* /*edx*/, uint32_t* outId)
{
    uint32_t objectId = self->field_002c;
    if ((objectId & 0x3FFFFF) != 0)
    {
        *outId = objectId;
        return outId;
    }

    Game_RaiseStatusNotice(0, 0, 0xE000000E, 0);
    *outId = 0;
    return outId;
}

HOOK(0x4A1980, C_00858180_GetObjectId, "auto");
