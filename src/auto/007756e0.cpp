#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies consecutive 60-byte records from source[0..] into [first, last).
// Each record: 4 dwords, 4 dwords, 1 dword, then a string object assigned from source+32 bytes.
// Returns last (the original's eax is always a2).
static std::uint32_t* __cdecl CopyRecordRange(std::uint32_t* first, std::uint32_t* last, std::uint32_t* source)
{
    std::uint32_t* result = last;
    std::uint32_t* recordHead = first;

    if (first != last)
    {
        std::uint32_t* recordFields = first + 4;
        do
        {
            // Header dword and the three following dwords.
            *recordHead = source[0];
            std::uint32_t* fieldsBase = recordFields - 3;
            fieldsBase[0] = source[1];
            fieldsBase[1] = source[2];
            fieldsBase[2] = source[3];

            // Four dwords of the second group.
            recordFields[0] = source[4];
            recordFields[1] = source[5];
            recordFields[2] = source[6];
            recordFields[3] = source[7];

            // String member at record offset 0x20, assigned from source offset 0x20 (count = -1 = whole string).
            AssignStringSubstring(reinterpret_cast<void*>(recordFields + 4), nullptr,
                                  reinterpret_cast<const void*>(source + 8), 0u, 0xFFFFFFFFu);

            result = last;
            recordHead += 15;
            recordFields += 15;
        } while (recordHead != last);
    }

    return result;
}

HOOK(0x7756E0, CopyRecordRange, "auto");
