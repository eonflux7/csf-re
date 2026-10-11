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

// Resolves the sub-object at this->field_0008 + 0x224 and tail-calls the action routine on it.
CSF_FUNC(void*, __fastcall, ResolveSubObject, 0x004DB190, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, RunSubObjectAction, 0x004D85D0, (void* self, void* edx));

static int __fastcall S_004223f0_ProcessSubObjectAt224(S_004223f0* self, void* /*edx*/)
{
    int subObjectOffset = static_cast<int>(self->field_0008 + 0x224);
    void* subObject = ResolveSubObject(self, nullptr, subObjectOffset);
    return RunSubObjectAction(subObject, nullptr);
}

HOOK(0x004D9150, S_004223f0_ProcessSubObjectAt224, "auto");
