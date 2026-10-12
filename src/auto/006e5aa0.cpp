#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameRotate, 0x00623890, (int a1, int a2, float a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const std::uint32_t* src));
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

// Rotates the RW frame of the alt-instance sub-object by (a2, a3) around axis 2,
// re-applying the saved translation before and after; destroys the matrix if the
// guard flag ends up set.
static char __fastcall C_00861ce0_RotateAltInstance(C_00861ce0* self, void* /*edx*/, int a2, float a3)
{
    auto* subObject = reinterpret_cast<std::uint8_t*>(self->field_000c);
    std::uint32_t* v3 = *reinterpret_cast<std::uint32_t**>(subObject + 4);
    std::uint32_t* v6 = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(v3) + 0x10);

    // Guard object on the stack: [0] = matrix pointer, [4] = destroy flag.
    alignas(8) std::uint8_t guardFrame[8];
    CSF_AT(std::uint32_t*, guardFrame, 0) = v6;
    CSF_AT(std::uint8_t, guardFrame, 4) = 0;

    // Saved translation taken from the frame (v3[16..18]).
    std::uint32_t savedTranslation[3];
    savedTranslation[0] = v3[16];
    savedTranslation[1] = v3[17];
    savedTranslation[2] = v3[18];

    GfxMatrix_CopyTranslationAndUpdate(guardFrame, nullptr, reinterpret_cast<const std::uint32_t*>(&g_ZeroVectorX));
    RwFrameRotate(reinterpret_cast<int>(v3), a2, a3, 2);
    GfxMatrix_CopyTranslationAndUpdate(guardFrame, nullptr, savedTranslation);

    char result = static_cast<char>(CSF_AT(std::uint8_t, guardFrame, 4));
    if (result != 0)
        return static_cast<char>(RwMatrixDestroy(CSF_AT(std::uint32_t*, guardFrame, 0)));
    return result;
}

HOOK(0x006E5AA0, C_00861ce0_RotateAltInstance, "auto");
