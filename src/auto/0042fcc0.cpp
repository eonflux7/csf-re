#include <cstddef>
#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Writes the default state of one 60-byte table entry (offsets relative to the entry start).
static void Hud_ResetTableElement(std::uint8_t* element)
{
    CSF_AT(std::int32_t, element, 0x04) = 0;
    CSF_AT(std::int32_t, element, 0x08) = 0;
    CSF_AT(std::int32_t, element, 0x0C) = 0;
    CSF_AT(std::int32_t, element, 0x10) = 0;
    CSF_AT(std::int32_t, element, 0x18) = 0;
    CSF_AT(std::int32_t, element, 0x1C) = 0;
    CSF_AT(std::int32_t, element, 0x20) = 0;
    CSF_AT(float, element, 0x24) = 1.0f;
    CSF_AT(std::int32_t, element, 0x28) = 0;
    CSF_AT(std::int32_t, element, 0x2C) = 0;
    CSF_AT(std::int32_t, element, 0x30) = 0;
    CSF_AT(std::int32_t, element, 0x34) = 0;
    CSF_AT(std::int32_t, element, 0x14) = 0;
    CSF_AT(std::int32_t, element, 0x00) = 0;
    CSF_AT(std::int32_t, element, 0x38) = -1;
}

// Object layout (as far as this function shows it):
//   +0x00 table pointer (60-byte entries)
//   +0x04 capacity counter (increased by extraCount)
//   +0x08 number of entries to copy from the old table
// Returns the old table pointer when it was null, otherwise the value returned by Mem_Free.
static const void* __fastcall Hud_GrowElementTable(void* self, void* /*edx*/, int extraCount)
{
    // 32-bit wrapping sum, as the original's integer add does.
    const std::int32_t totalCount = static_cast<std::int32_t>(
        static_cast<std::uint32_t>(CSF_AT(std::int32_t, self, 0x04)) +
        static_cast<std::uint32_t>(extraCount));

    void* newTable = nullptr;
    void* allocated = Mem_Alloc(static_cast<std::size_t>(static_cast<std::uint32_t>(totalCount) * 60u));
    if (allocated != nullptr) {
        newTable = allocated;

        // Signed test on the wrapped (totalCount - 1), as the original's jl does.
        const std::int32_t lastIndex = static_cast<std::int32_t>(static_cast<std::uint32_t>(totalCount) - 1u);
        if (lastIndex >= 0) {
            // The original loop runs totalCount times, counted as an unsigned 32-bit value.
            std::uint32_t remaining = static_cast<std::uint32_t>(totalCount);
            std::uint8_t* element = static_cast<std::uint8_t*>(newTable);
            do {
                Hud_ResetTableElement(element);
                element += 60;
                --remaining;
            } while (remaining != 0);
        }
    }

    const std::uint32_t oldUsedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (oldUsedCount != 0) {
        std::memcpy(newTable,
                    CSF_AT(void*, self, 0x00),
                    static_cast<std::size_t>(60u * oldUsedCount));
    }

    const void* result = CSF_AT(const void*, self, 0x00);
    const bool oldTableWasNull = (result == nullptr);
    CSF_AT(int, self, 0x04) = CSF_AT(int, self, 0x04) + extraCount;

    if (!oldTableWasNull) {
        result = reinterpret_cast<const void*>(Mem_Free(const_cast<void*>(result)));
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newTable;
    return result;
}

HOOK(0x0042FCC0, Hud_GrowElementTable, "auto");
