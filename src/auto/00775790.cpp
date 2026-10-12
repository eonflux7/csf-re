#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Element stride of the record array (0x34 bytes per entry).
static constexpr std::uintptr_t kRecordStride = 52;

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies the non-string payload of a template record into every record in [rangeBegin, rangeEnd).
// Each record first receives the template's string (whole string, position 0, count -1).
static char __cdecl CopyRecordRangeFromTemplate(int rangeBegin, int rangeEnd, int templateRecord)
{
    std::uint8_t* const endRecord = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(rangeEnd));
    std::uint8_t* const templ = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(templateRecord));

    char lastFlagByte = 0;
    for (std::uint8_t* record = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(rangeBegin));
         record != endRecord;
         record += kRecordStride)
    {
        AssignStringSubstring(reinterpret_cast<C_0082d410*>(record), nullptr, templ, 0u, 0xFFFFFFFFu);

        CSF_AT(std::uint32_t, record, 0x1C) = CSF_AT(std::uint32_t, templ, 0x1C);
        CSF_AT(std::uint32_t, record, 0x20) = CSF_AT(std::uint32_t, templ, 0x20);
        CSF_AT(std::uint32_t, record, 0x24) = CSF_AT(std::uint32_t, templ, 0x24);
        CSF_AT(std::uint32_t, record, 0x28) = CSF_AT(std::uint32_t, templ, 0x28);
        CSF_AT(std::uint32_t, record, 0x2C) = CSF_AT(std::uint32_t, templ, 0x2C);
        CSF_AT(std::uint8_t, record, 0x30) = CSF_AT(std::uint8_t, templ, 0x30);
        lastFlagByte = static_cast<char>(CSF_AT(std::uint8_t, templ, 0x31));
        CSF_AT(std::uint8_t, record, 0x31) = static_cast<std::uint8_t>(lastFlagByte);
        CSF_AT(std::uint8_t, record, 0x32) = CSF_AT(std::uint8_t, templ, 0x32);
    }
    return lastFlagByte;
}

HOOK(0x00775790, CopyRecordRangeFromTemplate, "auto");
