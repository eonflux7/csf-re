#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Binds to RwFrame's LTM getter (__cdecl, one stack argument).
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));

// Writes {frame LTM pointer, 0} into the caller-provided result buffer and returns it.
// Original: S_006adb40 *__thiscall(C_00860700 *this, int a2); ret 4.
static S_006adb40* __fastcall C_00860700_GetFrameLtm(C_00860700* self, void* /*edx*/, int resultOut) {
    CSF_AT(std::uint32_t*, resultOut, 0) = RwFrameGetLTM(self->field_000c);
    CSF_AT(std::uint8_t, resultOut, 4) = 0;
    return reinterpret_cast<S_006adb40*>(resultOut);
}

HOOK(0x6BF970, C_00860700_GetFrameLtm, "auto");
