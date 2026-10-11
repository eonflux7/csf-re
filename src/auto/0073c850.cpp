#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, NetSession_GetLastResultCode, 0x00646DE0, ());

// Calls the net layer for the last result code and stores a flag at this+0x150:
// 0 when the code is one of the listed benign/expected HRESULTs, 1 otherwise.
// The result code is returned unchanged.
static unsigned int __fastcall C_0085839c_UpdateFlagFromNetResult(C_0085839c* self, void* /*edx*/)
{
    unsigned int result = (unsigned int)NetSession_GetLastResultCode();

    if (result > 0xE0030010u) {
        if (result == 0xE0030011u || result == 0xE0030019u) {
            CSF_AT(uint32_t, self, 0x150) = 0;
            return result;
        }
    } else if (result == 0xE0030010u || result == 0xE000000Eu ||
               result == 0xE0000016u || result == 0xE003000Bu) {
        CSF_AT(uint32_t, self, 0x150) = 0;
        return result;
    }

    CSF_AT(uint32_t, self, 0x150) = 1;
    return result;
}

HOOK(0x0073C850, C_0085839c_UpdateFlagFromNetResult, "auto");
