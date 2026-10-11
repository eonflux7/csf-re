#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

static char __fastcall C_00862490_GetDirection(
    C_00862490* self, void* /*edx*/, std::uint32_t* direction)
{
    auto* handle =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);
    if (!handle)
        return 0;

    handle = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);

    auto* sceneObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, handle, 0)[0x18 / 4])(handle, nullptr);

    struct MatrixResult
    {
        std::uint32_t* matrix;
        char destroyMatrix;
        char padding[3];
    } result{};

    auto* matrixReference =
        reinterpret_cast<void* (__fastcall*)(void*, void*, MatrixResult*)>(
            CSF_AT(void**, sceneObject, 0)[0x8C / 4])(
                sceneObject, nullptr, &result);

    auto* matrix = reinterpret_cast<void*>(
        CSF_AT(std::uint32_t, matrixReference, 0));

    direction[0] = CSF_AT(std::uint32_t, matrix, 0x20);
    direction[1] = CSF_AT(std::uint32_t, matrix, 0x24);
    direction[2] = CSF_AT(std::uint32_t, matrix, 0x28);

    if (result.destroyMatrix)
        return static_cast<char>(RwMatrixDestroy(result.matrix));
    return result.destroyMatrix;
}

HOOK(0x00476100, C_00862490_GetDirection, "auto");
