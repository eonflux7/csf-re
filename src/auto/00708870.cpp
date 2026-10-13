#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: SceneLoader::GetWorldObjectCollection(a2), __fastcall (ecx = a2)
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));

// Original: RpWorldForAllWorldSectors(collection, callback, userData), __cdecl
CSF_FUNC(int, __cdecl, RpWorldForAllWorldSectors, 0x0058CFF0, (int a1, int (__cdecl* a2)(int*, int), int a3));

// Callback passed to RpWorldForAllWorldSectors (address 0x7087A0), used as a value
static int (__cdecl* WorldSectorCallback())(int*, int) {
    return reinterpret_cast<int (__cdecl*)(int*, int)>(CSF_ADDR_FN(0x007087A0));
}

static bool __fastcall C_00863ac0_ConsumeWorldSectorFlag(C_00863ac0* self, void* /*edx*/, C_00863ac0* a2) {
    // If the flag byte at a2+0x27 (high byte of field_0024) is set, nothing to do.
    if (CSF_AT(uint8_t, a2, 0x27) != 0) {
        return true;
    }

    int worldObjectCollection = SceneLoader_GetWorldObjectCollection(a2, nullptr);
    uint8_t* collection = reinterpret_cast<uint8_t*>(worldObjectCollection);
    uint32_t collectionFlags = CSF_AT(uint32_t, collection, 8);

    if ((collectionFlags & 8) == 0) {
        return false;
    }

    RpWorldForAllWorldSectors(worldObjectCollection, WorldSectorCallback(),
                              reinterpret_cast<int>(self));

    CSF_AT(uint32_t, collection, 8) = collectionFlags & 0xFFFFFFF7u;
    return true;
}

HOOK(0x708870, C_00863ac0_ConsumeWorldSectorFlag, "auto");
