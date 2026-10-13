#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_TranslateFrameByOffset, 0x006ADB90, (uint32_t* self, void* , int offset));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static char __fastcall C_00863260_InitializeAttachedMatrix(
    C_00863260* self, void* /*edx*/)
{
    const char initialized = CSF_AT(char, self, 0x38);
    if (initialized)
        return initialized;
    if (!self->field_0008)
        return 0;

    // Preserve the original frame's contiguous, overlapping temporary objects.
    alignas(8) std::uint8_t frame[0x30];
    void* transformedPoint = frame;
    void* translation = frame + 0x0C;
    void* localPoint = frame + 0x18;
    void* sourceMatrix = frame + 0x1C;
    void* matrixHolder = frame + 0x24;

    CSF_AT(std::uint32_t*, matrixHolder, 0) = RwMatrixCreate();
    CSF_AT(char, matrixHolder, 4) = 1;

    void* attachment = reinterpret_cast<void*>(self->field_0008);
    void* transform =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, attachment, 0)[0x18 / 4])(
                attachment, nullptr);
    CSF_AT(void*, matrixHolder, 8) = transform;

    void* matrixResult =
        reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, transform, 0)[0x8C / 4])(
                transform, nullptr, sourceMatrix);
    std::memcpy(
        CSF_AT(std::uint32_t*, matrixHolder, 0),
        CSF_AT(const void*, matrixResult, 0), 0x40);

    if (CSF_AT(char, sourceMatrix, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, sourceMatrix, 0));

    const float halfHeight =
        CSF_AT(float, self, 0x10) * g_HalfDurationScale;
    CSF_AT(std::uint32_t, localPoint, 0) =
        CSF_AT(std::uint32_t, self, 0x18);
    CSF_AT(std::uint32_t, localPoint, 8) =
        CSF_AT(std::uint32_t, self, 0x20);
    CSF_AT(float, translation, 4) = halfHeight;
    CSF_AT(std::uint32_t, translation, 0) = 0;
    CSF_AT(std::uint32_t, translation, 8) = 0;
    CSF_AT(float, localPoint, 4) =
        CSF_AT(float, translation, 4) + CSF_AT(float, self, 0x1C);

    transform = CSF_AT(void*, matrixHolder, 8);
    void* pointResult =
        reinterpret_cast<void* (__fastcall*)(void*, void*, void*, void*)>(
            CSF_AT(void**, transform, 0)[0xA4 / 4])(
                transform, nullptr, transformedPoint, localPoint);

    CSF_AT(std::uint32_t, translation, 0) =
        CSF_AT(std::uint32_t, pointResult, 0);
    CSF_AT(std::uint32_t, translation, 4) =
        CSF_AT(std::uint32_t, pointResult, 4);
    CSF_AT(std::uint32_t, translation, 8) =
        CSF_AT(std::uint32_t, pointResult, 8);

    GfxMatrix_TranslateFrameByOffset(
        reinterpret_cast<std::uint32_t*>(matrixHolder), nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(translation)));

    void* destination = CSF_AT(void*, self, 0x24);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, destination, 0)[0x0C / 4])(
            destination, nullptr, matrixHolder);

    const char ownsMatrix = CSF_AT(char, matrixHolder, 4);
    CSF_AT(char, self, 0x38) = 1;
    if (ownsMatrix)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixHolder, 0)));
    return ownsMatrix;
}

HOOK(0x00706230, C_00863260_InitializeAttachedMatrix, "auto");
