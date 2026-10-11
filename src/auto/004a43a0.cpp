#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns a status code describing the state of the object referenced by this->field_0008.
// Code 62 means the sub-object's flag at +0x1C4 is set; otherwise the value at +0x1D8 maps to 0, 22, 23, or `this`.
static int __fastcall C_00834598_GetStatusCodeFromState(C_00834598* self, void* /*edx*/)
{
    C_00833908* targetObject = reinterpret_cast<C_00833908*>(self->field_0008);

    if (CSF_AT(int, targetObject, 0x1c4) != 0)
        return 62;

    uint32_t stateKind = CSF_AT(uint32_t, targetObject, 0x1d8);
    int result;
    switch (stateKind)
    {
    case 0u:
        result = 0;
        break;
    case 1u:
        result = 22;
        break;
    case 2u:
    case 3u:
        result = 23;
        break;
    default:
        result = reinterpret_cast<int>(self);
        break;
    }
    return result;
}

HOOK(0x4A43A0, C_00834598_GetStatusCodeFromState, "auto");
