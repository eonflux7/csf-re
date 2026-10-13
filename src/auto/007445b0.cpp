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

// void* C_008310b8_GetSelf(this) - result unused by the original.
CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));

// Resets the callback-registration state of a C_008310b8 object to its empty
// values and returns the object pointer.
static int __fastcall C_008310b8_ClearCallbackState(C_008310b8* self, void* /*edx*/)
{
    C_008310b8_GetSelf(self, nullptr);

    CSF_AT(uint8_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    self->field_0008 = 0;
    self->field_0004 = 0;
    CSF_AT(uint8_t, self, 0x00) = 0;

    return reinterpret_cast<int>(self);
}

HOOK(0x7445B0, C_008310b8_ClearCallbackState, "auto");
