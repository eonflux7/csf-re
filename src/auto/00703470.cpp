#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Primary step: returns nonzero when it produced a result that should be returned directly.
CSF_FUNC(int, __fastcall, Emitter_RunPrimaryStep, 0x007030D0, (void* self, void* edx, int a2, unsigned int a3));
// Post-step notifier: receives the primary result as a flag (1 when the primary returned 0).
CSF_FUNC(int, __fastcall, Emitter_NotifyStepResult, 0x00703280, (void* self, void* edx, int a2, char a3));
// Fallback step, used when the primary step returned 0.
CSF_FUNC(int, __stdcall, Emitter_RunFallbackStep, 0x00702F40, (int a1, unsigned int a2));

// The original references this vtable at the primary-step call site; no store happens here.
[[maybe_unused]] static const auto kEmitterStepVtbl = CSF_VTBL(0x007030D0);

static int __fastcall ProcessEmitterStepWithFallback(void* self, void* edx, int a2, unsigned int a3)
{
    int primaryResult = Emitter_RunPrimaryStep(self, nullptr, a2, a3);
    Emitter_NotifyStepResult(self, nullptr, a2, static_cast<char>(primaryResult == 0));
    if (primaryResult)
        return primaryResult;
    return Emitter_RunFallbackStep(a2, a3);
}

HOOK(0x00703470, ProcessEmitterStepWithFallback, "auto");
