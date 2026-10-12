#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetPathStatus, 0x004AEB90, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiDistracted_ResetTaskState, 0x004B9860, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));

static char __stdcall AiDistracted_DispatchPathOrExpiredDistraction(void* task)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    if (GetPathStatus(linkedObject, nullptr) > 0)
    {
        Dispatch_NoLinkedTarget(task, nullptr, 1);
        return 1;
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    if (CSF_AT(std::uint8_t, linkedObject, 0x690) &&
        CSF_AT(float, linkedObject, 0x694) > CSF_AT(float, linkedObject, 0x698))
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
        linkedObject = linkedValue
            ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
            : nullptr;

        AiDistracted_ResetTaskState(linkedObject, nullptr);
        Dispatch_NoLinkedTarget(task, nullptr, 0);
        return 1;
    }

    return 0;
}

HOOK(0x004B9F60, AiDistracted_DispatchPathOrExpiredDistraction, "auto");
