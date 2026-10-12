#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));

// Returns the frame's LTM matrix pointer advanced by 0x30 bytes.
// The frame handle is stored at [this->field_0024 + 4].
static int __fastcall C_00862748_GetFrameLtmPlus30(C_00862748* self, void* /*edx*/) {
    std::uint8_t* holder = reinterpret_cast<std::uint8_t*>(self->field_0024);
    std::uint32_t frame = CSF_AT(std::uint32_t, holder, 4);
    std::uint32_t* ltm = RwFrameGetLTM(frame);
    return reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(ltm) + 48);
}

HOOK(0x6F3F20, C_00862748_GetFrameLtmPlus30, "auto");
