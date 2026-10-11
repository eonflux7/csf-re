#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// DirectSound/RW audio effect release: the callee takes the handle, a callback (null here) and a flag (0).
CSF_FUNC(int, __cdecl, AudioEffect_ReleaseHandle, 0x7DEF60, (void* handle, void* callback, int flags));

static int __fastcall C_00863ac0_ReleaseHandleAndClear(C_00863ac0* self, void* /*edx*/) {
    int result = static_cast<int>(self->field_0008);
    if (result) {
        result = AudioEffect_ReleaseHandle(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), nullptr, 0);
        self->field_0008 = 0;
    }
    return result;
}

HOOK(0x70F280, C_00863ac0_ReleaseHandleAndClear, "auto");
