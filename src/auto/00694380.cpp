#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, S_00693740_ReleaseResources, 0x00693BD0, (S_00693740* self, void* edx));

static int __fastcall S_00693740_ReleaseBuffers(S_00693740* self, void* /*edx*/)
{
    // 00694381 mov esi,ecx; 00694383 call 00693BD0: same object,
    // no stack arguments. This is not an embedded member or a virtual call.
    S_00693740_ReleaseResources(self, nullptr);

    if (self->field_002c)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_002c));
        self->field_002c = 0;
    }

    int result = 0;
    // 0069439F loads EAX from +1C, overriding the first free's result.
    // The null path returns zero; otherwise EAX is the second free's result.
    if (self->field_001c)
    {
        result = CoreMemory_Free(reinterpret_cast<void*>(self->field_001c));
        self->field_001c = 0;
    }
    return result;
}

HOOK(0x00694380, S_00693740_ReleaseBuffers, "auto");
