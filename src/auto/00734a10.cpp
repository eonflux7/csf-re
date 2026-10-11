#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static void* __fastcall C_00861120_GetSchedulerProxy(C_00861120* self, void* /*edx*/)
{
    return reinterpret_cast<void*>(self->field_00f4);
}

HOOK(0x00734A10, C_00861120_GetSchedulerProxy, "auto");
