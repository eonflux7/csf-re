#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (uint32_t* matrix));
CSF_FUNC(int, __fastcall, ScaleMatrix, 0x006ADB60,
         (void* self, void* edx, uint32_t x, uint32_t y, uint32_t z));
CSF_FUNC(int, __fastcall, TranslateMatrixHandle, 0x006ADBB0,
         (void* self, void* edx, uint32_t x, uint32_t y, uint32_t z));
CSF_FUNC(int, __fastcall, RotateMatrixHandle, 0x006AE210,
         (void* self, void* edx, float radians));
CSF_FUNC(int, __fastcall, C_00865a10_InvokeWithScaledFourthComponent, 0x00728AF0,
         (C_00865a10* self, void* edx, void* texture));

CSF_GLOBAL(float, g_DegreesToRadians, 0x008656B0);

static uint32_t NegateFloatBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, 4);
    value = -value;
    std::memcpy(&bits, &value, 4);
    return bits;
}

static char __fastcall C_00865a10_ApplyTextureTransform(
    C_00865a10* self, void* /*edx*/, float angleDegrees,
    const uint32_t* translation, uint32_t scaleX, uint32_t scaleY)
{
    void* texture = reinterpret_cast<void*>(self->field_0050);
    const void* pivot = ((const void* (__fastcall*)(void*, void*))
        CSF_AT(void**, texture, 0)[0x30 / 4])(texture, nullptr);

    const uint32_t pivotX = CSF_AT(uint32_t, pivot, 0);
    const uint32_t pivotY = CSF_AT(uint32_t, pivot, 4);

    struct MatrixHandle
    {
        uint32_t* matrix;
        char owned;
    };
    MatrixHandle handle;
    handle.matrix = RwMatrixCreate();
    handle.owned = 1;

    CSF_AT(uint32_t, handle.matrix, 0x28) = 0x3F800000;
    CSF_AT(uint32_t, handle.matrix, 0x14) = 0x3F800000;
    CSF_AT(uint32_t, handle.matrix, 0x00) = 0x3F800000;
    CSF_AT(uint32_t, handle.matrix, 0x10) = 0;
    CSF_AT(uint32_t, handle.matrix, 0x08) = 0;
    CSF_AT(uint32_t, handle.matrix, 0x04) = 0;
    CSF_AT(uint32_t, handle.matrix, 0x24) = 0;
    CSF_AT(uint32_t, handle.matrix, 0x20) = 0;
    CSF_AT(uint32_t, handle.matrix, 0x18) = 0;
    CSF_AT(uint32_t, handle.matrix, 0x38) = 0;
    CSF_AT(uint32_t, handle.matrix, 0x34) = 0;
    CSF_AT(uint32_t, handle.matrix, 0x30) = 0;
    CSF_AT(uint32_t, handle.matrix, 0x0C) |= 0x20003;

    const uint32_t negativeY = NegateFloatBits(pivotY);
    const uint32_t negativeX = NegateFloatBits(pivotX);
    TranslateMatrixHandle(&handle, nullptr, negativeX, negativeY, 0);

    const float radians = static_cast<float>(
        static_cast<double>(angleDegrees) *
        static_cast<double>(g_DegreesToRadians));
    RotateMatrixHandle(&handle, nullptr, radians);

    ScaleMatrix(&handle, nullptr, scaleX, scaleY, 0x3F800000);
    TranslateMatrixHandle(&handle, nullptr, pivotX, pivotY, 0);

    const uint32_t translationY = translation[1];
    const uint32_t translationX = translation[0];
    TranslateMatrixHandle(&handle, nullptr, translationX, translationY, 0);

    texture = reinterpret_cast<void*>(self->field_0050);
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, texture, 0)[0x60 / 4])(texture, nullptr, &handle);

    C_00865a10_InvokeWithScaledFourthComponent(
        self, nullptr, reinterpret_cast<void*>(self->field_0050));

    const char owned = handle.owned;
    if (owned)
        return static_cast<char>(RwMatrixDestroy(handle.matrix));
    return owned;
}

HOOK(0x007295F0, C_00865a10_ApplyTextureTransform, "auto");
