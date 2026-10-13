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

CSF_FUNC(char, __fastcall, CInputDirectInput_ReleaseDevices, 0x00713400, (void* self, void* edx));

// Shuts down the DirectInput device if one is pending, then clears the
// initialized state. Returns the initialized flag as it was before clearing.
static char __fastcall CInputDirectInput_ShutdownIfActive(CInputDirectInput* self, void* /*edx*/, int /*a2*/)
{
    char result = static_cast<char>(self->field_0c90);
    if (result)
    {
        CInputDirectInput_ReleaseDevices(self, nullptr);
        result = static_cast<char>(self->field_0004);
        if (result)
        {
            self->field_0004 = 0;
            self->field_0028 = 0;
            self->field_022c = 0;
        }
    }
    return result;
}

HOOK(0x007137A0, CInputDirectInput_ShutdownIfActive, "auto");
