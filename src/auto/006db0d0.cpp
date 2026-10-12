#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, AppendSelectionIndex, 0x005570F0, (S_00553fa0* self, void* /*edx*/, std::int32_t* a3));

// Clears/assigns selection slots marked -1 (pending) inside the entry array at this+8..this+12.
// Entries are 16-byte records; word +4 holds the slot index, word +8 holds a generation stamp
// compared against this+52 (incremented on each call).
static void __fastcall GfxCameraProjection_ApplyTransform(void* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x34) += 1;

    std::uint32_t entryCount = 0;
    std::uint32_t entriesBegin = CSF_AT(std::uint32_t, self, 0x08);
    if (entriesBegin != 0)
        entryCount = (CSF_AT(std::uint32_t, self, 0x0C) - entriesBegin) >> 4;

    std::uint32_t i = 0;
    if (entryCount == 0)
        return;

    std::uint32_t entryOffset = 0;
    do
    {
        std::uint32_t base = CSF_AT(std::uint32_t, self, 0x08);
        std::uint8_t* entry = reinterpret_cast<std::uint8_t*>(base + entryOffset);

        std::uint32_t slotValue = CSF_AT(std::uint32_t, entry, 0x04);
        if (slotValue == 0xFFFFFFFFu && CSF_AT(std::uint32_t, entry, 0x08) != CSF_AT(std::uint32_t, self, 0x34))
        {
            std::uint32_t selectionBegin = CSF_AT(std::uint32_t, self, 0x18);
            std::int32_t selectionCount = 0;
            if (selectionBegin != 0)
                selectionCount = static_cast<std::int32_t>(CSF_AT(std::uint32_t, self, 0x1C) - selectionBegin) >> 2;

            CSF_AT(std::int32_t, entry, 0x04) = selectionCount;

            std::int32_t selectionIndex = static_cast<std::int32_t>(i);
            AppendSelectionIndex(reinterpret_cast<S_00553fa0*>(static_cast<std::uint8_t*>(self) + 0x14),
                                 nullptr, &selectionIndex);
        }

        ++i;
        entryOffset += 0x10;
    } while (i < CSF_AT(std::uint32_t, self, 0x08) == 0 ? false : i < entryCount);
}

HOOK(0x006DB0D0, GfxCameraProjection_ApplyTransform, "auto");
