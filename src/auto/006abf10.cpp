#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));

static std::uint32_t* __fastcall GfxMatrix_TransformBasisAndPosition(
    std::uint32_t* self, void* /*edx*/, std::uint32_t* matrix)
{
    auto* bytes = reinterpret_cast<std::uint8_t*>(self);
    std::uint32_t transformed[3] = {0, 0, 0};

    RwV3dTransformVectors(
        reinterpret_cast<int>(transformed),
        reinterpret_cast<int>(bytes + 0x18),
        1, CSF_AT(int, matrix, 0x00));
    CSF_AT(std::uint32_t, self, 0x18) = transformed[0];
    CSF_AT(std::uint32_t, self, 0x1C) = transformed[1];
    CSF_AT(std::uint32_t, self, 0x20) = transformed[2];

    transformed[0] = 0;
    transformed[1] = 0;
    transformed[2] = 0;
    RwV3dTransformVectors(
        reinterpret_cast<int>(transformed),
        reinterpret_cast<int>(bytes + 0x24),
        1, CSF_AT(int, matrix, 0x00));
    CSF_AT(std::uint32_t, self, 0x24) = transformed[0];
    CSF_AT(std::uint32_t, self, 0x28) = transformed[1];
    CSF_AT(std::uint32_t, self, 0x2C) = transformed[2];

    transformed[0] = 0;
    transformed[1] = 0;
    transformed[2] = 0;
    RwV3dTransformVectors(
        reinterpret_cast<int>(transformed),
        reinterpret_cast<int>(bytes + 0x30),
        1, CSF_AT(int, matrix, 0x00));
    CSF_AT(std::uint32_t, self, 0x30) = transformed[0];
    CSF_AT(std::uint32_t, self, 0x34) = transformed[1];
    CSF_AT(std::uint32_t, self, 0x38) = transformed[2];

    RwV3dTransformPoints(
        reinterpret_cast<int>(transformed),
        reinterpret_cast<int>(self),
        1, CSF_AT(int, matrix, 0x00));
    CSF_AT(std::uint32_t, self, 0x00) = transformed[0];
    CSF_AT(std::uint32_t, self, 0x04) = transformed[1];
    CSF_AT(std::uint32_t, self, 0x08) = transformed[2];

    return self;
}

HOOK(0x006ABF10, GfxMatrix_TransformBasisAndPosition, "auto");
