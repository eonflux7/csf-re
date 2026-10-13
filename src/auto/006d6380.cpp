#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

namespace Concurrency {
struct IScheduler;
}

// Returns the scheduler pointer stored at +0xF0 (the scheduler handle of the SchedulerBase).
static Concurrency::IScheduler* __fastcall C_00861120_GetIScheduler(C_00861120* self, void* /*edx*/) {
    return reinterpret_cast<Concurrency::IScheduler*>(self->field_00f0);
}

HOOK(0x006D6380, C_00861120_GetIScheduler, "auto");
