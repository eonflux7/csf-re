#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// RwFrameGetLTM: returns the frame's LTM matrix pointer (RenderWare).
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));

// Returns the address of the LTM matrix stored inside the frame's owner
// (the 0x30-byte offset into the matrix returned by RwFrameGetLTM).
static int __fastcall FrameGetLtmMatrixAddress(C_00860700* self, void* /*edx*/)
{
    std::uint32_t* ltm = RwFrameGetLTM(self->field_000c);
    return reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(ltm) + 0x30);
}

HOOK(0x006BF450, FrameGetLtmMatrixAddress, "auto");
