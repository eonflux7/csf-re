#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(int, __fastcall, Unit_FindSlotForTarget, 0x004A6CF0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

// Referenced vtable of S_00492b20 (as listed by the packet for the call at 0x4A4A51).
[[maybe_unused]] static const void* kS00492b20VtblRef = CSF_VTBL(0x00492B20);

static int __fastcall S_00492b20_DispatchToResolvedTarget(S_00492b20** self, void* /*edx*/, S_00771b80* a2)
{
    // Handle stored at this[2] is turned into a handle pointer, then resolved in the current scene table.
    void* sceneHandleTable = g_pCurrentScene;
    char* handlePtr = S_00492b20_GetPtr0x1B4(CSF_AT(void*, self, 0x8), nullptr);
    int resolvedHandle = CHandleTable_Resolve(sceneHandleTable, nullptr,
                                              static_cast<int>(reinterpret_cast<intptr_t>(handlePtr)));
    if (resolvedHandle == 0)
        return -1;

    // Object at +0x48 of the resolved entry is the owner passed to the slot lookup.
    void* resolvedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(resolvedHandle));
    void* slotOwner = CSF_AT(void*, resolvedObject, 0x48);
    int slotIndex = Unit_FindSlotForTarget(slotOwner, nullptr, a2);
    if (slotIndex == -1)
        return -1;

    // Dispatch through the service object at +0x1FC4 (offset 8132 in the original's decimal view).
    int serviceHolderValue = GetValue8B0554();
    void* serviceHolder = reinterpret_cast<void*>(static_cast<uintptr_t>(serviceHolderValue));
    void* serviceObject = CSF_AT(void*, serviceHolder, 0x1FC4);
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, serviceObject, 0)[2])(serviceObject, nullptr, slotIndex);
}

HOOK(0x004A4A40, S_00492b20_DispatchToResolvedTarget, "auto");
