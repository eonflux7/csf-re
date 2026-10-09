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

// Local handler on this object; returns nonzero when the event was consumed.
CSF_FUNC(char, __fastcall, CCharActionFsm_TryHandleLocally, 0x004832A0, (void* self, void* edx));

// Fallback handler at 0x47FC20, reached when the local handler did not consume the event.
CSF_FUNC(char, __fastcall, CCharWeaponFsm_HandleEventFallback, 0x0047FC20, (void* self, void* edx, int a2));

// Returns the local handler's result if it is nonzero; otherwise forwards the event (and its argument) to the fallback.
static char __fastcall CCharActionFsm_ProcessEventOrFallback(CCharActionFsm* self, void* /*edx*/, int a2)
{
    char handled = CCharActionFsm_TryHandleLocally(self, nullptr);
    if (!handled)
        return CCharWeaponFsm_HandleEventFallback(self, nullptr, a2);
    return handled;
}

HOOK(0x00484140, CCharActionFsm_ProcessEventOrFallback, "auto");
