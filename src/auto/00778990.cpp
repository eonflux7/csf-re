#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binds (see packet): string assignment helper, __thiscall as __fastcall.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Binds (see packet): copies the 0x38-byte sub-block at +56 of a record.
CSF_FUNC(int, __fastcall, CopyRecordSubBlock, 0x00776DB0,
         (void* self, void* edx, int a4a));

// Copies strings and the sub-block for each 72-byte record in [firstRecord, endRecord),
// reading from the source record at sourceRecord. Returns endRecord.
static int __cdecl GameDataDefs_AssignRecordRange(int firstRecord, int endRecord, int sourceRecord)
{
    for (int record = firstRecord; record != endRecord; record += 0x48)
    {
        AssignStringSubstring(reinterpret_cast<C_0082d410*>(record), nullptr,
                              reinterpret_cast<const void*>(sourceRecord), 0, static_cast<std::uint32_t>(-1));

        AssignStringSubstring(reinterpret_cast<C_0082d410*>(record + 0x1c), nullptr,
                              reinterpret_cast<const void*>(sourceRecord + 0x1c), 0, static_cast<std::uint32_t>(-1));

        CopyRecordSubBlock(reinterpret_cast<void*>(record + 0x38), nullptr, sourceRecord + 0x38);
    }
    return endRecord;
}

HOOK(0x00778990, GameDataDefs_AssignRecordRange, "auto");
