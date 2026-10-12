#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t*, __fastcall, AiDistracted_ProcessObjectPair, 0x00449440,
               (void* self, void* edx, uint32_t* a2));

static uint32_t* __fastcall S_00448a80_ProcessSceneObject(
    S_00448a80* self, void* /*edx*/, S_00448a80* sceneObject)
{
    const uint32_t first = sceneObject->field_0004;
    const uint32_t second = sceneObject->field_0008;

    alignas(8) uint8_t pairBytes[0x18];
    auto* pair = reinterpret_cast<uint32_t*>(pairBytes);
    pair[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    pair[3] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    pair[2] = second;
    pair[1] = first;
    pair[4] = -1;
    pair[5] = -1;

    return AiDistracted_ProcessObjectPair(self, nullptr, pair);
}

HOOK(0x00449510, S_00448a80_ProcessSceneObject, "auto");
