#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(unsigned int, __fastcall, AppendPairRecord, 0x0046DA60, (void* self, void* edx, uint32_t* a3));

// Appends the current size of the list held at field_0044+0x4C as a new entry
// of the index table embedded at this+0x24, and returns that size.
static int __fastcall MissionScript_AppendListSizeRecord(S_0046c260* self, void* /*edx*/, int /*a2*/)
{
    uint8_t* listHolder = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0044)) + 0x4C;

    int listFirst = CSF_AT(int, listHolder, 4);
    int listCount;
    if (listFirst != 0)
        listCount = (CSF_AT(int, listHolder, 8) - listFirst) >> 3;
    else
        listCount = 0;

    int32_t sentinel[2] = { -1, -1 };
    AppendPairRecord(listHolder, nullptr, reinterpret_cast<uint32_t*>(sentinel));

    uint8_t* indexTable = reinterpret_cast<uint8_t*>(self) + 0x24;
    uint32_t usedCount = self->field_002c;
    uint32_t capacity = self->field_0028;
    if (usedCount >= capacity)
        GrowButtonArray(indexTable, nullptr, CSF_AT(int, indexTable, 0xC));

    uint32_t slotIndex = CSF_AT(uint32_t, indexTable, 8);
    uint8_t* tableData = CSF_AT(uint8_t*, indexTable, 0);
    *reinterpret_cast<int*>(tableData + 4 * slotIndex) = listCount;
    CSF_AT(uint32_t, indexTable, 8) = slotIndex + 1;

    (void)CSF_VTBL(0x0041E5F0);
    return listCount;
}

HOOK(0x0046E170, MissionScript_AppendListSizeRecord, "auto");
