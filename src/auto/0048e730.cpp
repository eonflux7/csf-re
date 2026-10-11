#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ApplyStringToObject, 0x004577D0, (void* self, void* edx, char* a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(unsigned int, __fastcall, SchedulerBase_GetNumberOfBoundContexts, 0x004FE490, (void* self, void*));

static int __fastcall C_008342e8_ApplyStringToResolvedObject(
    C_008342e8* self, void* /*edx*/, char* text)
{
    int globalValue = GetGlobalValue_8B0568();
    unsigned int resolvedObject = SchedulerBase_GetNumberOfBoundContexts(
        reinterpret_cast<void*>(globalValue), nullptr);
    int result = ApplyStringToObject(
        reinterpret_cast<void*>(resolvedObject), nullptr, text,
        reinterpret_cast<int>(self));
    self->field_0090 = result;
    return result;
}

HOOK(0x0048E730, C_008342e8_ApplyStringToResolvedObject, "auto");
