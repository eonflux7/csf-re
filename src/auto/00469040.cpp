#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneFallbackLookup, 0x004686A0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, SceneSlotMgr_ResolveSlotEntry, 0x00468FD0, (void* self, void* edx, int a2));

// Looks up a scene slot index for a2 through the fallback lookup; if one is found,
// resolves and returns its entry, otherwise returns nullptr.
static uint32_t* __fastcall SceneSlotMgr_FindSlot(S_004686a0* self, void* /*edx*/, int a2)
{
    int slotIndex = SceneFallbackLookup(self, nullptr, a2);
    if (slotIndex)
        return SceneSlotMgr_ResolveSlotEntry(self, nullptr, slotIndex);
    return nullptr;
}

HOOK(0x00469040, SceneSlotMgr_FindSlot, "auto");
