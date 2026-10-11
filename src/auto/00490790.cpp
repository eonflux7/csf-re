#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixMultiply, 0x00624630, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));

static const auto g_MatrixInvertVtable = CSF_VTBL(0x006ADB40);

static char __fastcall C_008342e8_SetObjectTransform(
    C_008342e8* self, void* /*edx*/, std::uint32_t* transform)
{
    alignas(4) std::uint8_t matrixState[0x8] = {};
    alignas(4) std::uint8_t inverseState[0xC] = {};

    void* object = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(self->vGetField016c()));

    void* transformOwner =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

    CSF_AT(void*, inverseState, 0x8) = transformOwner;

    int storedTarget =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0x98 / 4])(object, nullptr, 2200);

    if (storedTarget == -1)
    {
        return reinterpret_cast<char (__fastcall*)(void*, void*, std::uint32_t*)>(
            CSF_AT(void**, transformOwner, 0)[0x94 / 4])(
                transformOwner, nullptr, transform);
    }

    void* parent =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x8C / 4])(object, nullptr);

    void* parentTransform =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, parent, 0)[0x18 / 4])(parent, nullptr);

    CSF_AT(std::uint32_t*, inverseState, 0) = RwMatrixCreate();
    CSF_AT(char, inverseState, 0x4) = 1;

    S_006adb40* inversion =
        reinterpret_cast<S_006adb40* (__fastcall*)(void*, void*, void*, void*)>(
            CSF_AT(void**, parentTransform, 0)[0x90 / 4])(
                parentTransform, nullptr, matrixState, inverseState);

    Matrix_InvertWithStoredTarget(
        inversion, nullptr,
        reinterpret_cast<float**>(static_cast<std::uintptr_t>(storedTarget)));

    if (CSF_AT(char, matrixState, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixState, 0));

    std::uint32_t* product = RwMatrixCreate();
    RwMatrixMultiply(
        product,
        CSF_AT(std::uint32_t*, inverseState, 0),
        reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(*transform)));

    CSF_AT(std::uint32_t*, matrixState, 0) = RwMatrixCreate();
    std::memcpy(CSF_AT(std::uint32_t*, matrixState, 0), product, 0x40u);
    CSF_AT(char, matrixState, 0x4) = 1;
    RwMatrixDestroy(product);

    void* destination = CSF_AT(void*, inverseState, 0x8);
    reinterpret_cast<char (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, destination, 0)[0x94 / 4])(
            destination, nullptr, matrixState);

    if (CSF_AT(char, matrixState, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixState, 0));

    char result = CSF_AT(char, inverseState, 0x4);
    if (result)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, inverseState, 0)));

    return result;
}

HOOK(0x00490790, C_008342e8_SetObjectTransform, "auto");

