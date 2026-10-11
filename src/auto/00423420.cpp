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

CSF_FUNC(uint32_t*, __fastcall, sub_421CA0, 0x00421CA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_4DB1D0, 0x004DB1D0, (void* self, void* edx, int a2));

// Refreshes the owner character's cached position triple (owner+0x218..0x220)
// from the scene object's position vector, pushes that triple to the scene
// manager, and then runs the common post-update steps.
static int __fastcall TFncGirar_RefreshPositionAndNotify(TFncGirar* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    uint32_t ownerHandle = self->field_0008;
    uint8_t* owner = reinterpret_cast<uint8_t*>(ownerHandle);

    // If the scene manager reports the owner as its current entry, tell it to drop it.
    uint8_t* sceneObj = CSF_AT(uint8_t*, owner, 0x14);
    uint8_t* sceneManager = CSF_AT(uint8_t*, sceneObj, 0x48);
    uint32_t currentEntry = ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, sceneManager, 0)[16])(sceneManager, nullptr);
    if (currentEntry == ownerHandle)
    {
        uint8_t* managerNow = CSF_AT(uint8_t*, CSF_AT(uint8_t*, owner, 0x14), 0x48);
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, managerNow, 0)[15])(managerNow, nullptr, ownerHandle);
    }

    // Snapshot the scene object's position vector into the owner's cache at +0x218.
    uint8_t* positionSource = CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(self->field_0008), 0x14);
    auto* positionVector = reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(GetObjectPositionVector(positionSource, nullptr)));

    uint8_t* ownerForCache = reinterpret_cast<uint8_t*>(self->field_0008);
    CSF_AT(uint32_t, ownerForCache, 0x218) = positionVector[0];
    CSF_AT(uint32_t, ownerForCache, 0x21C) = positionVector[1];
    CSF_AT(uint32_t, ownerForCache, 0x220) = positionVector[2];

    // Hand the cached triple to the scene-side object at owner->scene+0x4C.
    uint8_t* ownerNow = reinterpret_cast<uint8_t*>(self->field_0008);
    uint32_t cachedTriple[3];
    cachedTriple[0] = CSF_AT(uint32_t, ownerNow, 0x218);
    cachedTriple[1] = CSF_AT(uint32_t, ownerNow, 0x21C);
    cachedTriple[2] = CSF_AT(uint32_t, ownerNow, 0x220);
    uint8_t* notifyTarget = CSF_AT(uint8_t*, CSF_AT(uint8_t*, ownerNow, 0x14), 0x4C);
    ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, notifyTarget, 0)[20])(notifyTarget, nullptr, cachedTriple);

    sub_421CA0(ownerNow, nullptr);
    sub_4DB1D0(self, nullptr, 0);

    return 0;
}

HOOK(0x00423420, TFncGirar_RefreshPositionAndNotify, "auto");
