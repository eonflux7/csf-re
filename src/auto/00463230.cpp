#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, ComputeKeyHash, 0x0050DCF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetPositionLookupTable, 0x0075C260, (void* self, void* edx));
CSF_FUNC(int, __fastcall, FindLookupEntryByKey, 0x0075CF00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SearchLookupTableForKey, 0x00462B10, (void* self, void* edx, uint32_t* a1, int a2, char a3, char a4));

// Output record: its +4 field holds a pointer to the position block.
static uint8_t* OutputRecordBase(uint32_t* outRecord)
{
    return reinterpret_cast<uint8_t*>(outRecord[1]);
}

// __thiscall method (ecx = this, 3 stack args, ret 12); Hex-Rays labelled it __stdcall.
static unsigned int __fastcall LookupPositionForKeyIntoRecord(void* self, void* /*edx*/, uint32_t* outRecord, int /*a2*/, uint32_t* keySource)
{
    C_0083e0d8* manager = *reinterpret_cast<C_0083e0d8**>(reinterpret_cast<uint8_t*>(GetValue8B0554()) + 8152);

    // Reset the output position to zero and both ids to -1.
    uint8_t* outBase = OutputRecordBase(outRecord);
    CSF_AT(float, outBase, 4) = g_ZeroVectorX;
    CSF_AT(float, outBase, 8) = g_ZeroVectorY;
    CSF_AT(float, outBase, 12) = g_ZeroVectorZ;
    CSF_AT(int32_t, OutputRecordBase(outRecord), 16) = -1;
    CSF_AT(int32_t, OutputRecordBase(outRecord), 20) = -1;

    // Key source chain: *keySource -> +4 -> +4 gives the lookup key.
    uint32_t* keyHolder = reinterpret_cast<uint32_t*>(keySource[0]);
    uint32_t* keyOwner = reinterpret_cast<uint32_t*>(keyHolder[1]);
    int lookupKey = static_cast<int>(keyOwner[1]);

    uint32_t* lookupTable = reinterpret_cast<uint32_t*>(GetPositionLookupTable(manager, nullptr));
    SearchLookupTableForKey(self, nullptr, lookupTable, lookupKey, 0, 0);

    uint32_t result = CSF_AT(uint32_t, lookupTable, 8);
    uint32_t slotCount = result;
    if (result != 0)
    {
        int globalState = GetGlobalState_50D190();
        uint32_t keyHash = static_cast<uint32_t>(ComputeKeyHash(reinterpret_cast<void*>(globalState), nullptr));
        uint32_t bucketIndex = keyHash % slotCount;

        // Each bucket entry is 8 bytes: { key, record index }.
        int* bucketEntry = reinterpret_cast<int*>(CSF_AT(uint32_t, lookupTable, 0) + 8 * bucketIndex);
        uint8_t* entryObject = reinterpret_cast<uint8_t*>(FindLookupEntryByKey(manager, nullptr, bucketEntry[0]));

        // Index is reloaded after the call, as the original does.
        uint8_t* entryRecords = CSF_AT(uint8_t*, entryObject, 0x38);
        uint32_t* record = reinterpret_cast<uint32_t*>(entryRecords + 44 * bucketEntry[1]);

        uint8_t* outBaseAfterLookup = OutputRecordBase(outRecord);
        CSF_AT(uint32_t, outBaseAfterLookup, 4) = record[1];
        CSF_AT(uint32_t, outBaseAfterLookup, 8) = record[2];
        CSF_AT(uint32_t, outBaseAfterLookup, 12) = record[3];
        CSF_AT(uint32_t, OutputRecordBase(outRecord), 16) = CSF_AT(uint32_t, entryObject, 0);

        result = record[0];
        CSF_AT(uint32_t, OutputRecordBase(outRecord), 20) = result;
    }

    manager->field_0030 = 0;
    return result;
}

HOOK(0x00463230, LookupPositionForKeyIntoRecord, "auto");
