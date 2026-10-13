#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxMatrix_SetFourRowsAndUpdate, 0x006ADA10, (S_006ada10* self, void* , uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));

static auto const g_MatrixUpdateVtableReference = CSF_VTBL(0x006ADA10);

static int __stdcall TypeRegistry_SerializeMatrixRows(S_006ada10* matrix, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const bool writing = CSF_AT(uint8_t, context, 0x14) != 0;

    alignas(8) uint8_t firstRowBytes[0xC] = {};
    alignas(8) uint8_t secondRowBytes[0xC] = {};
    alignas(8) uint8_t thirdRowBytes[0xC] = {};
    alignas(8) uint8_t fourthRowBytes[0xC] = {};

    auto* firstRow = reinterpret_cast<uint32_t*>(firstRowBytes);
    auto* secondRow = reinterpret_cast<uint32_t*>(secondRowBytes);
    auto* thirdRow = reinterpret_cast<uint32_t*>(thirdRowBytes);
    auto* fourthRow = reinterpret_cast<uint32_t*>(fourthRowBytes);

    if (writing)
    {
        const auto* source = reinterpret_cast<const uint8_t*>(matrix->field_0000);
        std::memcpy(secondRowBytes, source + 0x10, 0xC);
        std::memcpy(thirdRowBytes, source + 0x20, 0xC);
        std::memcpy(firstRowBytes, source, 0xC);
        std::memcpy(fourthRowBytes, source + 0x30, 0xC);
    }

    uint32_t* rows[] = {secondRow, thirdRow, firstRow, fourthRow};
    for (int rowIndex = 0; rowIndex < 4; ++rowIndex)
    {
        const bool alreadyFailed = CSF_AT(uint8_t, context, 0x00) != 0;
        CSF_AT(uint32_t, context, 0x1C) = 0;

        if (!alreadyFailed)
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            CSF_AT(uint8_t, context, 0x15) = 0;
            CSF_AT(uint8_t, context, 0x16) = 0;

            const bool failed =
                ((int(__fastcall*)(void*, void*, uint32_t*, int))
                    CSF_AT(void**, serializer, 0)[56 / 4])(
                        serializer, nullptr, rows[rowIndex], contextAddress) == -1;

            CSF_AT(uint8_t, context, 0x00) = failed;
            if (writing && rowIndex == 3)
                return -static_cast<int>(failed);
        }
    }

    if (!writing)
    {
        GfxMatrix_SetFourRowsAndUpdate(
            matrix, nullptr, firstRow, secondRow, thirdRow, fourthRow);
    }

    return -static_cast<int>(CSF_AT(uint8_t, context, 0x00) != 0);
}

HOOK(0x0054C7D0, TypeRegistry_SerializeMatrixRows, "auto");
