#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Binds the unnamed callee at 0x40ACC0 (__thiscall, one stack arg, ret 4).
CSF_FUNC(int, __fastcall, Sub40ACC0_CopySubBlock, 0x0040ACC0, (void* self, void* edx, int a2));

// __cdecl, ret 0: copies a 3-dword header from the template record into each
// 28-byte record in [first, end), then copies the sub-block at +12 via 0x40ACC0.
static uint32_t* __cdecl FillRecordRangeFromTemplate(uint32_t* first, uint32_t* end, uint32_t* templateRecord)
{
    uint32_t* result = end;
    for (uint32_t* record = first; record != end; record += 7)
    {
        record[0] = templateRecord[0];
        record[1] = templateRecord[1];
        record[2] = templateRecord[2];
        Sub40ACC0_CopySubBlock(reinterpret_cast<uint8_t*>(record) + 12, nullptr,
                               static_cast<int>(reinterpret_cast<uintptr_t>(templateRecord + 3)));
        result = end;
    }
    return result;
}

HOOK(0x0040B420, FillRecordRangeFromTemplate, "auto");
