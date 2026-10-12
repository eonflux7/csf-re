#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(bool, __fastcall, ForwardToDelegate, 0x0048E710, (void* self, void* edx, void* character));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall CPlayerCharacter_EvaluateFinalTarget(
    CPlayerCharacter* self, void* /*edx*/)
{
    const uint32_t finalTargetState = self->field_0298;
    if (finalTargetState != 0)
        return static_cast<char>(finalTargetState);

    const int32_t handleState = static_cast<int32_t>(self->field_0248);
    if (handleState < 0)
        return static_cast<char>(handleState);

    const int globalState = GetGlobalState_50D190();
    auto* state = reinterpret_cast<C_00833908*>(globalState);
    if (*reinterpret_cast<uint8_t*>(&state->field_00d0) != 0)
        return static_cast<char>(globalState);

    const int resolved = CHandleTable_Resolve(
        g_pCurrentScene, nullptr, reinterpret_cast<int>(&self->field_0240));
    if (resolved == 0)
        return static_cast<char>(resolved);

    auto* target = reinterpret_cast<C_00833908*>(resolved);
    if (target->field_0050 != 32)
        return static_cast<char>(resolved);

    const bool accepted = ForwardToDelegate(target, nullptr, self);
    if (!accepted)
        return static_cast<char>(accepted);

    auto* targetObject = reinterpret_cast<C_00833908*>(target->field_0034);
    if (!targetObject)
        return static_cast<char>(accepted);

    if ((*reinterpret_cast<uint8_t*>(&targetObject->field_0040) & 1) == 0)
        return static_cast<char>(accepted);

    self->field_0298 = 16;
    self->field_02a0 = CSF_AT(uint32_t, self, 0x244);
    self->field_02a4 = self->field_0248;

    const int descriptionId =
        MenuExtras_ParseDescriptionId(CSF_STR(0x00833DF0, "g006"));
    self->field_02ac = descriptionId;
    return static_cast<char>(descriptionId);
}

HOOK(0x00494700, CPlayerCharacter_EvaluateFinalTarget, "auto");
