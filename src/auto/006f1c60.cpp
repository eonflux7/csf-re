#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

static char __fastcall C_00862490_CopyChildMatrixVectors(
    C_00862490* self, void* /*edx*/)
{
    alignas(8) std::uint8_t matrixResult[0x8];

    void* child = reinterpret_cast<void*>(self->field_0024);
    std::uint32_t returnedAddress =
        ((std::uint32_t(__fastcall*)(void*, void*, std::uint32_t*))
             CSF_AT(void**, child, 0)[0x8C / 4])(
            child, nullptr, reinterpret_cast<std::uint32_t*>(matrixResult));

    auto* matrix = reinterpret_cast<std::uint8_t*>(
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(returnedAddress), 0));
    auto* vectors = matrix + 0x20;

    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, vectors, 0);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, vectors, 4);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, vectors, 8);

    char ownsMatrix = CSF_AT(char, matrixResult, 4);
    if (ownsMatrix) {
        return static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(
                CSF_AT(std::uint32_t, matrixResult, 0))));
    }
    return ownsMatrix;
}

HOOK(0x6F1C60, C_00862490_CopyChildMatrixVectors, "auto");
