#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));

static char __fastcall C_00860288_AttachObjectAtFrame(
    C_00860288* self, void* /*edx*/,
    C_00860288* object, int destinationFrameId, int sourceFrameId)
{
    // Retain the packet's explicit vtable binding for the matrix helper.
    (void)CSF_VTBL(0x006ADB40);

    alignas(8) std::uint8_t sourceMatrixBytes[0x8]{};
    alignas(8) std::uint8_t inverseMatrixBytes[0xC]{};

    CSF_AT(void*, inverseMatrixBytes, 0x8) = &self->field_0030;

    if (destinationFrameId != -1)
    {
        const int frameIndex = self->vSlot98(destinationFrameId);
        if (frameIndex != -1)
        {
            void* frameObject =
                reinterpret_cast<void*>(self->vSlot8C(frameIndex));
            CSF_AT(void*, inverseMatrixBytes, 0x8) =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, frameObject, 0)[0x18 / 4])(
                        frameObject, nullptr);
        }
    }

    const int sourceFrameIndex = object->vSlot98(sourceFrameId);
    if (sourceFrameIndex == -1)
        return static_cast<char>(sourceFrameIndex);

    void* objectFrame = object->vSlot18();
    void* sourceFrameObject =
        reinterpret_cast<void*>(object->vSlot8C(sourceFrameIndex));
    void* sourceFrame =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sourceFrameObject, 0)[0x18 / 4])(
                sourceFrameObject, nullptr);

    CSF_AT(std::uint32_t*, sourceMatrixBytes, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, sourceMatrixBytes, 0x4) = 1;

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, objectFrame, 0)[0x98 / 4])(
            objectFrame, nullptr, sourceMatrixBytes);

    if (CSF_AT(std::uint8_t, sourceMatrixBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, sourceMatrixBytes, 0));

    CSF_AT(std::uint32_t*, inverseMatrixBytes, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, inverseMatrixBytes, 0x4) = 1;

    S_006adb40* sourceMatrix =
        reinterpret_cast<S_006adb40* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, sourceFrame, 0)[0x8C / 4])(
                sourceFrame, nullptr, sourceMatrixBytes);

    Matrix_InvertWithStoredTarget(
        sourceMatrix, nullptr, reinterpret_cast<float**>(inverseMatrixBytes));

    if (CSF_AT(std::uint8_t, sourceMatrixBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, sourceMatrixBytes, 0));

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, objectFrame, 0)[0x98 / 4])(
            objectFrame, nullptr, inverseMatrixBytes);

    void* destinationFrame = CSF_AT(void*, inverseMatrixBytes, 0x8);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, destinationFrame, 0)[0x7C / 4])(
            destinationFrame, nullptr, objectFrame);

    const char ownsInverse =
        static_cast<char>(CSF_AT(std::uint8_t, inverseMatrixBytes, 0x4));
    if (ownsInverse)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, inverseMatrixBytes, 0)));

    return ownsInverse;
}

HOOK(0x006B2E20, C_00860288_AttachObjectAtFrame, "auto");
