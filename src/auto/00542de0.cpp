#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// CMenuControls::LatchStateFromDelta (0x542DE0)
// __thiscall, one stack argument (source record, ret 4).
// When the state field at +0x2D8 is 1, moves it to 2 (also mirrored at +0x2C8),
// and records whether source->+8 is <= 0 at +0x2CC.
// Returns 2 in the low dword and the flag in the high dword.
static int64_t __fastcall MenuControls_LatchStateFromDelta(void* self, void* /*edx*/, const void* source)
{
    int64_t result = 0;

    if (CSF_AT(int, self, 0x2D8) == 1)
    {
        int32_t latchedResult = 2;
        CSF_AT(int, self, 0x2C8) = latchedResult;

        int32_t sourceValue = CSF_AT(int, source, 0x08);
        int32_t flag = (sourceValue <= 0) ? 1 : 0;

        CSF_AT(int, self, 0x2D8) = latchedResult;
        CSF_AT(int, self, 0x2CC) = flag;

        result = (static_cast<int64_t>(flag) << 32) | static_cast<uint32_t>(latchedResult);
    }

    return result;
}

HOOK(0x542DE0, MenuControls_LatchStateFromDelta, "auto");
