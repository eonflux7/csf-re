#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));

[[maybe_unused]] static const auto kMatrixInversionVtable = CSF_VTBL(0x006ADB40);

static char __fastcall C_00860288_AttachObjectAtFrame(
    C_00860288* self, void* /*edx*/, void* object,
    int destinationFrame, int sourceFrame)
{
    alignas(8) std::uint8_t sourceMatrixBytes[0x8];
    alignas(8) std::uint8_t targetMatrixBytes[0xC];

    CSF_AT(void*, targetMatrixBytes, 0x8) =
        reinterpret_cast<std::uint8_t*>(self) + 0x30;

    if (destinationFrame != -1)
    {
        const int destinationIndex = self->vSlot98(destinationFrame);
        if (destinationIndex != -1)
        {
            void* frameObject =
                reinterpret_cast<void*>(self->vSlot8C(destinationIndex));
            CSF_AT(void*, targetMatrixBytes, 0x8) =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, frameObject, 0)[0x18 / 4])(
                        frameObject, nullptr);
        }
    }

    const int sourceIndex =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0x98 / 4])(
                object, nullptr, sourceFrame);

    if (sourceIndex == -1)
        return static_cast<char>(sourceIndex);

    void* objectFrame =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x18 / 4])(
                object, nullptr);

    void* sourceFrameObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*, unsigned int)>(
            CSF_AT(void**, object, 0)[0x8C / 4])(
                object, nullptr, sourceIndex);

    void* selectedSourceFrame =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sourceFrameObject, 0)[0x18 / 4])(
                sourceFrameObject, nullptr);

    CSF_AT(std::uint32_t*, sourceMatrixBytes, 0) = RwMatrixCreate();
    CSF_AT(char, sourceMatrixBytes, 0x4) = 1;

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, objectFrame, 0)[0x98 / 4])(
            objectFrame, nullptr, sourceMatrixBytes);

    if (CSF_AT(char, sourceMatrixBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, sourceMatrixBytes, 0));

    CSF_AT(std::uint32_t*, targetMatrixBytes, 0) = RwMatrixCreate();
    CSF_AT(char, targetMatrixBytes, 0x4) = 1;

    S_006adb40* sourceMatrix =
        reinterpret_cast<S_006adb40* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, selectedSourceFrame, 0)[0x8C / 4])(
                selectedSourceFrame, nullptr, sourceMatrixBytes);

    // 0x6B2ECB pushes targetMatrixBytes before the +0x8C call. That call
    // consumes only sourceMatrixBytes (ret 4); the earlier push is the
    // argument to Matrix_InvertWithStoredTarget, not a second vcall argument.
    Matrix_InvertWithStoredTarget(
        sourceMatrix, nullptr, reinterpret_cast<float**>(targetMatrixBytes));

    if (CSF_AT(char, sourceMatrixBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, sourceMatrixBytes, 0));

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, objectFrame, 0)[0x98 / 4])(
            objectFrame, nullptr, targetMatrixBytes);

    void* destination = CSF_AT(void*, targetMatrixBytes, 0x8);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, destination, 0)[0x7C / 4])(
            destination, nullptr, objectFrame);

    const char ownsTargetMatrix = CSF_AT(char, targetMatrixBytes, 0x4);
    if (ownsTargetMatrix)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, targetMatrixBytes, 0)));

    return ownsTargetMatrix;
}

HOOK(0x006B2E20, C_00860288_AttachObjectAtFrame, "auto");
