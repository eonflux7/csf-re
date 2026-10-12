#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static std::uint32_t __stdcall CoreFiles_AssignEntryToSlot(
    C_0082d410* destination, C_0082d410* source)
{
    if (!destination)
        return 0;

    CSF_AT(std::uint32_t, destination, 0x18) = 15;
    CSF_AT(std::uint32_t, destination, 0x14) = 0;
    CSF_AT(std::uint8_t, destination, 0x04) = 0;
    AssignStringSubstring(destination, nullptr, source, 0, -1);

    const std::uint32_t entry = CSF_AT(std::uint32_t, source, 0x1C);
    CSF_AT(std::uint32_t, destination, 0x1C) = entry;
    return entry;
}

HOOK(0x00697AD0, CoreFiles_AssignEntryToSlot, "auto");
