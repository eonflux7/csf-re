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

// Slot helpers operating on the by-value C_00858168 stack object (its address is `this`).
CSF_FUNC(uint32_t*, __fastcall, NetSessionSlot_SetName, 0x0066F130, (void* self, void* edx, char* a2));
CSF_FUNC(void, __fastcall, NetSessionSlot_Reset, 0x0066F150, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, NetSessionSlot_MatchesKey, 0x0066F1A0, (void* self, void* edx, void** a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));

// Walks the intrusive list of session entries. For each entry, the world-object
// collection name is fed to the slot, which then tests it against wantedKey.
// Returns true on the first entry that matches (slot reset before returning),
// false when the list is empty or exhausted.
static bool __stdcall NetSession_HasMatchingEntry(uint32_t listHeadRef, uint32_t wantedKey)
{
    uint8_t* listHead = reinterpret_cast<uint8_t*>(listHeadRef);
    uint8_t* sentinel = CSF_AT(uint8_t*, listHead, 4);
    uint8_t* node = CSF_AT(uint8_t*, sentinel, 0);
    if (node == sentinel)
        return false;

    while (true)
    {
        void* entryObject = node + 8;
        char* worldObjectName = reinterpret_cast<char*>(
            static_cast<uintptr_t>(SceneLoader_GetWorldObjectCollection(entryObject, nullptr)));
        NetSessionSlot_SetName(&listHeadRef, nullptr, worldObjectName);

        if (NetSessionSlot_MatchesKey(&listHeadRef, nullptr,
                reinterpret_cast<void**>(static_cast<uintptr_t>(wantedKey))))
            break;

        NetSessionSlot_Reset(&listHeadRef, nullptr);
        node = CSF_AT(uint8_t*, node, 0);
        if (node == CSF_AT(uint8_t*, listHead, 4))
            return false;
    }

    NetSessionSlot_Reset(&listHeadRef, nullptr);
    return true;
}

HOOK(0x0073D2A0, NetSession_HasMatchingEntry, "auto");
