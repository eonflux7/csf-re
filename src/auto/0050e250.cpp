#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Clears the pending handle stored at this+0x100. If one was set, it is released
// through the vtable slot 0x60 of the object at this+0x14, called as (handle, 0),
// and that call's return value becomes the result.
static int __fastcall BddLoader_ReleasePendingHandle(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x100);
    if (result)
    {
        void* owner = CSF_AT(void*, self, 0x14);
        result = ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, owner, 0)[0x60 / 4])(owner, nullptr, result, 0);
        CSF_AT(int, self, 0x100) = 0;
    }
    return result;
}

HOOK(0x0050E250, BddLoader_ReleasePendingHandle, "auto");
