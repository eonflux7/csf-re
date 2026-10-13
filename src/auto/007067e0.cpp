#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_TranslateFrameByOffset, 0x006ADB90, (uint32_t* self, void* , int offset));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static char __fastcall C_008632c0_InitializeAttachedTransform(
    C_008632c0* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0x38);
    if (result)
        return result;

    alignas(8) std::uint8_t finalOffsetBytes[0xC];
    alignas(8) std::uint8_t offsetBytes[0xC];
    alignas(8) std::uint8_t transformedBytes[0xC];
    alignas(8) std::uint8_t sourceMatrixBytes[0x8];
    alignas(8) std::uint8_t destinationMatrixBytes[0x8];

    void* owner = reinterpret_cast<void*>(self->field_0008);
    void* frame =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, owner, 0)[0x18 / 4])(owner, nullptr);

    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, frame, 0)[0x8C / 4])(
            frame, nullptr, sourceMatrixBytes);

    std::uint32_t* destinationMatrix = RwMatrixCreate();
    CSF_AT(std::uint32_t*, destinationMatrixBytes, 0) = destinationMatrix;
    std::uint32_t* sourceMatrix =
        CSF_AT(std::uint32_t*, sourceMatrixBytes, 0);

    // The original copies forward one dword at a time, including on overlap.
    for (unsigned byteOffset = 0; byteOffset < 0x40; byteOffset += 4)
        CSF_AT(std::uint32_t, destinationMatrix, byteOffset) =
            CSF_AT(std::uint32_t, sourceMatrix, byteOffset);

    CSF_AT(char, destinationMatrixBytes, 0x4) = 1;
    CSF_AT(int, transformedBytes, 0) = 0;
    CSF_AT(float, transformedBytes, 0x4) = 0.0f;
    CSF_AT(int, transformedBytes, 0x8) = 0;

    RwV3dTransformVectors(
        reinterpret_cast<int>(transformedBytes),
        reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x18),
        1,
        reinterpret_cast<int>(CSF_AT(std::uint32_t*, sourceMatrixBytes, 0)));

    CSF_AT(int, offsetBytes, 0) = CSF_AT(int, transformedBytes, 0);
    CSF_AT(int, offsetBytes, 0x8) = CSF_AT(int, transformedBytes, 0x8);
    CSF_AT(float, offsetBytes, 0x4) = CSF_AT(float, transformedBytes, 0x4);

    GfxMatrix_TranslateFrameByOffset(
        reinterpret_cast<std::uint32_t*>(destinationMatrixBytes),
        nullptr,
        reinterpret_cast<int>(offsetBytes));

    CSF_AT(float, offsetBytes, 0x4) =
        CSF_AT(float, self, 0x10) * g_HalfDurationScale;
    CSF_AT(int, offsetBytes, 0) = 0;
    CSF_AT(int, offsetBytes, 0x8) = 0;
    CSF_AT(int, transformedBytes, 0) = 0;
    CSF_AT(float, transformedBytes, 0x4) = 0.0f;
    CSF_AT(int, transformedBytes, 0x8) = 0;

    RwV3dTransformVectors(
        reinterpret_cast<int>(transformedBytes),
        reinterpret_cast<int>(offsetBytes),
        1,
        reinterpret_cast<int>(CSF_AT(std::uint32_t*, sourceMatrixBytes, 0)));

    CSF_AT(int, finalOffsetBytes, 0) = CSF_AT(int, transformedBytes, 0);
    CSF_AT(float, finalOffsetBytes, 0x4) = CSF_AT(float, transformedBytes, 0x4);
    CSF_AT(int, finalOffsetBytes, 0x8) = CSF_AT(int, transformedBytes, 0x8);

    GfxMatrix_TranslateFrameByOffset(
        reinterpret_cast<std::uint32_t*>(destinationMatrixBytes),
        nullptr,
        reinterpret_cast<int>(finalOffsetBytes));

    void* attachedObject = CSF_AT(void*, self, 0x24);
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, attachedObject, 0)[0xC / 4])(
            attachedObject, nullptr, destinationMatrixBytes);

    const char ownsDestination = CSF_AT(char, destinationMatrixBytes, 0x4);
    CSF_AT(char, self, 0x38) = 1;
    if (ownsDestination)
        RwMatrixDestroy(CSF_AT(std::uint32_t*, destinationMatrixBytes, 0));

    result = CSF_AT(char, sourceMatrixBytes, 0x4);
    if (result)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, sourceMatrixBytes, 0)));

    return result;
}

HOOK(0x007067E0, C_008632c0_InitializeAttachedTransform, "auto");
