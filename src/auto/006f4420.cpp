#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(int, __cdecl, RwFrameTranslate, 0x00623830, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwFrameTransform, 0x00623860, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __fastcall, C_00862748_InitBoundsAndTransform, 0x006F3A30, (C_00862748* self, void* , int* a2));

static char __fastcall C_00862748_UpdateParticleFrame(
    C_00862748* self, void* /*edx*/)
{
    alignas(8) std::uint8_t matrixResultBytes[0x8];
    int* matrixResult = reinterpret_cast<int*>(matrixResultBytes);

    const int frame = CSF_AT(
        int, reinterpret_cast<void*>(self->field_0024), 0x4);
    const std::uint8_t useFullTransform = CSF_AT(
        std::uint8_t, reinterpret_cast<void*>(self->field_0018), 0x4D);
    void* transformSource = reinterpret_cast<void*>(self->field_0028);

    if (useFullTransform)
    {
        int* transform = reinterpret_cast<int* (__fastcall*)(void*, void*, int*)>(
            CSF_AT(void**, transformSource, 0)[0x8C / 4])(
                transformSource, nullptr, matrixResult);

        RwFrameTransform(frame, *transform, 0);
        if (CSF_AT(std::uint8_t, matrixResultBytes, 0x4))
        {
            RwMatrixDestroy(
                CSF_AT(std::uint32_t*, matrixResultBytes, 0));
        }
    }
    else
    {
        const int translation =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, transformSource, 0)[0x10 / 4])(
                    transformSource, nullptr);
        RwFrameTranslate(frame, translation, 0);
    }

    RwFrameUpdateObjects(frame);

    transformSource = reinterpret_cast<void*>(self->field_0028);
    int* transform = reinterpret_cast<int* (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, transformSource, 0)[0x8C / 4])(
            transformSource, nullptr, matrixResult);
    C_00862748_InitBoundsAndTransform(self, nullptr, transform);

    const char ownsMatrix = CSF_AT(char, matrixResultBytes, 0x4);
    if (ownsMatrix)
    {
        return static_cast<char>(RwMatrixDestroy(
            CSF_AT(std::uint32_t*, matrixResultBytes, 0)));
    }
    return ownsMatrix;
}

HOOK(0x006F4420, C_00862748_UpdateParticleFrame, "auto");
