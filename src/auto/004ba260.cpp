#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetPathStatus, 0x004AEB90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiDistracted_PrepareAlertTransition, 0x004B3240, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiDistracted_UpdateExpiredDistraction, 0x004B9860, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void* AiDistracted_GetLinkedActor(void* context)
{
    const int linked = GetLinkedObjectValue_4DB5C0(context, nullptr);
    if (!linked)
        return nullptr;

    return reinterpret_cast<void*>(static_cast<std::uint32_t>(linked) - 4);
}

static char __stdcall AiDistracted_EvaluateAlertTransition(void* context)
{
    void* actor = AiDistracted_GetLinkedActor(context);
    const int pathStatus = GetPathStatus(actor, nullptr);
    const std::int32_t adjustedStatus =
        static_cast<std::int32_t>(static_cast<std::uint32_t>(pathStatus) - 2);

    if (pathStatus != -1 && adjustedStatus > 0)
    {
        Dispatch_NoLinkedTarget(context, nullptr, 1);
        return 1;
    }

    actor = AiDistracted_GetLinkedActor(context);
    if (CSF_AT(std::uint8_t, actor, 0x690) &&
        CSF_AT(float, actor, 0x694) > CSF_AT(float, actor, 0x698))
    {
        Dispatch_NoLinkedTarget(context, nullptr, 5);
        return 1;
    }

    actor = AiDistracted_GetLinkedActor(context);
    if (!CSF_AT(std::uint8_t, actor, 0x690) ||
        !(CSF_AT(float, actor, 0x694) < g_fZero))
    {
        return 0;
    }

    actor = AiDistracted_GetLinkedActor(context);
    AiDistracted_UpdateExpiredDistraction(actor, nullptr);

    actor = AiDistracted_GetLinkedActor(context);
    if (CSF_AT(std::uint32_t, actor, 0x688) >= 2u)
    {
        Dispatch_NoLinkedTarget(context, nullptr, 0);
        return 1;
    }

    actor = AiDistracted_GetLinkedActor(context);
    if (CSF_AT(int, CSF_AT(void*, actor, 0x2C), 0x50) == 1)
    {
        actor = AiDistracted_GetLinkedActor(context);
        AiDistracted_PrepareAlertTransition(actor, nullptr);
    }

    Dispatch_NoLinkedTarget(context, nullptr, 2);
    return 1;
}

HOOK(0x004BA260, AiDistracted_EvaluateAlertTransition, "auto");
