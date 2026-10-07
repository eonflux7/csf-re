#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static std::uint32_t* __fastcall CGuiListBox_SetEntryValue(
    CGuiListBox* self, void* /*edx*/,
    int containerIndex, int entryIndex, int value)
{
    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);

    const std::uint32_t containerAddress =
        self->field_0220 +
        static_cast<std::uint32_t>(containerIndex) * 16u + 4u;

    const std::uint32_t entriesAddress =
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(containerAddress), 0);

    auto* entry = reinterpret_cast<std::uint32_t*>(
        entriesAddress + static_cast<std::uint32_t>(entryIndex) * 60u);

    *entry = static_cast<std::uint32_t>(value);
    return entry;
}

HOOK(0x007231F0, CGuiListBox_SetEntryValue, "auto");
