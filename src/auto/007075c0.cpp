#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Releases the bone matrix at +0x1C (if any) and zeroes the matrix slot and the
// two cached values at +0x10 and +0x14. Returns the matrix pointer value that was
// read, or the RwMatrixDestroy result when a matrix was destroyed.
static int __fastcall GfxHAnim_ClearBoneMatrix(void* self, void* /*edx*/)
{
    std::uint32_t boneMatrixValue = CSF_AT(std::uint32_t, self, 0x1C);
    int result = static_cast<int>(boneMatrixValue);
    if (result)
    {
        result = RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(boneMatrixValue));
        CSF_AT(std::uint32_t, self, 0x1C) = 0;
    }
    CSF_AT(std::uint32_t, self, 0x10) = 0;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    return result;
}

HOOK(0x007075C0, GfxHAnim_ClearBoneMatrix, "auto");
