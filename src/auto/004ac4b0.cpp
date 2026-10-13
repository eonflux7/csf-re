#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Callees (bind as given by the packet)
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

// Resets the linked cover-attack task state when the linked object's sub-object
// has a nonzero flag at +0x1C4. Returns 1 if a reset was performed, else 0.
static char __stdcall AiCoverTasks_ResetLinkedTaskIfActive(int a1)
{
    char resetPerformed = 0;
    void* self = reinterpret_cast<void*>(a1);

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int ownerBase = linkedValue ? linkedValue - 4 : 0;

    uint8_t* owner = reinterpret_cast<uint8_t*>(ownerBase);
    int ownerSub = CSF_AT(int, owner, 0x2c);
    int subFlag = CSF_AT(int, reinterpret_cast<uint8_t*>(ownerSub), 0x1c4);

    if (subFlag != 0) {
        resetPerformed = 1;
        int taskBase = GetLinkedObjectField10(self, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, taskBase + 0xc);
    }

    return resetPerformed;
}

HOOK(0x004AC4B0, AiCoverTasks_ResetLinkedTaskIfActive, "auto");
