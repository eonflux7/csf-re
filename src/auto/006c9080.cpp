#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static int __stdcall CopyNameRecordFromSource(C_0082d410* self, int source)
{
    int result = 0;
    if (self)
    {
        std::uint8_t* selfBytes = reinterpret_cast<std::uint8_t*>(self);
        std::uint8_t* sourceBytes = reinterpret_cast<std::uint8_t*>(source);

        CSF_AT(std::uint32_t, selfBytes, 0x18) = 0xF;
        CSF_AT(std::uint32_t, selfBytes, 0x14) = 0;
        CSF_AT(std::uint8_t, selfBytes, 0x04) = 0;

        AssignStringSubstring(self, nullptr, reinterpret_cast<const void*>(source), 0u, 0xFFFFFFFFu);

        result = CSF_AT(int, sourceBytes, 0x1C);
        CSF_AT(int, selfBytes, 0x1C) = result;
        CSF_AT(std::uint16_t, selfBytes, 0x20) = CSF_AT(std::uint16_t, sourceBytes, 0x20);
    }
    return result;
}

HOOK(0x006C9080, CopyNameRecordFromSource, "auto");
