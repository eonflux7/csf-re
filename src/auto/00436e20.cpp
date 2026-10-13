#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetEntryCount, 0x0044BB80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetEntryAt, 0x0044BBA0, (void* self, void* edx, int a2));

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);

// Walks the default destination's entry ring backwards from the stored index and
// stores the first entry that has a negative field at +0x38 and a matching scene
// index at +0x1C into the object's index field at +0x17C.
static void __fastcall CMenuMissionInfo_ReleaseOwnedObject(void* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x17C) == 0xFFFFFFFFu)
        return;

    void* processor = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8B0568()));
    void* defaultDestination = UmsVirtualProcessor_GetDefaultDestination(processor, nullptr);
    uint32_t entryCount = static_cast<uint32_t>(GetEntryCount(defaultDestination, nullptr));

    uint32_t candidateIndex = CSF_AT(uint32_t, self, 0x17C);
    bool found = false;
    uint32_t visited = 0;

    while (visited < entryCount)
    {
        if (candidateIndex != 0)
            --candidateIndex;
        else
            candidateIndex = entryCount - 1;

        void* loopProcessor = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8B0568()));
        void* loopDestination = UmsVirtualProcessor_GetDefaultDestination(loopProcessor, nullptr);
        int entryAt = GetEntryAt(loopDestination, nullptr, static_cast<int>(candidateIndex));

        void* entry = reinterpret_cast<void*>(static_cast<uintptr_t>(entryAt));
        if (CSF_AT(int, entry, 0x38) < 0 && CSF_AT(int, entry, 0x1C) == g_CurrentSceneIndex)
        {
            CSF_AT(uint32_t, self, 0x17C) = candidateIndex;
            found = true;
        }

        ++visited;
        if (found)
            break;
    }
}

HOOK(0x00436E20, CMenuMissionInfo_ReleaseOwnedObject, "auto");
