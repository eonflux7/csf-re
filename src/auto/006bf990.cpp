#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720,
         (std::uint32_t frame));

static std::uint32_t* __fastcall C_00860700_GetFrameForwardVector(
    C_00860700* self, void* /*edx*/, std::uint32_t* output)
{
    std::uint32_t* matrix = RwFrameGetLTM(self->field_000c);

    CSF_AT(std::uint32_t, output, 0x00) =
        CSF_AT(std::uint32_t, matrix, 0x20);
    CSF_AT(std::uint32_t, output, 0x04) =
        CSF_AT(std::uint32_t, matrix, 0x24);
    CSF_AT(std::uint32_t, output, 0x08) =
        CSF_AT(std::uint32_t, matrix, 0x28);

    return output;
}

HOOK(0x006BF990, C_00860700_GetFrameForwardVector, "auto");
