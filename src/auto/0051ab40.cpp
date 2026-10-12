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

CSF_FUNC(char, __fastcall, TableEntry_QueryFlag, 0x00774B40, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GfxTextureLoader_ResetRecordRange, 0x006DEC40, (int* begin, int* end));
CSF_FUNC(void*, __fastcall, TableEntry_RebuildRecords, 0x0051AAC0, (void* self, void* edx, unsigned int a2, char a3, void* Block, int a5, int a6));

// Clears the record block owned by this entry, rebuilds the record table from the
// list header at field_0004 (one 96-byte record per element), and returns the flag
// reported by TableEntry_QueryFlag.
static char __fastcall C_00840724_ResetAndRebuild(C_00840724* self, void* /*edx*/, int a2)
{
    uint8_t* subObject = reinterpret_cast<uint8_t*>(self) + 8;

    char flagResult = TableEntry_QueryFlag(self, nullptr, a2);

    if (self->field_000c)
    {
        GfxTextureLoader_ResetRecordRange(
            reinterpret_cast<int*>(self->field_000c),
            reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x10)));
        if (self->field_000c)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_000c));
    }

    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;

    uint8_t* listHeader = reinterpret_cast<uint8_t*>(self->field_0004);
    uint32_t listBegin = CSF_AT(uint32_t, listHeader, 4);
    unsigned int recordCount = 0;
    if (listBegin != 0)
    {
        int spanBytes = (int)(CSF_AT(uint32_t, listHeader, 8) - listBegin);
        recordCount = (unsigned int)(spanBytes / 96);
    }

    TableEntry_RebuildRecords(subObject, nullptr, recordCount, 0, nullptr, 0, 0);
    return flagResult;
}

HOOK(0x0051AB40, C_00840724_ResetAndRebuild, "auto");
