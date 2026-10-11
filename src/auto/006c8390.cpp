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

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* edx, int value));
CSF_FUNC(char, __fastcall, GfxSkin_FinalizeObjectEntry, 0x006C4740, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxSkin_GrowRecordList, 0x006C7DD0, (void* self, void* edx, int a2));

// Returns the object stored at self->objects[index] (array pointer at +0x04, 4-byte entries).
static void* GfxSkin_ObjectAt(void* self, int index)
{
    uint32_t* objects = CSF_AT(uint32_t*, self, 0x04);
    return reinterpret_cast<void*>(static_cast<uintptr_t>(objects[index]));
}

// Registers object #objIndex in the record list at self+0x14 (12-byte records:
// object, provider, flag). Returns the new record count.
static int __fastcall GfxSkin_AppendObjectRecord(void* self, void* /*edx*/, int objIndex)
{
    // Provider of the object, stored into the record's second dword.
    uint32_t provider = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(
        GetCachedObjectProvider(GfxSkin_ObjectAt(self, objIndex), nullptr)));

    // Value of the last record's first dword (if any records exist).
    uint32_t recordCount = CSF_AT(uint32_t, self, 0x1C);
    int lastValue = 0;
    if (recordCount != 0) {
        uint8_t* records = CSF_AT(uint8_t*, self, 0x14);
        lastValue = CSF_AT(int, records, 12 * (recordCount - 1));
    }

    C_008670f0_SetField0C(reinterpret_cast<C_008670f0*>(GfxSkin_ObjectAt(self, objIndex)), nullptr, lastValue);
    GfxSkin_FinalizeObjectEntry(GfxSkin_ObjectAt(self, objIndex), nullptr);

    // Flag stored in the record's third dword.
    int recordFlag = 0;

    // Grow the record list if it is full.
    uint32_t usedCount = CSF_AT(uint32_t, self, 0x1C);
    uint32_t capacity = CSF_AT(uint32_t, self, 0x18);
    void* recordList = reinterpret_cast<uint8_t*>(self) + 0x14;
    if (usedCount >= capacity) {
        GfxSkin_GrowRecordList(recordList, nullptr, CSF_AT(int, self, 0x20));
    }

    // Append the record.
    uint32_t index = CSF_AT(uint32_t, self, 0x1C);
    int result = static_cast<int>(index + 1);
    CSF_AT(uint32_t, self, 0x1C) = static_cast<uint32_t>(result);

    uint8_t* record = CSF_AT(uint8_t*, self, 0x14) + 12 * index;
    CSF_AT(uint32_t, record, 0x00) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(GfxSkin_ObjectAt(self, objIndex)));
    CSF_AT(uint32_t, record, 0x04) = provider;
    CSF_AT(int, record, 0x08) = recordFlag;

    return result;
}

HOOK(0x006C8390, GfxSkin_AppendObjectRecord, "auto");
