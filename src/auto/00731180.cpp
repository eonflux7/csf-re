#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, InsertRepeatedValue, 0x00730BA0,
         (void* self, void* edx, std::uint32_t* position,
          std::uint32_t count, std::uint32_t* value));

static std::uint32_t __fastcall CGuiTextureListBox_AppendZeroEntries(
    CGuiTextureListBox* self, void* /*edx*/, int index,
    std::uint32_t count)
{
    if (count != 0)
    {
        const std::uint32_t entryAddress =
            self->field_01fc + (static_cast<std::uint32_t>(index) << 4);
        void* entry = reinterpret_cast<void*>(entryAddress);
        auto* position = reinterpret_cast<std::uint32_t*>(
            CSF_AT(std::uint32_t, entry, 0x08));

        const std::uint32_t insertionCount = count;
        count = 0;
        return InsertRepeatedValue(
            entry, nullptr, position, insertionCount, &count);
    }

    // The original leaves EAX unspecified on this path.
    std::uint32_t unspecifiedResult;
    return unspecifiedResult;
}

HOOK(0x00731180, CGuiTextureListBox_AppendZeroEntries, "auto");
