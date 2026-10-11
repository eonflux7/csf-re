#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Enumeration dispatcher from the audio module: runs a DirectSound/SOUNDMAX
// enum helper with a callback and a context pointer.
CSF_FUNC(void, __cdecl, Audio_RunEnumCallback, 0x7DE550,
         (int (__cdecl* callback)(int*, int), int context));

static int __fastcall C_00863ba8_SetEnumContextAndRun(C_00863ba8* self, void* /*edx*/, int context)
{
    int result = context;
    self->field_0024 = static_cast<uint32_t>(context);
    if (context != 0)
    {
        int contextBlock[2];
        contextBlock[0] = context;
        contextBlock[1] = 0;
        Audio_RunEnumCallback(
            reinterpret_cast<int (__cdecl*)(int*, int)>(CSF_ADDR_FN(0x70FBB0)),
            reinterpret_cast<int>(contextBlock));
        return 0;
    }
    return result;
}

HOOK(0x710F50, C_00863ba8_SetEnumContextAndRun, "auto");
