#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound original: RwFrameGetLTM(frame) -> pointer to the frame's 4x4 LTM matrix.
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));

// Copies the three dwords at LTM+0x30 (the translation row) of the frame referenced
// by this->field_000c into the caller-supplied buffer, and returns that buffer.
static std::uint32_t* __fastcall C_00861ce0_CopyFrameLtmTranslation(C_00861ce0* self, void* /*edx*/, std::uint32_t* outBuffer)
{
    // field_000c holds a pointer to an object; the frame handle is the dword at +4 of it.
    std::uint32_t frameHandle = CSF_AT(std::uint32_t, reinterpret_cast<std::uint8_t*>(self->field_000c), 4);

    std::uint8_t* ltm = reinterpret_cast<std::uint8_t*>(RwFrameGetLTM(frameHandle));
    std::uint32_t* translation = reinterpret_cast<std::uint32_t*>(ltm + 48);

    outBuffer[0] = translation[0];
    outBuffer[1] = translation[1];
    outBuffer[2] = translation[2];
    return outBuffer;
}

HOOK(0x006E5A40, C_00861ce0_CopyFrameLtmTranslation, "auto");
