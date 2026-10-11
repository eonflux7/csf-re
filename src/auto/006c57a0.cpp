#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, GetObjectFlagsPointer, 0x006BF210, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* , int value));

// Picks a scene region index from the object's flags and the virtual query on a2,
// then stores it into the item-count object's C_008670f0 via SetField0C.
static int __fastcall C_0085e7b8_ApplySkinRegionSelection(C_0085e7b8** self, void* /*edx*/, C_0086754c* a2)
{
    int currentItemCount = GetCurrentItemCount(a2, nullptr);

    // Virtual slot 0x14 / 4 = 5 on a2 (returns a byte flag).
    char useAlternateRegion =
        ((char (__fastcall*)(void*, void*))CSF_AT(void**, a2, 0)[5])(a2, nullptr);

    int regionIndex;
    if ((*GetObjectFlagsPointer(a2, nullptr) & 4) != 0)
        regionIndex = 5;
    else if (useAlternateRegion)
        regionIndex = 7;
    else
        regionIndex = 0;

    int sceneRegion = GetSceneRegionAt(self[1], nullptr, regionIndex);

    void* countObj = reinterpret_cast<void*>(static_cast<uintptr_t>(currentItemCount));
    // Virtual slot 0x1c / 4 = 7 on the item-count object.
    C_008670f0* itemObj =
        ((C_008670f0* (__fastcall*)(void*, void*))CSF_AT(void**, countObj, 0)[7])(countObj, nullptr);

    return C_008670f0_SetField0C(itemObj, nullptr, sceneRegion);
}

HOOK(0x006C57A0, C_0085e7b8_ApplySkinRegionSelection, "auto");
