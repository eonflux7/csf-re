#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __cdecl, RwMatrixInvert, 0x00624950, (float* a1, float* a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Inverts the 4x4 matrix pointed to by *self into a temporary matrix,
// copies the 64-byte result back over *self, and frees the temporary.
static int __fastcall GfxMatrix_InvertInPlace(void** self, void* /*edx*/) {
    std::uint32_t* scratch = RwMatrixCreate();
    RwMatrixInvert(reinterpret_cast<float*>(scratch), static_cast<float*>(*self));
    std::memcpy(*self, scratch, 0x40);
    return RwMatrixDestroy(scratch);
}

HOOK(0x6ADB10, GfxMatrix_InvertInPlace, "auto");
