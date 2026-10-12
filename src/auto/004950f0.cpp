#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns true when the inner object can be dispatched: the flag at +0x228 is clear,
// +0x398 is zero, bit 0x2000 of the dword at +0x2E0 is clear, bit 0x10000 (if set)
// requires the byte at +0x3D0 to be nonzero, and the byte at +0x3DE is zero.
// If the flag at +0x228 is set, the object is considered ready.
static bool __fastcall C_00833908_IsReadyForDispatch(C_00833908* self, void* /*edx*/)
{
    bool result = true;

    if (CSF_AT(uint8_t, self, 0x228) == 0)
    {
        uint32_t stateFlags = CSF_AT(uint32_t, self, 0x2E0);

        result = CSF_AT(uint32_t, self, 0x398) == 0
              && (stateFlags & 0x2000) == 0
              && ((stateFlags & 0x10000) == 0 || CSF_AT(uint8_t, self, 0x3D0) != 0)
              && CSF_AT(uint8_t, self, 0x3DE) == 0;
    }

    return result;
}

HOOK(0x004950F0, C_00833908_IsReadyForDispatch, "auto");
