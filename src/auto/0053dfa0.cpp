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

CSF_FUNC(int, __cdecl, IsMissionPanelSystemReady, 0x0044FB00, ());
CSF_FUNC(void*, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, GetDefaultDestinationRecord, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(void*, __fastcall, ResolveCurrentPanelKey, 0x0043E5B0, (void* self, void* edx));
CSF_FUNC(int*, __fastcall, FindPanelEntryByKey, 0x0043E4B0, (void* self, void* edx, void* key));

static char __stdcall CMenuMissionInfo_IsCurrentEntryAbsentFromList(int a1)
{
    if ((IsMissionPanelSystemReady() & 0xFF) == 0)
        return 0;

    void* ownerFirst = GetGlobalValue_8B0568();
    if (GetDefaultDestinationRecord(ownerFirst, nullptr) == nullptr)
        return 0;

    void* ownerSecond = GetGlobalValue_8B0568();
    void* destinationRecord = GetDefaultDestinationRecord(ownerSecond, nullptr);
    if (CSF_AT(uint32_t, destinationRecord, 0x1C) != 0)
        return 0;

    void* panelHolderForKey = GetGlobal8afc7c();
    void* currentKey = ResolveCurrentPanelKey(panelHolderForKey, nullptr);

    void* panelHolderForEntry = GetGlobal8afc7c();
    int* entry = FindPanelEntryByKey(panelHolderForEntry, nullptr, currentKey);
    if (entry == nullptr)
        return 0;

    void* list = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a1)));
    uint32_t listBegin = CSF_AT(uint32_t, list, 0x38);

    uint32_t elementCount = 0;
    if (listBegin != 0) {
        int32_t byteSpan = static_cast<int32_t>(CSF_AT(uint32_t, list, 0x3C) - listBegin);
        elementCount = static_cast<uint32_t>(byteSpan >> 4);
    }
    if (elementCount == 0)
        return 1;

    int32_t entryKey = static_cast<int32_t>(*entry);
    uint8_t* element = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(listBegin));
    for (uint32_t index = 0; index < elementCount; ++index, element += 16) {
        if (static_cast<int32_t>(CSF_AT(uint32_t, element, 0)) == entryKey)
            return 0;
    }
    return 1;
}

HOOK(0x0053DFA0, CMenuMissionInfo_IsCurrentEntryAbsentFromList, "auto");
