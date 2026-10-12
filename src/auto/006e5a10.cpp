#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Frame LTM getter bound by address (__cdecl, one stack arg, returns a matrix pointer).
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));

// Writes the frame's LTM pointer into the caller's result buffer, sets the flag byte
// at +4 to 0, and returns the buffer address (eax). The buffer is passed on the stack.
static int __fastcall C_00861ce0_GetFrameLtmResult(C_00861ce0* self, void* /*edx*/, int resultBuffer)
{
    // self->field_000c is a pointer-sized value; the frame pointer lives at +4 of that object.
    std::uint32_t* framePtrSlot = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uint8_t*>(self->field_000c) + 0x4);
    std::uint32_t frame = *framePtrSlot;

    std::uint32_t* ltm = RwFrameGetLTM(frame);

    std::uint8_t* out = reinterpret_cast<std::uint8_t*>(resultBuffer);
    *reinterpret_cast<std::uint32_t*>(out) = reinterpret_cast<std::uint32_t>(ltm);
    out[4] = 0;
    return resultBuffer;
}

HOOK(0x006E5A10, C_00861ce0_GetFrameLtmResult, "auto");
