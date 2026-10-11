#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixUpdate, 0x00624620, (int a1));

// Copies three consecutive dwords (one 3-component row) from src into dst.
static void CopyThreeDwords(uint32_t* dst, const uint32_t* src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

// Writes four 3-dword rows into the matrix buffer at field_0000 (offsets 0, 16, 32, 48),
// then asks RwMatrixUpdate to refresh it. Returns RwMatrixUpdate's result.
static int __fastcall GfxMatrix_SetFourRowsAndUpdate(S_006ada10* self, void* /*edx*/,
                                                    uint32_t* a2, uint32_t* a3,
                                                    uint32_t* a4, uint32_t* a5) {
    uint32_t* dst = reinterpret_cast<uint32_t*>(self->field_0000);
    CopyThreeDwords(dst, a2);

    dst = reinterpret_cast<uint32_t*>(self->field_0000 + 16);
    CopyThreeDwords(dst, a3);

    dst = reinterpret_cast<uint32_t*>(self->field_0000 + 32);
    CopyThreeDwords(dst, a4);

    dst = reinterpret_cast<uint32_t*>(self->field_0000 + 48);
    CopyThreeDwords(dst, a5);

    return RwMatrixUpdate(static_cast<int>(self->field_0000));
}

HOOK(0x006ADA10, GfxMatrix_SetFourRowsAndUpdate, "auto");
