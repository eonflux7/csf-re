#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static C_0082d410* __cdecl CoreFiles_CopyStringRecords(
    int first, int last, C_0082d410* destination)
{
    std::uint32_t sourceAddress = static_cast<std::uint32_t>(first);
    const std::uint32_t endAddress = static_cast<std::uint32_t>(last);
    auto* output = reinterpret_cast<std::uint8_t*>(destination);

    while (sourceAddress != endAddress)
    {
        const void* source = reinterpret_cast<const void*>(sourceAddress);
        AssignStringSubstring(output, nullptr, source, 0, -1);
        CSF_AT(std::uint32_t, output, 0x1C) =
            CSF_AT(std::uint32_t, source, 0x1C);
        sourceAddress += 32;
        output += 32;
    }

    return reinterpret_cast<C_0082d410*>(output);
}

HOOK(0x00697A40, CoreFiles_CopyStringRecords, "auto");
