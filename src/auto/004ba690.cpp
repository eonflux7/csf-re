#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, AiDistracted_HandleExpiredDistraction, 0x004B9860, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));

static char __stdcall AiDistracted_CheckExpiredDistraction(int objectValue)
{
    void* object = reinterpret_cast<void*>(objectValue);
    int linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4)
        : nullptr;

    if (!CSF_AT(std::uint8_t, linkedObject, 0x690))
        return 0;

    if (!(CSF_AT(float, linkedObject, 0x694) >
          CSF_AT(float, linkedObject, 0x698)))
        return 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4)
        : nullptr;

    AiDistracted_HandleExpiredDistraction(linkedObject, nullptr);
    Dispatch_NoLinkedTarget(object, nullptr, 4);
    return 1;
}

HOOK(0x004BA690, AiDistracted_CheckExpiredDistraction, "auto");
