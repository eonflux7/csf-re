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

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Fills every 80-byte record in [rangeBegin, rangeEnd) from a template record.
// Returns rangeEnd. Record layout (byte offsets): 0 = id dword, 4 = string,
// 32..44 = four dwords, 48 = string, 76 = trailing dword.
static int __cdecl GameConfig_FillRecordRange(int rangeBegin, int rangeEnd, std::uint32_t* templateRecord)
{
    int result = rangeEnd;
    for (int recordAddr = rangeBegin; recordAddr != rangeEnd; recordAddr += 80)
    {
        std::uint8_t* record = reinterpret_cast<std::uint8_t*>(static_cast<std::intptr_t>(recordAddr));
        std::uint8_t* templateBytes = reinterpret_cast<std::uint8_t*>(templateRecord);

        CSF_AT(std::uint32_t, record, 0) = templateRecord[0];
        AssignStringSubstring(record + 4, nullptr, templateRecord + 1, 0, 0xFFFFFFFFu);

        CSF_AT(std::uint32_t, record, 32) = templateRecord[8];
        CSF_AT(std::uint32_t, record, 36) = templateRecord[9];
        CSF_AT(std::uint32_t, record, 40) = templateRecord[10];
        CSF_AT(std::uint32_t, record, 44) = templateRecord[11];

        AssignStringSubstring(record + 48, nullptr, templateRecord + 12, 0, 0xFFFFFFFFu);

        result = rangeEnd;
        CSF_AT(std::uint32_t, record, 76) = templateRecord[19];
        (void)templateBytes;
    }
    return result;
}

HOOK(0x755D70, GameConfig_FillRecordRange, "auto");
