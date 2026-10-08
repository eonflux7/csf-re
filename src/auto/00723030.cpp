#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CGuiTextureListBox_GetColumnCount, 0x00714830,
         (void* self, void* edx));

static int __fastcall GetCellStringAddress(
    void* self, void* /*edx*/, int firstIndex, int secondIndex)
{
    if (firstIndex >=
        ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr))
    {
        return 0;
    }

    if (secondIndex >= CGuiTextureListBox_GetColumnCount(self, nullptr))
        return 0;

    const uint32_t entriesAddress = CSF_AT(uint32_t, self, 0x220);
    const uint32_t entryAddress =
        entriesAddress + static_cast<uint32_t>(firstIndex) * 16u;
    const uint32_t cellsAddress =
        CSF_AT(uint32_t, reinterpret_cast<void*>(entryAddress), 0x4);
    const uint32_t cellAddress =
        cellsAddress + static_cast<uint32_t>(secondIndex) * 60u;

    if (CSF_AT(uint32_t, reinterpret_cast<void*>(cellAddress), 0x30) < 8u)
        return static_cast<int>(cellAddress + 0x1Cu);

    return CSF_AT(int, reinterpret_cast<void*>(cellAddress), 0x1C);
}

HOOK(0x00723030, GetCellStringAddress, "auto");
