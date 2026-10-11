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

// Base-class update forwarder (tail-jumps to a virtual/function pointer).
CSF_FUNC(void, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, int a2));
// Rotation-state gate; returns a bool in al.
CSF_FUNC(bool, __fastcall, TCharTask_CheckRotationGate, 0x0048CBE0, (void* self, void* edx, char a2));
// Finishes a rotation on the TFncGirar part of this task.
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));

// Updates the base task state, then, if the rotation gate reports ready,
// signals rotation completion on this task.
static void __fastcall TCharTask_UpdateAndFinishRotation(void* self, void* /*edx*/, int a2)
{
    TCharTask_BaseUpdate(self, nullptr, a2);

    // Walk this->[+0x08]->[+0x14]->[+0x204] to find the gate's object.
    void* ownerPtr = CSF_AT(void*, self, 0x08);
    void* middlePtr = CSF_AT(void*, ownerPtr, 0x14);
    void* gateObject = CSF_AT(void*, middlePtr, 0x204);

    if (TCharTask_CheckRotationGate(gateObject, middlePtr, 1))
    {
        TFncGirar_OnRotationComplete(self, nullptr, 0);
    }
}

HOOK(0x4D6F90, TCharTask_UpdateAndFinishRotation, "auto");
