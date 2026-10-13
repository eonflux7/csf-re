#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __fastcall, NetPlayerListDdl_ProcessEntry, 0x0074EEB0, (void* self, void* edx, void* a2, int a3));

static int __stdcall NetPlayerListDdl_ForwardToProcessEntry(void* self, int value, void* entry)
{
    // The last push is the helper's first stack argument.
    // Observe EAX because the original forwarder leaves it unchanged.
    return reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
        NetPlayerListDdl_ProcessEntry)(self, nullptr, entry, value);
}

HOOK(0x0074F280, NetPlayerListDdl_ForwardToProcessEntry, "auto");
