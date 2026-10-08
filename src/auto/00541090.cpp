#include <cstdint>

#include "globals.h"
#include "hook.h"

// Original functions called by the target, bound by address.
CSF_FUNC(void*, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());
CSF_FUNC(int, __fastcall, sub_517150, 0x517150, (void* self, void* edx));
CSF_FUNC(int*, __fastcall, GetScenarioCollection, 0x515CE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearStorageAndSendEvent, 0x715660, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x716A60, (void* self, void* edx, int itemIndex));

// Rebuilds the profile listbox: refreshes the profile manager, clears the listbox,
// then adds one item per profile record (index, text) and finishes with AddSelectedItem(0).
static void __cdecl CMenuProfiles_RebuildProfileListBox(void* listBox)
{
    if (listBox == nullptr)
        return;

    void* globalContext = GetGlobalValue_8B056C();
    void* profileManager = CSF_AT(void*, globalContext, 0x8);

    // profileManager->vtbl[0x34 / 4]()  (no arguments, result unused)
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, profileManager, 0)[0x34 / 4])(profileManager, nullptr);
    sub_517150(profileManager, nullptr);
    int* profileRecords = GetScenarioCollection(profileManager, nullptr);

    // Record count of the vector at profileRecords: (end - begin) / 80,
    // computed with the original's signed multiply-high and sign correction.
    uint32_t recordCount = 0;
    const uint32_t recordsBegin = CSF_AT(uint32_t, profileRecords, 0x4);
    if (recordsBegin != 0)
    {
        const uint32_t recordsEnd = CSF_AT(uint32_t, profileRecords, 0x8);
        const int64_t scaledDifference =
            static_cast<int64_t>(1717986919) * static_cast<int32_t>(recordsEnd - recordsBegin);
        const int32_t quotient = static_cast<int32_t>(scaledDifference >> 32) >> 5;
        const uint32_t unsignedQuotient = static_cast<uint32_t>(quotient);
        recordCount = unsignedQuotient + (unsignedQuotient >> 31);
    }

    // listBox->vtbl[0x298 / 4]()  (no arguments)
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, listBox, 0)[0x298 / 4])(listBox, nullptr);
    CGuiListBoxBase_ClearStorageAndSendEvent(listBox, nullptr);

    uint32_t itemIndex = 0;
    if (recordCount != 0)
    {
        uint32_t recordByteOffset = 0;
        do
        {
            // The vector begin is re-read on every pass, as in the original.
            const uint32_t currentRecordsBegin = CSF_AT(uint32_t, profileRecords, 0x4);
            uint8_t* record =
                reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(currentRecordsBegin)) + recordByteOffset;

            // Text: inline buffer at +4 when the capacity at +0x18 is below 8,
            // otherwise a pointer stored at +4.
            const uint32_t textCapacity = CSF_AT(uint32_t, record, 0x18);
            const char* itemText = (textCapacity < 8)
                ? reinterpret_cast<const char*>(record + 4)
                : reinterpret_cast<const char*>(static_cast<uintptr_t>(CSF_AT(uint32_t, record, 0x4)));

            // listBox->vtbl[0x294 / 4](itemIndex, 0, itemText, itemIndex)
            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t, int, const char*, uint32_t)>(
                CSF_AT(void**, listBox, 0)[0x294 / 4])(listBox, nullptr, itemIndex, 0, itemText, itemIndex);

            ++itemIndex;
            recordByteOffset += 80;
        } while (itemIndex < recordCount);
    }

    CGuiListBoxBase_AddSelectedItem(listBox, nullptr, 0);
}

HOOK(0x00541090, CMenuProfiles_RebuildProfileListBox, "auto");
