#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ReleaseGatheringResources, 0x006AA760, (void* self, void* edx));

static int __fastcall C_0085fe28_TryFinishAndReset(C_0085fe28* self, void* /*edx*/)
{
    // Virtual call through primary vtable slot 3 (byte offset 0x0C), no arguments.
    auto pollReady = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, self, 0)[3]);
    int result = pollReady(self, nullptr);

    if (static_cast<uint8_t>(result))
    {
        ReleaseGatheringResources(self, nullptr);
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x006A7120, C_0085fe28_TryFinishAndReset, "auto");
