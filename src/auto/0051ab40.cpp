#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(char, __fastcall, C_00871a18_LoadAnimationTable, 0x00774B40, (void* self, void* edx, int a2));
CSF_FUNC(char, __cdecl, GfxTextureLoader_ResetRecordRange, 0x006DEC40, (std::uint32_t first, std::uint32_t last, void* records, int a2));
CSF_FUNC(char, __cdecl, MemDebug_FreeDebugBlock, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, BddTables_ResizeRecordVectorAndFreeBlock, 0x0051AAC0,
         (void* self, void* edx, std::uint32_t count, char fillFlag, void* fillBlock, int fillA, int fillB));

// Reloads the animation table, frees the record vector at +8 (begin +0xC, end +0x10, capacity +0x14) and resizes
// it to one record per 96-byte entry of the list at +4. The original leaves the fill record's first byte
// uninitialised (only +4..+0xC are stored); it is zero here.
static char __fastcall C_00840724_ResetAndRebuild(void* self, void* /*edx*/, int a2)
{
    std::uint8_t* records = static_cast<std::uint8_t*>(self) + 8;
    const char result = C_00871a18_LoadAnimationTable(self, nullptr, a2);
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x0C);
    if (begin)
    {
        GfxTextureLoader_ResetRecordRange(begin, CSF_AT(std::uint32_t, self, 0x10), records, a2);
        if (CSF_AT(std::uint32_t, self, 0x0C))
            MemDebug_FreeDebugBlock(CSF_AT(void*, records, 4));
    }
    CSF_AT(std::uint32_t, self, 0x0C) = 0;
    CSF_AT(std::uint32_t, self, 0x10) = 0;
    CSF_AT(std::uint32_t, self, 0x14) = 0;

    const std::uint8_t* list = CSF_AT(std::uint8_t*, self, 0x04);
    const std::int32_t first = CSF_AT(std::int32_t, list, 4);
    const std::int32_t count = first ? (CSF_AT(std::int32_t, list, 8) - first) / 96 : 0;
    BddTables_ResizeRecordVectorAndFreeBlock(records, nullptr, static_cast<std::uint32_t>(count), 0, nullptr, 0, 0);
    return result;
}

HOOK(0x0051AB40, C_00840724_ResetAndRebuild, "auto");
