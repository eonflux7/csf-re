#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int*, __fastcall, EraseWideStringRange, 0x004F21D0, (legacy::wstring* text, void* edx, int* result, std::uint32_t first, std::uint32_t last));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall C_008670f0_ClearOwnedData(
    C_008670f0* self, void* /*edx*/)
{
    // The packet also classifies the erase routine's address as a vtable.
    (void)CSF_VTBL(0x004F21D0);

    alignas(8) std::uint8_t resultBytes[0x4];
    int& eraseResult = *reinterpret_cast<int*>(resultBytes);

    auto* firstTextBytes = reinterpret_cast<std::uint8_t*>(self) + 0x20;
    auto* firstText = reinterpret_cast<legacy::wstring*>(firstTextBytes);
    const std::uint32_t firstCapacity =
        CSF_AT(std::uint32_t, firstTextBytes, 0x18);

    std::uint32_t first = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(firstTextBytes + 4));
    std::uint32_t last = firstCapacity < 8
        ? first
        : CSF_AT(std::uint32_t, firstTextBytes, 0x4);
    last += 2 * CSF_AT(std::uint32_t, firstTextBytes, 0x14);
    if (firstCapacity >= 8)
        first = CSF_AT(std::uint32_t, firstTextBytes, 0x4);

    EraseWideStringRange(firstText, nullptr, &eraseResult, first, last);

    const std::uint32_t secondCapacity = self->field_0054;
    auto* secondTextBytes = reinterpret_cast<std::uint8_t*>(self) + 0x3C;
    auto* secondText = reinterpret_cast<legacy::wstring*>(secondTextBytes);

    first = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(secondTextBytes + 4));
    last = secondCapacity < 8
        ? first
        : CSF_AT(std::uint32_t, self, 0x40);
    last += 2 * CSF_AT(std::uint32_t, self, 0x50);
    if (secondCapacity >= 8)
        first = CSF_AT(std::uint32_t, self, 0x40);

    EraseWideStringRange(secondText, nullptr, &eraseResult, first, last);

    std::uint32_t block = self->field_0058;
    self->field_005c = 0;
    self->field_0060 = 0;
    if (block)
    {
        Mem_Free(reinterpret_cast<void*>(block));
        self->field_0058 = 0;
    }

    block = self->field_0068;
    self->field_006c = 0;
    self->field_0070 = 0;
    if (block)
    {
        Mem_Free(reinterpret_cast<void*>(block));
        self->field_0068 = 0;
    }

    block = self->field_0078;
    self->field_007c = 0;
    self->field_0080 = 0;
    if (block)
    {
        Mem_Free(reinterpret_cast<void*>(block));
        self->field_0078 = 0;
    }

    void* result = reinterpret_cast<void*>(self->field_0088);
    self->field_008c = 0;
    self->field_0090 = 0;
    if (result)
    {
        result = reinterpret_cast<void*>(
            static_cast<std::uintptr_t>(Mem_Free(result)));
        self->field_0088 = 0;
    }
    return result;
}

HOOK(0x00734DF0, C_008670f0_ClearOwnedData, "auto");
