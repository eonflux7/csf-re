#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, C_00861d48_ResolveOrCreateMatrix, 0x006E6D90,
         (void* self, void* edx, int a2, int a3));

static char __fastcall C_008632c0_UpdateChildMatrices(
    C_008632c0* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0x39);
    if (result)
        return result;

    alignas(8) std::uint8_t resolvedMatrixBytes[0x8];
    alignas(8) std::uint8_t matrixStateBytes[0x10];

    auto* createdMatrix = RwMatrixCreate();
    const std::uint32_t initialCount = CSF_AT(std::uint32_t, self, 0x30);

    auto*& matrix = CSF_AT(std::uint32_t*, matrixStateBytes, 0x0);
    auto& ownsMatrix = CSF_AT(char, matrixStateBytes, 0x4);
    auto& index = CSF_AT(std::uint32_t, matrixStateBytes, 0x8);
    auto& entryOffset = CSF_AT(std::uint32_t, matrixStateBytes, 0xC);

    matrix = createdMatrix;
    ownsMatrix = 1;
    index = 0;

    if (initialCount > 0)
    {
        entryOffset = 0;
        do
        {
            auto* entries = CSF_AT(std::uint8_t*, self, 0x28);
            const int matrixKey = CSF_AT(int, entries + entryOffset, 0x4);
            const int resolved = C_00861d48_ResolveOrCreateMatrix(
                self, nullptr, reinterpret_cast<int>(resolvedMatrixBytes), matrixKey);

            const void* source = *reinterpret_cast<const void* const*>(resolved);
            std::memcpy(matrix, source, 0x40u);

            if (CSF_AT(char, resolvedMatrixBytes, 0x4))
                RwMatrixDestroy(CSF_AT(std::uint32_t*, resolvedMatrixBytes, 0x0));

            entries = CSF_AT(std::uint8_t*, self, 0x28);
            const std::uint32_t savedOffset = entryOffset;
            void* child = CSF_AT(void*, entries + savedOffset, 0x0);
            reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, child, 0x0)[0xC / 4])(
                    child, nullptr, matrixStateBytes);

            std::uint32_t nextIndex = index;
            const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x30);
            ++nextIndex;
            index = nextIndex;
            entryOffset = savedOffset + 0x10;
            if (!(nextIndex < count))
                break;
        } while (true);
    }

    result = ownsMatrix;
    CSF_AT(char, self, 0x39) = 1;
    if (result)
        return static_cast<char>(RwMatrixDestroy(matrix));
    return result;
}

HOOK(0x00706910, C_008632c0_UpdateChildMatrices, "auto");
