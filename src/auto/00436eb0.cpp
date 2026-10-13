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

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetEntryCount, 0x0044BB80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetEntryAt, 0x0044BBA0, (void* self, void* edx, int a2));

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);

// Converts a 32-bit value returned in eax into a pointer-typed object handle.
static void* AsObjectPointer(int value) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

// Walks the entry list cyclically starting after the stored cursor and, when an entry
// with a negative field at +0x38 belongs to the current scene (+0x1C == g_CurrentSceneIndex),
// stores its index in the control's cursor field.
static int __fastcall CGuiTacticalMap_SyncCurrentPageIndex(CGuiTacticalMap* self, void* /*edx*/) {
    void* processor = AsObjectPointer(GetGlobalValue_8B0568());
    void* destination = UmsVirtualProcessor_GetDefaultDestination(processor, nullptr);
    int entryCount = GetEntryCount(destination, nullptr);

    int result = entryCount;
    uint32_t cursor = self->field_017c;
    bool found = false;
    int visited = 0;

    while (visited < entryCount) {
        if (static_cast<int>(++cursor) >= entryCount) {
            cursor = 0;
        }

        void* processorNow = AsObjectPointer(GetGlobalValue_8B0568());
        void* destinationNow = UmsVirtualProcessor_GetDefaultDestination(processorNow, nullptr);
        int entryHandle = GetEntryAt(destinationNow, nullptr, static_cast<int>(cursor));
        void* entry = AsObjectPointer(entryHandle);

        if (CSF_AT(int, entry, 0x38) < 0) {
            result = CSF_AT(int, entry, 0x1C);
            if (result == g_CurrentSceneIndex) {
                self->field_017c = cursor;
                found = true;
            }
        }

        ++visited;
        if (found) {
            break;
        }
        result = entryCount;
    }

    return result;
}

HOOK(0x436EB0, CGuiTacticalMap_SyncCurrentPageIndex, "auto");
