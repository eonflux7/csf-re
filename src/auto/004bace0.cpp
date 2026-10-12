#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(void, __fastcall, AiDistracted_UpdateLinkedObject, 0x004B9860, (void* self, void* edx));
static CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
static CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));

static int __stdcall AiDistracted_UpdateLinkedTargetAndDispatch(
    int context, int /*unused2*/, int /*unused3*/)
{
    void* self = reinterpret_cast<void*>(context);
    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(self, nullptr);

    void* linkedObject = linkedObjectValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedObjectValue) - 4)
        : nullptr;
    AiDistracted_UpdateLinkedObject(linkedObject, nullptr);
    return Dispatch_NoLinkedTarget(self, nullptr, 4);
}

HOOK(0x004BACE0, AiDistracted_UpdateLinkedTargetAndDispatch, "auto");
