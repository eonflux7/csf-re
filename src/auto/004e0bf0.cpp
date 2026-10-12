#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, ActorTargetCheckPoint, 0x004E0A70, (void* self, void* edx, float* a2));

static bool __fastcall ActorTargetMatchesPoint(S_004e0a70* self, void* /*edx*/, float* a2)
{
    float pointXZ[2];
    pointXZ[0] = a2[0];
    pointXZ[1] = a2[2];
    return ActorTargetCheckPoint(self, nullptr, pointXZ);
}

HOOK(0x004E0BF0, ActorTargetMatchesPoint, "auto");
