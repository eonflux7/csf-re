#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_GridCoordinateScale, 0x0083E110);

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(unsigned int, __fastcall, S_004ddf20_InsertEntry, 0x004DE430,
         (void* self, void* edx, void* a2));

static bool __fastcall S_004ddf20_InitializeGrid(S_004ddf20* self, void* /*edx*/)
{
    const uint32_t columns = 1u - static_cast<uint32_t>(
        static_cast<int64_t>((self->field_0010 - self->field_000c) *
                             g_GridCoordinateScale));
    self->field_0004 = columns;

    const uint32_t rows = 1u - static_cast<uint32_t>(
        static_cast<int64_t>((self->field_0018 - self->field_0014) *
                             g_GridCoordinateScale));
    self->field_0008 = rows;

    const uint32_t cellCount = columns * rows;
    auto* allocation = static_cast<uint8_t*>(Mem_Alloc(16u * cellCount + 4u));
    uint8_t* cells = nullptr;

    if (allocation)
    {
        CSF_AT(uint32_t, allocation, 0) = cellCount;
        cells = allocation + 4;

        if (static_cast<int32_t>(cellCount - 1u) >= 0)
        {
            uint8_t* cell = cells;
            uint32_t remaining = cellCount;
            do
            {
                CSF_AT(uint32_t, cell, 0) = 0;
                CSF_AT(uint32_t, cell, 0x4) = 0;
                CSF_AT(uint32_t, cell, 0x8) = 0;
                CSF_AT(uint32_t, cell, 0xC) = 1;
                cell += 16;
            } while (--remaining);
        }
    }

    self->field_001c = static_cast<uint32_t>(
        reinterpret_cast<uintptr_t>(cells));

    if (cells)
    {
        const uint32_t initialEntryCount = self->field_0020;
        auto* entry = reinterpret_cast<uint8_t*>(self->field_0024);
        uint32_t index = 0;

        if (initialEntryCount > 0)
        {
            do
            {
                S_004ddf20_InsertEntry(self, nullptr, entry);
                const uint32_t entryCount = self->field_0020;
                ++index;
                entry += 64;
                if (index >= entryCount)
                    break;
            } while (true);
        }
    }

    return true;
}

HOOK(0x004DE5B0, S_004ddf20_InitializeGrid, "auto");
