#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using std::uint32_t;

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_TranslateFrameByOffset, 0x006ADB90, (uint32_t* self, void* , int offset));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static char __fastcall C_00863260_UpdateChildMatrices(
    C_00863260* self, void* /*edx*/)
{
    const char updated = CSF_AT(char, self, 0x39);
    if (updated)
        return updated;
    if (!self->field_0008)
        return 0;

    alignas(8) std::uint8_t transformResultBytes[0xC] = {};
    alignas(8) std::uint8_t positionBytes[0xC] = {};
    // The temporary matrix wrapper shares storage with the input vector's Y/Z.
    alignas(8) std::uint8_t inputPositionBytes[0xC] = {};
    alignas(8) std::uint8_t matrixBytes[0xC] = {};

    CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
    CSF_AT(char, matrixBytes, 4) = 1;

    void* source = reinterpret_cast<void*>(self->field_0008);
    void* transform =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[0x18 / 4])(source, nullptr);
    CSF_AT(void*, matrixBytes, 8) = transform;

    void* temporaryMatrix =
        ((void* (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, transform, 0)[0x8C / 4])(
                transform, nullptr, inputPositionBytes + 4);

    std::memcpy(
        CSF_AT(std::uint32_t*, matrixBytes, 0),
        CSF_AT(const void*, temporaryMatrix, 0),
        0x40);

    if (CSF_AT(char, inputPositionBytes, 8))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, inputPositionBytes, 4));

    const float halfHeight =
        CSF_AT(float, self, 0x10) * g_HalfDurationScale;
    CSF_AT(std::uint32_t, inputPositionBytes, 0) =
        CSF_AT(std::uint32_t, self, 0x18);
    CSF_AT(float, positionBytes, 4) = halfHeight;
    CSF_AT(std::uint32_t, inputPositionBytes, 8) =
        CSF_AT(std::uint32_t, self, 0x20);
    CSF_AT(std::uint32_t, positionBytes, 0) = 0;
    CSF_AT(std::uint32_t, positionBytes, 8) = 0;
    CSF_AT(float, inputPositionBytes, 4) =
        CSF_AT(float, positionBytes, 4) + CSF_AT(float, self, 0x1C);

    transform = CSF_AT(void*, matrixBytes, 8);
    const void* transformedPosition =
        ((void* (__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, transform, 0)[0xA4 / 4])(
                transform, nullptr, transformResultBytes, inputPositionBytes);
    std::memcpy(positionBytes, transformedPosition, 0xC);

    GfxMatrix_TranslateFrameByOffset(
        reinterpret_cast<uint32_t*>(matrixBytes), nullptr,
        reinterpret_cast<int>(positionBytes));

    std::uint32_t childIndex = 0;
    std::uint32_t entryOffset = 0;
    if (CSF_AT(std::uint32_t, self, 0x30))
    {
        do
        {
            auto* entries = CSF_AT(std::uint8_t*, self, 0x28);
            void* child = CSF_AT(void*, entries + entryOffset, 0);
            ((void (__fastcall*)(void*, void*, void*))
                CSF_AT(void**, child, 0)[0xC / 4])(
                    child, nullptr, matrixBytes);
            ++childIndex;
            entryOffset += 0x10;
        }
        while (childIndex < CSF_AT(std::uint32_t, self, 0x30));
    }

    const char ownsMatrix = CSF_AT(char, matrixBytes, 4);
    CSF_AT(char, self, 0x39) = 1;
    if (ownsMatrix)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0)));
    return ownsMatrix;
}

HOOK(0x00706320, C_00863260_UpdateChildMatrices, "auto");
