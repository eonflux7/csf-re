#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees (addresses from the packet)
CSF_FUNC(void*, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());
CSF_FUNC(uint32_t*, __fastcall, GetScenarioCollection, 0x515CE0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobal8afc7c, 0x43E010, ());
CSF_FUNC(uint32_t*, __fastcall, MenuLoading_FindMission, 0x43E4B0, (void* self, void* edx, int recordId));

// Returns 1 when the entry list of the CMenuExtras instance contains an entry whose id matches
// the first dword of the record found for recordId and whose flag byte (+8) is nonzero.
// Only the first id match is evaluated (the loop breaks on it).
static char __stdcall CMenuExtras_IsThumbnailUnlocked(int recordId)
{
    char isUnlocked = 0;

    void* managerRoot = GetGlobalValue_8B056C();
    void* extrasObject = CSF_AT(void*, managerRoot, 0x8);
    if (extrasObject == nullptr)
        return 0;

    uint32_t* entryRange = GetScenarioCollection(extrasObject, nullptr);
    uint32_t rangeBegin = entryRange[1];
    if (rangeBegin == 0)
        return 0;
    if (static_cast<int32_t>(entryRange[2] - rangeBegin) / 80 == 0)
        return 0;

    // Vtable slot 9 (+0x24) of extrasObject yields the CMenuExtras instance.
    CMenuExtras* extras = reinterpret_cast<CMenuExtras*>(
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, extrasObject, 0)[9])(extrasObject, nullptr));

    void* recordManager = GetGlobal8afc7c();
    uint32_t* recordPtr = MenuLoading_FindMission(recordManager, nullptr, recordId);
    if (recordPtr == nullptr)
        return isUnlocked;

    // Entry count = (end - begin) / 16, using an arithmetic (signed) shift; the count is
    // only read when the begin pointer is nonzero.
    uint32_t entriesBegin = extras->field_0038;
    uint32_t entryCount = 0;
    if (entriesBegin != 0)
    {
        int32_t entryBytes = static_cast<int32_t>(extras->field_003c - entriesBegin);
        entryCount = static_cast<uint32_t>(entryBytes >> 4);
    }

    uint32_t entryIndex = 0;
    uint32_t entryByteOffset = 0;
    while (entryIndex < entryCount)
    {
        uint32_t entryAddress = extras->field_0038 + entryByteOffset;
        uint8_t* entry = reinterpret_cast<uint8_t*>(entryAddress);
        bool idMatches = *reinterpret_cast<uint32_t*>(entry) == *recordPtr;
        if (!idMatches)
            isUnlocked = 0;
        else
            isUnlocked = (*(entry + 8) != 0) ? 1 : 0;

        ++entryIndex;
        entryByteOffset += 16;
        if (idMatches)
            break;
    }

    return isUnlocked;
}

HOOK(0x545F00, CMenuExtras_IsThumbnailUnlocked, "auto");
