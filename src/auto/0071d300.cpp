#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60,
         0x00429B60, (void* self, char* scanStart, char* text, std::uint32_t length));
CSF_FUNC(std::uint32_t, __fastcall, AppendElement64,
         0x0071D060, (void* self, void* edx, std::uint32_t* element));
CSF_FUNC(std::uint32_t, __fastcall, DestroyRecord_43E300,
         0x0043E300, (std::uint32_t* self, void* edx));

static std::uint32_t __fastcall AppendNamedEntry(
    void* self, void* /*edx*/, int value, char* name, char* alternateText)
{
    alignas(4) std::uint8_t entry[64];

    CSF_AT(std::uint32_t, entry, 0x18) = 15;
    CSF_AT(std::uint32_t, entry, 0x34) = 15;
    CSF_AT(std::uint32_t, entry, 0x14) = 0;
    CSF_AT(char, entry, 0x04) = 0;
    CSF_AT(std::uint32_t, entry, 0x30) = 0;
    CSF_AT(char, entry, 0x20) = 0;
    CSF_AT(std::uint32_t, entry, 0x38) = 0;
    CSF_AT(std::uint32_t, entry, 0x3C) = 0;

    std::uint32_t length = 0;
    while (name[length] != 0)
        ++length;

    AssignStringRange_429B60(entry, name + 1, name, length);

    char* text = name;
    if (alternateText)
        text = alternateText;

    length = 0;
    while (text[length] != 0)
        ++length;

    AssignStringRange_429B60(entry + 0x1C, text + 1, text, length);

    CSF_AT(int, entry, 0x38) = value;
    AppendElement64(static_cast<std::uint8_t*>(self) + 0x10,
                    nullptr, reinterpret_cast<std::uint32_t*>(entry));
    return DestroyRecord_43E300(reinterpret_cast<std::uint32_t*>(entry), nullptr);
}

HOOK(0x0071D300, AppendNamedEntry, "auto");
