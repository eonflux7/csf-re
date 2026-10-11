#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: 0x516DA0, __thiscall with 4 stack args (ret 12).
CSF_FUNC(void*, __fastcall, RefreshRecordRange, 0x00516DA0,
         (void* self, void* edx, void* a1, unsigned int a3, void* a2));

// Given a value a3 and a record index computed from the table's begin/end
// pointers (80-byte records), refreshes the range and writes the address of
// the matching record to *out. Returns out.
static uint32_t* __fastcall S_005170e0_ResolveRecordAddress(S_005170e0* self, void* /*edx*/,
                                                             uint32_t* out, int value, int extra)
{
    uint32_t recordBegin = self->field_0004;
    int recordIndex = 0;

    if (recordBegin != 0) {
        int recordCount = (int)(self->field_0008 - recordBegin) / 80;
        if (recordCount != 0) {
            recordIndex = (int)((uint32_t)value - recordBegin) / 80;
        }
    }

    RefreshRecordRange(self, nullptr,
                       reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value))),
                       1u,
                       reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(extra))));

    *out = self->field_0004 + 80u * (uint32_t)recordIndex;
    return out;
}

HOOK(0x005170E0, S_005170e0_ResolveRecordAddress, "auto");
