#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// std::string-like assign from another string at (pos, count); __thiscall, ret 12.
// Pushes: src, 0 (pos), -1 (count = npos). Returns this.
CSF_FUNC(void*, __fastcall, MenuExtras_AssignStringRange, 0x403B60,
         (void* self, void* edx, const void* src, uint32_t pos, uint32_t count));

// Copies a range of 0x44-byte records from src into [first, last).
// Each record: +0x00 dword, +0x04 string, +0x20 dword, +0x24 dword, +0x28 string.
// Returns last (a2) in every path.
static uint32_t* __cdecl CopyMenuExtrasRecordRange(uint32_t* first, uint32_t* last, uint32_t* src)
{
    uint32_t* cursor = first;
    while (cursor != last)
    {
        cursor[0] = src[0];
        MenuExtras_AssignStringRange(cursor + 1, nullptr, src + 1, 0, 0xFFFFFFFFu);
        cursor[8] = src[8];
        cursor[9] = src[9];
        MenuExtras_AssignStringRange(cursor + 10, nullptr, src + 10, 0, 0xFFFFFFFFu);
        cursor += 17;
    }
    return last;
}

HOOK(0x546FD0, CopyMenuExtrasRecordRange, "auto");
