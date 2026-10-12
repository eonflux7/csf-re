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

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetRtDictSchema, 0x006D6360, (void* self, void*));
CSF_FUNC(uint32_t*, __cdecl, RpTeamRemovePlayer, 0x007FE950, (uint32_t* a1, int a2));
CSF_FUNC(unsigned int, __fastcall, ApplySceneEntryValue, 0x006B1840, (void* self, void* edx, int a2));

// Removes this water node's player from the RpTeam dictionary and releases any
// pending scene entry value. Returns the low byte of the last value in eax.
static char __fastcall WaterNode_PostUpdate(C_008630e0* self, void* /*edx*/)
{
    char resultByte = 0;

    if (self->field_0011 != 0)
    {
        void* globalIntObject = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
        int renderHandle = RenderHandle_FromObject(globalIntObject, nullptr);

        uint32_t playerId = self->field_0004;
        int rtDictSchema = GetRtDictSchema(
            reinterpret_cast<void*>(static_cast<uintptr_t>(renderHandle)), nullptr);
        RpTeamRemovePlayer(
            reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(rtDictSchema)),
            static_cast<int>(playerId));

        uint32_t pendingValue = self->field_0024;
        resultByte = static_cast<char>(pendingValue);
        if (pendingValue != 0)
        {
            uint32_t valueToApply = self->field_0024;
            void* sceneObject = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
            resultByte = static_cast<char>(
                ApplySceneEntryValue(sceneObject, nullptr, static_cast<int>(valueToApply)));
            self->field_0024 = 0;
        }

        self->field_0011 = 0;
    }

    return resultByte;
}

HOOK(0x006FF710, WaterNode_PostUpdate, "auto");
