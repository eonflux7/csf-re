#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetPathStatus, 0x004AEB90, (void* self, void* edx));
CSF_FUNC(C_00833908*, __stdcall, Estatico_ForwardToLinkedHandleWhenState3, 0x004AFBB0, (int a1));
CSF_FUNC(void, __fastcall, AiDistracted_HandleNegativeTimer, 0x004B9860, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __fastcall TDudoso_UpdateTransition(
    TDudoso* /*self*/, void* /*edx*/, int actorValue)
{
    void* actor = reinterpret_cast<void*>(actorValue);

    int linkedValue = GetLinkedObjectValue_4DB5C0(actor, nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    int pathStatus = GetPathStatus(linkedObject, nullptr);
    if (pathStatus != -1 &&
        static_cast<std::int32_t>(
            static_cast<std::uint32_t>(pathStatus) - 1) > 0)
    {
        Dispatch_NoLinkedTarget(actor, nullptr, 1);
        return 1;
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(actor, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    if (CSF_AT(std::uint8_t, linkedObject, 0x690) &&
        CSF_AT(float, linkedObject, 0x694) >
            CSF_AT(float, linkedObject, 0x698))
    {
        Estatico_ForwardToLinkedHandleWhenState3(actorValue);
        Dispatch_NoLinkedTarget(actor, nullptr, 4);
        return 1;
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(actor, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    if (CSF_AT(std::uint8_t, linkedObject, 0x690) &&
        CSF_AT(float, linkedObject, 0x694) < g_fZero)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(actor, nullptr);
        linkedObject = linkedValue
            ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
            : nullptr;

        AiDistracted_HandleNegativeTimer(linkedObject, nullptr);
        Dispatch_NoLinkedTarget(actor, nullptr, 2);
        return 1;
    }

    return 0;
}

HOOK(0x004BA170, TDudoso_UpdateTransition, "auto");
