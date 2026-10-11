#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original: int __thiscall sub_6AD950(std::uint32_t* this, std::uint32_t* a2), ret 4
CSF_FUNC(int, __cdecl, RwMatrixUpdate, 0x00624620, (int a1));

static int __fastcall GfxMatrix_CopyVec3AndUpdate(uint32_t* self, void* /*edx*/, uint32_t* src) {
    uint32_t* dst = reinterpret_cast<uint32_t*>(self[0]);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    return RwMatrixUpdate(static_cast<int>(self[0]));
}

HOOK(0x006AD950, GfxMatrix_CopyVec3AndUpdate, "auto");
