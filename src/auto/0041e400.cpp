#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x006AEEA0: sub-state readiness test, takes its receiver in ecx, returns a byte-style bool.
CSF_FUNC(int, __fastcall, AiSubStateObject_TestReady, 0x006AEEA0, (void* self, void* edx));

// Returns 1 when the sub-state pointer at +0x14 is null, or when its embedded object at +4 passes the readiness test.
static int __fastcall AiTaskStates_IsSubStateReadyOrAbsent(void* self, void* /*edx*/)
{
    uint32_t subStatePointer = CSF_AT(uint32_t, self, 0x14);
    if (subStatePointer == 0) {
        return 1;
    }

    void* embeddedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(subStatePointer + 4));
    int testResult = AiSubStateObject_TestReady(embeddedObject, nullptr);
    if (static_cast<uint8_t>(testResult) != 0) {
        return 1;
    }
    return 0;
}

HOOK(0x0041E400, AiTaskStates_IsSubStateReadyOrAbsent, "auto");
