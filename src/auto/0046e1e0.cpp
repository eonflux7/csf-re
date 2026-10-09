#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: button-array growth (__thiscall, one stack arg, ret 4).
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));

// Callee: appends a 2-dword record {value, -1} to the list at this+0x5C of the sub-object (ret 4).
CSF_FUNC(unsigned int, __fastcall, ListAppendEntry, 0x0046DA60, (void* self, void* edx, uint32_t* a3));

// The original references S_0041e5f0's vtable at its call site; keep the binding present.
[[maybe_unused]] static const auto s_ButtonArrayVtbl = CSF_VTBL(0x0041E5F0);

// Appends a value to the mission-script index list and returns the index it was stored at.
static int __fastcall MissionScript_PushIndexEntry(S_0046c260* self, void* /*edx*/, int entryValue)
{
    // Current element count of the list at this->field_0044 + 0x5C (signed view).
    int* listHeader = reinterpret_cast<int*>(self->field_0044 + 0x5C);
    int entryIndex;
    int listBegin = listHeader[1];
    if (listBegin != 0)
        entryIndex = (listHeader[2] - listBegin) >> 3;
    else
        entryIndex = 0;

    // Record {entryValue, -1} handed to the list appender.
    uint32_t record[2];
    record[1] = 0xFFFFFFFFu;
    record[0] = static_cast<uint32_t>(entryValue);
    ListAppendEntry(listHeader, nullptr, record);

    // Index array embedded at this+0x34.
    S_0041e5f0* indexArray = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x34);
    if (self->field_003c < self->field_0038)
    {
        // No growth needed.
    }
    else
    {
        GrowButtonArray(indexArray, nullptr, indexArray->field_000c);
    }

    uint32_t slot = indexArray->field_0008;
    int* slotPtr = reinterpret_cast<int*>(indexArray->field_0000 + 4 * slot);
    indexArray->field_0008 = slot + 1;
    *slotPtr = entryIndex;
    return entryIndex;
}

HOOK(0x0046E1E0, MissionScript_PushIndexEntry, "auto");
