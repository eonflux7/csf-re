#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Virtual slots on the object at ctx+4 (byte offsets 0x78, 0x58, 0x7C -> slots 30, 22, 31).
static bool CallSlot30_TryEnterState(void* obj, int* stateValuePtr, int ctx) {
    return ((bool(__fastcall*)(void*, void*, int*, int))CSF_AT(void**, obj, 0)[30])(obj, nullptr, stateValuePtr, ctx);
}

static int CallSlot22_ProbeCandidate(void* obj, int candidateArg, int ctx) {
    return ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, obj, 0)[22])(obj, nullptr, candidateArg, ctx);
}

static bool CallSlot31_FinishSequence(void* obj, int ctx) {
    return ((bool(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[31])(obj, nullptr, ctx);
}

static int __stdcall RunCandidateProbeStateMachine(int firstCandidate, int ctx) {
    void* ctxPtr = reinterpret_cast<void*>(ctx);
    int stateValue = 4;

    // Phase 1: if the state byte is clear, ask the object to try the state; store the inverted refusal.
    bool enteredOk = false;
    if (CSF_AT(uint8_t, ctxPtr, 0) == 0) {
        bool refused = CallSlot30_TryEnterState(CSF_AT(void*, ctxPtr, 4), &stateValue, ctx);
        enteredOk = !refused;
    }
    CSF_AT(uint8_t, ctxPtr, 0) = enteredOk ? 1 : 0;

    // Phase 2: probe up to 4 candidates, stepping through the argument array.
    unsigned int probeIndex = 0;
    int candidateArg = firstCandidate;
    while (true) {
        bool matched = false;
        if (CSF_AT(uint8_t, ctxPtr, 0) == 0) {
            matched = CallSlot22_ProbeCandidate(CSF_AT(void*, ctxPtr, 4), candidateArg, ctx) == -1;
        }
        CSF_AT(uint8_t, ctxPtr, 0) = matched ? 1 : 0;
        if (matched) {
            break;
        }
        ++probeIndex;
        candidateArg += 4;
        if (probeIndex >= 4) {
            if (CallSlot31_FinishSequence(CSF_AT(void*, ctxPtr, 4), ctx)) {
                CSF_AT(uint8_t, ctxPtr, 0) = 0;
                return 0;
            }
            break;
        }
    }

    CSF_AT(uint8_t, ctxPtr, 0) = 1;
    return 1;
}

HOOK(0x4E4C50, RunCandidateProbeStateMachine, "auto");
