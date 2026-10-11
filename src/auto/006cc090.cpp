// Reimplementation of Concurrency::details::SchedulerBase::GetAnonymousScheduleGroup (0x006CC090).
// The original is a trivial __thiscall getter that returns the anonymous schedule group
// pointer stored at offset +0xB0 of the object.

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

namespace {

struct ScheduleGroupBase;

// Returns the anonymous schedule group stored on the scheduler object.
static ScheduleGroupBase* __fastcall C_00860bb8_GetAnonymousScheduleGroup(C_00860bb8* self, void* /*edx*/) {
    return reinterpret_cast<ScheduleGroupBase*>(self->field_00b0);
}

}  // namespace

HOOK(0x006CC090, C_00860bb8_GetAnonymousScheduleGroup, "auto");
