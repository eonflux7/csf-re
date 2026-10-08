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

// Callees, bound by address.
CSF_FUNC(char, __cdecl, ReadFirstStoredIndexedValue, 0x540FF0, (int a1, uint32_t* a2));
CSF_FUNC(int, __fastcall, SaveSlotInfo_AssignFromRecord, 0x512D80, (void* self, void* edx, uint32_t record));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x4109E0, (void* self, void* edx, void* source, uint32_t offset, uint32_t count));

// Reads the key dword stored at +4 of a save record (record is an address value).
static uint32_t SaveRecord_ReadKeyField(uint32_t record)
{
    return CSF_AT(uint32_t, reinterpret_cast<void*>(record), 0x4);
}

// Selects the save record whose key matches one of the remembered keys (+0x100 / +0x104),
// looks up its index, refreshes the detail view, then notifies via vtable slot 0x48.
static int __fastcall CMenuLoadGame_SelectSaveRecordAndRefresh(CMenuLoadGame* self, void* /*edx*/, uint32_t recordRef)
{
    // Original record address, saved before ReadFirstStoredIndexedValue overwrites the parameter slot.
    const uint32_t requestedRecord = recordRef;

    // Gate: the object at +0x14 must report ready through vtable slot 0x104 (index 65).
    void* readinessObject = reinterpret_cast<void*>(self->field_0014);
    const int isReady = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, readinessObject, 0)[65])(readinessObject, nullptr);
    if (isReady == 0)
        return isReady;

    // Key match: first remembered key (+0x100), otherwise second remembered key (+0x104).
    const uint32_t selectedKeyA = self->field_0100;
    bool keyMatches = false;
    if (selectedKeyA != 0 && selectedKeyA == SaveRecord_ReadKeyField(requestedRecord))
    {
        keyMatches = true;
    }
    else
    {
        const uint32_t selectedKeyB = self->field_0104;
        keyMatches = (selectedKeyB != 0 && selectedKeyB == SaveRecord_ReadKeyField(requestedRecord));
    }

    if (keyMatches)
    {
        // Overwrites recordRef with the stored index of the record.
        if (ReadFirstStoredIndexedValue(static_cast<int>(selectedKeyA), &recordRef) != 0)
        {
            const uint32_t tableBase = self->field_010c;
            if (tableBase != 0)
            {
                // Signed division of the table byte span by the 200-byte record size.
                const int32_t slotCount = static_cast<int32_t>(self->field_0110 - self->field_010c) / 200;

                // Unsigned index bound check.
                if (recordRef < static_cast<uint32_t>(slotCount))
                {
                    const uint32_t recordAddress = self->field_010c + recordRef * 200u;

                    // Copy the 200-byte record into the embedded object at +0x38.
                    SaveSlotInfo_AssignFromRecord(reinterpret_cast<uint8_t*>(self) + 0x38, nullptr, recordAddress);

                    // Assign the string at +0x88 into the string object at +0x1C (offset 0, count npos).
                    AssignStringSubstring(reinterpret_cast<uint8_t*>(self) + 0x1C, nullptr,
                                              reinterpret_cast<uint8_t*>(self) + 0x88, 0u, 0xFFFFFFFFu);

                    // Refresh through vtable slot 0x50 (index 20) on self.
                    reinterpret_cast<void(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, self, 0)[20])(self, nullptr);
                }
            }
        }
    }

    // Final notification through vtable slot 0x48 (index 18) with the original record address.
    return reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, self, 0)[18])(self, nullptr, requestedRecord);
}

HOOK(0x005399A0, CMenuLoadGame_SelectSaveRecordAndRefresh, "auto");
