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

// Dispatch_NoLinkedTarget: __fastcall(this, edx, int) at 0x4DBCB0.
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));

// Original: loads the first stack argument into ecx (this), pushes 4 and
// dispatches to Dispatch_NoLinkedTarget. The other two arguments are ignored.
// Stdcall with 3 stack args, so the callee pops 12 bytes.
static int __stdcall AiMoveTasks_DispatchNoLinkedTargetKind4(int a1, int /*a2*/, int /*a3*/)
{
    return Dispatch_NoLinkedTarget(reinterpret_cast<void*>(a1), nullptr, 4);
}

HOOK(0x004AB780, AiMoveTasks_DispatchNoLinkedTargetKind4, "auto");
