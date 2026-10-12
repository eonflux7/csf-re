#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, S_006abb80_InitializeBounds, 0x006ABB80,
         (S_006abb80* self, void* /*edx*/, float xFirst, float xSecond, float yFirst, float ySecond, float zFirst, float zSecond));
CSF_FUNC(std::uint32_t*, __fastcall, GfxMatrix_TransformBasisAndPosition, 0x006ABF10,
         (std::uint32_t* self, void* /*edx*/, std::uint32_t* matrix));

// Referenced by the bounds helper call site (its vtable address, not stored here).
[[maybe_unused]] static const auto* kBoundsHelperVtbl = CSF_VTBL(0x006ABB80);

static std::uint32_t* __fastcall C_00862748_InitBoundsAndTransform(C_00862748* self, void* /*edx*/, int* a2)
{
    if (self->field_002c != 1)
    {
        // Source data block referenced by field_0030; bounds are floats at +0x13C..+0x150.
        std::uint8_t* sourceBase = reinterpret_cast<std::uint8_t*>(self->field_0030);

        float xFirst  = CSF_AT(float, sourceBase, 0x148);
        float xSecond = CSF_AT(float, sourceBase, 0x13C);
        float yFirst  = CSF_AT(float, sourceBase, 0x14C);
        float ySecond = CSF_AT(float, sourceBase, 0x140);
        float zFirst  = CSF_AT(float, sourceBase, 0x150);
        float zSecond = CSF_AT(float, sourceBase, 0x144);

        S_006abb80* boundsObj = reinterpret_cast<S_006abb80*>(reinterpret_cast<std::uint8_t*>(self) + 0x44);
        S_006abb80_InitializeBounds(boundsObj, nullptr, xFirst, xSecond, yFirst, ySecond, zFirst, zSecond);
    }

    std::uint8_t* matrixObj = reinterpret_cast<std::uint8_t*>(self) + 0x44;
    return GfxMatrix_TransformBasisAndPosition(reinterpret_cast<std::uint32_t*>(matrixObj), nullptr,
                                               reinterpret_cast<std::uint32_t*>(a2));
}

HOOK(0x6F3A30, C_00862748_InitBoundsAndTransform, "auto");
