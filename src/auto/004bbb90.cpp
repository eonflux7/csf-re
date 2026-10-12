#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, AiDistracted_CheckReactionDelay, 0x00448170, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, AiDistracted_ResetReactionTimer, 0x00448190, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiDistracted_IsReactionReady, 0x00448240, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, AiDistracted_CheckActorEligibility, 0x004AA480, (void* self, void* edx));
CSF_FUNC(bool, __stdcall, AiDistracted_CheckLinkedActor, 0x004ABEC0, (int a1));
CSF_FUNC(bool, __stdcall, AiDistracted_CheckDistractionTarget, 0x004B67B0, (int a1));
CSF_FUNC(char, __fastcall, AiDistracted_CheckDistractionConditions, 0x004B68D0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AiDistracted_TryStartDistraction, 0x004B6A80, (void* self, void* edx, float arg0));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

static void __fastcall S_004b1050_UpdateDistraction(S_004b1050* self, void* /*edx*/, int actorHandle)
{
    if (!GetSceneMenuContext(g_pCurrentScene, nullptr))
        return;

    int linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(actorHandle), nullptr);
    void* actor = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    void* actorData = CSF_AT(void*, actor, 0x2C);
    if (CSF_AT(uint32_t, actorData, 0x78) != 1)
        return;

    self->field_0020 = 0;
    if (static_cast<uint8_t>(AiDistracted_CheckReactionDelay(
            CSF_AT(void*, g_pCurrentScene, 0xC4), nullptr, 5.0f)) == 0)
        return;

    linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(actorHandle), nullptr);
    actor = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;
    if (CSF_AT(int32_t, actor, 0x630) < 0)
        return;

    linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(actorHandle), nullptr);
    actor = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    void* menuContext = reinterpret_cast<void*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (CSF_AT(uint32_t, actor, 0x62C) != CSF_AT(uint32_t, menuContext, 0x20))
        return;

    self->field_0020 = 1;
    linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(actorHandle), nullptr);
    actor = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    if (!AiDistracted_CheckActorEligibility(actor, nullptr))
    {
        self->field_0020 = 2;
        return;
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(actorHandle), nullptr);
    actor = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;
    actorData = CSF_AT(void*, actor, 0x2C);
    void* actorInterface = CSF_AT(void*, actorData, 0x44);
    if (!((bool(__fastcall*)(void*, void*))
          CSF_AT(void**, actorInterface, 0)[0x90 / 4])(actorInterface, nullptr))
    {
        self->field_0020 = 2;
        return;
    }

    if (!AiDistracted_IsReactionReady(
            CSF_AT(void*, g_pCurrentScene, 0xC4), nullptr))
    {
        self->field_0020 = 3;
        return;
    }

    self->field_0008 = 1084227584;
    if (!AiDistracted_CheckLinkedActor(actorHandle) ||
        !AiDistracted_CheckDistractionTarget(actorHandle))
    {
        self->field_0020 = 4;
        return;
    }

    if (!AiDistracted_CheckDistractionConditions(self, nullptr, actorHandle))
        return;

    float distractionArgument;
    std::memcpy(&distractionArgument, &actorHandle, sizeof(distractionArgument));
    if (!AiDistracted_TryStartDistraction(self, nullptr, distractionArgument))
    {
        self->field_0020 = 7;
        return;
    }

    self->field_0004 = 3;
    AiDistracted_ResetReactionTimer(
        CSF_AT(void*, g_pCurrentScene, 0xC4), nullptr);
    self->field_0020 = 8;
}

HOOK(0x004BBB90, S_004b1050_UpdateDistraction, "auto");
