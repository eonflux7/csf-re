#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

// Resets the cover-attack task state attached to this object. The reset target is
// either field +0x0C or +0x10 of the linked object, chosen by a flag stored in the
// linked owner's component at +0x1C4.
static bool __stdcall AiCoverTasks_ResetLinkedTaskOnRequest(int a1)
{
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint8_t* ownerBase = linkedObjectValue
        ? reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedObjectValue - 4))
        : nullptr;

    void* ownerComponent = CSF_AT(void*, ownerBase, 0x2C);
    int componentFlag = CSF_AT(int, ownerComponent, 0x1C4);

    if (componentFlag != 0)
    {
        int linkedField10 = GetLinkedObjectField10(self, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField10 + 0xC);
    }
    else
    {
        int linkedField10 = GetLinkedObjectField10(self, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField10 + 0x10);
    }
    return true;
}

HOOK(0x004AC450, AiCoverTasks_ResetLinkedTaskOnRequest, "auto");
