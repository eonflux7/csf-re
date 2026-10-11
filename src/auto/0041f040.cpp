#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees of the original (bound by address).
CSF_FUNC(int, __fastcall, S_0041e750_ResetTaskListCursor, 0x0041E750, (void* self, void* edx));
CSF_FUNC(void, __fastcall, S_0041e750_AppendTaskRecord, 0x0041ED90, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __fastcall, S_0041e750_ReserveVectorCapacity, 0x004DE9B0, (void* self, void* edx, int a2));

// Original: char __thiscall sub_41F040(S_0041e750* this, std::uint32_t* a2); ret 4
// Loads a block of 20-byte task records from a2 (a2[0] = record array, a2[2] = count).
static bool __fastcall S_0041e750_LoadTaskRecords(S_0041e750* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t recordOffset = 0;
    self->field_000c = 0;

    uint32_t recordCount = a2[2];
    uint32_t currentCapacity = self->field_0008;
    if (recordCount > currentCapacity)
    {
        // Original passes `this + 4` as the receiver of this call.
        S_0041e750_ReserveVectorCapacity(reinterpret_cast<uint8_t*>(self) + 4, nullptr,
                                         static_cast<int>(recordCount - currentCapacity));
    }

    for (; recordCount != 0; --recordCount)
    {
        // The original reloads a2[0] on every iteration.
        uintptr_t recordAddress = static_cast<uintptr_t>(a2[0]) + recordOffset + 4;
        S_0041e750_AppendTaskRecord(self, nullptr, reinterpret_cast<uint32_t*>(recordAddress));
        recordOffset += 20;
    }

    S_0041e750_ResetTaskListCursor(self, nullptr);

    self->field_0000 = 0;
    self->field_0001 = 1;
    self->field_0002 = 1;
    return true;
}

HOOK(0x0041F040, S_0041e750_LoadTaskRecords, "auto");
